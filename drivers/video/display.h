struct display_driver {
	int  (*cols)(void);
	int  (*rows)(void);
	void (*put_cell)(int col, int row, char c, int color);
	void (*set_cursor)(int col, int row);
	int  (*backspace)(int current_row, int current_col);
	void (*clear)(void);
	void (*scroll)(void);
};

extern struct display_driver *display;

void register_display_driver(struct display_driver *driver);

enum { COLOR_WHITE, COLOR_RED };