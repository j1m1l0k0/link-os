/* kernel memory management */

#include <types.h>
#include <lib/string.h>
#include <lib/printk.h>

static unsigned long low = 0;
static unsigned long max = 0;


void *kmalloc(unsigned int size)
{
    void *ret = NULL;
    size += (size%4); /* nice even addresses */
    if((low + size) <= max){
        ret = (void *) low;
        low += size;
    }
    else{
        printk("fatal: out of memory\n");
        for(;;);
    }

    // zero out the memory
    memset(ret,0,size);
    return ret;
}

void *kmallocz(unsigned int size)
{
    void *ptr = kmalloc(size);
    if(ptr != NULL)
        memset(ptr,0,size);
    return ptr;
}

void kfree(void *memory)
{
    (void) memory;
    return;
}

int kmem_init(unsigned long start, unsigned long end)
{
    low = start;
    max = end;
    printk("kernel memory manager initialized (0x%x -> 0x%x)\n",low,max);
    return 0;
}

