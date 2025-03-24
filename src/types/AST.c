#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "types/AST.h"
#include "types/Function.h"
#include "types/Literal.h"
#include "utils/Vector.h"
#include "types/Scope.h"

struct AST {
	NodeKind kind;
	enum Type type;
	int id;
	unsigned char has_data;
	NodeData data;
	Vector* children;
};

extern Vector* stringliterals;

static FILE* fdump;

static void ast_print_rec(AST* ast, int parent);

static void ast_export_dot_rec(AST *node);



/* instantiation/destruction */

AST* ast_new_node(NodeKind kind) {
	static int node_id = 0;
	AST* node = malloc(sizeof(struct AST));
	node->kind = kind;
	node->type = TYPE_VOID;
	node->has_data = 0;
	node->children = vector_new(8);
	node->id = node_id++;
	return node;
}

AST* ast_new_subtree(NodeKind kind, ...) {
	va_list va;
	va_start(va, kind);

	AST* node = ast_new_node(kind);

	for(AST* subnode = va_arg(va, void*);
	    subnode != NULL;
	    subnode = va_arg(va, void*))
	{
		ast_add_child(node, subnode);
	}

	va_end(va);
	return node;
}

void ast_free(AST *tree) {
	if(tree == NULL){
		return ;
	};

	const size_t count = vector_get_size(tree->children);
	for(int i = 0; i < count; i++) {
		ast_free(ast_get_child(tree, i));
	}

	vector_destroy(&tree->children, NULL);
	free(tree);
}




/* getters */

AST* ast_get_child(AST *parent, int idx) {
	return vector_get_item(parent->children, idx);
}

NodeKind ast_get_kind(AST *node) {
	return node->kind;
}

NodeData ast_get_data(AST *node) {
	return node->data;
}

int ast_get_children_count(AST *node) {
	return vector_get_size(node->children);
}

enum Type ast_get_type(AST *node){
	return node->type;
}




/* setters/modifiers */

void ast_set_data(AST* node, NodeData data){
	switch(node->kind){
		case NODE_FUNC:
		case NODE_FCALL:
		case NODE_PRINTF:
		case NODE_SCANF:
			node->type = func_get_return(data.func.func);
			node->data = data;
			node->has_data = 1;
			break;
		case NODE_VAR_USE:
		case NODE_VAR_DECL:
		case NODE_ARRAY_VAL:
		case NODE_PTR_VAL:
			node->type = data.var.var.type;
			node->data = data;
			node->has_data = 1;
			break;
		case NODE_ASSIGN:
		case NODE_CHR_VAL:
		case NODE_EQ:
		case NODE_FLT_VAL:
		case NODE_INT_VAL:
		case NODE_LT:
		case NODE_MINUS:
		case NODE_OVER:
		case NODE_PLUS:
		case NODE_FUNC_RET:
		case NODE_STR_VAL:
		case NODE_TIMES:
		case NODE_GT:
		case NODE_NE:
		case NODE_OR:
		case NODE_AND:
		case NODE_MOD:
			node->type = data.lit.type;
			node->data = data;
			node->has_data = 1;
			break;
		case NODE_C2I:
		case NODE_F2I:
			node->type = TYPE_INT;
			node->has_data = 0;
			break;
		case NODE_I2F:
		case NODE_C2F:
			node->type = TYPE_FLT;
			node->has_data = 0;
			break;
		case NODE_F2C:
		case NODE_I2C:
			node->type = TYPE_CHAR;
			node->has_data = 0;
			break;
		case NODE_BLOCK:
			node->data.block = data.block;
			node->has_data = 1;
			break;
		case NODE_IF:
		case NODE_PROGRAM:
		case NODE_VAR_LIST:
		case NODE_WHILE:
		case NODE_FUNC_PARAMLIST:
		case NODE_FUNC_BODY:
			node->type = TYPE_VOID;
			node->has_data = 0;
		case NODE_NOCONV:
			break;
	}
}

void ast_add_child(AST *parent, AST *child) {
	vector_append(parent->children, child);
}

