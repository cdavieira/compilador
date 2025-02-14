%define parse.error verbose
%define parse.lac full

%{
#include <stdio.h>
#include "Literal.h"
#include "tables.h"
#include "scope.h"
#include "ast.h"

extern char* yytext;
extern int yylineno;
static ScopeManager* scope_manager;
AST* root;
Type TYPE_DECL;
int decl_list_created;
int stmt_list_created;

int yylex(void);
void yyerror(char const *s);

void parser_init(void);
void parser_deinit(void);
void enter_scope(void);
void exit_scope(void);
void unary_operation(
	Literal* op1,
	Literal* res,
	int (*operation)(void*, void*),
	const char* operator);
void binary_operation(
	AST* op1,
	AST* op2,
	AST** res,
	int (*operation)(Literal*, Literal*, BinaryOperationData*),
	const char* operator,
	NodeKind nodekind);
void add_var_declaration(char* name, enum Type type);
int check_var_declaration(char* name);
int check_assignment(char* name, enum Type type);
int check_if_repeat_expr(enum Type type);
void save_token_to_var(Literal* props, char* varname);
void load_var_to_token(char* varname, AST** res);
void parser_get_var(char* name, Variable* res);
%}

%code requires {
#include "ast.h"
#include "Literal.h"
union TokenData {
	Literal l;
	char* str;
	AST* ast;
};
}

%define api.value.type {union TokenData}

%token PROGRAM SEMI VAR INT BOOL REAL STRING BBEGIN READ WRITE IF THEN ELSE REPEAT UNTIL END LPAR RPAR
%token <l> FLOAT_VAL INT_VAL STRING_VAL TRUE FALSE
%token <str> ASSIGN EQ LT TIMES PLUS OVER MINUS ID
%nterm <ast> expr program vars_sect opt_var_decl
%nterm <ast> var_decl_list var_decl
%nterm <ast> stmt_sect stmt_list stmt
%nterm <ast> if_stmt repeat_stmt assign_stmt
%nterm <ast> read_stmt write_stmt

%left EQ LT
%left PLUS MINUS
%left TIMES OVER
%%

program: PROGRAM ID SEMI vars_sect stmt_sect {
		NodeData data;
		data.nil = NULL;
		root = ast_new_node(PROGRAM_NODE);
		ast_set_data(root, data);
		if($4 != NULL){
			ast_add_child(root, $4);
		}
		if($5 != NULL){
			ast_add_child(root, $5);			
		}
	}
;

vars_sect:
	VAR opt_var_decl {
		$$ = $2;
	}
;

opt_var_decl:
	%empty         { $$ = NULL; }
|	var_decl_list  {
		$$ = $1;
	}
;

var_decl_list:
	var_decl_list var_decl {
		if(decl_list_created == 0){
			NodeData data;
			data.nil = NULL;
			$$ = ast_new_node(VAR_LIST_NODE);
			ast_set_data($$, data);
			decl_list_created = 1;
		}
		ast_add_child($$, $2);
	}
|	var_decl { 
		if(decl_list_created == 0){
			NodeData data;
			data.nil = NULL;
			$$ = ast_new_node(VAR_LIST_NODE);
			ast_set_data($$, data);
			decl_list_created = 1;
		}
		ast_add_child($$, $1);
	}
;

var_decl:
	type_spec ID SEMI {
		add_var_declaration($2, TYPE_DECL);
		// NodeData data;
		load_var_to_token($2, &$$);
	}
;

type_spec:
	BOOL   { TYPE_DECL = TYPE_BOO; }
|	INT    { TYPE_DECL = TYPE_INT; }
|	REAL   { TYPE_DECL = TYPE_FLT; }
|	STRING { TYPE_DECL = TYPE_STR; }
;

stmt_sect:
	BBEGIN stmt_list END { $$ = $2; }
|	BBEGIN END           { $$ = NULL; }
;

stmt_list:
	stmt_list stmt  {
		if(stmt_list_created == 0){
			NodeData data;
			data.nil = NULL;
			$$ = ast_new_node(BLOCK_NODE);
			ast_set_data($$, data);
			stmt_list_created = 1;
		}
		if($2 != NULL){
			ast_add_child($$, $2);
		}
		else{
			printf("Can't add a child to stmt list (2)\n");
		}
	}
