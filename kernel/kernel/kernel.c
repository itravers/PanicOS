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
	printf("Hello, kernel World!\n");
}
