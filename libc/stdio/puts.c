/**
 * Author: Isaac Assegai
 * Date  : 6/1/2015
 * Create an alias for printf.
 */
#include <stdio.h>

int puts(const char* string){
	return printf("%s\n", string);
}
