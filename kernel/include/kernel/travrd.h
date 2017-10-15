/**travrd.h
 * Author: Isaac Assegai
 * Date: 10/13/2017
 * Defines the interface for and structures relating to the travrd ramdisk.
 */

#ifndef TRAVRD_H
#define TRAVRD_H

#include <stdlib.h>
#include "fs.h"

fs_node_t *version_node; //The node of the version.txt file

/* The very first structure on the ramdisk
 * Tells us now many files are included on the disk */
typedef struct{
  u32int nfiles; // The number of files in the ramdisk.
}travrd_header_t;

enum FILETYPE {DIRECTORY, AFILE};

/* The Header for a file.
 * These are included on the ramdisk immediately after the initrd_header_t */
typedef struct{
  u16int magic;     // Magic number, for error checking.
  u8int name[64];  // Filename.
  s32int parentNum; //The number of the parent directory
  u32int offset;   // Offset in the initrd that the file starts.
  u32int length;   // Length of the file.
  u32int fileNum;   // The number of this file
  enum FILETYPE type;    //the type of this file, DIRECTORY or AFILE
}travrd_file_header_t;

/* Initialises the initial ramdisk. It gets passed the address of the multiboot module,
 * and returns a completed filesystem node. */
fs_node_t *travrd_initialize(u32int location);

#endif
