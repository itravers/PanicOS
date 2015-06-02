#!/bin/sh
# Author: Isaac Assegai
# Date  : 6/1/15
# About : Makes sure the project headers are installed and 
#       : compile PanicOS.
##########################################################
set -e
. ./headers.sh

for PROJECT in $PROJECTS; do
  DESTDIR="$PWD/sysroot" $MAKE -C $PROJECT install
done
