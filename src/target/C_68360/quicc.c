/* Copyright (C) 2006 Kyle Lutz <kyle.r.lutz@gmail.com> */
/* Code for the Quicc SMC uart on cisco 68360 targets */

/* messy, clean this up */

#include <fs.h>
#include <thread.h>
#include <cpu/intr.h>
#include <lib/kmem.h>
#include <lib/printk.h>
#include <lib/string.h>
#include <target/quicc.h>

volatile struct quicc360_struct *quicc = (struct quicc360_struct *) QUICC_BASE;
volatile struct smc_uart *uart = (volatile struct smc_uart *) SMC1_BASE;

/**************************************
 * initial console i/o (for printk) *
 **************************************/

static void uart_putchar(char c)
{
    volatile struct qbd *txbd = (struct qbd *)(QUICC_BASE + uart->tbptr);
    txbd->buf = &c;
    txbd->len = 1;
    txbd->sc |= BD_READY;
    while(txbd->sc & BD_READY); /* Wait until its done TX */
}

static void uart_putstr(char *s)
{
    do {
        uart_putchar(*s);
        if (*s == '\n'){ uart_putchar('\r'); }
    } while(*s++ != '\0');
}


/****************
 * quicc driver *
 ****************/

/* QUICC CPM interrupts
 *
 * the cpm is set up in init_quicc() to use interrupt vectors 96-112
 * for all the cpm interrupts. A handler can be assigned to one of the 
 * cpm interrupts with the quicc_reqisr() function. 
 */

extern void quicc_interrupt(void);

#define QUICC_ISR_BASE 96
#define QUICC_ISR_MAX  128
#define NUM_QUICC_ISR  32
void *__quicc_isr_table[NUM_QUICC_ISR];

int quicc_reqisr(unsigned int vector, void (*handler)(void))
{
    if(vector >= NUM_QUICC_ISR)
        return -1;
    if(handler == NULL)
        return -1;

    __quicc_isr_table[vector] = handler;
    quicc->cimr |= (1 << vector);

    return 0;
}

void quicc_isr(struct sys_context ctx)
{
    void (*handler)(void);
    handler = __quicc_isr_table[((ctx.vec/4)-QUICC_ISR_BASE)];
    handler();
    return;
}

void quicc_interrupt(void);
asm("                           \
_quicc_interrupt:               \
    movem.l d0-d7/a0-a7, sp@-;  \
                                \
    jsr _quicc_isr;             \
                                \
    movem.l sp@+, d0-d7/a0-a7;  \
    rte;");

void quicc_clock_interrupt(void);
asm("                           \
_quicc_clock_interrupt:         \
    movem.l d0-d7/a0-a7, sp@-;  \
                                \
    jsr _reset_watchdog;        \
    jsr _clock_isr;             \
                                \
    movem.l sp@+, d0-d7/a0-a7;  \
    rte;");

void reset_watchdog(void)
{
    quicc->swsr = 0x55;
    quicc->swsr = 0xAA;
    return;
}



/**********************
 * console tty driver *
 **********************/

//#define IBUF_SIZE 32
//static char ibuf[IBUF_SIZE]; /* input buffer */
//static char *ibuf_head, *ibuf_tail;

static volatile char ibuf;
static volatile char *tty_ibuf = NULL;
static volatile unsigned int reading = 0;

static void uart_isr(void)
{
    unsigned char event = quicc->SMC[0].smce;

    if(event & SMCE_RX){
        volatile struct qbd *bd = (struct qbd *) (QUICC_BASE + uart->rbptr);
        if(reading && tty_ibuf){
            *tty_ibuf = *bd->buf;
            wake_thread(reading);
            get_thread(reading)->ctx.ret = 1; /* return value, 1 char read */
            reading = 0;
        }
        bd->sc |= BD_EMPTY;
    }

    else{
        printk("unknown uart interrupt %i\n",event);
    }

    quicc->SMC[0].smce = 0xFF; /* Reset all uart interrupts */
    quicc->cisr |= CIMR_SMC1; /* Clear interrupt in CISR */
}

static int console_write(char *buf, unsigned int len)
{
    int i = 0;
    while(len--){
        if(buf[i] == '\n')
            uart_putchar('\r');
        uart_putchar(buf[i++]);
    }
    return i;
}

static int console_read(char *buf, unsigned int len)
{
    (void) len;
    
    tty_ibuf = buf;
    
    reading = cthread->id;
    block_thread();

    //printk("console read finished tty_ibuf = %c\n",*tty_ibuf);
    
    /* this return value doesn't matter, the actual return value is set
     * by uart_isr(). */
    return 0;
}

struct tty_struct console_tty = {
    .name = "console",
    .write = console_write,
    .read = console_read
};


/* initialize the /dev/console and (if available) /dev/aux devices. */
int init_console(void)
{
    /* uart interrupts and buffer descriptors */
    quicc_reqisr(4,uart_isr);

    quicc->SMC[0].smcmr ^= 1; /* disable rx */
    uart->mrblr = 1;
    uart->max_idl = 0;
    //quicc->SMC[0].smcmr = 0x4820;


    uart->rbase = 1024;
    CR_EXEC(CR_INITRX,CR_SMC1);
    volatile struct qbd *bd = (struct qbd *) (QUICC_BASE + uart->rbase);
    bd->len = 0;
    bd->buf = (char *) &ibuf;
    bd->sc |= (BD_EMPTY | BD_INTR | BD_WRAP);

    quicc->SMC[0].smce = 0xff;
    quicc->SMC[0].smcm = 0x17;
    quicc->SMC[0].smcmr |= (1 << 0); /* RX Enable */
    quicc->SMC[0].smcmr |= (1 << 1); /* TX Enable */

    return 0;
}


/****************
 * init_quicc() *
 ****************/

void init_quicc(void)
{
    int i;
    register_printk(&uart_putstr);
    printk("quicc uart [smc1] initialized\n");

    /* set up our interrupt handler */
    for(i=96;i<112;i++)
        reqisr(i,quicc_interrupt);

    /* set up quicc interrupt controller */
    quicc->cimr = 0x00; /* mask all cpm interrupts */
    quicc->cicr |= (4 << 13); /* interrupt level 4 */
    quicc->cicr |= (3 << 5); /* vector base (3 == interrupts 96-112) */

    /* clock */
    reqisr(68,quicc_clock_interrupt);
    quicc->PICR.vec = 68;
    quicc->PICR.irl = 1;
    
    init_console();

    return;
}
