#!/bin/sh
# Author : Isaac Assegai
# Date   : 6/1/2015
# About  : Use the make system to install sysroot headers.
# Info   : Parts of this file may have been excerpted from osdev.com
set -e
. ./config.sh

mkdir -p sysroot

for PROJECT in $SYSTEM_HEADER_PROJECTS; do
  DESTDIR="$PWD/sysroot" $MAKE -C $PROJECT install-headers
done
