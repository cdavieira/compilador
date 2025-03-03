%define parse.error verbose
%define parse.lac full

%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "typesys.h"
#include "ScopeManager.h"
#include "VarTable.h"
#include "FuncTable.h"
#include "Literal.h"
#include "Vector.h"
#include "Stack.h"
#include "AST.h"

extern int yylineno;
int params_count;
int fcall_active;
char* funcname;
enum Type vartype;
enum Type retvartype;
ScopeManager* scope_manager;
FuncTable* functable;
Vector* funcargs;
AST* root;
int* block_defined;
Stack* block_defined_history;

int yylex(void);
void yyerror(char const *s);

void parser_init(void);
void parser_deinit(void);
void parser_info(void);

void enter_scope(void);
void exit_scope(void);
void assign_to_expr(Literal* src, Literal* expr);

AST* add_var_declaration(char* name, enum Type type, enum Qualifier qualifier, AST* init);
void check_var_declaration(char* name);
Variable* get_var(char* name);
void var_to_expr(char* name, Literal* exp);

int add_function_declaration(char* name, enum Type type, int definition);
void check_function_declaration(char* name);
int check_function_call(char* name);
int check_function_return(enum Type rettype);

AST* binary_operation(
  AST* op1, AST* op2, NodeKind nodekind,
  TypeData (*operation)(enum Type, enum Type)
);

void check_condition(enum Type type);
void check_assignment(char* name, AST* expr);

void ast_manager_add_declaration(AST** declarations, AST* declaration);
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

void fcall_start(char* name, int check){
    if(check){
	    check_function_declaration(name);
    }
    vector_clear(funcargs);
    fcall_active = 1;
}

void block_enter(void){
    stack_push(block_defined_history, block_defined);
    block_defined = calloc(1, sizeof(int));
    enter_scope();
}

void block_exit(void){
    free(block_defined);
    block_defined = stack_pop(block_defined_history);
    exit_scope();
}

void function_pre_body(char* name, enum Type type){
  if(add_function_declaration(name, type, 0) == -1){
    exit(1);
  }
  retvartype = type;
  funcname = name;
  *block_defined = 0;
}

void funcparams_begin(void){
  params_count = 0;
  enter_scope();
}

AST* function_settle(char *name, enum Type type, AST* params, AST* fblock){
  int function_definition = fblock == NULL ? 0 : 1;
  if(add_function_declaration(name, type, function_definition) == -1){
	  exit(1);
  }

  AST* ast = ast_new_node(NODE_FUNC);
  AST* head = params != NULL ? params : ast_new_node(NODE_FUNC_PARAMLIST);
  AST* body = ast_new_node(NODE_FUNC_BODY); 
  if(fblock != NULL){
    ast_add_child(body, fblock);
  }

  NodeData data;
  data.lit = (Literal){.type = retvartype};
  ast_add_child(ast, head);
  ast_add_child(ast, body);
  ast_set_data(ast, data);

  exit_scope();

  return ast;
}

%}

