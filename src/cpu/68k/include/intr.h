#ifndef _intr_h
#define _intr_h

#include <cpu/context.h>
#include <target/target.h>

/* Foward declarations */
struct stack_frame; /* Defined below */ 

/* Exported Functions */
extern int reqisr(unsigned int vector, void (*handler)(void));
extern void relisr(unsigned int vector);
extern void init_isr(void);

/* Exception Handlers */
/* Assembly routines from asm.S */
extern void exception(void); 
extern void error_exception(void);
extern void syscall(void);
extern void quicc_interrupt(void);
/* C exception handlers from intr.c */
extern void handle_exception(struct sys_context ctx);
extern void handle_error_exception(struct stack_frame *sf);

struct stack_frame {
    short sr;         /* Status Register */
    long pc;          /* Program Counter */
    unsigned fmt :4;  /* Format */
    unsigned vec :12; /* Exception Vector */
    union {
        struct {
            long addr; /* Instruction Address */
        }f2;
        struct {
            long addr; /* Instruction Address */
            /* internal regsters, 4 words */
        }f9;
        struct {
            short ireg; /* Internal Register */
            short ssw; /* Special Status Word */
            short ipsc;
            short ipsb;
            long addr; /* Faulting Address */
            /* two interal registers */
            /* two data output buffers */
            /* two internal registers */
        }f10;
        struct {
            short ireg; /* Internal Register */
            short ssw; /* Special Status Word */
            short ipsc;
            short ipsb;
            long addr; /* faulting address */
            /* two interal registers */
            /* two data output buffers */
            /* internal registers, 4 words */
            /* 2 stage B adresses */
            /* internal registers, 2 words */
            /* two data output buffers */
            /* internal registers, 3 words */
            /* version #, internal info */
            /* internal registers, 18 words */
        }f11;
        /* f12: cpu32+ address/bus error (ref 68360 UM pg 5-58) */
        struct {
            long addr;
            long dbuf; /* or pesr and fmt/vec word */
            long pc; /* Current/Faulted Instruction Program Counter */
            short itcr; /* Internal Transfer Count Register */
            unsigned fmt:2; /* TP/MV bits in SSR, determines the frame format */
            unsigned ssw:14; /* special status word */
        }f12;
    }; 
};

#define MAX_ISR 256


#endif /* _intr_h */
