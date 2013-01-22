

// kmod
//   -i <module>    insert a module
//   -r <module>    remove a module
//   -l             list kernel modules

extern void printk(char *fmt, ...);


int load(void)
{
    printk("module load() called\n");
    return 0;
}

void unload(void)
{
    return;
}

