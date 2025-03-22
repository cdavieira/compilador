#pragma once

#include <stdarg.h>
#include <stdio.h>
#include "types/Function.h"
#include "types/Literal.h"
#include "types/Scope.h"
#include "types/Variable.h"

typedef enum {
	/* lang */
	NODE_PROGRAM,
	NODE_BLOCK,

	/* variables */
	NODE_VAR_USE,
	NODE_VAR_DECL,
	NODE_VAR_LIST,

	/* types */
	NODE_INT_VAL,
	NODE_FLT_VAL,
	NODE_CHR_VAL,
	NODE_STR_VAL,
	NODE_ARRAY_VAL,
	NODE_PTR_VAL,

	/* operators */
	NODE_MINUS,
	NODE_OVER,
	NODE_PLUS,
	NODE_TIMES,
	NODE_MOD,
	NODE_LT,
	NODE_GT,
	NODE_EQ,
	NODE_NE,
	NODE_OR,
	NODE_AND,
	NODE_ASSIGN,

	/* lang constructs */
	NODE_WHILE,
	NODE_IF,

	/* functions */
	NODE_SCANF,
	NODE_PRINTF,
	NODE_FUNC,
	NODE_FUNC_PARAMLIST,
	NODE_FUNC_BODY,
	NODE_FUNC_RET,
	NODE_FCALL,

	/* conversions */
	NODE_I2F,
	NODE_I2C,
	NODE_C2I,
	NODE_C2F,
	NODE_F2I,
	NODE_F2C,
	NODE_NOCONV,
} NodeKind;

typedef union NodeData {
	struct {
		Scope* scope;
		Variable var;
	} var;
	struct {
		Function* func;
	} func;
	Literal lit;
	char* str;
	void* nil;
} NodeData;

typedef struct AST AST;

AST* ast_new_node(NodeKind kind);
void ast_free(AST *ast);
/* calling convention: last parameter should be NULL! */
AST* ast_new_subtree(NodeKind kind, ...);

AST* ast_get_child(AST *parent, int idx);
NodeKind ast_get_kind(AST *node);
enum Type ast_get_type(AST *node);
NodeData ast_get_data(AST *node);
int ast_get_children_count(AST *node);

void ast_set_data(AST* node, NodeData data);
void ast_add_child(AST *parent, AST *child);

int ast_has_literal(AST* node);
int ast_has_var(AST* node);
Literal* ast_get_literal(AST* node);
Variable* ast_get_variable(AST* node);

char* ast_kind2str(NodeKind kind);
void ast_export_dot(AST *ast, FILE* fpout);
void ast_print(AST* ast);
