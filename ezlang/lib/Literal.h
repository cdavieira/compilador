#pragma once

enum Type {
	TYPE_INT = 0,
	TYPE_FLT = 1,
	TYPE_BOO = 2,
	TYPE_STR = 3,
};

typedef union Value {
	float f;
	int i;
	int b;
	char* s;
} Value ;




//Exemplos de literal: 1, 2, 'a', 1.3
typedef struct Literal {
	Value value;
	enum Type type;
} Literal;

void literal_init(void);
void literal_deinit(void);
int literal_str_to_int(Literal* l, const char* text);
int literal_str_to_flt(Literal* l, const char* text);
int literal_str_to_boo(Literal* l, int v);
int literal_str(Literal* l, const char* text);
int literal_to_str(Literal* l, char** str);
const char* literal_get_type_name(enum Type type);
int literal_sum(
	Literal* op1,
	Literal* op2,
	Literal* res);
int literal_minus(
	Literal* op1,
	Literal* op2,
	Literal* res);
int literal_mult(
	Literal* op1,
	Literal* op2,
	Literal* res);
int literal_div(
	Literal* op1,
	Literal* op2,
	Literal* res);
int literal_lt(
	Literal* op1,
	Literal* op2,
	Literal* res);
int literal_eq(
	Literal* op1,
	Literal* op2,
	Literal* res);
int literal_assign(
	Literal* op1,
	Literal* res);





//Exemplos de Variável: float a = 1.5, var = 1
typedef struct Variable {
	char* name;
	int line;
	int scope_level;
	Literal literal;
} Variable ;

int variable_str(Variable* var, char* name);




/* This struct is an alternative to the callback-based solution when executing
 * binary operations */
typedef struct Operator {
	char* name;
	enum OperatorType {
		OPERATOR_SUM,
		OPERATOR_MINUS,
		OPERATOR_MUL,
		OPERATOR_DIV,
		OPERATOR_LT,
		OPERATOR_EQ,
		OPERATOR_ASSIGN,
	} type;
} Operator;

int operator_str(Operator* l, const char* text, enum OperatorType type);
