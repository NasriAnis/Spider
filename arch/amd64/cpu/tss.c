#include <tss.h>
#include <gdt.h>

static uint8_t double_fault_stack[16384]; // 16KB dedicated stack for #DF
struct TSS tss;

void set_tss_stack(uint64_t rsp0) {
    tss.rsp0 = rsp0;
}

void load_tss(uint16_t selector) {
    __asm__ volatile ("ltr %0" :: "r" (selector));
}

void init_tss(void) {
    // Zero out TSS structure
    uint8_t *ptr = (uint8_t *)&tss;
    for (size_t i = 0; i < sizeof(struct TSS); i++) {
        ptr[i] = 0;
    }

    // Setup IST1 stack pointer for Double Fault (#DF) exceptions
    tss.ist1 = (uintptr_t)double_fault_stack + sizeof(double_fault_stack);
    tss.iopb_offset = sizeof(struct TSS);

    // Encode 16-byte TSS descriptor into GDT index 5 & 6
    set_gdt_tss(5, (uintptr_t)&tss, sizeof(struct TSS) - 1);

    // Load Task Register (TR) with selector 0x28
    load_tss(0x28);
}