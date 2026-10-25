#include "commands.h"
#include <io.h>

void cmd_clear(int argc, char **argv) {
	(void)argc; (void)argv;
	clear_screen();
}

void cmd_echo(int argc, char **argv) {
	(void)argv;
	for (int i = 1; i < argc; i++) {
		print(argv[i]);
		print(" ");
	}
	print("\n");
}

const struct command commands[] = {
	{"clear", cmd_clear},
	{"echo",  cmd_echo},
};

const size_t commands_count = sizeof commands / sizeof commands[0];