|	stmt {
		if(stmt_list_created == 0){
			NodeData data;
			data.nil = NULL;
			$$ = ast_new_node(BLOCK_NODE);
			ast_set_data($$, data);
			stmt_list_created = 1;
		}
		if($1 != NULL){
			ast_add_child($$, $1);
		}
		else{
			printf("Can't add a child to stmt list (1)\n");
		}
	}
;

stmt:
	if_stmt     { $$ = $1; }
|	repeat_stmt { $$ = $1; }
|	assign_stmt { $$ = $1; }
|	read_stmt   { $$ = $1; }
|	write_stmt  { $$ = $1; }
;

if_stmt:
	IF expr THEN stmt_list if_stmt_pt2 {
		/* check_if_repeat_expr($2.type); */
		fprintf(stdout, "IF NODE INCOMPLETE\n");
		$$ = $2;
	}
;

if_stmt_pt2:
	END
|	ELSE stmt_list END
;

repeat_stmt:
	REPEAT stmt_list UNTIL expr {
		/* check_if_repeat_expr($4.type);
		fprintf(stdout, "REPEAT NODE INCOMPLETE\n");
		$$ = $4;
	 */ }
;

assign_stmt:
	ID ASSIGN expr SEMI { 
	    Literal* lit = ast_get_literal($3);
	    if(lit != NULL){
		    save_token_to_var(lit, $1);

			NodeData data;
			data.lit = *lit;
			$$ = ast_new_node(ASSIGN_NODE);
			ast_set_data($$, data);

			Variable var;
			parser_get_var($1, &var);
			data.var = var;
			AST* ass = ast_new_node(VAR_USE_NODE);
			ast_set_data(ass, data);
			ast_add_child($$, ass);
			ast_add_child($$, $3);
	    }
		else{
			$$ = NULL;
		}
	}
;

read_stmt:
	READ ID SEMI {
		Variable var;
		parser_get_var($2, &var);
		NodeData data;
		data.lit = var.literal;
		$$ = ast_new_node(READ_NODE);
		ast_set_data($$, data);

		AST* varread = NULL;
		load_var_to_token($2, &varread);
		ast_add_child($$, varread);
	}
;

write_stmt:
	WRITE expr SEMI { 
	    Literal* lit = ast_get_literal($2);
		if(lit != NULL){
			NodeData data;
			data.lit = *lit;
			$$ = ast_new_node(WRITE_NODE);
			ast_set_data($$, data);
			ast_add_child($$, $2);
		}
		else{
			fprintf(stdout, "Error write!\n");
		}
		// switch(ast_get_type($2)){
		// 	case TYPE_INT:
		// 		printf("write writes %d\n", $2.value.i);
		// 		break;
		// 	case TYPE_FLT:
		// 		printf("write writes %f\n", $2.value.f);
		// 		break;
		// 	case TYPE_BOO:
		// 		printf("write writes %d\n", $2.value.b);
		// 		break;
		// 	case TYPE_STR:
		// 		printf("write writes %s\n", $2.value.s);
		// 		break;
		// }
	}
;

//WARNING: STRING_VAL IS BEING PASSED BY REFERENCE, NOT BY COPY
expr:
	expr LT expr     { binary_operation($1, $3, &$$, literal_lt, $2, LT_NODE); }
|	expr EQ expr     { binary_operation($1, $3, &$$, literal_eq, $2, EQ_NODE); }
|	expr PLUS expr   { binary_operation($1, $3, &$$, literal_sum, $2, PLUS_NODE); }
|	expr MINUS expr  { binary_operation($1, $3, &$$, literal_minus, $2, MINUS_NODE); }
|	expr TIMES expr  { binary_operation($1, $3, &$$, literal_mult, $2, TIMES_NODE); }
|	expr OVER expr   { binary_operation($1, $3, &$$, literal_div, $2, OVER_NODE); }
|	LPAR expr RPAR   { $$ = $2; }
|	TRUE             { NodeData data; data.lit = $1; $$ = ast_new_node(BOOL_VAL_NODE); ast_set_data($$, data); }
|	FALSE            { NodeData data; data.lit = $1; $$ = ast_new_node(BOOL_VAL_NODE); ast_set_data($$, data); }
|	INT_VAL          { NodeData data; data.lit = $1; $$ = ast_new_node(INT_VAL_NODE);  ast_set_data($$, data); }
|	FLOAT_VAL        { NodeData data; data.lit = $1; $$ = ast_new_node(REAL_VAL_NODE); ast_set_data($$, data); }
|	STRING_VAL       { NodeData data; data.lit = $1; $$ = ast_new_node(STR_VAL_NODE);  ast_set_data($$, data); }
|	ID               { load_var_to_token($1, &$$); }
;
%%
#include <stdio.h>
#include "scanner.h"

