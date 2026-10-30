#include "mem/pmm.h"
#include <io.h>

void init_kernel_utilities(){
	pmm_init_regions();
	putsnl("[ok] init PMM sucesfully");
}
