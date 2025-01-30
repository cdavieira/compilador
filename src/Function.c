#include "Scope.h"
#include "Function.h"

#include <string.h>
#include <malloc.h>

struct Function {
	int declared;
	int defined;
	char* name;
	int param_count;
	Scope* scope;
	enum Type ret;
};

Function* func_new(char* name, Scope* scope, enum Type ret, int param_count){
	Function* func = malloc(sizeof(Function));		
	func->name = name;
	func->param_count = param_count;
	func->ret = ret;
	func->scope = scope;
	return func;
}

Function* func_destroy(Function* func){
	free(func);
	return NULL;
}

int func_compare_name(Function* func, char* name){
	return strcmp(func->name, name) == 0;
}
