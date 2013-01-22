#ifndef _pc16552_h
#define _pc16552_h

extern void init_uart(void);

#define PC16552_ADDR 0x6805fff0 /* Base Memory Address */

/* Registers for a single channel of the pc16552. */
struct pc16552_regs{
    union{
        unsigned char rhr;  /* RX Holding Register */
        unsigned char thr;  /* TX Holding Register */
    };
    unsigned char ier;      /* Interrupt Enable Register */
    union{
        unsigned char iir;  /* Interrupt Identification Register */
        unsigned char fcr;  /* FIFO Control Register */
    };
    unsigned char lcr;      /* Line Control Register */
    unsigned char mcr;      /* Modem Control Register */ 
    unsigned char lsr;      /* Line Status Register */
    unsigned char msr;      /* Modem Status Register */
    unsigned char scr;      /* Scratch Register */
    unsigned char dll;      /* Divisor Latch (LS) */
    unsigned char dlm;      /* Divisor Latch (MS) */
    unsigned char afr;      /* Alternate Function Register */
};



#endif /* _pc16552_h */
