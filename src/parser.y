/*
 * references
 * https://www.lysator.liu.se/c/ANSI-C-grammar-y.html
 * https://en.cppreference.com/w/c/language/constant_expression
 * https://en.cppreference.com/w/c/language/expressions
 */

/* https://www.gnu.org/software/bison/manual/html_node/_0025define-Summary.html */
%define parse.error verbose
%define parse.lac full


%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "ScopeManager.h"
#include "VarTable.h"
#include "FuncTable.h"
#include "Literal.h"
#include "Vector.h"
#include "Stack.h"
#include "AST.h"

typedef int (*fp_bin_op)(Literal*, Literal*, Literal*);

extern int yylineno;
int function_definition;
int params_count;
int fcall_active;
char* funcname;
enum Type vartype;
enum Type retvartype;
ScopeManager* scope_manager;
FuncTable* functable;
Vector* funcargs;
AST* root;
int varlist_defined;
int paramlist_defined;
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

AST* add_var_declaration(char* name, enum Type type);
void check_var_declaration(char* name);
Variable* get_var(char* name);
void var_to_expr(char* name, Literal* exp);

int add_function_declaration(char* name, enum Type type);
void check_function_declaration(char* name);
int check_function_call(char* name);
int check_function_return(enum Type rettype);

void binary_operation(Literal* op1, Literal* op2, Literal* res, fp_bin_op bin_op);

void check_if_while_condition(Literal* exp);
void check_assignment(Literal* op1, Literal* res);

void ast_manager_add_declarator(AST** declarators, AST* declarator);
void ast_manager_add_var_decl(AST** varlist, AST* var);
void ast_manager_add_param_decl(AST** paramlist, AST* param);
void ast_manager_add_to_block(AST** block, AST* item);

%}

/* Make 'union Token' definition available in 'parser.tab.h'
 * REF: https://www.gnu.org/software/bison/manual/html_node/Prologue-Alternatives.html */
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
%token INT FLOAT CHAR VOID
%token SHORT SIGNED DOUBLE UNSIGNED LONG

/* pointer type/function type related */
%token AMP INLINE RESTRICT RETURN STAR

/* special types */
%token ENUM STRUCT TYPEDEF UNION

/* values */
%token <l> CHR FLOATVAL INTVAL QUOTE STRING

/* arithmetic operations */
%token DIVISION MINUS MINUS1 MINUSAUTO MODULUS PLUS PLUS1 PLUSAUTO

/* logical operations */
%token AND EQ GT GTEQ LT LTEQ NOT NOTEQ OR

/* bitwise operations */
%token BITANDAUTO BITNOT BITNOTAUTO BITOR BITORAUTO BITXOR BITXORAUTO LSHIFT LSHIFTAUTO RSHIFT RSHIFTAUTO

/* type qualifier */
%token AUTO CONST EXTERN REGISTER STATIC VOLATILE

/* if else */
%token IF ELSE

/* switch */
%token CASE DEFAULT SWITCH

/* loops */
%token BREAK CONTINUE DO FOR WHILE

/* goto hell yeah */
%token GOTO

/* misc */
%token LBRACKET LCURLY COMMA DELI RBRACKET RCURLY LPAR RPAR SIZEOF
%token <str> PRINTF SCANF

%left LT GT EQ NOTEQ
%left AND OR
%left PLUS MINUS
%left STAR DIVISION MODULUS

%nterm <str> fcaller
%nterm <t> type_specifier
%nterm <l> expr exprs
%nterm <ast> program
%nterm <ast> declarator
%nterm <ast> declarators
%nterm <ast> declarator_var
%nterm <ast> init_declarator_list
%nterm <ast> init_declarator
%nterm <ast> declarator_func
%nterm <ast> opt_func_paramlist
%nterm <ast> func_paramlist
%nterm <ast> func_param 
%nterm <ast> declarator_func_end
%nterm <ast> func_block
%nterm <ast> block
%nterm <ast> block_item_list
%nterm <ast> block_item
%%

/* Recursos fora do escopo desse parser:
  * struct
  * enum
  * union
  * pointer
  * for
*/

/* Dicionário
  * declarator: declaracao de recursos que armazenam estado (variaveis e funcoes)
  * stmt: comandos 'builtin' da linguagem (if, while, ...) + chamada de funcoes
  * block: o bloco de definição de uma função, que pode ter stmts e declarators para variaveis
*/

