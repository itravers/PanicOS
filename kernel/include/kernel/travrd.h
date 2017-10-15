/**travrd.h
 * Author: Isaac Assegai
 * Date: 10/13/2017
 * Defines the interface for and structures relating to the travrd ramdisk.
 */

#ifndef INITRD_H
#define INITRD_H

#include <stdlib.h>
#include "fs.h"

fs_node_t *version_node; //The node of the version.txt file

/* The very first structure on the ramdisk
 * Tells us now many files are included on the disk */
typedef struct{
  u32int nfiles; // The number of files in the ramdisk.
}travrd_header_t;

/* The Header for a file.
 * These are included on the ramdisk immediately after the initrd_header_t */
typedef struct{
  u8int magic;     // Magic number, for error checking.
  s8int name[64];  // Filename.
  u32int offset;   // Offset in the initrd that the file starts.
  u32int length;   // Length of the file.
}travrd_file_header_t;

/* Initialises the initial ramdisk. It gets passed the address of the multiboot module,
 * and returns a completed filesystem node. */
fs_node_t *travrd_initialize(u32int location);

#endif
