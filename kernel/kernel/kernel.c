/* PanicOS - By Isaac Assegai
 * 5/13/2015
 * Called by start.asm
 * Responsible for initialzing the Kernel
 */

#include <kernel/multiboot.h>

/* Structure passed in by start.asm. Used to access bootloader info
   We use this in mm.c to find the location of usable memory. */
struct multiboot_info* mbt;

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
//int main(){
	gdt_install();
	idt_install();
  isrs_install();
  irq_install();
  terminal_initialize();
  timer_install();
  keyboard_install();
  mm_initialize();
  kheap_test(); 
  __asm__ __volatile__ ("sti");  //Enable Interrupts
  main_initialize();
    for (;;);
	return 0;
}
