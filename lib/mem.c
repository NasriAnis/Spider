#include <int.h>
#include <drivers/mem/pmm.h>

void memset(uint32_t *dest, uint8_t val, uint32_t len) {
	uint8_t *temp = (uint8_t *)dest;
	for ( ; len != 0; len--) *temp++ = val;
}

void *memcpy(void *dest, const void *src, size_t n) {
	uint8_t *d = dest;
	const uint8_t *s = src;
	for (size_t i = 0; i < n; i++)
		d[i] = s[i];
	return dest;
}

void *memmove(void *dest, const void *src, size_t n) {
	uint8_t *d = dest;
	const uint8_t *s = src;
	if (d < s) {
		for (size_t i = 0; i < n; i++) d[i] = s[i];       // forward
	} else if (d > s) {
		for (size_t i = n; i > 0; i--) d[i-1] = s[i-1];   // backward
	}
	return dest;
}

uintptr_t memalloc(uint64_t size){
	return pmm_alloc(size);
}

void memfree(uintptr_t addr, uint64_t size){
	pmm_free(addr, size);
}
