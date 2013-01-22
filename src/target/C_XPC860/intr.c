#include <lib/printk.h>
#include <target/intr.h>
#include <target/quicc.h>

/* Table of pointers to interrupt service routines */
void *__isr_table[MAX_ISR];

/* Request ISR 
 *
 * ISR Vector list on MPC860UM page 10-14.
 */
int reqisr(unsigned int vector, void (*handler)(void))
{    
    /* Some sanity checks */
    if(vector >= MAX_ISR)
        return -1;
    if(handler == NULL)
        return -1;

    __isr_table[vector] = handler; 

    /* Unmask the bit in simask */
    quicc->simask |= (1<<(31-vector));

    return 0;
}

/* Release ISR */
void relisr(unsigned int vector)
{
    __isr_table[vector] = nullisr;
    
    /* Mask interrupt source in simask */
    quicc->simask &= ~(1<<(31-vector));
    return;
}

void handler(void)
{
    printk("exception %i\n",quicc->sivec_intc);
}

/* Return from interrupt instruction */
#define RFI 0x4c000064

/* Install an exception handler for vector. This is not to be
 * used in driver code, use reqisr() to set up your interrupt 
 * handlers. Length of handler must be less than 256 bytes or
 * 64 instructions.
 */
static void install_handler(unsigned int vector, void *handler)
{
    long *voa; /* vector offset address */
    long *code; /* ISR code */

    voa = (long *) (EVT_BASE + (vector * 0x100));
    code = (long *) handler;

    do{
        *voa++ = *code;
    } while(*code++ != RFI);
    return;
}

void nullisr(void)
{
    printk("null isr fired\n");
}

void interrupt_handler(void)
{
    unsigned char vec;
    void (*handler)(void);

    vec = quicc->sivec_intc;
    handler = __isr_table[vec/4];
    handler();
}


/* handle system calls, called from syscall in asm.S */
void syscall_handler(unsigned int callnr)
{
    (void) callnr;
}



void init_isr(void)
{
    int i;
    
    /* Initialize the __isr_table */
    for(i=0;i<MAX_ISR;i++)
        __isr_table[i] = nullisr;

    /* Clear any pending interrupts and 
     * mask off all interrupt sources  */
    quicc->sipend = 0x0000;
    quicc->simask = 0x0000;

    /* Install Exception Handlers */
    //install_handler(2,exception); /* Machine Check Exception */
    install_handler(12,syscall); /* System Call */

    /* Install our main interrupt handler */
    install_handler(5,interrupt);

    return;
}
