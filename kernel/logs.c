#include <io.h>
#include <kernel/logs.h>

char* string_logtype_enum(enum LogType log_type){
	switch(log_type){
		case NOTICE: return "NOTICE";
		case MISS: return "MISS";
		case FAILS: return "FAILS";
	}
}

void register_log(enum LogType log_type, enum LogAtState at_state, char* issuer, char* log_message){
	int i = kernel_logs.log_number;
	kernel_logs.log[i].log_type = log_type;
	kernel_logs.log[i].issuer = issuer;
	kernel_logs.log[i].log_message = log_message;
	kernel_logs.log[i].at_state = at_state;
	kernel_logs.log_number += 1;
}

void show_logs(enum LogType log_type, enum LogAtState at_state){
	for (int i = 0; i<kernel_logs.log_number; i++){
		if ( kernel_logs.log[i].log_type == log_type &&
			kernel_logs.log[i].at_state == at_state
		){
			printf("[%s] ", string_logtype_enum(log_type));
			printf(":: %s ", kernel_logs.log[i].issuer);
			printf(":: %s\n", kernel_logs.log[i].log_message);
		}
	}
}