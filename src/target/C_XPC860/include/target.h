#ifndef _target_h
#define _target_h

#define TARGET_IS_CISCO

#ifdef TARGET_IS_c1700
#define TARGET_STR "c1700"
#endif
#ifdef TARGET_IS_c2600
#define TARGET_STR "c2600"
#endif

#define CPU_STR "XPC860"

#define CPU_IS_PPC

/* Memory Map */
#define RAM_BASE    0x00000000
#define RAM_MAX     0x027FFFFF
#define FLASH_BASE  0x60000000


#endif /* _target_h */
