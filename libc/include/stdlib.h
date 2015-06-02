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

#ifdef __cplusplus
}
#endif

#endif
