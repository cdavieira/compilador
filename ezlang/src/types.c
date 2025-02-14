#include "types.h"

/* '+' Table
		+ i r b s
		i i r i s
		r r r r s
		b i r b s
		s s s s s */
//WARNING: the indexes of this table should be of type 'enum Type'
static const enum BinaryOperationTable SumTableLookUp[][5] = {
	{II, IR, IB, IS, EE},
	{RI, RR, RB, RS, EE},
	{BI, BR, BB, BS, EE},
	{SI, SR, SB, SS, EE},
	{EE, EE, EE, EE, EE},
};

/* '-', '*', '/' Table
 * e stands for error
		  i r b s
		i i r e e
		r r r e e
		b e e e e
		s e e e e */
//WARNING: the indexes of this table should be of type 'enum Type'
static const enum BinaryOperationTable MinusTableLookUp[][5] = {
	{II, IR, EE, EE, EE},
	{RI, RR, EE, EE, EE},
	{EE, EE, EE, EE, EE},
	{EE, EE, EE, EE, EE},
	{EE, EE, EE, EE, EE},
};

/* '<', '=' Table
 * e stands for error
		  i r b s
		i 1 1 e e
		r 1 1 e e
		b e e e e
		s e e e 1 */
//WARNING: the indexes of this table should be of type 'enum Type'
static const enum BinaryOperationTable LogicalTableLookUp[][5] = {
	{II, IR, EE, EE, EE},
	{RI, RR, EE, EE, EE},
	{EE, EE, EE, EE, EE},
	{EE, EE, EE, SS, EE},
	{EE, EE, EE, EE, EE},
};

/* ':=' Table
 * e stands for error
		  i r b s
		i 1 e e e
		r 1 1 e e
		b e e 1 e
		s e e e 1 */
//WARNING: the indexes of this table should be of type 'enum Type'
static const enum BinaryOperationTable AssignTableLookUp[][5] = {
	{OK, EE, EE, EE, EE},
	{OK, OK, EE, EE, EE},
	{EE, EE, OK, EE, EE},
	{EE, EE, EE, OK, EE},
	{EE, EE, EE, EE, EE},
};

const char* type_name(enum Type type){
	switch(type){
		case TYPE_INT:
			return "int";
		case TYPE_FLT:
			return "float";
		case TYPE_BOO:
			return "boolean";
		case TYPE_STR:
			return "string";
		case TYPE_NOTHING:
			return "nothing";
		default:
			return "unknown type";
	}
}

enum BinaryOperationTable
binoptable_get_sumtype(
    Type op1,
    Type op2)
{
  return SumTableLookUp[op1][op2];
}

enum BinaryOperationTable
binoptable_get_minustype(
    Type op1,
    Type op2)
{
  return MinusTableLookUp[op1][op2];
}

enum BinaryOperationTable
binoptable_get_logicaltype(
    Type op1,
    Type op2)
{
  return LogicalTableLookUp[op1][op2];
}

enum BinaryOperationTable
binoptable_get_assigntype(
    Type op1,
    Type res)
{
  return AssignTableLookUp[res][op1];
}
