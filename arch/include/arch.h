#include <int.h>

void init_arch(void);

typedef struct {
    uint64_t hhdm_offset;
} KernelState;

extern KernelState kernel_state;