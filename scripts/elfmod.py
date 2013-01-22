#!/usr/bin/python

import sys
import struct

# take filename from argv
if len(sys.argv) == 2:
    filename = sys.argv[1]
else:
    print 'usage: elfmod.py /path/to/file.bin'
    sys.exit(-1)

try: f = open(filename,"rb+")
except: 
    print 'elfmod.py: could not open file %s' % filename
    sys.exit(-1)

# elf machine type
f.seek(0x13)
f.write(chr(0x33))

# sections, we need to point shoff to after the NULL header
f.seek(0x20)
shoff = struct.unpack(">L",f.read(4))[0]
shoff += 80 # a section header is 40 bytes long
f.seek(0x20)
f.write(struct.pack(">L",shoff))

f.seek(0x30)
nh = struct.unpack(">H",f.read(2))[0]
nh -= 1
f.seek(0x30)
f.write(struct.pack(">H",nh))

f.seek(0x32)
f.write(struct.pack(">H",4))

f.close()

