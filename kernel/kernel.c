#include <io.h>
#include <arch.h>
#include <int.h>
#include <shell/shell.h>
#include <drivers/init.h>
#include <drivers/mem/pmm.h>
#include <kernel/console.h>

void kmain(void){
	cls();

	init_arch();
	init_kernel_utilities();

	putsnl("\nWelcome to the Spider Os !\n");

	kernel_shell_run();

	while (1) {
	}
}
