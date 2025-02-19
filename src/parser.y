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

#include "typesys.h"
#include "ScopeManager.h"
#include "VarTable.h"
#include "FuncTable.h"
#include "Literal.h"
#include "Vector.h"
#include "Stack.h"
#include "AST.h"

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

AST* add_var_declaration(char* name, enum Type type, AST* init);
AST* add_arr_declaration(char* name, enum Type type, AST* init, int count);
void check_var_declaration(char* name);
Variable* get_var(char* name);
void var_to_expr(char* name, Literal* exp);

int add_function_declaration(char* name, enum Type type);
void check_function_declaration(char* name);
int check_function_call(char* name);
int check_function_return(enum Type rettype);

AST* binary_operation(
  AST* op1, AST* op2, NodeKind nodekind,
  TypeData (*operation)(enum Type, enum Type)
);

void check_if_while_condition(Literal* exp);
void check_assignment(char* name, AST* expr);

void ast_manager_add_declarator(AST** declarators, AST* declarator);
void ast_manager_add_var_decl(AST** varlist, AST* var);
void ast_manager_add_param_decl(AST** paramlist, AST* param);
void ast_manager_add_to_block(AST** block, AST* item);
AST* ast_manager_add_conv(AST* data, Conversion conv);
AST* ast_manager_from_literal(Literal* lit);
AST* ast_manager_from_id(char* name);
AST* ast_manager_assign_stmt(char* name, AST* expr);
AST* ast_manager_fcall(char* funcname);

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
%token DIVISION MINUS MINUS1 MINUSAUTO PLUS PLUS1 PLUSAUTO MODULUS

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
%left STAR DIVISION
// %left MODULUS

%nterm <str> fcaller
%nterm <t> type_specifier
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
%nterm <ast> expr
%nterm <ast> stmt
%nterm <ast> assign_stmt
%nterm <ast> if_stmt
%nterm <ast> while_stmt
%nterm <ast> jump_stmt
%nterm <ast> fcall_stmt
%nterm <ast> fcall
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
  ID                                                        { $$ = add_var_declaration($1, vartype,  NULL);  }
| ID ASSIGN expr                                            { $$ = add_var_declaration($1, vartype,  $3);  }
| ID LBRACKET INTVAL RBRACKET                               { $$ = add_var_declaration($1, vartype,  NULL);  }
| ID LBRACKET INTVAL RBRACKET ASSIGN LCURLY RCURLY          { $$ = add_var_declaration($1, vartype,  NULL);  }
| ID LBRACKET INTVAL RBRACKET ASSIGN LCURLY exprs RCURLY    { $$ = add_var_declaration($1, vartype,  NULL);  }
| ID LBRACKET INTVAL RBRACKET ASSIGN STRING                 { $$ = add_var_declaration($1, TYPE_STR, NULL); }
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
| type_specifier ID                                         { $$ = add_var_declaration($2, vartype, NULL); }
| type_specifier ID LBRACKET RBRACKET                       { add_var_declaration($2, vartype, NULL); }
| type_specifier ID LBRACKET expr RBRACKET                  { add_var_declaration($2, vartype, NULL); }
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
| stmt  { $$ = $1; }
| block { $$ = $1; }
;

stmt:
  assign_stmt { $$ = $1; }
| if_stmt     { $$ = NULL; }
| while_stmt  { $$ = NULL; }
| jump_stmt   { $$ = NULL; }
| fcall_stmt  { $$ = $1; }
;

jump_stmt:
  CONTINUE DELI      { $$ = NULL; }
| BREAK DELI         { $$ = NULL; }
| RETURN DELI        { check_function_return(TYPE_VOID); $$ = NULL; }
| RETURN expr DELI   { check_function_return(ast_get_type($2));  $$ = NULL; }
;

while_stmt:
  WHILE LPAR expr RPAR block   { check_if_while_condition(ast_get_literal($3)); }
;

if_stmt:
  IF LPAR expr RPAR block              { check_if_while_condition(ast_get_literal($3)); }
| IF LPAR expr RPAR block ELSE block   { check_if_while_condition(ast_get_literal($3)); }
| IF LPAR expr RPAR block ELSE if_stmt { check_if_while_condition(ast_get_literal($3)); }
;

assign_stmt:
  ID ASSIGN expr DELI                          { check_var_declaration($1); check_assignment($1, $3); $$ = ast_manager_assign_stmt($1, $3); }
