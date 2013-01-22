/* Copyright (C) 2006 Kyle Lutz <kyle.r.lutz@gmail.com> */
/* Link script for cisco 68030 based routers */

ENTRY(boot_kernel) /* In boot.S */
OUTPUT_FORMAT("a.out-zero-big")
OUTPUT_ARCH(m68k)

SECTIONS
{
    . = 0x1000; /* Entry Point */
    .text :
    {
        ___stext = . ;
        boot.o(.text)
        *(.text)
        *(.rodata)
        ___etext = . ;
    }
    .data :
    {
        ___sdata = . ;
        *(.data) 
        ___edata = . ;
    }
    .bss :
    {
        ___sbss = . ;
        *(.bss)
        *(COMMON)
        ___ebss = . ;
        . = ALIGN(4096);
        ___heap = .;
    }
}
