/**travrd.c
 * Author: Isaac Assegai
 * Date: 10/4/2017
 * Implements the methods relating to the initial travrd ramdisk. 
 */

#include <kernel/travrd.h>
#include <kernel/kheap.h> //For kmalloc
#include <stdlib.h>
#include <string.h>

travrd_header_t *travrd_header;     // The header.
travrd_file_header_t *file_headers; // The list of file headers.
fs_node_t *travrd_root;             // Our root directory node.
fs_node_t *travrd_dev;              // We also add a directory node for /dev, so we can mount devfs later on.
fs_node_t *root_nodes;              // List of file nodes.
unsigned int nroot_nodes;           // Number of file nodes.


struct dirent dirent;

/* Reads a file pointed to by node into a buffer. */
static u32int travrd_read(fs_node_t *node, u32int offset, u32int size, u8int *buffer){
  travrd_file_header_t header = file_headers[node->inode];
  if(offset > header.length){
    return 0;
  }
  
  if(offset+size > header.length){
    size = header.length-offset;
  }
   
  memcpy(buffer, (u8int*) (header.offset+offset), size);
  return size;
}

/* Returns a directory entry from node[index] */
static struct dirent *travrd_readdir(fs_node_t *node, u32int index){
  if(node == travrd_root && index == 0){
    strcpy(dirent.name, "dev");
    dirent.name[3] = 0;
    dirent.ino = 0;
    return &dirent;
  }

  if(index-1 >= nroot_nodes){
    return 0;
  }

  strcpy(dirent.name, root_nodes[index-1].name);
  dirent.name[strlen(root_nodes[index-1].name)] = 0;
  dirent.ino = root_nodes[index-1].inode;
  return &dirent;
}

/* Finds a directory of a specific name in the node. */
static fs_node_t *travrd_finddir(fs_node_t *node, char *name){
  if(node == travrd_root && !strcmp(name, "dev")){
    return travrd_dev;
  }

  unsigned int i;
  for (i = 0; i < nroot_nodes; i++){
    if (!strcmp(name, root_nodes[i].name)){
      return &root_nodes[i];
    }
  }
    return 0;
}

/* Initializes the ramdisk as a specific memory location. */
fs_node_t *travrd_initialize(u32int location){
  // Initialise the main and file header pointers and populate the root directory.
  travrd_header = (travrd_header_t *)location;
  file_headers = (travrd_file_header_t *) (location+sizeof(travrd_header_t));

  // Initialise the root directory.
  travrd_root = (fs_node_t*)kmalloc(sizeof(fs_node_t));
  strcpy(travrd_root->name, "travrd");
  travrd_root->mask = travrd_root->uid = travrd_root->gid = travrd_root->inode = travrd_root->length = 0;
  travrd_root->flags = FS_DIRECTORY;
  travrd_root->read = 0;
  travrd_root->write = 0;
  travrd_root->open = 0;
  travrd_root->close = 0;
  travrd_root->readdir = &travrd_readdir;
  travrd_root->finddir = &travrd_finddir;
  travrd_root->ptr = 0;
  travrd_root->impl = 0;

  // Initialise the /dev directory (required!)
  travrd_dev = (fs_node_t*)kmalloc(sizeof(fs_node_t));
  strcpy(travrd_dev->name, "dev");
  travrd_dev->mask = travrd_dev->uid = travrd_dev->gid = travrd_dev->inode = travrd_dev->length = 0;
  travrd_dev->flags = FS_DIRECTORY;
  travrd_dev->read = 0;
  travrd_dev->write = 0;
  travrd_dev->open = 0;
  travrd_dev->close = 0;
  travrd_dev->readdir = &travrd_readdir;
  travrd_dev->finddir = &travrd_finddir;
  travrd_dev->ptr = 0;
  travrd_dev->impl = 0;

  root_nodes = (fs_node_t*)kmalloc(sizeof(fs_node_t) * travrd_header->nfiles);
  nroot_nodes = travrd_header->nfiles;

  // For every file...
  unsigned int i;
  for(i = 0; i < travrd_header->nfiles; i++){
    // Edit the file's header - currently it holds the file offset
    // relative to the start of the ramdisk. We want it relative to the start
    // of memory.
    file_headers[i].offset += location;
    // Create a new file node.
    strcpy(root_nodes[i].name, file_headers[i].name);
    printf("\nroot_notes.name: %s", root_nodes[i].name);
    //save the version.txt files node, for later reference
    if(strcmp(root_nodes[i].name, "version.txt") == 0){
      version_node = &root_nodes[i];
      
      //printf("\nversion_node.name: %s", version_node->name);
    }

    root_nodes[i].mask = root_nodes[i].uid = root_nodes[i].gid = 0;
    root_nodes[i].length = file_headers[i].length;
    root_nodes[i].inode = file_headers[i].fileNum;
    root_nodes[i].parentInode = file_headers[i].parentNum;
    root_nodes[i].read = &travrd_read;
    root_nodes[i].write = 0;
    root_nodes[i].readdir = 0;
    root_nodes[i].finddir = 0;
    root_nodes[i].open = 0;
    root_nodes[i].close = 0;
    root_nodes[i].impl = 0;
    
    if(file_headers[i].type == AFILE){
      root_nodes[i].flags = FS_FILE;
    }else if(file_headers[i].type == DIRECTORY){
      root_nodes[i].flags = FS_DIRECTORY;
    }
  }
  return travrd_root;
}
