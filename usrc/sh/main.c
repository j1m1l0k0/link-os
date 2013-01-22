static char buffer[] = "string in buffer\n";

/*
jumping to 0x7fff69b0                                                         
sys_test called 1                                                             
null syscall  

something in the link register gets screwed up
we need to save it when we go into the isr.

*/

int main(void)
{
    //asm('move.l #42, %d0");
    //asm("trap #8");
    for(;;);
    return 0;
}
