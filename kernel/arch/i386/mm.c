/**
 * Author: Isaac Assegai
 * Date  : 8/28/2017
 * Provides Memory Management Control
*/

#include <kernel/multiboot.h>
#include <stdbool.h> //for bool type
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
struct mm_header{
  void* ptr;
  size_t size;
  bool used;
  void* next;
};

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

}
