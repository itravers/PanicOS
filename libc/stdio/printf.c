/**
 * Author: Isaac Assegai
 * Date  : 6/1/2015
 * Defines our printf function. 
 */
#include <stdbool.h>
#include <stdarg.h>
#include <stdio.h>
#include <string.h>

static void print(const char* data, size_t data_length){
	for ( size_t i = 0; i < data_length; i++ )
		putchar((int) ((const unsigned char*) data)[i]);
}

/**
 * Allows us to add variables in our print output.
 * If we add a %s specifier to our string, and add a 2nd string argument
 * when calling the function then the 2nd string will be inserted
 * at the location of the %s specifier. Likewise, we can do the same for an
 * int using a %c specifier .
 */
int printf(const char* restrict format, ...){
	va_list parameters; /* Used to Store additional parameters. */
	va_start(parameters, format); /* Store additional parameters from input string. */

	int written = 0; /* How many chars have been printed so far. */
	size_t amount;
	bool rejected_bad_specifier = false; /* Used to identify unavailable conversion types. */

	while ( *format != '\0' ){ /* Check for end of string. */
		if ( *format != '%' ){ /* We don't want to print the % symbol. */
		print_c:
			amount = 1;
			while ( format[amount] && format[amount] != '%' )
				amount++;
			print(format, amount);
			format += amount; /* Update the character pointer. */
			written += amount; /* Update num chars written. */
			continue;
		}
		const char* format_begun_at = format; /* Save the start of the string. */

		if ( *(++format) == '%' ) /* %% means we want to print % */
			goto print_c;

		if ( rejected_bad_specifier ){ /* This actually isn't a specifier, print it. */
		incomprehensible_conversion:
			rejected_bad_specifier = true;
			format = format_begun_at;
			goto print_c;
		}

		if ( *format == 'c' ){ /* Convert next argument to and int and print it. */
			format++;
			char c = (char) va_arg(parameters, int /* Convert char to int.*/);
			print(&c, sizeof(c));
		}
		else if ( *format == 's' ){ /* Print String in next argument. */
			format++;
			const char* s = va_arg(parameters, const char*);
			print(s, strlen(s));
		}
		else{ /* We don't recognize this specifier. */
			goto incomprehensible_conversion;
		}
	}
	va_end(parameters); /* Close up the parameter list. */
	return written; /* Return Num Chars Written. */
}
