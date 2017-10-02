/** timer.c
 * Author: Isaac Assegai
 * Date: 5/19/2015
 * This will keep track of how many ticks that the system
 * has been running for 
 * Based on osdev barebones tutorial
 */

/* The number of times timer_handler has been called since kernel booted. */
int timer_ticks = 0;

/* The number of seconds that have passed since the kernel booted. */
int seconds_passed = 0;

/* Handles the timer. In this case, it's very simple: We
   increment the 'timer_ticks' variable every time the
   timer fires. By default, the timer fires 18.222 times
   per second. */
void timer_handler(struct regs *r){
  /* Increment our 'tick count' */
  timer_ticks++;

  /* Every 18 clocks (approximately 1 second), we will
     increment seconds_passed */
  if (timer_ticks % 18 == 0){
    seconds_passed++;
  }
}

/* Sets up the system clock by installing the timer handler
   into IRQ0 */
void timer_install(){
  /* Installs 'timer_handler' to IRQ0 */
  irq_install_handler(0, timer_handler);
}

/* This will continuously loop until the given time has
   been reached */
void timer_wait(int ticks){
  unsigned long eticks;
  eticks = timer_ticks + ticks;
  while(timer_ticks < eticks);
}
