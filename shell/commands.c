#include "commands.h"
#include <io.h>
#include <kernel/console.h>

void cmd_clear(int argc, char **argv) {
	(void)argc; (void)argv;
	cls();
}

void cmd_echo(int argc, char **argv) {
	(void)argv;
	for (int i = 1; i < argc; i++) {
		puts(argv[i]);
		puts(" ");
	}
	puts("\n");
}

const struct command commands[] = {
	{"clear", cmd_clear},
	{"echo",  cmd_echo},
};

const size_t commands_count = sizeof commands / sizeof commands[0];
