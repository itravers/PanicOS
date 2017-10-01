/**
 * Author: Isaac Assegai
 * Date  : 6/1/2015
 * Define abort, which is used to kernel panic. We'll add
 * a proper kernel panic later.
 */
#include <stdlib.h>
#include <stdio.h>
#include <stdlib.h>

__attribute__((__noreturn__))
void abort(void){
	// TODO: Add proper kernel panic.
	printf("Kernel Panic: abort()\n");
	while ( 1 ) { }
	__builtin_unreachable();
}

extern void panic(const char *message, const char *file, u32int line)
{
    // We encountered a massive problem and have to stop.
    asm volatile("cli"); // Disable interrupts.

    printf("PANIC(");
    printf(message);
    printf(") at ");
    printf(file);
    printf(":");
    printf("%i", line);
    printf("\n");
    // Halt by going into an infinite loop.
    for(;;);
}

extern void panic_assert(const char *file, u32int line, const char *desc)
{
    // An assertion failed, and we have to panic.
    asm volatile("cli"); // Disable interrupts.

    printf("ASSERTION-FAILED(");
    printf(desc);
    printf(") at ");
    printf(file);
    printf(":");
    printf("%i", line);
    printf("\n");
    // Halt by going into an infinite loop.
    for(;;);
}
