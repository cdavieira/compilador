#include "type/Variable.h"
#include <stdio.h>
#include <string.h>

int variable_name_equals(Variable* var, const char* name){
	return strcmp(var->name, name) == 0;
}

void variable_print(Variable* var){
	printf("name='%s', line=%d, type=%d\n", var->name, var->line, var->token.type);
}