char* ast_kind2str(NodeKind kind) {
	switch(kind) {
		case NODE_PROGRAM:      return "program"; 
		case NODE_BLOCK:        return "block";
		case NODE_IF:           return "if"; 
		case NODE_VAR_LIST:     return "var_list";
		case NODE_VAR_USE:      return "var_use"; 
		case NODE_VAR_DECL:     return "var_decl"; 
		case NODE_INT_VAL:      return "int_val"; 
		case NODE_FLT_VAL:      return "flt_val"; 
		case NODE_CHR_VAL:      return "chr_val"; 
		case NODE_STR_VAL:      return "str_val"; 
		case NODE_MINUS:        return "minus";
		case NODE_OVER:         return "over";
		case NODE_PLUS:         return "plus";
		case NODE_TIMES:        return "times";
		case NODE_MOD:          return "modulus";
		case NODE_LT:           return "lt"; 
		case NODE_GT:           return "gt"; 
		case NODE_EQ:           return "eq"; 
		case NODE_NE:           return "ne"; 
		case NODE_OR:           return "or"; 
		case NODE_AND:          return "and";
		case NODE_ASSIGN:       return "assign"; 
		case NODE_WHILE:        return "while";
		case NODE_SCANF:        return "scanf";
		case NODE_PRINTF:       return "printf";
		case NODE_FUNC:         return "function";
		case NODE_FCALL:        return "fcall";
		case NODE_FUNC_BODY:    return "funcbody";
		case NODE_ARRAY_VAL:    return "array";
		case NODE_PTR_VAL:    return "ptr";
		case NODE_I2F:          return "i2f";
		case NODE_I2C:          return "i2c";
		case NODE_C2I:          return "c2i";
		case NODE_C2F:          return "c2f";
		case NODE_F2I:          return "f2i";
		case NODE_F2C:          return "f2c";
		case NODE_NOCONV:       return "noconv";
		case NODE_FUNC_PARAMLIST: return "paramlist";
		case NODE_FUNC_RET:     return "funcret";
	}
	return "ERROR!!";
}

int ast_has_literal(AST* node) {
	switch(node->kind) {
		case NODE_ASSIGN:
		case NODE_CHR_VAL:
		case NODE_EQ:
		case NODE_FLT_VAL:
		case NODE_INT_VAL:
		case NODE_LT:
		case NODE_MINUS:
		case NODE_OVER:
		case NODE_PLUS:
		case NODE_SCANF:
		case NODE_FUNC:
		case NODE_STR_VAL:
		case NODE_TIMES:
		case NODE_PRINTF:
		case NODE_GT:
		case NODE_NE:
		case NODE_OR:
		case NODE_AND:
		case NODE_MOD:
		    return 1;
		case NODE_VAR_USE:
		case NODE_VAR_DECL:
		case NODE_C2I:
		case NODE_F2I:
		case NODE_I2F:
		case NODE_C2F:
		case NODE_F2C:
		case NODE_I2C:
		case NODE_BLOCK:
		case NODE_IF:
		case NODE_PROGRAM:
		case NODE_VAR_LIST:
		case NODE_WHILE:
		case NODE_NOCONV:
		case NODE_ARRAY_VAL:
		case NODE_PTR_VAL:
		case NODE_FUNC_PARAMLIST:
		case NODE_FUNC_BODY:
		case NODE_FCALL:
		case NODE_FUNC_RET:
			break;
	}
	return 0;
}

int ast_has_var(AST* node) {
	switch(node->kind) {
		case NODE_VAR_DECL:
		case NODE_VAR_USE:
		case NODE_ARRAY_VAL:
		case NODE_PTR_VAL:
		    return 1;
		case NODE_PROGRAM:
		case NODE_BLOCK:
		case NODE_IF:
		case NODE_VAR_LIST:
		case NODE_INT_VAL:
		case NODE_FLT_VAL:
		case NODE_CHR_VAL:
		case NODE_STR_VAL:
		case NODE_MINUS:
		case NODE_OVER:
		case NODE_PLUS:
		case NODE_TIMES:
		case NODE_LT:
		case NODE_GT:
		case NODE_EQ:
		case NODE_NE:
		case NODE_OR:
		case NODE_AND:
		case NODE_ASSIGN:
		case NODE_WHILE:
		case NODE_SCANF:
		case NODE_PRINTF:
		case NODE_FUNC:
		case NODE_I2F:
		case NODE_I2C:
		case NODE_C2I:
		case NODE_C2F:
		case NODE_F2I:
		case NODE_F2C:
		case NODE_NOCONV:
		case NODE_FUNC_PARAMLIST:
		case NODE_FUNC_BODY:
		case NODE_FCALL:
		case NODE_FUNC_RET:
		case NODE_MOD:
			break;
	}
	return 0;
}

