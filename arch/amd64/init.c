#include <arch.h>
#include <io.h>
#include "boot/limine.h"

void init_arch(void){
	limine_parse_responses();
}