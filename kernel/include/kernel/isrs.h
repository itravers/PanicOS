/** isrs.h
 * Author: Isaac Assegai
 * Date: 10/6/2017
 * Declares interface needed for isrs.c to function
 */
#ifndef _ISRS_H
#define _ISRS_H

//#include "regs.h"
#include<regs.h>

void isrs_install(void);
typedef void (*handler)(struct regs *r);
//void register_interrupt_handler(int n, void (*handler)(struct regs *r));
void register_interrupt_handler(int n, handler callback);

#endif
