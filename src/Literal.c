#include "Literal.h"

#include "stdio.h"
#include <string.h>

/**/

/* each member should use only 4 bits.
 * therefore, this enum can identify up to 16 different types.
 * in reality, only 15 different types are useable (0x0 is reserved).
 * this enum should match/be a copy of the 'Type' enum. */
enum Operand {
	E = 0x0, //error (reserved)
	I = 0x1,
	C = 0x2,
	F = 0x3,
};

/* each member should use only 8 bits.
 * therefore, this enum can encode up to 256 combinations of sums.
 * in reality, only 225 are available, because all combinations which use 0x0
 * are reserved. */
enum BinaryOperationTable {
	EE = 0,
	OK = 0x01,
#define OPCODE(N,M) ((N << 4) | M)
	II = OPCODE(I, I),
	FF = OPCODE(F, F),
	CC = OPCODE(C, C),
	FI = OPCODE(F, I),
	CI = OPCODE(C, I),
	CF = OPCODE(C, F),
	IF = OPCODE(I, F),
	IC = OPCODE(I, C),
	FC = OPCODE(F, C),
#undef OPCODE
};

static const enum BinaryOperationTable arith_table_lookup[][4] = {
	{EE, EE, EE, EE},
	{EE, II, IC, IF},
	{EE, CI, CC, CF},
	{EE, FI, FC, FF},
};

static const enum BinaryOperationTable logical_table_lookup[][4] = {
	{EE, EE, EE, EE},
	{EE, II, IC, IF},
	{EE, CI, CC, CF},
	{EE, FI, FC, FF},
};

static const enum BinaryOperationTable assign_table_lookup[][4] = {
	{EE, EE, EE, EE},
	{EE, OK, OK, OK},
	{EE, OK, OK, OK},
	{EE, OK, OK, OK},
};


const char* literal_get_typename(const Literal* td){
	switch(td->type){
		case TYPE_INT:
			return "int";
		case TYPE_FLT:
			return "float";
		case TYPE_STR:
			return "char*";
		case TYPE_CHAR:
			return "char";
		case TYPE_VOID:
			return "void";
	}
	return "unknown typename";
}



/**/

#define FUNCDEF(NAME, OPERATION) \
int literal_ ## NAME ( \
	Literal* op1, \
	Literal* op2, \
	Literal* res) \
{ \
	switch(arith_table_lookup[op1->type][op2->type]){ \
		case II: \
			res->type = TYPE_INT; \
			/* res->value.i = op1->value.i OPERATION op2->value.i; */ \
			break; \
		case IC: \
			res->type = TYPE_INT; \
			/* res->value.i = op1->value.i OPERATION op2->value.c; */ \
			break; \
		case IF: \
			res->type = TYPE_INT; \
			/* res->value.f = ((float)op1->value.i) OPERATION op2->value.f; */ \
			break; \
		case FF: \
			res->type = TYPE_FLT; \
			/* res->value.f = op1->value.f OPERATION op2->value.f; */ \
			break; \
		case FI: \
			res->type = TYPE_FLT; \
			/* res->value.f = op1->value.f OPERATION ((float)op2->value.i); */ \
			break; \
		case FC: \
			res->type = TYPE_FLT; \
			/* res->value.f = op1->value.f OPERATION ((float)op2->value.c); */ \
			break; \
		case CC: \
			res->type = TYPE_INT; \
			/* res->value.c = op1->value.c OPERATION op2->value.c; */ \
			break; \
		case CI: \
			res->type = TYPE_INT; \
			/* res->value.c = op1->value.c OPERATION op2->value.i; */ \
			break; \
		case CF: \
			res->type = TYPE_FLT; \
			/* res->value.c = op1->value.c OPERATION ((char)op2->value.f); */ \
			break; \
		default: \
			return -1; \
	} \
	return 0; \
}
FUNCDEF(sum, +)
FUNCDEF(sub, -)
FUNCDEF(mul, *)
FUNCDEF(div, /)
#undef FUNCDEF


#define FUNCDEF(NAME, OPERATION) \
int literal_ ## NAME ( \
	Literal* op1, \
	Literal* op2, \
	Literal* res) \
{ \
	\
	switch(logical_table_lookup[op1->type][op2->type]){ \
		case II: \
		case IC: \
		case IF: \
		case FF: \
		case FI: \
		case FC: \
		case CC: \
		case CI: \
		case CF: \
			res->type = TYPE_INT; \
			break ; \
		default: \
			return -1; \
	} \
	return 0; \
}
FUNCDEF(lt, <)
FUNCDEF(gt, >)
FUNCDEF(eq, ==)
FUNCDEF(and, &&)
FUNCDEF(or, ||)
FUNCDEF(ne, !=)
#undef FUNCDEF


int literal_assign (
	Literal* op1,
	Literal* res)
{
	switch(assign_table_lookup[res->type][op1->type]){
		case II:
			res->value.i = op1->value.i;
			break;
		case IC:
			res->value.i = op1->value.c;
			break;
		case IF:
			res->value.f = ((float)op1->value.f);
			break;
		case FF:
			res->value.f = op1->value.f;
			break;
		case FI:
			res->value.f = ((float)op1->value.i);
			break;
		case FC:
			res->value.f = ((float)op1->value.c);
			break;
		case CC:
			res->value.c = op1->value.c;
			break;
		case CI:
			res->value.c = op1->value.i;
			break;
		case CF:
			res->value.c = ((char)op1->value.f);
			break;
		default:
			return -1;
	}
	return 0;
}


/**/

int variable_name_equals(Variable* var, const char* name){
	return strcmp(var->name, name) == 0;
}

void variable_print(Variable* var){
	printf("name='%s', line=%d, type=%d\n", var->name, var->line, var->token.type);
}
