#ifndef _quicc_h
#define _quicc_h

/* QUICC's Base Address in memory */
#define QUICC_BASE 0xFF000000

#define DPRAM_BASE  (QUICC_BASE+0x2000)

/* PRAM Memory Map (ref 18-12) */
#define SCC1_BASE   (DPRAM_BASE+0x1C00)
#define I2C_BASE    (DPRAM_BASE+0x1C80)
/* miscellaneous    (DPRAM_BASE+0x1CB0) */
#define IDMA1_BASE  (DPRAM_BASE+0x1CC0)
#define SCC2_BASE   (DPRAM_BASE+0x1D00)
#define SPI_BASE    (DPRAM_BASE+0x1D80)
#define TIMER_BASE  (DPRAM_BASE+0x1DB0)
#define IDMA2_BASE  (DPRAM_BASE+0x1DC0)
#define SCC3_BASE   (DPRAM_BASE+0x1E00)
#define SMC1_BASE   (DPRAM_BASE+0x1E80)
/* reserved         (DPRAM_BASE+0x1EC0) */
#define SCC4_BASE   (DPRAM_BASE+0x1F00)
#define SMC2_BASE   (DPRAM_BASE+0x1F80)
#define PIP_BASE    (DPRAM_BASE+0x1F80)
/* reserved         (DPRAM_BASE+0x1FC0) */

/* asm may need to use the above memory map but not
 * the rest of this stuff */
#ifndef __LANGUAGE_ASSEMBLY__

/* QUICC860 Memory Map (ref 2-1) */
struct quicc860_struct {
    
    /* General System Interface Unit */
    unsigned long siumcr; /* SIU Module configuration register */
    unsigned long sypcr; /* System protection control register */
    char r1[6];
    unsigned short swsr; /* Software service register */
    unsigned long sipend; /* SIU interrupt pending register */
    unsigned long simask; /* SIU interrupt mask register */
    unsigned long siel; /* SIU interrupt edge/level register */
    union{
        unsigned long sivec; /* SIU interrupt vector register */
        struct{
            unsigned intc   :7;
            unsigned unused :25;
        }SIVEC;
        unsigned char sivec_intc;
    };
    unsigned long tesr; /* Transfer error status register */
    char r2[12];
    unsigned long sdcr; /* SDMA configuration register */
    char r3[76];
    
    /* PCMCIA */
    struct {
        unsigned long br;
        unsigned long or;
    } POR[8]; /* PCMCIA Option Registers */
    char r4[32];
    unsigned long pgcra; /* PCMCIA interface control register A */
    unsigned long pgcrb; /* PCMCIA interface control register B */
    unsigned long pscr; /* PCMCIA interface status changed register */
    char r5[4];
    unsigned long pipr; /* PCMCIA interface input pins register */
    char r6[4];
    unsigned long per; /* PCMCIA interface enable register */
    char r7[4];

    /* Memory Controller */
    struct{
        unsigned long br;
        unsigned long or;
    } MOR[8]; /* Memory option registers */
    char r8[36];
    unsigned long mar; /* Memory address register */
    unsigned long mcr; /* Memory command register */
    char r9[4];
    unsigned long mamr; /* Machine A mode register */
    unsigned long mbmr; /* Machine B mode register */
    unsigned short mstat; /* Memory status register */
    unsigned short mptpr; /* Memory periodic timer prescaler */
    unsigned long mdr; /* Memory data register */
    char r10[128];

    /* System Integration Timers */
    unsigned short tbscr; /* Timebase status and control register */
    char r11[2];
    unsigned long tbrefa; /* Timebase reference register A */
    unsigned long tbrefb; /* Timebase reference register A */
    char r12[20];
    unsigned short rtcsc; /* Real-time clock status and control register */
    char r13[2];
    unsigned long rtc; /* Real-time clock register */
    unsigned long rtsec; /* Real-time alarm seconds */
    unsigned long rtcal; /* Real-time alarm register */
    char r14[16];
    unsigned short piscr; /* Periodic interrupt status and control register */
    char r15[2];
    unsigned long pitc; /* Periodic interrupt count register */
    unsigned long pitr; /* Periodic interrupt timer register */
    char r16[52];

