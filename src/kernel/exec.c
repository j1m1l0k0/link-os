/* Copyright (C) 2007 Kyle Lutz <kyle.r.lutz@gmail.com> */


/* Sources of information for this code:
 * ELF Specification -- http://www.x86.org/ftp/manuals/tools/elf.pdf
 * Linkers & Loaders -- http://www.iecc.com/linker/
 */

#include <fs.h>
#include <elf.h>
#include <exec.h>
#include <types.h>
#include <thread.h>
#include <lib/kmem.h>
#include <lib/string.h>
#include <lib/printk.h>
#include <cpu/context.h>
#include <target/target.h>

//#define WITH_AOUT_EXEC

#include <rdfs.h>

static void *elf_load(void *data);


unsigned int exec(char *name, char **argv, char **envp)
{
    int argc = 0;

    /* locate file */
    struct rdfs_file *f = rdfs_getfile(name);
    if(!f){
        printk("could not exec file '%s'. file does not exist.\n",name);
        return 0;
    }
    
    /* load file */
    void *entry_point = elf_load(f->data);
    if(!entry_point){
        printk("could not exec file '%s'. file could not be loaded.\n",name);
        return 0;
    }

    /* setup thread */
    struct thread_struct *t = create_thread();
    t->ctx.pc = (long) entry_point;

    memset(t->stack,0,THREAD_STACK_SIZE);

    if(argv != NULL)
        while(argv[++argc] != NULL); /* Count number of arguments */

    // these are cpu specific and should be moved somewhere else
#ifdef CPU_IS_68k
    t->ctx.sp = (long) t->stack;
    t->ctx.sr = 0x2000;

    /* args for main() */
    t->ctx.d1 = (long) argc;
    t->ctx.d2 = (long) argv;
    t->ctx.d3 = (long) envp;
#endif

#ifdef CPU_IS_PPC
    t->ctx.r1 = (long) t->stack;

    t->ctx.r3 = (long) argc;
    t->ctx.r4 = (long) argv;
    t->ctx.r5 = (long) envp;
#endif

    t->state = THREAD_READY;

    return t->id;
}

/* old code from when we used a.out binarys. */
#ifdef WITH_AOUT_EXEC
void exec(void *data, char **argv, char **envp)
{
    int argc;
    long entry;
    long *stack;
    struct task_struct *task = NULL;
    
    entry = do_aout_reloc(data); /* for now we only support a.out */
    //task = task_create(entry);
    
    /* Push arguments for task's main() onto its stack */
    stack = (long *) task->ctx.sp;
    argc = 0;
    if(argv != NULL){
        while(argv[++argc] != NULL); /* Count number of arguments */ 
    }
    *(stack+0) = (long) argc; /* int argc */
    *(stack+1) = (long) argv; /* char **argv */
    *(stack+2) = (long) envp; /* char **envp */

    return;
}


/*===========================================================================*/
/*  Internal Functions                                                       */
/*===========================================================================*/

/*------------------*
 * a.out Relocation *
 *------------------*/

#define OMAGIC 0407
#define NMAGIC 0410
#define ZMAGIC 0413
#define QMAGIC 0314

/* a.out exec header */
/* ref http://leaf.dragonflybsd.org/cgi/web-man?command=a.out&section=5 */
/*     http://cvs.opensolaris.org/source/xref/on/usr/src/ucbhead/a.out.h */
struct exec {
    u8 a_dynamic :1;
    u8 a_flags   :7;
    u8 a_machine;
    u16 a_magic;
    u32 a_text;
    u32 a_data;
    u32 a_bss;
    u32 a_syms;
    u32 a_entry;
    u32 a_trsize;
    u32 a_drsize;
};

/* a.out relocation header */
struct aout_reloc {
    u32 r_address;
    u32 r_symbolnum :24;
    u8 r_pcrel      :1;
    u8 r_length     :2;
    u8 r_extern     :1;
    u8 r_baserel    :1;
    u8 r_jmptable   :1;
    u8 r_relative   :1;
    u8 r_copy       :1;
};

static long do_aout_reloc(void *data)
{
    unsigned int i;
    long *lptr;
    struct exec *hdr = (struct exec *) data;
    long text_base = (long) data + sizeof(struct exec);
    long data_base = text_base + hdr->a_text;
    long entry_point = text_base + hdr->a_entry;
    struct aout_reloc *rel = (struct aout_reloc *) (data_base + hdr->a_data);

    /* do the relocations (text & data) */  
    for(i=0; i < hdr->a_trsize/sizeof(struct aout_reloc); i++,rel++){
        /*printk("text relocate at 0x%x sn=%i pcrel=%i len=%i extern=%i baserel=%i jmp=%i rel=%i copy=%i\n",
                rel->r_address,rel->r_symbolnum,rel->r_pcrel,rel->r_length,rel->r_extern,rel->r_baserel,
                rel->r_jmptable,rel->r_relative,rel->r_copy);*/
        /* if not a PC relative jump */
        if(!rel->r_pcrel){
            if(rel->r_length == 2){ /* 32 bit address */
                lptr = (long *) (text_base + rel->r_address);
                *lptr += text_base;
            }
        }
    }
    for(i=0; i < hdr->a_drsize/sizeof(struct aout_reloc); i++,rel++){
        /*printk("data relocate at 0x%x sn=%i pcrel=%i len=%i extern=%i baserel=%i jmp=%i rel=%i copy=%i\n",
                rel->r_address,rel->r_symbolnum,rel->r_pcrel,rel->r_length,rel->r_extern,rel->r_baserel,
                rel->r_jmptable,rel->r_relative,rel->r_copy);*/
        if(!rel->r_pcrel){
            if(rel->r_length == 2){ /* 32 bit address */
                lptr = (long *) (data_base + rel->r_address);
                *lptr += data_base;
            }
        }
    }
    return entry_point;
}
#endif /* WITH_AOUT_EXEC */



