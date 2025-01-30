#pragma once

#include "FuncVar.h"

/* FuncTable */
typedef struct FuncTable FuncTable;

FuncTable* func_table_new(void);
void func_table_destroy(FuncTable** table);
int func_table_add(FuncTable* table, char* name, Scope* scope, enum Type ret, int par_count);
FuncVar* func_table_search(FuncTable* table, char* name);
