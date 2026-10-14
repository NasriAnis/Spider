#include "screen.h"
#include "../arch/x86/ports.h"

unsigned char keyboard_map[128] = {
    0,  27, '1', '2', '3', '4', '5', '6', '7', '8', '9', '0', '-', '=', '\b',
    '\t', 'q', 'w', 'e', 'r', 't', 'y', 'u', 'i', 'o', 'p', '[', ']', '\n',
    0, 'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', ';', '\'', '`',
    0, '\\', 'z', 'x', 'c', 'v', 'b', 'n', 'm', ',', '.', '/', 0,
    '*', 0, ' ', 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
};

void keyboard_handler(){
    unsigned char status;
    unsigned char scancode;

    status = port_byte_in(0x64);
    if (status & 0x01) {
        scancode = port_byte_in(0x60);

        // Check if it's a key release (break code) by checking the high bit
        if (scancode & 0x80) {
            // Key released (ignore for now)
            return;
        } else {
            // Translate scancode to ASCII character
            char letter = keyboard_map[scancode];
            if (letter != 0) {
                char str[2] = {letter, '\0'};
                print(str); // Print the actual character typed!
            }
        }
    }
}