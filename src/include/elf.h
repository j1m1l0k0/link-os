#ifndef _elf_h
#define _elf_h

#include <types.h>
#include <target/target.h>

/* ELF header */
typedef struct Elf32_Ehdr{ 
    unsigned char e_ident[16];
    u16 e_type;
    u16 e_machine;
    u32 e_version;
    u32 e_entry;
    u32 e_phoff;
    u32 e_shoff;
    u32 e_flags;
    u16 e_ehsize;
    u16 e_phentsize;
    u16 e_phnum;
    u16 e_shentsize;
    u16 e_shnum;
    u16 e_shstrndx;
} Elf32_Ehdr;

/* ELF Section Header */
typedef struct Elf32_Shdr{
    u32 sh_name;
    u32 sh_type;
    u32 sh_flags;
    u32 sh_addr;
    u32 sh_offset;
    u32 sh_size;
    u32 sh_link;
    u32 sh_info;
    u32 sh_addralign;
    u32 sh_entsize;
} Elf32_Shdr;

/* ELF Symbol Table Entry */
typedef struct Elf32_Sym{
    u32 st_name;
    u32 st_value;
    u32 st_size;
    u8  st_info;
    u8  st_other;
    u16 st_shndx;
} Elf32_Sym;

/* ELF Relocation */
typedef struct Elf32_Rel{
    u32 r_offset;
    u32 r_info;
} Elf32_Rel;

typedef struct Elf32_Rela{
    u32 r_offset;
    u32 r_info;
    s32 r_addend;
} Elf32_Rela;

/* ELF Program Header */
typedef struct Elf32_Phdr{
    u32 p_type;
    u32 p_offset;
    u32 p_vaddr;
    u32 p_paddr;
    u32 p_filesz;
    u32 p_memsz;
    u32 p_flags;
    u32 p_align;
} Elf32_Phdr;

/* e_type */
#define ET_NONE   0x0000 /* no file type */
#define ET_REL    0x0001 /* relocatable file */
#define ET_EXEC   0x0002 /* executable file */
#define ET_DYN    0x0003 /* shared object file */
#define ET_CORE   0x0004 /* core file */
#define ET_LOPROC 0xff00 /* processor-specific */
#define ET_HIPROC 0xffff /* processor-specific */

/* e_machine */
#define EM_NONE  0 /* no machine */
#define EM_68K   4 /* m68k */

/* e_version */
#define EV_NONE    0 /* invalid version */
#define EV_CURRENT 1 /* current version */

/* sh_type */
#define SHT_NULL     0
#define SHT_PROGBITS 1
#define SHT_SYMTAB   2
#define SHT_STRTAB   3
#define SHT_RELA     4
#define SHT_HASH     5
#define SHT_DYNAMIC  6
#define SHT_NOTE     7
#define SHT_NOBITS   8
#define SHT_REL      9
#define SHT_SHLIB    10
#define SHT_DYNSYM   11
#define SHT_LOPROC   0x70000000
#define SHT_HIPROC   0x7fffffff
#define SHT_LOUSER   0x80000000
#define SHT_HIUSER   0xffffffff

/* sh_flags */
#define SHF_WRITE     0x1
#define SHF_ALLOC     0x2
#define SHF_EXECINSTR 0x3
#define SHF_MASKPROC  0xf0000000

/* symbol table */
#define ELF32_ST_BIND(i) ((i)>>4)
#define ELF32_ST_TYPE(i) ((i)&0xf)
#define ELF32_ST_INFO(b,t) (((b)<<4)+((t)&0xf))

/* ELF32_ST_BIND */
#define STB_LOCAL  0
#define STB_GLOBAL 1
#define STB_WEAK   2
#define STB_LOPROC 13
#define STB_HIPROC 15

/* ELF32_ST_TYPE */
#define STT_NOTYPE  0
#define STT_OBJECT  1
#define STT_FUNC    2
#define STT_SECTION 3
#define STT_FILE    4
#define STT_LOPROC  13
#define STT_HIPROC  15

/* relocation entries */
#define ELF32_R_SYM(i)    ((i)>>8)
#define ELF32_R_TYPE(i)   ((unsigned char)(i))
#define ELF32_R_INFO(s,t) (((s)<<8)+(unsigned char)(t))

/* 68k relocation types */
#define R_68K_NONE      0
#define R_68K_32        1
#define R_68K_16        2
#define R_68K_8         3
#define R_68K_PC32      4
#define R_68K_PC16      5
#define R_68K_PC8       6
#define R_68K_GOT32     7
#define R_68K_GOT16     8
#define R_68K_GOT8      9
#define R_68K_GOT32O    10
#define R_68K_GOT16O    11
#define R_68K_GOT8O     12
#define R_68K_PLT32     13
#define R_68K_PLT16     14
#define R_68K_PLT8      15
#define R_68K_PLT32O    16
#define R_68K_PLT16O    17
#define R_68K_PLT8O     18
#define R_68K_COPY      19
#define R_68K_GLOB_DAT  20
#define R_68K_JMP_SLOT  21
#define R_68K_RELATIVE  22

/* p_type */
#define PT_NULL    0
#define PT_LOAD    1
#define PT_DYNAMIC 2
#define PT_INTERP  3
#define PT_NOTE    4
#define PT_SHLIB   5
#define PT_PHDR    6
#define PT_LOPROC  0x70000000
#define PT_HIPROC  0x7fffffff

/* Determine the elf machine for this target */

#ifdef CPU_IS_68k
#define ELF_MACHINE_68K
#endif

#ifdef CPU_IS_XPC860
#define ELF_MACHINE_PPC
#endif

#endif /* _elf_h */
