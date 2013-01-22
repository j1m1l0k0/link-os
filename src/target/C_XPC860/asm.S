#include <syscall.h>
#include <target/quicc.h>

/* SPR's */
#define XER   1   /* integer exception register */
#define LR    8   /* link register */
#define CTR   9   /* count register */
#define SRR0  26  /* Save/Restore register 0 */
#define SRR1  27  /* Save/Restore register 1 */

/* Quicc Offsets */
#define SIVEC 0xff00001c

.globl exception
exception:
    /* load handler address into the link register */ 
    lis  r9,handler@ha
    addi r9,r9,handler@l
    mtspr LR,r9
    /* jump to handler */
    blrl
    /* return */
    rfi

/* System Calls all start here */
.globl syscall
syscall:
    
    /* save scratch regs on top of cthread's stack */
    stw r1, 0(r1) /* sp */
    stw r6, 4(r1) /* r6 -- scratch register */
     
    /* r0 contains call number, r3-r5 are args */
    /* TODO: check if r0 < MAX_SYSCALL */
    mulli r0,r0,4 /* call number --> offset in syscall_table */

    /* lookup and jump to syscall handler (found in kernel/syscall.c) */
    lis r6,__syscall_table@ha
    addi r6,r6,__syscall_table@l
    lwzx r6,r6,r0
    mtspr LR,r6
    blrl

    /* we're finished -- restore registers and return */
    rfi


/* Installed to 0x500, handles all external interrupts
 * coming into the 860. 
 */
.globl interrupt
interrupt:
      
    lis  r9,interrupt_handler@ha
    addi r9,r9,interrupt_handler@l
    mtspr LR,r9
    blrl

    rfi

#if 0 
    /* load vector from sivec */ 
    lis r3,SIVEC@ha
    lwz r3,SIVEC@l(r3)
    lbz r3,r3(0)
    
    /* load handler address from isr_table
       into the link register and jump to it */
    lis r4,isr_table@ha
    addi r4,r4,isr_table@l
    add r4,r4,r3
    mtspr LR,(4)
    blrl

    rfi
#endif
