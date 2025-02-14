#include "type/Type.h"
#include "type/Literal.h"

static const enum BinaryOperationTable arith_table_lookup[][4] = {
	{BINOP_EE, BINOP_EE, BINOP_EE, BINOP_EE},
	{BINOP_EE, BINOP_II, BINOP_IF, BINOP_IC},
	{BINOP_EE, BINOP_FI, BINOP_FF, BINOP_FC},
	{BINOP_EE, BINOP_CI, BINOP_CF, BINOP_CC},
};

static const enum BinaryOperationTable logical_table_lookup[][4] = {
	{BINOP_EE, BINOP_EE, BINOP_EE, BINOP_EE},
	{BINOP_EE, BINOP_II, BINOP_IF, BINOP_IC},
	{BINOP_EE, BINOP_FI, BINOP_FF, BINOP_FC},
	{BINOP_EE, BINOP_CI, BINOP_CF, BINOP_CC},
};

static const enum BinaryOperationTable assign_table_lookup[][4] = {
	{BINOP_EE, BINOP_EE, BINOP_EE, BINOP_EE},
	{BINOP_EE, BINOP_OK, BINOP_OK, BINOP_OK},
	{BINOP_EE, BINOP_OK, BINOP_OK, BINOP_OK},
	{BINOP_EE, BINOP_OK, BINOP_OK, BINOP_OK},
};


const char* type_name(enum Type type){
	switch(type){
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



static const char* operation_table_get_typename(enum BinaryOperationTable val){
	switch(val){
		case BINOP_EE:
		              return "INVALID";
		case BINOP_OK:
		              return "VALID";
		case BINOP_II:
		              return "int int";
		case BINOP_FF:
		              return "float float";
		case BINOP_CC:
		              return "char char";
		case BINOP_FI:
		              return "float int";
		case BINOP_CI:
		              return "char int";
		case BINOP_CF:
		              return "char float";
		case BINOP_IF:
		              return "int float";
		case BINOP_IC:
		              return "int char";
		case BINOP_FC:
			return "float char";
	}
	return "???";
}


enum BinaryOperationTable
binoptable_get_arithtype(
    enum Type op1,
    enum Type op2)
{
	return arith_table_lookup[op1][op2];
}

enum BinaryOperationTable
binoptable_get_logicaltype(
    enum Type op1,
    enum Type op2)
{
	return logical_table_lookup[op1][op2];
}

enum BinaryOperationTable
binoptable_get_assigntype(
    enum Type op1,
    enum Type res)
{
	return assign_table_lookup[res][op1];
}
