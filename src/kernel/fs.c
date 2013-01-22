/* file system */

#include <fs.h>
#include <thread.h>
#include <lib/kmem.h>
#include <lib/string.h>
#include <lib/printk.h>

#include <rdfs.h>

struct file_struct **files;
static unsigned int maxfd = 3; /* highest fd in list **files */

int fs_init(void)
{
    files = kmalloc(10 * sizeof(struct file_struct *));
    printk("filesystem initialized\n");

    /* load the ramdisk filesystem */
    if(rdfs_load(&ramdisk) != 0){
        printk("failed to load ramdisk at 0x%x\n",&ramdisk);
        for(;;);
    }

    devfs_init();

    return 0;
}

/* create a file descriptor */
int fdalloc(void)
{
    return 0;
}

/* free a file descriptor */
void fdfree(void)
{
}



/* determine if current thread has access to the file 'fd' 
 *
 * returns 1 if current thread has access.
 * returns a number less than 0 otherwise.
 */
int fallowed(unsigned int fd)
{
    /* everyone can access stdin, stdout, and stderr */
    if(fd < 3)
        return 1;
    /* if it is an invalid fd */
    else if((fd > maxfd) || files[fd] == NULL)
        return -2;
    /* the owner of the fd can access it */
    else if(files[fd]->owner == cthread->id)
        return 1;
    /* no one else can access it */
    else
        return -1;
}


int do_read(int fd, char *buf, unsigned int len)
{
    struct file_struct *f = files[fd];
    if(!f)
        return -1;
    return f->read(f,buf,len);
}

int do_open(char *name, int flags)
{
    (void) flags;
    struct file_struct *f = kmalloc(sizeof(struct file_struct));

    memset(f,0,sizeof(struct file_struct));

    if(strncmp(name,"/dev/",5) == 0){
        if(devfs_open(name,flags,f) != 0)
            return -1;
    }

    else{
        if(rdfs_open(name,flags,f) != 0)
            return -1;
    }

    f->fpos = 0;
    files[maxfd] = f;

    return maxfd++;
}

int close(int fd)
{
    (void) fd;
    return 0;
}


/*******************************
 * devfs -- device file system *
 *******************************/

static struct devfs_file **devfs_files;
static unsigned char devfs_nexti = 0;

int devfs_register(struct devfs_file *devf)
{
    devfs_files[devfs_nexti++] = devf;
    return 0;
}

int devfs_open(char *name, int flags, struct file_struct *f)
{
    (void) flags;

    int i;
    struct devfs_file *df = NULL;

    (unsigned long) name += 5; /* get rid of '/dev/' and leave just the name */

    for(i = 0; i < devfs_nexti; i++){
        if(strcmp(devfs_files[i]->name,name) == 0){
            df = devfs_files[i];
            break;
        }
    }

    if(df == NULL)
        return -1; /* we didn't find the file */

    f->fsdata = df;
    f->read = df->read;

    return 0;
}


/* /dev/nvram device */
static int nvram_read(struct file_struct *f, char *buf, unsigned int len)
{
    unsigned int i = 0;
    char *base = (char *) 0x0E0002C0;
    (void) f;
    (void) buf;
    (void) len;

    while((i < len) && base[f->fpos] != '\0')
        buf[i++] = base[f->fpos++];
    
    if(i != len)
        return -1;
    return i;
}

static int nvram_write(struct file_struct *f, char *buf, unsigned int len)
{
    (void) f;
    (void) buf;
    (void) len;

    /* no support for writing nvram (yet) */
    return -1;
}

static struct devfs_file nvram_file = {
    .name = "nvram",
    .read = nvram_read,
    .write = nvram_write,
};


int devfs_init(void)
{
    devfs_files = kmalloc(10 * sizeof(struct devfs_file));

    devfs_register(&nvram_file);
    return 0;
}

 
