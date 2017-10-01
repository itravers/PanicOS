/**
 * Author: Isaac Assegai
 * Date  : 6/1/2015
 * Prototype function used for kernel panics.
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

#ifdef __cplusplus
}
#endif

#endif
