/** idt.h
 * Author: Isaac Assegai
 * Date: 10/6/2017
 * Declares interface needed for idt.c to function
 */
#ifndef _IDT_H
#define _IDT_H

void idt_install(void);
void idt_set_gate(unsigned char num, unsigned long base, unsigned short sel, unsigned char flags);

#endif
