/**
 * Author: Isaac Assegai
 * Date  : 6/1/2015
 * Define abort, which is used to kernel panic. We'll add
 * a proper kernel panic later.
 */
#include <stdlib.h>
#include <stdio.h>
#include <stdlib.h>

__attribute__((__noreturn__))
void abort(void){
	// TODO: Add proper kernel panic.
	printf("Kernel Panic: abort()\n");
	while ( 1 ) { }
	__builtin_unreachable();
}
