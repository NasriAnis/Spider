#include <int.h>
#include "pmm.h"
#include <arch.h>

// Declare kernel symbols as byte arrays to preserve full 64-bit pointer addresses
extern uint8_t _kernel_start[];
extern uint8_t _kernel_end[];

struct bitmap_loc {
    uint64_t row;
    uint64_t col;
};

struct mm_available_regions *mm_available_regions = 0;
struct pmm_bitmap *pmm_bitmap = 0;

// Address conversion helpers
static inline void *phys_to_virt(uintptr_t phys) {
    return (void *)(phys + kernel_state.hhdm_offset);
}

static inline uintptr_t virt_to_phys(void *virt) {
    return (uintptr_t)virt - kernel_state.hhdm_offset;
}

struct bitmap_loc get_bitmap_addr(uintptr_t phys_addr) {
    uint64_t frame_index = phys_addr / PAGE_SIZE;
    struct bitmap_loc loc = {
        .row = frame_index / BITS_PER_BITMAP_ROW,
        .col = frame_index % BITS_PER_BITMAP_ROW,
    };
    return loc;
}

uintptr_t get_physical_addr(uint64_t row, uint64_t col) {
    return ((row * BITS_PER_BITMAP_ROW) + col) * PAGE_SIZE;
}

uint32_t get_pmm_bit(uint64_t row, uint64_t col) {
    return (pmm_bitmap->data[row] >> col) & 0b1;
}

void fill_bitmap(uint64_t len, uintptr_t phys_addr, uint8_t value) {
    for (uint64_t current = phys_addr; current < phys_addr + len; current += PAGE_SIZE) {
        struct bitmap_loc loc = get_bitmap_addr(current);

        if (value == 0) {
            pmm_bitmap->data[loc.row] &= ~(1u << loc.col);
        } else {
            pmm_bitmap->data[loc.row] |= (1u << loc.col);
        }
    }
}

void init_mm_tracking(void) {
    // Store region tracking structure in kernel BSS space
    mm_available_regions = (struct mm_available_regions *)_kernel_end;
    mm_available_regions->regions_number = 0;
    mm_available_regions->highest_address = 0;
}

void available_mm_register(uintptr_t addr, uint64_t len) {
    if (!mm_available_regions) {
        init_mm_tracking();
    }

    uint32_t idx = mm_available_regions->regions_number;

    mm_available_regions->region[idx].addr = addr;
    mm_available_regions->region[idx].len = len;
    mm_available_regions->regions_number++;

    if (mm_available_regions->highest_address < (addr + len)) {
        mm_available_regions->highest_address = addr + len;
    }
}

void init_bitmap(uintptr_t end_addr) {
    uint64_t total_frames = (end_addr + PAGE_SIZE - 1) / PAGE_SIZE;
    uint64_t bitmap_bytes = (total_frames + 7) / 8;
    uint64_t required_bytes = sizeof(struct pmm_bitmap) + bitmap_bytes;

    uintptr_t bitmap_phys_addr = 0;

    // Find a usable physical RAM block large enough for the bitmap
    for (uint32_t i = 0; i < mm_available_regions->regions_number; i++) {
        if (mm_available_regions->region[i].len >= required_bytes) {
            bitmap_phys_addr = mm_available_regions->region[i].addr;
            break;
        }
    }

    if (bitmap_phys_addr == 0) {
        return; // Failed to locate physical memory for bitmap
    }

    // Map physical RAM address to kernel virtual space via HHDM
    pmm_bitmap = (struct pmm_bitmap *)phys_to_virt(bitmap_phys_addr);
    pmm_bitmap->data = (uint8_t *)pmm_bitmap + sizeof(struct pmm_bitmap);
    pmm_bitmap->max_rows = (bitmap_bytes + (BITS_PER_BITMAP_ROW / 8) - 1) / (BITS_PER_BITMAP_ROW / 8);

    // Initialize all frames as allocated (1 = used)
    for (uint64_t i = 0; i < pmm_bitmap->max_rows; i++) {
        pmm_bitmap->data[i] = 0xFF;
    }
}

void pmm_init_regions(void) {
    init_bitmap(mm_available_regions->highest_address);

    // 1. Mark reported usable RAM regions as free (0)
    uint32_t entries = mm_available_regions->regions_number;
    for (uint32_t i = 0; i < entries; i++) {
        fill_bitmap(mm_available_regions->region[i].len, mm_available_regions->region[i].addr, 0);
    }

    // 2. Protect page 0 (NULL pointer trap)
    fill_bitmap(PAGE_SIZE, 0, 1);

    // 3. Protect kernel physical memory range
    // Adjust kernel base physical offset according to your linker base (e.g. 0x100000 or Limine address response)
    uintptr_t kstart_phys = 0x100000;
    uintptr_t kend_phys = kstart_phys + ((uintptr_t)_kernel_end - (uintptr_t)_kernel_start);
    fill_bitmap(kend_phys - kstart_phys, kstart_phys, 1);

    // 4. Protect bitmap's own physical memory region
    uintptr_t bitmap_phys = virt_to_phys(pmm_bitmap);
    uint64_t bitmap_size = sizeof(struct pmm_bitmap) + (pmm_bitmap->max_rows * (BITS_PER_BITMAP_ROW / 8));
    fill_bitmap(bitmap_size, bitmap_phys, 1);
}

uintptr_t pmm_alloc(uint32_t size) {
    uint32_t pages = (size + PAGE_SIZE - 1) / PAGE_SIZE;
    if (pages == 0) return 0;

    uint64_t total = pmm_bitmap->max_rows * BITS_PER_BITMAP_ROW;
    uint32_t run = 0, start = 0;

    for (uint64_t i = 0; i < total; i++) {
        uint64_t row = i / BITS_PER_BITMAP_ROW;
        uint64_t col = i % BITS_PER_BITMAP_ROW;

        if (get_pmm_bit(row, col) == 0) {
            if (run == 0) start = i;
            if (++run == pages) {
                uintptr_t phys_addr = get_physical_addr(start / BITS_PER_BITMAP_ROW, start % BITS_PER_BITMAP_ROW);
                fill_bitmap((uint64_t)pages * PAGE_SIZE, phys_addr, 1);
                return phys_addr;
            }
        } else {
            run = 0;
        }
    }
    return 0; // Out of memory
}

void pmm_free(uintptr_t pointer, uint64_t size) {
    fill_bitmap(size, pointer, 0);
}