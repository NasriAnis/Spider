#include "../../arch.h"
#include "./cpu/gdt.h"
#include "./cpu/idt.h"
#include "../../../kernel/console.h"

void init_arch(void){
	init_idt();
	printnl("[/] Init IDT tables and interrupts succesfully");
	init_gdt();
	printnl("[/] Init GDT tables succesfully");
}