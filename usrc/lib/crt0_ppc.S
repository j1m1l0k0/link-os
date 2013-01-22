/* crt0 for powerpc targets */

.globl _start
_start:

    bl main /* jump to program's main() */
    
    /* sys_exit() */
    /* move exit status to r3 */
    lis 0, 4
    sc


