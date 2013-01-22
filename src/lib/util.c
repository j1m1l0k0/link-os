#include <config.h>
#include <lib/printk.h>
#include <lib/string.h>
#include <target/target.h>

/* an error we cannot recover from */
// this should be a macro so that printk(...) works
void panic(char *message)
{
    printk(message);
    for(;;);
}

/* some ugly code to print a 'pretty' message while we boot. we could probably 
 * create the strings and message at compile time with a little scripting
 * but this will do for now. */
void print_booting_message(void)
{
    unsigned int i;
    unsigned int width; /* width of each line */
    static char line1[] = "-- linkOS kernel v" VERSION_STR " booting --";
    static char line2[] = "-- cpu: " CPU_STR " target: " TARGET_STR " ";
    
    width = strlen(line1);

    printk("\n");
    for(i=0;i<width;i++)
       printk("-");
    printk("\n");

    printk(line1);
    printk("\n");
    printk(line2);
    for(i=strlen(line2);i<strlen(line1)-2;i++)
        printk(" ");
    printk("--\n");

   for(i=0;i<width;i++)
       printk("-");
    printk("\n");
}
