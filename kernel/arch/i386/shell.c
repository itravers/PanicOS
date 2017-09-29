/**
 * Author: Isaac Assegai
 * Date  : 8/27/2017
 * Controls the initial built in shell
*/

char lastChar; /* The last character sent from keyboard to shell */
char* command; /* The command parsed */

void shell_initialize(void){
  printf("shell initialize \n");
  lastChar = '\p';
  
}

void shell_run(void){
  //printf("Running Shell");
  while(1){//the shell always runs
    output_prompt();
    wait_for_command();
    process_command(command);
  }
}

void output_prompt(){
  printf("\n");
  printf("$");
}

void wait_for_command(){
  while(lastChar != '\n'){
    terminal_getRow();//filler, not used except to cause while loop to wait
  //  printf("\n");
   // printf(lastChar);
  }
  //printf("while loop over");
  command = "testCommand";
  command = terminal_getCurrentRowChars();
}

void process_command(char* command){
  if(command == 0)return;
  printf("\n");
  printf("processing command: %s", command);

  printf("stringcompare: %i ", strncmp("cls", command, 3));
  //reset lastChar and command so we can get a new one with no interference
  lastChar = '\P';
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
 //printf(" \n Putting Char:");
 //terminal_putchar(c);
  if(c == '\n'){
    //handle enter key
    //right now we will just terminal_newline, later we will process a string
    terminal_newLine();
  }else{
    terminal_putchar(c);
  }


    lastChar = c;
}
