#!/bin/sh


#powerpc-elf-gcc -S main.c
#powerpc-elf-gcc -c main.c -o main.o 
#powerpc-elf-ld ../lib/crt0.o main.o -o test -pie

m68k-elf-gcc -c main.c -o main.o
m68k-elf-ld ../lib/crt0.o main.o -o test -pie

cp test ../rd/test
