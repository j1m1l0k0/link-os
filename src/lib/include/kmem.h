#ifndef _kmem_h
#define _kmem_h

extern void *kmalloc(unsigned int size);
extern void *kmallocz(unsigned int size);
extern void kfree(void *memory);
extern int kmem_init(unsigned long start, unsigned long end);

#endif /* _kmem_h */