program:
  declarators { root = $1; }
;

declarators:
  declarator             { ast_manager_add_declarator(&$$, $1); }
| declarator declarators { ast_manager_add_declarator(&$2, $1); $$ = $2; }
;

declarator:
  declarator_var  { $$ = $1; }
| declarator_func { $$ = $1; }
;

declarator_var:
  type_specifier init_declarator_list DELI { $$ = $2; varlist_defined = 0; }
;

init_declarator_list:
  init_declarator                             { ast_manager_add_var_decl(&$$, $1); }
| init_declarator COMMA init_declarator_list  { ast_manager_add_var_decl(&$3, $1); $$ = $3; }
;

init_declarator:
  ID                                                        { $$ = add_var_declaration($1, vartype);  }
| ID ASSIGN expr                                            { $$ = add_var_declaration($1, vartype);  }
| ID LBRACKET INTVAL RBRACKET                               { $$ = add_var_declaration($1, vartype);  }
| ID LBRACKET INTVAL RBRACKET ASSIGN LCURLY RCURLY          { $$ = add_var_declaration($1, vartype);  }
| ID LBRACKET INTVAL RBRACKET ASSIGN LCURLY exprs RCURLY    { $$ = add_var_declaration($1, vartype);  }
| ID LBRACKET INTVAL RBRACKET ASSIGN STRING                 { $$ = add_var_declaration($1, TYPE_STR); }
;

declarator_func:
  type_specifier ID { retvartype = $1; funcname = $2; } LPAR {
	params_count = 0;
	enter_scope();
  }
  opt_func_paramlist RPAR declarator_func_end {
	if(add_function_declaration($2, $1) == -1){
		exit(1);
	}
	$$ = ast_new_node(NODE_FUNC);
	NodeData data;
	data.lit = (Literal){.type = retvartype};
	ast_add_child($$, $6);
	ast_add_child($$, $8);
	ast_set_data($$, data);
	exit_scope();
  }
;

//um pouco criminoso, mas foi necessario para evitar conflitos de shift e reduce
declarator_func_end:
  DELI        { function_definition = 0; $$ = ast_new_node(NODE_FUNC_BODY); }
| func_block  { function_definition = 1; $$ = ast_new_node(NODE_FUNC_BODY); if($1 != NULL){ ast_add_child($$, $1); } }
;

opt_func_paramlist:
  %empty          { $$ = ast_new_node(NODE_FUNC_PARAMLIST); }
| func_paramlist  { $$ = $1 ? $1 : ast_new_node(NODE_FUNC_PARAMLIST); }
;

func_paramlist:
  func_param {
    params_count++;
    if($1 != NULL){
      ast_manager_add_param_decl(&$$, $1);
    }
    else{
      $$ = NULL;
    }
  }
| func_param { params_count++; } COMMA func_paramlist {
    if($1 != NULL){
      ast_manager_add_param_decl(&$4, $1);
    }
    $$ = $4 ? $4 : NULL;
  }
;

func_param:
  type_specifier                                            { $$ = NULL; }
| type_specifier ID                                         { $$ = add_var_declaration($2, vartype); }
| type_specifier ID LBRACKET RBRACKET                       { add_var_declaration($2, vartype); }
| type_specifier ID LBRACKET expr RBRACKET                  { add_var_declaration($2, vartype); }
| type_specifier LPAR STAR RPAR LPAR func_paramlist RPAR    { $$ = NULL; } /* int (*)(...) */
| type_specifier LPAR STAR ID RPAR LPAR func_paramlist RPAR { $$ = NULL; } /* int (*varname)(...) */
;

/* foi necessario criar um bloco que sirva como ponto de entrada de funções e
 * que não crie um novo escopo para a função (isso já é feito ao criar o escopo
 * da lista de argumentos) */
func_block:
  LCURLY RCURLY { $$ = NULL; *block_defined = 0; }
| LCURLY {
    function_definition = 0;
    if(add_function_declaration(funcname, retvartype) == -1){

    }
    function_definition = 1;
    *block_defined = 0;
  } block_item_list RCURLY {
    $$ = $3;
  }

block:
  LCURLY RCURLY { $$ = NULL; }
