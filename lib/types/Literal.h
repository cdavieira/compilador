#pragma once
#include "types/Type.h"

typedef union Value {
	int i;
	float f;
	char c;
	char* s;
} Value ;

typedef struct Literal {
	enum Type type;
	Value value;
} Literal;
