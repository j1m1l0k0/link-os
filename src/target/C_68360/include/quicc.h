/* Copyright (C) 2006 Kyle Lutz <kyle.r.lutz@gmail.com> */
/* The reference pages in the comments below refer to the MC68360 User Manual */
#ifndef _quicc_h
#define _quicc_h


extern void init_quicc(void);
extern volatile struct quicc360_struct *quicc;
extern volatile struct smc_uart *uart;

extern struct tty_struct console_tty;


/* Base Memory Addresses */
#define QUICC_BASE 0x0FF00000 /* Cisco Boot ROM sets mbar here */

#define SCC1_BASE  (QUICC_BASE + 0xC00)
#define MISC_BASE  (QUICC_BASE + 0xCB0)
#define SCC2_BASE  (QUICC_BASE + 0xD00)
#define SPI_BASE   (QUICC_BASE + 0xD80)
#define TIMER_BASE (QUICC_BASE + 0xDB0)
#define SCC3_BASE  (QUICC_BASE + 0xE00)
#define IDMA1_BASE (QUICC_BASE + 0xE70)
#define SMC1_BASE  (QUICC_BASE + 0xE80)
#define SCC4_BASE  (QUICC_BASE + 0xF00)
#define IDMA2_BASE (QUICC_BASE + 0xF70)
#define SMC2_BASE  (QUICC_BASE + 0xF80)

/* Quicc Memory Map (ref 3-5) */
struct quicc360_struct {
    /* DUAL PORT RAM */
    unsigned char dpr[2048];  /* 2K -- MBAR + 0x000 */
    unsigned char r1[1024];   /* 3K -- MBAR + 0x800 */
    /* PRAM */
    unsigned char pram[768];  /* 4K -- MBAR + 0xC00 */
    unsigned char dpr2[256];
    
    /* REGB = MBAR + 0x1000 */

    /* SIM60 -- REGB + 0x000*/
    unsigned long mcr;
    char r2[4];
    unsigned char avr;
    unsigned char rsr;
    char r3[2];
    unsigned char clkocr;
    char r4[3]; 
    unsigned short pllcr;
    char r5[2];
    unsigned short cdvcr;
    unsigned short pepar;
    char r6[10];
    unsigned char sypcr;
    unsigned char swiv;
    char r7[2];
    struct {
        unsigned char zero :5; /* Unused / Always Zero */
        unsigned char irl  :3; /* Interrupt Request Level */
        unsigned char vec; /* Interrupt Vector */
    } PICR; /* Periodic Interrupt Control Register */
    char r8[2];
    unsigned short pitr;
    char r9[3];
    unsigned char swsr;
    unsigned long bkar;
    unsigned long bkcr;
    char r10[8];

    /* MEMC Registers */ 
    struct {
        unsigned char rcnt  :8;
        unsigned char refn  :1;
        unsigned char rcyc  :2;
        unsigned char pgs   :3;
        unsigned char dps   :2;
        unsigned char wbt40 :1;
        unsigned char wbtq  :1;
        unsigned char sync  :1;
        unsigned char emws  :1;
        unsigned char opar  :1;
        unsigned char pbee  :1;
        unsigned char tss40 :1;
        unsigned char ncs   :1;
        unsigned char dqw   :1;
        unsigned char dw40  :1;
        unsigned char gamx  :1;
        unsigned char res   :5;
    } GMR; /* Global Memory Register */
    unsigned short mstat;
    char r11[2];
    /* MEMC Option Registers */
    struct {
        char r1[8];
        unsigned long br;
        unsigned long or;
    } OR[8]; // + 0x0c4
    char r12[40];
    char r13[16];
    char r14[768];

    
    /* CPM */
    char r19[256];

    /* IDMA1 Registers */
    unsigned short iccr;
    char r20[2];
    unsigned short cmr1;
    char r21[2];
    unsigned long sapr1;
    unsigned long dapr1;
    unsigned long bcr1;
    unsigned char fcr1;
    char r22[1];
    unsigned char cmar1;
    char r23[1];
    unsigned char csr1;
    char r24[3];

    /* SDMA Registers */
    unsigned char sdsr;
    char r25[1];
    unsigned short sdcr;
    unsigned long sdar;

