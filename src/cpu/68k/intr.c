/* Copyright (C) 2006 Kyle Lutz <kyle.r.lutz@gmail.com> */
/* 68k/intr.c -- Functions for handling 68k interrupts */

/* 68360 UM pg 5-36 */

#include <thread.h>
#include <cpu/intr.h>
#include <cpu/context.h>
#include <target/target.h>
#include <lib/printk.h>

long EVT_BASE; /* Base Address of the exception vector table. */

/* Install an exception handler for the specified vector. */
static void install_handler(unsigned int vector, void *handler) 
{
    unsigned long *vaddr;
    vaddr = (unsigned long *) (EVT_BASE + (vector * 4));
    *vaddr = (unsigned long) handler;
}

int reqisr(unsigned int vector, void (*handler)(void))
{
    if(vector >= MAX_ISR)
        return -1;
    if(handler == NULL)
        return -1;

    install_handler(vector,handler);
    return 0;
}

void relisr(unsigned int vector)
{
    (void) vector;
    return;
}

/* init_isr()  -- Initialize interrupt handlers.
 */
void init_isr(void)
{
    int i;
    
    /* Set EVT_BASE. For cisco we just use whatever the bootrom sets the VBR to. */
    asm("movec  vbr, d0\n\t"
        "move.l d0, %0":"=m"(EVT_BASE));

    install_handler(2,error_exception); /* Bus Error */ 
    install_handler(3,error_exception); /* Address Error */
    install_handler(4,error_exception); /* Illegal Instruction */
    install_handler(5,error_exception); /* Division by Zero */
    install_handler(6,error_exception);
    install_handler(8,error_exception); /* Privilege Violation */
    install_handler(10,error_exception); /* Line 1010 Emulator */
    install_handler(11,error_exception); /* Line 1111 Emulator */
    install_handler(12,error_exception); /* Hardware Breakpoint */
    install_handler(14,error_exception); /* Format Error */
    install_handler(15,exception); /* Uninitialized Interrupt */
    install_handler(24,exception); /* Spurious Interrupt */
    
    for(i=25;i<31;i++){ install_handler(i,exception); } /* Autovector (1-7) */
    for(i=33;i<48;i++){ install_handler(i,exception); } /* Trap Handlers (1-15) */
    install_handler(40,syscall); /* Trap #8 -- system call */
    for(i=64;i<256;i++){ install_handler(i,exception); } /* User-Defined Vectors */
}


/* handle_exception() -- Handle a CPU Exception (interrupts / traps). 
 * 
 * Called from _exception in asm.S 
 */
void handle_exception(struct sys_context ctx)
{
    switch(ctx.vec/4){
        /* Uninitialized Interrupt */
        case 15:
            printk("Uninitialized Interrupt at 0x%x\n",ctx.pc);
            break;

        /* Spurious Interrupt */
        case 24:
            printk("Spurious Interrupt at 0x%x\n",ctx.pc);
            break;

        /* Trap #9 -- test */
        case 41:
            printk("test\n");
            break;
        
        /* Trap #10 */
        case 42:
            break;

        default:
            printk("unhandled exception vector %i\n",ctx.vec/4);
            break;
    }
    
    return;
}

/* handle_error_exception() -- Handle a CPU Error
 * Called from _error_exception in asm.S 
 */
void handle_error_exception(struct stack_frame *sf)
{
    long addr = 0;
    switch(sf->vec/4){
        /* Bus/Address Error */
        case 2: case 3:
            switch(sf->fmt){
                case 10:
                    addr = sf->f10.addr; break;
                case 11:
                    addr = sf->f11.addr; break;
                case 12:
                    addr = sf->f12.addr; break;
            }
            printk("\n%s Error at 0x%x\n",(sf->vec==8?"Bus":"Address"),addr);
            printk("pc=0x%x  sr=0x%x  fmt=%i  vec=%i\n",sf->pc,sf->sr,sf->fmt,sf->vec/4);
            if(sf->fmt == 12){
                printk("$C fmt = %i ssw = 0x%x\n",sf->f12.fmt,sf->f12.ssw);
                if(sf->f12.fmt == 0){
                    printk("operation was a %s\n",(sf->f12.ssw & (1<<6)) ? "read":"write");
                }
            }

            break;
        
        /* Illegal Instruction */
        case 4:
            printk("Illegal instruction at 0x%x\n",sf->pc);
            break;
        /* Divide By Zero */
        case 5:
            printk("Division by Zero at 0x%x\n",sf->pc);
            break;
        /* Privilege Violation */
        case 8:
            printk("CPU Privilege Violation at 0x%x\n",sf->pc);
            break;
        /* Line 1010 Emulator */
        case 10:
            printk("Line 1010 Emulator at 0x%x\n",sf->pc);
            break;
        /* Line 1111 Emulator */
        case 11:
            printk("Line 1111 Emulator at 0x%x\n",sf->pc);
            break;
        /* Hardware Breakpoint */ 
        case 12:
            printk("Hardware Breakpoint at 0x%x\n",sf->pc);
            break;
        /* Format Error */
        case 14:
            printk("Format Error at 0x%x\n",sf->pc);
            break;
        default:
            printk("unknown error_exception vec %i PC=0x%x\n",
                    sf->vec/4,sf->pc,sf->sr);
    }

    //TODO: implement recovery - kill current thread and go onto the next
    destroy_thread(cthread->id);
    reschedule();
    return;
}

