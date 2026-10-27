#include <io.h>
#include <drivers/video/display.h>

static int cur_col = 0;
static int cur_row = 0;

static void putcc(char c, int color) {
	if (c == '\n') {
		cur_col = 0;
		cur_row++;
	} else {
		display->put_cell(cur_col, cur_row, c, color);
		cur_col++;
		if (cur_col >= display->cols()) { cur_col = 0; cur_row++; }
	}

	if (cur_row >= display->rows()) {
		display->scroll();
		cur_row = display->rows() - 1;
	}
	display->set_cursor(cur_col, cur_row);
}

static void putsc(char *s, int color) {
	for (int i = 0; s[i]; i++) putcc(s[i], color);
}

void backspace(){
	if (display->backspace(cur_row, cur_col)){
		cur_col -= 1;
		cur_row -= 1;
	}
	else {
		cur_col -= 1;
	}
}

void puts(char *m)     { putsc(m, COLOR_WHITE); }
void eputs(char *m)    { putsc(m, COLOR_RED); }
void putsnl(char *m)   { puts(m);  putcc('\n', COLOR_WHITE); }
void eputsnl(char *m)  { eputs(m); putcc('\n', COLOR_RED); }

void cls(void) {
	display->clear();
	cur_col = 0;
	cur_row = 0;
}
