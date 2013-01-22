/* Copyright (C) 2006 Kyle Lutz <kyle.r.lutz@gmail.com> */

#include <cpu/intr.h>
#include <target/target.h>

/* Forward Declarations */
static long probe_memory(long base, long range);

/*===========================================================================*/
/*  Exported Functions                                                       */
/*===========================================================================*/

/* Return amount of RAM in bytes */
long ramsize(void)
{
    static long size = 0;
    if(!size){
        size = probe_memory(RAM_BASE,RAM_MAX_SIZE);
    }
    return size;
}


/*===========================================================================*/
/*  Internal Functions                                                       */
/*===========================================================================*/

void hwd_berr_isr(void); /* from asm.S */
volatile unsigned char hwd_bus_error_fired = 0; /* flag */

/* Return amount of memory (in KB) between base and base + range (in bytes) */
static long probe_memory(long base, long range)
{
    int i = 1024; /* Increment */ 
    long kb;
    long *addr;
    volatile char tmp;
    long *israddr;
    long saved_isr;
    extern long EVT_BASE;
   
    israddr = (long *) (EVT_BASE + (2*4));
    saved_isr = *israddr; 

    hwd_bus_error_fired = 0; 
    *israddr = (long) hwd_berr_isr;
    for (kb = 0; kb <= range; kb += i){
        addr = (long *)(base + (kb * i));
        tmp = *addr;
        if (hwd_bus_error_fired){
            break;
        }
    }
    *israddr = saved_isr;
    return kb;
}
