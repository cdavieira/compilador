#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Literal.h"
#include "vector.h"

/* each member should use only 4 bits.
 * therefore, this enum can identify up to 16 different types.
 * in reality, only 15 different types are useable (0x0 is reserved).
 * this enum should match/be a copy of the 'TokenType' enum. */
enum OperandType {
	E = 0x0, //error (reserved)
	I = 0x1,
	R = 0x2,
	B = 0x3,
	S = 0x4,
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
	RR = OPCODE(R, R),
	BB = OPCODE(B, B),
	SS = OPCODE(S, S),
	RI = OPCODE(R, I),
	BI = OPCODE(B, I),
	SI = OPCODE(S, I),
	BR = OPCODE(B, R),
	BS = OPCODE(B, S),
	SR = OPCODE(S, R),
	IR = OPCODE(I, R),
	IB = OPCODE(I, B),
	IS = OPCODE(I, S),
	RB = OPCODE(R, B),
	SB = OPCODE(S, B),
	RS = OPCODE(R, S),
#undef OPCODE
};


/* '+' Table
		+ i r b s
		i i r i s
		r r r r s
		b i r b s
		s s s s s */
static const enum BinaryOperationTable SumTableLookUp[][4] = {
	{II, IR, IB, IS},
	{RI, RR, RB, RS},
	{BI, BR, BB, BS},
	{SI, SR, SB, SS},
};

/* '-', '*', '/' Table
 * e stands for error
		  i r b s
		i i r e e
		r r r e e
		b e e e e
		s e e e e */
static const enum BinaryOperationTable MinusTableLookUp[][4] = {
	{II, IR, EE, EE},
	{RI, RR, EE, EE},
	{EE, EE, EE, EE},
	{EE, EE, EE, EE},
};

/* '<', '=' Table
 * e stands for error
		  i r b s
		i 1 1 e e
		r 1 1 e e
		b e e e e
		s e e e 1 */
static const enum BinaryOperationTable LogicalTableLookUp[][4] = {
	{II, IR, EE, EE},
	{RI, RR, EE, EE},
	{EE, EE, EE, EE},
	{EE, EE, EE, SS},
};

/* ':=' Table
 * e stands for error
		  i r b s
		i 1 e e e
		r 1 1 e e
		b e e 1 e
		s e e e 1 */
static const enum BinaryOperationTable AssignTableLookUp[][4] = {
	{OK, EE, EE, EE},
	{OK, OK, EE, EE},
	{EE, EE, OK, EE},
	{EE, EE, EE, OK},
};

static Vector* strings;

static char* int_to_str(int num, const int base);
static char* flt_to_str(float num, const int base);
static char* alloc_string(unsigned size);
static char* dup_string(const char* src);


void literal_init(void){
	strings = vector_new(32);
}

void literal_deinit(void){
	vector_destroy(&strings, free);
	strings = NULL;
}

int literal_str_to_int(Literal* l, const char* text){
	l->value.i = atoi(text);
	l->type = TYPE_INT;
	return 0;
}

int literal_str_to_flt(Literal* l, const char* text){
	l->value.f = atof(text);
	l->type = TYPE_FLT;
	return 0;
}

int literal_str_to_boo(Literal* l, int v){
	l->value.b = v > 0 ? 1 : 0;
	l->type = TYPE_BOO;
	return 0;
}

int literal_str(Literal* l, const char* text){
	char* str = dup_string(text);
	l->value.s = str;
	l->type = TYPE_STR;
	return 0;
}

int literal_to_str(Literal* l, char** str){
	switch(l->type){
		case TYPE_INT:
			*str = int_to_str(l->value.i, 10);
			break;
		case TYPE_FLT:
			*str = flt_to_str(l->value.f, 10);
			break;
		case TYPE_BOO:
			*str = alloc_string(2);
			strcat(*str, l->value.b == 0? "0" : "1");
			break;
		case TYPE_STR:
			*str = l->value.s;
			break;
		default:
			*str = NULL;
			return -1;
	}
	return 0;
}

