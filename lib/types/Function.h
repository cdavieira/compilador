#pragma once

typedef struct Scope Scope;
enum Type;

/* Function */
typedef struct Function Function;

Function* func_new(char* name, Scope* scope, enum Type ret, int param_count, int definition);
Function* func_destroy(Function* func);
Function* func_search(Function* func, char* name);
int func_compare_name(Function* func, char* name);

int func_is_defined(Function* func);
int func_is_declared(Function* func);
void func_add_definition(Function* func, Scope* scope);
char* func_get_name(Function* func);
enum Type func_get_return(Function* func);
int func_get_nparams(Function* func);
Scope* func_get_scope(Function* func);
