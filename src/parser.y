%define parse.error verbose
%define parse.lac full

%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "parser/typesys.h"
#include "parser/ScopeManager.h"
#include "parser/VarTable.h"
#include "parser/FuncTable.h"
#include "types/Literal.h"
#include "types/AST.h"
#include "utils/Vector.h"
#include "utils/Stack.h"

extern int yylineno;

//counts how many parameters have been read when parsing a function declaration or function call
int params_count;

//1 if the parser is currently handling a function call
int fcall_active;

//name and return type of the current function being parsed
char* funcname;
enum Type retvartype;

//type of the current variable or group of variables when parsing variable declaration or function parameter declaration
enum Type vartype;

//handles entering/exiting scopes and keeping track of the current scope in use
ScopeManager* scope_manager;

//stores all functions declared/defined so far
FuncTable* functable;

//stores all arguments passed to a function during a function call
Vector* funcargs;

//the Abstract Syntax Tree for this parser
AST* root;

//workaround to deal with the problem of creating nested blocks and adding them to the AST
int* ast_block;
Stack* ast_block_history;

//count of how many variables and funcparams have been created so far in this program.
unsigned var_count;

/* bison internals */
int yylex(void);
void yyerror(char const *s);

/* parser state (global variables) related */
void parser_init(void);
void parser_deinit(void);
void parser_print(void);
void parser_scope_enter(void);
void parser_scope_exit(void);
void fcall_start(char* name, int check);
void block_enter(void);
void block_exit(void);
void function_pre_body(char* name, enum Type type);
void funcparams_begin(void);

/* adding/creating variables/functions */
AST* add_var_declaration(char* name, enum Type type, enum Qualifier qualifier, AST* init);
int add_function_declaration(char* name, enum Type type, int definition);

/* checking variable/function existence, expression validity */
void check_var_declaration(char* name);
void check_function_declaration(char* name);
void check_function_call(char* name);
void check_function_return(enum Type rettype);
void check_condition(enum Type type);
void check_assignment(char* name, AST* expr);

/* variable/function resolution by parameters */
Variable* get_var(char* name);

/* AST misc */
AST* ast_manager_add_declaration(AST* program, AST* declaration, int create_declarations);
AST* ast_manager_add_var_decl(AST* varlist, AST* var, int create_varlist);
AST* ast_manager_add_param_decl(AST* paramlist, AST* param, int create_paramlist);
AST* ast_manager_add_to_block(AST* block, AST* item);
AST* ast_manager_add_conv(AST* data, Conversion conv);
AST* ast_manager_from_literal(Literal* lit);
AST* ast_manager_from_id(char* name);
AST* ast_manager_assign_stmt(char* name, AST* expr);
AST* ast_manager_assign_stmt_arr(char* name, AST* idx, AST* val);
AST* ast_manager_if_stmt(AST* expr, AST* ifblock, AST* elseblock);
AST* ast_manager_while_stmt(AST* expr, AST* block);
AST* ast_manager_add_funcret(AST* expr);
AST* ast_manager_fcall(char* funcname);
AST* ast_manager_add_function(char *name, enum Type type, AST* params, AST* fblock, int function_definition);
AST* ast_manager_binop(AST* op1, AST* op2, NodeKind nodekind, TypeData (*operation)(enum Type, enum Type));

%}

/* Make 'union Token' definition available in 'parser.tab.h' */
%code requires {
#include "types/Literal.h"
#include "types/AST.h"
union Token {
	Literal l;
	enum Type t;
	char* str;
	AST* ast;
};
}

/* https://www.gnu.org/software/bison/manual/html_node/Union-Decl.html */
/* https://www.gnu.org/software/bison/manual/html_node/Token-Values.html */
/* https://www.gnu.org/software/bison/manual/html_node/Type-Generation.html */
%define api.value.type { union Token }

/* variable related */
%token <str> ID
%token ASSIGN

/* basic types */
%token VOID
%token CHAR 
%token FLOAT 
%token INT 
%token LONG
%token UNSIGNED 
%token DOUBLE 
%token SIGNED 
%token SHORT 

/* pointer type/function type related */
%token STAR
%token RETURN 
%token RESTRICT 
%token INLINE 
%token AMP 

/* special types */
%token UNION
%token TYPEDEF 
%token STRUCT 
%token ENUM 

/* values */
%token <l> FLOATVAL
%token <l> INTVAL
%token <l> QUOTE
%token <l> STRING
%token <l> CHR

