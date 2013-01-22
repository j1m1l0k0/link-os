/* System Calls */

#include <fs.h>
#include <exec.h>
#include <clock.h>
#include <thread.h>
#include <lib/printk.h>
#include <lib/string.h>

/* system call handler functions */


int sys_null(void)
{
    printk("null syscall\n");
    return 0;
}

int sys_write(unsigned int fd, char *buf, unsigned int len)
{
    if(fd < 3 && cthread->tty != NULL)
        cthread->tty->write(buf,len);
    else
        printk("write called, fd %i buf at 0x%x len %i\n",fd,buf,len);
    return 0;
}

int sys_read(unsigned int fd, char *buf, unsigned int len)
{
    if(fd < 3 && cthread->tty != NULL)
        return cthread->tty->read(buf,len);
    else
        return do_read(fd,buf,len);
}

int sys_exit(int status)
{
    (void) status;
    //printk("thread %i exited. status %i\n",cthread->id,status);
        
    destroy_thread(cthread->id);

    /* schedule next thread */
    reschedule();

    return 0;
}

int sys_test(struct sys_context ctx)
{
    (void) ctx;

#if 0
    printk("\nd0 %i d1 %i d2 %i d3 %i d4 %i d5 %i d6 %i d7 %i\n",
            ctx.d0,ctx.d1,ctx.d2,ctx.d3,ctx.d4,ctx.d5,ctx.d6,ctx.d7);
    printk("a0 %i a1 %i a2 %i a3 %i a4 %i a5 %i a6 %i a7 %i\n",
            ctx.a0,ctx.a1,ctx.a2,ctx.a3,ctx.a4,ctx.a5,ctx.a6,ctx.a7);
#endif

    return 0;
}

unsigned int sys_exec(char *name, char **argv, char **envp)
{
    unsigned int id; /* thread id of new thread */
    id = exec(name,argv,envp);
    return id;
}

int sys_waitfor(unsigned int id)
{
    //printk("%i waiting for %i\n",cthread->id, id);

    if(get_thread(id) != NULL)
       get_thread(id)->waiting_thread = cthread->id;
    else{
        printk("invalid thread %i\n",id);
        return -1;
    }

    block_thread();

    return 0;
}

int sys_open(char *name, int flags)
{
    return do_open(name,flags);
}

unsigned long sys_ticks(void)
{
    return get_ticks();
}

void *__syscall_table[] = {
    sys_null,       /* 0 */
    sys_write,      /* 1 */
    sys_read,       /* 2 */
    sys_exit,       /* 3 */
    sys_test,       /* 4 */
    sys_exec,       /* 5 */
    sys_waitfor,    /* 6 */
    sys_open,       /* 7 */
    sys_ticks,      /* 8 */
};

/* We've recieved and invalid system call */
void syscall_invalid(int callnr)
{
    printk("invalid system call -- callnr %i\n",callnr);
    if(cthread != NULL){
        printk("thread id %i\n",cthread->id);

        /* kill the task */
        destroy_thread(cthread->id);
        reschedule();
    }
    return;
}

