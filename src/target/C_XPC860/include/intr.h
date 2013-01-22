#ifndef _intr_h
#define _intr_h

extern int reqisr(unsigned int vector, void (*handler)(void));
extern void relisr(unsigned int vector);
extern void init_isr(void);

extern void nullisr(void);
extern void exception(void);
extern void interrupt(void);
extern void syscall(void);

/* Base address of the Exception Vector Table */
#define EVT_BASE 0x80000000

/* Total of 16 ISR vectors */
#define MAX_ISR 16

#endif /* _intr_h */
