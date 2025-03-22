#pragma once

#include "types/Function.h"

/* FuncTable */
typedef struct FuncTable FuncTable;

FuncTable* func_table_new(void);
void func_table_destroy(FuncTable** table);
int func_table_add(FuncTable* table, char* name, Scope* scope, enum Type ret, int param_count, int definition);
unsigned func_table_get_size(FuncTable* table);
Function* func_table_get(FuncTable* table, int idx);
Function* func_table_search(FuncTable* table, char* name);
void func_table_print(FuncTable* table);
