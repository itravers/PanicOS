#!/bin/sh
# Author : Isaac Assegai
# Date   : 6/1/2015
# About  : Create ISO install it to the iso boot directory
#        : and adjust our grub config to show our new kernel
############################################################
set -e
. ./build.sh

mkdir -p isodir
mkdir -p isodir/boot
mkdir -p isodir/boot/grub

cp sysroot/boot/PanicOS.kernel isodir/boot/PanicOS.kernel
#cp sysroot/boot/initrd.img isodir/boot/initrd.img
cp sysroot/boot/travrd.img isodir/boot/travrd.img
cat > isodir/boot/grub/grub.cfg << EOF
set timeout=1
menuentry "PanicOS" {
	multiboot /boot/PanicOS.kernel
  module /boot/travrd.img 
}
EOF
grub-mkrescue -o PanicOS.iso isodir
mv PanicOS.iso isodir/boot/PanicOS.iso
