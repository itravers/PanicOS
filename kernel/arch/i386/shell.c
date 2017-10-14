/**
 * Author: Isaac Assegai
 * Date  : 8/27/2017
 * Controls the initial built in shell
 */

#include<kernel/fs.h> //for list_fs & read_fs
#include<kernel/initrd.h> //for version_node
#include<kernel/serial.h>
#include<kernel/tty.h> //terminal_ functions

#include <stdio.h> //for printf
#include <stdlib.h> //for u32int datatypes
#include <string.h> //for u32int datatypes

char lastChar; /* The last character sent from keyboard to shell */
char command_buf[256];
int buf_location = 0;
char* command; /* The command parsed */

/* An array to hold the last 8 commands. */
char* pre_command;

extern int seconds_passed; /* From timer.c, the seconds passed since system boot, uptime*/
extern u32int memLoc; /* Location of beginning of memory. Defined in mm.c. */
extern int memAmt; /* The amount of memory detected. Defined in mm.c */

//extern char module;

void process_command(void);
void wait_for_command(void);
void output_prompt(void);

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
  
  //command = terminal_getCurrentRowChars();
  command = command_buf;
  
  strcpy(pre_command, command);
  lastChar = 0;
  //printf("command is: %s", command); 
}

/* Process a shell command. */
void process_command(){
  //printf("process_command:%s", command); 
  // Test our command to see if it matches a given command, if so, executes that command
  if(strcmp("cls", command) == 0){
    terminal_clearScreen();//cls clears the screen
  }else if(strcmp("uptime", command) == 0){
    printf(" Uptime: %x Seconds", seconds_passed);//prints uptime in seconds
  }else if(strcmp("help", command) == 0){
    printf(" Possible Commands:\n");
    printf(" \t help       : Show this list \n");
    printf(" \t cls        : Clears the Screen\n");
    printf(" \t uptime     : Shows System Uptime in Seconds\n");
    printf(" \t mem        : Lists Memory/RAM Info\n");
    printf(" \t ls         : Lists the File System\n");
    printf(" \t startscreen: Shows the Start Screen\n");
    printf(" \t version    : Shows the Kernel Version\n");
    printf(" \t getpid     : Shows the Current Tasks ID");
  }else if(strcmp("mem", command) == 0){
    printf(" Memory Location: 0x%x\n", (u32int) memLoc);
    printf(" Memory Amount  : 0x%x", memAmt);
  }else if(strcmp("ls", command) == 0){
    list_fs();
    //printf("module location: 0x%x", module);
  }else if(strcmp("version", command) == 0){
    char* verBuf[256];
    u32int sz = read_fs(version_node, 0, 256, verBuf);
    printf(" PanicOS Version: %s", verBuf);
  }else if(strcmp("getpid", command) == 0){
    printf(" PID: %i", getpid());
  }else if(strcmp("startscreen", command) == 0){
    terminal_setWelcomeScreen();
  }else{
    printf(" Error: Command Not Found: %s", command);
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
    command_buf[buf_location] = '\0';
    buf_location = 0;
    terminal_newLine();
  }else if(c == '\b'){
    //don't backspace if we are at the beginning of the command buffer
    if(buf_location > 0){
      buf_location--;
      terminal_backspace();
    }
    //printf("\\b");
  }else{
    command_buf[buf_location] = c;
    buf_location++;
    terminal_putchar(c);
  }
  lastChar = c;
}
