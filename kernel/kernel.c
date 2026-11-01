#include <io.h>
#include <arch.h>
#include <int.h>
#include <shell/shell.h>
#include <drivers/init.h>
#include <drivers/mem/pmm.h>
#include <kernel/console.h>

KernelState kernel_state = {0};
void log_kernel_state();

void kmain(void){
	init_arch();
	init_kernel_utilities();
	log_kernel_state();

	putsnl("\n\nWelcome to the Spider Os !\n");

	kernel_shell_run();

	putsnl("Looping...");
	while (1) {
	}
}

void log_kernel_state(){
	printf("\nKernel State:");
	printf("\n- Kernel Physical base   : %x", kernel_state.kernel_physical_base);
	printf("\n- Kernel Physical end    : %x", kernel_state.kernel_physical_end);
	printf("\n- Kernel Virtual base    : %x", kernel_state.kernel_virtual_base);
	printf("\n- Kernel Virtual end     : %x", kernel_state.kernel_virtual_end);
	printf("\n- Kernel HHDM offset     : %x", kernel_state.hhdm_offset);
}