| ID LBRACKET expr RBRACKET ASSIGN expr DELI   { check_var_declaration($1); check_assignment($1, $3); $$ = ast_manager_assign_stmt($1, $3); }
;

fcall_stmt:
  fcall DELI { $$ = $1; }
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

fcall:
  fcaller LPAR RPAR         { check_function_call($1); fcall_active = 0; $$ = ast_manager_fcall($1); }
| fcaller LPAR exprs RPAR   { check_function_call($1); fcall_active = 0; $$ = ast_manager_fcall($1); }
;

fcaller:
  ID      { check_function_declaration($1); vector_clear(funcargs); fcall_active = 1; $$ = $1; }
| PRINTF  { vector_clear(funcargs); fcall_active = 1; $$ = $1; }
| SCANF   { vector_clear(funcargs); fcall_active = 1; $$ = $1; }
;

exprs:
  expr { if(fcall_active == 1){ vector_append(funcargs, $1) ; }; }
| expr { if(fcall_active == 1){ vector_append(funcargs, $1) ; }; } COMMA exprs
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

AST* add_var_declaration(char* name, enum Type type, AST* init){
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
	if(init != NULL){
		ast_add_child(ast, init);
	}
	return ast;
}

// AST* add_arr_declaration(char* name, enum Type type, AST* init, int count){
// 	Scope* scope = scope_manager_get_current_scope(scope_manager);
// 	if(scope_add(scope, name, yylineno, type) == -1){
// 		Variable* var = scope_search_by_name(scope, name);
// 		printf("SEMANTIC ERROR (%d): variable %s has been declared already\n", var->line, name);
// 		exit(1);
// 	}
// 	if(type == TYPE_VOID){
// 		Variable* var = scope_search_by_name(scope, name);
// 		printf("SEMANTIC ERROR (%d): variable %s has been declared with type VOID\n", var->line, name);
// 		exit(1);
// 	}
// #ifdef DEBUG_SCOPE
// 	printf("Variable %s added to scope %d\n", name, scope_get_id(scope));
// #endif

// 	Variable* var = scope_search_by_name(scope, name);
// 	AST* ast = ast_new_node(NODE_VAR_DECL);
// 	NodeData data;
// 	data.arr.var = *var;
// 	data.arr.sz = count;
// 	ast_set_data(ast, data);
// 	if(init != NULL){
// 		ast_add_child(ast, init);
// 	}
// 	return ast;
// }

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
	AST* arg = NULL;
	
	if(nargs != nparams){
		fprintf(stdout, "TYPE ERROR (%d); function %s called with insufficient number of arguments\n", yylineno, name);
		return -1;
	}

	for(int i=0; i<nparams; i++){
		param = vartable_idx(vt, i);
		arg = vector_get_item(funcargs, i);
		if(ast_get_type(arg) != param->token.type){
			fprintf(stdout, "TYPE ERROR (%d); function %s called with argument of wrong type; '%s' should be '%s'\n", yylineno, name, type_name(ast_get_type(arg)), type_name(param->token.type));
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

void check_if_while_condition(Literal* exp){
	switch(exp->type){
		case TYPE_INT:
			break;
		case TYPE_VOID:
		case TYPE_FLT:
		case TYPE_STR:
		case TYPE_CHAR:
		default:
		fprintf(stdout, "SEMANTIC ERROR (%d): conditional expression in IF/WHILE", yylineno);
		fprintf(stdout, " is '%s' instead of 'int'\n", type_name(exp->type));
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
	TypeData data = typesys_assign(var->token.type, ast_get_type(expr));
	if(data.type == TYPE_VOID){
		printf("SEMANTIC ERROR (%d): ", yylineno);
		printf("incompatible types for operator '=', ");
		printf("LHS is '%s' and RHS is '%s'.\n", type_name(var->token.type), type_name(ast_get_type(expr)));
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
	AST* stmt = ast_new_node(NODE_ASSIGN);
	AST* var = ast_manager_from_id(name);
	NodeData data;
	data.lit.type = ast_get_type(expr);
	ast_add_child(stmt, var);
	ast_add_child(stmt, expr);
	ast_set_data(stmt, data);
	return stmt;
}

AST* ast_manager_fcall(char* funcname){
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

void parser_info(void){
#ifdef DEBUG_PARSER
	scope_manager_print(scope_manager);
	func_table_print(functable);
#endif
	ast_print(root);
	print_dot(root);
}
