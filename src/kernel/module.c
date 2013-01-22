
#include <elf.h>
#include <rdfs.h>
#include <module.h>
#include <lib/kmem.h>
#include <lib/printk.h>
#include <lib/string.h>

struct module_struct{
    char *name;

    void *base;
    void (*unload)(void);
};


struct ksym_struct{
    char *symbol;
    void *address;

    /* int module; (0 = kernel, module id) */

    struct ksym_struct *next;
};

static struct ksym_struct *ksyms_head = NULL;
static struct ksym_struct *ksyms_tail = NULL;


int export_ksym(char *symbol, void *address)
{
    struct ksym_struct *ksym;

    //printk("exporting symbol '%s' at 0x%x\n",symbol,address);
    
    ksym = kmalloc(sizeof(struct ksym_struct));
    if(ksym == NULL)
        return -1;

    ksym->symbol = symbol;
    ksym->address = address;
    ksym->next = NULL;

    /* add it to the ksyms list */
    ksyms_tail->next = ksym;
    ksyms_tail = ksym;

    return 0;
}

void dump_ksyms(void)
{
    struct ksym_struct *p = ksyms_head;

    printk("ksyms:\n");
    while((p = p->next) != NULL)
        printk(" %s @0x%x\n",p->symbol,p->address);

    return;
}

static int remove_ksym(char *symbol)
{
    struct ksym_struct *ksym, *ksym_prev;
    ksym = ksym_prev = ksyms_head;

    /* check if it is safe to remoexecve symbol */
    //here
    

    while(ksym != NULL){

        if(strcmp(ksym->symbol, symbol) == 0){
            /* found symbol */
            ksym_prev->next = ksym->next;
            kfree(ksym);
            return 0;
        }
        ksym_prev = ksym;
        ksym = ksym->next;
    }
    
    /* did not find symbol */
    return -1;
}

static void *get_ksym(char *symbol)
{
    struct ksym_struct *ksym = ksyms_head; 

    while(ksym != NULL){
        if(strcmp(ksym->symbol, symbol) == 0)
            return ksym->address;
        ksym = ksym->next;
    }

    return NULL;
}