/* arithmetic operations */
%token MODULUS
%token PLUSAUTO 
%token PLUS1 
%token PLUS 
%token MINUSAUTO 
%token MINUS1 
%token MINUS 
%token DIVISION 

/* logical operations */
%token OR
%token NOTEQ 
%token NOT 
%token LTEQ 
%token LT 
%token GTEQ 
%token GT 
%token EQ 
%token AND 

/* bitwise operations */
%token RSHIFTAUTO
%token RSHIFT 
%token LSHIFTAUTO 
%token LSHIFT 
%token BITXORAUTO 
%token BITXOR 
%token BITORAUTO 
%token BITOR 
%token BITNOTAUTO 
%token BITNOT 
%token BITANDAUTO 

/* type qualifier */
%token VOLATILE
%token STATIC 
%token REGISTER 
%token EXTERN 
%token CONST 
%token AUTO 

/* if else */
%token ELSE
%token IF 

/* switch */
%token SWITCH
%token DEFAULT 
%token CASE 

/* loops */
%token WHILE
%token FOR 
%token DO 
%token CONTINUE 
%token BREAK 

/* goto hell yeah */
%token GOTO

/* misc */
%token SIZEOF
%token RPAR 
%token LPAR 
%token RCURLY 
%token RBRACKET 
%token DELI 
%token COMMA 
%token LCURLY 
%token LBRACKET 
%token <str> SCANF
%token <str> PRINTF

%left GT
%left EQ
%left NOTEQ
%left LT
%left OR
%left AND
%left MINUS
%left PLUS
%left DIVISION
%left STAR
%left MODULUS

%nterm <str> fcaller
%nterm <t> type_specifier
%nterm <ast> program
%nterm <ast> declaration
%nterm <ast> declarations
%nterm <ast> declaration_var
%nterm <ast> declarators
%nterm <ast> declarator
%nterm <ast> declarator_basic
%nterm <ast> declarator_array
%nterm <ast> declaration_func
%nterm <ast> func_paramlist
%nterm <ast> func_params
%nterm <ast> func_param 
%nterm <ast> func_param_basic
%nterm <ast> func_param_array
%nterm <ast> func_param_fp
%nterm <ast> block
%nterm <ast> block_stmts
%nterm <ast> block_stmt
%nterm <ast> expr
%nterm <ast> stmt
%nterm <ast> stmt_assign
%nterm <ast> stmt_if
%nterm <ast> stmt_while
%nterm <ast> stmt_jump
%nterm <ast> stmt_fcall
%nterm <ast> fcall
%%

program:
  declarations { root = $1; }
;

declarations:
  declaration              { $$ = ast_manager_add_declaration($$, $1, 1); }
| declaration declarations { $$ = ast_manager_add_declaration($2, $1, 0); }
;


/* Declaração de variáveis (global/scope) */

declaration:
  declaration_var  { $$ = $1; }
| declaration_func { $$ = $1; }
;

declaration_var:
  type_specifier declarators DELI { $$ = $2; }
;

declarators:
  declarator                    { $$ = ast_manager_add_var_decl($$, $1, 1); }
| declarator COMMA declarators  { $$ = ast_manager_add_var_decl($3, $1, 0); }
;

declarator:
  declarator_basic { $$ = $1; }
| declarator_array { $$ = $1; }
;

declarator_basic:
  ID             { $$ = add_var_declaration($1, vartype, QUALIFIER_BASIC, NULL);  }
| ID ASSIGN expr { $$ = add_var_declaration($1, vartype, QUALIFIER_BASIC, $3);  }
;

declarator_array:
  ID LBRACKET INTVAL RBRACKET                            { $$ = add_var_declaration($1, vartype,  $3.value.i, NULL);  }
| ID LBRACKET INTVAL RBRACKET ASSIGN LCURLY RCURLY       { $$ = add_var_declaration($1, vartype,  $3.value.i, NULL);  }
| ID LBRACKET INTVAL RBRACKET ASSIGN LCURLY exprs RCURLY { $$ = add_var_declaration($1, vartype,  $3.value.i, NULL);  }
| ID LBRACKET INTVAL RBRACKET ASSIGN STRING              { $$ = add_var_declaration($1, TYPE_STR, $3.value.i, NULL); }
;


/* Declaração/definição de funções */

