#include <limine-protocol/include/limine.h>
#include <int.h>
#include <arch.h>
#include <drivers/mem/pmm.h>
#include "limine.h"

extern void init_fb_driver(uintptr_t addr, uint32_t width, uint32_t height, uint32_t pitch);

#define LIMINE_REVISION 6

__attribute__((used, section(".limine_requests")))
static volatile uint64_t limine_base_revision[] = LIMINE_BASE_REVISION(LIMINE_REVISION);

__attribute__((used, section(".limine_requests")))
static volatile struct limine_memmap_request memmap_request = {
    .id = LIMINE_MEMMAP_REQUEST_ID,
    .revision = 0
};

__attribute__((used, section(".limine_requests")))
static volatile struct limine_hhdm_request hhdm_request = {
    .id = LIMINE_HHDM_REQUEST_ID,
    .revision = 0
};

__attribute__((used, section(".limine_requests")))
static volatile struct limine_framebuffer_request framebuffer_request = {
    .id = LIMINE_FRAMEBUFFER_REQUEST_ID,
    .revision = 0
};

__attribute__((used, section(".limine_requests")))
static volatile struct limine_executable_address_request kaddr_req = {
    .id = LIMINE_EXECUTABLE_ADDRESS_REQUEST_ID,
    .revision = 0
};

__attribute__((used, section(".limine_requests_start")))
static volatile uint64_t limine_requests_start[4] = LIMINE_REQUESTS_START_MARKER;

__attribute__((used, section(".limine_requests_end")))
static volatile uint64_t limine_requests_end[2] = LIMINE_REQUESTS_END_MARKER;

extern uint8_t _kernel_start[];
extern uint8_t _kernel_end[];

void get_kernel_addr(void){
	// add check
	kernel_state.kernel_physical_base = kaddr_req.response->physical_base;
	kernel_state.kernel_virtual_base = kaddr_req.response->virtual_base;
	kernel_state.kernel_physical_end = kernel_state.kernel_physical_base + ((uintptr_t)_kernel_end - (uintptr_t)_kernel_start);
	kernel_state.kernel_virtual_end = (uintptr_t)_kernel_end;
}

void fb_init(void) {
	if (framebuffer_request.response == NULL || framebuffer_request.response->framebuffer_count < 1) {
		return; /* Guard against NULL dereference */
	}

	for (uint64_t i = 0; i < framebuffer_request.response->framebuffer_count; i++) {
		struct limine_framebuffer *framebuffer = framebuffer_request.response->framebuffers[i];

		if (framebuffer->memory_model != LIMINE_FRAMEBUFFER_RGB || framebuffer->bpp != 32) {
			continue;
		}
		init_fb_driver((uintptr_t)framebuffer->address, framebuffer->width, framebuffer->height, framebuffer->pitch);
	}
}

void mmap_init(void) {
	if (memmap_request.response == NULL || memmap_request.response->entry_count < 1) {
		return; /* Guard against NULL dereference */
	}

	for (uint64_t i = 0; i < memmap_request.response->entry_count; i++) {
		struct limine_memmap_entry *mm_entry = memmap_request.response->entries[i];

		if (mm_entry->type == LIMINE_MEMMAP_USABLE) {
			available_mm_register(mm_entry->base, mm_entry->length);
		}
	}
}

void hhdm_init(void) {
	if (hhdm_request.response == NULL) {
		return; /* Guard against NULL dereference */
	}
	kernel_state.hhdm_offset = hhdm_request.response->offset;
}

void limine_parse_responses(void) {
	if (LIMINE_BASE_REVISION_SUPPORTED(limine_base_revision) == 0) {
		return;
	}

	get_kernel_addr();
	fb_init();
	mmap_init();
	hhdm_init();
}