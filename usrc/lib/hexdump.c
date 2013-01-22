#include "../include/sys.h"

/* Number of bytes to print per row */
unsigned char linesize = 16;

unsigned char num_lines = 12;

void print_byte(unsigned char byte)
{
    if(byte < 16)
        printf("0");
    printf("%x ",byte);
}


int main(int argc, char **argv)
{
    int i,j;

    unsigned char *addr = (unsigned char *) 0x00d00000;

    printf("dumping memory starting at 0x%x\n",addr);

    for(i=0; i<num_lines; i++){
        printf("0x%x: ",addr);
        for(j=0; j < linesize; j++)
            print_byte(addr[j]);
        printf("\n");
        addr += linesize;
    }

    return 0;
}