declaration_func:
  type_specifier ID func_paramlist DELI          { funcname = $2; retvartype = $1; $$ = ast_manager_add_function($2, $1, $3, NULL, 0); }
| type_specifier ID func_paramlist LCURLY RCURLY { funcname = $2; retvartype = $1; $$ = ast_manager_add_function($2, $1, $3, NULL, 1); }
| type_specifier ID func_paramlist LCURLY { function_pre_body($2, $1); } block_stmts RCURLY { $$ = ast_manager_add_function($2, $1, $3, $6, 1); }
;

func_paramlist:
  LPAR { funcparams_begin(); } RPAR             { $$ = NULL; }
| LPAR { funcparams_begin(); } func_params RPAR { $$ = $3; }
;

func_params:
  func_param                   { $$ = ast_manager_add_param_decl($$, $1, 1); }
| func_param COMMA func_params { $$ = ast_manager_add_param_decl($3, $1, 0); }
;

func_param:
  func_param_basic { $$ = $1; }
| func_param_array { $$ = $1; }
| func_param_fp    { $$ = $1; }
;

func_param_basic:
  type_specifier    { $$ = NULL; }
| type_specifier ID { $$ = add_var_declaration($2, vartype, QUALIFIER_BASIC, NULL); }
;

func_param_array:
  type_specifier ID LBRACKET INTVAL RBRACKET { $$ = add_var_declaration($2, vartype, $4.value.i, NULL); }
;

func_param_fp:
  type_specifier LPAR STAR RPAR LPAR func_params RPAR    { $$ = NULL; } /* int (*)(...) */
| type_specifier LPAR STAR ID RPAR LPAR func_params RPAR { $$ = NULL; } /* int (*varname)(...) */
;

block:
  LCURLY RCURLY { $$ = NULL; }
| LCURLY { block_enter(); } block_stmts RCURLY { block_exit(); $$ = $3; }
;

block_stmts:
  block_stmt             { $$ = $1 ? ast_manager_add_to_block($$, $1) : NULL ; }
| block_stmts block_stmt { $$ = ast_manager_add_to_block($1, $2); }
;

block_stmt:
  declaration_var { $$ = $1; }
| stmt  { $$ = $1; }
| block { $$ = $1; }
;

stmt:
  stmt_assign { $$ = $1; }
| stmt_if     { $$ = $1; }
| stmt_while  { $$ = $1; }
| stmt_jump   { $$ = $1; }
| stmt_fcall  { $$ = $1; }
;

stmt_jump:
  CONTINUE DELI      { $$ = NULL; }
| BREAK DELI         { $$ = NULL; }
| RETURN DELI        { $$ = ast_manager_add_funcret(NULL); }
| RETURN expr DELI   { $$ = ast_manager_add_funcret($2); }
;

stmt_while:
  WHILE LPAR expr RPAR block { $$ = ast_manager_while_stmt($3, $5); }
;

stmt_if:
  IF LPAR expr RPAR block              { $$ = ast_manager_if_stmt($3, $5, NULL); }
| IF LPAR expr RPAR block ELSE block   { $$ = ast_manager_if_stmt($3, $5, $7); }
| IF LPAR expr RPAR block ELSE stmt_if { $$ = ast_manager_if_stmt($3, $5, $7); }
;

stmt_assign:
  ID ASSIGN   expr DELI                      { $$ = ast_manager_assign_stmt($1, $3); }
| ID LBRACKET expr RBRACKET ASSIGN expr DELI { $$ = ast_manager_assign_stmt_arr($1, $3, $6); }
;

stmt_fcall:
  fcall DELI { $$ = $1; }
;

fcall:
  fcaller LPAR RPAR         { $$ = ast_manager_fcall($1); }
| fcaller LPAR exprs RPAR   { $$ = ast_manager_fcall($1); }
;

expr:
  LPAR expr RPAR            { $$ = $2 ; }
