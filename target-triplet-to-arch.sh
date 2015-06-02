#!/bin/sh
# Author : Isaac Assegai
# Date   : 6/1/2015
# About  : Helps target the correct architecture.
# Info   : Parts of this file may have been excerpted from osdev.com
if echo "$1" | grep -Eq 'i[[:digit:]]86-'; then
  echo i386
else
  echo "$1" | grep -Eo '^[[:alnum:]_]*'
fi