    /* IDMA2 Registers */
    char r26[2];
    unsigned short cmr2;
    unsigned long sapr2;
    unsigned long dapr2;
    unsigned long bcr2;
    unsigned char fcr2;
    char r27[1];
    unsigned char cmar2;
    char r28[1];
    unsigned char csr2;
    char r29[7];

    /* CPIC Registers */
    unsigned long cicr;
    unsigned long cipr;
    unsigned long cimr;
    unsigned long cisr;
    unsigned short padir;
    unsigned short papar;
    unsigned short paodr;
    unsigned short padat;
    char r30[8];
    unsigned short pcdir;
    unsigned short pcpar;
    unsigned short pcso;
    unsigned short pcdat;
    unsigned short pcint;
    char r31[22];
    unsigned short tgcr;
    char r32[14];
    unsigned short tmr1;
    unsigned short tmr2;
    unsigned short trr1;
    unsigned short trr2;
    unsigned short tcr1;
    unsigned short tcr2;
    unsigned short tcn1;
    unsigned short tcn2;
    unsigned short tmr3;
    unsigned short tmr4;
    unsigned short trr3;
    unsigned short trr4;
    unsigned short tcr3;
    unsigned short tcr4;
    unsigned short tcn3;
    unsigned short tcn4;
    unsigned short ter1;
    unsigned short ter2;
    unsigned short ter3;
    unsigned short ter4;
    char r33[8];
    unsigned short cr; /* Command Register */
    unsigned short rccr;
    char r34[18];
    unsigned short rter;
    char r35[2];
    unsigned short rtmr;
    char r36[20];

    /* BRG Registers */
    unsigned long brgc1;
    unsigned long brgc2;    
    unsigned long brgc3;
    unsigned long brgc4;

    /* SCC's Registers */
    struct {
        struct {
            /* gsmr_l */
            char r1:1;
            unsigned char edge   :2; /* Clock Edge */
            unsigned char tci    :1; /* TX Clock Invert */
            unsigned char tsnc   :2; /* TX Sense */
            unsigned char rinv   :1; /* DPLL RX Input Invert Data */
            unsigned char tinv   :1; /* DPLL TX Input Invert Data */
            unsigned char tpl    :3; /* TX Preamble Length */
            unsigned char tpp    :2; /* TX Preamble Pattern */
            unsigned char tend   :1; /* TX Frame Ending */
            unsigned char tdcr   :2; /* TX Divide Clock Rate */
            unsigned char rdcr   :2; /* RX DPLL Clock Rate */
            unsigned char renc   :3; /* RX Decoding Method */
            unsigned char tenc   :3; /* TX Encoding Method */
            unsigned char diag   :2; /* Diagnostic Mode */
            unsigned char enr    :1; /* Enable RX */
            unsigned char ent    :1; /* Enable TX */
            unsigned char mode   :4; /* Channel Protocol Mode */
            /* gsmr_h */
            short r2:15;
            unsigned char gde    :1; /* Glitch Detect Enable */
            unsigned char tcrc   :2; /* Transparent CRC */
            unsigned char revd   :1; /* Reverse Data */
            unsigned char trx    :1; /* Transparent RX */
            unsigned char ttx    :1; /* Transparent TX */
            unsigned char cdp    :1; /* CD Pulse */
            unsigned char ctsp   :1; /* CTS Pulse */
            unsigned char cds    :1; /* CD Sampling */
            unsigned char ctss   :1; /* CTS Sampling */
            unsigned char tfl    :1; /* TX FIFO Length */
            unsigned char rfw    :1; /* RX FIFO Width */
            unsigned char txsy   :1; /* TX Synchronized to RX */
            unsigned char synl   :2; /* Sync Length */
            unsigned char rtsm   :1; /* RTS Mode */
            unsigned char rsyn   :1; /* RX Synchronization Timing */
        } GSMR; /* General SCC Mode Register (ref 7-111) */
        unsigned short psmr;
        char r1[2];
        unsigned short todr; /* TX on Demand Register */
        unsigned short dsr; /* Data Synchronization Register */
        unsigned short scce;
        char r2[2];
        unsigned short sccm;
        char r3[1];
        unsigned char sccs;
        char r4[8];
    } SCC[4];

