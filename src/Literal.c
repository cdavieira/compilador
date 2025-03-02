#include "Literal.h"
#include <stdio.h>
#include <string.h>

const char* type_name(enum Type type){
	switch(type){
		case TYPE_INT:
			return "int";
		case TYPE_FLT:
			return "float";
		case TYPE_STR:
			return "char*";
		case TYPE_CHAR:
			return "char";
		case TYPE_VOID:
			return "void";
		default:
			break;
	}
	return "unknown typename";
}

int variable_name_equals(Variable* var, const char* name){
	return strcmp(var->name, name) == 0;
}

void variable_print(Variable* var){
	printf("name='%s', line=%d, type=%d, qualifier=%d\n", var->name, var->line, var->type, var->qualifier);
}
