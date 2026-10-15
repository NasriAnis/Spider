#include "../../lib/int.h"
#include "../console.h"
#include "../../drivers/io/keyboard.h"
#include "../../lib/string.h"
#include "commands.h"

void kernel_shell_run(void) {
	char line[128];
	char *argv[16];
	for (;;) {
		print("spider> ");
		readline(line, sizeof line);
		int argc = tokenize(line, argv, 16);
		if (argc == 0) continue;
		int found = 0;
		for (size_t i = 0; i < commands_count ; i++)
			if (strcmp(argv[0], commands[i].name) == 0) {
				commands[i].fn(argc, argv); found = 1; break;
			}
		if (!found) eprintnl("unknown command");
	}
}