/* Copyright (C) 2006 Kyle Lutz <kyle.r.lutz@gmail.com> */
/* basic string functions */

#include <lib/string.h>

unsigned int strlen(const char *s)
{
    unsigned int len = 0;
    while (s[++len] != '\0')
        ;
    return len;
}

int strcmp(char *s1, char *s2)
{
    while(*s1++ == *s2++){
        if(*s1 == '\0' && *s2 == '\0')
            return 0;
    }
    return -1;
}

int strncmp(char *s1, char *s2, unsigned int n)
{
    while(*s1++ == *s2++ && n-- > 0){
        if((*s1 == '\0' && *s2 == '\0') || n == 0)
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

