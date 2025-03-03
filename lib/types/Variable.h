#pragma once

#include "types/Type.h"

enum Qualifier {
	QUALIFIER_POINTER = -1,
	QUALIFIER_BASIC = 0,
	//QUALIFIER_ARRAY > 0
};

typedef struct Variable {
	char* name;
	int line;
	enum Qualifier qualifier;
	enum Type type;
} Variable;

int variable_name_equals(Variable* var, const char* name);

void variable_print(Variable* var);