    /* Clocks and Reset */
    unsigned long sccr; /* System clock control register */
    unsigned long plprcr; /* PLL, low-power, and reset control register */
    unsigned long rsr; /* Reset status register */
    char r17[116];

    /* System Integration Timers Keys */
    unsigned long tbscrk; /* Timebase status and control register key */
    unsigned long tbrefak; /* Timebase reference register A key */
    unsigned long tbrefbk; /* Timebase reference register B key */
    unsigned long tbk; /* Timebase/decrementer register key */
    char r18[16];
    unsigned long rtcsck; /* Real-time clock status and control register key */
    unsigned long rtck; /* Real-time clock register key */
    unsigned long rtseck; /* Real-time alarm seconds key */
    unsigned long rtcalk; /* Real-time alarm register key */
    char r19[16];
    unsigned long piscrk; /* Periodic interrupt status and control register key */
    unsigned long pitck; /* Periodic interrupt count register key */
    char r20[56];

    /* Clocks and Reset Keys */
    unsigned long sccrk; /* System clock control key */
    unsigned long plprcrk; /* PLL, low power and reset control register key */
    unsigned long rsrk; /* Reset status register key */
    char r21[1236];

    /* I2C Controller */
    unsigned char i2mod; /* I2C mode register */
    char r22[3];
    unsigned char i2add; /* I2C address register */
    char r23[3];
    unsigned char i2brg; /* I2C BRG register */
    char r24[3];
    unsigned char i2com; /* I2C command register */
    char r25[3];
    unsigned char i2cer; /* I2C event register */
    char r26[3];
    unsigned char i2cmr; /* I2C mask register */
    char r27[139];

    /* DMA */
    char r28[4];
    unsigned long sdar; /* SDMA address register */
    unsigned char sdsr; /* SDMA status register */
    char r29[3];
    unsigned char sdmr; /* SDMA mask register */
    char r30[3];
    unsigned char idsr1; /* IDMA1 status register */
    char r31[3];
    unsigned char idmr1; /* IDMA1 mask register */
    char r32[3];
    unsigned char idsr2; /* IDMA2 status register */
    char r33[3];
    unsigned char idmr2; /* IDMA2 mask register */
    char r34[19];

    /* Communications Processor Module Interrupt Control */
    unsigned short civr; /* CPM interrupt vector register */
    char r35[14];
    unsigned long cicr; /* CPM interrupt configuration register */
    unsigned long cipr; /* CPM interrupt pending register */
    unsigned long cimr; /* CPM interrupt mask register */
    unsigned long cisr; /* CPM interrupt in-service register */

    /* Input/Output Port */
    unsigned short padir; /* Port A data direction register */
    unsigned short papar; /* Port A pin assignment register */
    unsigned short paodr; /* Port A open drain register */
    unsigned short padat; /* Port A data register */
    char r36[8];
    unsigned short pcdir; /* Port C data direction register */
    unsigned short pcpar; /* Port C pin assignment register */
    unsigned short pcso;  /* Port C special options register */
    unsigned short pcdat; /* Port C data register */
    unsigned short pcint; /* Port C interrupt control register */
    char r37[6];
    unsigned short pddir; /* Port D data direction register */
    unsigned short pdpar; /* Port D pin assignment register */
    char r38[2];
    unsigned short pddat; /* Port D data register */
    char r39[8];

