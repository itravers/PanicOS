#include<kernel/tty.h>
#include<kernel/shell.h>

extern int seconds_passed;
extern int bss;

int main_initialize(){
  printf("\nmain_initialize()");
//  printf("bss: 0x%x \n", &bss);


   while(seconds_passed < 1){
    terminal_getRow();
  }

  printf("init terminial");
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
  return 0;
}
