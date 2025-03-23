#pragma once

#include "parser/VarTable.h"

typedef struct Scope Scope;

Scope* scope_new(int id, int parent, int level);
Scope* scope_destroy(Scope* scope);
void scope_print(Scope* scope);

int scope_get_id(const Scope* scope);
int scope_get_parent(const Scope* scope);
int scope_get_level(const Scope* scope);
VarTable* scope_get_vartable(const Scope* scope);
Variable* scope_get_var(Scope* scope, int idx);

int scope_add(
    Scope* scope,
    char* name,
    int line,
    enum Type type,
    enum Qualifier qualifier,
    unsigned uid);
Variable* scope_search_by_name(Scope* scope, const char* name);
Variable* scope_search(Scope* scope, Variable* search);