    /* CPM General-Purpose Timers */
    unsigned short tgcr; /* Timer global configuration register */
    char r40[14];
    unsigned short tmr1; /* Timer 1 mode register */
    unsigned short tmr2; /* Timer 2 mode register */
    unsigned short trr1; /* Timer 1 reference register */
    unsigned short trr2; /* Timer 2 reference register */
    unsigned short tcr1; /* Timer 1 capture register */
    unsigned short tcr2; /* Timer 2 capture register */
    unsigned short tcn1; /* Timer 1 counter */
    unsigned short tcn2; /* Timer 2 counter */
    unsigned short tmr3; /* Timer 3 mode register */
    unsigned short tmr4; /* Timer 4 mode register */
    unsigned short trr3; /* Timer 3 reference register */
    unsigned short trr4; /* Timer 4 reference register */
    unsigned short tcr3; /* Timer 3 capture register */
    unsigned short tcr4; /* Timer 4 capture register */
    unsigned short tcn3; /* Timer 3 counter */
    unsigned short tcn4; /* Timer 4 counter */
    unsigned short ter1; /* Timer 1 event register */
    unsigned short ter2; /* Timer 2 event register */
    unsigned short ter3; /* Timer 3 event register */
    unsigned short ter4; /* Timer 4 event register */
    char r41[8];

    /* Communications Processor (CP) */
    unsigned short cpcr; /* CP command register */
    char r42[2];
    unsigned short rccr; /* RISC controller configuration register */
    char r43[7];
    unsigned short rctr1; /* RISC configuration trap register 1 */
    unsigned short rctr2; /* RISC configuration trap register 2 */
    unsigned short rctr3; /* RISC configuration trap register 3 */
    unsigned short rctr4; /* RISC configuration trap register 4 */
    char r44[2];
    unsigned short rter; /* RISC timer event register */
    char r45[2];
    unsigned short rtmr; /* RISC timer mask register */
    char r46[20];

    /* Baud Rate Generators */
    unsigned long brgc1; /* BRG1 configuration register */
    unsigned long brgc2; /* BRG2 configuration register */
    unsigned long brgc3; /* BRG3 configuration register */
    unsigned long brgc4; /* BRG4 configuration register */

    /* Serial Communications Controllers (SCC 1-4) */
    struct{
        unsigned long gsmr_l; /* SCC general mode register low */
        unsigned long gsmr_h; /* SCC general mode register high */
        unsigned short psmr;
        char r1[2];
        unsigned short todr; /* SCC transmit-on-demand register */
        unsigned short dsr; /* SCC data synchronization register */
        unsigned short scce; /* SCC event register */
        char r2[2];
        unsigned short sccm; /* SCC mask register */
        char r3[1];
        unsigned char sccs; /* SCC status register */
        char r4[8];
    } SCC[4];
    char r47[2];

    /* Serial Management Controller 1 (SMC1) */
    unsigned short smcmr1; /* SMC1 mode register */
    char r48[2];
    unsigned char smce1; /* SMC1 event register */
    char r49[3];
    unsigned char smcm1; /* SMC1 mask register */
    char r50[7];

    /* Serial Management Controller 2 (SMC2) */
    unsigned short smcmr2; /* SMC2 mode register */
    char r51[2];
    unsigned char smce2; /* SMC2 event register */
    char r52[3];
    unsigned char smcm2; /* SMC2 mask register */
    char r53[5];


    /* Serial Peripheal Interface (SPI) */
    unsigned short spmode; /* SPI mode register */
    char r54[4];
    unsigned char spie; /* SPI event register */
    char r55[3];
    unsigned char spim; /* SPI mask register */
    char r56[2];
    unsigned char spcom; /* SPI command register */
    char r57[4];

    /* Parallel Interface Port (PIP and Port B) */
    unsigned short pipc; /* PIP configuration register */
    char r58[2];
    unsigned short ptpr; /* PIP timer parameters register */
    unsigned long pbdir; /* Port B data direction register */
    unsigned long pbpar; /* Port B pin assignment register */
    unsigned long pbodr; /* Port B open drain register */
    unsigned long pbdat; /* Port B data register */
    char r59[24];

    /* Serial Interface (SI) */
    unsigned long simode; /* SI mode register */
    unsigned char sigmr; /* SI global mode register */
    char r60[1];
    unsigned char sistr; /* SI status register */
    unsigned char sicmr; /* SI command register */
    char r61[4];
    unsigned long sicr; /* SI clock route register */
    unsigned long sirp; /* Serial interface RAM pointer register */
    char r62[268];
    unsigned char siram[512]; /* SI routing RAM */
    char r63[4608];

