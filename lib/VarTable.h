#pragma once

#include "Literal.h"

typedef struct VarTable VarTable;

VarTable* vartable_new();

int vartable_add(
	VarTable* vt,
	char* s,
	int line,
	enum Type type);

Variable* vartable_search(VarTable* vt, const char* name);

void vartable_print(VarTable* vt);

void vartable_destroy(VarTable* vt);

