#include <int.h>
#include "pmm.h"
#include <kernel/kernel.h>

// helper struct for physical address
// localisation in the bitmap
struct bitmap_loc {
	uint64_t row;
	uint64_t col;
};

// bitmap will be mapped later at init_bitmap()
struct pmm_bitmap *pmm_bitmap = 0;
struct memmap *memmap = 0;

static struct bitmap_loc get_bitmap_addr(uintptr_t phys_addr) {
	uint64_t frame_index = phys_addr / PAGE_SIZE;
	struct bitmap_loc loc = {
		.row = frame_index / BITS_PER_BITMAP_ROW,
		.col = frame_index % BITS_PER_BITMAP_ROW,
	};
	return loc;
}

static uintptr_t get_physical_addr(uint64_t row, uint64_t col) {
	return ((row * BITS_PER_BITMAP_ROW) + col) * PAGE_SIZE;
}

static uint32_t get_pmm_bit(uint64_t row, uint64_t col) {
	return (pmm_bitmap->data[row] >> col) & 0b1;
}

static void fill_bitmap(uint64_t len, uintptr_t phys_addr, uint8_t value) {
	for (uint64_t current = phys_addr; current < phys_addr + len; current += PAGE_SIZE) {
		struct bitmap_loc loc = get_bitmap_addr(current);

		if (value == 0) {
			pmm_bitmap->data[loc.row] &= ~(1u << loc.col);
		} else {
			pmm_bitmap->data[loc.row] |= (1u << loc.col);
		}
	}
}

static void init_bitmap(uintptr_t end_addr) {
	uint64_t total_frames = (end_addr + PAGE_SIZE - 1) / PAGE_SIZE;
	uint64_t bitmap_bytes = (total_frames + 7) / 8;
	uint64_t required_bytes = sizeof(struct pmm_bitmap) + bitmap_bytes;

	uintptr_t bitmap_phys_addr = 0;

	// Find a usable physical RAM block large enough for the bitmap
	for (uint32_t i = 0; i < memmap->regions_number; i++) {
		if (memmap->region[i].length >= required_bytes) {
			bitmap_phys_addr = memmap->region[i].base;
			break;
		}
	}

	// this is a fatal error if we cannot allocate space for the bitmap
	if (bitmap_phys_addr == 0) {
		register_log(FAILS, KERNEL, "PMM", "Failed to locate physical memory for bitmap");
		PANIC("FAILED TO LOCATE PHYSICAL MEMORY FOR THE BITMAP");
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

/*
 * Public Physical memory managment fonctions
 */
void pmm_init_regions(void) {
	init_bitmap(memmap->highest_address);

	// 1. Mark reported usable RAM regions as free (0)
	uint32_t entries = memmap->regions_number;
	for (uint32_t i = 0; i < entries; i++) {
		if (memmap->region[i].type == MEMMAP_USABLE){
			fill_bitmap(memmap->region[i].length, memmap->region[i].base, 0);
		} else {
			fill_bitmap(memmap->region[i].length, memmap->region[i].base, 1);
		}
	}

	// 2. Protect page 0 (NULL pointer trap)
	fill_bitmap(PAGE_SIZE, 0, 1);

	// 3. Protect kernel physical memory range
	uintptr_t kstart_phys = kstate.kernel_physical_base;
	uintptr_t kend_phys = kstate.kernel_physical_end;
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
	register_log(FAILS, KERNEL, "PMM", "Cannot allocate out of memory");
	return 0; // Out of memory
}

void pmm_free(uintptr_t pointer, uint64_t size) {
	fill_bitmap(size, pointer, 0);
}

// Initialize the memory mapped structure to save the mmap of the device
void fill_memmap_struct(uintptr_t addr, uint64_t len, uint64_t type){
	// if the structure hasnt been initialized into memory
	if (!memmap) {
		memmap = (struct memmap *)kstate.kernel_virtual_end;
		memmap->regions_number = 0;
		memmap->highest_address = 0;
	}
		uint32_t idx = memmap->regions_number;

		memmap->region[idx].base = addr;
		memmap->region[idx].length = len;
		memmap->region[idx].type = type;
		memmap->regions_number++;

		if (memmap->highest_address < (addr + len)) {
			memmap->highest_address = addr + len;
		}
}
