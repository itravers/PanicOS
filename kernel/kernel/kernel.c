/* bkerndev - Bran's Kernel Development Tutorial
*  By:   Brandon F. (friesenb@gmail.com)
*  Desc: Main.c: C code entry.
*
*  Notes: No warranty expressed or implied. Use at own risk. */

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

int main()
{
//    int i;

    gdt_install();
    idt_install();
    isrs_install();
    irq_install();
    terminal_initialize();
//    init_video();
//    timer_install();
//    keyboard_install();

//    __asm__ __volatile__ ("sti");

    printf("Hello World!\n");
//	setWelcomeScreen();

//    i = 10 / 0;
//    putch(i);

    for (;;);
	return 0;
}
