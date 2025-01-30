#include "Literal.h"
#include "FuncVar.h"
#include "FuncTable.h"
#include "Vector.h"
#include <malloc.h>

struct FuncTable {
	Vector* functions;
};

FuncTable* func_table_new(void){
	FuncTable* table = malloc(sizeof(FuncTable));
	table->functions = vector_new(16);
	return table;
}

void func_table_destroy(FuncTable** table){
	vector_destroy(&(*table)->functions, (void (*)(void*)) func_destroy);
	free(*table);
	*table = NULL;
}

int func_table_add(FuncTable* table, char* name, Scope* scope, enum Type ret, int par_count){
	if(func_table_search(table, name) != NULL){
		return -1;
	}
	FuncVar* func = func_new(name, scope, ret, par_count);
	vector_append(table->functions, func);
	return 0;
}

FuncVar* func_table_search(FuncTable* table, char* name){
	FuncVar* func = (FuncVar*) vector_search(table->functions, name, (int (*)(void*, void*)) func_compare_name);
	return func;
}
