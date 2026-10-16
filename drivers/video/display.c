#include "display.h"

struct display_driver *display = 0;

void register_display_driver(struct display_driver *driver) {
	display = driver;
}