/**
 * Author: Isaac Assegai
 * Date  : 8/28/2017
 * Provides Memory Management Control
*/

#include <kernel/multiboot.h>
#include <stdbool.h> //for bool type
#include <stdlib.h> //for roundUp
#include <stddef.h> //for size_t

/* The multiboot_info passed into kernel main */
extern struct multiboot_info* mbt;

/* memLoc is a pointer to the beginning of physical memory as determined by multiboot */
void* memLoc; 

/* memAmt is the amount of bytes in memory as determined by multiboot */
int memAmt;

/* memEndLoc is a pointer to the end of memory, as calculated in mm_initialze (memLoc+memAmt)*/
void* memEndLoc;

/* The linkedlist header used for memory allocation. */
typedef struct mm_header{
  void* ptr;
  size_t size;
  bool used;
  void* next;
};

/* Function prototypes */
void* pAlloc(unsigned int);

/**
 * Sets up memory location
 */
void mm_initialize(void){
  //Get the memory map from the multiboot_info
  multiboot_memory_map_t* mmap = mbt->mmap_addr;

  //loop through the memory map for a usable (1) portion that is bigger than 0x1000000
  while(mmap < mbt->mmap_addr + mbt->mmap_length) {
    if(mmap->type == 1 && mmap->len > 0x1000000){
      //if found, save that memLoc and memAmt for later use
      memLoc = (int*)mmap->addr;
      memAmt = mmap->len;
    }
    mmap = (multiboot_memory_map_t*) ( (unsigned int)mmap + mmap->size + sizeof(mmap->size) );
  }

  /* Calculate a pointer to the end of physical memory. */
  memEndLoc = memLoc + memAmt;
  printf("\nMemory Found At Location: 0x%x", memLoc);
  printf("\nMemory Amount Located   : 0x%x", memAmt);
  printf("\nEnd of Phyisical Memeory: 0x%x", memEndLoc);

  setupHeader(0x101010);
  //setupHeader(0x101010);  
  //void* someMemory = pAlloc(50);
  //printf("\nRETURNED"); 
 //if(someMemory != NULL)printf("\nUsing someMemory: 0x%x", someMemory);
  //pAlloc(100);
}

/**
 * Set up an ending mm_header struct at the given address
 * This represents the LAST header in the linked list
 * Therefore size is from ptr to memEndLoc
 * Therefore used if false, and next is null
 */
void setupHeader(void* l){
  printf("\nsetupHeader(0x%x)", l);
  /* Ptr points to the area directly after this header*/
  void* ptr = l+sizeof(struct mm_header);

  /* Size if the size of the memory location from end of header, to end of memory */
  size_t size = memEndLoc - ptr;

  /* A new header always has it's memory marked as unused. It's not been allocated yet*/
  bool used = false;  

  /* A new headers next is always NULL, as this is the last header in the list */
  void* next = NULL;

  //setup the beginning of a linked list at mLoc
  struct mm_header header = {ptr, size, used, next};
  
 // printf("\ndebugbefore");
 *(struct mm_header*)l = header;  //PROBLEM IS HERE
 // l = &header;
 // printf("\ndebugafter");

  //printf("\nHeaderSize: %i", sizeof(struct mm_header));
  printf("\nheader.ptr : 0x%x", header.ptr);
  printf("\nheader.size: 0x%x", header.size);
  printf("\nheader.used: 0x%x", header.used);
  printf("\nheader.next: 0x%x", header.next);

  struct mm_header* h = l;  
  
  printf("\nh->ptr : 0x%x", h->ptr);
  printf("\nh->size: 0x%x", h->size);
  printf("\nh->used: 0x%x", h->used);
  printf("\nh->next: 0x%x", h->next);
  return;

//  printf("\nmemLoc: %x", ((struct mm_header*)memLoc)->size);

}

/**
 * Allocate amt of memory, to the nearest 4KB
 * Prereq: A mm_header has been setup at memLoc
 * 1. Follow list until we find an unused entry w/ enough space
 *    If we don't find it, return NULL to signify allocation failure.
 *    if we do find it, set used, size and next
 *    then setup next's header
 *    then return ptr address of found item
 */
void* pAlloc(unsigned int amt){
  printf("\npAlloc(%i)", amt);
  
  //Cast memLoc to a mm_header we can use for the search
  //struct mm_header* header = ((struct mm_header*)memLoc);
  struct mm_header* header = memLoc;
  //printf("\nheader->size: %x", header->size);

  printf("\nDEBUG");
  /* calculate size needed to satisfy request
     first round amt to nearest 4kb boundry
     then add size needed to create another header */
  unsigned int r_amt = roundUp(amt, 4096);
  unsigned int sizeNeeded = r_amt + sizeof(struct mm_header); 

  //printf("\namt  : %i", amt); 
  //printf("\nr_amt: %i", r_amt); 

  printf("\nStarting at header: 0x%x", header->next);
  //Follow linked list at memLoc until we find unused entry with enough space
 // int i = 0;
  while(1){
  printf("\nstart while loop");
  //  i++;
    if(header->used == false && header->size >= sizeNeeded){
      //we found some free memory at header->ptr
      //set used, size & next
      header->used == true;
      header->size = r_amt;

      // if this is the last mem location header->next will be NULL
     //  if it is, we need to setup a new end header. 
     //  if it is not null, this is not the last header, so the
     //  next header has already been setup 
      if(header->next == NULL){
        header->next = header->ptr + header->size; 
        setupHeader(header->next);     
      }
 
      printf("\nFree Memory Found: 0x%x", header->ptr);
      return header->ptr;
    }else if(header->size < sizeNeeded && header->next == NULL){
      printf("\nSearched all memory, didn't find block big enough");
      return NULL;
    }else{
      //traverse rest of list
     // printf("\n Traverse - header: 0x%x ", header);
      //header = header->next;
    }
    //printf("\nheader: 0x%x", header);  
  }
  printf("\nFree Memory not Found!!!");
  return NULL;
}
