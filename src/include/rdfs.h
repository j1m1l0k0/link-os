/* Copyright (C) 2006 Kyle Lutz <kyle.r.lutz@gmail.com> */
/* RamDisk File System */
#ifndef _rdfs_h
#define _rdfs_h

#include <fs.h>

/* Foward Declarations */
struct rdfs_header;
struct rdfs_file;

/* Exported Functions */
extern int rdfs_load(void *rd);
extern int rdfs_open(char *name, int flags, struct file_struct *f);

/* only still here for exec.c */
extern struct rdfs_file *rdfs_getfile(char *name);


extern unsigned char ramdisk[];


#define RDFS_MAGIC 0xFF33EE22

struct rdfs_header {
    unsigned long magic;
    unsigned short nfiles;
} __attribute__((packed));

struct rdfs_file {
    char *name;
    void *data;
    unsigned long size;
} __attribute__((packed));

#endif /* _rdfs_h */
