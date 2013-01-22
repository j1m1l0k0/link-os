#!/usr/bin/python
# Copyright (C) 2006 Kyle Lutz <kyle.r.lutz@gmail.com>
# PYthon Build System 

import os
import sys
import time

# Build options
target = 'c1000'
#target = 'c1600'
#target = 'c2500'
#target = 'c1720'

#optimize = '-O2'


# Runtime options
global force
force = 1 # force compiling of all files

img_name = target + '.bin'

top_dir = os.getcwd() + '/'
build_dir = top_dir + 'build/'
src_dir = top_dir + "src/"

kernel_dir = src_dir + 'kernel/'
lib_dir = src_dir + 'lib/'
include_dir = src_dir + 'include/'


cflags = " -S -W -Wall -fomit-frame-pointer -fno-strength-reduce -fno-builtin -nostdinc "
try: cflags += " " + optimize + " "
except: pass

aflags = " -D "

if target != 'c1720':
    tools = "/opt/cross/m68k-aout/bin/"
else: 
    tools = "powerpc-elf-"

as = tools + "as "
gcc = tools + "gcc "
ld = tools + "ld "
strip = tools + "strip "

    
cflags += "-I" + include_dir + ' '

lflags = " -dN "

if target == 'c1000':
    target_dir = src_dir + "target/C_68360/"
    cpu_dir = src_dir + "cpu/68k/"
    cflags += ' -m68020 '
    aflags += ' -m68360 ' # -mc68020
    cflags += ' -mnobitfield ' # needed for '360
    link_script = target_dir + 'c1000.ld '
    lflags += ' -T ' + link_script
elif target == 'c1600':
    target_dir = src_dir + "target/C_68360/"
    cpu_dir = src_dir + "cpu/68k/"
    cflags += ' -m68020 '
    aflags += ' -m68360 ' # -mc68020
    cflags += ' -mnobitfield ' # needed for '360
    link_script = target_dir + 'c1600.ld '
    lflags += ' -T ' + link_script
elif target == 'c2500':
    target_dir = src_dir + "target/C_68030/"
    cpu_dir = src_dir + "cpu/68k/"
    cflags += ' -m68020 '
    aflags += ' -m68020 ' # -mc68020
    link_script = target_dir + 'link.ld '
    lflags += ' -T ' + link_script
elif target == 'c1720':
    target_dir = src_dir + "target/C_XPC860/"
    cpu_dir = src_dir + "cpu/ppc/"
    aflags += ' -mregnames '
    link_script = target_dir + 'link.ld '
    lflags += ' -T ' + link_script





output = '' # string that will be filled with output by gcc and friends

def fatal(s):
    print "FATAL: ", s
    sys.exit(-1)

# return True if build dir is present
def have_build():
    return os.path.isdir(build_dir)

#return a list of object files in build dir
def ls_build():
    flist = []
    for file in os.walk(build_dir):
        for f in file[2]:
            if f[-2:] == '.o':
                flist.append(f)
    return flist
             
# return True if source file had been modified since it was last compiled
def modified(s_file):
    if s_file[-2:] not in ['.S','.s','.c']:
        print 'bad file ', s_file[-2:]
        return -1;
    s_fname = s_file.split('/')[-1]
    o_file = s_fname[:-2] + '.o'    
    if o_file not in ls_build():
        "%s is new" % s_fname
        return True
    elif os.path.getmtime(s_file) > os.path.getmtime(build_dir + o_file):
        print "%s has been modified" % s_fname
        return True
    else:
        print "%s is up to date" % s_fname
        return False

# file.S -> file.o
def assemble(file,force=0):
    #check if .S file is newer than .o file
    if not force and not modified(file):
        return
    fname = file.split("/")[-1][:-2]
    print "assembling %s" % file[len(src_dir)-1:]
    if file[-2:] == '.S':
        os.popen(gcc + ' -I ' + include_dir + ' -E '  + file + ' > ' + fname + '.s')
        file = fname + '.s'
    as_cmd = as + aflags + "-o " + fname + ".o " + file
    os.popen(as_cmd)

# file.c -> file.o
def compile(file,force=0):
    #check if .c file is newer than .o file
    if not force and not modified(file):
        return    
    global output
    fname = file.split("/")[-1][:-2]
    print "compiling %s" % file[len(src_dir)-1:]
    gcc_cmd = gcc + cflags + file
    os.system(gcc_cmd)
    #stdin, stdout = os.popen4(gcc_cmd)
    #output += stdout.read()
    as_cmd = as + aflags + "-o " + fname + ".o " + fname + ".s"
    stdin, stdout = os.popen4(as_cmd) 
    output += stdout.read()

    
def link():
    global output
    print 'linking files'
    ld_cmd = ld + lflags + ' -o ' + img_name + ' *.o'
    os.system(ld_cmd)
    #stdin, stdout, stderr = os.popen3(ld_cmd + ' -M > ./mmap.txt')
    #output += stdout.read() + stderr.read()
    print img_name + ' created'

def strip(file):
    print 'stripping %s' % file
    os.popen(strip + file)

# create symlinks for include directorys
def symlink():
    try:
        os.unlink(include_dir + 'cpu')
        os.unlink(include_dir + 'target')
    except: pass
    os.system("ln -sf %s %s" % (cpu_dir + 'include/', include_dir + 'cpu'))
    os.system("ln -sf %s %s" % (target_dir + 'include/', include_dir + 'target'))
    os.system("ln -sf %s %s" % (lib_dir + 'include/', include_dir + 'lib'))

# Compile all .c and .S files in dir 
def compile_dir(dir):
    global force
    for flist in os.walk(dir):
        for file in [file for file in flist[2] if file[-2:] == '.S']:
            assemble(dir + file,force)
        for file in [file for file in flist[2] if file[-2:] == '.c']:
            compile(dir + file,force)
  
def clean():
    os.chdir(top_dir)
    os.system('rm -rf *.bin')
    ret = os.system('rm -rf ./build')

def print_gcc_ver():
    ver = os.popen(gcc + ' -dumpversion').read()[:-1]
    arch = os.popen(gcc + ' -dumpmachine').read()[:-1]
    print "using gcc version %s for %s" % (ver, arch)

def make():
    stime = time.time()
    print "Building OS -- Target: %s" % target
    print_gcc_ver()  
    os.chdir(top_dir)

    if force:
        os.system("rm -rf %s" % build_dir)

    if not os.path.isdir(build_dir):
        try: os.mkdir(build_dir)
        except OSError, e:
            fatal("could not create build directory %s" % e)
     
    os.chdir(build_dir)

    symlink()
    
    # Compile all the files 
    compile_dir(target_dir)
    compile_dir(kernel_dir)
    compile_dir(lib_dir)
    compile_dir(cpu_dir)
    
    link()

    
    # Move the binary to wherever
    os.system('cp ' + img_name + ' ' + top_dir)    
    if os.system('cp ' + img_name + ' /var/tftpboot/os.bin') != 0:
        print 'WARNING: failed to copy image, %s still in build dir' % img_name       
    
    print 'Image size: %.4gk' % (os.path.getsize(top_dir + img_name)/1024.0)
    print 'Finished in %.4g seconds' % float(time.time()-stime) 
    
    if output:
        print "\nERRORS: "
        print output

   
    
def parse_cmd(args):
    if '-f' in args:
        global force
        force = 1
    return
        
if __name__ == '__main__':
    if len(sys.argv) > 1:
        parse_cmd(sys.argv)
    make()    
