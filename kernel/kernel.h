#include <io.h>

#define PANIC(fmt, ...)                                                        \
    do {                                                                       \
        printf("PANIC: %s:%d: " fmt "\n", __FILE__, __LINE__, ##__VA_ARGS__);  \
        while (1) {}                                                           \
    } while (0)

#define KPHYS(v) ((uint64_t)(v) - kstate.kernel_virtual_base \
                                + kstate.kernel_physical_base)