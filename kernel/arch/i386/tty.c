/**
 * Author: Isaac Assegai
 * Date  : 6/1/2015
 * Create functions needed for writing to the terminal.
 */
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h> //for outb
#include <kernel/vga.h>
#include <kernel/tty.h>
#include <kernel/serial.h>

/* The current terminal row the cursor is on. */
size_t terminal_row;

/* The current terminal column the cursor is on. */
size_t terminal_column;

/* The current color (background|foreground) the terminal is. */
uint8_t terminal_color;

/* The address where the terminal is located. */
uint16_t* terminal_buffer;

void terminal_back(int);

/**
 * Moves the cursor to the correct position as given by
 * terminal_row, and terminal_column
 */
void terminal_moveCursor(void){
  unsigned temp;

  /* Find the index in memory */
  temp = terminal_row * VGA_WIDTH + terminal_column;

  /* Send a command to indicies 14 and 15 in the
   * CRT Control Register of the VGA controller
   * These are the high and low butes of the index
   * That show where the harder cursor is to be 
   * blinking */
  outb(0x3D4, 14);
  outb(0x3D5, temp >> 8);
  outb(0x3D4, 15);
  outb(0x3D5, temp);
}

/**
 * Convert an array of VGA entries to a corresponding array of chars
 * Removes the vga color info so all we have left are the chars
 */
/*uint8_t* vgaArray_to_charArray(uint8_t* charArray, uint16_t* currentRowVga, int arrayLength){
  //uint8_t charArray[arrayLength];
  for(int n = 0; n < arrayLength; n++){
    charArray[n] = make_charFromVgaEntry(currentRowVga[n]);
  }
  //charArray[arrayLength] = '\0';	
   printf(&charArray); //without this, the charArray is not recognized... 
  //return charArray;//changed this, we are passing by reference now
}*/

/* Returns a char array of all the character on the terminals current row. */
/*char* terminal_getCurrentRowChars(){
  uint16_t* currentRowVga[VGA_WIDTH];//array of vgaEntries in last terminal_row
  uint8_t* charArray; //create a char array the size of last terminal_row

  //first we find the index of the LAST $ in the terminalBuffer
  //loop from end of terminal buffer to beginning
  int j = terminal_row*VGA_WIDTH+terminal_column;
  char currentChar;
  
  for(int i = j; i >=0; i--){
    currentChar = make_charFromVgaEntry(terminal_buffer[i]);
    if(currentChar == '$'){
  //    printf("\nfound $");
      int arrayLength = VGA_WIDTH - (i%80);
    //  printf("\narrayLength: %i", arrayLength);
      memcpy(currentRowVga, &terminal_buffer[i], arrayLength);
      //charArray = vgaArray_to_charArray(currentRowVga, arrayLength);
      vgaArray_to_charArray(charArray, currentRowVga, arrayLength);
      break;
    }
    j = i;
  }
  return charArray;
}
*/

/**
 * Returns the current terminal_row of the tty
 */
size_t terminal_getRow(){
  return terminal_row;
}

/**
 * Returns the current terminal_column of the tty
 */
size_t terminal_getColumn(){
  return terminal_column;
}

/* Initialzes the kernel
 * Sets cursor location, terminal color,
 * Sets where the terminal_biffer is located
 * blanks out all the character in the terminal. */
void terminal_initialize(void){
	terminal_row = 0;
	terminal_column = 0;
	terminal_color = make_color(COLOR_LIGHT_GREY, COLOR_BLACK);
	terminal_buffer = VGA_MEMORY;
	for ( size_t y = 0; y < VGA_HEIGHT; y++ ){
		for ( size_t x = 0; x < VGA_WIDTH; x++ ){
			const size_t index = y * VGA_WIDTH + x;
			terminal_buffer[index] = make_vgaentry(' ', terminal_color);
		}
	}
  terminal_moveCursor();
}

/* Sets the terminal color. */
void terminal_setcolor(uint8_t color){
	terminal_color = color;
}

