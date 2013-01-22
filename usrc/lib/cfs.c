/*
http://www.si.org/cffs/
http://www.cisco.com/warp/public/63/pcmciamatrix.html

*/

/*
PCMCIA flash directory:
File  Length   Name/status
  1   3068440  c1600-y-l.111-8.AA  
[3068504 bytes used, 1125800 available, 4194304 total]
4096K bytes of processor board PCMCIA flash (Read ONLY)

08000000: BAD00B1E 002ED218 2467FFFF 00000000  :P....R.$g......
08000010: 63313630 302D792D 6C2E3131 312D382E  c1600-y-l.111-8.
08000020: 414100FF FFFFFFFF FFFFFFFF FFFFFFFF  AA..............
08000030: FFFFFFFF FFFFFFFF FFFFFFFF FFFFFFFF  ................
08000040: 00020107 002D87B4 00014A44 00023C38  .....-.4..JD..<8
08000050: 00000000 08000060 DADE001D 00000000  .......`Z^......
08000060: 60FF0000 01DE0000 00000000 00000000  `....^..........
08000070: 8002B0EE 00010000 000001E0 0801A1E4  ..0n.......`..!d
08000080: 02005000 002BE630 00014A44 00023C38  ..P..+f0..JD..<8
08000090: 0000026C 00019D38 08000060 02005000  ...l...8...`..P.
080000A0: 02005000 0001A1A4 002D87D4 993362C3  ..P...!$.-.T.3bC
080000B0: EE4D324C 002DB820 0000304C D6C4120E  nM2L.-8 ..0LVD..
080000C0: D6C4120E D185E18A 00000001 00000001  VD..Q.a.........
080000D0: 00000001 0000001D FFFFFFFF FFFFFFFF  ................
080000E0: FFFFFFFF FFFFFFFF FFFFFFFF FFFFFFFF  ................
*/

#define CFS_TYPE_INVALID 0
#define CFS_TYPE_A 1
#define CFS_TYPE_B 2
#define CFS_TYPE_C 3

/* These values and structure definitions were taken from the cffs project. Copyright (C) 2002 Simon Evans (spse@secret.org.uk). More information on the project at http://www.si.org/cffs/. */

/* cisco filesystem magic numbers */
#define CLASS_A_MAGIC 0x07158805
#define CLASS_B_MAGIC 0xBAD00B1E

/* Class A file header */

struct ca_hdr {
	unsigned long	magic;	
    unsigned long	filenum;	/* 0x00000001 */
	char		    name[64];	/* filename */
	unsigned long	size;		/* size in bytes */
	unsigned long	seek;		/* location of next file */
	unsigned long	crc;		/* File CRC */
	unsigned long	type;		/* ? 1 = config, 2 = image */
	unsigned long	date;		/* Unix type format */
	unsigned long	unk;
	unsigned long	flag1;		/* 0xFFFFFFF8 */
	unsigned long	flag2;		/* 0xFFFEFFFF is deleted file, all F's isn't */
	unsigned char	pad[128-104];
};


/* Class B file header */
struct cb_hdr {
	unsigned long magic;	
	unsigned long size; /* file size in bytes */
	unsigned short chksum;
    unsigned short flags;
	unsigned long date;
    char name[48];
    char data[0]; /* file data */
};

/* Class B Flags */

#define FLAG_DELETED   1
#define FLAG_HASDATE   2

#if 0
struct cfs_hdr {
	unsigned long magic;
	off_t pos;
	union{
		struct ca_hdr cafh;	/* Class A file */
		struct cb_hdr cbfh;	/* Class B file */
	} hdr;
};
#endif


#define FLASH_BASE  0x08000000

/* Returns the cfs type of header pointed to by 'addr' */
int cfs_type(unsigned long *addr)
{
    switch(*addr){
        case CLASS_A_MAGIC:
            return CFS_TYPE_A;
        case CLASS_B_MAGIC:
            return CFS_TYPE_B;

        /* Invaild CFS Type */
        default:
            return CFS_TYPE_INVALID;
    }
}


int main(void)
{
    int type;
    printf("examining flash memory at 0x%x\n",FLASH_BASE);
    type = cfs_type((unsigned long *) FLASH_BASE);

    if(type == CFS_TYPE_INVALID){
        printf("invalid cfs header\n");
        return -1;
    }
    printf("CFS type %s\n",(type == CFS_TYPE_A ? "A" : "B"));


    printf("list of files:\n");

    /* untested for class a */
    if(type == CFS_TYPE_A){
        struct ca_hdr *hdr = (struct ca_hdr *) FLASH_BASE;
        while(hdr->magic == CLASS_A_MAGIC){
            printf(" name: %s size: %i KB\n",hdr->name,hdr->size/1024);
            hdr = (struct ca_hdr *) ((long) hdr + hdr->size + sizeof(struct ca_hdr));
        }
    }

    else if(type == CFS_TYPE_B){
        struct cb_hdr *hdr = (struct cb_hdr *) FLASH_BASE;
        while(hdr->magic == CLASS_B_MAGIC){
            printf(" name: %s size: %i KB\n",hdr->name,hdr->size/1024);
            hdr = (struct cb_hdr *) ((long) hdr + hdr->size + sizeof(struct cb_hdr));
        }
    }

      
    return 0;
}