    /* SMC's Registers */
    struct {
        char r1[2];
        /*struct {
            char r1:1;
            unsigned char clen  :4;
            unsigned char sl    :1;
            unsigned char pen   :1;
            unsigned char pm    :1;
            char r2:2;
            unsigned char sm    :2;
            unsigned char dm    :2;
            unsigned char ten   :1;
            unsigned char ren   :1;
        } SMCMR;*/
        unsigned short smcmr;
        char r2[2];
        unsigned char smce;
        char r3[3];
        unsigned char smcm;
        char r4[5];
    } SMC[2];

    /* SPI Registers */
    unsigned short spmode;
    char r37[4];
    unsigned char spie;
    char r38[3];
    unsigned char spim;
    char r39[2];
    unsigned char spcom;
    char r40[4];

    /* PIP Registers */
    unsigned short pipc;
    char r41[2];
    unsigned short ptpr;
    unsigned long pbdir;
    unsigned long pbpar;
    unsigned short pbodr;
    char r42[2];
    unsigned long pbdat;
    char r43[24];

    /* SI Registers */
    unsigned long simode;
    unsigned char sigmr;
    char r44[1];
    unsigned char sistr;
    unsigned char sicmr;
    char r45[4];
    unsigned long sicr;
    char r46[2]; // check this
    unsigned long sirp;
    char r47[12];
    unsigned char siram[256];
};

/* QBD: Quicc Buffer Descriptor (ref 7-10) */
struct qbd {
    unsigned short sc; /* Status/Control */
    unsigned short len; /* Length */
    char *buf; /* Buffer Pointer */
};

/* RISC Revision PRAM (ref 7-5) */
struct risc_revision {
    unsigned short rev_num; /* Microcode Revision Number */
    char res[10];
};

/* RISC TIMER (ref 7-12) */
struct risc_timer {
    unsigned short tm_base;
    unsigned short tm_ptr;
    unsigned short r_tmr;
    unsigned short r_tmv;
    unsigned long tm_cmd;
    unsigned long tm_cnt;
};

/* IDMA (ref 7-35) */
struct idma {
    unsigned short ibase;
    unsigned short ibptr;
    unsigned long istate;
    unsigned long itemp;
};

/* PRAM Common to all SCC's (ref 7-127) */
#define SCC_COMMON                                              \
    unsigned short rbase; /* RX QBD base */                     \
    unsigned short tbase; /* TX QBD base */                     \
    unsigned char rfcr; /* RX function code */                  \
    unsigned char tfcr; /* TX function code */                  \
    unsigned short mrblr; /* Max RX Buffer Length */            \
    unsigned long rstate; /* RX internal state */               \
    unsigned long rxidp; /* RX internal data pointer */         \
    unsigned short rbptr; /* RX QBD pointer */                  \
    unsigned short rxibc; /* RX internal byte count */          \
    unsigned long rxtemp; /* RX temp */                         \
    unsigned long tstate; /* TX internal state */               \
    unsigned long txidp; /* TX internal data pointer */         \
    unsigned short tbptr; /* TX QBD pointer */                  \
    unsigned short txibc; /* TX internal byte count */          \
    unsigned long txtemp; /* TX temp */                         \
    unsigned long rcrc; /* Temp RX CRC */                       \
    unsigned long tcrc; /* Temp TX CRC */ 
    /* Protocol Specific PRAM Follows */

/* SCC UART PRAM (ref 7-147) */
struct scc_uart {
    SCC_COMMON;
    char res[8];
    unsigned short max_idl;
    unsigned short idlc;
    unsigned short brkcr;
    unsigned short parec;
    unsigned short frmec;
    unsigned short nosec;
    unsigned short brkec;
    unsigned short brkln;
    unsigned short uaddr1;
    unsigned short uaddr2;
    unsigned short rtemp;
    unsigned short toseq;
    unsigned short ch1;
    unsigned short ch2;
    unsigned short ch3;
    unsigned short ch4;
    unsigned short ch5;
    unsigned short ch6;
    unsigned short ch7;
    unsigned short ch8;
    unsigned short rccm;
    unsigned short rlbc;
};