    /* Fast Ethernet Controller (FEC) */
    unsigned long fec_addr_low;
    unsigned long fec_addr_high;
    unsigned long fec_hash_table_high;
    unsigned long fec_hash_table_low;
    unsigned long fec_r_des_start;
    unsigned long fec_x_des_start;
    unsigned long fec_r_buff_size;
    unsigned long fec_ecntrl;
    unsigned long fec_ievent;
    unsigned long fec_imask;
    unsigned long fec_ivec;
    unsigned long fec_r_des_active;
    unsigned long fec_x_des_active;
    unsigned long fec_mii_data;
    unsigned long fec_mii_speed;
    unsigned long fec_r_bound;
    unsigned long fec_r_fstart;
    unsigned long fec_x_wmrk;
    unsigned long fec_x_fstart;
    unsigned long fec_fun_code;
    unsigned long fec_r_cntrl;
    unsigned long fec_r_hash;
    unsigned long fec_x_cntrl;
    char r64[4215];

    /* Dual-Port RAM (DPRAM) */
    unsigned char dpsram[4096]; /* Dual-port system RAM */
    unsigned char dpsrame[3072]; /* Dual-port system RAM expansion */
    unsigned char pram[1024]; /* Dual-port Parameter RAM */
    
} __attribute__((packed));

/* QUICC Buffer Descriptor */
struct qbd{
    unsigned short sc; /* Status/Control */
    unsigned short len; /* Data Length */
    char *buf; /* Buffer Pointer */
} __attribute__((packed));

/* QUICC Buffer Descriptor Status/Control Codes */
#define QBD_OVERRUN      (1 << 1)  /* Overrun */
#define QBD_UNDERRUN     (1 << 1)  /* Underrun */
#define QBD_PARITY       (1 << 3)  /* Parity Error */
#define QBD_FRAME        (1 << 4)  /* Framing Error */
#define QBD_BREAK        (1 << 5)  /* Recieved Break */
#define QBD_IDLE         (1 << 8)  /* Recieved Idle */
#define QBD_PREAMB       (1 << 8)  /* Send Preamble */
#define QBD_CONT         (1 << 9)  /* Continuous Mode */
#define QBD_LAST         (1 << 11) /* Last in message */
#define QBD_INTR         (1 << 12) /* Interrupt */
#define QBD_WRAP         (1 << 13) /* Wrap (Final BD in ring) */
#define QBD_READY        (1 << 15) /* Ready */
#define QBD_EMPTY        (1 << 15) /* Empty */


/* SMC UART PRAM */
struct smc_uart_pram{
    unsigned short rbase; /* RX BD base address offset */
    unsigned short tbase; /* TX BD base address offset */
    unsigned char rfcr; /* RX function code */
    unsigned char tfcr; /* TX function code */
    unsigned short mrblr; /* Maximum receive buffer length */
    unsigned long rstate; /* RX internal state */
    unsigned long ridp; /* RX internal data pointer */
    unsigned short rbptr; /* RX BD pointer */
    unsigned short ribc; /* RX internal byte count */
    unsigned long rtemp; /* RX temp */
    unsigned long tstate; /* TX internal state */
    unsigned long tidp; /* TX internal data pointer */
    unsigned short tbptr; /* TX BD pointer */
    unsigned short tibc; /* TX internal byte count */
    unsigned long ttemp; /* TX temp */
    
    /* UART-Specific PRAM (ref 29-11) */
    unsigned short max_idl; /* Maximum idle characters */
    unsigned short idlc; /* Temporary idle counter */
    unsigned short brklen; /* Last received break length */
    unsigned short brkec; /* Receive break condition counter */
    unsigned short brkcr; /* Break count register */
    unsigned short r_mask; /* Temporary bit mask */
} __attribute__((packed));


extern volatile struct quicc860_struct *quicc;

#endif /* !__LANGUAGE_ASSEMBLY__ */
#endif /* _quicc_h */
