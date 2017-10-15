/**travrd_generator.c
 * Author: Isaac Assegai
 * Date: 10/13/2017
 * Generates a travRD Initial Ram Disk
 * Specifically for use in PanicOS
 *
 * The travrd.img generated will look as follows
 * |4 bytes-|----------------------------------------------
 * |numFiles| header | header | header | for as many files as there are 
 * -|offset after headers|---------------------------------
 * |file1|file2|file3|file4 etc
 * --------------------------------------------------------
 */

#include <stdio.h> //For printf
#include <stdlib.h> //For malloc
#include <stdint.h> //For u8int types
#include <dirent.h> //For directory functions
#include <string.h> //for strcmp
#include <unistd.h> //For chdir()

/** Declare new types, enums, and structs */

enum FILETYPE {DIRECTORY, AFILE};

/* Used in the header section of travRD, to show where a file is */
typedef struct travrd_header{
  uint16_t   magic; //Check if we are really at a header
  char            name[64]; //Names can be 64 characters long
  int             parentNum; //the number of the parent file
  unsigned int    offset; //Offset from the start of the file section the header is located
  unsigned int    length; //The length of the file
  unsigned int    fileNum;
  FILETYPE type; //The type of file this header points to
}travrd_header_t;

/** Field Variables */

/* Tracks the number of files in the travRD*/
unsigned int numFiles = 0;

/* Space for the header of every file */
travrd_header_t* headers;

/* Array of pointers to character arrays, each child character array will
   save the contents of 1 file or directory */
char* fileContent[1024];

/* Tracks the current header number, used in setup_headers */
unsigned int currentHeader = 0;
unsigned int currentHeaderOffset = 0;
unsigned int offsetToFiles = 0;

/** Function Prototypes. */
unsigned int count_entries(const char* dirName);
void setup_headers(const char* dirName, int parentNum);
int setup_headers_directory(const char* dirName, int parentNum);
void setup_headers_file(const char* fileName, int parentNum);
void print_header(travrd_header_t h);

/** Function Definitions. */

int main(int argc, char **argv){

  //if the directory to work on has been passed in, change to it
  if(argc == 2){
    chdir(argv[1]);
  }

  printf("PACKING FILESYSTEM FROM DIR: %s\n", get_current_dir_name());
    
  //printf("travrd_header size: %u bytes \n", sizeof(travrd_header));
  //printf("unsigned int size: %u bytes \n", sizeof(unsigned int));
  //printf("FILETYPE size: %d bytes \n", sizeof(FILETYPE));
  printf("==================================\n");

  //Count the number of files in the travrd file system
  numFiles = count_entries("files");

  //allocate room for all the travrd headers
  headers = (travrd_header_t*)malloc(numFiles * sizeof(travrd_header));

  //set memory in headers to all 0
  memset(headers, 0, numFiles * sizeof(travrd_header));

  //allocate enough room for all the pointers needed in fileContent
  int allocSize = (numFiles+1) * sizeof(char*);

  //set the offset to where the files start
  //which is 1 int, and numFiles travrd_headers
  offsetToFiles = sizeof(unsigned int) + (sizeof(travrd_header) * numFiles);
//  printf("offsetToFiles = 0x%x bytes\n", offsetToFiles);

  //the first headers offset will be the offsetToFiles
  currentHeaderOffset = offsetToFiles;

  //setup the file headers for files directory, and all directories below it.
  setup_headers("files", -1);    
//  printf("numHeader: %d\n", currentHeader);

  for(int i = 0; i < numFiles; i++){
  //  printf("\nfileContent[%d] :", i);
  //  printf(" %s\n", fileContent[i]);
    print_header(headers[i]);
  }//looks like headers and files are correct now

  //Open the img file, write the numFiles, then write the headers, then write the filecontents
  FILE *wstream = fopen("./travrd.img", "w");
  
  //write the number of files first
  fwrite(&numFiles, sizeof(int), 1, wstream);

  //write the headers
  fwrite(headers, sizeof(struct travrd_header), numFiles, wstream);

  //write the files
  for(int i = 0; i < numFiles; i++){
    fwrite(fileContent[i], 1, headers[i].length, wstream);
  }

  //close the img file
  fclose(wstream);


  printf("==================================\n");
  printf("\nnumFiles: %d\n", numFiles);
  return 0; 
}

void print_header(travrd_header_t h){
  printf("PRINTING HEADER =========\n");
  printf("h.magic        : 0x%x\n", h.magic);
  printf("h.name         : %s\n", h.name);
  printf("h.parentNum    : %i\n", h.parentNum);
  printf("h.offset       : 0x%x\n", h.offset);
  printf("h.length       : 0x%x\n", h.length);
  printf("h.fileNum      : %d\n", h.fileNum);
  printf("h.type         : 0x%x\n", h.type);
  printf("=========================\n");
}

/* Counts the total number of directories & files 
   under a given directory */
