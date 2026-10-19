#include "../../../../lib/int.h"
#include "../../../../kernel/console.h"
#include "multiboot2.h"

extern void kmain(void);
extern void init_vga_text_driver(void);
extern void init_vga_text_driver(void);
extern void init_fb_driver(uintptr_t addr, uint32_t width, uint32_t height, uint32_t pitch);

#define MB2_BOOT_MAGIC 0x36D76289

static int using_framebuffer = 0;

void multiboot2_check(uint32_t magic, uint32_t mbi_addr) {
	if (magic != MB2_BOOT_MAGIC) {
		eprintnl("Not booted by a Multiboot2 loader");
		asm volatile("cli; hlt");
	}

	// Verify alignment (Multiboot2 requires 8-byte alignment)
	if (mbi_addr & 7) {
		eprintnl("Unaligned Multiboot2 info pointer!");
		return;
	}

	// The total size of the multiboot structure is in the first 4 bytes
	uint32_t total_size = *(uint32_t *)mbi_addr;

	// Tags begin 8 bytes after the address (skip total_size and reserved field)
	struct multiboot_tag *tag = (struct multiboot_tag *)(mbi_addr + 8);

	// Loop until we hit the end tag (type 0)
	while (tag->type != MULTIBOOT_TAG_TYPE_END) {
		switch (tag->type) {
			case MULTIBOOT_TAG_TYPE_MMAP: {
				struct multiboot_tag_mmap *mmap = (struct multiboot_tag_mmap *)tag;
				struct multiboot_mmap_entry *entry;

				// Iterate through all memory map regions
				for (entry = mmap->entries;
					 (uint8_t *)entry < (uint8_t *)tag + mmap->size;
					 entry = (struct multiboot_mmap_entry *)((uint8_t *)entry + mmap->entry_size)) {

					// entry->type == 1 means standard available RAM
					if (entry->type == MULTIBOOT_MEMORY_AVAILABLE) {
						// Pass entry->addr and entry->len to physical memory manager (PMM)
					}
				}
				break;
			}

			case MULTIBOOT_TAG_TYPE_FRAMEBUFFER: {
				struct multiboot_tag_framebuffer *tagfb = (struct multiboot_tag_framebuffer *)tag;

				uintptr_t fb_address = (uintptr_t)tagfb->common.framebuffer_addr;
				uint32_t width       = tagfb->common.framebuffer_width;
				uint32_t height      = tagfb->common.framebuffer_height;
				uint32_t pitch       = tagfb->common.framebuffer_pitch;
				uint8_t bpp          = tagfb->common.framebuffer_bpp;

				init_fb_driver(fb_address, width, height, pitch);
				using_framebuffer = 1;
				break;
			}
		}

		// Advance to the next tag (tags are strictly 8-byte aligned)
		tag = (struct multiboot_tag *)((uint8_t *)tag + ((tag->size + 7) & ~7));
	}

	if (!using_framebuffer) {
		init_vga_text_driver();
	}
}