/* SCC HDLC PRAM (ref 7-172) */
struct scc_hdlc {
    SCC_COMMON;
    char res[4];
    unsigned long c_mask;
    unsigned long c_pres;
    unsigned short disfc;
    unsigned short crcec;
    unsigned short abtsc;
    unsigned short nmarc;
    unsigned short retrc;
    unsigned short mflr;
    unsigned short max_cnt;
    unsigned short rfthr;
    unsigned short rfcnt;
    unsigned short hmask;
    unsigned short haddr1;
    unsigned short haddr2;
    unsigned short haddr3;
    unsigned short haddr4;
    unsigned short tmp;
    unsigned short tmp_mb;
};

/* SCC BISYNC PRAM (ref 7-203) */
struct scc_bisync {
    SCC_COMMON;
    char res[4];
    // finish this
};

/* SCC TRANSPARENT PRAM (ref 7-225) */
struct scc_transparent {
    SCC_COMMON;
    unsigned long crc_p;
    unsigned long crc_c;
};

/* SCC ETHERNET PRAM (ref 7-247) */
struct scc_ethernet {
    SCC_COMMON;
    unsigned long c_pres;
    unsigned long c_mask;
    unsigned long crcec;
    unsigned long alec;
    unsigned long disfc;
    unsigned short pads;
    unsigned short ret_lim;
    unsigned short ret_cnt;
    unsigned short mflr;
    unsigned short minflr;
    unsigned short maxd1;
    unsigned short maxd2;
    unsigned short maxd;
    unsigned short dma_cnt;
    unsigned short max_b;
    unsigned short gaddr1;
    unsigned short gaddr2;
    unsigned short gaddr3;
    unsigned short gaddr4;
    struct {
        long data0;
        long data1;
        long rba0;
        long crc;
        short bcnt;
    } TBUF0;
    unsigned short paddr1_l;
    unsigned short paddr1_m;
    unsigned short paddr1_h;
    unsigned short p_per;
    unsigned short rfbd_ptr;
    unsigned short tfbd_ptr;
    unsigned short tlbd_ptr;
    struct {
        long data0;
        long data1;
        long rba0;
        long crc;
        short bcnt;
    } TBUF1;
    unsigned short tx_len;
    unsigned short iaddr1;
    unsigned short iaddr2;
    unsigned short iaddr3;
    unsigned short iaddr4;
    unsigned short boff_cnt;
    unsigned short taddr_l;
    unsigned short taddr_m;
    unsigned short taddr_h;
};

/* SMC UART PRAM (ref 7-271) */
struct smc_uart {
    unsigned short rbase; /* RX QBD base */
    unsigned short tbase; /* TX QBD base */
    unsigned char rfcr; /* RX function code */
    unsigned char tfcr; /* TX function code */
    unsigned short mrblr; /* Max RX Buffer Length */
    unsigned long rstate; /* RX internal state */
    unsigned long ridp; /* RX internal data pointer */
    unsigned short rbptr; /* RX QBD pointer */ //make this struct qbd* ??
    unsigned short ribc; /* RX internal byte count */
    unsigned long rtemp; /* RX temp */
    unsigned long tstate; /* TX internal state */
    unsigned long tidp; /* TX internal data pointer */
    unsigned short tbptr; /* TX QBD pointer */
    unsigned short tibc; /* TX internal byte count */
    unsigned long ttemp; /* TX temp */
    /* (ref 7-277) */ 
    unsigned short max_idl; /* Maximum idle characters */
    unsigned short idlc; /* Temporary idle counter */
    unsigned short brkln; /* Last recieved break length */
    unsigned short brkec; /* Recieve break condition counter */
    unsigned short brkcr; /* Break count register */
    unsigned short r_mask; /* Temporary bit mask */
};

/* SPI PRAM (ref 7-320) */