| ID                        { check_var_declaration($1); $$ = ast_manager_from_id($1); }
| INTVAL                    { $$ = ast_manager_from_literal(&$1); }
| FLOATVAL                  { $$ = ast_manager_from_literal(&$1); }
| STRING                    { $$ = ast_manager_from_literal(&$1); }
| CHR                       { $$ = ast_manager_from_literal(&$1); }
| expr PLUS expr            { $$ = ast_manager_binop($1, $3, NODE_PLUS, typesys_sum);  }
| expr MINUS expr           { $$ = ast_manager_binop($1, $3, NODE_MINUS, typesys_sub); }
| expr STAR expr            { $$ = ast_manager_binop($1, $3, NODE_TIMES, typesys_mul); }
| expr DIVISION expr        { $$ = ast_manager_binop($1, $3, NODE_OVER, typesys_div);  }
| expr MODULUS expr         { $$ = ast_manager_binop($1, $3, NODE_MOD, typesys_mod);   }
| expr OR expr              { $$ = ast_manager_binop($1, $3, NODE_OR, typesys_or);   }
| expr AND expr             { $$ = ast_manager_binop($1, $3, NODE_AND, typesys_and); }
| expr EQ expr              { $$ = ast_manager_binop($1, $3, NODE_EQ, typesys_eq);   }
| expr NOTEQ expr           { $$ = ast_manager_binop($1, $3, NODE_NE, typesys_ne);   }
| expr LT expr              { $$ = ast_manager_binop($1, $3, NODE_LT, typesys_lt);   }
| expr GT expr              { $$ = ast_manager_binop($1, $3, NODE_GT, typesys_gt);   }
| MINUS INTVAL              { $$ = ast_manager_from_literal(&$2); }
| PLUS INTVAL               { $$ = ast_manager_from_literal(&$2); }
| MINUS FLOATVAL            { $$ = ast_manager_from_literal(&$2); }
| PLUS FLOATVAL             { $$ = ast_manager_from_literal(&$2); }
| fcall                     { $$ = $1; }
| AMP ID                    { check_var_declaration($2); $$ = NULL; }
| ID LBRACKET expr RBRACKET { check_var_declaration($1); $$ = NULL; } //TODO: improve this
;

fcaller:
  ID      { fcall_start($1, 1); $$ = $1; }
| PRINTF  { fcall_start($1, 0); $$ = $1; }
| SCANF   { fcall_start($1, 0); $$ = $1; }
;

exprs:
  expr { if(fcall_active == 1){ vector_append(funcargs, $1) ; }; }
| expr { if(fcall_active == 1){ vector_append(funcargs, $1) ; }; } COMMA exprs
;

type_specifier:
  INT     { vartype = TYPE_INT;  $$ = vartype; }
| CHAR    { vartype = TYPE_CHAR; $$ = vartype; }
| FLOAT   { vartype = TYPE_FLT;  $$ = vartype; }
| VOID    { vartype = TYPE_VOID; $$ = vartype; }
;

%%

/* parser state (global variables) related */
void parser_init(void){
	functable = func_table_new();
	scope_manager = scope_manager_new();
	func_table_add(functable, "printf", scope_manager_get_current_scope(scope_manager), TYPE_VOID, 10, 1);
	func_table_add(functable, "scanf", scope_manager_get_current_scope(scope_manager), TYPE_VOID, 10, 1);
	funcargs = vector_new(12);
	ast_block = calloc(1, sizeof(int));
	ast_block_history = stack_new(12);
}

void parser_deinit(void){
	scope_manager_destroy(&scope_manager);
	func_table_destroy(&functable);
	vector_destroy(&funcargs, NULL);
	ast_free(root);
	root = NULL;
	free(ast_block);
	stack_destroy(&ast_block_history, free);
}

void parser_print(void){
#ifdef DEBUG_PARSER
	scope_manager_print(scope_manager);
	func_table_print(functable);
#endif
#ifdef DEBUG_AST
	ast_print(root);
	FILE* fpout = fopen("tmp.dot", "w");
	ast_export_dot(root, fpout);
	fclose(fpout);
#endif
}

void parser_scope_enter(void){
	scope_manager_enter(scope_manager);
}

void parser_scope_exit(void){
	scope_manager_exit(scope_manager);
}

void fcall_start(char* name, int check){
	if(check){
		check_function_declaration(name);
	}
	vector_clear(funcargs);
	fcall_active = 1;
}

void block_enter(void){
	stack_push(ast_block_history, ast_block);
	ast_block = calloc(1, sizeof(int));
	parser_scope_enter();
}

void block_exit(void){
	free(ast_block);
	ast_block = stack_pop(ast_block_history);
	parser_scope_exit();
}

void function_pre_body(char* name, enum Type type){
	add_function_declaration(name, type, 0);
	retvartype = type;
	funcname = name;
	*ast_block = 0;
}

void funcparams_begin(void){
	params_count = 0;
	parser_scope_enter();
}



