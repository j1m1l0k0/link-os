/* Copyright (C) 2006 Kyle Lutz <kyle.r.lutz@gmail.com> */
/* Link Script for cisco 1600 series router, differs from c1000.ld 
 * with its entry point */

ENTRY(boot_kernel) /* In boot.S */
OUTPUT_ARCH(m68k)

/*MEMORY 
{
}*/

SECTIONS
{
    . = 0x02005000; /* Entry Point */
    .text :
    {
        ___stext = . ;
        boot.o(.text) /* must come first (for boot_kernel) */
        *(.text)
        /* *(.rodata) */
        ___etext = . ;
    }
    .data :
    {
        ___sdata = . ;
        *(.data)
        /* *(.rodata) */
        ___edata = . ;
    }
    .bss :
    {
        ___sbss = . ;
        *(.bss)
        *(COMMON)
        ___ebss = .;
        . = ALIGN(4096);
        ___heap = .;
    }
    . = . ;
    ___gp = . ;
}
