#ifndef _sys_h
#define _sys_h

#define NULL ((void *) 0)

#define print(s) write(0,s,strlen(s))

extern unsigned int strlen(const char *s);
extern int write(int fd, char *buf, int len);
extern int read(int fd, char *buf, int len);
extern unsigned int exec(char *name, char **argv, char **envp);
extern int getline(char *buf);
extern int waitfor(unsigned int id);

extern int printf(char *fmt, ...);
extern void *memset(void *dst, int c, unsigned int size);

#endif /* _sys_h */
