#ifndef PIO_H
#define PIO_H

#include <int.h>

static inline uint8_t port_byte_in(uint16_t port) {
	uint8_t result;
	__asm__ volatile ("inb %w1, %b0" : "=a"(result) : "Nd"(port));
	return result;
}

static inline void port_byte_out(uint16_t port, uint8_t data) {
	__asm__ volatile ("outb %b0, %w1" : : "a"(data), "Nd"(port));
}

static inline uint16_t port_word_in(uint16_t port) {
	uint16_t result;
	__asm__ volatile ("inw %w1, %w0" : "=a"(result) : "Nd"(port));
	return result;
}

static inline void port_word_out(uint16_t port, uint16_t data) {
	__asm__ volatile ("outw %w0, %w1" : : "a"(data), "Nd"(port));
}

static inline void io_wait(void) {
	__asm__ volatile ("outb %b0, $0x80" : : "a"((uint8_t)0));
}

#endif