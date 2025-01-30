#include "scope.h"
#include "vector.h"
#include "stack.h"
#include <stdio.h>
#include <malloc.h>
#include <string.h>

#define DEBUG 1

struct Scope {
	VarTable* vartable; //the symbolic table of variables for only this scope
	int id; //the identifier of this ID
	int parent; //the identifier of the parental scope
	int level; //the depth/level of how deep this scope is in a nested-scope context
};

Scope* scope_new(int id, int parent, int level){
	Scope* scope = malloc(sizeof(Scope));
	scope->vartable = create_var_table();
	scope->id = id;
	scope->parent = parent;
	scope->level = level;
	return scope;
}

Scope* scope_destroy(Scope* scope){
	free_var_table(scope->vartable);
	free(scope);
	return NULL;
}

int scope_get_id(Scope* scope){
	return scope->id;
}

int scope_get_parent(Scope* scope){
	return scope->parent;
}

int scope_get_level(Scope* scope){
	return scope->level;
}

VarTable* scope_get_vartable(Scope* scope){
	return scope->vartable;
}

int scope_search(Scope* scope, Variable* entry){
	return lookup_var(scope->vartable, entry->name, entry->scope_level);
}

int scope_search_by_name(Scope* scope, char* name){
	Variable lookup;
	lookup = (Variable) {.name = name, .scope_level = scope->level,};
	//strcpy(lookup.name, name);
	return scope_search(scope, &lookup);
}

int scope_add(Scope* scope, char* name, int line, enum Type type){
	int idx;
	if((idx = scope_search_by_name(scope, name)) != -1){
		return -1;
	}

#ifdef DEBUG
	printf("Adding new variable '%s' to scope %d\n", name, scope->id);
#endif

	return add_var(scope->vartable, name, line, type, scope->level);
}




/* ScopeManager */

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
	ScopeManager* manager, Scope* scope, Variable* data);

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
		printf("Scope %d: parent=%d, level=%d\n", scope->id, scope->parent, scope->level);
		scope_manager_print_recursive(manager, scope);
		printf("====\n");
	}
}

static void scope_manager_print_recursive(
		ScopeManager* manager, Scope* scope)
{
	print_var_table(scope->vartable);
	if(scope->parent == -1){
		return ;
	}
	Scope* parent = vector_get_item(manager->scopes, scope->parent);
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

Scope* scope_manager_search(ScopeManager* manager, Variable* data){
	Scope* scope = scope_manager_get_current_scope(manager);
	int idx = scope_manager_search_recursive(manager, scope, data);
	if(idx == -1){
		return NULL;
	}
	scope = vector_get_item(manager->scopes, idx);
	return scope;
}

Scope* scope_manager_search_by_name(ScopeManager* manager, char* name){
	Variable entry = {.name = name, .scope_level = manager->depth};
	//strcpy(entry.name, name);
	return scope_manager_search(manager, &entry);
}

static int scope_manager_search_recursive(
	ScopeManager* manager, Scope* scope, Variable* data)
{
	int idx = scope_search(scope, data);
	if(idx != -1){
		return scope->id;
	}

	if(scope->parent == -1){
		return -1;
	}

	Scope* parent = vector_get_item(manager->scopes, scope->parent);
	return scope_manager_search_recursive(manager, parent, data);
}

static int* int_new(int value){
	int *v = malloc(sizeof(int));
	*v = value;
	return v;
}

static void int_destroy(int* value){
	free(value);
}





/* FuncVar */

struct FuncVar {
	char* name;
	Scope* scope;
	enum Type ret;
};

static int func_compare_name(FuncVar* func, char* name);

FuncVar* func_new(char* name, Scope* scope, enum Type ret){
	FuncVar* func = malloc(sizeof(FuncVar));		
	func->name = name;
	func->ret = ret;
	func->scope = scope;
	return func;
}

FuncVar* func_destroy(FuncVar* func){
	free(func);
	return NULL;
}

static int func_compare_name(FuncVar* func, char* name){
	return strcmp(func->name, name) == 0;
}





/* FuncTable */
struct FuncTable {
	Vector* functions;
};

FuncTable* func_table_new(void){
	FuncTable* table = malloc(sizeof(FuncTable));
	table->functions = vector_new(16);
	return table;
}

void func_table_destroy(FuncTable** table){
	vector_destroy(&(*table)->functions, (void (*)(void*)) func_destroy);
	free(*table);
	*table = NULL;
}

int func_table_add(FuncTable* table, char* name, Scope* scope, enum Type ret){
	if(func_table_search(table, name) != -1){
		return -1;
	}
	FuncVar* func = func_new(name, scope, ret);
	vector_append(table->functions, func);
	return 0;
}

int func_table_search(FuncTable* table, char* name){
	FuncVar* func = (FuncVar*) vector_search(table->functions, name, (int (*)(void*, void*)) func_compare_name);
	return func == NULL? -1 : 1;
}
