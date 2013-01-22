#include <thread.h>
#include <types.h>
#include <lib/kmem.h>
#include <lib/string.h>
#include <lib/printk.h>

/* currently executing thread */
volatile struct thread_struct *cthread = NULL;

/* pointer to system context (only available during an interrupt) */
volatile struct sys_context *__ctx = NULL;

/* flag stating if we need to reschedule */
volatile unsigned char reschedule_flag = 0;

static unsigned int nthreads = 0; /* number of threads */
static struct thread_struct *threads[10];


struct thread_struct *create_thread(void)
{
    struct thread_struct *t = kmalloc(sizeof(struct thread_struct));

    threads[nthreads++] = t;

    memset(t,0,sizeof(struct thread_struct));
    t->id = nthreads;
    t->stack = kmalloc(THREAD_STACK_SIZE);

    // for now all threads have the console as their tty
    extern struct tty_struct console_tty;
    t->tty = &console_tty;

    return t;
}

int destroy_thread(unsigned int id)
{
    struct thread_struct *t = get_thread(id);

    if(t == NULL)
    {
        printk("tried to destory an invalid thread. id = %i\n",id);
        return -1;
    }

    /* notify listeners */
    if(t->waiting_thread != 0)
        wake_thread(t->waiting_thread);

    // kfree(threads[id])

    threads[id-1] = NULL;
    nthreads--;
    return 0;
}

struct thread_struct *get_thread(unsigned int id)
{
    return threads[id-1];
}

void wake_thread(unsigned int id)
{
    if(threads[id-1] != NULL)
        threads[id-1]->state = THREAD_READY;
    reschedule();
    return;
}

/* block_thread() & preempt_thread() */
void block_thread(void)
{
    cthread->state = THREAD_BLOCKED;
    memcpy((void *) &cthread->ctx, (void *) __ctx, sizeof(struct sys_context)); /* save threads context */
    reschedule();
    return;
}

/* @ctx - the system context of current thread */
void sched_thread(struct sys_context ctx)
{
    (void) ctx;

    unsigned int i;
    struct thread_struct *t = NULL; /* next thread to run */

    //printk("sched()ing\n");

    /* find which thread to run. this also serves as our 'idle thread' as it will
     * keep looping until a thread becomes runnable.
     * */
    while(t == NULL){
        /* look through all threads and try to find a ready one */
        for(i=0;i<nthreads;i++){
            if(threads[i] && threads[i]->state == THREAD_READY){ 
                t = threads[i];
                //printk("t:%i ",t->id);
                break;
            }
        }
    }

    /* switch the thread's context */
    memcpy((void *) &ctx, (void *) &t->ctx, sizeof(struct sys_context));
    cthread = t;

    reschedule_flag = 0;

    return;
}

int thread_init(void)
{
    return 0;
}

void start_threads(void)
{

    /* we start off with the very first thread */
    cthread = threads[0];

    if(cthread == NULL){
        printk("no threads to start\n");
        return;
    }
    
    printk("jumping to thread %i; entry_point: 0x%x\n\n",cthread->id,cthread->ctx.pc);
    CPU_JUMP(cthread->ctx.pc);

    /* we never get here */
}

