#include <paging.h>
#include <int.h>
#include <mem.h>
#include <drivers/mem/pmm.h>
#include <kernel/kernel.h>

extern char __limine_start[], __limine_end[];
extern char __text_start[],   __text_end[];
extern char __rodata_start[], __rodata_end[];
extern char __data_start[],   __data_end[];   // includes .bss

static uint64_t *pml4_virt_addr;
static uint64_t pml4_phys_addr;

/*
 * Allocate a 4096 space, zero it out and return its physical address.
 * registers a lot when the pmm alocator returns null for out of memory.
 */
static uint64_t alloc_table(void) {
	uint64_t phys = (uint64_t)pmm_alloc(4096);
	if (!phys) {
		register_log(FAILS, ARCH, "paging", "out of memory for page tables");
		for (;;) asm volatile("hlt");
	}
	memset(phys_to_virt(phys), 0, 4096);
	return phys;
}

/*
 * Allocate a table if doesnt exist or has no present bit and ret it phys addr,
 * or just directly return the table address if it already exists
 */
static uint64_t *next_table(uint64_t *table, size_t idx) {
    if (!(table[idx] & PTE_P))
        table[idx] = alloc_table() | PTE_P | PTE_RW;
    return (uint64_t *)phys_to_virt(table[idx] & PTE_ADDR);
}

/*
 * Map a virtual address page to a physical address
 */
static void map_page(uint64_t virt, uint64_t phys, uint64_t flags) {
    uint64_t *pdpt = next_table(pml4_virt_addr, (virt >> 39) & 0x1FF);
    uint64_t *pd   = next_table(pdpt,           (virt >> 30) & 0x1FF);
    uint64_t *pt   = next_table(pd,             (virt >> 21) & 0x1FF);
    pt[(virt >> 12) & 0x1FF] = (phys & PTE_ADDR) | flags | PTE_P;
}

/*
 * Mapp the virtual address to physical address for a certin lenght
 * by page offset 4069
*/
void map_range(uint64_t virt, uint64_t phys, uint64_t len, uint64_t flags) {
    for (uint64_t off = 0; off < len; off += 4096)
        map_page(virt + off, phys + off, flags);
}

static inline void load_cr3(void* cr3_value){
	asm volatile("mov %0, %%cr3" :: "r"((uint64_t)cr3_value) : "memory");
}

static void construct_pml4(void) {
	pml4_phys_addr = alloc_table();
	pml4_virt_addr = phys_to_virt(pml4_phys_addr);

	// upper half: each entry points to its own empty PDPT (kernel only, no US)
	for (int i = 256; i < 512; i++)
		pml4_virt_addr[i] = alloc_table() | PML4_P | PML4_RW;
}

/*
* setup kernel page tables
*/
void setup_paging(void) {
	construct_pml4();

	// 1. HHDM over the memory map
	for (size_t i = 0; i < memmap->regions_number; i++) {
		if (memmap->region[i].type == MEMMAP_BAD_MEMORY) continue;
		// base and end are physical addresses
		uint64_t base = memmap->region[i].base & ~0xFFFull;
		uint64_t end  = (memmap->region[i].base + memmap->region[i].length + 0xFFF) & ~0xFFFull;
		map_range((uint64_t)phys_to_virt(base), base, end - base, PTE_RW | PTE_NX);
	}

	// 2. Kernel image, per section
	// We use KPHYS because we are using the linker script fetched memory addresses
	// so to get the real physiscal address we should add to it the difference of
	// the kernel virtual and phys base
	map_range((uint64_t)__limine_start, KPHYS(__limine_start),
		__limine_end - __limine_start,     PTE_RW | PTE_NX);   // requests (RW)
	map_range((uint64_t)__text_start,   KPHYS(__text_start),
		__text_end - __text_start,         0);                 // R+X
	map_range((uint64_t)__rodata_start, KPHYS(__rodata_start),
		__rodata_end - __rodata_start,     PTE_NX);            // R
	map_range((uint64_t)__data_start,   KPHYS(__data_start),
		__data_end - __data_start,         PTE_RW | PTE_NX);   // RW

	// 3. Switch
	load_cr3((void *)pml4_phys_addr);
}
