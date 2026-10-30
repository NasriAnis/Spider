#include <int.h>

#define IDT_ENTRIES 256

struct InterruptDescriptorEntry {
    uint16_t offset_1;        // Offset bits 0..15
    uint16_t selector;        // Code segment selector in GDT (0x08)
    uint8_t  ist;             // Interrupt Stack Table index (0..7)
    uint8_t  type_attributes; // Gate type, DPL, Present flag
    uint16_t offset_2;        // Offset bits 16..31
    uint32_t offset_3;        // Offset bits 32..63
    uint32_t zero;            // Reserved, set to 0
} __attribute__((packed));

struct IDTR {
    uint16_t limit;
    uint64_t base;
} __attribute__((packed));

void set_idt_gate(int n, uintptr_t handler, uint8_t ist, uint8_t type_attr);
void init_idt(void);