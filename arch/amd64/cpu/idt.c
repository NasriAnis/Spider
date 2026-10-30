#include <idt.h>
#include <pic.h>

extern void default_isr_stub(void);
extern void keyboard_handler_stub(void);

struct InterruptDescriptorEntry IDT[IDT_ENTRIES];
struct IDTR idtr;

void set_idt_gate(int n, uintptr_t handler, uint8_t ist, uint8_t type_attr) {
    IDT[n].offset_1 = handler & 0xFFFF;
    IDT[n].selector = 0x08; // 64-bit Kernel Code Segment
    IDT[n].ist = ist & 0x07;
    IDT[n].type_attributes = type_attr;
    IDT[n].offset_2 = (handler >> 16) & 0xFFFF;
    IDT[n].offset_3 = (handler >> 32) & 0xFFFFFFFF;
    IDT[n].zero = 0;
}

void load_idt(void) {
    __asm__ volatile ("lidt %0" :: "m" (idtr));
}

void init_idt(void) {
    /* Fill all 256 gates with default exception handler */
    for (int i = 0; i < IDT_ENTRIES; i++) {
        set_idt_gate(i, (uintptr_t)default_isr_stub, 0, 0x8E);
    }

    /* Remap PIC using port_byte_out */
    pic_setup_icw();

    /* Register IRQ1 (Keyboard) at vector 0x21 */
    set_idt_gate(0x21, (uintptr_t)keyboard_handler_stub, 0, 0x8E);

    /* Mask all IRQs except IRQ1 (Keyboard) on Master PIC */
    pic_set_masks(0xFD, 0xFF); // 0xFD = 1111 1101 (Bit 1 unmasked)

    /* Load IDTR */
    idtr.limit = (sizeof(struct InterruptDescriptorEntry) * IDT_ENTRIES) - 1;
    idtr.base  = (uintptr_t)&IDT;

    load_idt();
}