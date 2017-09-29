extern int seconds_passed;

int main_initialize(){
  //print the welcome screen for 2 seconds and then display a prompt
	printf("Welcome To PanicOS!");
 
 // shell_initialize();
  while(seconds_passed < 1){
    terminal_getRow();
  }
  terminal_clearScreen(); 
  //shell_initialize();
  shell_run();
}
