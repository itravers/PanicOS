#include<kernel/tty.h>

extern int seconds_passed;

int main_initialize(){

   while(seconds_passed < 1){
    terminal_getRow();
  }
  //print the welcome screen for 2 seconds and then display a prompt
	//printf("\nWelcome To PanicOS!");
  terminal_setWelcomeScreen(); 


 // shell_initialize();
  while(seconds_passed < 2){
    terminal_getRow();
  }
  //terminal_clearScreen(); 
  //shell_initialize();
  shell_run();
}
