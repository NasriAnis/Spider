#include <io.h>

#define PANIC(fmt, ...)                                                        \
    do {                                                                       \
        printf("PANIC: %s:%d: " fmt "\n", __FILE__, __LINE__, ##__VA_ARGS__);  \
        while (1) {}                                                           \
    } while (0)

#define KPHYS(v) ((uint64_t)(v) - kernel_state.kernel_virtual_base \
                                + kernel_state.kernel_physical_base)