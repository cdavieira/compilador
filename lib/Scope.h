#pragma once

#include "VarTable.h"

typedef struct Scope Scope;

Scope* scope_new(int id, int parent, int level);
Scope* scope_destroy(Scope* scope);

int scope_get_id(Scope* scope);
int scope_get_parent(Scope* scope);
int scope_get_level(Scope* scope);
VarTable* scope_get_vartable(Scope* scope);

int scope_add(Scope* scope, char* name, int line, enum Type type);
Variable* scope_search_by_name(Scope* scope, const char* name);
