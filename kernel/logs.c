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
	int i = klogs.log_number;
	klogs.log[i].log_type = log_type;
	klogs.log[i].issuer = issuer;
	klogs.log[i].log_message = log_message;
	klogs.log[i].at_state = at_state;
	klogs.log_number += 1;
}

void show_logs(enum LogType log_type, enum LogAtState at_state){
	for (int i = 0; i<klogs.log_number; i++){
		if ( klogs.log[i].log_type == log_type &&
			klogs.log[i].at_state == at_state
		){
			printf("[%s] ", string_logtype_enum(log_type));
			printf(":: %s ", klogs.log[i].issuer);
			printf(":: %s\n", klogs.log[i].log_message);
		}
	}
}