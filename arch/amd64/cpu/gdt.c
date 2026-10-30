#include <gdt.h>

uint64_t GDT[GDT_ENTRIES];
struct GDTR gdtr;

void set_gdt_tss(int index, uintptr_t tss_addr, uint32_t tss_size) {
    struct SystemDescriptor64 *desc = (struct SystemDescriptor64 *)&GDT[index];
    desc->limit_low = tss_size & 0xFFFF;
    desc->base_low = tss_addr & 0xFFFF;
    desc->base_mid = (tss_addr >> 16) & 0xFF;
    desc->access = 0x89; // Present, Ring 0, 64-bit TSS (Available)
    desc->flags_limit = (tss_size >> 16) & 0x0F;
    desc->base_high_byte = (tss_addr >> 24) & 0xFF;
    desc->base_high_dword = (tss_addr >> 32) & 0xFFFFFFFF;
    desc->reserved = 0;
}

void load_gdt(void) {
    __asm__ volatile (
        "lgdt %0\n\t"
        "mov $0x10, %%ax\n\t"
        "mov %%ax, %%ds\n\t"
        "mov %%ax, %%es\n\t"
        "mov %%ax, %%fs\n\t"
        "mov %%ax, %%gs\n\t"
        "mov %%ax, %%ss\n\t"
        "pushq $0x08\n\t"
        "leaq 1f(%%rip), %%rax\n\t"
        "pushq %%rax\n\t"
        "lretq\n\t"
        "1:\n\t"
        :
        : "m" (gdtr)
        : "rax", "memory"
    );
}

void init_gdt(void) {
    // Populate GDT entries
    GDT[0] = 0x0000000000000000; // Null Descriptor
    GDT[1] = 0x00AF9A0000000000; // Kernel Code 64-bit (Selector 0x08)
    GDT[2] = 0x00CF920000000000; // Kernel Data 64-bit (Selector 0x10)
    GDT[3] = 0x00CFF20000000000; // User Data 64-bit   (Selector 0x18 | 3 = 0x1B)
    GDT[4] = 0x00AFFA0000000000; // User Code 64-bit   (Selector 0x20 | 3 = 0x23)

    // Setup GDTR register pointer
    gdtr.limit = (sizeof(uint64_t) * GDT_ENTRIES) - 1;
    gdtr.base  = (uintptr_t)&GDT;

    // Load GDT into CPU
    load_gdt();
}