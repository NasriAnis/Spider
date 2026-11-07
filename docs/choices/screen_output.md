# Screen output

Depending on the registered driver at the screen output will be different and this gets chosen by the boot loader.

for now only the linear frame buffer and VGA (not really used) are supported.

## How everything is set up

**1. Getting the addresses and other information:**
First of all the boot loader fetches the address of the linear frame buffer

**2. Initializing the frame buffer driver:**
After getting the fb address, the boot loader calls `void init_fb_driver(uintptr_t addr, uint32_t width, uint32_t height, uint32_t pitch)` function with all the parameters fetched from the boot loader.

this function initialize the variables from the functions parameters, then build display structure defined at `display.h` :
```
struct display_driver {
	int  (*cols)(void);
	int  (*rows)(void);
	void (*put_cell)(int col, int row, char c, int color);
	void (*set_cursor)(int col, int row);
	int  (*backspace)(int current_row, int current_col);
	void (*clear)(void);
	void (*scroll)(void);
};
```

Once filled with all the specified parameters (functions), `register_display_driver(&fb_driver);` is called with the structure address that will get used for drawing to screen.

## Using the FB API's

All the API's are specified inside this `display_driver` structure, and are self explanatory.

Using this API's is through the structure access. Example :
```
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
```