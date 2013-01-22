/* Copyright (C) 2006 Kyle Lutz <kyle.r.lutz@gmail.com> */

#include <fs.h>
#include <clock.h>
#include <cpu/intr.h>
#include <lib/printk.h>
#include <target/scn2681.h>

static volatile struct scn2681_regs *uart = (struct scn2681_regs *) SCN2681_ADDR;

/* Status Flags */
#define TX_RDY   0x04  /* Ready to transmit */

/*************************
 * console i/o functions *
 *************************/

static void putch(char c)
{
    while(!(uart->sra & TX_RDY))
        /* Wait for it to be ready */; 
    uart->thra = c; /* Send char */
}

/* Send a string to the console */
static void uart_putstr(char *s)
{
    do{
        putch(*s);
        if (*s == '\n'){ putch('\r'); } 
    } while(*s++ != '\0');
}

/**********************
 * console tty driver *
 **********************/

static volatile char *tty_ibuf = NULL;
static volatile unsigned char reading = 0;

/* interrupt mask */
#define INTR_MASK (SIM_RXA | SIM_CTR)

void scn2681_isr(void)
{
    char tmp;
    unsigned char event = (uart->isr & INTR_MASK);

    /* recieve a character */
    if(event & SIM_RXA){
        if(reading && tty_ibuf){
            *tty_ibuf = uart->rhra;
            reading = 0;
        }
        else
            tmp = uart->rhra;
    }

    /* counter fired -- clock tick */
    if(event & SIM_CTR){
       //printk("clock fired\n");
       clock_isr();
       tmp = (char) uart->stopc;
    }

    return;
}

asm("                           \
_scn2681_interrupt:             \
    movem.l d0-d7/a0-a7, sp@-;  \
                                \
    jsr _scn2681_isr;           \
                                \
    movem.l sp@+, d0-d7/a0-a7;  \
    rte;");

static int console_write(char *buf, unsigned int len)
{
    int i = 0;
    while(len--){
        if(buf[i] == '\n')
            putch('\r');
        putch(buf[i++]);
    }
    return i;
}

static int console_read(char *buf, unsigned int len)
{
    (void) len;

    tty_ibuf = buf;
    reading = 1;
    while(reading)
        ;

    return 1;
}

struct tty_struct console_tty = {
    .name = "console",
    .write = console_write,
    .read = console_read,
};


void init_uart(void)
{
    unsigned short tmp;

    uart->imr = 0x00; /* disable interrupts from uart */
    reqisr(29,scn2681_interrupt);

    /* set up the clock */
    uart->acr = (7 << 4); /* clk / 16 */
    tmp = uart->startc;

    /* n = (counter_clock_frequency / (16 * 2 * baud_rate_desired)) */

    uart->imr = INTR_MASK;

    register_printk(&uart_putstr);
    printk("scn2681 uart initialized\n");
    return;
}

