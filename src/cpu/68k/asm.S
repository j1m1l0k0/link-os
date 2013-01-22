/* Copyright (C) 2006 Kyle Lutz <kyle.r.lutz@gmail.com> */

/* interrupt handlers */

#include <target/target.h>


/*
d0 - 0
d1 - 4
d2 - 8
d3 - 12
d4 - 16
d5 - 20
d6 - 24
d7 - 28
a0 - 32
a1 - 36
a2 - 40
a3 - 44
a4 - 48
a5 - 52
a6 - 56
a7 - 60
*/

.globl _syscall
_syscall:
    movem.l d0-d7/a0-a7, sp@-
    move.l sp, ___ctx
    
    /* push args */
    movem.l d1-d3, sp@-
   
    /* load address of handler from syscall_table */
    lea ___syscall_table, a0
    lsl.l #2, d0
    move.l (a0,d0), a0

    jsr (a0)          /* jump to system call handler */
    add.l #12, sp     /* clean up the stack */

    move.l d0, (sp)   /* put return value in threads d0 register */

    /* if(reschedule_flag > 0)
     *     sched(struct sys_context);
     * else
     *     return
     */
    cmpi #0, _reschedule_flag 
    beq  syscall_done

    bsr _sched_thread

syscall_done:
    movem.l sp@+, d0-d7/a0-a7
    rte
    
.globl _exception
_exception:
    /* Save System Context */
    movem.l d0-d7/a0-a7, sp@-
#ifndef TARGET_IS_c2500
    move.l usp, a0
    move.l a0, sp@-
#else
    move.l ssp, a0
    move.l a0, sp@-
#endif

    bsr _handle_exception /* in cpu/intr.c */
    
    /* Restore Context and Return */
#ifndef TARGET_IS_c2500
    move.l sp@+, a0
    move.l a0, usp
#else
    move.l sp@+, a0
    move.l a0, ssp
#endif
    movem.l sp@+, d0-d7/a0-a7
    rte

.globl _error_exception
_error_exception:
    move.l    %sp,%sp@- /* Stack Frame Pointer */    
    bsr       _handle_error_exception 
    addq.l    #4,%sp

    cmpi #0, _reschedule_flag  
    beq  error_exception_done

    movem.l d0-d7/a0-a7, sp@-
    bsr _sched_thread
    movem.l %sp@+, d0-d7/a0-a7
    
error_exception_done:
    rte


/* Used by hwd.c for memory detection */
.globl _hwd_berr_isr
_hwd_berr_isr:
#ifdef CPU_IS_68360
    move.l  a0, sp@-
    move.l  sp,sp@-
    andi.w  #0xCEFF, sp@(14)
    move.l  sp@+, a0
    addq.l  #4, sp
#else
    andi.w  #0xCEFF, sp@(10)
#endif
    addq.b  #1, _hwd_bus_error_fired  /* set flag */  
    rte

.globl _reset_isr
_reset_isr:
    rte
