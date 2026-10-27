#include <int.h>
#include <io.h>
#include <drivers/io/keyboard.h>
#include <string.h>
#include "commands.h"

static const struct command *find_command(const char *name);

void kernel_shell_run(void) {
	char line[128];
	char *argv[16];
	while(1) {

		puts("spider> ");
		readline(line, sizeof line);

		int argc = tokenize(line, argv, 16);
		if (argc == 0) continue;

		const struct command *cmd = find_command(argv[0]);

		if (cmd) cmd->fn(argc, argv);
		else eputsnl("unknown command");
	}
}

static const struct command *find_command(const char *name) {
    for (size_t i = 0; i < commands_count; i++)
        if (strcmp(name, commands[i].name) == 0) return &commands[i];

    return 0;
}
