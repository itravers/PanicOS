/**
 * Author: Isaac Assegai
 * Date  : 6/1/2015
 * Define abort, which is used to kernel panic. We'll add
 * a proper kernel panic later.
 */
#include <stdlib.h>
#include <stdio.h>
#include <stdlib.h>

// Write a byte out to the specified port.
void outb(u16int port, u8int value)
{
    asm volatile ("outb %1, %0" : : "dN" (port), "a" (value));
}

// Reads a bit from the specified port
u8int inb(u16int port)
{
    u8int ret;
    asm volatile("inb %1, %0" : "=a" (ret) : "dN" (port));
    return ret;
}

u16int inw(u16int port)
{
    u16int ret;
    asm volatile ("inw %1, %0" : "=a" (ret) : "dN" (port));
    return ret;
}
