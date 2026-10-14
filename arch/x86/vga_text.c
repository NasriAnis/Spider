#include "../../drivers/video/display.h"
#include "../../arch/x86/pio.h"

#define VGA_MEM   ((volatile unsigned char *)0xB8000)

#define CTRL_PORT 0x3D4
#define DATA_PORT 0x3D5

#define COLS      80
#define ROWS      25

#define ATTR_WHITE 0x0F
#define ATTR_RED   0x04

static unsigned char attr_of(int color) {
	return color == COLOR_RED ? ATTR_RED : ATTR_WHITE;
}

int display_cols(void) { return COLS; }
int display_rows(void) { return ROWS; }

void display_put_cell(int col, int row, char c, int color) {
	int off = 2 * (row * COLS + col);
	VGA_MEM[off]     = c;
	VGA_MEM[off + 1] = attr_of(color);
}

void display_set_cursor(int col, int row) {
	int pos = row * COLS + col;
	port_byte_out(CTRL_PORT, 14);
	port_byte_out(DATA_PORT, (unsigned char)(pos >> 8));
	port_byte_out(CTRL_PORT, 15);
	port_byte_out(DATA_PORT, (unsigned char)(pos & 0xFF));
}

void display_clear(void) {
	for (int r = 0; r < ROWS; r++)
		for (int c = 0; c < COLS; c++)
			display_put_cell(c, r, ' ', COLOR_WHITE);
	display_set_cursor(0, 0);
}

void display_scroll(void) {
	for (int i = 0; i < (ROWS - 1) * COLS * 2; i++)
		VGA_MEM[i] = VGA_MEM[i + COLS * 2];
	for (int c = 0; c < COLS; c++)
		display_put_cell(c, ROWS - 1, ' ', COLOR_WHITE);
}