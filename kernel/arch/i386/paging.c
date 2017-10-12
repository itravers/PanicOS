/**
 * paging.c
 * Author: Isaac Assegai
 * 10/1/2017
 * Paging Implementation
 * Based on Tutorial by JamesMolloy
 */
#include <kernel/paging.h>
#include <kernel/kheap.h>
#include <kernel/isrs.h> //for register_interrupt_hander

#include <stdio.h> //for printf
#include <string.h> //for memset

/* The kernel's page directory. */
page_directory_t *kernel_directory=0;

/* The current page directory. */
page_directory_t *current_directory=0;

/* A bitset of frames - used or free. */
u32int *frames;

/* The total number of frames available in ram */
u32int nframes;

/* The address where the heap is initialized. Defined in mm.c */
extern u32int placement_address;

/* The kernel heap, defined in kheap.c */
extern heap_t *kheap;

/* defined in mm.c - The amount of memory usable for allocation, after the kernel. */
extern int memUsable;

/* Macros used in the bitset algorithms. */
#define INDEX_FROM_BIT(a) (a/(8*4))
#define OFFSET_FROM_BIT(a) (a%(8*4))

/* Function declarations */
page_table_t* clone_table(page_table_t* src, u32int* physAddr);

/* Marks a frame as used in the frames[] bitset. */
static void set_frame(u32int frame_addr){
  u32int frame = frame_addr/0x1000;
  u32int idx = INDEX_FROM_BIT(frame);
  u32int off = OFFSET_FROM_BIT(frame);
  frames[idx] |= (0x1 << off);
}

/* Marks a frame as unused in the frames[] bitset. */
static void clear_frame(u32int frame_addr){
  u32int frame = frame_addr/0x1000;
  u32int idx = INDEX_FROM_BIT(frame);
  u32int off = OFFSET_FROM_BIT(frame);
  frames[idx] &= ~(0x1 << off);
}

/* Tests if a Frame at frame_address is set in the frames[] bitset. */
/*static u32int test_frame(u32int frame_addr){
  u32int frame = frame_addr/0x1000;
  u32int idx = INDEX_FROM_BIT(frame);
  u32int off = OFFSET_FROM_BIT(frame);
  return (frames[idx] & (0x1 << off));
}*/

/* Returns index of the first unused frame in the frame[] bitset. */
static u32int first_frame(){
  u32int i, j;
  for (i = 0; i < INDEX_FROM_BIT(nframes); i++){
    if (frames[i] != 0xFFFFFFFF){ // nothing free, exit early.
      // at least one bit is free here.
      for (j = 0; j < 32; j++){
        u32int toTest = 0x1 << j;
        if (!(frames[i]&toTest)){
          return i*4*8+j;
        }
      }
    }
  }
  return -1;
}

/* Allocate a new frame */
void alloc_frame(page_t *page, int is_kernel, int is_writeable){
  if (page->frame != 0){
    return;
  }else{
    u32int idx = first_frame();
    if (idx == (u32int)-1){
      // PANIC! no free frames!!
      PANIC("NO Free Frames!");
    }
    set_frame(idx*0x1000);
    page->present = 1;
    page->rw = (is_writeable)?1:0;
    page->user = (is_kernel)?0:1;
    page->frame = idx;
  }
}

// Deallocated a previously allocated frame. */
void free_frame(page_t *page){
  u32int frame;
  if (!(frame=page->frame)){
    return;
  }else{
    clear_frame(frame);
    page->frame = 0x0;
  }
}

