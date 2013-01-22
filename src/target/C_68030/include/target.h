/* Copyright (C) 2006 Kyle Lutz <kyle.r.lutz@gmail.com> */

#ifndef _target_h
#define _target_h

#include <types.h>

#define TARGET_IS_c2500
#define TARGET_STR "c2500"

#define TARGET_IS_CISCO

#define CPU_STR "M68020"
#define CPU_IS_68k

#define RAM_BASE    0x00000000
#define RAM_MAX_SIZE (16*MB)


#endif /* _target_h */