/* adding/creating variables/functions */
AST* add_var_declaration(char* name, enum Type type, enum Qualifier qualifier, AST* init){
	if(type == TYPE_VOID){
		printf("SEMANTIC ERROR (%d): variable %s has been declared with type VOID\n", yylineno, name);
		exit(1);
	}

	Scope* scope = scope_manager_get_current_scope(scope_manager);
	int idx = scope_add(scope, name, yylineno, type, qualifier, var_count++);
	int var_exists = idx == -1;
	Variable* var = var_exists ? scope_search_by_name(scope, name) : scope_get_var(scope, idx);

	if(var_exists){
		printf("SEMANTIC ERROR (%d): variable %s has been declared already\n", var->line, name);
		exit(1);
	}
#ifdef DEBUG_SCOPE
	printf("Variable %s added to scope %d\n", name, scope_get_id(scope));
#endif

	AST* ast = ast_new_node(NODE_VAR_DECL);
	NodeData data;
	data.var.var = *var;
	data.var.scope = scope;
	ast_set_data(ast, data);

	//making sure we are dealing with either a basic type or with an array type
	assert((qualifier == QUALIFIER_BASIC) || (qualifier != QUALIFIER_POINTER));

	if(qualifier == QUALIFIER_BASIC){
		if(init != NULL){
			ast_add_child(ast, init);
		}
	}
	else{ //array
		AST* arrayval = ast_new_node(NODE_ARRAY_VAL);
		ast_set_data(arrayval, data);
		ast_add_child(ast, arrayval);
	}

	return ast;
}

int add_function_declaration(char* name, enum Type type, int definition){
	Scope* scope = scope_manager_get_current_scope(scope_manager);
	if(func_table_add(functable, name, scope, type, params_count, definition) == -1){
		if(definition){
			printf("SEMANTIC ERROR (%d): function %s has been defined already\n", yylineno, name);
			exit(1);
		}

		printf("SEMANTIC WARNING (%d): function %s has been declared already\n", yylineno, name);
	}
	return 0;
}



/* checking variable/function existence, expression validity */
void check_var_declaration(char* name){
	if(scope_manager_search_by_name(scope_manager, name) == NULL){
		printf("SEMANTIC ERROR (%d): variable %s hasn't been declared yet\n", yylineno, name);
		exit(1);
	}
}

void check_function_declaration(char* name){
	int missing_function_declared = func_table_search(functable, name) == NULL;
	if(missing_function_declared){
		printf("SEMANTIC ERROR (%d): function %s hasn't been declared yet\n", yylineno, name);
	}
}

void check_function_call(char* name){
#define STRING_EQ(name1, name2) (strcmp(name1, name2) == 0)
	if(STRING_EQ(name, "printf") || STRING_EQ(name, "scanf")){
		return ;
	}
#undef STRING_EQ
	Function* f = func_table_search(functable, name);
	if(f == NULL){
		fprintf(stdout, "SEMANTIC ERROR (%d): function %s does not exist\n", yylineno, name);
		exit(1);
	}

	// if(func_is_defined(f) == 0){
	// 	fprintf(stdout, "SEMANTIC ERROR (%d): function %s isn't defined\n", yylineno, name);
	// 	exit(1);
	// }

	Scope* scope = func_get_scope(f);
	VarTable* vt = scope_get_vartable(scope);
	const int nparams = func_get_nparams(f);
	const int nargs = vector_get_size(funcargs);
	Variable* param = NULL;
	AST* arg = NULL;
	
	if(nargs != nparams){
		fprintf(stdout, "TYPE ERROR (%d): function %s called with insufficient number of arguments\n", yylineno, name);
		exit(1);
	}

	for(int i=0; i<nparams; i++){
		param = vartable_idx(vt, i);
		arg = vector_get_item(funcargs, i);
		if(ast_get_type(arg) != param->type){
			fprintf(stdout, "TYPE ERROR (%d): function %s called with argument of wrong type: '%s' should be '%s'\n", yylineno, name, type_name(ast_get_type(arg)), type_name(param->type));
			exit(1);
		}
	}
}

void check_function_return(enum Type rettype){
	if(retvartype != rettype){
		fprintf(stdout, "TYPE ERROR (%d): function %s with wrong return type: '%s' should be '%s'\n", yylineno, funcname, type_name(rettype), type_name(retvartype));
		exit(1);
	}
}

