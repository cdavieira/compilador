#include "type/Literal.h"

const char* literal_get_typename(const Literal* td){
	return type_name(td->type);
}

#define FUNCDEF(OPERATION) \
	switch(binoptable_get_arithtype(op1->type, op2->type)){ \
		case II: \
			res->type = TYPE_INT; \
			res->left = CONV_NONE; \
			res->right = CONV_NONE; \
			break; \
		case IC: \
			res->type = TYPE_INT; \
			res->left = CONV_NONE; \
			res->right = CONV_C2I; \
			break; \
		case IF: \
			res->type = TYPE_FLT; \
			res->left = CONV_I2F; \
			res->right = CONV_NONE; \
			break; \
		case FF: \
			res->type = TYPE_FLT; \
			res->left = CONV_NONE; \
			res->right = CONV_NONE; \
			break; \
		case FI: \
			res->type = TYPE_FLT; \
			res->left = CONV_NONE; \
			res->right = CONV_I2F; \
			break; \
		case FC: \
			res->type = TYPE_FLT; \
			res->left = CONV_NONE; \
			res->right = CONV_C2F; \
			break; \
		case CC: \
			res->type = TYPE_INT; \
			res->left = CONV_NONE; \
			res->right = CONV_NONE; \
			break; \
		case CI: \
			res->type = TYPE_INT; \
			res->left = CONV_C2I; \
			res->right = CONV_NONE; \
			break; \
		case CF: \
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
		literal_get_typename(l1), \
		#op,\
		literal_get_typename(l2), \
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
		case II: \
			res->type = TYPE_INT; \
			res->left = CONV_NONE; \
			res->right = CONV_NONE; \
			break ; \
		case IC: \
			res->type = TYPE_INT; \
			res->left = CONV_NONE; \
			res->right = CONV_C2I; \
			break ; \
		case IF: \
			res->type = TYPE_INT; \
			res->left = CONV_I2F; \
			res->right = CONV_NONE; \
			break ; \
		case FF: \
			res->type = TYPE_INT; \
			res->left = CONV_NONE; \
			res->right = CONV_NONE; \
			break ; \
		case FI: \
			res->type = TYPE_INT; \
			res->left = CONV_NONE; \
			res->right = CONV_I2F; \
			break ; \
		case FC: \
			res->type = TYPE_INT; \
			res->left = CONV_NONE; \
			res->right = CONV_C2F; \
			break ; \
		case CC: \
			res->type = TYPE_INT; \
			res->left = CONV_NONE; \
			res->right = CONV_NONE; \
			break ; \
		case CI: \
			res->type = TYPE_INT; \
			res->left = CONV_C2I; \
			res->right = CONV_NONE; \
			break ; \
		case CF: \
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
		literal_get_typename(l1), \
		#op,\
		literal_get_typename(l2), \
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
		literal_get_typename(res), literal_get_typename(op1),
		operation_table_get_typename(binoptable_get_assigntype(op1->type, res->type)));
#endif
	switch(binoptable_get_assigntype(op1->type, res->type)){
		case II:
			res->type = TYPE_INT;
			res->right = CONV_NONE;
			// res->value.i = op1->value.i;
			break;
		case IC:
			res->type = TYPE_INT;
			res->right = CONV_C2I;
			// res->value.i = op1->value.c;
			break;
		case IF:
			res->type = TYPE_INT;
			res->right = CONV_F2I;
			// res->value.f = ((float)op1->value.f);
			break;
		case FF:
			res->type = TYPE_FLT;
			res->right = CONV_NONE;
			// res->value.f = op1->value.f;
			break;
		case FI:
			res->type = TYPE_FLT;
			res->right = CONV_I2F;
			// res->value.f = ((float)op1->value.i);
			break;
		case FC:
			res->type = TYPE_FLT;
			res->right = CONV_C2F;
			// res->value.f = ((float)op1->value.c);
			break;
		case CC:
			res->type = TYPE_INT;
			res->right = CONV_NONE;
			// res->value.c = op1->value.c;
			break;
		case CI:
			res->type = TYPE_CHAR;
			res->right = CONV_I2C;
			// res->value.c = op1->value.i;
			break;
		case CF:
			res->type = TYPE_CHAR;
			res->right = CONV_F2C;
			// res->value.c = ((char)op1->value.f);
			break;
		default:
			return -1;
	}
	return 0;
}