| LCURLY {
    stack_push(block_defined_history, block_defined);
    block_defined = calloc(1, sizeof(int));
    enter_scope();
  } block_item_list RCURLY {
    $$ = $3;
    free(block_defined);
    block_defined = stack_pop(block_defined_history);
    exit_scope();
  }
;

block_item_list:
  block_item {
    if($1)
      ast_manager_add_to_block(&$$, $1);
    else
      $$ = NULL;
  }
| block_item_list block_item {
    if($2){
      ast_manager_add_to_block(&$1, $2);
    }
    $$ = $1 ? $1 : NULL;
  }
;

block_item:
  declarator_var { $$ = $1; }
| stmt  { $$ = NULL; }
| block { $$ = $1; }
;

stmt:
  assign_stmt
| if_stmt
| while_stmt
| jump_stmt
| fcall_stmt
;

jump_stmt:
  CONTINUE DELI
| BREAK DELI
| RETURN DELI        { check_function_return(TYPE_VOID); }
| RETURN expr DELI   { check_function_return($2.type);  }
;

while_stmt:
  WHILE LPAR expr RPAR block   { check_if_while_condition(&$3); }
;

if_stmt:
  IF LPAR expr RPAR block              { check_if_while_condition(&$3); }
| IF LPAR expr RPAR block ELSE block   { check_if_while_condition(&$3); }
| IF LPAR expr RPAR block ELSE if_stmt { check_if_while_condition(&$3); }
;

assign_stmt:
  ID ASSIGN expr DELI                          { check_var_declaration($1); }
| ID LBRACKET expr RBRACKET ASSIGN expr DELI   { check_var_declaration($1); }
;

fcall_stmt:
	fcall DELI
;

expr:
  LPAR expr RPAR            { $$ = $2 ; }
| ID                        { check_var_declaration($1); var_to_expr($1, &$$); }
| INTVAL                    { $$ = $1; }
| FLOATVAL                  { $$ = $1; }
| STRING                    { $$ = $1; }
| CHR                       { $$ = $1; }
| expr PLUS expr            { binary_operation(&$1, &$3, &$$, literal_sum); }
| expr MINUS expr           { binary_operation(&$1, &$3, &$$, literal_sub); }
| expr STAR expr            { binary_operation(&$1, &$3, &$$, literal_mul); }
| expr DIVISION expr        { binary_operation(&$1, &$3, &$$, literal_div); }
| expr MODULUS expr         {  }
| expr OR expr              { binary_operation(&$1, &$3, &$$, literal_or);  }
| expr AND expr             { binary_operation(&$1, &$3, &$$, literal_and); }
| expr EQ expr              { binary_operation(&$1, &$3, &$$, literal_eq);  }
| expr NOTEQ expr           { binary_operation(&$1, &$3, &$$, literal_ne);  }
| expr LT expr              { binary_operation(&$1, &$3, &$$, literal_lt);  }
| expr GT expr              { binary_operation(&$1, &$3, &$$, literal_gt);  }
| MINUS INTVAL              { $$ = $2; }
| PLUS INTVAL               { $$ = $2; }
| MINUS FLOATVAL            { $$ = $2; }
| PLUS FLOATVAL             { $$ = $2; }
| fcall                     {  }
| AMP ID                    { check_var_declaration($2); }
| ID LBRACKET expr RBRACKET { check_var_declaration($1); $$.type = vartype; } //TODO: improve this
;

fcall:
  fcaller LPAR RPAR         { check_function_call($1); fcall_active = 0; }
| fcaller LPAR exprs RPAR   { check_function_call($1); fcall_active = 0; }
;

fcaller:
  ID      { check_function_declaration($1); vector_clear(funcargs); fcall_active = 1; $$ = $1; }
| PRINTF  { vector_clear(funcargs); fcall_active = 1; $$ = $1; }
| SCANF   { vector_clear(funcargs); fcall_active = 1; $$ = $1; }
;

exprs:
  expr { if(fcall_active == 1){ vector_append(funcargs, &$1) ; }; }
| expr { if(fcall_active == 1){ vector_append(funcargs, &$1) ; }; } COMMA exprs
;

/* WARNING: o tipo void deverá ser tratado durante a analise semântica: NÂO permitir a declaração de variáveis desse tipo. */
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

