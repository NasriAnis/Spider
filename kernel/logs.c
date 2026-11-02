#include <io.h>
#include <kernel/logs.h>

void register_error_message(enum ErrorSeverity severity, char* issuer, char* error_message){
	int i = arch_error_log.errors_number;
	arch_error_log.errors_log[i].severity = severity;
	arch_error_log.errors_log[i].issuer = issuer;
	arch_error_log.errors_log[i].error_message = error_message;
	arch_error_log.errors_number += 1;
}

void handle_critical_logs(){
	for (int i = 0; i<arch_error_log.errors_number; i++){
		if (arch_error_log.errors_log[i].severity == HIGH){
			eputs("[Critical Err] ");
			printf(":: %s ", arch_error_log.errors_log[i].issuer);
			printf(":: %s\n", arch_error_log.errors_log[i].error_message);
		}
	}
}