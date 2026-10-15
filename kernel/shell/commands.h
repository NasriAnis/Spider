#include "../../lib/int.h"

struct command {
    const char *name;
    void (*fn)(int argc, char **argv);
};

extern const struct command commands[];
extern const size_t commands_count;

void cmd_clear(int argc, char **argv);
void cmd_echo(int argc, char **argv);