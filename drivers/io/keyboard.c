#include "../../kernel/console.h"
#include "../../arch/x86/ports.h"

unsigned char keyboard_map[128] = {
	0,  27, '1', '2', '3', '4', '5', '6', '7', '8', '9', '0', '-', '=', '\b',
	'\t', 'q', 'w', 'e', 'r', 't', 'y', 'u', 'i', 'o', 'p', '[', ']', '\n',
	0, 'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', ';', '\'', '`',
	0, '\\', 'z', 'x', 'c', 'v', 'b', 'n', 'm', ',', '.', '/', 0,
	'*', 0, ' ', 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
};

void keyboard_on_scancode(unsigned char scancode) {
	/* high bit set = key release, ignore for now */
	if (scancode & 0x80)
		return;

	char letter = keyboard_map[scancode];
	if (letter != 0) {
		char str[2] = { letter, '\0' };
		print(str);
	}
}