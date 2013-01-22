#!/bin/sh

#powerpc-elf-as crt0.S -o crt0.o
m68k-elf-as crt0_68k.S -o crt0.o
m68k-elf-gcc -c sys.c -fPIC -nostdinc

m68k-elf-gcc -c ls.c -fPIC
m68k-elf-gcc -c sh.c -fPIC
m68k-elf-gcc -c help.c -fPIC
m68k-elf-gcc -c cat.c -fPIC
m68k-elf-gcc -c cfs.c -fPIC
m68k-elf-gcc -c time.c -fPIC
m68k-elf-gcc -c cookie.c -fPIC
m68k-elf-gcc -c hexdump.c -fPIC

m68k-elf-ld crt0.o sys.o ls.o -o ls -pie
m68k-elf-ld crt0.o sys.o sh.o -o sh -pie
m68k-elf-ld crt0.o sys.o help.o -o help -pie
m68k-elf-ld crt0.o sys.o cat.o -o cat -pie
m68k-elf-ld crt0.o sys.o cfs.o -o cfs -pie
m68k-elf-ld crt0.o sys.o time.o -o time -pie
m68k-elf-ld crt0.o sys.o cookie.o -o cookie -pie
m68k-elf-ld crt0.o sys.o hexdump.o -o hexdump -pie

cp ls ../rd/
cp sh ../rd/
cp help ../rd/
cp cat ../rd/
cp cfs ../rd/
cp time ../rd/
cp cookie ../rd/
cp hexdump ../rd/

../../pybs rdfs > /dev/null
