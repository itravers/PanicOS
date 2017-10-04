/**
 * Author: Isaac Assegai
 * Date  : 8/27/2017
 * Controls the initial built in shell
 */

#include<kernel/fs.h> //for list_fs

char lastChar; /* The last character sent from keyboard to shell */
char* command; /* The command parsed */

extern int seconds_passed; /* From timer.c, the seconds passed since system boot, uptime*/
extern int* memLoc; /* Location of beginning of memory. Defined in mm.c. */
extern int memAmt; /* The amount of memory detected. Defined in mm.c */

/* Start the shell. */
void shell_initialize(void){
  printf("shell initialize \n");
  lastChar = 0;
}

/* Run the shell. */
void shell_run(void){
  while(1){//the shell always runs
    output_prompt();
    wait_for_command();
    process_command();
  }
}

/* Display the output prompt. */
void output_prompt(){
 // printf("\n");
  printf("\n $");
}

/* Loop until we hit the enter key. */
void wait_for_command(){
  while(lastChar != '\n'){
    terminal_getRow();//filler, not used except to cause while loop to wait
  }
  command = terminal_getCurrentRowChars();
  lastChar = 0; 
}

/* Process a shell command. */
void process_command(){
/* //we don't need this check
  if(command == 0x0){
    printf("\nCommand is Empty: 0x%x", command);
    printf("\nCommand is Empty: %s", command);
    return 0;
  }
*/

  //Trim the command to get rid of un-needed whitespace
  int originalLength = strlen(command);
  char* trimmedCommand = strtrim(command, originalLength);
  int trimmedLength = strlen(trimmedCommand);

  //printf("processing command: %s", trimmedCommand);

  // Test our trimmedCommand to see if it matches a given command, if so, executes that command
  if(strcmp("$cls", trimmedCommand) == 0){
    terminal_clearScreen();//cls clears the screen
  }else if(strcmp("$uptime", trimmedCommand) == 0){
    printf(" Uptime: %i Seconds", seconds_passed);//prints uptime in seconds
  }else if(strcmp("$help", trimmedCommand) == 0){
    printf(" Possible Commands: cls, uptime, help, mem, ls");
  }else if(strcmp("$mem", trimmedCommand) == 0){
    printf(" Memory Location: 0x%x\n", memLoc);
    printf(" Memory Amount  : 0x%x", memAmt);
  }else if(strcmp("$ls", trimmedCommand) == 0){
    list_fs();
  }else{
    printf(" Error: Command Not Found: %s", trimmedCommand);
  }
 
  //reset lastChar and command so we can get a new one with no interference
  lastChar = 0;
  command = 0;
}

/**
 * Receives characters from the keyboard
 * decides what to do with them,
 * If there are any special characters it will
 * handle them.
 * Send any normal input to the terminal to be output to screen
 */
void shell_putchar(char c){
  if(c == '\n'){
    //handle enter key
    //right now we will just terminal_newline, later we will process a string
    terminal_newLine();
  }else{
    terminal_putchar(c);
  }
  lastChar = c;
}
