#include <arch.h>
#include <io.h>
#include "cpu/gdt.h"
#include "cpu/idt.h"

void init_arch(void){
	init_idt();
	printnl("[/] Init IDT tables and interrupts succesfully");
	init_gdt();
	printnl("[/] Init GDT tables succesfully");
}