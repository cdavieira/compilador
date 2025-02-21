#pragma once

#include "Scope.h"

typedef struct ScopeManager ScopeManager;

ScopeManager* scope_manager_new(void);
void scope_manager_destroy(ScopeManager** manager);
void scope_manager_print(ScopeManager* manager);

void scope_manager_enter(ScopeManager* manager);
void scope_manager_exit(ScopeManager* manager);
Scope* scope_manager_get_current_scope(ScopeManager* manager);
Scope* scope_manager_search_by_name(ScopeManager* manager, const char* name);
Scope* scope_manager_search(ScopeManager* manager, Variable* var);
