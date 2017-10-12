/**task.c
 * Author: Isaac Assegai
 * Date: 10/11/2017
 * Defines the functions needed to implement tasking
 * Based on the Tutorial by James Molloy
 */

#include <kernel/task.h>
#include <kernel/paging.h>

/* The currently Running Task. */
volatile task_t* current_task;

/* The start of the task linked list. */
volatile task_t* ready_queue;


/* Externs defined in other files. */
extern page_directory_t* kernel_directory; //from paging.c
extern page_directory_t* current_directory; //from paging.c
extern void alloc_frame(page_t* page, int is_kernel, int is_writable);
extern u32int initial_esp; //from kernel.c
extern u32int read_eip(); //from process.asm
extern void perform_task_switch(u32int eip, u32int physicalAddr, u32int ebp, u32int esp);

/* The next available process id. */
u32int next_pid = 1;

/* Initialize the tasking system, creates one blank task. */
void tasking_initialize(){
  //disable interrupts
  asm volatile("cli");
 
  //Relocate the stack, so we know where it is.
  move_stack((void*)0xE0000000, 0x2000);
  printf("\ntasking_initialize() 1"); 
 
  //Initialize the first task (kernel task)
  current_task = ready_queue = (task_t*)kmalloc(sizeof(task_t));
  current_task->id = next_pid++; //set id and increment next_pid
  current_task->esp = current_task->ebp = 0; //initialize esp & ebp to 0
  current_task->eip = 0; //initialize instruction pointer to 0
  current_task->page_directory = current_directory;//set page directory
  current_task->next = 0;//so far this is the only task

  //Reenable Interrupts
  asm volatile("sti");

  printf("\ntasking_initialize() 2"); 
}

/* Create a new process. Clone the address space of parent process
   And start the new process running at the same place the 
   parent process is currently at. */
int fork(){
//  printf("\nfork() 1");
  //Disable Interrupts
  asm volatile("cli");

//  printf("\nfork() 2");
  //Create a pointer to the parents process' task struct for later reference
  task_t* parent_task = (task_t*)current_task;

  //Clone the parents address space
  page_directory_t* directory = clone_directory(current_task->page_directory);

//  printf("\nfork() 3");
  //create a new processes
  task_t* new_task = (task_t*)kmalloc(sizeof(task_t));
  new_task->id = next_pid++; //set the id and increase next_pid;
  new_task->esp = new_task->ebp = 0; //initialize stack pointers to 0
  new_task->eip = 0; //initialize instruction pointer to 0
  new_task->page_directory = directory;
  new_task->next = 0; //this is the latest task

  //Add this new task to the end of the ready_queue so it can be processed
  task_t* tmp_task = (task_t*)ready_queue;
  while(tmp_task->next){
    tmp_task = tmp_task->next;
  }
  tmp_task->next = new_task;

//  printf("\nfork() 4");
  //get the instruction pointer (code entry point) for the new process
  u32int eip = read_eip();
  
  //after read_eip, we will either be the parent, or the child task
  //we need to check
  if(current_task == parent_task){
    //we are still the parent_task
    //We need to set up the esp, ebp and eip for our new child
    u32int esp; asm volatile("mov %%esp, %0" : "=r" (esp)); //read esp register into esp variable
    u32int ebp; asm volatile("mov %%ebp, %0" : "=r" (ebp)); //read ebp register into variable.
    new_task->esp = esp;
    new_task->ebp = ebp;
    new_task->eip = eip;

  //printf("\nfork() 5");
    //re-enable interrupts
    asm volatile("sti");
    
    //return the pid of the new child task
    return new_task->id;
  }else{
    //we have been forked and are the child task
    //therefore fork is done
    return 0; //the child returns 0 from fork
  }

}

