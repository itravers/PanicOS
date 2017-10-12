/* PanicOS - By Isaac Assegai
 * 5/13/2015
 * Called by start.asm
 * Responsible for initialzing the Kernel
 */

/* Include Kernel Headers */
#include <kernel/multiboot.h>
#include <kernel/fs.h>
#include <kernel/serial.h>
#include <kernel/main.h>//For main_initialize
#include <kernel/initrd.h> //For initrd_initialize
#include <kernel/paging.h> //for paging_initialize
#include <kernel/task.h> //for tasking_initialize
#include <kernel/mm.h> //For mm_initialize
#include <kernel/kb.h> //For keyboard_install
#include <kernel/timer.h> //For timer_install
#include <kernel/tty.h> //For terminal_initialize
#include <kernel/irq.h> //For irq_install
#include <kernel/isrs.h> //For isrs_install
#include <kernel/idt.h> //For idt_install
#include <kernel/gdt.h> //For gdt_install

/* Include Standard Headers */
#include <stdlib.h>
#include <stdio.h> //For printf


//#include <kernel/paging.h>

/* Structure passed in by start.asm. Used to access bootloader info
   We use this in mm.c to find the location of usable memory. */
struct multiboot_info* mbt;
u32int initrd_location;

/*Passed in by start.asm. Tells us the physical location
  of the initial stack. */
unsigned int initial_esp;

/**
 * Reads a single byte from an 8-bit port
 */
unsigned char inportb (unsigned short _port){
  unsigned char rv;
  __asm__ __volatile__ ("inb %1, %0" : "=a" (rv) : "dN" (_port));
  return rv;
}

/**
 * Writes a single byte to an 8-bit port
 */
void outportb (unsigned short _port, unsigned char _data){
  __asm__ __volatile__ ("outb %1, %0" : : "dN" (_port), "a" (_data));
}

/**
 * Initialze the kernel
 * Input: mbtt - Info passed into kernel from bootloader
 *        defined in multiboot.h
 */
int main(struct multiboot_info* mbtt, unsigned int magic, unsigned int initial_stack){
  mbt = mbtt; //we access mbt in mm.c
  initial_esp = initial_stack; //provide the location of the initial stack pointer

  /* Find the initrd location from mbt */
  initrd_location = *((u32int*)mbt->mods_addr);

	gdt_install();
	idt_install();
  isrs_install();
  irq_install();
  serial_initialize(SERIAL_COM1_BASE);
  terminal_initialize();
  timer_install();
  keyboard_install();
  //serial_write(SERIAL_COM1_BASE, 'h');
  mm_initialize(initrd_location);
  printf("\nMultiboot Magic 0x%x", magic);
  printf("\nMultiboot Mods Loaded: %i", mbt->mods_count);//must be after terminal_init
  printf("\nInitial Stack: 0x%x", initial_esp);

  /* Make sure the initial ram disk is loaded (initrd) */
  ASSERT(mbt->mods_count > 0);
  
  /* Start Paging */
  paging_initialize();

  /* Start multitasking */
  tasking_initialize();
  

  //Initiaze the initial ramdisk (initrd) and set it as the filesystem root
  fs_root = initrd_initialize(initrd_location);
  printf("\ninitrd_location: 0x%x \n", (unsigned int)initrd_location);
  printf("\nfs_root in init: 0x%x \n", (unsigned int)fs_root);
/*//page fault testing
  printf("\nhello paging world!");
  u32int *ptr = (u32int*)0xA0000000;
   u32int do_page_fault = *ptr;
  printf("\n0x%x", do_page_fault);
*/

/*Tasking testing
  int ret = fork();
  int pid = getpid();
  printf("\nfork() returned: %i", ret);
  printf("\ngetpid() returned: %i", pid);
  printf("\n==========================================");
*/


  asm volatile("sti");
   __asm__ __volatile__ ("sti");  //Enable Interrupts
  main_initialize();
    for (;;);
	return 0;
}
