/**
 * Author: Isaac Assegai
 * Date  : 6/2/2015
 * Converts an integer into its representative string 
 * in the desired base number system.
 */

#include <string.h>

char* itoa(int convertFrom, char* convertTo, int numBase){
	int i = 0;
	_Bool isNeg = 0; /* Are we converting a neg number? */
	
	if(convertFrom == 0){ /* Convert 0 to string. */
		convertTo[i] = '0';
		i++;
		convertTo[i] = '\0';
		return convertTo;
	}

	if(convertFrom < 0 && numBase == 10){ /* We only handle signed numbers in base 10. */
		isNeg = 1;
		convertFrom = -convertFrom;
	}

	/* We'll loop through each digit of the number. */
	while(convertFrom != 0){
		int newNum = convertFrom % numBase;
		if(newNum > 9){ /* This num is larger than our base. */
			convertTo[i++] = (newNum - 10) + 'a';
		}else{
			convertTo[i++] = newNum + '0';
		}
		convertFrom = convertFrom / numBase; /* change base for next loop. */
	}

	if(isNeg){ /* Neg numbers will need a - on them. */
		convertTo[i++] = '-';
	}

	convertTo[i] = '\0'; /* Terminate the String. */
	convertTo = reverseString(convertTo);
	return convertTo;
}
