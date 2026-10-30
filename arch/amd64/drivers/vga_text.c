#include <drivers/video/display.h>
#include <pio.h>

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

int vga_cols(void) { return COLS; }
int vga_rows(void) { return ROWS; }

void vga_put_cell(int col, int row, char c, int color) {
	int off = 2 * (row * COLS + col);
	VGA_MEM[off]     = c;
	VGA_MEM[off + 1] = attr_of(color);
}

void vga_set_cursor(int col, int row) {
	int pos = row * COLS + col;
	port_byte_out(CTRL_PORT, 14);
	port_byte_out(DATA_PORT, (unsigned char)(pos >> 8));
	port_byte_out(CTRL_PORT, 15);
	port_byte_out(DATA_PORT, (unsigned char)(pos & 0xFF));
}

void vga_clear(void) {
	for (int r = 0; r < ROWS; r++)
		for (int c = 0; c < COLS; c++)
			vga_put_cell(c, r, ' ', COLOR_WHITE);
	vga_set_cursor(0, 0);
}

void vga_scroll(void) {
	for (int i = 0; i < (ROWS - 1) * COLS * 2; i++)
		VGA_MEM[i] = VGA_MEM[i + COLS * 2];
	for (int c = 0; c < COLS; c++)
		vga_put_cell(c, ROWS - 1, ' ', COLOR_WHITE);
}

int vga_backspace(int current_row, int current_col){
	if (current_row-1 < 0){
		vga_put_cell(current_col-1, current_row-1, ' ', COLOR_WHITE);
		vga_set_cursor(current_col-1, current_row-1);
		return 1;
	}
	vga_put_cell(current_col-1, current_row, ' ', COLOR_WHITE);
	vga_set_cursor(current_col-1, current_row);
	return 0;
}

static struct display_driver vga_driver = {
	.cols = vga_cols,
	.rows = vga_rows,
	.put_cell = vga_put_cell,
	.set_cursor = vga_set_cursor,
	.backspace = vga_backspace,
	.clear = vga_clear,
	.scroll = vga_scroll
};

void init_vga_text_driver(void) {
	register_display_driver(&vga_driver);
}

