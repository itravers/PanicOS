/**
 * Author: Isaac Assegai
 * Date  : 6/2/2015
 * Reverses a c-string
 */

#include <string.h>

char* reverseString(char *string){
	char temp; /* Used to hold the swap value. */
	size_t stringLength = strlen(string) - 1;
	size_t n = stringLength; /* The place we are swapping with. */
	for(size_t i = 0; i < stringLength; i++){ /* We won't go through entire loop. */
		temp = string[n]; /* Save Last Value. */
		string[n] = string[i]; /* Assign first value to last value. */
		string[i] = temp; /* Former last value is now first value. */
		n--;
		if(n == (stringLength / 2)){
			break; /* All swapping is done at this point. */
		}
	}
	return string;
}
