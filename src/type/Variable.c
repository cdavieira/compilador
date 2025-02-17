#include "type/Variable.h"
#include <stdio.h>
#include <string.h>

int variable_cmp(Variable* var1, Variable* var2){
	int same_name = strcmp(var1->name, var2->name) == 0;
	int same_line = var1->line == var2->line;
	/* int same_type = var1->token.type == var2->token.type; */
	return same_name && same_line;
}

int variable_name_equals(Variable* var, const char* name){
	return strcmp(var->name, name) == 0;
}

void variable_print(Variable* var){
	printf("name='%s', line=%d, type=%d\n", var->name, var->line, var->token.type);
}
