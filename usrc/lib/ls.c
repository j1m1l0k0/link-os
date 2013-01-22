#include "../include/sys.h"

int main(void)
{
    char *argv[] = {"cat",".",NULL};

    int id = exec("cat",argv,NULL);
    if(id)
        waitfor(id);
    return 0;
}
