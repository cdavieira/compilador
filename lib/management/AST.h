#pragma once

#include <stdarg.h>
#include "type/Literal.h"
#include "type/Variable.h"
#include "management/VarTable.h"

typedef enum {
	/* lang */
	NODE_PROGRAM,
	NODE_DECLARATORS,
	NODE_BLOCK,
	NODE_IF,

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

	/* operators */
	NODE_MINUS,
	NODE_OVER,
	NODE_PLUS,
	NODE_TIMES,
	NODE_LT,
	NODE_GT,
	NODE_EQ,
	NODE_NE,
	NODE_OR,
	NODE_AND,
	NODE_ASSIGN,

	/* lang constructs */
	NODE_WHILE,

	/* functions */
	NODE_SCANF,
	NODE_PRINTF,
	NODE_FUNC_DECL,
	NODE_FUNC_USE,
	NODE_FUNC_PARAMLIST,

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
	  Variable var;
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

char* ast_kind2str(NodeKind kind);
void print_dot(AST *ast, VarTable* vt);
void ast_print(AST* ast);