int ast_has_scope(AST* node){
	switch(node->kind) {
		case NODE_VAR_DECL:
		case NODE_VAR_USE:
		case NODE_ARRAY_VAL:
		case NODE_PTR_VAL:
		case NODE_BLOCK:
		    return 1;
		case NODE_PROGRAM:
		case NODE_IF:
		case NODE_VAR_LIST:
		case NODE_INT_VAL:
		case NODE_FLT_VAL:
		case NODE_CHR_VAL:
		case NODE_STR_VAL:
		case NODE_MINUS:
		case NODE_OVER:
		case NODE_PLUS:
		case NODE_TIMES:
		case NODE_LT:
		case NODE_GT:
		case NODE_EQ:
		case NODE_NE:
		case NODE_OR:
		case NODE_AND:
		case NODE_ASSIGN:
		case NODE_WHILE:
		case NODE_SCANF:
		case NODE_PRINTF:
		case NODE_FUNC:
		case NODE_I2F:
		case NODE_I2C:
		case NODE_C2I:
		case NODE_C2F:
		case NODE_F2I:
		case NODE_F2C:
		case NODE_NOCONV:
		case NODE_FUNC_PARAMLIST:
		case NODE_FUNC_BODY:
		case NODE_FCALL:
		case NODE_FUNC_RET:
		case NODE_MOD:
			break;
	}
	return 0;
}

int ast_is_array(AST* node){
	int case1 = (node->kind == NODE_VAR_USE) &&
		    (node->data.var.var.qualifier > 0) &&
	            (vector_get_size(node->children) == 0);
	// int case2 = (node->kind == NODE_VAR_DECL) &&
	//             (vector_get_size(node->children) > 0) &&
	// 	    (((AST*)vector_get_item(node->children, 0))->kind == NODE_ARRAY_VAL);
	int case2 = (node->kind == NODE_VAR_DECL) &&
		    (node->data.var.var.qualifier > 0);
	return case1 || case2;
}

int ast_is_pointerlike(AST* node){
	int case2 = ((node->kind == NODE_VAR_USE) || (node->kind == NODE_VAR_DECL)) &&
	            (node->data.var.var.qualifier == QUALIFIER_POINTER);
	return ast_is_array(node) || case2;
}

Literal* ast_get_literal(AST* node){
	switch(node->kind){
		case NODE_ASSIGN:
		case NODE_CHR_VAL:
		case NODE_EQ:
		case NODE_FLT_VAL:
		case NODE_INT_VAL:
		case NODE_LT:
		case NODE_MINUS:
		case NODE_OVER:
		case NODE_PLUS:
		case NODE_SCANF:
		case NODE_FUNC:
		case NODE_STR_VAL:
		case NODE_TIMES:
		case NODE_PRINTF:
		case NODE_GT:
		case NODE_NE:
		case NODE_OR:
		case NODE_AND:
		case NODE_MOD:
			return &node->data.lit;
		case NODE_VAR_DECL:
		case NODE_VAR_USE:
		case NODE_ARRAY_VAL:
		case NODE_PTR_VAL:
		case NODE_PROGRAM:
		case NODE_BLOCK:
		case NODE_IF:
		case NODE_VAR_LIST:
		case NODE_WHILE:
		case NODE_I2F:
		case NODE_I2C:
		case NODE_C2I:
		case NODE_C2F:
		case NODE_F2I:
		case NODE_F2C:
		case NODE_NOCONV:
		case NODE_FUNC_PARAMLIST:
		case NODE_FUNC_BODY:
		case NODE_FCALL:
		case NODE_FUNC_RET:
			break;
	}
	return NULL;
}

Variable* ast_get_variable(AST* node){
	switch(node->kind){
		case NODE_VAR_USE:
		case NODE_VAR_DECL:
		case NODE_ARRAY_VAL:
		case NODE_PTR_VAL:
			return &node->data.var.var;
		case NODE_ASSIGN:
		case NODE_CHR_VAL:
		case NODE_EQ:
		case NODE_FLT_VAL:
		case NODE_INT_VAL:
		case NODE_LT:
		case NODE_MINUS:
		case NODE_OVER:
		case NODE_PLUS:
		case NODE_SCANF:
		case NODE_FUNC:
		case NODE_STR_VAL:
		case NODE_TIMES:
		case NODE_PRINTF:
		case NODE_GT:
		case NODE_NE:
		case NODE_OR:
		case NODE_AND:
		case NODE_MOD:
		case NODE_PROGRAM:
		case NODE_BLOCK:
		case NODE_IF:
		case NODE_VAR_LIST:
		case NODE_WHILE:
		case NODE_I2F:
		case NODE_I2C:
		case NODE_C2I:
		case NODE_C2F:
		case NODE_F2I:
		case NODE_F2C:
		case NODE_NOCONV:
		case NODE_FUNC_PARAMLIST:
		case NODE_FUNC_BODY:
		case NODE_FCALL:
		case NODE_FUNC_RET:
			break;
	}
	return NULL;
}

