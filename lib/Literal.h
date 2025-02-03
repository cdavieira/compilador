#pragma once

typedef union Value {
	int i;
	float f;
	char c;
	char* s;
} Value ;

enum Type {
	TYPE_VOID = 0,
	TYPE_INT,
	TYPE_FLT,
	TYPE_CHAR,
	TYPE_STR
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

const char* type_get_name(enum Type type);
const char* literal_get_typename(const Literal* td);
int literal_sum(Literal* op1, Literal* op2, Literal* res);
int literal_sub(Literal* op1, Literal* op2, Literal* res);
int literal_mul(Literal* op1, Literal* op2, Literal* res);
int literal_div(Literal* op1, Literal* op2, Literal* res);
int literal_and(Literal* op1, Literal* op2, Literal* res);
int literal_or(Literal* op1, Literal* op2, Literal* res);
int literal_lt(Literal* op1, Literal* op2, Literal* res);
int literal_gt(Literal* op1, Literal* op2, Literal* res);
int literal_ne(Literal* op1, Literal* op2, Literal* res);
int literal_eq(Literal* op1, Literal* op2, Literal* res);
int literal_assign(Literal* op1, Literal* res);

int variable_name_equals(Variable* var, const char* name);
void variable_print(Variable* var);
