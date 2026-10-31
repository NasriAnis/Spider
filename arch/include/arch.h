#include <int.h>

void init_arch(void);

typedef struct {
    uintptr_t hhdm_offset;
    uintptr_t kernel_physical_base;
    uintptr_t kernel_physical_end;
    uintptr_t kernel_virtual_base;
    uintptr_t kernel_virtual_end;
} KernelState;

extern KernelState kernel_state;