/**
 * Author: Isaac Assegai
 * Date  : 8/28/2017
 * Provides Memory Management Control
*/

#include <kernel/multiboot.h>

/* The multiboot_info passed into kernel main */
extern struct multiboot_info* mbt;

/* The location of usable memory */
int* memLoc;
int memAmt;

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
  printf("\nMemory Found At Location: 0x%x", memLoc);
  printf("\nMemory Amount Located   : 0x%x", memAmt);

}
