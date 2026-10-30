#include <pio.h>
#include <drivers/io/keyboard.h>

#define PS2_STATUS_PORT 0x64
#define PS2_DATA_PORT   0x60

void ps2_keyboard_init(void) {
	// Enable Keyboard Port (Command 0xAE)
	while (port_byte_in(PS2_STATUS_PORT) & 0x02);
	port_byte_out(PS2_STATUS_PORT, 0xAE);

	// Read PS/2 Controller Command Byte (Command 0x20)
	while (port_byte_in(PS2_STATUS_PORT) & 0x02);
	port_byte_out(PS2_STATUS_PORT, 0x20);

	while (!(port_byte_in(PS2_STATUS_PORT) & 0x01));
	unsigned char command_byte = port_byte_in(PS2_DATA_PORT);

	// Enable IRQ1 (Bit 0 = 1) and Enable Keyboard Clock (Bit 4 = 0)
	command_byte |= (1 << 0);
	command_byte &= ~(1 << 4);

	// Write PS/2 Controller Command Byte (Command 0x60)
	while (port_byte_in(PS2_STATUS_PORT) & 0x02);
	port_byte_out(PS2_STATUS_PORT, 0x60);

	while (port_byte_in(PS2_STATUS_PORT) & 0x02);
	port_byte_out(PS2_DATA_PORT, command_byte);
}

void keyboard_handler(void) {
	unsigned char status = port_byte_in(PS2_STATUS_PORT);
	if (status & 0x01) {
		unsigned char scancode = port_byte_in(PS2_DATA_PORT);
		keyboard_on_scancode(scancode);
	}
}