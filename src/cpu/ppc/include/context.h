#ifndef _context_h
#define _context_h

#define CPU_JUMP(addr) do{ asm volatile("mtspr CTR, %0"::"r"(addr)); \
                           asm volatile("bctrl");} while(0)


struct sys_context{
    union{
        long r0;
        long ret;
    };
    union{
        long r1;
        long sp;
    };
    long r2,r3,r4,r5,r6,r7,r8,r9,r10;
    long r11,r12,r13,r14,r15,r16,r17;
    long r18,r19,r20,r21,r22,r23,r24;
    long r25,r26,r27,r28,r29,r30,r31;

    long pc;
} __attribute__((packed));

#endif /* _context_h */
