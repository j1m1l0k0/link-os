#include <fs.h>
#include <lib/printk.h>
#include <target/intr.h>
#include <target/pc16552.h>

/* Datasheet: NS16C552.pdf */

static volatile struct pc16552_regs *uart = (struct pc16552_regs *) PC16552_ADDR;

/* LSR - Line Status Register */
#define DATA_RDY    (1<<0) /* Data Ready */
#define OVER_ERR    (1<<1) /* Overrun Error */
#define PARITY_ERR  (1<<2) /* Parity Error */
#define FRAME_ERR   (1<<3) /* Framing Error */
#define BREAK_INTR  (1<<4) /* Break Interrupt */
#define THRE        (1<<5) /* TX Holding Register Empty */
#define TEMT        (1<<6) /* Transmiter Empty */

/* IIR - Interrupt ID Register */ 
#define IIR_NONE  0x01 /* None */
#define IIR_RLS   0x06 /* RX Line Status Change */
#define IIR_RXDA  0x04 /* RX Data Available */
#define IIR_CHT   0x0C /* Character Timeout */
#define IIR_THRE  0x02 /* TX Holding Register Empty */
#define IIR_MODEM 0x00 /* MODEM Status change */

/* IER - Interrupt Enable Register */
#define IER_RXDA  (1<<0) /* RX Data Available */
#define IER_THRE  (1<<1) /* TX Holding Register Empty */
#define IER_RLS   (1<<2) /* RX Line Status */
#define IER_MODEM (1<<3) /* MODEM Status */

/* Send a character out the serial port */
static void putch(char c)
{
    while(!(uart->lsr & THRE))
        /* wait */; 
    uart->thr = c;
}

static void putstr(char *s)
{
    do{
        putch(*s);
        if (*s == '\n'){ putch('\r'); }
    } while(*s++ != '\0');
}

volatile char inbuf[255];
volatile unsigned char cnt;

static void pc16552_isr(void)
{
    unsigned char iir; /* Interrupt ID register */
    iir = (uart->iir & 0x0f);

    /* Line Status changed */
    if(iir == IIR_RLS){
        printk("i_rls\n");
    }

    /* Recieved a char */
    else if(iir & IIR_RXDA){
        char c = uart->rhr;

        inbuf[cnt++] = c;
        if(c == '\r'){
            inbuf[cnt] = '\0';
            printk("\nrx = %s\n",inbuf);
            cnt = 0;
        }
        putch(c);
    }

    /* Unknown interrupt */
    else{
        printk("unhandled uart interrupt iir=%i\n",iir);
    }
}

void init_uart(void)
{
    register_printk(&putstr);

    /* Set up our interrupt service routine */
    reqisr(2,pc16552_isr);
    uart->ier = IER_RXDA | IER_RLS;

    dprintk("pc16552 uart initialized\n");
    return;
}

/**********************
 * console tty driver *
 **********************/

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
    (void) buf;
    (void) len;
    return 0;
}

struct tty_struct console_tty = {
    .name = "console",
    .write = console_write,
    .read = console_read,
};