void check_condition(enum Type type){
	switch(type){
		case TYPE_INT:
			break;
		case TYPE_VOID:
		case TYPE_FLT:
		case TYPE_STR:
		case TYPE_CHAR:
		default:
			fprintf(stdout, "SEMANTIC ERROR (%d): conditional expression in IF/WHILE", yylineno);
			fprintf(stdout, " is '%s' instead of 'int'\n", type_name(type));
			exit(1);
	}
}

void check_assignment(char* name, AST* expr){
	Variable* var = get_var(name);
	TypeData data = typesys_assign(var->type, ast_get_type(expr));
	if(data.type == TYPE_VOID){
		printf("SEMANTIC ERROR (%d): ", yylineno);
		printf("incompatible types for operator '=', ");
		printf("LHS is '%s' and RHS is '%s'.\n", type_name(var->type), type_name(ast_get_type(expr)));
	}
}



/* variable/function resolution by parameters */
Variable* get_var(char* name){
	Scope* scope = scope_manager_search_by_name(scope_manager, name);
  	if(scope == NULL){
		return NULL;
  	}
	return scope_search_by_name(scope, name);
}



/* AST misc */
AST* ast_manager_add_declaration(
	AST* program,
	AST* declaration,
	int create_declarations)
{
	if(create_declarations){
		program = ast_new_node(NODE_PROGRAM);
	}
	ast_add_child(program, declaration); 
	return program;
}

AST* ast_manager_add_var_decl(AST* varlist, AST* var, int create_varlist){
	if(create_varlist == 1){
		varlist = ast_new_node(NODE_VAR_LIST);
	}
	ast_add_child(varlist, var); 
	return varlist;
}

AST* ast_manager_add_param_decl(AST* paramlist, AST* param, int create_paramlist){
	if(create_paramlist == 1){
		paramlist = ast_new_node(NODE_FUNC_PARAMLIST);
	}
	params_count++;
	if(param){
		ast_add_child(paramlist, param); 
	}
	return paramlist;
}

AST* ast_manager_add_to_block(AST* block, AST* item){
	if(*ast_block == 0){
		block = ast_new_node(NODE_BLOCK);
		*ast_block = 1;
	}
	if(item){
		ast_add_child(block, item); 
	}
	return block;
}

AST* ast_manager_add_conv(AST* raw, Conversion conv){
	AST* converted = raw;
	NodeKind kind;
	NodeData data;
	switch(conv){
		case CONV_I2F:
			kind = NODE_I2F;
			data.lit.type = TYPE_FLT;
			break;
		case CONV_I2C:
			kind = NODE_I2C;
			data.lit.type = TYPE_CHAR;
			break;
		case CONV_C2I:
			kind = NODE_C2I;
			data.lit.type = TYPE_INT;
			break;
		case CONV_C2F:
			kind = NODE_C2F;
			data.lit.type = TYPE_FLT;
			break;
		case CONV_F2I:
			kind = NODE_F2I;
			data.lit.type = TYPE_INT;
			break;
		case CONV_F2C:
			kind = NODE_F2C;
			data.lit.type = TYPE_CHAR;
			break;
		default:
			kind = NODE_NOCONV;
			data.lit.type = ast_get_type(raw);
	}
	if(kind != NODE_NOCONV){
		converted = ast_new_subtree(kind, raw, NULL);
		ast_set_data(converted, data);
	}
	return converted;
}

AST* ast_manager_from_literal(Literal* lit){
	AST* node = NULL;
	NodeData data;
	data.lit = *lit;
	switch(lit->type){
		case TYPE_INT:
			node = ast_new_node(NODE_INT_VAL);
			break;
		case TYPE_FLT:
			node = ast_new_node(NODE_FLT_VAL);
			break;
		case TYPE_CHAR:
			node = ast_new_node(NODE_CHR_VAL);
			break;
		case TYPE_STR:
			node = ast_new_node(NODE_STR_VAL);
			break;
		default:
			node = NULL;
	}
	if(node != NULL){
		ast_set_data(node, data);
	}
	return node;
}

AST* ast_manager_from_id(char* name){
	Variable* var = get_var(name);
	Scope* scope = scope_manager_search_by_name(scope_manager, name);
	if(var == NULL){
		fprintf(stdout, "SEMANTIC ERROR (%d): variable '%s' not found in the current scope\n", yylineno, name);
		return NULL;
	}
	AST* node = ast_new_node(NODE_VAR_USE);
	NodeData data;
	data.var.var = *var;
	data.var.scope = scope;
	ast_set_data(node, data);
	return node;
}

