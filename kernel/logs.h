#include <int.h>

typedef struct {
	uintptr_t hhdm_offset;
	uintptr_t kernel_physical_base;
	uintptr_t kernel_physical_end;
	uintptr_t kernel_virtual_base;
	uintptr_t kernel_virtual_end;
} KernelState;

enum LogType { NOTICE, MISS, FAILS };
enum LogAtState { ARCH, KERNEL, USERLAND };

typedef struct {
	enum LogType log_type;
	enum LogAtState at_state;
	char *issuer;
	char *log_message;
} LogInfo;

typedef struct {
	int log_number;
	LogInfo log[];
} KernelLogs;

extern KernelState kernel_state;
extern KernelLogs kernel_logs;

void register_log(enum LogType log_type, enum LogAtState at_state, char* issuer, char* log_message);
void show_logs(enum LogType log_type, enum LogAtState at_state);
char* string_logtype_enum(enum LogType log_type);