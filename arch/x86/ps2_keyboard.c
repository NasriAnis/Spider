#include "ports.h"
#include "../../drivers/io/keyboard.h"

#define PS2_STATUS_PORT 0x64
#define PS2_DATA_PORT   0x60

void keyboard_handler(void) {
	unsigned char status = port_byte_in(PS2_STATUS_PORT);
	if (status & 0x01) {
		unsigned char scancode = port_byte_in(PS2_DATA_PORT);
		keyboard_on_scancode(scancode);
	}
}