AST* ast_manager_assign_stmt(char* name, AST* expr){
	check_var_declaration(name);
	check_assignment(name, expr); 

	AST* stmt = ast_new_node(NODE_ASSIGN);
	AST* var = ast_manager_from_id(name);
	NodeData data;
	data.lit.type = ast_get_type(var);
	ast_add_child(stmt, var);
	ast_add_child(stmt, expr);
	ast_set_data(stmt, data);
	return stmt;
}


AST* ast_manager_fcall(char* funcname){
	check_function_call(funcname);
	fcall_active = 0; 

	AST* node  = ast_new_node(NODE_FCALL);
	NodeData data;
	Function* f = func_table_search(functable, funcname);
	const int nargs = vector_get_size(funcargs);
	data.func.func = f;
	ast_set_data(node, data);
	for(int i=0; i<nargs; i++){
		ast_add_child(node, vector_get_item(funcargs, i));
	}
	return node;
}

AST* ast_manager_if_stmt(AST* expr, AST* ifblock, AST* elseblock){
	enum Type type = ast_get_type(expr);
	check_condition(type); 

	AST* node  = ast_new_node(NODE_IF);
	ast_add_child(node, expr);
	if(ifblock != NULL){
		ast_add_child(node, ifblock);
	}
	if(elseblock != NULL){
		ast_add_child(node, elseblock);
	}
	return node;
}

AST* ast_manager_while_stmt(AST* expr, AST* block){
	enum Type type = ast_get_type(expr);
	check_condition(type); 

	AST* node  = ast_new_node(NODE_WHILE);
	if(expr != NULL){
		ast_add_child(node, expr);	
	}
	if(block != NULL){
		ast_add_child(node, block);
	}

	return node;
}

AST* ast_manager_assign_stmt_arr(char* name, AST* idx, AST* val){
	check_var_declaration(name);
	check_assignment(name, val); 

	AST* stmt = ast_new_node(NODE_ASSIGN);
	AST* var = ast_manager_from_id(name);
	NodeData data;
	data.lit.type = ast_get_type(var);
	ast_add_child(var, idx);
	ast_add_child(stmt, var);
	ast_add_child(stmt, val);
	ast_set_data(stmt, data);
	return stmt;
}

AST* ast_manager_add_funcret(AST* expr){
	enum Type type = expr == NULL ? TYPE_VOID : ast_get_type(expr);
	check_function_return(type);

	AST* stmt = ast_new_node(NODE_FUNC_RET);
	if(expr != NULL){
		ast_add_child(stmt, expr);
	}
	NodeData data;
	data.lit.type = type;
	ast_set_data(stmt, data);

	return stmt;
}

AST* ast_manager_add_function(char *name, enum Type type, AST* params, AST* fblock, int function_definition){
	add_function_declaration(name, type, function_definition);

	AST* ast = ast_new_node(NODE_FUNC);
	AST* head = params != NULL ? params : ast_new_node(NODE_FUNC_PARAMLIST);
	AST* body = ast_new_node(NODE_FUNC_BODY); 
	if(fblock != NULL){
		ast_add_child(body, fblock);
	}

	NodeData data;
	Function* f = func_table_search(functable, funcname);
	data.func.func = f;
	ast_add_child(ast, head);
	ast_add_child(ast, body);
	ast_set_data(ast, data);

	parser_scope_exit();

	return ast;
}

AST* ast_manager_binop(
  AST* op1, AST* op2, NodeKind nodekind,
  TypeData (*operation)(enum Type, enum Type)
){
	TypeData data = operation(ast_get_type(op1), ast_get_type(op2));
	if(data.type == TYPE_VOID){
		fprintf(stdout, "SEMANTIC ERROR (%d):", yylineno);
		fprintf(stdout, "incompatible types for operator: ");
		fprintf(stdout, "LHS is '%s'", type_name(ast_get_type(op1)));
		fprintf(stdout, " and RHS is '%s'\n", type_name(ast_get_type(op2)));
		exit(1);
	}
	AST* left = ast_manager_add_conv(op1, data.left);
	AST* right = ast_manager_add_conv(op2, data.right);
	AST* res = ast_new_subtree(nodekind, left, right, NULL);
	NodeData ndata;
	ndata.lit.type = data.type;
	ast_set_data(res, ndata);
	return res;
}
