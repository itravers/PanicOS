PanicOS - A Hobby Operating System
==================================
Version 0.0.3

<p align="center">
   <img src="https://i.imgur.com/6vBowK0.png" width="540" height="300" alt="PanicOS Screenshot"></img>
</p>

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
   
Features
--------
* GDT & IDT tables properly setup.
* Interrupt Requests & Service Routines Working (IRQ's & ISRS's)
* Protected Mode
* Simple Keyboard Driver 
* Simple TTY Driver
* Simple Shell
* Simple Memory Management
* Paging
* Initial Ramdisk Setup and Working (initrd)
* Basic Serial Driver for debugging purposes
   
Download PanicOS
----------------
```shell
git clone https://github.com/itravers/PanicOS.git
```

Requirements of PanicOS
-----------------------
PanicOS is designed to be run on x86 cpu's. 
In order to run and compile PanicOS the following is
required of the host operating system.

* Linux OS
* Make
* i686-elf-as cross assembler
* i686-elf-gcc cross compiler
* qemu Virtual Machine

### Installing Requirements

Configure PATH variables in preparation for compiling binutils and gcc
```shell
export PREFIX="$HOME/opt/cross"
export TARGET=i686-elf
export PATH="$PREFIX/bin:$PATH"
```

##### Libs needed to build lib-gcc
```shell
sudo apt-get install libgmp3-dev
sudo apt-get install libmpfr-dev
```

##### make
make is the build system that calls binutils and gcc to compile PanicOS
```shell
sudo apt-get install make
```

##### GCC
You'll need a copy of GCC for your hosts operating system in order to compile
the GCC Cross compiler. You actually need 2 different GCC's altogether.
Luckily this one is easy to install.
```shell
sudo apt-get install g++
```

##### binutils  
binutils includes the linker required to cross-compile PanicOS (39MB)(3-5 Minute Compile)
```shell
mkdir $HOME/src
cd $HOME/src
wget ftp://sourceware.org/pub/binutils/snapshots/binutils-2.29.1.tar.gz
tar -xvf binutils-2.29.1.tar.gz
mkdir build-binutils
cd build-binutils
../binutils-2.29.1/configure --target=$TARGET --prefix="$PREFIX" --with-sysroot --disable-nls --disable-werror
make
```

##### nasm
nasm is the assembler we use to assemble PanicOS
```shell
sudo apt-get install nasm
```

##### GCC Cross Compiler
GCC Cross Compiler includes the c compiler required to cross-compile PanicOS (107MB)(30 Minute Compile)

```shell
cd $HOME/src
wget http://www.netgull.com/gcc/releases/gcc-7.2.0/gcc-7.2.0.tar.gz
tar -xvf gcc-7.2.0.tar.gz
```
download gmp mpfr and mpc
```shell
cd gcc-7.2.0
contrib/download_prerequisites

cd $HOME/src
mkdir build-gcc
cd build-gcc
../gcc-7.2.0/configure --target=$TARGET --prefix="$PREFIX" --disable-nls --enable-languages=c,c++ --without-headers

make all-gcc
make all-target-libgcc
make install-gcc
make install-target-libgcc
```

To use your new compiler simply by invoking $TARGET-gcc, add $HOME/opt/cross/bin to your $PATH by typing:
```shell
export PATH="$HOME/opt/cross/bin:$PATH"
```
At this point the ./build script works, but grub-mkrescue doesn't because xorriso is to old

##### xorriso
xorriso is used by grub-mkrescue to generate an iso of the PanicOS kernel
```shell
sudo apt-get install xorriso
```

##### qemu
qemu is the virtual machine that emulates a x86 system that we will run the PanicOS kernel in (53MB)
```shell
sudo apt-get install qemu-system-x86
```

Building PanicOS
--------------
PanicOS includes shell scripts to make building easy
just invoke the ./build.sh shell script
```shell
./build.sh
```
to cleanup the build environment just invoke
```shell
./clean.sh
```

Booting PanicOS
PanicOS includes shell scripts to boot the qemu VM, just invoke
```shell
./qemu.sh
```

Issues
--------------

