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
  kernel_directory = (page_directory_t*)kmalloc_a(sizeof(page_directory_t));
  memset(kernel_directory, 0, sizeof(page_directory_t));
  current_directory = kernel_directory;
  printf("\nkernel_directory: 0x%x", kernel_directory);
  printf("\nsizeof page_directory_t %i", sizeof(page_directory_t));
  printf("\nsizeof page_t %i", sizeof(page_t));

  /* Map some pages in the kernel heap area.
     Here we call get_page but not alloc_frame. This causes page_table_t's 
     to be created where necessary. We can't allocate frames yet because they
     they need to be identity mapped first below, and yet we can't increase
     placement_address between identity mapping and enabling the heap! */
  unsigned int i = 0;
  for (i = KHEAP_START; i < KHEAP_START+KHEAP_INITIAL_SIZE; i += 0x1000){
    if(i == KHEAP_START){
      page_t* firstPageAddress = get_page(i, 1, kernel_directory);
      printf("\nfirst page Address: 0x%x", firstPageAddress);
      printf("\nfirst page content: 0x%x", *firstPageAddress);
      printf("\npage->present: 0x%x", firstPageAddress->present);
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
  printf("\nIdentity mapping to: 0x%x", placement_address);
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
  switch_page_directory(kernel_directory);


  // Initialise the kernel heap.
  kheap = create_heap(KHEAP_START, KHEAP_START+KHEAP_INITIAL_SIZE, 0xCFFFF000, 0, 0);
}

/* Switch the top level page directory to a new one. */
void switch_page_directory(page_directory_t *dir){
  printf("\nswitch_page_directory");
  printf("\n &dir->tablesPhysical: 0x%x", &dir->tablesPhysical);
  printf("\n &dir->physicalAddr: 0x%x", &dir->physicalAddr);
  printf("\n &dir->tables[0]: 0x%x", &dir->tables[0]);
  current_directory = dir;
  asm volatile("mov %0, %%cr3":: "r"(&dir->tablesPhysical));
  u32int cr0;
  asm volatile("mov %%cr0, %0": "=r"(cr0));
  cr0 |= 0x80000000; // Enable paging!
  asm volatile("mov %0, %%cr0":: "r"(cr0));
  printf("\nafter paging, dir: 0x%x", dir);
  printf("\nkheap: 0x%x", &kheap);
  printf("\nplacement_address: x%x", placement_address);
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
    printf("\n &dir->tablesPhysical[0]: 0x%x:", &dir->tablesPhysical[0]);
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
