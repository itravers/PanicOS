/**
 * Author: Isaac Assegai
 * Date  : 8/27/2017
 * Compares two strings.
 */
#include <string.h>

/**
 * Remove any trailing whitespace from a char* or char[]
 * Loop through String from end until we find a char that
 * is not whitespace.
 * Then increase index by one and null terminate the string
 */
char* strtrim(const char* s, size_t size){
  //debugprint //printf("strtrim(%s)", s);
  int foundIndex = 0;
  //start loop at size-1, because size is the null terminator
  for(int i = size-1; i > 1; i--){
    if(s[i] != ' '){
      foundIndex = i;
      //debugprint// printf("\n charFound:%x:", s[foundIndex]);
      break;
    }
  }
  foundIndex++;
  char* returnVal = s;
  returnVal[foundIndex] = '\0';
  return returnVal;
}