/* Puts a character of a given color, at a specific location in the terminal. */
void terminal_putentryat(char c, uint8_t color, size_t x, size_t y){
	const size_t index = y * VGA_WIDTH + x;
	terminal_buffer[index] = make_vgaentry(c, color);
}

/**
 * Clears the terminals screen
 */
void terminal_clearScreen(){
  terminal_initialize();
}

/* Scrolls the terminal based on where the cursor is. */
void scroll(void){
  uint16_t blank, temp;

  /* A blank is defined as a space... we need to give it
     backcolor too */
	terminal_color = make_color(COLOR_LIGHT_GREY, COLOR_BLACK);
	blank = make_vgaentry('.', terminal_color);
  blank = blank;//remove unused compiler warning

  /* Row 25 is the end, this means we need to scroll up */
  if(terminal_row >= 25){
    /* Move the current text chunk that makes up the screen
       back in the buffer by a line */
    temp = terminal_row - 25 + 1;
    memcpy (VGA_MEMORY, VGA_MEMORY + temp * 80, (25 - temp) * 80 * 2);

    /* Finally, we set the chunk of memory that occupies
       the last line of text to our 'blank' character */
	  for(int i = 0; i < 80; i++){
		  terminal_putentryat(' ', terminal_color, i, 24);
    }
	  terminal_row = 25 - 1;
   }
}

/* Sets terminal cursor location to a new line. */
void terminal_newLine(){
  terminal_column = 0;
  terminal_row++;
}

/* Adds a character to the next location in the terminal
 * or adds a new line, if newline character was passed in. */
void terminal_putchar(char c){
	if(c == '\n'){
	  terminal_newLine();
  }else{
		terminal_putentryat(c, terminal_color, terminal_column, terminal_row);
	}
	
	if(++terminal_column == VGA_WIDTH){
		terminal_column = 0;
    terminal_row++;
	}
	scroll(); //scroll if needed
  terminal_moveCursor();
  serial_write(SERIAL_COM1_BASE, c);
}

/* Writes a char array of a given size to the terminal. */
void terminal_write(const char* data, size_t size){
	for ( size_t i = 0; i < size; i++ )
		terminal_putchar(data[i]);
}

/* Writes a string to the terminal. */
void terminal_writestring(const char* data){
	terminal_write(data, strlen(data));
}

/* Sets the terminal back 1 space, sets that entry to ' ', then sets the terminal back 1 space again. */
void terminal_backspace(void){
  terminal_back(1);
  terminal_putchar(' ');
  terminal_back(1);
  terminal_moveCursor();
}

/* Sets the current location back x space.*/
void terminal_back(int x){
  //move to previous row if needed
  if((int)terminal_column - x < 0){
    terminal_row--;
    terminal_column = VGA_WIDTH - (x - terminal_column);
  }else{
    terminal_column -= x;
  }
}

/* Prints an ascii array to the screen */
void printScreen(char *title, int titleLength, char *subTitle, int subTitleLength, int newAttribute){
  newAttribute = newAttribute; //fix compiler nonuse warning
  /* Reset the carriage to the start of the page. */
  terminal_row = 0;
  terminal_column = 0;
  /* Save the old attribute so we can restore it after the loop. */
  //int oldAttrib = attrib;
  /*We can loop through the char Array pulling each char out
    mixing it with the cooresponding attribute then print
    the character. */

  //attrib = newAttribute;
  terminal_clearScreen();
  terminal_column = (80 / 2) - (titleLength / 2);
  terminal_row = 10;
  printf(title);

  terminal_column = (80 / 2) - (subTitleLength / 2);
  terminal_row = 14;
  printf(subTitle);

  //attrib = oldAttrib;
  terminal_row = 0;
  terminal_column = 0;
  //puts("screen printed\n");
}

void terminal_setWelcomeScreen(){
  //printf("Setting Welcome Screen\n");
  int titleLength = 7;
  char* title = "PanicOS";
  char* subTitle = "PLEASE DON'T PANIC";
  int subTitleLength = 18;
  int att =(0x04 << 4)|(0x00 & 0X0F);
//  puts(s);
  printScreen(title, titleLength, subTitle, subTitleLength, att);
}
