#include "../../../lib/int.h"
#include "../../../kernel/console.h"

extern void kmain(void);

#define MB2_BOOT_MAGIC 0x36D76289

void boot_check(uint32_t magic, void *mbi){
	if (magic != MB2_BOOT_MAGIC) {
		eprintnl("Not booted by a Multiboot2 loader");
		asm volatile("cli; hlt");
	}
	(void)mbi; // will parse the tags later

	// call main kernel
	kmain();
}