const char* literal_get_type_name(enum Type type){
	switch(type){
		case TYPE_INT:
			return "int";
		case TYPE_FLT:
			return "float";
		case TYPE_BOO:
			return "boolean";
		case TYPE_STR:
			return "string";
		default:
			return "unknown type";
	}
}

int literal_sum(
	Literal* op1,
	Literal* op2,
	Literal* res)
{
	// union Value temp;
	// unsigned size;
	switch(SumTableLookUp[op1->type][op2->type]){
		case II:
			res->type = TYPE_INT;
			/*res->value.i = op1->value.i + op2->value.i;*/
			break;
		case RR:
			res->type = TYPE_FLT;
			/*res->value.f = op1->value.f + op2->value.f;*/
			break;
		case BB:
			res->type = TYPE_BOO;
			/*res->value.b = op1->value.b + op2->value.b;*/
			break;
		case SS:
			res->type = TYPE_STR;
			/*size = strlen(op1->value.s) + strlen(op2->value.s) + 1;
			res->value.s = alloc_string(size);
			strcat(res->value.s, op1->value.s);
			strcat(res->value.s, op2->value.s);*/
			break;
		case RI:
			res->type = TYPE_FLT;
			/*res->value.f = op1->value.f + (float)op2->value.i;*/
			break ;
		case IR:
			res->type = TYPE_FLT;
			/*res->value.f = (float)op1->value.i + op2->value.f;*/
			break;
		case BI:
			res->type = TYPE_INT;
			/*res->value.i = (int)op1->value.b + op2->value.i;*/
			break;
		case IB:
			res->type = TYPE_INT;
			/*res->value.i = op1->value.i + (int)op2->value.b;*/
		case BR:
			res->type = TYPE_FLT;
			/*res->value.i = (float) op1->value.b + op2->value.f;*/
			break;
		case RB:
			res->type = TYPE_FLT;
			/*res->value.i = op1->value.f + (float) op2->value.b;*/
			break;
		case SI:
			res->type = TYPE_STR;
			/*temp.s = int_to_str(op2->value.i, 10);
			size = strlen(op1->value.s) + strlen(temp.s) + 1;
			res->value.s = alloc_string(size);
			strcat(res->value.s, op1->value.s);
			strcat(res->value.s, temp.s);
			free(temp.s);*/
			break;
		case IS:
			res->type = TYPE_STR;
			/*temp.s = int_to_str(op1->value.i, 10);
			size = strlen(temp.s) + strlen(op2->value.s) + 1;
			res->value.s = alloc_string(size);
			strcat(res->value.s, temp.s);
			strcat(res->value.s, op2->value.s);
			free(temp.s);*/
			break;
		case BS:
			res->type = TYPE_STR;
			/*temp.s = strdup(op1->value.b == 0 ? "0" : "1");
			size = strlen(op2->value.s) + 2;
			res->value.s = alloc_string(size);
			strcat(res->value.s, temp.s);
			strcat(res->value.s, op2->value.s);
			free(temp.s);*/
			break;
		case SB:
			res->type = TYPE_STR;
			/*temp.s = strdup(op2->value.b == 0 ? "0" : "1");
			size = strlen(op1->value.s) + 2;
			res->value.s = alloc_string(size);
			strcat(res->value.s, op1->value.s);
			strcat(res->value.s, temp.s);
			free(temp.s);*/
			break;
		case SR:
			res->type = TYPE_STR;
			/*temp.s = flt_to_str(op2->value.f, 10);
			size = strlen(op1->value.s) + strlen(temp.s) + 1;
			res->value.s = alloc_string(size);
			strcat(res->value.s, op1->value.s);
			strcat(res->value.s, temp.s);
			free(temp.s);*/
			break;
		case RS:
			res->type = TYPE_STR;
			/*temp.s = flt_to_str(op1->value.f, 10);
			size = strlen(op2->value.s) + strlen(temp.s) + 1;
			res->value.s = alloc_string(size);
			strcat(res->value.s, temp.s);
			strcat(res->value.s, op2->value.s);
			free(temp.s);*/
			break;
		default:
			return -1;
	}
	return 0;
}

