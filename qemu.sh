#!/bin/sh
# Author : Isaac Assegai
# Date   : 6/1/2015
# About  : Use qemu to launch PanicOS's compile iso
###################################################
set -e
. ./clean.sh
. ./iso.sh

#qemu-system-$(./target-triplet-to-arch.sh $HOST) -kernel isodir/boot/PanicOS.kernel
qemu-system-$(./target-triplet-to-arch.sh $HOST) -cdrom isodir/boot/PanicOS.iso
