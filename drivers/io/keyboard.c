#include <io.h>
#include <pio.h>
#include <kernel/console.h>
#include <string.h>
#include "keyboard.h"

unsigned char keyboard_map[128] = {
	0,  0, '1', '2', '3', '4', '5', '6', '7', '8', '9', '0', '-', '=', '\b',
	'\t', 'q', 'w', 'e', 'r', 't', 'y', 'u', 'i', 'o', 'p', '[', ']', '\n',
	0, 'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', ';', '\'', '`',
	0, '\\', 'z', 'x', 'c', 'v', 'b', 'n', 'm', ',', '.', '/', 0,
	'*', 0, ' ', 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
};
unsigned char keyboard_map_shift[128] = {
	0,  0, '!', '@', '#', '$', '%', '^', '&', '*', '(', ')', '_', '+', '\b',
	'\t', 'Q', 'W', 'E', 'R', 'T', 'Y', 'U', 'I', 'O', 'P', '{', '}', '\n',
	0, 'A', 'S', 'D', 'F', 'G', 'H', 'J', 'K', 'L', ':', '"', '~',
	0, '|', 'Z', 'X', 'C', 'V', 'B', 'N', 'M', '<', '>', '?', 0,
	'*', 0, ' ', 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
};

static int shift = 0;
static int extended = 0;

#define BACKSPACE 0xE
#define ENTER 0x1c
#define ARROW 0xE0
#define RIGHT_SHIFT_PRESS 0x36
#define LEFT_SHIFT_PRESS 0x2A
#define RIGHT_SHIFT_RELEASE 0xB6
#define LEFT_SHIFT_RELEASE 0xAA

#define KBD_BUF_SIZE 128
static volatile char kbd_buf[KBD_BUF_SIZE];
static volatile uint32_t kbd_head = 0;
static volatile uint32_t kbd_tail = 0;

static void kbd_push(char c) {
	uint32_t next = (kbd_head + 1) % KBD_BUF_SIZE;
	if (next == kbd_tail) return;
	kbd_buf[kbd_head] = c;
	kbd_head = next;
}

void keyboard_on_scancode(unsigned char sc) {
	/* arrow keys */
	if (sc == ARROW) { extended = 1; return; }
	if (extended)   { extended = 0; return; }

	if (sc == LEFT_SHIFT_PRESS || sc == RIGHT_SHIFT_PRESS) { shift = 1; return; }
	if (sc == LEFT_SHIFT_RELEASE || sc == RIGHT_SHIFT_RELEASE) { shift = 0; return; }

	/* high bit set = key release, ignore for now */
	if (sc & 0x80) return;

	char c = shift ? keyboard_map_shift[sc] : keyboard_map[sc];
	if (c) kbd_push(c);
}

char getchar(void) {
	while (kbd_head == kbd_tail) /* empty */
		__asm__ volatile ("hlt");

	asm volatile("cli");
	char c = kbd_buf[kbd_tail];
	kbd_tail = (kbd_tail + 1) % KBD_BUF_SIZE;
	asm volatile("sti");
	return c;
}

void readline(char *line, size_t max) {
	size_t len = 0;
	for (;;) {
		char c = getchar();

		if (c == '\n') {
			puts("\n");
			line[len] = '\0';
			return;
		}
		if (c == '\b') {
			if (len > 0) {
				len--;
				backspace();
			}
			continue;
		}
		if (c == '\t') c = ' '; /* tab = space for now */

		if (len < max - 1) { /* Add null terminator */
			line[len++] = c;
			char s[2] = { c, '\0' };
			puts(s);
		}
		/* if the line is full, extra chars are silently ignored */
	}
}
