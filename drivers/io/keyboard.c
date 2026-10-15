#include "../../kernel/console.h"
#include "../../arch/x86/pio.h"
#include "../../lib/string.h"
#include "keyboard.h"

unsigned char keyboard_map[128] = {
	0,  0, '1', '2', '3', '4', '5', '6', '7', '8', '9', '0', '-', '=', '\b',
	'\t', 'q', 'w', 'e', 'r', 't', 'y', 'u', 'i', 'o', 'p', '[', ']', '\n',
	0, 'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', ';', '\'', '`',
	0, '\\', 'z', 'x', 'c', 'v', 'b', 'n', 'm', ',', '.', '/', 0,
	'*', 0, ' ', 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
};
unsigned char keyboard_map_shift[128] = {
    [2]='!', [3]='@', [4]='#', [5]='$', [6]='%', [7]='^', [8]='&', [9]='*',
    [10]='(', [11]=')', [12]='_', [13]='+', [14]='\b', [15]='\t',
    [16]='Q', [17]='W', [18]='E', [19]='R', [20]='T', [21]='Y', [22]='U',
    [23]='I', [24]='O', [25]='P', [26]='{', [27]='}', [28]='\n',
    [30]='A', [31]='S', [32]='D', [33]='F', [34]='G', [35]='H', [36]='J',
    [37]='K', [38]='L', [39]=':', [40]='"', [41]='~', [43]='|',
    [44]='Z', [45]='X', [46]='C', [47]='V', [48]='B', [49]='N', [50]='M',
    [51]='<', [52]='>', [53]='?', [57]=' ',
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
    asm volatile("cli");
    while (kbd_head == kbd_tail) /* empty */
        asm volatile("sti; hlt; cli");

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
            print("\n");
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
            print(s);
        }
        /* if the line is full, extra chars are silently ignored */
    }
}