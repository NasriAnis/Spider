#include <io.h>
#include <int.h>
#include <shell/shell.h>
#include <drivers/init.h>
#include <drivers/mem/pmm.h>
#include <kernel/console.h>
#include <kernel/logs.h>
#include <arch.h>

KernelState kernel_state = {0};
ArchErrors arch_error_log;

void log_kernel_state();

void kmain(void){
	arch_error_log.errors_number = 0;

	init_arch();
	handle_critical_logs();
	init_kernel_utilities();
	log_kernel_state();

	putsnl("\nWelcome to the Spider Os !\n");

	kernel_shell_run();

	putsnl("Looping...");
	while (1) {
	}
}

void log_kernel_state(){
	printf("Kernel State:\n");
	printf("- Kernel Physical base   : %p\n", kernel_state.kernel_physical_base);
	printf("- Kernel Physical end    : %p\n", kernel_state.kernel_physical_end);
	printf("- Kernel Virtual base    : %p\n", kernel_state.kernel_virtual_base);
	printf("- Kernel Virtual end     : %p\n", kernel_state.kernel_virtual_end);
	printf("- Kernel HHDM offset     : %x\n", kernel_state.hhdm_offset);
}