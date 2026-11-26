#include <int.h>
#include <kernel/logs.h>

struct pmm_bitmap {
	uint8_t *data;
	uint32_t max_rows;
};

/*
 * Memory map regions strcutre gathered from LMINE
 */

#define MEMMAP_USABLE                 0
#define MEMMAP_RESERVED               1
#define MEMMAP_ACPI_RECLAIMABLE       2
#define MEMMAP_ACPI_NVS               3
#define MEMMAP_BAD_MEMORY             4
#define MEMMAP_BOOTLOADER_RECLAIMABLE 5
#define MEMMAP_EXECUTABLE_AND_MODULES 6
#define MEMMAP_FRAMEBUFFER            7
#define MEMMAP_RESERVED_MAPPED        8

struct entries {
	uintptr_t base;
	uint64_t length;
	uint64_t type;
};

struct memmap {
	uint32_t regions_number;
	uint64_t highest_address;
	struct entries region[];
};
extern struct memmap *memmap;

void fill_memmap_struct(uintptr_t addr, uint64_t len, uint64_t type);

#define BITS_PER_BITMAP_ROW 8
#define PAGE_SIZE 4096

/*
 * Specify the size in bytes
 */
uintptr_t pmm_alloc(uint32_t size);
void pmm_free(uintptr_t pointer, uint64_t size);

void available_mm_register(uintptr_t addr, uint64_t len);
void pmm_init_regions();

static inline void *phys_to_virt(uintptr_t phys) {
	return (void *)(phys + kernel_state.hhdm_offset);
}

static inline uintptr_t virt_to_phys(void *virt) {
	return (uintptr_t)virt - kernel_state.hhdm_offset;
}