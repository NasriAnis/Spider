enum { COLOR_WHITE, COLOR_RED };

int  display_cols(void);
int  display_rows(void);
void display_put_cell(int col, int row, char c, int color);
void display_set_cursor(int col, int row);
void display_clear(void);
void display_scroll(void);