/* for c1700
rommon 1 > cookie
cookie:                                                                      
01 01 00 0a 8a 41 4d 64 33 00 01 ff 00 b2 00 00                              
00 00 00 00 00 00 00 00 56 4d 53 07 03 30 35 53                              
48 06 01 00 00 00 00 00 00 ff ff ff 58 04 49 0c                              
81 06 ff ff ff ff ff ff ff ff ff ff ff ff ff ff                              
ff ff ff ff ff ff ff ff ff ff ff ff ff ff ff ff                              
ff ff ff ff ff ff ff ff ff ff ff ff ff ff ff ff                              
ff ff ff ff ff ff ff ff ff ff ff ff ff ff ff ff                              
ff ff ff ff ff ff ff ff ff ff ff ff ff ff ff ff

rommon 25 > dump 0x68000000 100                                              
68000000  8d74 0101 000a 8a41 4d64 3300 01ff 00b2 .t.....AMd3.....           
68000010  0000 0000 0000 0000 0000 564d 5307 0330 ..........VMS..0           
68000020  3553 4806 0100 0000 0000 00ff ffff 5804 5SH...........X.           
68000030  490c 8106 ffff ffff ffff ffff ffff ffff I...............           
68000040  ffff ffff ffff ffff ffff ffff ffff ffff ................           
68000050  ffff ffff ffff ffff ffff ffff ffff ffff ................           
68000060  ffff ffff ffff ffff ffff ffff ffff ffff ................           
68000070  ffff ffff ffff ffff ffff ffff ffff ffff ................           
68000080  ffff e99f ffff ffff 0000 0000 0000 0000 ................           
68000090  2102 defd feed face 0000 0000 0000 0019 !...............
*/


/* for c2500
e0 mac 0060 5cf3 7ec8
LANCE unit 0, idb 0x54F18, ds 0x56CD0, regaddr = 0x2130000

Router(boot)#show ver
Cisco Internetwork Operating System Software 
IOS (tm) 3000 Bootstrap Software (IGS-RXBOOT), Version 10.2(8a), RELEASE SOFTWARE )
Copyright (c) 1986-1995 by cisco Systems, Inc.
Compiled Tue 24-Oct-95 15:46 by mkamson
Image text-base: 0x01020000, data-base: 0x00001000

ROM: System Bootstrap, Version 5.2(8a), RELEASE SOFTWARE

Router uptime is 1 minute
System restarted by power-on
Running default software

cisco CPA2500 (68030) processor (revision F) with 16380K/2048K bytes of memory.
Processor board serial number 04134821 with hardware revision 00000000
X.25 software, Version 2.0, NET2, BFE and GOSIP compliant.
Authorized for CiscoPro software set only.  (0x80)
1 Ethernet/IEEE 802.3 interface.
2 Serial network interfaces.
32K bytes of non-volatile configuration memory.

Configuration register is 0x2102


*/

#include "../include/sys.h"

//#define TARGET_IS_c1000
//#define TARGET_IS_c2500


#ifdef TARGET_IS_c2500
// we need to find it
#endif

#ifdef TARGET_IS_c1000

#endif

#ifdef TARGET_IS_c1700
#define COOKIE_ADDR 0x68000000

struct cookie_struct{
    short ukn;
};

#endif


/* c1600 */
#define COOKIE_ADDR 0x0E000284

struct cookie_struct{
    short ukn;
    unsigned char e0_mac[6]; /* ethernet 0 mac address */
    char ukn2[8];
    unsigned long board_id; /* board id */
    char ukn3[12];
};


/*
0x0E000284

cookie:
01 01 00 50 73 08 b4 e9 09 00 00 00 00 02 00 00 
11 89 14 61 00 00 00 00 00 00 00 00 00 00 00 00

0E000280: 66368D74 01010050 7308B4E9 09000000  f6.t...Ps.4i....
0E000290: 00020000 11891461 00000000 00000000  .......a........

*/


int main(void)
{

#ifdef COOKIE_ADDR
    printf("reading cookie at 0x%x\n",COOKIE_ADDR);
#else
    printf("location of cookie unknown for this platform\n");
    return -1;
#endif

    struct cookie_struct *cookie = (struct cookie_struct *) COOKIE_ADDR;

    printf("board id: %x\n",cookie->board_id);
    printf("e0 mac: %x:%x:%x:%x:%x:%x\n",
            cookie->e0_mac[0],cookie->e0_mac[1],cookie->e0_mac[2],
            cookie->e0_mac[3],cookie->e0_mac[4],cookie->e0_mac[5]);

    
    return 0;
}

