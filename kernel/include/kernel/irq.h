/** irq.h
 * Author: Isaac Assegai
 * Date: 10/6/2017
 * Declares interface needed for irq.c to function
 */
#ifndef _IRQ_H
#define _IRQ_H

#include <regs.h>

void irq_install(void);
typedef void (*irqhandler)(struct regs* r);
void irq_install_handler(int n, irqhandler callback);


#endif
