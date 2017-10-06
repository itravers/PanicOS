/**
 * Author: Isaac Assegai
 * Date  : 6/1/2015
 * Prototype functions needed for string manipulations.
 */
#ifndef _STRING_H
#define _STRING_H 1

#include <sys/cdefs.h>

#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

int memcmp(const void*, const void*, size_t);
void* memcpy(void* __restrict, const void* __restrict, size_t);
void* memmove(void*, const void*, size_t);
void* memset(void*, int, size_t);
size_t strlen(const char*);
char* reverseString(char*);
char* itoa(int, char*, int);
int strcmp(const char*, const char*);
char* strtrim(const char*, size_t);
void strcpy(char*, char*);

#ifdef __cplusplus
}
#endif

#endif
