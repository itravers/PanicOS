/**travrd_generator.c
 * Author: Isaac Assegai
 * Date: 10/13/2017
 * Generates a travRD Initial Ram Disk
 * Specifically for use in PanicOS
 */

#include <stdio.h> //For printf

enum FILETYPE {DIRECTORY, AFILE};

/* Used in the header section of travRD, to show where a file is */
struct travrd_header{
  unsigned char magic; //Check if we are really at a header
  char name[64]; //Names can be 64 characters long
  unsigned int offset; //Offset from the start of the file section the header is located
  unsigned int length; //The length of the file
  FILETYPE type; //The type of file this header points to
};

int main(char argc, char **argv){
  printf("travrd_header size: %d bytes \n", sizeof(travrd_header));
  printf("FILETYPE size: %d bytes \n", sizeof(FILETYPE));

  return 0; 
}
