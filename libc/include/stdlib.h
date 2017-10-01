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

#ifdef __cplusplus
}
#endif

#endif
