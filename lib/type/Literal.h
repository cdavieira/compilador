#pragma once

#include "type/Type.h"
#include "type/Value.h"

typedef struct Literal {
	enum Type type;
	Value value;
} Literal;

const char* literal_get_typename(const Literal* td);
int literal_sum(Literal* op1, Literal* op2, BinaryOperationData* res);
int literal_sub(Literal* op1, Literal* op2, BinaryOperationData* res);
int literal_mul(Literal* op1, Literal* op2, BinaryOperationData* res);
int literal_div(Literal* op1, Literal* op2, BinaryOperationData* res);
int literal_and(Literal* op1, Literal* op2, BinaryOperationData* res);
int literal_or(Literal* op1, Literal* op2, BinaryOperationData* res);
int literal_lt(Literal* op1, Literal* op2, BinaryOperationData* res);
int literal_gt(Literal* op1, Literal* op2, BinaryOperationData* res);
int literal_ne(Literal* op1, Literal* op2, BinaryOperationData* res);
int literal_eq(Literal* op1, Literal* op2, BinaryOperationData* res);
int literal_assign(Literal* op1, UnaryOperationData* res);
