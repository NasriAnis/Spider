#include <int.h>

typedef struct {
	uintptr_t hhdm_offset;
	uintptr_t kernel_physical_base;
	uintptr_t kernel_physical_end;
	uintptr_t kernel_virtual_base;
	uintptr_t kernel_virtual_end;
} KernelState;

enum ErrorSeverity { LOW, MEDIUM, HIGH };

typedef struct {
	enum ErrorSeverity severity;
	char *issuer;
	char *error_message;
} ErrorLog;

typedef struct {
	int errors_number;
	ErrorLog errors_log[];
} ArchErrors;

extern KernelState kernel_state;
extern ArchErrors arch_error_log;

void register_error_message(enum ErrorSeverity severity, char* issuer, char* error_message);
void handle_critical_logs();