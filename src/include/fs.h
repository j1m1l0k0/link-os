#ifndef _fs_h
#define _fs_h

extern int fs_init(void);
extern int do_open(char *name, int flags);
extern int do_read(int fd, char *buf, unsigned int len);


struct file_struct{
    unsigned char type;
    char *name;
    int (*open)(void);
    int (*write)(struct file_struct *f, char *buf, unsigned int len);
    int (*read)(struct file_struct *f, char *buf, unsigned int len);
    int (*close)(void);

    unsigned long fpos; /* position in file */

    void *fsdata;

    unsigned int owner; /* thread id of owner */
};

struct tty_struct{
    char *name;
    int (*write)(char *buf, unsigned int len);
    int (*read)(char *buf, unsigned int len);
};

extern struct file_struct **files;

/* devfs stuff */

struct devfs_file{
    char *name;
    void *data;
    unsigned long size;

    int (*read)(struct file_struct *f, char *buf, unsigned int len);
    int (*write)(struct file_struct *f, char *buf, unsigned int len);
    //int (*devctl)(unsigned int attr, void *value);
};

extern int devfs_init(void);
extern int devfs_register(struct devfs_file *devf);
extern int devfs_open(char *name, int flags, struct file_struct *f);

#endif /* _fs_h */
