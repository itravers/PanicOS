#!/bin/sh
# Author : Isaac Assegai
# Date   : 6/1/2015
# About  : Helps target the correct architecture.
####################################################################
if echo "$1" | grep -Eq 'i[[:digit:]]86-'; then
  echo i386
else
  echo "$1" | grep -Eo '^[[:alnum:]_]*'
fi
