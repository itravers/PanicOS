/**fs.c
 * Authos: Isaac Assegai
 * Date: 10/4/2017
 * Implements the virtual file system
 * Based on tutorial by James Molloy
 */

#include <kernel/fs.h>

/* The root of the filesystem. */
fs_node_t *fs_root = 0;

/* Lists the Directories, Files and File Contents of the initrd. */
void list_fs(void){
  int i = 0;
  struct dirent *node = 0;
  printf("Listing FileSystem \n");

  while ( (node = readdir_fs(fs_root, i)) != 0){
    printf("\n");
    printf(node->name);
    fs_node_t *fsnode = finddir_fs(fs_root, node->name);

    if((fsnode->flags&0x7) == FS_DIRECTORY){
            printf(" (directory)\n");
    }else{
      printf(" (file)");
      char buf[256];
      for(int i = 0; i < 256; i++){
        buf[i] = 'X';
      }
      
      printf("\n");

      /* Read the contents of the file into buf */
      u32int sz = read_fs(fsnode, 0, 256, buf);
      int j;

      /* Print out the contents of the file. */
      for (j = 0; j < sz; j++){
        putchar(buf[j]);
      }
    }
    i++;
  }
}

/* Reads a file into a buffer
 * *node: the node representing the file
 * offset: how far into the file to start reading, should normally be 0
 * size: how much of the file to read.
 * buffer: the buffer we are reading the file into
 * returns: the size of the file read. 
 */
u32int read_fs(fs_node_t *node, u32int offset, u32int size, u8int *buffer){
  //This blank printf needs to be here, otherwise the command buffer gets messup up.. weird.
  printf(" ");

  // Has the node got a read callback?
  if (node->read != 0){
    return node->read(node, offset, size, buffer);
  }else{
    return 0;
  }
}

/* Writes data from a buffer into a file. */
u32int write_fs(fs_node_t *node, u32int offset, u32int size, u8int *buffer){
  // Has the node got a write callback?
  if(node->write != 0){
    return node->write(node, offset, size, buffer);
  }else{
    return 0;
  }
}

/* Opens a file given by *node. */
void open_fs(fs_node_t *node, u8int read, u8int write){
  // Has the node got an open callback?
  if (node->open != 0){
    return node->open(node);
  }
}

/* Closes a file given by *node. */
void close_fs(fs_node_t *node){
  // Has the node got a close callback?
  if (node->close != 0){
    return node->close(node);
  }
}

/* Reads a directory given by *node. */
struct dirent *readdir_fs(fs_node_t *node, u32int index){
  // Is the node a directory, and does it have a callback?
  if((node->flags&0x7) == FS_DIRECTORY && node->readdir != 0){
    return node->readdir(node, index);
  }else{
    return 0;
  }
}

/* Finds a directory of a given name. */
fs_node_t *finddir_fs(fs_node_t *node, char *name){
  // Is the node a directory, and does it have a callback?
  if((node->flags&0x7) == FS_DIRECTORY && node->finddir != 0){
    return node->finddir(node, name);
  }else{
    return 0;
  }
}
