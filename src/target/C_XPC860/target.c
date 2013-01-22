#include <types.h>
#include <lib/printk.h>
#include <target/quicc.h>
#include <target/pc16552.h>
#include <target/target.h>
#include <target/intr.h>

volatile struct quicc860_struct *quicc = (struct quicc860_struct *) QUICC_BASE;

#if 0
// generate a machine check exception
void mkerr(void)
{
    volatile char *ptr = (char *) 0x51001234;
    volatile char tmp;
    tmp = *ptr;
    return;
}
#endif

void init_quicc(void)
{
    return;
}



void init_target(void)
{
    init_isr();
    init_uart();
    init_quicc();

    /* enable interrupts */
    asm("mtspr 80, 0");

    dprintk("init_target() complete\n");
    return;
}
