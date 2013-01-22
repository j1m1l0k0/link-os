/* Copyright (C) 2006 Kyle Lutz <kyle.r.lutz@gmail.com> */

#include <types.h>
#include <lib/printk.h>
#include <cpu/hwd.h>
#include <cpu/intr.h>
#include <target/quicc.h>
#include <target/target.h>

void berr(void)
{
    volatile char *invalid = (char *) 0x40000000;
    volatile char tmp;
    tmp = *invalid;
    return;
}

void init_target(void)
{
    init_isr(); /* setup isr's */
    init_quicc(); /* setup quicc and uart */
    printk("ramsize = %ik\n",ramsize());

    /* enable interrupts */
    asm("move.w #0x2000, sr"); 

    printk("init_target() complete\n");
    return;
}

