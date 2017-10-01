/**
 * Author: Isaac Assegai
 * Date  : 8/28/2017
 * mm.c Figures out where usable ram is.
 * Uses the multiboot header and symbols from the linker script
 * to figure out where usable ram is that will not write over
 * kernel ram.
*/

#include <kernel/multiboot.h>
#include <stdbool.h> //for bool type
#include <stdlib.h> //for roundUp
#include <stddef.h> //for size_t

/* The multiboot_info passed into kernel main */
extern struct multiboot_info* mbt;

/* &end is defined in linker.ld, it is the end of the kernel */
extern void end;
extern void code;

/* memLoc is a pointer to the beginning of physical memory as determined by multiboot */
void* memLoc; 

/* Used by kmalloc to keep watermark track of physical allocations. 
 * Can't seem to declare this in kheap.c, get a const error */
u32int placement_address;

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

  /* Only used to display the start location of Ram, then promptly forgotten */
  void* startOfRam = memLoc;

  /* Calculate a pointer to the end of physical memory. */
  memEndLoc = memLoc + memAmt;

  /* Add the size of the kernel to memLoc at the nearest 4kb boundry */
  memLoc += roundUp(&end - &code, 4096);  
  
  printf("\nEnd of Kernel           : 0x%x", &end);
  printf("\nStart of Kernel         : 0x%x", &code);
  printf("\nSize of Kernel          : 0x%x", (&end - &code));

  printf("\nMemory Found At Location: 0x%x", startOfRam);
  printf("\nMemory Amount Located   : 0x%x", memAmt);
  printf("\nEnd of Phyisical Memory : 0x%x", memEndLoc);
  printf("\nUsable Memeory Starts At: 0x%x", memLoc);
  placement_address = memLoc;
}
