#include "../../init.h"
#include "gdt.h"
#include "idt.h"

void init_arch(void){
	initGDT();
	init_idt();
}