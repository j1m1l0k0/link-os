/* Copyright (C) 2006 Kyle Lutz <kyle.r.lutz@gmail.com> */
/* Boot code for Cisco 68360 based targets (1000,1600) */

/* boot_kernel(), where it all begins */
.globl boot_kernel
boot_kernel:
    move.w #0x2700, %sr /* Mask Interrupts */

    move.l #0x2003000, sp

/* Clear .bss */
    lea ___sbss, %a0
    lea ___ebss, %a1
clrbss:
    move.l  #0, (%a0)+
    cmp.l   %a0, %a1
    bge     clrbss

    
    jsr _init_target

    jmp _init_kernel

    
    /* we won't ever reach here (hopefully) */
    

