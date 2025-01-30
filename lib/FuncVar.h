#pragma once

typedef struct Scope Scope;
enum Type;

/* FuncVar */
typedef struct FuncVar FuncVar;

FuncVar* func_new(char* name, Scope* scope, enum Type ret, int par_count);
FuncVar* func_destroy(FuncVar* func);
FuncVar* func_search(FuncVar* func, char* name);
int func_compare_name(FuncVar* func, char* name);
