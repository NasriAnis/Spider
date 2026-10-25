#include "./../../lib/int.h"
#include "pmm.h"

struct bitmap_loc get_bitmap_addr(uintptr_t addr);
uintptr_t get_physical_addr(uint32_t row, uint32_t col);
uint32_t get_pmm_bit(uint32_t row, uint32_t col);
void init_mm_tracking();
void init_bitmap(uintptr_t end_addr);
void fill_bitmap(uint64_t len, uintptr_t addr, uint8_t value);

struct bitmap_loc {
	uint32_t row;
	uint32_t col;
};

struct mm_available_regions *mm_available_regions = 0;
struct pmm_bitmap *pmm_bitmap;
extern uint32_t _kernel_start;
extern uint32_t _kernel_end;

uintptr_t pmm_alloc(uint32_t size) {
	uint32_t pages = (size + PAGE_SIZE - 1) / PAGE_SIZE;
	if (pages == 0) return 0;

	uint32_t total = pmm_bitmap->max_rows * BITS_PER_BITMAP_ROW;
	uint32_t run = 0, start = 0;

	for (uint32_t i = 0; i < total; i++) {
		uint32_t row = i / BITS_PER_BITMAP_ROW;
		uint32_t col = i % BITS_PER_BITMAP_ROW;

		if (get_pmm_bit(row, col) == 0) {
			if (run == 0) start = i;
			if (++run == pages) {
				uintptr_t addr = get_physical_addr(start / BITS_PER_BITMAP_ROW, start % BITS_PER_BITMAP_ROW);
				fill_bitmap((uint64_t)pages * PAGE_SIZE, addr, 1);
				return addr;
			}
		} else {
			run = 0;
		}
	}
	return 0;
}

void pmm_free(uintptr_t pointer, uint64_t size){
	fill_bitmap(size, pointer, 0);
}

void fill_bitmap(uint64_t len, uintptr_t addr, uint8_t value){
	for (uint64_t current = addr; current < addr + len; current += PAGE_SIZE) {
		struct bitmap_loc loc = get_bitmap_addr((uint32_t)current);

		if (value == 0) {
			pmm_bitmap->data[loc.row] &= ~(1u << loc.col);
		} else {
			pmm_bitmap->data[loc.row] |= (1u << loc.col);
		}
	}
}

void pmm_init_regions(){
	init_bitmap(mm_available_regions->highest_address);
	uint32_t entries = mm_available_regions->regions_number;
	for (int i = 0; i<entries; i++) {
		fill_bitmap(mm_available_regions->region[i].len, mm_available_regions->region[i].addr, 0);
	}
	pmm_bitmap->max_rows =
		get_bitmap_addr((uintptr_t)mm_available_regions->highest_address).row + 1;

	uintptr_t kstart = (uintptr_t)&_kernel_start;
	uintptr_t kend   = (uintptr_t)pmm_bitmap->data + pmm_bitmap->max_rows;
	fill_bitmap(kend - kstart, kstart, 1);

	fill_bitmap(PAGE_SIZE, 0, 1);
}

void available_mm_register(uintptr_t addr, uint64_t len){
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
void init_bitmap(uintptr_t end_addr){
	size_t tracking_size = sizeof(struct mm_available_regions) + 
		(mm_available_regions->regions_number * sizeof(struct region));
	pmm_bitmap = (struct pmm_bitmap *)((uint8_t *)mm_available_regions + tracking_size);
	pmm_bitmap->data = (uint8_t *)pmm_bitmap + sizeof(struct pmm_bitmap);
	for (int i = 0; i<=get_bitmap_addr((uint32_t)end_addr).row; i++){
		pmm_bitmap->data[i] = 255;
	}
}

struct bitmap_loc get_bitmap_addr(uintptr_t addr){
	uint32_t frame_index = addr / PAGE_SIZE;
	struct bitmap_loc bitmap_loc = {
		.row = frame_index / BITS_PER_BITMAP_ROW,
		.col = frame_index % BITS_PER_BITMAP_ROW,
	};
	return bitmap_loc;
}

uintptr_t get_physical_addr(uint32_t row, uint32_t col){
	return ((row * BITS_PER_BITMAP_ROW) + col ) * PAGE_SIZE;
}

uint32_t get_pmm_bit(uint32_t row, uint32_t col){
	return (pmm_bitmap->data[row] >> col) & 0b1;
}


void init_mm_tracking(){
	mm_available_regions = (struct mm_available_regions *)&_kernel_end;
	mm_available_regions->regions_number = 0;
	mm_available_regions->highest_address = 0;
}
