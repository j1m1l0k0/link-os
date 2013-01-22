/* linkOS shell */

// lsh

#include "../include/sys.h"

int main(void)
{
    int i,j;
    int sz;
    unsigned int id;
    char buf[64];
    char *argv[6];

    for(;;){

        /* clear out our buffers */
        memset(buf,0,64);
        memset(argv,0,6*4);

        print("sh# ");
        sz = getline(buf);
        if(sz > 0){
            argv[0] = buf;
            for(i=0,j=1;i<sz;i++){
                if(buf[i] == ' '){
                    buf[i] = '\0';
                    argv[j++] = &buf[i+1];
                }
            }

            id = exec(buf,argv,NULL);
            if(id > 0)
                waitfor(id);
        }
    }
    return 0;
}

// execs(char *s)
