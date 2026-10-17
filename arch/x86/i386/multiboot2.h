#ifndef MULTIBOOT_HEADER
#define MULTIBOOT_HEADER 1

#include "../../../lib/int.h"

#define MULTIBOOT2_BOOTLOADER_MAGIC  0x36d76289

#define MULTIBOOT_TAG_TYPE_END         0
#define MULTIBOOT_TAG_TYPE_MMAP        6
#define MULTIBOOT_TAG_TYPE_FRAMEBUFFER 8

#define MULTIBOOT_MEMORY_AVAILABLE     1

struct multiboot_tag {
	uint32_t type;
	uint32_t size;
};

// --- Memory Map Structures ---
struct multiboot_mmap_entry {
	uint64_t addr;
	uint64_t len;
	uint32_t type;
	uint32_t zero;
};

struct multiboot_tag_mmap {
	uint32_t type;
	uint32_t size;
	uint32_t entry_size;
	uint32_t entry_version;
	struct multiboot_mmap_entry entries[0];
};

// --- Framebuffer Structures ---
struct multiboot_color {
	uint8_t red;
	uint8_t green;
	uint8_t blue;
};

struct multiboot_tag_framebuffer_common {
	uint32_t type;
	uint32_t size;
	uint64_t framebuffer_addr;
	uint32_t framebuffer_pitch;
	uint32_t framebuffer_width;
	uint32_t framebuffer_height;
	uint8_t  framebuffer_bpp;
#define MULTIBOOT_FRAMEBUFFER_TYPE_INDEXED 0
#define MULTIBOOT_FRAMEBUFFER_TYPE_RGB     1
	uint8_t  framebuffer_type;
	uint16_t reserved;
};

struct multiboot_tag_framebuffer {
	struct multiboot_tag_framebuffer_common common;
	union {
		struct { // for type 0 FB
			uint16_t framebuffer_palette_num_colors;
			struct multiboot_color framebuffer_palette[0];
		};
		struct { // for type 1 FB
			uint8_t framebuffer_red_field_position;
			uint8_t framebuffer_red_mask_size;
			uint8_t framebuffer_green_field_position;
			uint8_t framebuffer_green_mask_size;
			uint8_t framebuffer_blue_field_position;
			uint8_t framebuffer_blue_mask_size;
		};
	};
};

#endif /* !MULTIBOOT_HEADER */