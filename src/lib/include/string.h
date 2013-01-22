/* Copyright (C) 2006 Kyle Lutz <kyle.r.lutz@gmail.com> */

#ifndef _string_h
#define _string_h

#include <types.h>

extern unsigned int strlen(const char *s);
extern int strcmp(char *s1, char *s2);
extern int strncmp(char *s1, char *s2, unsigned int n);
extern char *strcpy(char *dst, char *src);
extern void strev(char *s);
extern int itoa(int i, char *str, int base);

extern void memcpy(void *dst, void *src, unsigned int size);
extern void bcopy(void *dst, void *src, unsigned int size);
extern void memset(void *dst, char c, unsigned int size);


#endif /* _string_h */
