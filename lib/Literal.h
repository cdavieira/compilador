#pragma once

typedef union Value {
	int i;
	float f;
	char c;
	char* s;
} Value ;

enum Type {
	TYPE_OK = -1,
	TYPE_VOID = 0,
	TYPE_INT,
	TYPE_FLT,
	TYPE_CHAR,
	TYPE_STR,
};

typedef struct Literal {
	enum Type type;
	Value value;
} Literal;

typedef struct Variable {
	char* name;
	int line;
	Literal token;
} Variable;

const char* type_name(enum Type type);
int variable_name_equals(Variable* var, const char* name);
void variable_print(Variable* var);
