#include "idt.h"
#include "../../../../lib/int.h"
#include "../../pio.h"
#include "../../../../drivers/io/keyboard.h"
#include "isr.h"

static struct InterruptDescriptorEntry IDT[IDT_ENTRIES];
static struct IDTR idtr;

void load_idt();
void pic_setup_icw();
void pic_imr_setup(uint8_t mask);
void set_idt_gate(int n, unsigned long handler);

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

void load_idt(){
	__asm__ volatile ("lidt %0" :: "m" (idtr));
	__asm__ volatile ("sti");
}

void set_idt_gate(int n, unsigned long handler){
	unsigned long handler_address = (unsigned long)handler;
	IDT[n].offset_1 = handler_address & 0xffff;
	IDT[n].selector = 0x08;
	IDT[n].zero = 0;
	IDT[n].type_attributes = 0x8e;
	IDT[n].offset_2 = (handler_address & 0xffff0000) >> 16;
}

void pic_setup_icw(){
	/*
	*	      | PIC1	PIC2
	* -----------------------
	* Command | 0x20	0xA0
	* Data	  | 0x21	0xA1
	*/

	/* ICW1 - begin initialization */
	port_byte_out(0x20 , 0x11);
	port_byte_out(0xA0 , 0x11);

	/* ICW2 - remap offset address of IDT */
	/*
	* In x86 protected mode, we have to remap the PICs
	* beyond 0x20 because Intel have designated the
	* first 32 interrupts as "reserved" for cpu
	* exceptions
	*/
	port_byte_out(0x21 , 0x20);
	port_byte_out(0xA1 , 0x28);

	/* ICW3 - setup cascading */
	port_byte_out(0x21 , 0x00);
	port_byte_out(0xA1 , 0x00);

	/* ICW4 - environment info */
	port_byte_out(0x21 , 0x01);
	port_byte_out(0xA1 , 0x01);
}

void pic_imr_setup(uint8_t mask){
	port_byte_out(0x21 , mask);
	port_byte_out(0xA1 , mask);
}