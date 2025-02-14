#pragma once

#include <stdarg.h>
#include "Literal.h"
#include "tables.h"

typedef enum {
	/* lang */
	PROGRAM_NODE,
	BLOCK_NODE,
	IF_NODE,
	VAR_LIST_NODE,

	/* variables */
	VAR_USE_NODE,
	VAR_DECL_NODE,

	/* types */
	BOOL_VAL_NODE,
	INT_VAL_NODE,
	REAL_VAL_NODE,
	STR_VAL_NODE,

	/* operators */
	LT_NODE,
	MINUS_NODE,
	OVER_NODE,
	PLUS_NODE,
	ASSIGN_NODE,
	EQ_NODE,
	REPEAT_NODE,
	TIMES_NODE,

	/* functions */
	READ_NODE,
	WRITE_NODE,

	/* conversions */
	B2I_NODE,
	B2R_NODE,
	B2S_NODE,
	I2R_NODE,
	I2S_NODE,
	R2S_NODE,
	NOCONV_NODE
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
AST* ast_new_subtree(NodeKind kind,  ...);

AST* ast_get_child(AST *parent, int idx);
NodeKind ast_get_kind(AST *node);
Type ast_get_type(AST *node);
NodeData ast_get_data(AST *node);
int ast_get_children_count(AST *node);

void ast_set_data(AST* node, NodeData data);
void ast_add_child(AST *parent, AST *child);

int ast_has_literal(AST* node);
int ast_has_var(AST* node);
Literal* ast_get_literal(AST* node);

char* ast_kind2str(NodeKind kind);
void print_tree(AST *ast);
void print_dot(AST *ast, VarTable* vt);
