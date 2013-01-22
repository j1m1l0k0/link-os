#ifndef _printk_h
#define _printk_h

#include <types.h>
#include <config.h>
#include <stdarg.h>

extern void printk(char *fmt, ...);
extern int snprintf(char *buf, int size, char *fmt, ...);
extern int vsnprintf(char *buf, int size, char *fmt, va_list args);
extern int register_printk(void (*func)(char *s));

/* version of printk() only called when DEBUG is enabled */
#if DEBUG
#define dprintk(fmt, args...) printk(fmt,##args)
#else
#define dprintk(fmt, args...) (void) 0
#endif

#endif /* _printk_h */
