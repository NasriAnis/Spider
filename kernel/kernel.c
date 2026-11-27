#include <io.h>
#include <int.h>
#include <shell/shell.h>
#include <drivers/mem/pmm.h>
#include <kernel/console.h>
#include <kernel/logs.h>
#include <arch.h>

KernelState kstate = {0};
KernelLogs klogs = {0};

void log_kernel_state();

void kmain(void){
	klogs.log_number = 0;

	init_arch();
	log_kernel_state();

	putsnl("\nWelcome to the Spider Os !\n");

	kernel_shell_run();

	putsnl("Looping...");
	while (1) {
	}
}

void log_kernel_state(){
	printf("Kernel State:\n");
	printf("- Kernel Physical base   : %p\n", kstate.kernel_physical_base);
	printf("- Kernel Physical end    : %p\n", kstate.kernel_physical_end);
	printf("- Kernel Virtual base    : %p\n", kstate.kernel_virtual_base);
	printf("- Kernel Virtual end     : %p\n", kstate.kernel_virtual_end);
	printf("- Kernel HHDM offset     : %p\n", kstate.hhdm_offset);
}