#include "../drivers/screen.h"
#include "../arch/init.h"
#include "../lib/int.h"

#define MB2_BOOT_MAGIC 0x36D76289

void kmain(uint32_t magic, void *mbi){
	clear_screen();

	if (magic != MB2_BOOT_MAGIC) {
		eprintnl("Not booted by a Multiboot2 loader");
		asm volatile("cli; hlt");
	}
	(void)mbi; // will parse the tags later

	printnl("Welcome to the Spider Os kernel !");

	init_arch();
	printnl("Init architecture finished !");
	while (1) {
	}
}