/* calculate the amount of memory (in bytes) that needs to be 
 * allocated for an ELF binary.
 *
 * @phdr = pointer to first program header
 * @phnum = number of phdrs
 */
static unsigned long elf_calc_size(Elf32_Phdr *phdr, unsigned int phnum)
{
    unsigned int i;
    unsigned long total = 0;
    for(i=0; i < phnum; i++,phdr++)
        if(phdr->p_type == PT_LOAD && ((phdr->p_vaddr + phdr->p_memsz) > total))
            total = (phdr->p_vaddr + phdr->p_memsz);
    return total;
}


/* exec elf binarys */
static void *elf_load(void *data)
{
    char *base; /* program base */
    struct Elf32_Ehdr *hdr; /* ELF header */
    struct Elf32_Shdr *shdr; /* section headers */
    struct Elf32_Phdr *phdr; /* program headers */
    struct Elf32_Sym *dsymtab; /* dynamic symbol table */
    long entry_point;
    
    unsigned int i,j; /* tmp's */

    hdr = (struct Elf32_Ehdr *) ((long) data);
    shdr = (struct Elf32_Shdr *) (((long) data) + hdr->e_shoff);
    phdr = (struct Elf32_Phdr *) (((long) data) + hdr->e_phoff);
    dsymtab = NULL;
    
    /* Check for proper ident in header */
    if(hdr->e_ident[0] != 0x7f || hdr->e_ident[1] != 'E' ||
       hdr->e_ident[2] != 'L' || hdr->e_ident[3] != 'F'){
           printk("invalid elf header at 0x%x\n",hdr);
           return NULL;
    }

    //assert hdr->e_machine == 4 for 68k
    
    base = kmalloc(elf_calc_size(phdr,hdr->e_phnum));

    /* load sections into memory */
    for(i=0; i < hdr->e_phnum; i++, phdr++){
        if(phdr->p_type == PT_LOAD){
            //printk("type: %i offs: 0x%x vaddr: 0x%x memsz: %i\n",
            //    phdr->p_type,phdr->p_offset,phdr->p_vaddr,phdr->p_memsz);
            //printk("0x%x -> 0x%x %i\n",
            //        data + phdr->p_offset, base + phdr->p_vaddr, phdr->p_memsz);
            memcpy(base + phdr->p_vaddr, data + phdr->p_offset, phdr->p_memsz);
        }
    }

    /* find the dynamic symbol table */
    for(i=0; i < hdr->e_shnum; i++)
        if(shdr[i].sh_type == SHT_DYNSYM)
            dsymtab = (struct Elf32_Sym *) (base + shdr[i].sh_offset);

    /* do the relocations */
    for(i=0; i < hdr->e_shnum; i++,shdr++){
        if(shdr->sh_type == SHT_RELA){
            struct Elf32_Rela *rela = (struct Elf32_Rela *) (data + shdr->sh_offset);
            for(j=0; j < (shdr->sh_size / sizeof(struct Elf32_Rela)); j++,rela++){

                /* Determine the relocation type and perform the reloaction */
                switch(ELF32_R_TYPE(rela->r_info)){

                #ifdef ELF_MACHINE_68K
                    case R_68K_GLOB_DAT:
                        *((long *)(base + rela->r_offset)) = 
                            ((long) base + dsymtab[ELF32_R_SYM(rela->r_info)].st_value);
                        break;
                    case R_68K_RELATIVE:
                        *((long *)(base + rela->r_offset)) += (long) base;
                        break;
                #endif /* ELF_MACHINE_68K */

                    default:
                        printk("unknow relocation type %i\n",ELF32_R_TYPE(rela->r_info));
                }
            }
        }
    }

    entry_point = ((long) base + hdr->e_entry);

    return (void *) entry_point;
}

int elf_verify(void *data)
{
    (void) data;
    return 1;
}

static inline unsigned long elf_hash(const char *name){
    unsigned long h = 0, g;
    while(*name){
	    h = (h << 4) + *name++;
        if((g = h & 0xf0000000))
            h ^= g >> 24;
        h &= ~g;
    }
    return h;
}

