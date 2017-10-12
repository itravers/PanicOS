#include<kernel/tty.h>
#include<kernel/shell.h>

extern int seconds_passed;
extern int bss;

int main_initialize(){
  printf("\nmain_initialize()");
  while(seconds_passed < 1){
    terminal_getRow();
  }

  printf("init terminial");
  terminal_setWelcomeScreen(); 
 
  while(seconds_passed < 2){
    terminal_getRow();
  }
  
  shell_run();
  return 0;
}
