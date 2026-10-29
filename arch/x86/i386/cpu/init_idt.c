#include <idt.h>
#include <int.h>
#include <pio.h>
#include <drivers/io/keyboard.h>
#include <isr.h>

extern void keyboard_handler_stub(void);

void init_idt(){
	pic_setup_icw();
	pic_imr_setup(0xFF);

	cpu_interrupts_setup();
	set_idt_gate(0x21, (unsigned long)keyboard_handler_stub); // for keyboard

	idtr.limit = (sizeof(struct InterruptDescriptorEntry) * IDT_ENTRIES) - 1;
	idtr.base  = (uint32_t)&IDT;

	load_idt();

	pic_imr_setup(0xFD); // for keyboard
}