/* Buffer Descriptor Status/Control Codes */
#define BD_OVERRUN      (1 << 1)  /* Overrun */
#define BD_UNDERRUN     (1 << 1)  /* Underrun */
#define BD_PARITY       (1 << 3)  /* Parity Error */
#define BD_FRAME        (1 << 4)  /* Framing Error */
#define BD_BREAK        (1 << 5)  /* Recieved Break */
#define BD_IDLE         (1 << 8)  /* Recieved Idle */
#define BD_PREAMB       (1 << 8)  /* Send Preamble */
#define BD_CONT         (1 << 9)  /* Continuous Mode */
#define BD_LAST         (1 << 11) /* Last in message */
#define BD_INTR         (1 << 12) /* Interrupt */
#define BD_WRAP         (1 << 13) /* Wrap (Final BD in ring) */
#define BD_READY        (1 << 15) /* Ready */
#define BD_EMPTY        (1 << 15) /* Empty */

/* RISC Command register (CR) (ref pg 7-5) */
/* Execute Command Register operation on channel */
#define CR_EXEC(op,ch) quicc->cr = ((op << 8) | (ch << 4) | 1); \
                       while(quicc->cr & 1)
/* CR Opcodes */
#define CR_INITTXRX     0   /* Init RX & TX Parameters */
#define CR_INITRX       1   /* Init RX Parameters */
#define CR_INITTX       2   /* Init TX Parameters */
#define CR_ENTHUNT      3   /* Enter Hunt Mode */
#define CR_STOPTX       4   /* Stop TX */
#define CR_GSTOPTX      5   /* Graceful Stop TX */
#define CR_INITIDMA     5   /* Init IDMA */
#define CR_RESTX        6   /* Restart TX */
#define CR_CLSRBD       7   /* Close RX BD */
#define CR_SETGA        8   /* Set Group Address */
#define CR_GCITO        9   /* GCI Timeout */
#define CR_RSTBCS       10  /* Reset BCS */
#define CR_GCIAR        10  /* GCI Abort Req */
/* CR Channels */
#define CR_SCC1         0   /* SCC1 */
#define CR_SCC2         4   /* SCC2 */
#define CR_SPI          5   /* SPI */
#define CR_TIMERS       5   /* RISC Timers */
#define CR_SCC3         8   /* SCC3 */
#define CR_SMC1         9   /* SMC1 */
#define CR_IDMA1        9   /* IDMA1 */
#define CR_SCC4         12  /* SCC4 */
#define CR_SMC2         13  /* SMC2 */
#define CR_IDMA2        13  /* IDMA2 */

/* CPIC Interrupt Mask's (ref 7-380) */
#define CIMR_PC11   (1 << 1)
#define CIMR_PC10   (1 << 2)
#define CIMR_SMC2   (1 << 3)
#define CIMR_PIP    (1 << 3)
#define CIMR_SMC1   (1 << 4)
#define CIMR_SPI    (1 << 5)
#define CIMR_PC9    (1 << 6)
#define CIMR_TIMER4 (1 << 7)
#define CIMR_PC8    (1 << 9)
#define CIMR_PC7    (1 << 10)
#define CIMR_PC6    (1 << 11)
#define CIMR_TIMER3 (1 << 12)
#define CIMR_PC5    (1 << 14)
#define CIMR_PC4    (1 << 15)
#define CIMR_RTT    (1 << 17)
#define CIMR_TIMER2 (1 << 18)
#define CIMR_IDMA2  (1 << 20)
#define CIMR_IDMA1  (1 << 21)
#define CIMR_SDMA   (1 << 22)
#define CIMR_PC3    (1 << 23)
#define CIMR_PC2    (1 << 24)
#define CIMR_TIMER1 (1 << 25)
#define CIMR_PC1    (1 << 26)
#define CIMR_SCC4   (1 << 27)
#define CIMR_SCC3   (1 << 28)
#define CIMR_SCC2   (1 << 29)
#define CIMR_SCC1   (1 << 30)
#define CIMR_PC0    (1 << 31)

/* SMCE Masks */
#define SMCE_RX     (1 << 0)
#define SMCE_TX     (1 << 1)
#define SMCE_BSY    (1 << 2)
#define SMCE_BRK    (1 << 4)
#define SMCE_TXE    (1 << 4)
#define SMCE_BRKE   (1 << 6)


#endif /* _quicc_h */
