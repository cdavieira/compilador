#include "types/Literal.h"
#include "types/Function.h"
#include "types/Scope.h"
#include "parser/FuncTable.h"
#include "parser/VarTable.h"
#include "utils/Vector.h"
#include <malloc.h>


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

int func_table_add(FuncTable* table, char* name, Scope* scope, enum Type ret, int param_count, int definition){
	Function* f = func_table_search(table, name);

	if(f){
		//the function already exists
		if(func_is_defined(f)){
			return -1;
		}
		func_add_definition(f, scope);
		return 0;
	}

	//the function does not exist yet
	Function* func = func_new(name, scope, ret, param_count, definition);
	vector_append(table->functions, func);
	return 0;
}

Function* func_table_search(FuncTable* table, char* name){
	Function* func = (Function*) vector_search(table->functions, name, (int (*)(void*, void*)) func_compare_name);
	return func;
}

void func_table_print(FuncTable* table){
	Function* func;
	const char* funcname;
	int num_params;
	enum Type ret;
	Scope* scope;
	int scope_id;
	int scope_parent;
	VarTable* vt;

	printf("====== FuncTable BEGIN ====== \n");

	const size_t sz = vector_get_size(table->functions);
	for(size_t i=0; i<sz; i++){
		func = vector_get_item(table->functions, i);
		funcname = func_get_name(func);
		num_params = func_get_nparams(func);
		ret = func_get_return(func);
		scope = func_get_scope(func);
		scope_id = scope_get_id(scope);
		scope_parent = scope_get_parent(scope);
		vt = scope_get_vartable(scope);

		printf("Function '%s'\n", funcname);
		printf("number of params: %d\n", num_params);
		printf("return type: '%s'\n", type_name(ret));
		printf("Scope: id=%d, parent=%d\n", scope_id, scope_parent);
		vartable_print(vt);

		if((i+1) != sz){
			putchar('\n');
			putchar('\n');
		}
	}

	printf("====== FuncTable END ====== \n");
}
