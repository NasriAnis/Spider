#include "console.h"
#include "../arch/arch.h"
#include "../lib/int.h"
#include "shell/shell.h"

#define MB2_BOOT_MAGIC 0x36D76289

void kmain(uint32_t magic, void *mbi){
	clear_screen();

	if (magic != MB2_BOOT_MAGIC) {
		eprintnl("Not booted by a Multiboot2 loader");
		asm volatile("cli; hlt");
	}
	(void)mbi; // will parse the tags later

	init_arch();

	printnl("\nWelcome to the Spider Os !\n");

	kernel_shell_run();

	while (1) {
	}
}