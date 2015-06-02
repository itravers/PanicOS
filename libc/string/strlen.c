/**
 * Author: Isaac Assegai
 * Date  : 6/1/2015
 * Count the number of characters in a string.
 */
#include <string.h>

size_t strlen(const char* string){
	size_t result = 0;
	while ( string[result] )
		result++;
	return result;
}
