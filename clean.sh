#!/bin/sh
# Author: Isaac Assegai
# Date  : 6/1/15
# About : Cleans the project of all compile files.
###################################################################
set -e
. ./config.sh

for PROJECT in $PROJECTS; do
  $MAKE -C $PROJECT clean
done

rm -rfv sysroot
rm -rfv isodir
rm -rfv PanicOS.iso
