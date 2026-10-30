#include <int.h>

#define GDT_ENTRIES 7

struct GDTR {
    uint16_t limit;
    uint64_t base;
} __attribute__((packed));

struct SystemDescriptor64 {
    uint16_t limit_low;
    uint16_t base_low;
    uint8_t  base_mid;
    uint8_t  access;
    uint8_t  flags_limit;
    uint8_t  base_high_byte;
    uint32_t base_high_dword;
    uint32_t reserved;
} __attribute__((packed));

void set_gdt_tss(int index, uintptr_t tss_addr, uint32_t tss_size);
void init_gdt(void);