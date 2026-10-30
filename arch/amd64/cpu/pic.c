#include <pic.h>
#include <pio.h>

#define PIC1_COMMAND 0x20
#define PIC1_DATA    0x21
#define PIC2_COMMAND 0xA0
#define PIC2_DATA    0xA1

void pic_setup_icw(void) {
	/* ICW1: Start initialization sequence */
	port_byte_out(PIC1_COMMAND, 0x11);
	io_wait();
	port_byte_out(PIC2_COMMAND, 0x11);
	io_wait();

	/* ICW2: Remap Master PIC to 0x20 (IRQs 0-7), Slave PIC to 0x28 (IRQs 8-15) */
	port_byte_out(PIC1_DATA, 0x20);
	io_wait();
	port_byte_out(PIC2_DATA, 0x28);
	io_wait();

	/* ICW3: Setup cascading (IRQ2 connects to Slave PIC) */
	port_byte_out(PIC1_DATA, 0x04);
	io_wait();
	port_byte_out(PIC2_DATA, 0x02);
	io_wait();

	/* ICW4: 8086 / 8088 mode */
	port_byte_out(PIC1_DATA, 0x01);
	io_wait();
	port_byte_out(PIC2_DATA, 0x01);
	io_wait();
}

void pic_set_masks(uint8_t master_mask, uint8_t slave_mask) {
	port_byte_out(PIC1_DATA, master_mask);
	io_wait();
	port_byte_out(PIC2_DATA, slave_mask);
	io_wait();
}

void pic_send_eoi(uint8_t irq) {
	if (irq >= 8) {
		port_byte_out(PIC2_COMMAND, 0x20);
	}
	port_byte_out(PIC1_COMMAND, 0x20);
}