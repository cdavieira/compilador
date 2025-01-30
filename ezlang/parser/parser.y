%define parse.error verbose
%define parse.lac full

%{
#include <stdio.h>
#include "Literal.h"
#include "tables.h"
#include "scope.h"

extern char* yytext;
extern int yylineno;
static ScopeManager* scope_manager;

int yylex(void);
void yyerror(char const *s);

void parser_init(void);
void parser_deinit(void);
void enter_scope(void);
void exit_scope(void);
void do_operation(
	Literal* op1,
	Literal* op2,
	Literal* res,
	Operator* operator);
void add_var_declaration(char* name, enum Type type);
int check_var_declaration(char* name);
int check_assignment(char* name, enum Type type);
int check_if_repeat_expr(enum Type type);
void save_token_to_var(Literal* props, char* varname);
void load_var_to_token(char* varname, Literal* res);
%}

%code requires {
#include "Literal.h"
union TokenData {
	Operator o;
	Literal l;
	char* str;
};
}

%define api.value.type {union TokenData}

%token PROGRAM SEMI VAR INT BOOL REAL STRING BBEGIN READ WRITE IF THEN ELSE REPEAT UNTIL END LPAR RPAR
%token <o> ASSIGN EQ LT TIMES PLUS OVER MINUS
%token <l> FLOAT_VAL INT_VAL STRING_VAL TRUE FALSE
%nterm <l> type_spec expr
%token <str> ID

%left EQ LT
%left PLUS MINUS
%left TIMES OVER
%%

program: PROGRAM ID SEMI vars_sect stmt_sect
;

vars_sect:
	VAR opt_var_decl
;

opt_var_decl:
	%empty
|	var_decl_list
;

var_decl_list:
	var_decl_list var_decl
|	var_decl
;

var_decl:
	type_spec ID SEMI { add_var_declaration($2, $1.type); }
;

type_spec:
	BOOL   { $$.type = TYPE_BOO; }
|	INT    { $$.type = TYPE_INT; }
|	REAL   { $$.type = TYPE_FLT; }
|	STRING { $$.type = TYPE_STR; }
;

stmt_sect:
	BBEGIN stmt_list END
|	BBEGIN END
;

stmt_list:
	stmt_list stmt
|	stmt
;

stmt:
	if_stmt
|	repeat_stmt
|	assign_stmt
|	read_stmt
|	write_stmt
;

if_stmt:
	IF expr { check_if_repeat_expr($2.type); } THEN stmt_list if_stmt_pt2
;

if_stmt_pt2:
	END
|	ELSE stmt_list END
;

repeat_stmt:
	REPEAT stmt_list UNTIL expr { check_if_repeat_expr($4.type); }
;

assign_stmt:
	ID ASSIGN expr { save_token_to_var(&$3, $1); } SEMI
;

read_stmt:
	READ ID SEMI
;

write_stmt:
	WRITE expr SEMI { 
		switch($2.type){
			case TYPE_INT:
				printf("write writes %d\n", $2.value.i);
				break;
			case TYPE_FLT:
				printf("write writes %f\n", $2.value.f);
				break;
			case TYPE_BOO:
				printf("write writes %d\n", $2.value.b);
				break;
			case TYPE_STR:
				printf("write writes %s\n", $2.value.s);
				break;
		}
	}
;

//WARNING: STRING_VAL IS BEING PASSED BY REFERENCE, NOT BY COPY
expr:
	expr LT expr     { do_operation(&$1, &$3, &$$, &$2); }
|	expr EQ expr     { do_operation(&$1, &$3, &$$, &$2); }
|	expr PLUS expr   { do_operation(&$1, &$3, &$$, &$2); }
|	expr MINUS expr  { do_operation(&$1, &$3, &$$, &$2); }
|	expr TIMES expr  { do_operation(&$1, &$3, &$$, &$2); }
|	expr OVER expr   { do_operation(&$1, &$3, &$$, &$2); }
|	LPAR expr RPAR   { $$ = $2; }
|	TRUE             { $$.type = $1.type; $$.value.b = $1.value.b; }
|	FALSE            { $$.type = $1.type; $$.value.b = $1.value.b; }
|	INT_VAL          { $$.type = $1.type; $$.value.i = $1.value.i; }
|	FLOAT_VAL        { $$.type = $1.type; $$.value.f = $1.value.f; }
|	STRING_VAL       { $$.type = $1.type; $$.value.s = $1.value.s; }
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
		printf("SEMANTIC ERROR (XX): variable %s has been declared already at line %d\n", name, get_line(vartable, idx));
	}
}

int check_var_declaration(char* name){
	if(scope_manager_search_by_name(scope_manager, name) == NULL){
		printf("SEMANTIC ERROR (XX): variable %s hasn't been declared yet\n", name);
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
		literal_get_type_name(vartype),
		literal_get_type_name(type)
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
		fprintf(stderr, " is '%s' instead of 'bool'\n", literal_get_type_name(type));
	}
	return ret;
}

void do_operation(
	Literal* op1,
	Literal* op2,
	Literal* res,
	Operator* operator)
{
	int fail = 0;
	switch(operator->type){
		case OPERATOR_SUM:
			fail = literal_sum(op1, op2, res);
			break;
		case OPERATOR_MINUS:
			fail = literal_minus(op1, op2, res);
			break;
		case OPERATOR_MUL:
			fail = literal_mult(op1, op2, res);
			break;
		case OPERATOR_DIV:
			fail = literal_div(op1, op2, res);
			break;
		case OPERATOR_LT:
			fail = literal_lt(op1, op2, res);
			break;
		case OPERATOR_EQ:
			fail = literal_eq(op1, op2, res);
			break;
		case OPERATOR_ASSIGN:
			fail = literal_assign(op1, res);
			break;
		default:
			fprintf(stderr, "Unknown Operation %s\n", operator->name);
	}
	if(fail){
		fprintf(stderr, "SEMANTIC ERROR (%d):", yylineno);
		fprintf(stderr, "incompatible types for operator '%s': ", operator->name);
		fprintf(stderr, "LHS is '%s'", literal_get_type_name(op1->type));
		fprintf(stderr, " and RHS is '%s'\n", literal_get_type_name(op2->type));
	}
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

void load_var_to_token(char* varname, Literal* res){
	if(check_var_declaration(varname) == -1){
		return ;
	}
	Scope* scope = scope_manager_search_by_name(scope_manager, varname);
	VarTable* table = scope_get_vartable(scope);
	int idx = lookup_var(table, varname, scope_get_level(scope));
	res->type = get_type(table, idx);
	res->value = get_val(table, idx);
}
