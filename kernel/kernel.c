#include <io.h>
#include <arch.h>
#include <int.h>
#include <shell/shell.h>
#include <drivers/init.h>

void kmain(void){
	clear_screen();

	init_arch();
	init_kernel_utilities();

	printnl("\nWelcome to the Spider Os !\n");

	kernel_shell_run();

	while (1) {
	}
}
