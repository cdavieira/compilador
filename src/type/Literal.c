#include "type/Literal.h"

#define FUNCDEF(OPERATION) \
	switch(binoptable_get_arithtype(op1->type, op2->type)){ \
		case BINOP_II: \
			res->type = TYPE_INT; \
			res->left = CONV_NONE; \
			res->right = CONV_NONE; \
			break; \
		case BINOP_IC: \
			res->type = TYPE_INT; \
			res->left = CONV_NONE; \
			res->right = CONV_C2I; \
			break; \
		case BINOP_IF: \
			res->type = TYPE_FLT; \
			res->left = CONV_I2F; \
			res->right = CONV_NONE; \
			break; \
		case BINOP_FF: \
			res->type = TYPE_FLT; \
			res->left = CONV_NONE; \
			res->right = CONV_NONE; \
			break; \
		case BINOP_FI: \
			res->type = TYPE_FLT; \
			res->left = CONV_NONE; \
			res->right = CONV_I2F; \
			break; \
		case BINOP_FC: \
			res->type = TYPE_FLT; \
			res->left = CONV_NONE; \
			res->right = CONV_C2F; \
			break; \
		case BINOP_CC: \
			res->type = TYPE_INT; \
			res->left = CONV_NONE; \
			res->right = CONV_NONE; \
			break; \
		case BINOP_CI: \
			res->type = TYPE_INT; \
			res->left = CONV_C2I; \
			res->right = CONV_NONE; \
			break; \
		case BINOP_CF: \
			res->type = TYPE_FLT; \
			res->left = CONV_C2F; \
			res->right = CONV_NONE; \
			break; \
		default: \
			return -1; \
	} \
	return 0;

#define PRINT_EXPR(l1, l2, op) \
	printf("Expression type debug: %s %s %s = %s\n", \
		type_name(l1->type), \
		#op,\
		type_name(l2->type), \
		operation_table_get_typename(binoptable_get_arithtype(op1->type, op2->type)));

int literal_sum(Literal* op1, Literal* op2, BinaryOperationData* res){
#ifdef DEBUG_EXPR_TYPE
	PRINT_EXPR(op1, op2, +);
#endif
FUNCDEF(+)
}

int literal_sub(Literal* op1, Literal* op2, BinaryOperationData* res){
#ifdef DEBUG_EXPR_TYPE
	PRINT_EXPR(op1, op2, -);
#endif
FUNCDEF(-)
}

int literal_mul(Literal* op1, Literal* op2, BinaryOperationData* res){
#ifdef DEBUG_EXPR_TYPE
	PRINT_EXPR(op1, op2, *);
#endif
FUNCDEF(*)
}

int literal_div(Literal* op1, Literal* op2, BinaryOperationData* res){
#ifdef DEBUG_EXPR_TYPE
	PRINT_EXPR(op1, op2, /);
#endif
FUNCDEF(/)
}
#undef PRINT_EXPR
#undef FUNCDEF



#define FUNCDEF(OPERATION) \
	switch(binoptable_get_logicaltype(op1->type, op2->type)){ \
		case BINOP_II: \
			res->type = TYPE_INT; \
			res->left = CONV_NONE; \
			res->right = CONV_NONE; \
			break ; \
		case BINOP_IC: \
			res->type = TYPE_INT; \
			res->left = CONV_NONE; \
			res->right = CONV_C2I; \
			break ; \
		case BINOP_IF: \
			res->type = TYPE_INT; \
			res->left = CONV_I2F; \
			res->right = CONV_NONE; \
			break ; \
		case BINOP_FF: \
			res->type = TYPE_INT; \
			res->left = CONV_NONE; \
			res->right = CONV_NONE; \
			break ; \
		case BINOP_FI: \
			res->type = TYPE_INT; \
			res->left = CONV_NONE; \
			res->right = CONV_I2F; \
			break ; \
		case BINOP_FC: \
			res->type = TYPE_INT; \
			res->left = CONV_NONE; \
			res->right = CONV_C2F; \
			break ; \
		case BINOP_CC: \
			res->type = TYPE_INT; \
			res->left = CONV_NONE; \
			res->right = CONV_NONE; \
			break ; \
		case BINOP_CI: \
			res->type = TYPE_INT; \
			res->left = CONV_C2I; \
			res->right = CONV_NONE; \
			break ; \
		case BINOP_CF: \
			res->type = TYPE_INT; \
			res->left = CONV_C2F; \
			res->right = CONV_NONE; \
			break ; \
		default: \
			return -1; \
	} \
	return 0;

