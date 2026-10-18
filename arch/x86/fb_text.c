#include "../../drivers/video/display.h"
#include "../../lib/int.h"
#include "../../font/font.h"

static uintptr_t fb_addr = 0;
static uint32_t fb_width = 0;
static uint32_t fb_height = 0;
static uint32_t fb_pitch = 0;

#define FONT_WIDTH  8
#define FONT_HEIGHT 16
#define BG_COLOR 0x00000000

int fb_cols(void) { 
	return fb_width / FONT_WIDTH;
}

int fb_rows(void) { 
	return fb_height / FONT_HEIGHT;
}

static void fb_put_pixel(int x, int y, uint32_t color) {
	if (x < 0 || x >= (int)fb_width || y < 0 || y >= (int)fb_height) return;

	uint32_t *pixel_addr = (uint32_t *)(fb_addr + (y * fb_pitch) + (x * sizeof(uint32_t)));
	*pixel_addr = color;
}

void fb_put_cell(int col, int row, char c, int color) {
	int start_x = col * FONT_WIDTH;
	int start_y = row * FONT_HEIGHT;

	uint32_t fg_color = (color == COLOR_RED) ? 0x00FF0000 : 0x00FFFFFF;
	uint32_t bg_color = 0x00000000;

	const uint8_t *glyph = psf_get_glyph((unsigned char)c);

	for (int y = 0; y < FONT_HEIGHT; y++) {
		uint8_t bits = glyph[y];
		for (int x = 0; x < FONT_WIDTH; x++) {
			uint32_t px = (bits & (0x80 >> x)) ? fg_color : bg_color;
			fb_put_pixel(start_x + x, start_y + y, px);
		}
	}
}

void fb_set_cursor(int col, int row) {
	(void)col; (void)row;
}

void fb_clear(void) {
	for (uint32_t y = 0; y < fb_height; y++) {
		for (uint32_t x = 0; x < fb_width; x++) {
			fb_put_pixel(x, y, BG_COLOR);
		}
	}
}

void fb_scroll(void) {
	// Implement scrolling by copying row memory upwards
}

int fb_backspace(int current_row, int current_col) {
	return current_col;
}

static struct display_driver fb_driver = {
	.cols = fb_cols,
	.rows = fb_rows,
	.put_cell = fb_put_cell,
	.set_cursor = fb_set_cursor,
	.backspace = fb_backspace,
	.clear = fb_clear,
	.scroll = fb_scroll
};

void init_fb_driver(uintptr_t addr, uint32_t width, uint32_t height, uint32_t pitch) {
	fb_addr = addr;
	fb_width = width;
	fb_height = height;
	fb_pitch = pitch;
	psf_init(); // initialize the fonts
	register_display_driver(&fb_driver);
}