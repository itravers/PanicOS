/**
 * Author: Isaac Assegai
 * Date  : 6/1/2015
 * Create functions needed for writing to the terminal.
 */
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <string.h>
#include <kernel/vga.h>

size_t terminal_row;
size_t terminal_column;
uint8_t terminal_color;
uint16_t* terminal_buffer;

//uint16_t* terminal_getBuffer(){
//  return terminal_buffer;
//}

uint8_t* vgaArray_to_charArray(uint16_t* currentRowVga, int arrayLength){
  uint8_t charArray[arrayLength];
  for(int n = 0; n < arrayLength; n++){
    charArray[n] = make_charFromVgaEntry(currentRowVga[n]);
  }
  return charArray;
}

char* terminal_getCurrentRowChars(){
  uint16_t* currentRowVga[VGA_WIDTH];//array of vgaEntries in last terminal_row
  uint8_t* charArray; //create a char array the size of last terminal_row

  //first we find the index of the LAST $ in the terminalBuffer
  //loop from end of terminal buffer to beginning
  int j = terminal_row*VGA_WIDTH+terminal_column;
  //printf("\n preloop terminal_row = %i ", terminal_row);
  char currentChar;
  for(int i = j; i >=0; i--){
    //printf("%i ",i);
    currentChar = make_charFromVgaEntry(terminal_buffer[i]);
    if(currentChar == '$'){
      //printf(" i=%i ", i);
      int arrayLength = VGA_WIDTH - (i%80);
      //printf("arraylength: %i", i);  
      memcpy(currentRowVga, &terminal_buffer[i], arrayLength);
      //printf("post memcpy i=%i ", i);
      charArray = vgaArray_to_charArray(currentRowVga, arrayLength);
     // printf("\n post vgaArray_to_char i=%i ", i);
      //return charUINTArray;
      break;
    }

    //printf("\n post if i %i ", i);
    j = i;
  }

  //printf("currentChar %x", currentChar);

  //printf("post for");

  //printf("j = %i", j);
  //printf("\n postloop terminal_row: %i ", terminal_row);
  //first we test by printing the char array
  //printf("chararray: %s", charUINTArray); 


  return charArray;
}

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
}

void terminal_setcolor(uint8_t color){
	terminal_color = color;
}

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

void scroll(void){
    uint16_t blank, temp;

    /* A blank is defined as a space... we need to give it
    *  backcolor too */
    //blank = 0x20 | (attrib << 8);
	terminal_color = make_color(COLOR_LIGHT_GREY, COLOR_BLACK);
	blank = make_vgaentry('.', terminal_color);

    /* Row 25 is the end, this means we need to scroll up */
    if(terminal_row >= 25)
    {
        /* Move the current text chunk that makes up the screen
        *  back in the buffer by a line */
        temp = terminal_row - 25 + 1;
        memcpy (VGA_MEMORY, VGA_MEMORY + temp * 80, (25 - temp) * 80 * 2);

        /* Finally, we set the chunk of memory that occupies
        *  the last line of text to our 'blank' character */
        //memset (VGA_MEMORY + (25 - temp) * 80, blank, 80);
	for(int i = 0; i < 80; i++)
		terminal_putentryat(' ', terminal_color, i, 24);

	terminal_row = 25 - 1;
    }
}

void terminal_newLine(){
  terminal_column = 0;
  terminal_row++;
}

void terminal_putchar(char c){
	if(c == '\n'){
//		terminal_column = 0;
//		terminal_row++;
	  terminal_newLine();
  //}else if(c == 0x38){
   // terminal_clearScreen();
  }else{
		terminal_putentryat(c, terminal_color, terminal_column, terminal_row);
	}
	
	if(++terminal_column == VGA_WIDTH){
		terminal_column = 0;
    terminal_row++;
	}
	scroll(); //scroll if needed
}

void terminal_write(const char* data, size_t size){
	for ( size_t i = 0; i < size; i++ )
		terminal_putchar(data[i]);
}

void terminal_writestring(const char* data){
	terminal_write(data, strlen(data));
}
