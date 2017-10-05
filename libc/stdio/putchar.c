/**
 * Author: Isaac Assegai
 * Date  : 6/1/2015
 * Put a single character to the terminal.
 */
#include <stdio.h>

#if defined(__is_myos_kernel)
#include <kernel/tty.h>
#include <kernel/serial.h>
#endif

int putchar(int ic){
#if defined(__is_myos_kernel)
	char c = (char) ic;
	terminal_write(&c, sizeof(c));
//  serial_write(SERIAL_COM1_BASE, c);
#else
	/* TODO: Implement a write system call. */
#endif
	return ic;
}