void lexer_init(void);
void lexer_deinit(void);

int main(void) {
	lexer_init();
	parser_init();

	if(yyparse() == 0){
		printf("PARSE SUCCESSFUL!\n");
		Scope* scope = scope_manager_search_by_name(scope_manager, "xxxx");
		VarTable* vartable = scope_get_vartable(scope);
		print_dot(root, vartable);
	}
	else{
		printf("PARSE FAILED!\n");
	}

	parser_deinit();
	lexer_deinit();
	yylex_destroy();

	return 0;
}

void yyerror(char const *s) {
	fprintf(stderr, "%s\n", s);
}

void parser_init(void){
	scope_manager = scope_manager_new();
}

void parser_deinit(void){
	scope_manager_destroy(&scope_manager);
	ast_free(root);
	root = NULL;
}

void enter_scope(void){
	scope_manager_enter(scope_manager);
}

void exit_scope(void){
	scope_manager_exit(scope_manager);
}

void add_var_declaration(char* name, enum Type type){
	Scope* scope = scope_manager_get_current_scope(scope_manager);
	if(scope_add(scope, name, yylineno, type) == -1){
		VarTable* vartable = scope_get_vartable(scope);
		int idx = scope_search_by_name(scope, name);
		printf("SEMANTIC ERROR (%d): variable %s has been declared already at line %d\n", yylineno, name, get_line(vartable, idx));
	}
}

int check_var_declaration(char* name){
	if(scope_manager_search_by_name(scope_manager, name) == NULL){
		printf("SEMANTIC ERROR (%d): variable %s hasn't been declared yet\n", yylineno, name);
		return -1;
	}
	return 0;
}

int check_assignment(char* name, enum Type type){
	if(check_var_declaration(name) == -1){
		return -1;
	}

	Scope* scope = scope_manager_search_by_name(scope_manager, name);
	VarTable* table = scope_get_vartable(scope);
	int idx = lookup_var(table, name, scope_get_level(scope));
	if(idx == -1){
		fprintf(stderr, "check_assignment: idx error\n");
		return -1;
	}

	/*
	enum Type vartype = get_type(table, idx);
	fprintf(
		stderr, "%s == %s?\n",
		type_name(vartype),
		type_name(type)
	);
	if(vartype == type){
		fprintf(stderr, "%s matches the assignment type!\n", name);
	}
	else{
		fprintf(stderr, "%s DOESN'T match the assignment type!\n", name);
	}
	*/

	return 0;
}

int check_if_repeat_expr(enum Type type){
	int ret = type != TYPE_BOO;
	if(ret != 0){
		fprintf(stderr, "SEMANTIC ERROR (%d): conditional expression in IF/REPEAT", yylineno);
		fprintf(stderr, " is '%s' instead of 'bool'\n", type_name(type));
	}
	return ret;
}

void unary_operation(
	Literal* op1,
	Literal* res,
	int (*operation)(void*, void*),
	const char* operator)
{
	int fail = operation(op1, res);
	if(fail){
		fprintf(stderr, "SEMANTIC ERROR (%d):", yylineno);
		fprintf(stderr, "incompatible types for operator '%s': ", operator);
		fprintf(stderr, "LHS is '%s'", type_name(res->type));
		fprintf(stderr, " and RHS is '%s'\n", type_name(op1->type));
	}
}

