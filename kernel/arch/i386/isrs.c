/** isrs.c
 * Author: Isaac Assegai 
 * Date: 5/18/2015
 * Handles Installation, Registration, and Usage of Interrupt Service Routines
 * Based on Brans Kernel Development Tutorial
 */

#include <kernel/isrs.h>
#include <kernel/idt.h> //for idt_set_gate
#include "regs.h"
#include <stdio.h> //for puts

/* Array of registered interrupt handlers. */
void* interrupt_handlers[256];

/* These are function prototypes for all of the exception
   handlers: The first 32 entries in the IDT are reserved
   by Intel, and are designed to service exceptions! */
extern void isr0();
extern void isr1();
extern void isr2();
extern void isr3();
extern void isr4();
extern void isr5();
extern void isr6();
extern void isr7();
extern void isr8();
extern void isr9();
extern void isr10();
extern void isr11();
extern void isr12();
extern void isr13();
extern void isr14();
extern void isr15();
extern void isr16();
extern void isr17();
extern void isr18();
extern void isr19();
extern void isr20();
extern void isr21();
extern void isr22();
extern void isr23();
extern void isr24();
extern void isr25();
extern void isr26();
extern void isr27();
extern void isr28();
extern void isr29();
extern void isr30();
extern void isr31();

/* This is a very repetitive function... it's not hard, it's
   just annoying. As you can see, we set the first 32 entries
   in the IDT to the first 32 ISRs. We can't use a for loop
   for this, because there is no way to get the function names
   that correspond to that given entry. We set the access
   flags to 0x8E. This means that the entry is present, is
   running in ring 0 (kernel level), and has the lower 5 bits
   set to the required '14', which is represented by 'E' in
   hex. */
void isrs_install(){
  idt_set_gate(0, (unsigned)isr0, 0x08, 0x8E);
  idt_set_gate(1, (unsigned)isr1, 0x08, 0x8E);
  idt_set_gate(2, (unsigned)isr2, 0x08, 0x8E);
  idt_set_gate(3, (unsigned)isr3, 0x08, 0x8E);
  idt_set_gate(4, (unsigned)isr4, 0x08, 0x8E);
  idt_set_gate(5, (unsigned)isr5, 0x08, 0x8E);
  idt_set_gate(6, (unsigned)isr6, 0x08, 0x8E);
  idt_set_gate(7, (unsigned)isr7, 0x08, 0x8E);

  idt_set_gate(8, (unsigned)isr8, 0x08, 0x8E);
  idt_set_gate(9, (unsigned)isr9, 0x08, 0x8E);
  idt_set_gate(10, (unsigned)isr10, 0x08, 0x8E);
  idt_set_gate(11, (unsigned)isr11, 0x08, 0x8E);
  idt_set_gate(12, (unsigned)isr12, 0x08, 0x8E);
  idt_set_gate(13, (unsigned)isr13, 0x08, 0x8E);
  idt_set_gate(14, (unsigned)isr14, 0x08, 0x8E);
  idt_set_gate(15, (unsigned)isr15, 0x08, 0x8E);

  idt_set_gate(16, (unsigned)isr16, 0x08, 0x8E);
  idt_set_gate(17, (unsigned)isr17, 0x08, 0x8E);
  idt_set_gate(18, (unsigned)isr18, 0x08, 0x8E);
  idt_set_gate(19, (unsigned)isr19, 0x08, 0x8E);
  idt_set_gate(20, (unsigned)isr20, 0x08, 0x8E);
  idt_set_gate(21, (unsigned)isr21, 0x08, 0x8E);
  idt_set_gate(22, (unsigned)isr22, 0x08, 0x8E);
  idt_set_gate(23, (unsigned)isr23, 0x08, 0x8E);

  idt_set_gate(24, (unsigned)isr24, 0x08, 0x8E);
  idt_set_gate(25, (unsigned)isr25, 0x08, 0x8E);
  idt_set_gate(26, (unsigned)isr26, 0x08, 0x8E);
  idt_set_gate(27, (unsigned)isr27, 0x08, 0x8E);
  idt_set_gate(28, (unsigned)isr28, 0x08, 0x8E);
  idt_set_gate(29, (unsigned)isr29, 0x08, 0x8E);
  idt_set_gate(30, (unsigned)isr30, 0x08, 0x8E);
  idt_set_gate(31, (unsigned)isr31, 0x08, 0x8E);
}

/* This is a simple string array. It contains the message that
   corresponds to each and every exception. We get the correct
   message by accessing like:
   exception_message[interrupt_number] */
char *exception_messages[] = {
  "Division By Zero",
  "Debug",
  "Non Maskable Interrupt",
  "Breakpoint",
  "Into Detected Overflow",
  "Out of Bounds",
  "Invalid Opcode",
  "No Coprocessor",
  "Double Fault 007",
  "Coprocessor Segment Overrun",
  "Bad TSS",
  "Segment Not Present",
  "Stack Fault",
  "General Protection Fault",
  "Page Fault!",
  "Unknown Interrupt",
  "Coprocessor Fault",
  "Alignment Check",
  "Machine Check",
  "Reserved",
  "Reserved",
  "Reserved",
  "Reserved",
  "Reserved",
  "Reserved",
  "Reserved",
  "Reserved",
  "Reserved",
  "Reserved",
  "Reserved",
  "Reserved",
  "Reserved"
};

/* Register a function as an interrupt handler, for interrupt num n. */
//void register_interrupt_handler(int n, void(*handler)(struct regs *r)){
void register_interrupt_handler(int n, handler handler){
  interrupt_handlers[n] = handler;
}

/* All of our Exception handling Interrupt Service Routines will
   point to this function. This will tell us what exception has
   happened! Right now, we simply halt the system by hitting an
   endless loop. All ISRs disable interrupts while they are being
   serviced as a 'locking' mechanism to prevent an IRQ from
   happening and messing up kernel data structures */
void fault_handler(struct regs *r){  
  void (*handler)(struct regs *r);
 
  //If there is an interrupt_handler registered for this interrupt, call it.
  if(interrupt_handlers[r->int_no] != 0){
   handler = interrupt_handlers[r->int_no];
   handler(r);
   //printf("isrs handler for r->int_no= %i done", r->int_no);
  }
    
  if(r->int_no < 32){
    puts("\nException. System Halted!");
    puts(exception_messages[r->int_no]);
    for (;;);
  }
}