unsigned int count_entries(const char* dirName){
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

/* Sets up the headers array for the current directory
 * If parent name is NULL it means this is the top
 * level directory of the travRD filesystem.*/
void setup_headers(const char* dirName, int parentNum){
  DIR* d;
  struct dirent* dir;
  d = opendir(dirName);
  int thisFileNum = setup_headers_directory(dirName, parentNum);
  if(d){
    while((dir = readdir(d)) != NULL){
      if(dir->d_type == DT_DIR &&
          strcmp(dir->d_name, ".") != 0 &&
          strcmp(dir->d_name, "..") != 0){
        //recursively call this function on next directory
        chdir(dirName);
        setup_headers(dir->d_name, thisFileNum);
      }else if(dir->d_type == DT_REG){
        chdir(dirName);
        setup_headers_file(dir->d_name, thisFileNum);
        chdir("..");
      }
    }
    //printf("\n\n CLOSE DIR \n\n"); 
    //closedir(d);
  }
  if(parentNum == -1){
    //we are the top level directory, don't chdir to parent
  }else{
    //printf("cd parent: %s\n\n", parentName);
    chdir("..");
  }
}

int setup_headers_directory(const char* dirName, int parentNum){
  //printf("\nsetting headers for directory: %s\n", dirName);
  DIR* d;
  struct dirent* dir;

  //get the size of all the strings in the directory
  int size = 0;
  //open the directory and loop through entries summing size of each
  d = opendir(dirName);
  if(d){
    while((dir = readdir(d)) != NULL){
      //we don't want to include . or .. in our calculations
      if(strcmp(dir->d_name, ".") != 0 && strcmp(dir->d_name, "..") != 0){
        //get the size of each name and add it to size + 1 for a seperator we'll insert later
        int sizeOfString = ((strlen(dir->d_name)+1) * sizeof(char));
        size += sizeOfString;
      }
    }
  }//end getting size of entries
  closedir(d);//close the directory

  //now that we have the size of the directory, we can allocate enough
  //room in a char* to contain all the information
  char* directoryContent = (char*)malloc((size+2)); //+2 so we can insert a - and  '\0' 
  //printf("allocating %d size for directoryContent\n", ((size+2))); 
  strcpy(directoryContent, "-");  

  //now loop through the directory again, and concatonate each name to directoryInfo
  // with a seperator, lets use -
  d = opendir(dirName);
  if(d){
    while((dir = readdir(d)) != NULL){
      //again, we don't want to include . or .. in our string
      if(strcmp(dir->d_name, ".") != 0 && strcmp(dir->d_name, "..") != 0 ){
        strcat(directoryContent, dir->d_name);
        strcat(directoryContent, "-");
      }
    }
    strcat(directoryContent, "\0");
  }
  closedir(d);
  
  //printf("DIRCONTENT[%d] %s\n", currentHeader, directoryContent);
  fileContent[currentHeader] = directoryContent;
  
  headers[currentHeader].magic = 0xBABE;
  strcpy(headers[currentHeader].name, dirName);
  headers[currentHeader].parentNum = parentNum;
  //strcpy(headers[currentHeader].parentName, parentName);
  headers[currentHeader].offset = currentHeaderOffset;
  headers[currentHeader].length = size;
  headers[currentHeader].fileNum = currentHeader;
  headers[currentHeader].type = DIRECTORY;

  int thisFileNum = currentHeader;

  //increase currentHeaderOffset here
  currentHeaderOffset += size; 
  currentHeader++;
  return thisFileNum;
}

void setup_headers_file(const char* fileName, int parentNum){
  //printf("setting headers for file     : %s", fileName);
  //printf(" : parent - %d\n", parentNum);
    
  //get file size here
  int fileSize = 0;
  
  //open the file and seek to the end
  FILE *stream = fopen(fileName, "r");
  if(stream == 0){
    printf("Error: file not found: %s\n", fileName);
    return;
  }else{
    //printf("FILE FOUND: %s\n", fileName);
    fseek(stream, 0, SEEK_END);
    fileSize = ftell(stream);
    fclose(stream);
  } 

  unsigned char* content = (unsigned char*)malloc(fileSize+2); //+2 so we can insert a - and  '\0'
  //printf(" Allocating room for file: %d\n", fileSize+2); 

  //now that we have an allocated char* of correct size, read file into it
  FILE* readstream = fopen(fileName, "r");
  if(stream == 0){
    printf("Error: can't open file: %s\n", fileName);
    return;
  }else{
    //printf("READING FROM FILE: %s\n", fileName);
    fread(content, 1, fileSize, readstream);
  } 

  //printf("FILE CONTENTS[%d]: %s\n", currentHeader, content);
  fileContent[currentHeader] = (char*) content;
  fclose(readstream);

  //setup header for file
  headers[currentHeader].magic = 0xBABE;
  strcpy(headers[currentHeader].name, fileName);
  //strcpy(headers[currentHeader].parentName, parentName);
  headers[currentHeader].parentNum = parentNum;
  headers[currentHeader].offset = currentHeaderOffset;
  headers[currentHeader].length = fileSize;
  headers[currentHeader].fileNum = currentHeader;
  headers[currentHeader].type = AFILE;

  //increase currentHeaderOffset Here
  currentHeaderOffset += fileSize;
  currentHeader++;
}

