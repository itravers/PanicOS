/* PanicOS - By Isaac Assegai
 * based on bkerndev - Bran's Kernel Development Tutorial
 */

#include <kernel/multiboot.h>

/*
typedef struct multiboot_memory_map {
  unsigned int size;
  unsigned int base_addr_low,base_addr_high;
// You can also use: unsigned long long int base_addr; if supported.
  unsigned int length_low,length_high;
// You can also use: unsigned long long int length; if supported.
  unsigned int type;
} multiboot_memory_map_t;
*/

unsigned char inportb (unsigned short _port)
{
    unsigned char rv;
    __asm__ __volatile__ ("inb %1, %0" : "=a" (rv) : "dN" (_port));
    return rv;
}

void outportb (unsigned short _port, unsigned char _data)
{
    __asm__ __volatile__ ("outb %1, %0" : : "dN" (_port), "a" (_data));
}

//int main(multiboot* multiboot, unsigned int magic){
int main(struct multiboot_info* mbt, unsigned int magic){
//int main(){
	gdt_install();
	idt_install();
  isrs_install();
  irq_install();
  terminal_initialize();
  timer_install();
  keyboard_install();
  __asm__ __volatile__ ("sti");

	/*for(int i = 0; i < 20; i++){
		printf("Hello World!\n");
	}*/
//	setWelcomeScreen();

  //memory testing
  printf("\nmbt->flags: 0x%x", mbt->flags);
  printf("\nmbt->mmap_length: 0x%x", mbt->mmap_length);
  printf("\nmbt->mem_lower: 0x%x", mbt->mem_lower);
  printf("\nmbt->mem_upper: 0x%x", mbt->mem_upper);
  printf("\nmbt->boot_loader_name: 0x%x ", mbt->boot_loader_name);
  multiboot_memory_map_t* mmap = mbt->mmap_addr;
  while(mmap < mbt->mmap_addr + mbt->mmap_length) {
    
    printf("\nmmap->addr: 0x%x ", mmap->addr);
    //printf("\nmmap->size: 0x%x", mmap->size);
    printf("\nmmap->len: 0x%x", mmap->len);
    printf("\nmmap->type: 0d%i", mmap->type);
    mmap = (multiboot_memory_map_t*) ( (unsigned int)mmap + mmap->size + sizeof(mmap->size) );
    
  }

  printf("\nmbt->mmap_addr: 0x%x ", mbt->mmap_addr);
  printf("\nmmap->size: 0x%x ", mmap->size);
  printf("\nmmap->len: 0x%x ", mmap->len);

  short* a = (short*) 0x100000;
  *a = 0xAAAA ;
  a++;
  *a = 0xBBBB;
  a++;
  *a = 0xCCCC;

  main_initialize();

    for (;;);
	return 0;
}
