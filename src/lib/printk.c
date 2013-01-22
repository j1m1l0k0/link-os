/* Copyright (C) 2006 Kyle Lutz <kyle.r.lutz@gmail.com> */

#include <types.h>
#include <stdarg.h>
#include <lib/string.h>

void printk(char *fmt, ...);

/* Pointer to a putstr() like function for use by printk() */
static void (*print_function)(char *s) = NULL;

int vsnprintf(char *buf, int size, char *fmt, va_list args)
{
    int i = 0;
    int j = 0;
    char *s;
    char tbuf[128];

    while(i < size && ((buf[i] = *fmt++) != '\0')){
        if(buf[i] == '%'){ /* Formating char */ 
            switch(*fmt++){
                case 'c': /* char */
                    buf[i++] = va_arg(args,int);
                    break;
                case 'i': /* int */
                    i--;
                    itoa(va_arg(args,int),tbuf,10);
                    do {
                        buf[++i] = tbuf[j];                        
                    } while (tbuf[j++] != '\0');
                    j=0;
                    break;
                case 'x': /* hex */
                    i--;
                    itoa(va_arg(args,int),tbuf,16);
                    do {
                        buf[++i] = tbuf[j];                        
                    } while (tbuf[j++] != '\0');
                    j=0;
                    break;
                case 'b': /* binary */
                    i--;
                    itoa(va_arg(args,int),tbuf,2);
                    do {
                        buf[++i] = tbuf[j];                        
                    } while (tbuf[j++] != '\0');
                    j=0;
                    break;
                case 's': /* string */
                    i--;
                    s = va_arg(args,char *);
                    while((buf[++i] = *s++) != '\0');
                    break;               
            }
        }
        else { i++; } /* Normal char */
    }
    buf[i] = '\0'; /* terminate the string */
    return i;
} 


void printk(char *fmt, ...)
{
    va_list args;
    char sbuf[1024]; /* final string for printing goes here */
    va_start(args,fmt);
    vsnprintf(sbuf,sizeof(sbuf),fmt,args);
    va_end(args);
    print_function(sbuf); /* print the string */
    return;
}

int snprintf(char *buf, int size, char *fmt, ...)
{
    va_list args;
    va_start(args,fmt);
    return vsnprintf(buf,size,fmt,args);
}

int register_printk(void (*func)(char *s))
{
    print_function = func;
    return 0;
}
