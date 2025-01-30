#include "Scope.h"
#include "FuncVar.h"

#include <string.h>
#include <malloc.h>

struct FuncVar {
	char* name;
	int par_count;
	Scope* scope;
	enum Type ret;
};

FuncVar* func_new(char* name, Scope* scope, enum Type ret, int par_count){
	FuncVar* func = malloc(sizeof(FuncVar));		
	func->name = name;
	func->par_count = par_count;
	func->ret = ret;
	func->scope = scope;
	return func;
}

FuncVar* func_destroy(FuncVar* func){
	free(func);
	return NULL;
}

int func_compare_name(FuncVar* func, char* name){
	return strcmp(func->name, name) == 0;
}
