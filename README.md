PanicOS - A Hobby Operating System
==================================

Introduction
------------

PanicOS is a Unix-like Operating System made for learning purposes in my spare time.
It is derived from the following tutorials

1. bkerndev - Bran's Kernel Development Tutorial (http://www.osdever.net/tutorials/view/brans-kernel-development-tutorial)

   The first steps to realize PanicOS based on Bran's Kernel Development
   Tutorial.
   In particular, the initialization of GDT, IDT and the interrupt handlers are derived
   from this tutorial.

2. OsDev - Bare Bones Tutorial (http://wiki.osdev.org/Bare_Bones)
   This tutorial serves as an example of how to create a minimal system, but not 
   as an example of how to properly structure your project. These instructions are 
   community reviewed and follow the current recommendations for good reasons.

3. OsDev - Meaty Skeleton Tutorial (http://wiki.osdev.org/Meaty_Skeleton)
   This tutorial implements neccesary ABI features needed to satisfy the ABI 
   and compiler contracts to prevent possible mysterious errors.
   This tutorial also serves as the initial template tutorial on how to create 
   your own libc (Standard C Library).

4. JamesM's kernel development tutorials (http://www.jamesmolloy.co.uk/tutorial_html/index.html)
   The first version of PanicOS's virtual filesystem and its initial
   ramdisks is is derived from JamesM's kernel development tutorials.

REquirements of PanicOS

* i686-elf-gcc cross compiler

binutils  

export PREFIX="$HOME/opt/cross"
export TARGET=i686-elf
export PATH="$PREFIX/bin:$PATH"
mkdir $HOME/src
cd $HOME/src

  wget ftp://sourceware.org/pub/binutils/snapshots/binutils-2.29.1.tar.gz
  tar -xvf binutils-2.29.1.tar.gz
mkdir build-binutils
cd build-binutils
../binutils-2.29.1/configure --target=$TARGET --prefix="$PREFIX" --with-sysroot --disable-nls --disable-werror
make

gcc

cd $HOME/src
wget http://www.netgull.com/gcc/releases/gcc-7.2.0/gcc-7.2.0.tar.gz
tar -xvf gcc-7.2.0.tar.gz

//download gmp mpfr and mpc
cd gcc-7.2.0
contrib/download_prerequisites

cd $HOME/src

mkdir build-gcc
cd build-gcc
../gcc-7.2.0/configure --target=$TARGET --prefix="$PREFIX" --disable-nls --enable-languages=c,c++ --without-headers
make all-gcc

  

Version 0.0.1
