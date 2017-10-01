/**
 * Author: Isaac Assegai
 * Date  : 6/1/2015
 * Prototype functions 
 * Command typedefs.
 */
#ifndef _STDLIB_H
#define _STDLIB_H 1

#include <sys/cdefs.h>

#ifdef __cplusplus
extern "C" {
#endif

__attribute__((__noreturn__))
void abort(void);

/** Rounds an unsigned int up to the nearest multiple of a number*/
unsigned int roundUp(unsigned int, unsigned int);

/** Common typedefs to standardize sizes across platforms*/
typedef unsigned int u32int;
typedef int s32int;
typedef unsigned short u16int;
typedef short s16int;
typedef unsigned char u8int;
typedef char s8int;

void outb(u16int port, u8int value);
u8int inb(u16int port);
u16int inw(u16int port);

#define PANIC(msg) panic(msg, __FILE__, __LINE__);
#define ASSERT(b) ((b) ? (void)0 : panic_assert(__FILE__, __LINE__, #b))

extern void panic(const char *message, const char *file, u32int line);
extern void panic_assert(const char *file, u32int line, const char *desc);

#ifdef __cplusplus
}
#endif

#endif