int load_module(char *filename)
{
    unsigned int i, j, size;

    char *data, *base;
    struct Elf32_Ehdr *hdr; /* ELF header */
    struct Elf32_Shdr *shdr; /* section headers */

    struct Elf32_Sym *symtab; /* symbol table */
    struct Elf32_Shdr *symtabsh; /* symbol table section header */
    char *strtab, *shstrtab;  /* string table and section header string table */

    int (*load)(void) = NULL;

    struct rdfs_file *f = rdfs_getfile(filename);
    if(!f){
        printk("could not load module '%s'. file does not exist.\n",filename);
        return -1;
    }
    data = (char *) f->data;
    

    hdr = (struct Elf32_Ehdr *) ((long) data);
    shdr = (struct Elf32_Shdr *) (((long) data) + hdr->e_shoff);
    shstrtab = (char *) (((long) data) + (shdr[hdr->e_shstrndx].sh_offset));
   
    size = 0; 
    /* walk through the symbol table */
    /* find the symbol table and string table */
    for(i=0; i < hdr->e_shnum; i++){

        /* total size we need to allocate */
        if(shdr[i].sh_flags & SHF_ALLOC)
            size += shdr[i].sh_size;

        /* symbol table */
        if(shdr[i].sh_type == SHT_SYMTAB){
            symtabsh = &shdr[i];
            symtab = (struct Elf32_Sym *) (data + shdr[i].sh_offset);
        }

        /* string table */
        else if(shdr[i].sh_type == SHT_STRTAB && !strcmp(".strtab",&shstrtab[shdr[i].sh_name]))
            strtab = (char *) (((long) data) + (shdr[i].sh_offset));
    }

    // make some sanity checks on info from section headers
    // if(strtab == NULL)

    /* allocate space for it */
    base = kmalloc(size);

    //printk("base = 0x%x\n",(long) base);

    /* copy sections into memory */
    j = 0;
    for(i=0; i < hdr->e_shnum; i++){
        if(shdr[i].sh_flags & SHF_ALLOC){
            memcpy((void *) (base + j), (void *) (data + shdr[i].sh_offset), shdr[i].sh_size);
            shdr[i].sh_addr = ((long) base + j);
            //printk("ndx = %i addr = 0x%x\n",i,shdr[i].sh_addr);
            if(shdr[i].sh_type == SHT_NOBITS)
                memset((void *) (base + j), 0, shdr[i].sh_size);
            j += shdr[i].sh_size;
        }
    }

    /* relocations */
    for(i=0; i < hdr->e_shnum; i++){
        if(shdr[i].sh_type == SHT_RELA){
            struct Elf32_Rela *rela = (struct Elf32_Rela *) (data + shdr[i].sh_offset);
            for(j=0; j < (shdr[i].sh_size / sizeof(struct Elf32_Rela)); j++,rela++){
                struct Elf32_Sym *sym = &symtab[ELF32_R_SYM(rela->r_info)];
                char *symbol = &strtab[sym->st_name];

                /* kernel symbol */
                if(ELF32_ST_BIND(sym->st_info) == STB_GLOBAL && (strcmp("_GLOBAL_OFFSET_TABLE_",symbol) != 0)){
                    void *address = get_ksym(symbol);
                    if(address == NULL){
                        printk("failed to link ksym '%s'\n",symbol);
                        return -1;
                    }
                    
                    //printk("linking ksym '%s' (address 0x%x) at 0x%x\n",symbol,(long) address,(long) (base + rela->r_offset));
                    
                    switch(ELF32_R_TYPE(rela->r_info)){
                        /* absolute address */
                        case R_68K_32:
                            *((long *)(base + rela->r_offset)) = (long) address;
                            break;

                        /* pc relative address */
                        case R_68K_PC32:
                            *((long *)(base + rela->r_offset)) = ((long) address - (long)(base + rela->r_offset));
                            break;
                    }

                }

                else{

                    switch(ELF32_R_TYPE(rela->r_info)){  

                        default:
                            /*
                            printk("shndx = %i  sh_addr = 0x%x addend = %i\n",
                                symtab[ELF32_R_SYM(rela->r_info)].st_shndx,
                                shdr[symtab[ELF32_R_SYM(rela->r_info)].st_shndx].sh_addr,
                                rela->r_addend);
                            */
                            //printk("addr = 0x%x\n", ( shdr[symtab[ELF32_R_SYM(rela->r_info)].st_shndx].sh_addr + rela->r_addend));
                            
                            *((long *)(base + rela->r_offset)) =
                                ( shdr[symtab[ELF32_R_SYM(rela->r_info)].st_shndx].sh_addr + rela->r_addend);
                    }
                }


            }
        }
    }

    /* symbols */
    for(i=0; i < (symtabsh->sh_size / sizeof(struct Elf32_Sym)); i++){
        struct Elf32_Sym *sym = &symtab[i];
        if(strcmp("load",&strtab[sym->st_name]) == 0)
            load = (void *) (base + sym->st_value); 
    }

    if(load == NULL){
        printk("failed to load module, function load() not found\n");
        return -1;
    }

    /* call module's load() function */
    int ret = load();
    if(ret != 0){
        printk("module failed to load itself, error code %i\n",ret);
        return -1;
    }

    return 0;
}

int unload_module(char *name)
{
    (void) name;
    remove_ksym(NULL);
    return 0;
}


int module_init(void)
{
    ksyms_head = kmallocz(sizeof(struct ksym_struct));
    if(ksyms_head == NULL)
        return -1;
    ksyms_tail = ksyms_head;


    /* do all of the kernel symbol exports */
    EXPORT_SYMBOL(printk);

    load_module("mod.km");

    return 0;
}
    

