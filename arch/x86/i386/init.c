#include "../../arch.h"
#include "gdt.h"
#include "idt.h"

void init_arch(void){
	init_gdt();
	init_idt();
}