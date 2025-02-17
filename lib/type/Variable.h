#pragma once

#include "type/Literal.h"

typedef struct Variable {
	char* name;
	int line;
	Literal token;
} Variable;

int variable_cmp(Variable* var1, Variable* var2);
int variable_name_equals(Variable* var, const char* name);
void variable_print(Variable* var);
