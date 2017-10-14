/**travrd_generator.c
 * Author: Isaac Assegai
 * Date: 10/13/2017
 * Generates a travRD Initial Ram Disk
 * Specifically for use in PanicOS
 */

#include <stdio.h> //For printf
#include <stdlib.h> //For malloc
#include <dirent.h> //For directory functions
#include <string.h> //for strcmp
#include <unistd.h> //For chdir()

/** Declare new types, enums, and structs */

enum FILETYPE {DIRECTORY, AFILE};

/* Used in the header section of travRD, to show where a file is */
typedef struct travrd_header{
  unsigned char   magic; //Check if we are really at a header
  char            name[64]; //Names can be 64 characters long
  unsigned int    offset; //Offset from the start of the file section the header is located
  unsigned int    length; //The length of the file
  FILETYPE type; //The type of file this header points to
}travrd_header_t;

/** Field Variables */

/* Tracks the number of files in the travRD*/
unsigned int numFiles = 0;

/* Space for the header of every file */
travrd_header_t* headers;

unsigned int offsetToFiles = 0;
/** Function Prototypes. */
void read_directory(char* dirName, char* parentName);
unsigned int count_entries(char* dirName);
void read_file(char* fileName);

/** Function Definitions. */

int main(char argc, char **argv){
  printf("travrd_header size: %d bytes \n", sizeof(travrd_header));
  printf("FILETYPE size: %d bytes \n", sizeof(FILETYPE));
  printf("==================================\n");

  //Count the number of files in the travrd file system
  numFiles = count_entries("files");

  //allocate room for all the travrd headers
  headers = (travrd_header_t*)malloc(numFiles * sizeof(travrd_header));

  //set memory in headers to all 0
  memset(headers, 0, numFiles * sizeof(travrd_header));

  //set the offset to where the files start
  //which is 1 int, and numFiles travrd_headers
  offsetToFiles = sizeof(unsigned int) + (sizeof(travrd_header) * numFiles);
  printf("offsetToFiles = 0x%x bytes\n", offsetToFiles);













  printf("==================================\n");
  printf("\nnumFiles: %d\n", numFiles);

  return 0; 
}


/* Counts the total number of directories & files 
   under a given directory */
unsigned int count_entries(char* dirName){
  unsigned int count = 0;
  DIR* d;
  struct dirent* dir;
  d = opendir(dirName);
  if(d){
    while((dir = readdir(d)) != NULL){
       if(dir->d_type == DT_DIR && 
            strcmp(dir->d_name, ".") != 0 &&
            strcmp(dir->d_name, "..") != 0){
         int ret =  chdir(dirName);
         count++;
         count += count_entries(dir->d_name);
         //printf("found: %s\n", dir->d_name);
      }else if(dir->d_type == DT_REG){
        count++;
      }
    }
    closedir(d);
  }
  if(strcmp(dirName, "files") == 0){
    //dont chdir to parent
  }else{
    chdir("..");
  }
  return count;
}

void read_directory(char* dirName, char* parentName){
  DIR* d;
  struct dirent* dir;
  d = opendir(dirName);
  if(d){
    if(strcmp(dirName, "files") == 0){
    
    }else{
      printf("READING DIRECTORY %s\n", dirName);
    }
    while((dir = readdir(d)) != NULL){
      //read all children directories, but not . or ..
      //printf("ORDER: %s\n", dir->d_name);
       if(dir->d_type == DT_DIR && 
            strcmp(dir->d_name, ".") != 0 &&
            strcmp(dir->d_name, "..") != 0){
        int ret =  chdir(dirName);
       // printf("chdir ret: %d", ret);
         read_directory(dir->d_name, dirName);
         //printf("found: %s\n", dir->d_name);
      }else if(dir->d_type == DT_REG){
        read_file(dir->d_name);
      }
    }
    closedir(d);
  }
  if(strcmp(dirName, "files") == 0){
    //dont chdir to parent
  }else{
    chdir("..");
  }
  return;
}

void read_file(char* fileName){
  printf("READING FILE      %s\n", fileName);
  return;
}

















