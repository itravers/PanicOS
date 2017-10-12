/**
 * task.h
 * Author: Isaac Assegai
 * 10/11/2017
 * Definitions and Data Structures used to implement tasks.
 */

#ifndef TASK_H
#define TASK_H

#include <stdlib.h> //for datatypes
#include <kernel/paging.h> //for page_directory_t
#include <regs.h>

//Structure to define what a task is
typedef struct task{
  int id;                           // The Process ID
  u32int esp, ebp;                  // Stack & Base pointers
  u32int eip;                       // The Instruction Pointer
  page_directory_t* page_directory; // The Page Directory
  struct task* next;                // The next Task (linked list)
} task_t;

//Initializes the tasking system
void tasking_initialize();

// Called by the timer, this changes the running process.
void task_switch(struct regs* r);

//Forks the current process, creates a new one with different memory space
int fork();

//Moves the current processes stack to a new location
void move_stack(void* new_stack_start, u32int size);

//return the id (pid) of the current process
int getpid();

#endif
