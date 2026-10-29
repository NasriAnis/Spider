#include <io.h>
#include <arch.h>
#include <int.h>
#include <shell/shell.h>
#include <drivers/init.h>
#include <drivers/mem/pmm.h>
#include <kernel/console.h>

KernelState kernel_state = {0};

void kmain(void){
	init_arch();
	init_kernel_utilities();

	putsnl("\nWelcome to the Spider Os !\n");

	kernel_shell_run();

	while (1) {
	}
}
