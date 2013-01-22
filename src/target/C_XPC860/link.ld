ENTRY(boot_kernel)
/*STARTUP(boot.o) */

SECTIONS
{
    .comment :
    {
        *(.comment)
    }
    . = 0x80008000; /* Entry Point */
    .text :
    {
        _stext = . ;
        *(.text)
        *(.rodata)
        _etext = . ;
    }
    .data :
    {
        _sdata = . ;
        *(.data)
        _edata = . ;
        _sbss = . ;
        *(.bss)
        _ebss = . ;
        . = ALIGN(4096);
        __heap = .;
    }
}
