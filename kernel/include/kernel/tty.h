/**
 * Author: Isaac Assegai
 * Date  : 6/1/2015
 * Declares functions that will allow us to print to a terminal.
 */
#ifndef _KERNEL_TTY_H
#define _KERNEL_TTY_H

#include <stddef.h>

void terminal_initialize(void);
void terminal_putchar(char c);
void terminal_write(const char* data, size_t size);
void terminal_writestring(const char* data);

#endif
