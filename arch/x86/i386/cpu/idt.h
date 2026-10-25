#include <int.h>

struct InterruptDescriptorEntry {
	uint16_t offset_1;        // offset bits 0..15
	uint16_t selector;        // a code segment selector in GDT or LDT
	uint8_t  zero;            // unused, set to 0
	uint8_t  type_attributes; // gate type, dpl, and p fields
	uint16_t offset_2;        // offset bits 16..31
};

struct IDTR {
	uint16_t limit;
	uint32_t base;
} __attribute__((packed));

#define IDT_ENTRIES 0x22

void init_idt();
void load_idt();
void pic_setup_icw();
void pic_imr_setup(uint8_t mask);
void set_idt_gate(int n, unsigned long handler);