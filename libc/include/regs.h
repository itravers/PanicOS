#include <stdlib.h>

#ifndef _REGS_H
#define _REGS_H

/* This defines what the stack looks like after an ISR was running */
struct regs
{
    unsigned int gs, fs, es, ds;      /* pushed the segs last */
    unsigned int edi, esi, ebp, /*esp*/useless_value, ebx, edx, ecx, eax;  /* pushed by 'pusha' */
    unsigned int int_no, err_code;    /* our 'push byte #' and ecodes do this */
    unsigned int eip, cs, eflags, /*useresp*/ esp, ss;   /* pushed by the processor automatically */
};

typedef struct registers
{
    u32int ds;                  // Data segment selector
    u32int edi, esi, ebp, /*esp*/useless_value, ebx, edx, ecx, eax; // Pushed by pusha.
    u32int int_no, err_code;    // Interrupt number and error code (if applicable)
    u32int eip, cs, eflags, /*useresp*/esp, ss; // Pushed by the processor automatically.
} registers_t;

#endif
