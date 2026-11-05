# PMM

The physical memory manager in the Spider Operating system uses a bitmap
to tell which memory location is free or in use.

## How everything is set up

**1. Getting the available memory regions:**
Depending the arch specific boot loader gets available memory regions.

**2. Registering the available regions:**
Upon getting this region it calls the `available_mm_register(uintptr_t addr, uint64_t len)` with the available to use regions address and length.

These regions are then saved into a `mm_available_regions` structure in the `pmm.h` file as that get loaded by :
```
struct region {
	uintptr_t addr;  // region address
	uint64_t len;    // regions length
};

struct mm_available_regions {
	uint32_t regions_number;   // hold how much individual regions registred
	uint64_t highest_address;  // hold highest avilable address in all regions
	struct region region[];    // thiis holds the regions info
};
```

This will be helpful to later computer the total available memory and build our bitmap.

**3. Initializing the bitmap with the fetched regions information:**
After all regions has been saved to the previously cited structure, the kernel calls `pmm_init_regions()` function.

This function first init the bitmap by computing how much bits are needed to hold the whole available memory (by using the information gathered by the previous structure like `mm_available_regions->highest_address`, `PAGE_SIZE` and `BITS_PER_BITMAP_ROW`), then finding a large enough region to hold the bitmap structure.

This structure is then zero out completely then the kernel and bitmap own address bits are switched to 1 (marked as busy).

## Using the PMM API's

After the bitmap is set up, we can use the PMM API's :
- `uintptr_t pmm_alloc(uint32_t size);`
- `void pmm_free(uintptr_t pointer, uint64_t size);`