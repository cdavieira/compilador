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

typedef int (*fp_bin_op)(Literal*, Literal*, Literal*);

extern int yylineno;
int function_declared;
int params_count;
enum Type vartype;
ScopeManager* scope_manager;
FuncTable* functable;

int yylex(void);
void yyerror(char const *s);

void parser_init(void);
void parser_deinit(void);
void parser_info(void);

void enter_scope(void);
void exit_scope(void);

void add_var_declaration(char* name, enum Type type);
void check_var_declaration(char* name);
Variable* get_var(char* name);
void var_to_expr(char* name, Literal* exp);

void add_function_declaration(char* name, enum Type type);
void check_function_declaration(char* name);

void binary_operation(Literal* op1, Literal* op2, Literal* res, fp_bin_op bin_op);

void check_if_while_condition(Literal* exp);
void check_assignment(Literal* op1, Literal* res);
%}

/* Make 'union Token' definition available in 'parser.tab.h'
 * REF: https://www.gnu.org/software/bison/manual/html_node/Prologue-Alternatives.html */
%code requires {
#include "Literal.h"
union Token {
	Literal l;
	enum Type t;
	char* str;
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
%token LBRACKET LCURLY COMMA DELI RBRACKET RCURLY LPAR RPAR
%token SIZEOF PRINTF SCANF

%left LT GT EQ NOTEQ
%left AND OR
%left PLUS MINUS
%left STAR DIVISION MODULUS

%nterm <t> type_specifier
%nterm <l> expr
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
  declarators
;

declarators:
  declarator
| declarator declarators
;

declarator:
  declarator_var
| declarator_func
;

declarator_var:
  type_specifier init_declarator_list DELI
;

init_declarator_list:
  init_declarator
| init_declarator COMMA init_declarator_list
;

init_declarator:
  ID                                                        { add_var_declaration($1, vartype); }
| ID ASSIGN expr                                            { add_var_declaration($1, vartype); }
| ID LBRACKET INTVAL RBRACKET                               { add_var_declaration($1, vartype); }
| ID LBRACKET INTVAL RBRACKET ASSIGN LCURLY RCURLY          { add_var_declaration($1, vartype); }
| ID LBRACKET INTVAL RBRACKET ASSIGN LCURLY exprs RCURLY    { add_var_declaration($1, vartype); }
| ID LBRACKET INTVAL RBRACKET ASSIGN STRING                 { add_var_declaration($1, TYPE_STR); }
;

declarator_func:
  type_specifier ID LPAR { params_count = 0; enter_scope(); } opt_func_paramlist RPAR declarator_func_end { exit_scope(); add_function_declaration($2, $1); }
;

//um pouco criminoso, mas foi necessario para evitar conflitos de shift e reduce
declarator_func_end:
  DELI        { function_declared = 1; }
| func_block  { function_declared = 0; }
;

opt_func_paramlist:
  %empty
| func_paramlist
;

func_paramlist:
  func_param    { params_count++; }
| func_param    { params_count++; } COMMA func_paramlist
;

func_param:
  type_specifier
| type_specifier ID                                       { add_var_declaration($2, vartype); }
| type_specifier ID LBRACKET RBRACKET                     { add_var_declaration($2, vartype); }
| type_specifier ID LBRACKET expr RBRACKET                { add_var_declaration($2, vartype); }
| type_specifier LPAR STAR RPAR LPAR func_paramlist RPAR    /* int (*)(...) */
| type_specifier LPAR STAR ID RPAR LPAR func_paramlist RPAR /* int (*varname)(...) */
;

/* foi necessario criar um bloco que sirva como ponto de entrada de funções e
 * que não crie um novo escopo para a função (isso já é feito ao criar o escopo
 * da lista de argumentos) */
func_block:
  LCURLY RCURLY
| LCURLY block_item_list RCURLY

block:
  LCURLY RCURLY
| LCURLY { enter_scope(); } block_item_list RCURLY { exit_scope(); }
;

block_item_list:
  block_item
| block_item_list block_item
;

block_item:
  declarator_var
| stmt
| block
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
| RETURN DELI
| RETURN expr DELI
;

while_stmt:
  WHILE LPAR expr RPAR block
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
| ID LBRACKET expr RBRACKET { check_var_declaration($1); }
;

fcall:
  fcaller LPAR RPAR         { /* lmao how do i do this */ }
| fcaller LPAR exprs RPAR   { /* lmao how do i do this */ }
;

fcaller:
  ID      { check_function_declaration($1); }
| PRINTF
| SCANF
;

exprs:
  expr
| expr COMMA exprs
;

/* WARNING: o tipo void deverá ser tratado durante a analise semântica: NÂO permitir a declaração de variáveis desse tipo. */
type_specifier:
  INT     { vartype = TYPE_INT;    }
| CHAR    { vartype = TYPE_CHAR;   }
| FLOAT   { vartype = TYPE_FLT;    }
| VOID    { vartype = TYPE_VOID;   }
;

%%
void enter_scope(void){
	scope_manager_enter(scope_manager);
}

void exit_scope(void){
	scope_manager_exit(scope_manager);
}

void add_var_declaration(char* name, enum Type type){
	if(type == TYPE_VOID){
		printf("SEMANTIC ERROR (%d): variable %s has been declared with type VOID\n", var->line, name);
		exit(1);
	}
	Scope* scope = scope_manager_get_current_scope(scope_manager);
	if(scope_add(scope, name, yylineno, type) == -1){
		Variable* var = scope_search_by_name(scope, name);
		printf("SEMANTIC ERROR (%d): variable %s has been declared already\n", var->line, name);
	}
	//else{
	//	printf("Variable %s added to scope %d\n", name, scope_get_id(scope));
	//}
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

void add_function_declaration(char* name, enum Type type){
	Scope* scope = scope_manager_get_current_scope(scope_manager);
	if(function_declared == 1){
		if(func_table_add(functable, name, scope, type, params_count) == -1){
			printf("SEMANTIC WARNING (%d): function %s has been declared already\n", yylineno, name);
			return;
		}
		return ;
	}

	//func definition
	if(func_table_add(functable, name, scope, type, params_count) == -1){
		printf("SEMANTIC ERROR (%d): function %s has been defined already\n", yylineno, name);
		return;
	}
}

void check_function_declaration(char* name){
	int missing_function_declared = func_table_search(functable, name) == NULL;
	//int missing_func_ptr = 0;
	if(missing_function_declared){
		printf("SEMANTIC ERROR (%d): function %s hasn't been declared yet\n", yylineno, name);
	}
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
		fprintf(stderr, "SEMANTIC ERROR (%d): conditional expression in IF/REPEAT", yylineno);
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
}

void parser_deinit(void){
	scope_manager_destroy(&scope_manager);
	func_table_destroy(&functable);
}

void parser_info(void){
	scope_manager_print(scope_manager);
}
