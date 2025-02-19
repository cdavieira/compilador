#include "typesys.h"

static const TypeData arith_table_lookup[][4] = {
	{{TYPE_VOID,}, {TYPE_VOID,}, {TYPE_VOID,}, {TYPE_VOID,}},
	{{TYPE_VOID,}, {TYPE_INT,CONV_NONE,CONV_NONE}, {TYPE_FLT,CONV_I2F,CONV_NONE}, {TYPE_INT,CONV_NONE,CONV_C2I}},
	{{TYPE_VOID,}, {TYPE_FLT,CONV_NONE,CONV_I2F}, {TYPE_FLT,CONV_NONE,CONV_NONE}, {TYPE_FLT,CONV_NONE,CONV_C2F}},
	{{TYPE_VOID,}, {TYPE_INT,CONV_C2I,CONV_NONE}, {TYPE_FLT,CONV_C2F,CONV_NONE}, {TYPE_INT,CONV_C2I,CONV_C2I}},
};

static const TypeData logical_table_lookup[][4] = {
	{{TYPE_VOID,}, {TYPE_VOID,}, {TYPE_VOID,}, {TYPE_VOID,}},
	{{TYPE_VOID,}, {TYPE_INT,CONV_NONE,CONV_NONE}, {TYPE_FLT,CONV_I2F,CONV_NONE},  {TYPE_INT,CONV_NONE,CONV_C2I}},
	{{TYPE_VOID,}, {TYPE_FLT,CONV_NONE,CONV_I2F},  {TYPE_FLT,CONV_NONE,CONV_NONE}, {TYPE_FLT,CONV_NONE,CONV_C2F}},
	{{TYPE_VOID,}, {TYPE_INT,CONV_C2I,CONV_NONE},  {TYPE_FLT,CONV_C2F,CONV_NONE},  {TYPE_INT,CONV_C2I,CONV_C2I}},
};

static const TypeData assign_table_lookup[][4] = {
	{{TYPE_VOID,}, {TYPE_VOID,}, {TYPE_VOID,}, {TYPE_VOID,}},
	{{TYPE_VOID,}, {TYPE_INT,CONV_NONE,CONV_NONE}, {TYPE_FLT,CONV_NONE,CONV_F2I},  {TYPE_INT,CONV_NONE,CONV_C2I}},
	{{TYPE_VOID,}, {TYPE_FLT,CONV_NONE,CONV_I2F},  {TYPE_FLT,CONV_NONE,CONV_NONE}, {TYPE_FLT,CONV_NONE,CONV_C2F}},
	{{TYPE_VOID,}, {TYPE_CHAR,CONV_NONE,CONV_I2C}, {TYPE_FLT,CONV_NONE,CONV_C2F},  {TYPE_INT,CONV_NONE,CONV_C2I}},
}; 

#define PRINT_EXPR(l1, l2, op) \
	printf("Expression type debug: %s %s %s = ...\n", \
		type_name(l1), \
		#op,\
		type_name(l2))

const TypeData typesys_sum(enum Type op1, enum Type op2){
#ifdef DEBUG_EXPR_TYPE
	PRINT_EXPR(op1, op2, +);
#endif
	return arith_table_lookup[op1][op2];
}

const TypeData typesys_sub(enum Type op1, enum Type op2){
#ifdef DEBUG_EXPR_TYPE
	PRINT_EXPR(op1, op2, -);
#endif
	return arith_table_lookup[op1][op2];
}

const TypeData typesys_mul(enum Type op1, enum Type op2){
#ifdef DEBUG_EXPR_TYPE
	PRINT_EXPR(op1, op2, *);
#endif
	return arith_table_lookup[op1][op2];
}

const TypeData typesys_div(enum Type op1, enum Type op2){
#ifdef DEBUG_EXPR_TYPE
	PRINT_EXPR(op1, op2, /);
#endif
	return arith_table_lookup[op1][op2];
}

const TypeData typesys_lt(enum Type op1, enum Type op2){
#ifdef DEBUG_EXPR_TYPE
	PRINT_EXPR(op1, op2, <);
#endif
	return logical_table_lookup[op1][op2];
}

const TypeData typesys_gt(enum Type op1, enum Type op2){
#ifdef DEBUG_EXPR_TYPE
	PRINT_EXPR(op1, op2, >);
#endif
	return logical_table_lookup[op1][op2];
}

const TypeData typesys_eq(enum Type op1, enum Type op2){
#ifdef DEBUG_EXPR_TYPE
	PRINT_EXPR(op1, op2, =);
#endif
	return logical_table_lookup[op1][op2];
}

const TypeData typesys_and(enum Type op1, enum Type op2){
#ifdef DEBUG_EXPR_TYPE
	PRINT_EXPR(op1, op2, &&);
#endif
	return logical_table_lookup[op1][op2];
}

const TypeData typesys_or(enum Type op1, enum Type op2){
#ifdef DEBUG_EXPR_TYPE
	PRINT_EXPR(op1, op2, ||);
#endif
	return logical_table_lookup[op1][op2];
}

const TypeData typesys_ne(enum Type op1, enum Type op2){
#ifdef DEBUG_EXPR_TYPE
	PRINT_EXPR(op1, op2, !=);
#endif
	return logical_table_lookup[op1][op2];
}

const TypeData typesys_assign(enum Type lhs, enum Type rhs){
#ifdef DEBUG_EXPR_TYPE
	printf("Expression type debug: %s = %s\n",
		type_name(res), type_name(op1->type->type));
#endif
	return assign_table_lookup[lhs][rhs];
}

#undef FUNCDEF
