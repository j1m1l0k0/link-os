#ifndef _thread_h
#define _thread_h

#include <fs.h>
#include <cpu/context.h>

/* thread states */
#define THREAD_RUN      0x00 /* currently running */
#define THREAD_READY    0x01 /* ready to run */
#define THREAD_BLOCKED  0x02 /* waiting/blocked */

/* Size (in bytes) of each threads stack */
#define THREAD_STACK_SIZE 1024

struct thread_struct{
    struct sys_context ctx;
    unsigned int id;
    unsigned char state;
    unsigned char *stack; 
    struct tty_struct *tty;

    // thread waiting for an event from this one.
    // this will change to become more robust, but 
    // it works for now.
    unsigned int waiting_thread; 

} __attribute__((packed));

extern volatile struct thread_struct *cthread;
extern volatile unsigned char reschedule_flag;
#define reschedule() reschedule_flag++

extern struct thread_struct *create_thread(void);
extern int destroy_thread(unsigned int id);
extern void wake_thread(unsigned int id);
extern void block_thread(void);
extern struct thread_struct *get_thread(unsigned int id);
extern void start_threads(void);

#endif /* _thread_h */
