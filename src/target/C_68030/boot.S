/* Copyright (C) 2006 Kyle Lutz <kyle.r.lutz@gmail.com> */
/* Boot code for Cisco 68030 based targets (2500,3000,4000) */

/* boot_kernel(), where it all begins */
.globl boot_kernel
boot_kernel:
    move.w #0x2700, %sr
    
    jsr _init_target
    jmp _init_kernel

end:
    trap #0
    rts 
