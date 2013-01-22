#ifndef _module_h
#define _module_h

#define EXPORT_SYMBOL(symbol) export_ksym(#symbol, &symbol)

extern int module_init(void);
extern int export_ksym(char *symbol, void *address);

extern void dump_ksyms(void);

#endif /* _module_h */
