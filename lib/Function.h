#pragma once

typedef struct Scope Scope;
enum Type;

/* Function */
typedef struct Function Function;

Function* func_new(char* name, Scope* scope, enum Type ret, int param_count);
Function* func_destroy(Function* func);
Function* func_search(Function* func, char* name);
int func_compare_name(Function* func, char* name);
