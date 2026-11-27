#include <io.h>

#define PANIC(fmt, ...)                                                        \
    do {                                                                       \
        printf("PANIC: %s:%d: " fmt "\n", __FILE__, __LINE__, ##__VA_ARGS__);  \
        while (1) {}                                                           \
    } while (0)

/*
 * Kernel image:  virt = phys + (kernel_virtual_base - kernel_physical_base)
 *               (e.g. phys 0xbfb00000 -> virt 0xffffffff80000000)
 *
 * the linker script puts the kernel at 0xffffffff80000000,
 * but Limine loaded the file at some physical address like 0xbfb00000.
 * The symbols like __text_start hold addresses in the second form.
 *
 * this Macro gives an Address of a function or global in the kernel binary
 * in a phys addr format
 */
#define KPHYS(v) ((uint64_t)(v) - kstate.kernel_virtual_base \
                                + kstate.kernel_physical_base)