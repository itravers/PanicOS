/**
 * Author: Isaac Assegai
 * Date  : 6/1/15
 * This is the main kernel file. This is were the bulk of
 * the future action is going to take place.
 */
#include <stddef.h>
#include <stdint.h>
#include <string.h>
#include <stdio.h>
#include <kernel/tty.h>

void kernel_early(void){
	terminal_initialize();
}

void kernel_main(void){
	char* trans;
	printf("Hello %i there!\n", 13);

	//char* s = "Hello\0";
	//printf(reverseString(s));



/*	int charsPrinted = 0;

	for(int i = 0; i < 15; i++){
		charsPrinted += printf("%s Holy Sh*t I'm in kernel Space!!\n", 
					  "thisisarg1");
		printf("Printed %c Chars\n", i + 48);
	}
*/
	
}
