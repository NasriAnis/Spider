#include "./../../lib/int.h"
#include "pmm.h"

struct bitmap_loc get_bitmap_addr(uint32_t addr);
void init_mm_tracking();
void init_bitmap(uint64_t end_addr);

struct bitmap_loc {
	uint32_t row;
	uint32_t col;
};

struct mm_available_regions *mm_available_regions;
struct pmm_bitmap *pmm_bitmap;
extern uint32_t _kernel_end;

uintptr_t pmm_alloc(){
	return 0;
}

uintptr_t pmm_free(){
	return 0;
}

void fill_bitmap(uint64_t len, uint64_t addr, uint8_t value){
	for (uint64_t current = addr; current < addr + len; current += PAGE_SIZE) {
		struct bitmap_loc loc = get_bitmap_addr((uint32_t)current);

		if (value == 0) {
			pmm_bitmap->data[loc.row] &= ~(1 << loc.col);
		} else {
			pmm_bitmap->data[loc.row] |= (1 << loc.col);
		}
	}
}

void pmm_init_regions(){
	init_bitmap(mm_available_regions->highest_address);
	uint32_t entries = mm_available_regions->regions_number;
	for (int i = 0; i<entries; i++) {
		fill_bitmap(mm_available_regions->region[i].len,
			  mm_available_regions->region[i].addr, 0);
	}
}

void available_mm_register(uint64_t addr, uint64_t len){
	if (!mm_available_regions){
		init_mm_tracking();
	}

	uint32_t idx = mm_available_regions->regions_number;

	mm_available_regions->region[idx].addr = addr;
	mm_available_regions->region[idx].len = len;

	mm_available_regions->regions_number++;

	if (mm_available_regions->highest_address < (addr + len)){
		mm_available_regions->highest_address = addr + len;
	}
}

// NOTE: unaligned address to PAGE_SIZE doesnt get used there
void init_bitmap(uint64_t end_addr){
	size_t tracking_size = sizeof(struct mm_available_regions) + 
		(mm_available_regions->regions_number * sizeof(struct region));
	pmm_bitmap = (struct pmm_bitmap *)((uint8_t *)mm_available_regions + tracking_size);
	pmm_bitmap->data = (uint8_t *)pmm_bitmap + sizeof(struct pmm_bitmap);
	for (int i = 0; i<=get_bitmap_addr((uint32_t)end_addr).row; i++){
		pmm_bitmap->data[i] = 255;
	}
}

struct bitmap_loc get_bitmap_addr(uint32_t addr){
	uint32_t frame_index = addr / PAGE_SIZE;
	struct bitmap_loc bitmap_loc = {
		.row = frame_index / BITS_PER_BITMAP_ROW,
		.col = frame_index % BITS_PER_BITMAP_ROW,
	};
	return bitmap_loc;
}

void init_mm_tracking(){
    mm_available_regions = (struct mm_available_regions *)&_kernel_end;
    mm_available_regions->regions_number = 0;
}
