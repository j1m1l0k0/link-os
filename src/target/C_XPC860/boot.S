.globl boot_kernel
boot_kernel:
    mtspr 81, 0 /* disable interrupts */

    bl init_target

    b init_kernel /* go to init_kernel in kernel/main.c */

    /* we will never get here */

