// Moves the characters from source to the characters in dest
void strcpy(char dest[], const char source[]) 
{
    int i = 0;
    while ((dest[i] = source[i]) != '\0')
    {
        i++;
    } 
}
