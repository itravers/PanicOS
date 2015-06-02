#!/bin/sh
# Author: Isaac Assegai
# Date  : 6/1/15
# About : Cleans the project of all compile files.
# Info  : Parts of this file may have been excerpted from osdev.com
set -e
. ./config.sh

for PROJECT in $PROJECTS; do
  $MAKE -C $PROJECT clean
done

rm -rfv sysroot
rm -rfv isodir
rm -rfv myos.iso
