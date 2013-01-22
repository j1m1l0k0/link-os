/* Copyright (C) 2006 Kyle Lutz <kyle.r.lutz@gmail.com> */

#ifndef _context_h
#define _context_h

#define CPU_JUMP(entry)   asm("move.l %0, a0\n\t"\
                              "jsr (a0)"::"m"(entry))

/* SET_SP() -- Set Stack Pointer for task */
#ifdef TARGET_IS_c2500
#define SET_SP(addr)      asm("move.l %0, a0\n\t" \
                              "move.l a0, ssp"::"g"(addr))
#else
#define SET_SP(addr)      asm("move.l %0, a0\n\t" \
                              "move.l a0, usp"::"g"(addr))
#endif


struct sys_context {
    /* Registers */
    union{
        long d0;
        long ret;
    };
    long d1,d2,d3,d4,d5,d6,d7;
    long a0,a1,a2,a3,a4,a5,a6;
    union{
        long a7;
        long sp;
    };

    /* Exception Stack Frame */
    short sr;
    long pc;
    unsigned fmt :4;
    unsigned vec :12;
} __attribute__((packed));

extern struct sys_context *context;

#endif /* _context_h */
