#include "mem/pmm.h"
#include "../kernel/console.h"

void init_kernel_utilities(){
	pmm_init_regions();
	printnl("[/] init PMM sucesfully");
}