#define FUNCDEF(NAME, OPERATION) \
int literal_ ## NAME ( \
	Literal* op1, \
	Literal* op2, \
	Literal* res) \
{ \
	switch(MinusTableLookUp[op1->type][op2->type]){ \
		case II: \
			res->type = TYPE_INT; \
			/*res->value.i = op1->value.i OPERATION op2->value.i;*/ \
			break; \
		case RR: \
			res->type = TYPE_FLT; \
			/*res->value.f = op1->value.f OPERATION op2->value.f;*/ \
			break; \
		case RI: \
			res->type = TYPE_FLT; \
			/*res->value.f = op1->value.f OPERATION (float)op2->value.i;*/ \
			break ; \
		case IR: \
			res->type = TYPE_FLT; \
			/*res->value.f = (float)op1->value.i OPERATION op2->value.f;*/ \
			break; \
		default: \
			return -1; \
	} \
	return 0; \
}
FUNCDEF(minus, -)
FUNCDEF(mult, *)
FUNCDEF(div, /)
#undef FUNCDEF

#define FUNCDEF(NAME, OPERATION) \
int literal_ ## NAME ( \
	Literal* op1, \
	Literal* op2, \
	Literal* res) \
{ \
	switch(LogicalTableLookUp[op1->type][op2->type]){ \
		case II: \
			res->type = TYPE_BOO; \
			/*res->value.b = op1->value.i OPERATION op2->value.i ? 1 : 0;*/ \
			break; \
		case IR: \
			res->type = TYPE_BOO; \
			/*res->value.b = (float) op1->value.i OPERATION op2->value.f ? 1 : 0;*/ \
			break; \
		case RI: \
			res->type = TYPE_BOO; \
			/*res->value.b = op1->value.f OPERATION (float) op2->value.i ? 1 : 0;*/ \
			break; \
		case RR: \
			res->type = TYPE_BOO; \
			/*res->value.b = op1->value.f OPERATION op2->value.f ? 1 : 0;*/ \
			break; \
		case SS: \
			res->type = TYPE_BOO; \
			/*res->value.b = strcmp(op1->value.s, op2->value.s) == 0 ? 1 : 0;*/ \
			break; \
		default: \
			return -1; \
	} \
	return 0; \
}
FUNCDEF(lt, <)
FUNCDEF(eq, ==)
#undef FUNCDEF

int literal_assign(
	Literal* op1,
	Literal* res)
{
	switch(AssignTableLookUp[res->type][op1->type]){
		case II:
			res->value.b = op1->value.b;
			break;
		case RI:
			res->value.f = (float) op1->value.i;
			break;
		case RR:
			res->value.f = op1->value.f;
			break;
		case BB:
			res->value.b = op1->value.b;
			break;
		case SS:
			res->value.s = op1->value.s;
			break;
		default:
			return -1;
	}
	return 0;
}

int operator_str(Operator* l, const char* text, enum OperatorType type){
	char* str = dup_string(text);
	l->name = str;
	l->type = type;
	return 0;
}

int variable_str(Variable* var, char* name){
	char* str = dup_string(name);
	var->name = str;
	return 0;
}

static char* int_to_str(int num, const int base){
	char* str;
	int cpy = num;
	//+1 because of '\0'
	//+1 because of the sign ('+' or '-')
	unsigned dec = 2;

	while(cpy != 0){
		cpy /= base;
		dec++;
	}

	//see why dec can't be 0?
	//also, this string will be free'd at the callee's site, so don't worry
	str = calloc(dec, sizeof(char));
	sprintf(str, "%d", num);
	return str;
}

static char* flt_to_str(float num, const int base){
	char* str;
	float cpy = num;
	//+4 : "-0.'\0'"
	unsigned dec = 4;

	while(cpy != 0.0){
		cpy /= base;
		dec++;
	}

	//this string will be free'd at the callee's site, so don't worry
	str = calloc(dec, sizeof(char));
	sprintf(str, "%f", num);
	return str;
}

static char* alloc_string(unsigned size){
	char* str = calloc(size, sizeof(char));
	vector_append(strings, str);
	return str;
}

static char* dup_string(const char* src){
	char* str = strdup(src);
	vector_append(strings, str);
	return str;
}
