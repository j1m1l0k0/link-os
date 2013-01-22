/* Copyright (C) 2006 Kyle Lutz <kyle.r.lutz@gmail.com> */


#include <lib/printk.h>
#include <cpu/hwd.h>
#include <cpu/intr.h>
#include <target/target.h>
#include <target/scn2681.h>

void init_target(void)
{
    init_isr();
    init_uart();

    printk("ramsize = %ik\n",ramsize());
    asm("move.w #0x2000, sr");
    printk("init_target() complete\n");
}

