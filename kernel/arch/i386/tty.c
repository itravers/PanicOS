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

void scroll(void){
    unsigned blank, temp;

    /* A blank is defined as a space... we need to give it
    *  backcolor too */
    //blank = 0x20 | (attrib << 8);
	blank = make_vgaentry(' ', terminal_color);

    /* Row 25 is the end, this means we need to scroll up */
    if(terminal_row >= 25)
    {
        /* Move the current text chunk that makes up the screen
        *  back in the buffer by a line */
        temp = terminal_row - 25 + 1;
        memcpy (VGA_MEMORY, VGA_MEMORY + temp * 80, (25 - temp) * 80 * 2);

        /* Finally, we set the chunk of memory that occupies
        *  the last line of text to our 'blank' character */
        memset (VGA_MEMORY + (25 - temp) * 80, blank, 80);
        terminal_row = 25 - 1;
    }
}


void terminal_putchar(char c){
	if(c == '\n'){
		terminal_column = 0;
		terminal_row++;
	}else{
		terminal_putentryat(c, terminal_color, terminal_column, terminal_row);
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