void binary_operation(
	AST* op1,
	AST* op2,
	AST** res,
	int (*operation)(Literal*, Literal*, BinaryOperationData*),
	const char* operator_symb,
	NodeKind nodekind)
{
	Literal* l1 = ast_get_literal(op1);
	Literal* l2 = ast_get_literal(op2);
	if(l1 == NULL || l2 == NULL || *res == NULL){
		*res = NULL;
		fprintf(stdout, "Got nil when performing operation %s\n", operator_symb);
		return ;
	}

	BinaryOperationData meta;
	int fail = operation(l1, l2, &meta);
	if(fail){
		fprintf(stderr, "SEMANTIC ERROR (%d):", yylineno);
		fprintf(stderr, "incompatible types for operator '%s': ", operator_symb);
		fprintf(stderr, "LHS is '%s'", type_name(ast_get_type(op1)));
		fprintf(stderr, " and RHS is '%s'\n", type_name(ast_get_type(op2)));
	}

	AST* leftnode = op1;
	AST* rightnode = op2;
	NodeKind kind;

	switch(meta.left){
		case CONV_B2I:
			kind = B2I_NODE;
			break;
		case CONV_B2R:
			kind = B2R_NODE;
			break;
		case CONV_B2S:
			kind = B2S_NODE;
			break;
		case CONV_I2R:
			kind = I2R_NODE;
			break;
		case CONV_I2S:
			kind = I2S_NODE;
			break;
		case CONV_R2S:
			kind = R2S_NODE;
			break;
		default:
			kind = NOCONV_NODE;
	}
	if(kind != NOCONV_NODE){
		leftnode = ast_new_subtree(kind, op1, NULL);
		ast_set_data(leftnode, ast_get_data(op1));
	}

	switch(meta.right){
		case CONV_B2I:
			kind = B2I_NODE;
			break;
		case CONV_B2R:
			kind = B2R_NODE;
			break;
		case CONV_B2S:
			kind = B2S_NODE;
			break;
		case CONV_I2R:
			kind = I2R_NODE;
			break;
		case CONV_I2S:
			kind = I2S_NODE;
			break;
		case CONV_R2S:
			kind = R2S_NODE;
			break;
		default:
			kind = NOCONV_NODE;
	}
	if(kind != NOCONV_NODE){
		rightnode = ast_new_subtree(kind, op2, NULL);
		ast_set_data(rightnode, ast_get_data(op2));
	}

	NodeData resdata;
	resdata.lit.type = meta.type;
	*res = ast_new_subtree(nodekind, leftnode, rightnode, NULL);
	ast_set_data(*res, resdata);
}

void save_token_to_var(Literal* props, char* varname){
	if(check_var_declaration(varname) == -1){
		return ;
	}
	if(check_assignment(varname, props->type) == -1){
		return ;
	}

	/*
	switch($3.type){
		case TYPE_INT:
			printf("Assignment: var '%s' = %d\n", varname, props->value.i);
			break;
		case TYPE_FLT:
			printf("Assignment: var '%s' = %f\n", varname, props->value.f);
			break;
		case TYPE_BOO:
			printf("Assignment: var '%s' = %d\n", varname, props->value.b);
			break;
		case TYPE_STR:
			printf("Assignment: var '%s' = %s\n", varname, props->value.s);
			break;
	}
	*/

	Scope* scope = scope_manager_search_by_name(scope_manager, varname);
	VarTable* table = scope_get_vartable(scope);
	int idx = lookup_var(table, varname, scope_get_level(scope));
	set_val(table, idx, props->value);
}

void load_var_to_token(char* varname, AST** res){
	if(check_var_declaration(varname) == -1){
		return ;
	}
	Scope* scope = scope_manager_search_by_name(scope_manager, varname);
	VarTable* table = scope_get_vartable(scope);
	int idx = lookup_var(table, varname, scope_get_level(scope));
	Variable var = get_var(table, idx);
	NodeData data = {.var = var};
	*res = ast_new_node(VAR_USE_NODE);
	ast_set_data(*res, data);
}

void parser_get_var(char* varname, Variable* res){
	if(check_var_declaration(varname) == -1){
		return ;
	}
	Scope* scope = scope_manager_search_by_name(scope_manager, varname);
	VarTable* table = scope_get_vartable(scope);
	int idx = lookup_var(table, varname, scope_get_level(scope));
	Variable var = get_var(table, idx);
	*res = var;
}
