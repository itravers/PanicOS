/**
 * Author: Isaac Assegai
 * Date  : 6/1/2015
 * Set a block of memory to a specific value. 
 */
#include <string.h>
unsigned short *memsetw(unsigned short *dest, unsigned short val, int count)
{
    unsigned short *temp = (unsigned short *)dest;
    for( ; count != 0; count--) *temp++ = val;
    return dest;
}