void move_stack(void* new_stack_start, u32int size){
  //Allocate space for the new stack
  //printf("\nmove_stack 1"); 
  //the stack grows downward, so increment backwards from start by a page size
  for(u32int i = (u32int)new_stack_start; i >=((u32int)new_stack_start-size); i -= 0x1000){
    //set new stack pages/frames to user mode and writable
    alloc_frame(get_page(i, 1, current_directory), 0/*user-mode*/, 1/*writable*/);
    printf("\nalloc_frame at: 0x%x", i);
  }

  //printf("\nmove_stack 2"); 
  //Paging has changed, flush the TLB, read cr3 into pd_addr, and write it back
  u32int pd_addr;
  asm volatile("mov %%cr3, %0" : "=r" (pd_addr)); //read cr3 into pd_addr
  asm volatile("mov %0, %%cr3" : : "r" (pd_addr)); //write pd_addr back to cr3

  
  //printf("\nmove_stack 3"); 
  //Read the current stack & base pointers
  u32int old_stack_pointer; asm volatile("mov %%esp, %0" : "=r" (old_stack_pointer));
  u32int old_base_pointer;  asm volatile("mov %%ebp, %0" : "=r" (old_base_pointer));


  //printf("\nmove_stack 4"); 
  //calculate an offset to get from an address on the old stack to an address on the new
  u32int offset = (u32int)new_stack_start - initial_esp;
 
  //calculate new stack pointer, and base pointer
  u32int new_stack_pointer = old_stack_pointer + offset;
  u32int new_base_pointer = old_base_pointer + offset;

  printf("\n old_stack_pointer 0x%x", old_stack_pointer);
  printf("\n old_base_pointer 0x%x", old_base_pointer);
  printf("\n offset 0x%x", offset);
  printf("\n new_stack_pointer 0x%x", new_stack_pointer);
  printf("\n new_base_pointer 0x%x", new_base_pointer);

  //printf("\nmove_stack 5");
  //printf("\nmemcpy((void*)0x%x ,",new_stack_pointer);  
  //printf("\n(void*)0x%x ,",old_stack_pointer);  
  //printf("\n(%i))",(initial_esp - old_stack_pointer));  
  //copy the old stack to the new location
  memcpy((void*)new_stack_pointer, 
         (void*)old_stack_pointer,
         (initial_esp - old_stack_pointer));

  //according to the tutorial here you should backtrace through the original
  //stack looking for magic numbers and update them to new magic numbers
  //this doesn't seem like the correct thing to do 
  //So I'm leaving it out for now, however, if we start getting weird page faults
  // when returning from function, we'll try it out.
  // the bugs wiki says "If you wish to create a new kernel thread, simply decide
  // which registers it should have and point its stack pointer at its freshly
  // allocated stack


  // Backtrace through the original stack, copying new values into
  // the new stack.
  for(u32int i = (u32int)new_stack_start; i > (u32int)new_stack_start-size; i -= 4){
    u32int tmp = * (u32int*)i;
    // If the value of tmp is inside the range of the old stack, assume it is a base pointer
    // and remap it. This will unfortunately remap ANY value in this range, whether they are
    // base pointers or not.
    if (( old_stack_pointer < tmp) && (tmp < initial_esp)){
      tmp = tmp + offset;
      u32int *tmp2 = (u32int*)i;
      *tmp2 = tmp;
    }
  } 
// end of backtrace area

  
  //printf("\nmove_stack 6"); 
  //change stacks
  asm volatile("mov %0, %%esp" : : "r" (new_stack_pointer));
  asm volatile("mov %0, %%ebp" : : "r" (new_base_pointer)); 
 
  //printf("\nmove_stack 7"); 
}


/* Switch tasks to the next task on the ready_queue */
void switch_task(){
  //if we havn't initialized tasking yet, just return
  if(!current_task){
    return;
  }

  //read esp and ebp now for saving later on
  u32int esp, ebp, eip;
  asm volatile("mov %%esp, %0" : "=r" (esp));
  asm volatile("mov %%ebp, %0" : "=r" (ebp));

  // Comment is copied verbatim from JamesM's tutorial
  // Read the instruction pointer. We do some cunning logic here:
  // One of two things could have happened when this function exits - 
  //   (a) We called the function and it returned the EIP as requested.
  //   (b) We have just switched tasks, and because the saved EIP is essentially
  //       the instruction after read_eip(), it will seem as if read_eip has just
  //       returned.
  // In the second case we need to return immediately. To detect it we put a dummy
  // value in EAX further down at the end of this function. As C returns values in EAX,
  // it will look like the return value is this dummy value! (0x12345).
  eip = read_eip();

  //Were the tasks switched
  if(eip == 0x12345){
    //yes the tasks were switched, return now
    return;
  }

  //We didn't switch tasks, We need to save our register values, and then switch
  current_task->eip = eip;
  current_task->esp = esp;
  current_task->ebp = ebp;

  //get the next task to run, using a round robin approach
  current_task = current_task->next;
  
  //check if we just ran off the end of the linked list and start again at beginning
  if(!current_task) current_task = ready_queue;

  //set the cpu registers eip, esp, and ebp to the current tasks registers 
  perform_task_switch(current_task->eip, 
                     current_task->page_directory->physicalAddr,
                     current_task->ebp,
                     current_task->esp);
}

/* Returns the id of the currently running task. */
int getpid(){
  return current_task->id;
}




