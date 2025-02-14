#include "type/Scope.h"
#include "type/Function.h"

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

Function* func_new(char* name, Scope* scope, enum Type ret, int param_count, int definition){
	Function* func = malloc(sizeof(Function));
	func->name = name;
	func->param_count = param_count;
	func->ret = ret;
	func->scope = scope;
	if(definition){
		func->defined = 1;
	}
	else {
		func->declared = 1;
	}
	return func;
}

Function* func_destroy(Function* func){
	free(func);
	return NULL;
}

int func_compare_name(Function* func, char* name){
	return strcmp(func->name, name) == 0;
}

int func_is_declared(Function* func){
	return func->declared == 1;
}

int func_is_defined(Function* func){
	return func->defined == 1;
}

void func_add_definition(Function* func, Scope* scope){
	if(func->defined == 1){
		return ;
	}
	func->defined = 1;
	func->scope = scope;
}

char* func_get_name(Function* func){
	return func->name;
}

enum Type func_get_return(Function* func){
	return func->ret;
}

int func_get_nparams(Function* func){
	return func->param_count;
}

Scope* func_get_scope(Function* func){
	return func->scope;
}
