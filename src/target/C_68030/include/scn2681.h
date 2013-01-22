/* Copyright (C) 2006 Kyle Lutz <kyle.r.lutz@gmail.com> */

#ifndef _scn2681_h
#define _scn2681_h

extern void init_uart(void);
extern void scn2681_interrupt(void);

#define SCN2681_ADDR 0x02120100 /* Base Memory Address */

struct scn2681_regs{
    unsigned char mra;       /* Mode Register A */
    union{
        unsigned char sra;   /* Status Register A */
        unsigned char csra;  /* Clock Select Register A */
    };
    union{
        unsigned char brgt;  /* BRG Test */
        unsigned char cra;   /* Command Register A */
    };
    union{
        unsigned char rhra;  /* RX Holding Register A */
        unsigned char thra;  /* TX Holding Register A */
    };
    union{
        unsigned char ipcr;  /* Input Port Change Register */
        unsigned char acr;   /* Aux. Control Register */
    };
    union{
        unsigned char isr;   /* Interrupt Status Register */
        unsigned char imr;   /* Interrupt Mask Register */
    };
    union{
        unsigned short ct;   /* Counter/Timer Value */
        unsigned short crp;  /* Counter/Timer Preset Value */
    };
    unsigned char mrb;       /* Mode Register B */
    union{
        unsigned char srb;   /* Status Register B */
        unsigned char csrb;  /* Clock Select Register B */
    };
    union{
        unsigned char xtest; /* 1X/16X Test */
        unsigned char crb;   /* Command Register B */ 
    };
    union{
        unsigned char rhrb;  /* RX Holding Register B */
        unsigned char thrb;  /* TX Holding Register B */
    };
    unsigned char res;       /* Reserved */
    union{
        unsigned char inp;   /* Input Ports IP0-IP6 */
        unsigned char opcr;  /* Output Port Conf Register */
    };
    union{
        unsigned char startc; /* Start Counter Command */
        unsigned char obitc;  /* Set Output Port Bits Command */
    };
    union{
        unsigned char stopc;  /* Stop Counter Command */
        unsigned char rbitc;  /* Reset Output Port Bits Command */
    };
};

/* Scn2681 Interrupt Masks */
#define SIM_TXA     (1 << 0) /* TX A ready */
#define SIM_RXA     (1 << 1) /* RX A ready */
#define SIM_BRKA    (1 << 2) /* break A */
#define SIM_CTR     (1 << 3) /* counter ready */
#define SIM_TXB     (1 << 4) /* TX B ready */
#define SIM_RXB     (1 << 5) /* RX B reeady */
#define SIM_BRKB    (1 << 6) /* break B */
#define SIM_PORT    (1 << 7) /* input port change */





#endif /* _scn2681_h */
