#include "console.h"
#include "../drivers/video/display.h"

static int cur_col = 0;
static int cur_row = 0;

static void put_char(char c, int color) {
	if (c == '\n') {
		cur_col = 0;
		cur_row++;
	} else {
		display_put_cell(cur_col, cur_row, c, color);
		cur_col++;
		if (cur_col >= display_cols()) { cur_col = 0; cur_row++; }
	}

	if (cur_row >= display_rows()) {
		display_scroll();
		cur_row = display_rows() - 1;
	}
	display_set_cursor(cur_col, cur_row);
}

static void put_string(char *s, int color) {
	for (int i = 0; s[i]; i++) put_char(s[i], color);
}

void backspace(){
	if (display_backspace(cur_row, cur_col)){
		cur_col -= 1;
		cur_row -= 1;
	}
	else {
		cur_col -= 1;
	}
}

void print(char *m)     { put_string(m, COLOR_WHITE); }
void eprint(char *m)    { put_string(m, COLOR_RED); }
void printnl(char *m)   { print(m);  put_char('\n', COLOR_WHITE); }
void eprintnl(char *m)  { eprint(m); put_char('\n', COLOR_RED); }

void clear_screen(void) {
	display_clear();
	cur_col = 0;
	cur_row = 0;
}