AST* add_var_declaration(char* name, enum Type type){
	Scope* scope = scope_manager_get_current_scope(scope_manager);
	if(scope_add(scope, name, yylineno, type) == -1){
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

void var_to_expr(char* name, Literal* exp){
	Variable* var = get_var(name);
	if(var == NULL){
		fprintf(stderr, "SEMANTIC ERROR (%d): variable '%s' not found in the current scope\n", yylineno, name);
		return ;
	}
	exp->type = var->token.type;
	exp->value = var->token.value;
}

int add_function_declaration(char* name, enum Type type){
	Scope* scope = scope_manager_get_current_scope(scope_manager);
	if(func_table_add(functable, name, scope, type, params_count, function_definition) == -1){
		if(function_definition){
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
	Literal* arg = NULL;
	
	if(nargs != nparams){
		fprintf(stderr, "TYPE ERROR (%d); function %s called with insufficient number of arguments\n", yylineno, name);
		return -1;
	}

	for(int i=0; i<nparams; i++){
		param = vartable_idx(vt, i);
		arg = vector_get_item(funcargs, i);
		if(arg->type != param->token.type){
			fprintf(stderr, "TYPE ERROR (%d); function %s called with argument of wrong type; '%s' should be '%s'\n", yylineno, name, literal_get_typename(arg), literal_get_typename(&param->token));
			return -1;
		}
	}

	return 0;
}

int check_function_return(enum Type rettype){
	if(retvartype != rettype){
		fprintf(stderr, "TYPE ERROR (%d); function %s with wrong return type; '%s' should be '%s'\n", yylineno, funcname, type_get_name(rettype), type_get_name(retvartype));
		return -1;
	}
	return 0;
}

void check_if_while_condition(Literal* exp){
	switch(exp->type){
		case TYPE_INT:
			break;
		case TYPE_VOID:
		case TYPE_FLT:
		case TYPE_STR:
		case TYPE_CHAR:
		default:
		fprintf(stderr, "SEMANTIC ERROR (%d): conditional expression in IF/WHILE", yylineno);
		fprintf(stderr, " is '%s' instead of 'int'\n", literal_get_typename(exp));
			break;
	}
}

void binary_operation(Literal* op1, Literal* op2, Literal* res, fp_bin_op bin_op){
	if(bin_op(op1, op2, res) == -1){
		fprintf(stderr, "SEMANTIC ERROR (%d):", yylineno);
		fprintf(stderr, "incompatible types for operator: ");
		fprintf(stderr, "LHS is '%s'", literal_get_typename(op1));
		fprintf(stderr, " and RHS is '%s'\n", literal_get_typename(op2));
		exit(1);
	}
}

void check_assignment(Literal* op1, Literal* res){
	if(literal_assign(op1, res) == -1){
		printf("SEMANTIC ERROR (%d): ", yylineno);
		printf("incompatible types for operator '=', ");
		printf("LHS is '%s' and RHS is '%s'.\n", literal_get_typename(res), literal_get_typename(op1));
	}
}

void parser_init(void){
	functable = func_table_new();
	scope_manager = scope_manager_new();
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

void ast_manager_add_declarator(AST** program, AST* declarator){
	static int declarators_defined = 0;
	if(declarators_defined == 0){
		*program = ast_new_node(NODE_PROGRAM);
		declarators_defined = 1;
	}
	ast_add_child(*program, declarator); 
}

void ast_manager_add_var_decl(AST** varlist, AST* var){
	if(varlist_defined == 0){
		*varlist = ast_new_node(NODE_VAR_LIST);
		varlist_defined = 1;
	}
	ast_add_child(*varlist, var); 
}

void ast_manager_add_param_decl(AST** paramlist, AST* param){
	if(paramlist_defined == 0){
		*paramlist = ast_new_node(NODE_FUNC_PARAMLIST);
		paramlist_defined = 1;
	}
	ast_add_child(*paramlist, param); 
}

void ast_manager_add_to_block(AST** block, AST* item){
	if(*block_defined == 0){
		*block = ast_new_node(NODE_BLOCK);
		*block_defined = 1;
	}
	ast_add_child(*block, item); 
}

void parser_info(void){
#ifdef DEBUG_PARSER
	scope_manager_print(scope_manager);
	func_table_print(functable);
#endif
	print_dot(root);
}
