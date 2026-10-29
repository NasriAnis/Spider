#include <tss.h>
#include <gdt.h>

void set_kernel_stack(uint32_t stack) { // Used when an interrupt occurs
	tss_entry.esp0 = stack;
}

void load_task_register(){
	asm("mov $(5 * 8), %ax");
	asm("ltr %ax");
}