#include <arch.h>
#include <io.h>
#include <gdt.h>
#include <idt.h>

void init_arch(void){
	init_idt();
	putsnl("[/] Init IDT tables and interrupts succesfully");
	init_gdt();
	putsnl("[/] Init GDT tables succesfully");
}