Scope* ast_get_scope(AST* node){
	switch(node->kind){
		case NODE_VAR_USE:
		case NODE_VAR_DECL:
		case NODE_ARRAY_VAL:
		case NODE_PTR_VAL:
			return node->data.var.scope;
		case NODE_BLOCK:
			return node->data.block.scope;
		case NODE_ASSIGN:
		case NODE_CHR_VAL:
		case NODE_EQ:
		case NODE_FLT_VAL:
		case NODE_INT_VAL:
		case NODE_LT:
		case NODE_MINUS:
		case NODE_OVER:
		case NODE_PLUS:
		case NODE_SCANF:
		case NODE_FUNC:
		case NODE_STR_VAL:
		case NODE_TIMES:
		case NODE_PRINTF:
		case NODE_GT:
		case NODE_NE:
		case NODE_OR:
		case NODE_AND:
		case NODE_MOD:
		case NODE_PROGRAM:
		case NODE_IF:
		case NODE_VAR_LIST:
		case NODE_WHILE:
		case NODE_I2F:
		case NODE_I2C:
		case NODE_C2I:
		case NODE_C2F:
		case NODE_F2I:
		case NODE_F2C:
		case NODE_NOCONV:
		case NODE_FUNC_PARAMLIST:
		case NODE_FUNC_BODY:
		case NODE_FCALL:
		case NODE_FUNC_RET:
			break;
	}
	return NULL;
}

void ast_print(AST* ast){
	ast_print_rec(ast, -1);
}

void ast_export_dot(AST *tree, FILE* fpout) {
	fdump = fpout;
	fprintf(fdump, "digraph {\ngraph [ordering=\"out\"];\n");
	ast_export_dot_rec(tree);
	fprintf(fdump, "}\n");
	fdump = NULL;
}

static void ast_print_rec(AST* ast, int parent){
	const char* nodename = ast_kind2str(ast->kind);
	const char* typename = type_name(ast->type);
	const size_t child_count = vector_get_size(ast->children);
	void* child;
	printf("===BEGIN %d (%d)===\n", ast->id, parent);
	printf("nodekind: %s\n", nodename);
	printf("type: %s\n", typename);
	printf("has data: %d\n", ast->has_data);
	printf("child count: %lu\n", child_count);
	for(size_t i=0; i<child_count; i++){
		child = vector_get_item(ast->children, i);
		if(child == NULL){
			printf("W: child %lu is NULL\n", i);
			continue;
		}
		ast_print_rec(child, ast->id);
	}
	printf("===END %d===\n", ast->id);
}

static void ast_export_dot_rec(AST *node) {
	NodeKind kind = node->kind;
	NodeData data = node->data;
	enum Type type = node->type;
	const char* typename = type_name(type);
	const size_t count = ast_get_children_count(node);

	//beginning of label
	fprintf(fdump, "node%d[label=\"", node->id);

	if(type != TYPE_VOID) {
		fprintf(fdump, "(%s) ", typename);
	}

	if(kind == NODE_VAR_DECL || kind == NODE_VAR_USE) {
		fprintf(fdump, "%s@%d", data.var.var.name, scope_get_id(data.var.scope));
	}
	else{
		fprintf(fdump, "%s", ast_kind2str(kind));
	}

	if(kind == NODE_ARRAY_VAL){
		fprintf(fdump, "#%d", node->data.var.var.qualifier);
	}
	else if(kind == NODE_PTR_VAL){
		// fprintf(fdump, "");
	}

	if(ast_has_literal(node)) {
		if(node->kind == NODE_FLT_VAL) {
			fprintf(fdump, "%.2f", data.lit.value.f);
		}
		else if(node->kind == NODE_STR_VAL) {
			fprintf(fdump, "@%d", data.lit.value.i);
		}
		else if(node->kind == NODE_CHR_VAL) {
			fprintf(fdump, "%c", data.lit.value.c);
		}
		else if(node->kind == NODE_INT_VAL) {
			fprintf(fdump, "%d", data.lit.value.i);
		}
	}

	fprintf(fdump, "\"];\n");
	//end of label

	AST* child;
	// printf("%lu\n", count);
	for(int i = 0; i < count; i++) {
		child = ast_get_child(node, i);
		if(child != NULL){
		  ast_export_dot_rec(child);
		  fprintf(fdump, "node%d -> node%d;\n", node->id, child->id);
		}
		else{
		  fprintf(stdout, "node%d is null from node%d\n", i, node->id);
		}
	}
}