/* Make 'union Token' definition available in 'parser.tab.h' */
%code requires {
#include "Literal.h"
#include "AST.h"
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
// %left MODULUS

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
  declaration              { ast_manager_add_declaration(&$$, $1); }
| declaration declarations { ast_manager_add_declaration(&$2, $1); $$ = $2; }
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
  type_specifier ID func_paramlist DELI          { $$ = function_settle($2, $1, $3, NULL); }
| type_specifier ID func_paramlist LCURLY RCURLY { $$ = function_settle($2, $1, $3, NULL); }
| type_specifier ID func_paramlist LCURLY { function_pre_body($2, $1); } block_stmts RCURLY { $$ = function_settle($2, $1, $3, $6); }
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
| expr PLUS expr            { $$ = binary_operation($1, $3, NODE_PLUS, typesys_sum);   }
| expr MINUS expr           { $$ = binary_operation($1, $3, NODE_MINUS, typesys_sub); }
| expr STAR expr            { $$ = binary_operation($1, $3, NODE_TIMES, typesys_mul); }
| expr DIVISION expr        { $$ = binary_operation($1, $3, NODE_OVER, typesys_div);  }
| expr OR expr              { $$ = binary_operation($1, $3, NODE_OR, typesys_or);   }
| expr AND expr             { $$ = binary_operation($1, $3, NODE_AND, typesys_and); }
| expr EQ expr              { $$ = binary_operation($1, $3, NODE_EQ, typesys_eq);   }
| expr NOTEQ expr           { $$ = binary_operation($1, $3, NODE_NE, typesys_ne);   }
| expr LT expr              { $$ = binary_operation($1, $3, NODE_LT, typesys_lt);   }
| expr GT expr              { $$ = binary_operation($1, $3, NODE_GT, typesys_gt);   }
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
void enter_scope(void){
	scope_manager_enter(scope_manager);
}

void exit_scope(void){
	scope_manager_exit(scope_manager);
}

AST* add_var_declaration(char* name, enum Type type, enum Qualifier qualifier, AST* init){
	Scope* scope = scope_manager_get_current_scope(scope_manager);
	if(scope_add(scope, name, yylineno, type, qualifier) == -1){
		Variable* var = scope_search_by_name(scope, name);
		printf("SEMANTIC ERROR (%d): variable %s has been declared already\n", var->line, name);
		exit(1);
	}
	if(type == TYPE_VOID){
		Variable* var = scope_search_by_name(scope, name);
		printf("SEMANTIC ERROR (%d): variable %s has been declared with type VOID\n", var->line, name);
		exit(1);
	}
#ifdef DEBUG_SCOPE
	printf("Variable %s added to scope %d\n", name, scope_get_id(scope));
#endif

	Variable* var = scope_search_by_name(scope, name);
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

void check_var_declaration(char* name){
	if(scope_manager_search_by_name(scope_manager, name) == NULL){
		printf("SEMANTIC ERROR (%d): variable %s hasn't been declared yet\n", yylineno, name);
	}
}

Variable* get_var(char* name){
	Scope* scope = scope_manager_search_by_name(scope_manager, name);
  	if(scope == NULL){
		return NULL;
  	}
	return scope_search_by_name(scope, name);
}

int add_function_declaration(char* name, enum Type type, int definition){
	Scope* scope = scope_manager_get_current_scope(scope_manager);
	if(func_table_add(functable, name, scope, type, params_count, definition) == -1){
		if(definition){
			printf("SEMANTIC ERROR (%d): function %s has been defined already\n", yylineno, name);
			return -1;
		}

		printf("SEMANTIC WARNING (%d): function %s has been declared already\n", yylineno, name);
	}
	return 0;
}

void check_function_declaration(char* name){
	int missing_function_declared = func_table_search(functable, name) == NULL;
	//int missing_func_ptr = 0;
	if(missing_function_declared){
		printf("SEMANTIC ERROR (%d): function %s hasn't been declared yet\n", yylineno, name);
	}
}

int check_function_call(char* name){
#define STRING_EQ(name1, name2) (strcmp(name1, name2) == 0)
	if(STRING_EQ(name, "printf") || STRING_EQ(name, "scanf")){
		return 0;
	}
#undef STRING_EQ
	Function* f = func_table_search(functable, name);
	if(f == NULL){
		return -1;
	}

	Scope* scope = func_get_scope(f);
	VarTable* vt = scope_get_vartable(scope);
	const int nparams = func_get_nparams(f);
	const int nargs = vector_get_size(funcargs);
	Variable* param = NULL;
	AST* arg = NULL;
	
	if(nargs != nparams){
		fprintf(stdout, "TYPE ERROR (%d); function %s called with insufficient number of arguments\n", yylineno, name);
		return -1;
	}

	for(int i=0; i<nparams; i++){
		param = vartable_idx(vt, i);
		arg = vector_get_item(funcargs, i);
		if(ast_get_type(arg) != param->type){
			fprintf(stdout, "TYPE ERROR (%d); function %s called with argument of wrong type; '%s' should be '%s'\n", yylineno, name, type_name(ast_get_type(arg)), type_name(param->type));
			return -1;
		}
	}

	return 0;
}

int check_function_return(enum Type rettype){
	if(retvartype != rettype){
		fprintf(stdout, "TYPE ERROR (%d); function %s with wrong return type; '%s' should be '%s'\n", yylineno, funcname, type_name(rettype), type_name(retvartype));
		return -1;
	}
	return 0;
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
			break;
	}
}

AST* binary_operation(
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

void check_assignment(char* name, AST* expr){
	Variable* var = get_var(name);
	TypeData data = typesys_assign(var->type, ast_get_type(expr));
	if(data.type == TYPE_VOID){
		printf("SEMANTIC ERROR (%d): ", yylineno);
		printf("incompatible types for operator '=', ");
		printf("LHS is '%s' and RHS is '%s'.\n", type_name(var->type), type_name(ast_get_type(expr)));
	}
}

void parser_init(void){
	functable = func_table_new();
	scope_manager = scope_manager_new();
	func_table_add(functable, "printf", scope_manager_get_current_scope(scope_manager), TYPE_VOID, 10, 1);
	func_table_add(functable, "scanf", scope_manager_get_current_scope(scope_manager), TYPE_VOID, 10, 1);
	funcargs = vector_new(12);
	block_defined = calloc(1, sizeof(int));
	block_defined_history = stack_new(12);
}

void parser_deinit(void){
	scope_manager_destroy(&scope_manager);
	func_table_destroy(&functable);
	vector_destroy(&funcargs, NULL);
	ast_free(root);
	root = NULL;
	free(block_defined);
	stack_destroy(&block_defined_history, free);
}

void ast_manager_add_declaration(AST** program, AST* declaration){
	static int declarations_defined = 0;
	if(declarations_defined == 0){
		*program = ast_new_node(NODE_PROGRAM);
		declarations_defined = 1;
	}
	ast_add_child(*program, declaration); 
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
	if(*block_defined == 0){
		block = ast_new_node(NODE_BLOCK);
		*block_defined = 1;
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

void parser_info(void){
#ifdef DEBUG_PARSER
	scope_manager_print(scope_manager);
	func_table_print(functable);
#endif
#ifdef DEBUG_AST
	ast_print(root);
	print_dot(root);
#endif
}
