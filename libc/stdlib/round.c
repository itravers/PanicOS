/**
 * Author: Isaac Assegai
 * Date  : 8/30/2017
 * Define different rounding functions
 */
#include <stdlib.h>
#include <stdio.h>

/**
 * Rounds an unsigned int up to the nearest multiple of a number
 */
unsigned int roundUp(unsigned int numToRound, unsigned int multiple){
  if (multiple == 0)
    return numToRound;

  int remainder = abs(numToRound) % multiple;
  if (remainder == 0)
    return numToRound;

  if (numToRound < 0)
    return -(abs(numToRound) - remainder);
  else
    return numToRound + multiple - remainder;
}
