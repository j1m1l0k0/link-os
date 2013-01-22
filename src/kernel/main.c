/*  Copyright (C) 2007 Kyle Lutz <kyle.r.lutz@gmail.com> */

#include <fs.h>
#include <rdfs.h>
#include <exec.h>
#include <types.h>
#include <config.h>
#include <thread.h>
#include <module.h>
#include <lib/kmem.h>
#include <lib/util.h>
#include <lib/printk.h>
#include <target/target.h>


#ifdef TARGET_IS_c1700
static char buf[64] = "buffer\n";
static unsigned int len = 8;

void test_syscall(void)
{
    asm("li r0, 0"); /* call number 1 */
    asm("li r3, 0"); /* stdout */
    asm("lis r4,%0@h"::"g"(buf));
    asm("la r4, %0@l(4)"::"g"(buf));
    asm("lwz r5, %0"::"g"(len));
    asm("sc");

    printk("done with syscall\n");
}

#endif

//struct thread_struct cthread_space;
//struct thread_struct *tptr = &cthread_space;

extern const void __heap, __etext;

int func1(void)
{
    printk("func1()\n");
    return 0;
}


void init_kernel(void)
{
    print_booting_message();

    // __heap -> RAM_BASE + ramsize() 
    kmem_init((long)&__heap, ((long)&__heap + 2*MB));
    fs_init();
    module_init();

    //dump_ksyms();

    exec("sh",NULL,NULL);

    start_threads();

    /* we should not get down here */
    printk("start_threads() returned\n");
    for(;;);
}
