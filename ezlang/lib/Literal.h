#pragma once

#include "types.h"

typedef union Value {
	float f;
	int i;
	int b;
	char* s;
} Value ;

//Exemplos de literal: 1, 2, 'a', 1.3
typedef struct Literal {
	Value value;
	Type type;
} Literal;

//Exemplos de Variável: float a = 1.5, var = 1
typedef struct Variable {
	char* name;
	int line;
	int scope_level;
	Literal literal;
} Variable ;

/* program state */
void literal_init(void);
void literal_deinit(void);

/* conversions */
int literal_str_to_int(Literal* l, const char* text);
int literal_str_to_flt(Literal* l, const char* text);
int literal_str_to_boo(Literal* l, int v);
int literal_str(Literal* l, const char* text);
int literal_to_str(Literal* l, char** str);

/* operations */
int literal_sum(Literal* op1, Literal* op2, BinaryOperationData* meta);
int literal_minus(Literal* op1, Literal* op2, BinaryOperationData* meta);
int literal_mult(Literal* op1, Literal* op2, BinaryOperationData* meta);
int literal_div(Literal* op1, Literal* op2, BinaryOperationData* meta);
int literal_lt(Literal* op1, Literal* op2, BinaryOperationData* meta);
int literal_eq(Literal* op1, Literal* op2, BinaryOperationData* meta);
int literal_assign(Literal* op1, UnaryOperationData* meta);

/* variable */
int variable_str(Variable* var, char* name);
