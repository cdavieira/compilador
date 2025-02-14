#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ast.h"
#include "vector.h"

struct AST {
	NodeKind kind;
	Type type;
	NodeData data;
	Vector* children;
};



/* instantiation/destruction */

AST* ast_new_node(NodeKind kind, NodeData data) {
	AST* node = malloc(sizeof(struct AST));
	node->kind = kind;
	ast_set_data(node, data);
	node->children = vector_new(8);
	return node;
}

AST* ast_new_subtree(NodeKind kind, NodeData data, ...) {
	va_list va;
	va_start(va, data);

	AST* node = ast_new_node(kind, data);

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

Type ast_get_type(AST *node){
	return node->type;
}




/* setters/modifiers */

void ast_set_data(AST* node, NodeData data){
	switch(node->kind){
		case VAR_USE_NODE:
		case VAR_DECL_NODE:
			node->type = data.var.literal.type;
			node->data = data;
			break;
		case READ_NODE:
		case WRITE_NODE:
			node->type = data.lit.type;
			node->data = data;
			break;
		case BOOL_VAL_NODE:
		case INT_VAL_NODE:
		case REAL_VAL_NODE:
		case STR_VAL_NODE:
			node->type = data.lit.type;
			node->data = data;
			break;
		case PROGRAM_NODE:
		case BLOCK_NODE:
		case IF_NODE:
		case VAR_LIST_NODE:
			node->type = TYPE_NOTHING;
			node->data = data;
			break;
		case LT_NODE:
		case MINUS_NODE:
		case OVER_NODE:
		case PLUS_NODE:
		case ASSIGN_NODE:
		case EQ_NODE:
		case REPEAT_NODE:
		case TIMES_NODE:
			node->type = data.lit.type;
			node->data = data;
			break;
		case B2I_NODE:
			node->type = TYPE_INT;
			break;
		case B2R_NODE:
		case I2R_NODE:
			node->type = TYPE_FLT;
			break;
		case B2S_NODE:
		case I2S_NODE:
		case R2S_NODE:
			node->type = TYPE_STR;
			break;
		default:
			node->type = TYPE_NOTHING;
			break;
	}
}

void ast_add_child(AST *parent, AST *child) {
	vector_append(parent->children, child);
}

char* ast_kind2str(NodeKind kind) {
	switch(kind) {
		case ASSIGN_NODE:   return ":=";
		case EQ_NODE:       return "=";
		case BLOCK_NODE:    return "block";
		case BOOL_VAL_NODE: return "";
		case IF_NODE:       return "if";
		case INT_VAL_NODE:  return "";
		case LT_NODE:       return "<";
		case MINUS_NODE:    return "-";
		case OVER_NODE:     return "/";
		case PLUS_NODE:     return "+";
		case PROGRAM_NODE:  return "program";
		case READ_NODE:     return "read";
		case REAL_VAL_NODE: return "";
		case REPEAT_NODE:   return "repeat";
		case STR_VAL_NODE:  return "";
		case TIMES_NODE:    return "*";
		case VAR_DECL_NODE: return "var_decl";
		case VAR_LIST_NODE: return "var_list";
		case VAR_USE_NODE:  return "var_use";
		case WRITE_NODE:    return "write";
		case B2I_NODE:      return "B2I";
		case B2R_NODE:      return "B2R";
		case B2S_NODE:      return "B2S";
		case I2R_NODE:      return "I2R";
		case I2S_NODE:      return "I2S";
		case R2S_NODE:      return "R2S";
		default:            return "ERROR!!";
	}
}

int ast_has_literal(AST* node) {
	switch(node->kind) {
		case BOOL_VAL_NODE:
		case INT_VAL_NODE:
		case REAL_VAL_NODE:
		case STR_VAL_NODE:
		    return 1;
		default:
		    return 0;
	}
}

int ast_has_var(AST* node) {
	switch(node->kind) {
		case VAR_DECL_NODE:
		case VAR_USE_NODE:
		    return 1;
		default:
		    return 0;
	}
}

Literal* ast_get_literal(AST* node){
	switch(node->kind){
		case BOOL_VAL_NODE:
		case INT_VAL_NODE:
		case REAL_VAL_NODE:
		case STR_VAL_NODE:
		case LT_NODE:
		case MINUS_NODE:
		case OVER_NODE:
		case PLUS_NODE:
		case ASSIGN_NODE:
		case EQ_NODE:
		case TIMES_NODE:
			return &node->data.lit;
		case VAR_DECL_NODE:
		case VAR_USE_NODE:
			return &node->data.var.literal;
		default:
			return NULL;
	}
}



// Dot output.

void print_tree(AST *ast){
	printf("KIND: %s\n", ast_kind2str(ast->kind));
	printf("TYPE: %s\n", type_name(ast->type));
}

int nr;
static int print_node_dot(AST *node, VarTable* vt);
void print_dot(AST *tree, VarTable* vt) {
	nr = 0;
	fprintf(stderr, "digraph {\ngraph [ordering=\"out\"];\n");
	print_node_dot(tree, vt);
	fprintf(stderr, "}\n");
}

static int print_node_dot(AST *node, VarTable* vt) {
	// if(node == NULL){
	//   puts("Deu ruim");
	//   return -1;
	// }
	// if(vt == NULL){
	//   puts("Deu ruim dnv");
	//   return -1;
	// }
	int my_nr = nr++;
	NodeKind kind = node->kind;
	NodeData data = node->data;
	Type type = node->type;
	const char* typename = type_name(type);
	const size_t count = ast_get_children_count(node);

	//beginning of label
	fprintf(stderr, "node%d[label=\"", my_nr);

	if(type != TYPE_NOTHING) {
		fprintf(stderr, "(%s) ", typename);
	}
	if(kind == VAR_DECL_NODE || kind == VAR_USE_NODE) {
		fprintf(stderr, "%s@", data.var.name);
	}
	else{
		fprintf(stderr, "%s", ast_kind2str(kind));
	}

	if(ast_has_literal(node)) {
		if(node->kind == REAL_VAL_NODE) {
			fprintf(stderr, "%.2f", data.lit.value.f);
		}
		else if(node->kind == STR_VAL_NODE) {
			fprintf(stderr, "@%s", data.lit.value.s);
		}
		else{
			fprintf(stderr, "%d", data.lit.value.i);
		}
	}

	fprintf(stderr, "\"];\n");
	//end of label

	for(int i = 0; i < count; i++) {
		int child_nr = print_node_dot(ast_get_child(node, i), vt);
		fprintf(stderr, "node%d -> node%d;\n", my_nr, child_nr);
	}

	return my_nr;
}
