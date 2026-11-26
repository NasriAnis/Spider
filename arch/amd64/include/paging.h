#include <int.h>

// PML4 flags
#define PML4_P          (0b1 << 0)
#define PML4_RW         (0b1 << 1)
#define PML4_US         (0b1 << 2)
#define PML4_PWT        (0b1 << 3)
#define PML4_PCD        (0b1 << 4)
#define PML4_PDPT(addr) (addr << 12)

// #define CR4_PCID  1
// #define CR4_LA57  0
// #define CR0_PG    1
// #define EFER_LME  1

void setup_paging(void);