#define PRINT_EXPR(l1, l2, op) \
	printf("Expression type debug: %s %s %s = %s\n", \
		type_name(l1->type), \
		#op,\
		type_name(l2->type), \
		operation_table_get_typename(binoptable_get_logicaltype(op1->type, op2->type)));

int literal_lt(Literal* op1, Literal* op2, BinaryOperationData* res){
#ifdef DEBUG_EXPR_TYPE
	PRINT_EXPR(op1, op2, <);
#endif
FUNCDEF(<)
}

int literal_gt(Literal* op1, Literal* op2, BinaryOperationData* res){
#ifdef DEBUG_EXPR_TYPE
	PRINT_EXPR(op1, op2, >);
#endif
FUNCDEF(>)
}

int literal_eq(Literal* op1, Literal* op2, BinaryOperationData* res){
#ifdef DEBUG_EXPR_TYPE
	PRINT_EXPR(op1, op2, =);
#endif
FUNCDEF(==)
}

int literal_and(Literal* op1, Literal* op2, BinaryOperationData* res){
#ifdef DEBUG_EXPR_TYPE
	PRINT_EXPR(op1, op2, &&);
#endif
FUNCDEF(&&)
}

int literal_or(Literal* op1, Literal* op2, BinaryOperationData* res){
#ifdef DEBUG_EXPR_TYPE
	PRINT_EXPR(op1, op2, ||);
#endif
FUNCDEF(||)
}

int literal_ne(Literal* op1, Literal* op2, BinaryOperationData* res){
#ifdef DEBUG_EXPR_TYPE
	PRINT_EXPR(op1, op2, !=);
#endif
FUNCDEF(!=)
}
#undef PRINT_EXPR
#undef FUNCDEF



int literal_assign (
	Literal* op1,
	UnaryOperationData* res)
{
#ifdef DEBUG_EXPR_TYPE
	printf("Expression type debug: %s = %s (%s)\n",
		type_name(res), type_name(op1->type->type),
		operation_table_get_typename(binoptable_get_assigntype(op1->type, res->type)));
#endif
	switch(binoptable_get_assigntype(op1->type, res->type)){
		case BINOP_II:
			res->type = TYPE_INT;
			res->right = CONV_NONE;
			// res->value.i = op1->value.i;
			break;
		case BINOP_IC:
			res->type = TYPE_INT;
			res->right = CONV_C2I;
			// res->value.i = op1->value.c;
			break;
		case BINOP_IF:
			res->type = TYPE_INT;
			res->right = CONV_F2I;
			// res->value.f = ((float)op1->value.f);
			break;
		case BINOP_FF:
			res->type = TYPE_FLT;
			res->right = CONV_NONE;
			// res->value.f = op1->value.f;
			break;
		case BINOP_FI:
			res->type = TYPE_FLT;
			res->right = CONV_I2F;
			// res->value.f = ((float)op1->value.i);
			break;
		case BINOP_FC:
			res->type = TYPE_FLT;
			res->right = CONV_C2F;
			// res->value.f = ((float)op1->value.c);
			break;
		case BINOP_CC:
			res->type = TYPE_INT;
			res->right = CONV_NONE;
			// res->value.c = op1->value.c;
			break;
		case BINOP_CI:
			res->type = TYPE_CHAR;
			res->right = CONV_I2C;
			// res->value.c = op1->value.i;
			break;
		case BINOP_CF:
			res->type = TYPE_CHAR;
			res->right = CONV_F2C;
			// res->value.c = ((char)op1->value.f);
			break;
		default:
			return -1;
	}
	return 0;
}
