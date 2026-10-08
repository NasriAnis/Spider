#include "screen.h"
#include "ports.h"
#include "../lib/string.h"

// private functions
int print_char(char charactere, int col, int row, char attribute_byte);
int get_mem_screen_offset(int col, int row);
int get_mem_cursor_offset();
void set_mem_cursor_offset(int offset);
void clear_screen();
int scroll_down(void);

void print_at (char *message, int col, int row);
void eprint_at (char *message, int col, int row);

/*
 * ################ Public acessible API functions ################
 */

/*
 * print to the screen (following cursor offset)
 * an array of chars white on black
 */
void print(char *message) { print_at(message, -1, -1); }

void printnl(char *message) {
	/*
	 * print to the screen (following cursor offset)
	 * an array of chars followed by a new line in
	 * white on black
	 */
	print_at(message, -1, -1);
	print_at("\n", -1, -1);
}

/*
 * print to the screen (following cursor offset)
 * an array of chars red on black
 */
void eprint(char *message) { eprint_at(message, -1, -1); }

void eprintnl(char *message) {
	/*
	 * print to the screen (following cursor offset)
	 * an array of chars followed by a new line in
	 * red on black
	 */
	eprint_at(message, -1, -1);
	eprint_at("\n", -1, -1);
}

static void print_attr(char *message, int col, int row, char attr) {
	if (col >= 0 && row >= 0)
		set_mem_cursor_offset(get_mem_screen_offset(col, row));
	for (int i = 0; message[i]; i++){
		print_char(message[i], -1, -1, attr);
	}
}

void print_at(char *m, int c, int r)  { print_attr(m, c, r, WHITE_ON_BLACK); }
void eprint_at(char *m, int c, int r) { print_attr(m, c, r, RED_ON_BLACK); }

/*
 * ################ Private Kernel functions ################
 */

int print_char(char charactere, int col, int row, char attribute_byte){
	unsigned char *vidmem = (unsigned char *) VIDEO_ADDRESS;

	if (!attribute_byte) {
		attribute_byte = WHITE_ON_BLACK;
	}

	int current_offset;
	if (col>=0 && row>=0){
		current_offset = get_mem_screen_offset(col, row);
	} else {
		current_offset = get_mem_cursor_offset();
	}


	if (charactere == '\n'){
		int current_row = current_offset / (2 * MAX_COLS);
		int next_row = current_row + 1;
		current_offset = get_mem_screen_offset(-1, next_row);
	} else {
		vidmem[current_offset] = charactere;
		vidmem[current_offset+1] = attribute_byte;
	}


	int next_cell_offset = current_offset + 2;
	if (next_cell_offset >= MAX_ROWS * MAX_COLS * 2) {
		next_cell_offset = scroll_down();
	}

	set_mem_cursor_offset(next_cell_offset);
	return next_cell_offset;
}

int get_mem_cursor_offset() {
	/*
	 * use vga crtr ports to get the current cursor location
	 */
	port_byte_out(REG_SCREEN_CTRL, 14);
	int offset = port_byte_in(REG_SCREEN_DATA) << 8;
	port_byte_out(REG_SCREEN_CTRL, 15);
	offset += port_byte_in(REG_SCREEN_DATA);
	return offset * 2; /* offset*2 because the offset
		is a cell 2*2 (char+attribute) */
}

void set_mem_cursor_offset(int offset) {
	/*
	 * Use vga crt ports to write cursor offset
	 */
	offset /= 2; // offset/2 because the offset is a cell 2*2 (char+attribute)
	port_byte_out(REG_SCREEN_CTRL, 14);
	port_byte_out(REG_SCREEN_DATA, (unsigned char)(offset >> 8));
	port_byte_out(REG_SCREEN_CTRL, 15);
	port_byte_out(REG_SCREEN_DATA, (unsigned char)(offset & 0xff));
}

void clear_at(int start, int size) {
	/*
	 * fill screen from start addr
	 * until addr+size with space chars
	 */
	char *start_p = (char *) start;
	for (int i = 0; i < size; i++) {
		start_p[i*2] = ' ';
		start_p[i*2+1] = WHITE_ON_BLACK;
	}
}

void clear_screen(){
	clear_at(VIDEO_ADDRESS, MAX_COLS*MAX_ROWS);
	set_mem_cursor_offset(get_mem_screen_offset(0, 0));
}

int scroll_down(void){
	/* scroll down by one row and return the last row offset */
	memcpy(
		(char*) VIDEO_ADDRESS + get_mem_screen_offset(0, 1),
		(char*) VIDEO_ADDRESS + get_mem_screen_offset(0, 0),
		(MAX_ROWS - 1) * MAX_COLS * 2
	);
	int last_row = get_mem_screen_offset(0, MAX_ROWS - 1);
	clear_at(VIDEO_ADDRESS + last_row, MAX_COLS);
	return last_row; // return the offset to the last row
}

/*
 * return the specific memory address
 * of a cell in vga address space following
 * a specific row/col cell
 */
int get_mem_screen_offset(int col, int row){ return 2 * (row * MAX_COLS + col); }