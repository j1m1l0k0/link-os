#include "../include/sys.h"

int main(int argc, char **argv)
{
    int fd;
    char ch;
    char *filename;

    if(argc != 2){
        printf("usage: cat <filename>\n");
        return -1;
    }
    
    filename = argv[1];

    fd = open(filename,0);
    if(fd < 0){
        printf("failed to open file '%s'\n",filename);
        return -2;
    }

    while(read(fd,&ch,1) > 0){
        printf("%c",ch); 
    }

    printf("\n");
    return 0;
}
