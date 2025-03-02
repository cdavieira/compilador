#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "VarTable.h"
#include "Literal.h"
#include "Vector.h"

struct VarTable {
	Vector* t;
};

VarTable* vartable_new() {
	VarTable *vt = malloc(sizeof(struct VarTable));
	vt->t = vector_new(32);
	return vt;
}

int vartable_add(
	VarTable* vt,
	char* s,
	int line,
	enum Type type,
	enum Qualifier qualifier)
{
	Variable* var = malloc(sizeof(Variable));
	if(var == NULL){
		return -1;
	}
	var->name = s;
	var->line = line;
	var->qualifier = qualifier;
	var->type = type;
	vector_append(vt->t, var);
	return 0;
}

Variable* vartable_search(VarTable* vt, const char* s) {
	return vector_search(vt->t, (void*) s, (int (*)(void*, void*)) variable_name_equals);
}

// Variable* vartable_sofisticated_search(VarTable* vt, Variable* var){
// 	return vector_search(vt->t, (void*) var, (int (*)(void*, void*)) variable_cmp);
// }

Variable* vartable_idx(VarTable* vt, size_t idx){
	return vector_get_item(vt->t, idx);
}

void vartable_print(VarTable* vt){
	printf("Variables table:\n");
	Variable* var;
	const size_t size = vector_get_size(vt->t);
	for (int i = 0; i < size; i++) {
		var = vector_get_item(vt->t, i);
		printf("Var %d -- ", i);
		printf("name: %s, ", var->name);
		printf("line: %d, ", var->line);
		printf("type: %s\n", type_name(var->type));
		printf("qualifier: %d\n", var->qualifier);
	}
}

void vartable_destroy(VarTable* vt) {
	vector_destroy(&vt->t, free);
	free(vt);
}
