/*  Copyright (C) 2007 Kyle Lutz <kyle.r.lutz@gmail.com> */

static volatile unsigned long ticks = 0;

void clock_isr(void)
{
    ticks++;
}

unsigned long get_ticks(void)
{
    return ticks;
}

