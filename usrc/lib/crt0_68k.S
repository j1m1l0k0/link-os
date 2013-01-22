/* crt0 for 68k targets */

.globl _start
_start:

    /* push args onto stack for main() */
    movem.l %d1-%d3, %sp@-
    jsr main

    /* sys_exit(0) */
    move.l #3, %d0
    move.l #0, %d1
    trap #8

