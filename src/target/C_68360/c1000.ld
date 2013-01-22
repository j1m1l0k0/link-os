/* Copyright (C) 2006 Kyle Lutz <kyle.r.lutz@gmail.com> */
/* Link script for cisco 1000 series routers */

ENTRY(boot_kernel) /* in boot.S */
OUTPUT_ARCH(m68k)


/*MEMORY 
{
}*/

SECTIONS
{
    . = 0x02004000; /* entry point */
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
