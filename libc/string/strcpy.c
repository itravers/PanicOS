/**strcpy.c
 * Author: Isaac Assegai
 * Date: 10/1/2017
 * Copies the characters from source to the characters in dest
 */
void strcpy(char dest[], const char source[]){
  int i = 0;
  while((dest[i] = source[i]) != '\0'){
    i++;
  } 
}
