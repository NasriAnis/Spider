#include "./../../lib/int.h"

struct pmm_bitmap {
	uint8_t *data;
	uint32_t max_rows;
};

struct region {
	uintptr_t addr;
	uint64_t len;
};

struct mm_available_regions {
	uint32_t regions_number;
	uint64_t highest_address;
	struct region region[];
};

#define BITS_PER_BITMAP_ROW 8
#define PAGE_SIZE 4096

// Not woking yet
uintptr_t pmm_free();
uintptr_t pmm_alloc();

void available_mm_register(uintptr_t addr, uint64_t len);
void pmm_init_regions();
