/**
 * Author: Isaac Assegai
 * Date  : 8/27/2017
 * Compares two strings.
 */
#include <string.h>

/**
 * Compares two strings for equality
 * Returns an integer greater than, equal to, or less than zero
 * accordingly as the string tointed to by X is greater than, equal to or less than
 * the string pointed to by Y
 */

int strcmp(const char *X, const char *Y){
  while(*X){
    //if chars differ or end of 2nd string is reached
    if(*X != *Y){
      break;
    }

    //move to next pair of chars
    X++;
    Y++;
  }

  //return the difference after converting char* to unsigned char*
  return *(const unsigned char*)X - *(const unsigned char*)Y;
}