/* Initializes the paging system. */
void paging_initialize(){
  //Calculated the number of frames available based on the amound of memUsable
  nframes = memUsable / 0x1000;
  frames = (u32int*)kmalloc_a(INDEX_FROM_BIT(nframes));
  memset(frames, 0, INDEX_FROM_BIT(nframes));
    
  // Create the top level page directory
  //u32int phys; //Added for multitasking
  kernel_directory = (page_directory_t*)kmalloc_a(sizeof(page_directory_t));
  memset(kernel_directory, 0, sizeof(page_directory_t));
  //current_directory = kernel_directory; //removed for multitasking
  kernel_directory->physicalAddr = (u32int)kernel_directory->tablesPhysical;//added for multitasking

  //printf("\nkernel_directory: 0x%x", kernel_directory);
  //printf("\nsizeof page_directory_t %i", sizeof(page_directory_t));
  //printf("\nsizeof page_t %i", sizeof(page_t));

  /* Map some pages in the kernel heap area.
     Here we call get_page but not alloc_frame. This causes page_table_t's 
     to be created where necessary. We can't allocate frames yet because they
     they need to be identity mapped first below, and yet we can't increase
     placement_address between identity mapping and enabling the heap! */
  unsigned int i = 0;
  for (i = KHEAP_START; i < KHEAP_START+KHEAP_INITIAL_SIZE; i += 0x1000){
    if(i == KHEAP_START){
      page_t* firstPageAddress = get_page(i, 1, kernel_directory);
    //  printf("\nfirst page Address: 0x%x", firstPageAddress);
    //  printf("\nfirst page content: 0x%x", *firstPageAddress);
    //  printf("\npage->present: 0x%x", firstPageAddress->present);
    }
    get_page(i, 1, kernel_directory);
  }

  /* We need to identity map (phys addr = virt addr) from
     0x0 to the end of used memory, so we can access this
     transparently, as if paging wasn't enabled.
     NOTE that we use a while loop here deliberately.
     inside the loop body we actually change placement_address
     by calling kmalloc(). A while loop causes this to be
     computed on-the-fly rather than once at the start.
     Allocate a lil' bit extra so the kernel heap can be
     initialised properly. */
  i = 0;
  //printf("\nIdentity mapping to: 0x%x", placement_address);
    // Kernel code is readable but not writeable from userspace.
  while (i < placement_address+0x1000){
  //while (i < 1024*1024){
    alloc_frame( get_page(i, 1, kernel_directory), 0, 0);
    i += 0x1000;
  }

  // Now allocate those pages we mapped earlier.
  for (i = KHEAP_START; i < KHEAP_START+KHEAP_INITIAL_SIZE; i += 0x1000){
    alloc_frame( get_page(i, 1, kernel_directory), 0, 0);
  }

  // Before we enable paging, we must register our page fault handler.
  register_interrupt_handler(14, page_fault);
  
  // Now, enable paging!
  //switch_page_directory(kernel_directory);
  enable_paging(kernel_directory);

  // Initialise the kernel heap.
  kheap = create_heap(KHEAP_START, KHEAP_START+KHEAP_INITIAL_SIZE, 0xCFFFF000, 0, 0);

  //Added for multitasking, clone kernel_directory page dir, and switch to the clone
  current_directory = clone_directory(kernel_directory);
//    printf("\nkernel_directory: 0x%x", kernel_directory->physicalAddr);
//    printf("\ncurrent_directory: 0x%x\n", current_directory->physicalAddr);
  switch_page_directory(current_directory);

}

/* Switch the top level page directory to a new one. */
void switch_page_directory(page_directory_t *dir){
  printf("\nswitch_page_directory &tablesPhysical: 0x%x", &dir->tablesPhysical);
  printf("\nswitch_page_directory tablesPhysical: 0x%x", dir->tablesPhysical);
  printf("\nswitch_page_directory &physicalAddr: 0x%x", &dir->physicalAddr);
  printf("\nswitch_page_directory USING:  physicalAddr 0x%x", dir->physicalAddr);
  current_directory = dir;
  //asm volatile("mov %0, %%cr3":: "r"(&dir->tablesPhysical));
  asm volatile("mov %0, %%cr3":: "r"(dir->physicalAddr));//modified for tasking
  u32int cr0;
  asm volatile("mov %%cr0, %0": "=r"(cr0));
  cr0 |= 0x80000000; // Enable paging!
  asm volatile("mov %0, %%cr0":: "r"(cr0));
  printf("\nwe are paging!");  
}

/* Should only be called once. */
void enable_paging(page_directory_t *dir){
  printf("\nenable_paging USING: &tablesPhysical: 0x%x", &dir->tablesPhysical);
  printf("\nenable_paging tablesPhysical: 0x%x", dir->tablesPhysical);
  printf("\nenable_paging &physicalAddr: 0x%x", &dir->physicalAddr);
  printf("\nenable_paging physicalAddr 0x%x", dir->physicalAddr);
  current_directory = dir;
  asm volatile("mov %0, %%cr3":: "r"(&dir->tablesPhysical));
  //asm volatile("mov %0, %%cr3":: "r"(&dir->physicalAddr));//modified for tasking
  u32int cr0;
  asm volatile("mov %%cr0, %0": "=r"(cr0));
  cr0 |= 0x80000000; // Enable paging!
  asm volatile("mov %0, %%cr0":: "r"(cr0));
  printf("\nwe are paging!");  
}

/* Get a page from a page directory
 * u32int address: the address of the page to retrieve.
 * int make: should we create a page if one does not exist
 * page_directory_t *dir: the page directory to get the page from 
 */
page_t *get_page(u32int address, int make, page_directory_t *dir){
  // Turn the address into an index.
  address /= 0x1000;
  
  // Find the page table containing this address.
  u32int table_idx = address / 1024;
  if (dir->tables[table_idx]){ // If this table is already assigned
    return &dir->tables[table_idx]->pages[address%1024];
  }else if(make){
    u32int tmp;
    dir->tables[table_idx] = (page_table_t*)kmalloc_ap(sizeof(page_table_t), &tmp);
    dir->tablesPhysical[table_idx] = tmp | 0x7; // PRESENT, RW, US.
    //printf("\n &dir->tablesPhysical[0]: 0x%x:", &dir->tablesPhysical[0]);
    return &dir->tables[table_idx]->pages[address%1024];
  }else{
    return 0;
  }
}

