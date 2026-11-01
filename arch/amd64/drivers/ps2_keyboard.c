#include <pio.h>
#include <idt.h>
#include <pic.h>
#include <drivers/io/keyboard.h>
#include <ps2_keyboard.h>

extern void keyboard_handler_stub(void);

#define PS2_STATUS_PORT 0x64
#define PS2_DATA_PORT   0x60

void ps2_keyboard_init(void) {
	/* Register IRQ1 (Keyboard) at vector 0x21 */
	set_idt_gate(0x21, (uintptr_t)keyboard_handler_stub, 0, 0x8E);

	/* Unmask IRQ1 (Keyboard) on Master PIC */
	pic_unmask_irq(1);
}

void keyboard_handler(void) {
	unsigned char status = port_byte_in(PS2_STATUS_PORT);
	if (status & 0x01) {
		unsigned char scancode = port_byte_in(PS2_DATA_PORT);
		keyboard_on_scancode(scancode);
	}
}