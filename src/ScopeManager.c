#include "ScopeManager.h"
#include "Scope.h"
#include "Vector.h"
#include "Stack.h"

#include <stdio.h>
#include <malloc.h>
#include <string.h>

struct ScopeManager {
	Vector* scopes; //all scopes managed by this instance

	//a stack of integers (scope ids)
	Stack* scope_tracker; //stores the history of all scopes entered/exited

	int current; //the index of the current scope within 'scopes'
	int depth; //the depth/level of how deep the current scope is
	int id; //incremental integer that generates ids for different scopes
};

static int* int_new(int value);
static void int_destroy(int* value);
static void scope_manager_print_recursive(
		ScopeManager* manager, Scope* scope);
static int scope_manager_search_recursive(
	ScopeManager* manager, Scope* scope, const char* name);

ScopeManager* scope_manager_new(void){
	ScopeManager* manager = malloc(sizeof(ScopeManager));
	manager->scopes = vector_new(32);
	manager->scope_tracker = stack_new(16);
	manager->current = 0;
	manager->depth = 0;
	manager->id = 0;

	Scope* global = scope_new(0, -1, 0);
	vector_append(manager->scopes, global);

	return manager;
}

void scope_manager_destroy(ScopeManager** manager){
	vector_destroy(&(*manager)->scopes, (void (*)(void*)) scope_destroy);
	stack_destroy(&(*manager)->scope_tracker, free);
	free(*manager);
	*manager = NULL;
}

void scope_manager_print(ScopeManager* manager){
	size_t size = vector_get_size(manager->scopes);
	Scope* scope;
	for(size_t i=0; i<size; i++){
		scope = vector_get_item(manager->scopes, i);
		printf("Scope %d: parent=%d, level=%d\n", scope_get_id(scope), scope_get_parent(scope), scope_get_level(scope));
		scope_manager_print_recursive(manager, scope);
		printf("====\n");
	}
}

static void scope_manager_print_recursive(
		ScopeManager* manager, Scope* scope)
{
	vartable_print(scope_get_vartable(scope));
	if(scope_get_parent(scope) == -1){
		return ;
	}
	Scope* parent = vector_get_item(manager->scopes, scope_get_parent(scope));
	scope_manager_print_recursive(manager, parent);
}

void scope_manager_enter(ScopeManager* manager){
	int *prev_scope;
	Scope *new_scope;

	//saving the id of the current scope as the old one
	prev_scope = int_new(manager->current);
	stack_push(manager->scope_tracker, prev_scope);

	//entering a new scope (in a deeper level)
#ifdef DEBUG
	printf("Entering new scope (prev = %d, next = %d, depth = %d)\n", *prev_scope, manager->id+1, manager->depth+1);
#endif

	new_scope = scope_new(manager->id + 1, *prev_scope, manager->depth + 1);
	vector_append(manager->scopes, new_scope);
	manager->id++;
	manager->depth++;
	manager->current = manager->id;
}

Scope* scope_manager_get_current_scope(ScopeManager* manager){
	return vector_get_item(manager->scopes, manager->current);
}

void scope_manager_exit(ScopeManager* manager){
	//saving the id of the current scope as the old one
	int old_scope = manager->current;

	//leaving the previous scope
	int *v = stack_pop(manager->scope_tracker);
	manager->current = *v;
	int_destroy(v);
	manager->depth--;

#ifdef DEBUG
	printf("Exiting scope (prev = %d, next = %d, depth = %d)\n", old_scope, manager->current, manager->depth);
#endif
}

Scope* scope_manager_search_by_name(ScopeManager* manager, const char* varname){
	Scope* scope = scope_manager_get_current_scope(manager);
	int idx = scope_manager_search_recursive(manager, scope, varname);
	if(idx == -1){
		return NULL;
	}
	scope = vector_get_item(manager->scopes, idx);
	return scope;
}

static int scope_manager_search_recursive(
	ScopeManager* manager, Scope* scope, const char* name)
{
	if(scope_search_by_name(scope, name) != NULL){
		return scope_get_id(scope);
	}

	if(scope_get_parent(scope) == -1){
		return -1;
	}

	Scope* parent = vector_get_item(manager->scopes, scope_get_parent(scope));
	return scope_manager_search_recursive(manager, parent, name);
}

static int* int_new(int value){
	int *v = malloc(sizeof(int));
	*v = value;
	return v;
}

static void int_destroy(int* value){
	free(value);
}