/* Called when a page fault occurs. This function needs to be registered in the isr handler. */
void page_fault(struct regs* regs){ 
  // A page fault has occurred.
  // The faulting address is stored in the CR2 register.
  u32int faulting_address;
  asm volatile("mov %%cr2, %0" : "=r" (faulting_address));
 
  // The error code gives us details of what happened.
  int present   = !(regs->err_code & 0x1); // Page not present
  int rw = regs->err_code & 0x2;           // Write operation?
  int us = regs->err_code & 0x4;           // Processor was in user-mode?
  int reserved = regs->err_code & 0x8;     // Overwritten CPU-reserved bits of page entry?
  int id = regs->err_code & 0x10;          // Caused by an instruction fetch?
  id = id;

  // Output an error message.
  printf("\nPage fault! ( ");
  if (present) {printf("present ");}
  if (rw) {printf("read-only ");}
  if (us) {printf("user-mode ");}
  if (reserved) {printf("reserved ");}
  printf(")\n at 0x");
  printf("%x", faulting_address);
  printf("\n");
  
  if(!present && !rw && !us){
    printf("\nSupervisory process tried to read a non-present page entry.");
  }else if(!present && !rw && us){
    printf("\nSupervisory process tried to read a page and cause a protection fault.");
  }else if(!present && rw && !us){
    printf("\nSupervisory process tried to write to a non-present page entry.");
  }else if(!present && rw && us){
    printf("\nSupervisory process tried to write a page and caused a protection fault.");
  }else if(present && !rw && !us){
    printf("\nUser Process tried to read a non-present page entry.");
  }else if(present && !rw && us){
    printf("\nUser process tried to read a page and caused a protection fault.");
  }else if(present && rw && !us){
    printf("\nUer process tried to write to a non-present page entry.");
  }else if(present && rw && us){
    printf("\nUser process tried to write a page and caused a protection fault.");
  }
  printf("\n"); 
 
  PANIC("Page fault");
}

/* makes a copy of a page directory. */
page_directory_t* clone_directory(page_directory_t* src){
  //First off we need to create a new directory. Use kmalloc_ap to
  //obtain an address aligned on a page boundry, and to retrieve
  //it's physical address. Then ensure it is completely blank
  
  u32int phys;
  
  //Make a new directory & get its physical address
  page_directory_t* dir = (page_directory_t*)kmalloc_ap(sizeof(page_directory_t), &phys);
  memset(dir, 0, sizeof(page_directory_t));

  //We need the physical address of the tablesPhysical member
  u32int offset = (u32int)dir->tablesPhysical - (u32int)dir;
  dir->physicalAddr = phys + offset;

  //copy each page table, unless it is 0, then we just leave it alone, because we initialized everything to 0
  for(int i = 0; i < 1024; i++){
    if(!src->tables[i]){
      continue;
    }
 
    //test if we should link a page or copy it. We link it if it's a part of the kernel
    //its a part of the kernel if it exists in the kernel_directory
    if(kernel_directory->tables[i] == src->tables[i]){//we link the page
      //this page table exists as part of the kernel, we link it by using the same pointer
      dir->tables[i] = src->tables[i];
      dir->tablesPhysical[i] = src->tablesPhysical[i];
      //printf("clone directory is kernel_dir");
    }else{//else we copy the page
      u32int phys;
      dir->tables[i] = clone_table(src->tables[i], &phys);
      dir->tablesPhysical[i] = phys | 0x07;//set writable, usermode and present bits
    }
  }//end for loop
  printf("\nclone_directory returns dir @: 0x%x", dir);
  return dir;
}

/* Clones a page table. */
page_table_t* clone_table(page_table_t* src, u32int* physAddr){
  printf("\nclone_table");

  //create a new page table that is page aligned
  page_table_t* table = (page_table_t*)kmalloc_ap(sizeof(page_table_t), physAddr);

  //Make sure the new table is all blank
  //memset(table, 0, sizeof(page_table_t));//the tutorial shows this as sizeof(page_directory_t) which i think is wrong.
  memset(table, 0, sizeof(page_directory_t));//the tutorial shows this as sizeof(page_directory_t) which i think is wrong.

  //for every entry in the table. get a new frame, copy flags, and phyically copy the data
  for(int i = 0; i < 1024; i++){
    //if the src entry is 0, we don't need to do anything, as we already memset every location to 0
    if(!src->pages[i].frame){//this is different in tutorial src, vs examples
      printf("\nclone_table frame is 0");
      continue;
    }

    //get a new frame for this page, don't set it as kernel, or writable
    alloc_frame(&table->pages[i], 0, 0);
    
    //clone the flags from the source to the destination
    if(src->pages[i].present)   table->pages[i].present = 1;
    if(src->pages[i].rw)        table->pages[i].rw = 1;
    if(src->pages[i].user)      table->pages[i].user = 1;
    if(src->pages[i].accessed)  table->pages[i].accessed = 1;
    if(src->pages[i].dirty)     table->pages[i].dirty = 1;

    printf("\n copying frame: 0x%x", src->pages[i].frame*0x1000);
    //physically copy the data from src to table frame using process.asm
    copy_page_physical(src->pages[i].frame*0x1000, table->pages[i].frame*0x1000);
    
  }
  return table;
}









