#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "type/Literal.h"
#include "container/Vector.h"

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



int literal_sum(
	Literal* op1,
	Literal* op2,
	BinaryOperationData* meta)
{
	switch(binoptable_get_sumtype(op1->type, op2->type)){
		case BB:
			meta->type = TYPE_BOO;
			meta->left = CONV_NONE;
			meta->right = CONV_NONE;
			break;
		case II:
			meta->type = TYPE_INT;
			meta->left = CONV_NONE;
			meta->right = CONV_NONE;
			break;
		case BI:
			meta->type = TYPE_INT;
			meta->left = CONV_B2I;
			meta->right = CONV_NONE;
			break;
		case IB:
			meta->type = TYPE_INT;
			meta->left = CONV_NONE;
			meta->right = CONV_B2I;
			break;
		case RR:
			meta->type = TYPE_FLT;
			meta->left = CONV_NONE;
			meta->right = CONV_NONE;
			break;
		case RI:
			meta->type = TYPE_FLT;
			meta->left = CONV_NONE;
			meta->right = CONV_I2R;
			break;
		case IR:
			meta->type = TYPE_FLT;
			meta->left = CONV_I2R;
			meta->right = CONV_NONE;
			break;
		case BR:
			meta->type = TYPE_FLT;
			meta->left = CONV_B2R;
			meta->right = CONV_NONE;
			break;
		case RB:
			meta->type = TYPE_FLT;
			meta->left = CONV_NONE;
			meta->right = CONV_B2R;
			break;
		case SS:
			meta->type = TYPE_STR;
			meta->left = CONV_NONE;
			meta->right = CONV_NONE;
			break;
		case SI:
			meta->type = TYPE_STR;
			meta->left = CONV_NONE;
			meta->right = CONV_I2S;
			break;
		case IS:
			meta->type = TYPE_STR;
			meta->left = CONV_I2S;
			meta->right = CONV_NONE;
			break;
		case BS:
			meta->type = TYPE_STR;
			meta->left = CONV_B2S;
			meta->right = CONV_NONE;
			break;
		case SB:
			meta->type = TYPE_STR;
			meta->left = CONV_NONE;
			meta->right = CONV_B2S;
			break;
		case SR:
			meta->type = TYPE_STR;
			meta->left = CONV_NONE;
			meta->right = CONV_R2S;
			break;
		case RS:
			meta->type = TYPE_STR;
			meta->left = CONV_R2S;
			meta->right = CONV_NONE;
			break;
		default:
			return -1;
	}
	return 0;
}



#define FUNCDEF(OPERATION) \
	switch(binoptable_get_sumtype(op1->type, op2->type)){ \
		case II: \
			meta->type = TYPE_INT; \
			meta->left = CONV_NONE; \
			meta->right = CONV_NONE; \
			break; \
		case RR: \
			meta->type = TYPE_FLT; \
			meta->left = CONV_NONE; \
			meta->right = CONV_NONE; \
			break; \
		case RI: \
			meta->type = TYPE_FLT; \
			meta->left = CONV_NONE; \
			meta->right = CONV_I2R; \
			break ; \
		case IR: \
			meta->type = TYPE_FLT; \
			meta->left = CONV_I2R; \
			meta->right = CONV_NONE; \
			break; \
		default: \
			return -1; \
	}
int literal_minus(
	Literal* op1,
	Literal* op2,
	BinaryOperationData* meta)
{
	FUNCDEF(-)
	return 0;
}
int literal_mult(
	Literal* op1,
	Literal* op2,
	BinaryOperationData* meta)
{
	FUNCDEF(*)
	return 0;
}
int literal_div(
	Literal* op1,
	Literal* op2,
	BinaryOperationData* meta)
{
	FUNCDEF(/)
	return 0;
}
#undef FUNCDEF





#define FUNCDEF(OPERATION) \
	switch(binoptable_get_logicaltype(op1->type, op2->type)){ \
		case II: \
			meta->type = TYPE_BOO; \
			meta->left = CONV_NONE; \
			meta->right = CONV_NONE; \
			break; \
		case IR: \
			meta->type = TYPE_BOO; \
			meta->left = CONV_I2R; \
			meta->right = CONV_NONE; \
			break; \
		case RI: \
			meta->type = TYPE_BOO; \
			meta->left = CONV_NONE; \
			meta->right = CONV_I2R; \
			break; \
		case RR: \
			meta->type = TYPE_BOO; \
			meta->left = CONV_NONE; \
			meta->right = CONV_NONE; \
			break; \
		case SS: \
			meta->type = TYPE_BOO; \
			meta->left = CONV_NONE; \
			meta->right = CONV_NONE; \
			break; \
		default: \
			return -1; \
	}

int literal_lt(
	Literal* op1,
	Literal* op2,
	BinaryOperationData* meta)
{
	FUNCDEF(<)
	return 0;
}
int literal_eq(
	Literal* op1,
	Literal* op2,
	BinaryOperationData* meta)
{
	FUNCDEF(==)
	return 0;
}
#undef FUNCDEF


int literal_assign(
	Literal* op1,
	UnaryOperationData* meta)
{
	switch(binoptable_get_assigntype(op1->type, meta->type)){
		case II:
			/* meta->value.b = op1->value.b; */
			meta->type = TYPE_INT;
			meta->right = CONV_NONE;
			break;
		case RI:
			/* res->value.f = (float) op1->value.i; */
			meta->type = TYPE_FLT;
			meta->right = CONV_I2R;
			break;
		case RR:
			/* res->value.f = op1->value.f; */
			meta->type = TYPE_FLT;
			meta->right = CONV_NONE;
			break;
		case BB:
			/* res->value.b = op1->value.b; */
			meta->type = TYPE_BOO;
			meta->right = CONV_NONE;
			break;
		case SS:
			/* res->value.s = op1->value.s; */
			meta->type = TYPE_STR;
			meta->right = CONV_NONE;
			break;
		default:
			return -1;
	}
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
