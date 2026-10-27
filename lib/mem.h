#include <int.h>

void memset(uint32_t *dest, uint8_t val, uint32_t len);
void *memcpy(void *dest, const void *src, size_t n);
void *memmove(void *dest, const void *src, size_t n);
uintptr_t memalloc(uint64_t size);
void memfree(uintptr_t addr, uint64_t size);
