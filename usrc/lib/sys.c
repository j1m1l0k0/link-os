#include "../include/sys.h"
#include "stdarg.h"

#define NULL ((void *) 0)

unsigned int strlen(const char *s)
{
    unsigned int len = 0;
    while (s[++len] != '\0')
        ;
    return len;
}

int open(char *name, int flags)
{
    int ret;
    asm("move.l #7, %d0");
    asm("move.l %0, %%d1"::"g"(name));
    asm("move.l %0, %%d2"::"g"(flags));
    asm("trap #8":::"d0");
    asm("move.l %%d0, %0":"=g"(ret));
    return ret;
}

int write(int fd, char *buf, int len)
{
    int ret;
    asm("move.l #1, %d0");
    asm("move.l %0, %%d1"::"g"(fd));
    asm("move.l %0, %%d2"::"g"(buf));
    asm("move.l %0, %%d3"::"g"(len));
    asm("trap #8":::"d0");
    asm("move.l %%d0, %0":"=g"(ret));
    return ret;
}

int read(int fd, char *buf, int len)
{
    int ret;
    asm("move.l #2, %d0");
    asm("move.l %0, %%d1"::"g"(fd));
    asm("move.l %0, %%d2"::"g"(buf));
    asm("move.l %0, %%d3"::"g"(len));
    asm("trap #8":::"d0");
    asm("move.l %%d0, %0":"=g"(ret));
    return ret;
}

unsigned int exec(char *name, char **argv, char **envp)
{
    int ret;
    asm("move.l #5, %d0");
    asm("move.l %0, %%d1"::"g"(name));
    asm("move.l %0, %%d2"::"g"(argv));
    asm("move.l %0, %%d3"::"g"(envp));
    asm("trap #8":::"d0");
    asm("move.l %%d0, %0":"=r"(ret));
    return ret;
}

int waitfor(unsigned int id)
{
    int ret;
    asm("move.l #6, %d0");
    asm("move.l %0, %%d1"::"g"(id));
    asm("trap #8":::"d0");
    asm("move.l %%d0, %0":"=g"(ret));
    return ret;
}

unsigned long ticks(void)
{
    unsigned long ret;
    asm("move.l #8, %d0");
    asm("trap #8":::"d0");
    asm("move.l %%d0, %0":"=g"(ret));
    return ret;
}

int sys_test(void)
{
    int ret;
    asm("move.l #4, %d0");
    asm("trap #8":::"d0");
    asm("move.l %d0, %0":"=g"(ret));
    return ret;
}

int getline(char *buf)
{
    int i = 0;
    char ch;
    while(1){
        read(0,&ch,1);
        write(1,&ch,1);
        if(ch == '\r'){
            ch = '\n';
            write(1,&ch,1);
            break;
        }
        buf[i++] = ch;
    }
    buf[i] = '\0';
    return i;
}



/* printf and str*() functions */
int strcmp(char *s1, char *s2)
{
    while(*s1++ == *s2++){
        if(*s1 == '\0' && *s2 == '\0')
            return 0;
    }
    return -1;
}

char *strcpy(char *dst, char *src)
{
    char *tmp = dst;
    while((*dst++ = *src++) != '\0')
        ;
    return tmp;
}


/* Swap two char's */
static inline void swap(char *a, char *b)
{
    char tmp = *a;
    *a = *b;
    *b = tmp;
}

int itoa(int i, char *str, int base)
{
    int c = 0; /* current index in str */
    int t;
    char tmp[64];

    /* some sanity checks */
    if (i < 0){
        i *= -1;
    }
    if( (base < 2) || (base > 36)){ 
        return -1; 
    }

    do{
        t = (i % base);
        if (t < 10) { tmp[c++] = (t + 48); } /* number */
        else { tmp[c++] = (t + 87); } /* lowercase hex char */
        /* else { s[c++] = (t + 55); } uppercase hex char */
        i /= base;
    } while (i > 0);
    tmp[c--] = '\0'; /* terminate the string and set c to strlen(tmp) */ 

    /* reverse the string */
    t = 0;
    while(t < c)
        swap(&tmp[t++],&tmp[c--]); 
    
    strcpy(str,tmp);
    return 0;
}

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


int printf(char *fmt, ...)
{
    va_list args;
    char sbuf[1024]; /* final string for printing goes here */
    va_start(args,fmt);
    vsnprintf(sbuf,sizeof(sbuf),fmt,args);
    va_end(args);
    print(sbuf); /* print the string */
    return 0;
}

int snprintf(char *buf, int size, char *fmt, ...)
{
    va_list args;
    va_start(args,fmt);
    return vsnprintf(buf,size,fmt,args);
}

void *memcpy(void *dst, void *src, unsigned int size)
{
    unsigned char *d = dst;
    unsigned char *s = src;
    while(size--){
        *d++ = *s++;
    }
    return dst;
}



void *memset(void *dst, int c, unsigned int size)
{
    unsigned char *d = dst;
    while(size--){
        *d++ = c;
    }
    return dst;
}
