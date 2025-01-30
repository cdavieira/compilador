#include "Literal.h"
#include "Function.h"
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

int func_table_add(FuncTable* table, char* name, Scope* scope, enum Type ret, int param_count){
	if(func_table_search(table, name) != NULL){
		return -1;
	}
	Function* func = func_new(name, scope, ret, param_count);
	vector_append(table->functions, func);
	return 0;
}

Function* func_table_search(FuncTable* table, char* name){
	Function* func = (Function*) vector_search(table->functions, name, (int (*)(void*, void*)) func_compare_name);
	return func;
}

void func_table_print(FuncTable* table){

}