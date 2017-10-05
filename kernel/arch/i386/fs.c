// fs.c -- Defines the interface for and structures relating to the virtual file system.
//         Written for JamesM's kernel development tutorials.

#include <kernel/fs.h>

fs_node_t *fs_root = 0; // The root of the filesystem.

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
      //char* buf = kmalloc_a(256);
      char buf[256];
     // printf("\n buf: 0X%x", buf);
      for(int i = 0; i < 256; i++){
        buf[i] = 'X';
      }

      //buf[10] = '\0';

      //putchar(buf[9]);
      //printf("\nbuf: %s", buf);

      printf("\n");

      //u32int sz = 256;
      u32int sz = read_fs(fsnode, 0, 256, buf); //this write over something weird
 
      int j;
      for (j = 0; j < sz; j++)
        putchar(buf[j]);
            
      //printf("\"\n");
      
    }
    i++;
  }
}

u32int read_fs(fs_node_t *node, u32int offset, u32int size, u8int *buffer)
{
    printf(" ");//This needs to be here, otherwise the command buffer gets messed up???
    // Has the node got a read callback?
    if (node->read != 0)
        return node->read(node, offset, size, buffer);
    else
        return 0;
}

u32int write_fs(fs_node_t *node, u32int offset, u32int size, u8int *buffer)
{
    // Has the node got a write callback?
    if (node->write != 0)
        return node->write(node, offset, size, buffer);
    else
        return 0;
}

void open_fs(fs_node_t *node, u8int read, u8int write)
{
    // Has the node got an open callback?
    if (node->open != 0)
        return node->open(node);
}

void close_fs(fs_node_t *node)
{
    // Has the node got a close callback?
    if (node->close != 0)
        return node->close(node);
}

struct dirent *readdir_fs(fs_node_t *node, u32int index)
{
    // Is the node a directory, and does it have a callback?
    if ( (node->flags&0x7) == FS_DIRECTORY &&
         node->readdir != 0 )
        return node->readdir(node, index);
    else
        return 0;
}

fs_node_t *finddir_fs(fs_node_t *node, char *name)
{
    // Is the node a directory, and does it have a callback?
    if ( (node->flags&0x7) == FS_DIRECTORY &&
         node->finddir != 0 )
        return node->finddir(node, name);
    else
        return 0;
}
