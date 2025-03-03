#include "types/Literal.h"
#include "types/Scope.h"
#include "parser/VarTable.h"

#include <stdio.h>
#include <malloc.h>
#include <string.h>

struct Scope {
	VarTable* vartable; //the symbolic table of variables for only this scope
	int id; //the identifier of this ID
	int parent; //the identifier of the parental scope
	int level; //the depth/level of how deep this scope is in a nested-scope context
};


Scope* scope_new(int id, int parent, int level){
	Scope* scope = malloc(sizeof(Scope));
	scope->vartable = vartable_new();
	scope->id = id;
	scope->parent = parent;
	scope->level = level;
	return scope;
}

Scope* scope_destroy(Scope* scope){
	vartable_destroy(scope->vartable);
	free(scope);
	return NULL;
}

void scope_print(Scope* scope){

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

Variable* scope_search_by_name(Scope* scope, const char* name){
	return vartable_search(scope->vartable, name);
}

Variable* scope_get_var(Scope* scope, int idx){
	return vartable_idx(scope->vartable, idx);
}

int scope_add(Scope* scope, char* name, int line, enum Type type, enum Qualifier qualifier){
	if(scope_search_by_name(scope, name) != NULL){
		return -1;
	}

	//printf("Adding new variable '%s' to scope %d\n", name, scope->id);

	return vartable_add(scope->vartable, name, line, type, qualifier);
}
