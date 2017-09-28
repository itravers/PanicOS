/* PanicOS - By Isaac Assegai
 * based on bkerndev - Bran's Kernel Development Tutorial
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

int main(){
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
	setWelcomeScreen();

    for (;;);
	return 0;
}
