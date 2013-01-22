/* Copyright (C) 2006 Kyle Lutz <kyle.r.lutz@gmail.com> */
#ifndef _target_h
#define _target_h

#include <types.h>

#define TARGET_IS_CISCO

/* TARGET_MODEL */
#ifdef TARGET_IS_c1000
#define TARGET_STR "c1000"
#endif /* TARGET_IS_c1000 */
#ifdef TARGET_IS_c1600
#define TARGET_STR "c1600"
#endif /* TARGET_IS_c1600 */


#define CPU_STR "M68360"
#define CPU_IS_68360
#define CPU_IS_68k

/* ENTRY_POINT */
#ifdef TARGET_IS_c1000
#define ENTRY_POINT 0x02004000
#endif /* TARGET_IS_c1000 */
#ifdef TARGET_IS_c1600
#define ENTRY_POINT 0x02005000
#endif /* TARGET_IS_c1600 */


/* Memory Map */
#ifdef TARGET_IS_c1000
#define ROM_BASE    0x00000000
#define RAM_BASE    0x02000000
#define FLASH_BASE  0x08000000
#define NVRAM_BASE  0x0E000000
#define RAM_MAX_SIZE (16*MB)
#endif /* TARGET_IS_c1000 */
#ifdef TARGET_IS_c1600
#define ROM_BASE    0x00000000
#define RAM_BASE    0x02000000
#define FLASH_BASE  0x08000000
#define NVRAM_BASE  0x0E000000
#define RAM_MAX_SIZE (16*MB)
#endif /* TARGET_IS_c1600 */


/* Macro Functions */

/* Cisco sets up trap 0 to go to rommon */
#define start_rommon() asm("trap #0\n\t")





#endif /* _target_h */
