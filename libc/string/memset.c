/**
 * Author: Isaac Assegai
 * Date  : 6/1/2015
 * Set a block of memory to a specific value. 
 */
#include <string.h>

void* memset(void* bufptr, int value, size_t size){
	unsigned char* buf = (unsigned char*) bufptr;
	for ( size_t i = 0; i < size; i++ )
		buf[i] = (unsigned char) value;
	return bufptr;
}
