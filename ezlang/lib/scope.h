#pragma once

#include "Literal.h"
#include "tables.h"

/* Scope */
typedef struct Scope Scope;

Scope* scope_new(int id, int parent, int level);
Scope* scope_destroy(Scope* scope);

int scope_get_id(Scope* scope);
int scope_get_parent(Scope* scope);
int scope_get_level(Scope* scope);
VarTable* scope_get_vartable(Scope* scope);

int scope_add(Scope* scope, char* name, int line, enum Type type);
int scope_search(Scope* scope, Variable* entry);
//also considers the depth of this scope
int scope_search_by_name(Scope* scope, char* name);




/* Scope Manager */
typedef struct ScopeManager ScopeManager;

ScopeManager* scope_manager_new(void);
void scope_manager_destroy(ScopeManager** manager);
void scope_manager_print(ScopeManager* manager);

void scope_manager_enter(ScopeManager* manager);
void scope_manager_exit(ScopeManager* manager);
Scope* scope_manager_get_current_scope(ScopeManager* manager);

Scope* scope_manager_search(ScopeManager* manager, Variable* data);
Scope* scope_manager_search_by_name(ScopeManager* manager, char* name);





/* Function */
typedef struct Function Function;

Function* func_new(char* name, Scope* scope, enum Type ret);
Function* func_destroy(Function* func);
Function* func_search(Function* func, char* name);




/* FuncTable */
typedef struct FuncTable FuncTable;

FuncTable* func_table_new(void);
void func_table_destroy(FuncTable** table);
int func_table_add(FuncTable* table, char* name, Scope* scope, enum Type ret);
int func_table_search(FuncTable* table, char* name);
