/* Copyright (C) 2006 Kyle Lutz <kyle.r.lutz@gmail.com> */

#include <fs.h>
#include <rdfs.h>
#include <types.h>
#include <lib/string.h>
#include <lib/printk.h>

/* the ramdisk */
#include <rd.h>

static struct rdfs_header *hdr = NULL; /* RamDisk header */
static struct rdfs_file *first = NULL; /* First file header in ramdisk. */


/* Load and initialize the ramdisk. Must be called prior to using the ramdisk. 
 * Returns 0 on success, < 0 on failure. */
int rdfs_load(void *rd)
{
    int i;
    hdr = (struct rdfs_header *) rd;
    
    if(hdr->magic != RDFS_MAGIC){
        printk("bad rdfs_header magic 0x%x\n",hdr->magic);
        return -1;
    }

    printk("loading ramdisk at 0x%x (%i files)\n",hdr,hdr->nfiles); 
    first = (struct rdfs_file *) (rd + sizeof(struct rdfs_header));
    for(i=0; i < hdr->nfiles; i++){
        first[i].name += ((long) rd) + sizeof(struct rdfs_header) + 
                         (hdr->nfiles * sizeof(struct rdfs_file));
        first[i].data += ((long) rd) + sizeof(struct rdfs_header) + 
                         (hdr->nfiles * sizeof(struct rdfs_file));
    }
    return 0;
}

/* Returns a pointer to the rdfs_file struct that describes file 'name'.
 * Returns NULL if the file cannot be found. */
struct rdfs_file *rdfs_getfile(char *name)
{
    int i;
    for(i=0;i<hdr->nfiles;i++){
        if(strcmp(name,first[i].name) == 0)
            return &first[i];
    }
    return NULL;
}


static int rdfs_read(struct file_struct *f, char *buf, unsigned int len)
{
    unsigned int i = 0;
    char *data = ((struct rdfs_file *) f->fsdata)->data;
    unsigned long size = ((struct rdfs_file *) f->fsdata)->size;

    while((i < len) && (f->fpos < size))
        buf[i++] = data[f->fpos++];

    if(f->fpos == size)
        return -1;

    return i;
}

static int rdfs_write(struct file_struct *f, char *buf, unsigned int len)
{
    /* cant write rdfs files */
    (void) f;
    (void) buf;
    (void) len;
    return -1;
}

int rdfs_open(char *name, int flags, struct file_struct *f)
{
    (void) flags;

    f->fsdata = rdfs_getfile(name);
    if(f->fsdata == NULL)
        return -1;

    f->read = rdfs_read;
    f->write = rdfs_write;

    return 0;
}



