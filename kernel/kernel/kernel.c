/* PanicOS - By Isaac Assegai
 * 5/13/2015
 * Called by start.asm
 * Responsible for initialzing the Kernel
 */

#include <kernel/multiboot.h>
#include <kernel/fs.h>
#include <stdlib.h>
#include <kernel/serial.h>
//#include <kernel/paging.h>

/* Structure passed in by start.asm. Used to access bootloader info
   We use this in mm.c to find the location of usable memory. */
struct multiboot_info* mbt;
u32int initrd_location;

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
int main(struct multiboot_info* mbtt, unsigned int magic){
  mbt = mbtt; //we access mbt in mm.c

  /* Find the initrd location from mbt */
  initrd_location = *((u32int*)mbt->mods_addr);

	gdt_install();
	idt_install();
  isrs_install();
  irq_install();
  serial_initialize();
  terminal_initialize();
  timer_install();
  keyboard_install();
  //serial_write(SERIAL_COM1_BASE, 'h');
  mm_initialize(initrd_location);
  printf("\nMultiboot Mods Loaded: %i", mbt->mods_count);//must be after terminal_init

  /* Make sure the initial ram disk is loaded (initrd) */
  ASSERT(mbt->mods_count > 0);
  
  initialise_paging();

  //Initiaze the initial ramdisk (initrd) and set it as the filesystem root
  fs_root = initialise_initrd(initrd_location);

  printf("\nfs_root in init: 0x%x \n", fs_root);
/*//page fault testing
  printf("\nhello paging world!");
  u32int *ptr = (u32int*)0xA0000000;
   u32int do_page_fault = *ptr;
  printf("\n0x%x", do_page_fault);
*/
   __asm__ __volatile__ ("sti");  //Enable Interrupts
  main_initialize();
    for (;;);
	return 0;
}
