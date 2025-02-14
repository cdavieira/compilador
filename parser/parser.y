%define parse.error verbose
%define parse.lac full

%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "management/ScopeManager.h"
#include "management/VarTable.h"
#include "management/FuncTable.h"
#include "management/AST.h"
#include "type/Literal.h"
#include "container/Vector.h"

/* bison's internal variable that stores the current line number being processed. */
extern int yylineno;

/* if function_definition = 1, then we're currently reading a function definition block.
   This is useful for the parser to know if it is reading a declaration or a function definition. */
int function_definition;

/* params_count stores how mant parameters a function has. */
int params_count;

/* if fcall_active = 1, then all arguments parsed by the parser inside parenthesis get stored in 'funcargs' */
int fcall_active;

/* funcname stores the name of the current function.
   This is a trick to allow the parser to add a function declaration before start reading the function definition block (if it has one) */
char* funcname;

/* This is a trick to allow the parser to add a batch of variables from the same type.
   For example:
      int a, b, c, d; */
enum Type vartype;

/* This is also a trick which works together with 'funcname' for the same purpose, but to store the function return type. */
enum Type retvartype;

/* This is the variable which controls and stores all scopes in this program.
   It can find any variable definition. */
ScopeManager* scope_manager;

/* This is the variable which stores all functions in this program.
   Each functin has one scope associated with it, which always has access to the global scope. */
FuncTable* functable;

/* This variable stores any number of arguments that get passed to a function. */
Vector* funcargs;

AST* root;


/* bison mandatory */
int yylex(void);
void yyerror(char const *s);

/* internals */
void parser_init(void);
void parser_deinit(void);
void parser_info(void);

/* scope */
void scope_enter(void);
void scope_exit(void);

/* variables */
void var_add_declaration(char* name, enum Type type);
int var_check_declaration(char* name);
// searchs a variable named 'name' in the current scope
Variable* var_search(char* name);
void var_to_expr(char* name, Literal* exp);
enum Type var_get_type(char* name);

/* functions */
int function_add_declaration(char* name, enum Type type);
enum Type function_get_returntype(char* name);
void function_check_declaration(char* name);
int function_check_call(char* name);
int function_check_return(enum Type rettype);
void function_start_fcall();
void function_end_fcall();

/* misc */
void binary_operation(
  AST* op1,
  AST* op2,
  AST** res,
  NodeKind nodekind,
  int (*operation)(Literal*, Literal*, BinaryOperationData*));
// void check_assignment(AST* op1, AST* res);
void check_if_while_condition(AST* exp);

/* ast related */
void ast_manager_add_declarator(AST** declarators, AST* declarator);
AST* ast_manager_new_node_literal(NodeKind kind, Literal lit);
AST* ast_manager_new_node_id(char* name);
AST* ast_manager_new_node_function(NodeKind kind, char* name, enum Type type);
AST* ast_manager_new_node_fcall(char* name);

%}

/* Make 'union Token' definition available in 'parser.tab.h' */
%code requires {
#include "type/Literal.h"
#include "type/Type.h"
#include "management/AST.h"
union Token {
	Literal l;
	char* str;
	AST* ast;
};
}

%define api.value.type { union Token }

/* variable related */
%token <str> ID
%token ASSIGN

/* basic types */
%token INT
%token FLOAT
%token CHAR
%token VOID
%token SHORT
%token SIGNED
%token DOUBLE
%token LONG
%token UNSIGNED

/* pointer type/function type related */
%token AMP
%token STAR
%token RETURN
%token RESTRICT
%token INLINE

/* special types */
%token ENUM
%token UNION
%token TYPEDEF
%token STRUCT

/* values (literal) */
%token <l> CHR
%token <l> STRING
%token <l> QUOTE
%token <l> INTVAL
%token <l> FLOATVAL

/* arithmetic operations */
%token PLUS
%token MINUS
%token DIVISION
%token MODULUS
%token PLUSAUTO
%token MINUSAUTO
%token PLUS1
%token MINUS1

/* logical operations */
%token AND
%token OR
%token NOTEQ
%token NOT
%token LTEQ
%token LT
%token GTEQ
%token GT
%token EQ

/* bitwise operations */
%token BITANDAUTO
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

/* type qualifier */
%token AUTO
%token VOLATILE
%token STATIC
%token REGISTER
%token EXTERN
%token CONST

/* if else */
%token IF
%token ELSE

/* switch */
%token CASE
%token SWITCH
%token DEFAULT

/* loops */
%token BREAK
%token WHILE
%token FOR
%token DO
%token CONTINUE

/* goto hell yeah */
%token GOTO

/* delimiters */
%token LBRACKET
%token SIZEOF
%token RPAR
%token LPAR
%token RCURLY
%token RBRACKET
%token DELI
%token COMMA
%token LCURLY

/* functions */
%token <ast> PRINTF
%token <ast> SCANF

/* non terminals */
%nterm <ast> program
%nterm <ast> declarator
%nterm <ast> declarators
%nterm <ast> declarator_var
%nterm <ast> declarator_func
%nterm <ast> type_specifier
%nterm <ast> init_declarator
%nterm <ast> init_declarator_list
%nterm <ast> opt_func_paramlist
%nterm <ast> func_param
%nterm <ast> func_paramlist
%nterm <ast> func_block stmt
%nterm <ast> block_item
%nterm <ast> block_item_list
%nterm <ast> block
%nterm <ast> assign_stmt
%nterm <ast> fcall_stmt
%nterm <ast> jump_stmt
%nterm <ast> while_stmt
%nterm <ast> if_stmt
%nterm <ast> fcall
%nterm <str> fcaller
%nterm <ast> expr
%nterm <ast> exprs
%nterm <ast> number

%left LT GT EQ NOTEQ
%left AND OR
%left PLUS MINUS
%left STAR DIVISION MODULUS
%%

program:
  declarators { root = ast_new_subtree(NODE_PROGRAM, $1, NULL); }
;

declarators:
  declarator             { ast_manager_add_declarator(&$$, $1); }
| declarator declarators { ast_manager_add_declarator(&$$, $1); }
;

declarator:
  declarator_var  { $$ = $1; }
| declarator_func { $$ = $1; }
;

declarator_var:
  type_specifier init_declarator_list DELI
;

init_declarator_list:
  init_declarator
| init_declarator COMMA init_declarator_list
;

init_declarator:
  ID                                                        { var_add_declaration($1, vartype); }
| ID ASSIGN expr                                            { var_add_declaration($1, vartype); }
| ID LBRACKET INTVAL RBRACKET                               { var_add_declaration($1, vartype); }
| ID LBRACKET INTVAL RBRACKET ASSIGN LCURLY RCURLY          { var_add_declaration($1, vartype); }
| ID LBRACKET INTVAL RBRACKET ASSIGN LCURLY exprs RCURLY    { var_add_declaration($1, vartype); }
| ID LBRACKET INTVAL RBRACKET ASSIGN STRING                 { var_add_declaration($1, TYPE_STR); }
;

declarator_func:
  type_specifier ID {
	retvartype = vartype;
	funcname = $2;
  } LPAR {
	params_count = 0;
	scope_enter();
  }
  opt_func_paramlist RPAR declarator_func_end {
	if(function_add_declaration($2, retvartype) == -1){
		exit(1);
	}
	scope_exit();
  }
;

//um pouco criminoso, mas foi necessario para evitar conflitos de shift e reduce
declarator_func_end:
  DELI        { function_definition = 0; }
| func_block  { function_definition = 1; }
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
| type_specifier ID                                       { var_add_declaration($2, vartype); }
| type_specifier ID LBRACKET RBRACKET                     { var_add_declaration($2, vartype); }
| type_specifier ID LBRACKET expr RBRACKET                { var_add_declaration($2, vartype); }
| type_specifier LPAR STAR RPAR LPAR func_paramlist RPAR    /* int (*)(...) */
| type_specifier LPAR STAR ID RPAR LPAR func_paramlist RPAR /* int (*varname)(...) */
;

/* foi necessario criar um bloco que sirva como ponto de entrada de funções e
 * que não crie um novo escopo para a função (isso já é feito ao criar o escopo
 * da lista de argumentos) */
func_block:
  LCURLY RCURLY
| LCURLY {
	function_definition = 0;
	if(function_add_declaration(funcname, retvartype) == -1){
		fprintf(stdout, "Err when adding temporary func declaration\n");
	}
	function_definition = 1;
  } block_item_list RCURLY

block:
  LCURLY RCURLY
| LCURLY { scope_enter(); } block_item_list RCURLY { scope_exit(); }
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
| RETURN DELI        { function_check_return(TYPE_VOID); }
| RETURN expr DELI   { function_check_return(ast_get_type($2));  }
;

while_stmt:
  WHILE LPAR expr RPAR block   { check_if_while_condition($3); }
;

if_stmt:
  IF LPAR expr RPAR block              { check_if_while_condition($3); }
| IF LPAR expr RPAR block ELSE block   { check_if_while_condition($3); }
| IF LPAR expr RPAR block ELSE if_stmt { check_if_while_condition($3); }
;

assign_stmt:
  ID ASSIGN expr DELI                          { var_check_declaration($1); }
| ID LBRACKET expr RBRACKET ASSIGN expr DELI   { var_check_declaration($1); }
;

fcall_stmt:
	fcall DELI
;

expr:
  ID                        { ast_manager_new_node_id($1); }
| number                    { $$ = $1; }
| STRING                    { $$ = ast_manager_new_node_literal(NODE_STR_VAL, $1);; }
| CHR                       { $$ = ast_manager_new_node_literal(NODE_CHR_VAL, $1);; }
| LPAR expr RPAR            { $$ = $2 ; }
| expr PLUS expr            { binary_operation($1, $3, &$$, NODE_PLUS, literal_sum); }
| expr MINUS expr           { binary_operation($1, $3, &$$, NODE_MINUS, literal_sub); }
| expr STAR expr            { binary_operation($1, $3, &$$, NODE_TIMES, literal_mul); }
| expr DIVISION expr        { binary_operation($1, $3, &$$, NODE_OVER, literal_div); }
| expr OR expr              { binary_operation($1, $3, &$$, NODE_OR, literal_or);  }
| expr AND expr             { binary_operation($1, $3, &$$, NODE_AND, literal_and); }
| expr EQ expr              { binary_operation($1, $3, &$$, NODE_EQ, literal_eq);  }
| expr NOTEQ expr           { binary_operation($1, $3, &$$, NODE_NE, literal_ne);  }
| expr LT expr              { binary_operation($1, $3, &$$, NODE_LT, literal_lt);  }
| expr GT expr              { binary_operation($1, $3, &$$, NODE_GT, literal_gt);  }
| fcall                     { $$ = $1; }
| AMP ID                    { var_check_declaration($2); }
| ID LBRACKET expr RBRACKET { var_check_declaration($1); } //TODO: improve this
;

number:
  INTVAL                    { $$ = ast_manager_new_node_literal(NODE_INT_VAL, $1); }
| FLOATVAL                  { $$ = ast_manager_new_node_literal(NODE_FLT_VAL, $1); }
| MINUS number              { $$ = $2; }
| PLUS number               { $$ = $2; }
;

fcall:
  fcaller LPAR RPAR         { function_check_call($1); function_end_fcall(); }
| fcaller LPAR exprs RPAR   { function_check_call($1); function_end_fcall(); }
;

fcaller:
  ID      { function_check_declaration($1); function_start_fcall(); }
| PRINTF  { function_start_fcall(); }
| SCANF   { function_start_fcall(); }
;

exprs:
  expr { if(fcall_active == 1){ vector_append(funcargs, &$1) ; }; }
| expr { if(fcall_active == 1){ vector_append(funcargs, &$1) ; }; } COMMA exprs
;

/* WARNING: o tipo void deverá ser tratado durante a analise semântica: NÂO permitir a declaração de variáveis desse tipo. */
type_specifier:
  INT     { vartype = TYPE_INT;  }
| CHAR    { vartype = TYPE_CHAR; }
| FLOAT   { vartype = TYPE_FLT;  }
| VOID    { vartype = TYPE_VOID; }
;

%%

/* parser internals */

void parser_init(void){
	functable = func_table_new();
	scope_manager = scope_manager_new();
	funcargs = vector_new(12);
}

void parser_deinit(void){
	scope_manager_destroy(&scope_manager);
	func_table_destroy(&functable);
	vector_destroy(&funcargs, NULL);
}

void parser_info(void){
#ifdef DEBUG_PARSER
	scope_manager_print(scope_manager);
	func_table_print(functable);
#endif
}


/* scope */

void scope_enter(void){
	scope_manager_enter(scope_manager);
}

void scope_exit(void){
	scope_manager_exit(scope_manager);
}



/* variables */

void var_add_declaration(char* name, enum Type type){
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
}

int var_check_declaration(char* name){
	if(scope_manager_search_by_name(scope_manager, name) == NULL){
		printf("SEMANTIC ERROR (%d): variable %s hasn't been declared yet\n", yylineno, name);
		return -1;
	}
	return 0;
}

Variable* var_search(char* name){
	Scope* scope = scope_manager_search_by_name(scope_manager, name);
  	if(scope == NULL){
		return NULL;
  	}
	return scope_search_by_name(scope, name);
}

void var_to_expr(char* name, Literal* exp){
	Variable* var = var_search(name);
	if(var == NULL){
		fprintf(stderr, "SEMANTIC ERROR (%d): variable '%s' not found in the current scope\n", yylineno, name);
		return ;
	}
	exp->type = var->token.type;
	exp->value = var->token.value;
}

enum Type var_get_type(char* name){
	Variable* var = var_search(name);
	if(var == NULL){
		fprintf(stderr, "SEMANTIC ERROR (%d): variable '%s' not found in the current scope\n", yylineno, name);
		return -1;
	}
	return var->token.type;
}



/* functions */

int function_add_declaration(char* name, enum Type type){
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

enum Type function_get_returntype(char* name){
	Function* func = func_table_search(functable, name);
	return func_get_return(func);
}

void function_check_declaration(char* name){
	int missing_function_declared = func_table_search(functable, name) == NULL;
	//int missing_func_ptr = 0;
	if(missing_function_declared){
		printf("SEMANTIC ERROR (%d): function %s hasn't been declared yet\n", yylineno, name);
	}
}

int function_check_call(char* name){
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
			fprintf(stderr, "TYPE ERROR (%d); function %s called with argument of wrong type; '%s' should be '%s'\n", yylineno, name, type_name(arg->type), type_name(param->token.type));
			return -1;
		}
	}

	return 0;
}

int function_check_return(enum Type rettype){
	if(retvartype != rettype){
		fprintf(stderr, "TYPE ERROR (%d); function %s with wrong return type; '%s' should be '%s'\n", yylineno, funcname, type_name(rettype), type_name(retvartype));
		return -1;
	}
	return 0;
}

void function_start_fcall(){
	vector_clear(funcargs);
	fcall_active = 1;
}

void function_end_fcall(){
	fcall_active = 0;
}



/* misc */

void check_if_while_condition(AST* exp){
	enum Type type = ast_get_type(exp);
	switch(type){
		case TYPE_INT:
			break;
		case TYPE_VOID:
		case TYPE_FLT:
		case TYPE_STR:
		case TYPE_CHAR:
		default:
			fprintf(stderr, "SEMANTIC ERROR (%d): conditional expression in IF/WHILE", yylineno);
			fprintf(stderr, " is '%s' instead of 'int'\n", type_name(type));
			break;
	}
}

void binary_operation(
  AST* op1,
  AST* op2,
  AST** res,
  NodeKind nodekind,
  int (*operation)(Literal*, Literal*, BinaryOperationData*))
{
	Literal* l1;
	Literal* l2;
	AST* leftnode;
	AST* rightnode;
	NodeKind kind;
	NodeData resdata;
	BinaryOperationData meta;

	l1 = ast_get_literal(op1);
	l2 = ast_get_literal(op2);

	if(l1 == NULL || l2 == NULL){
		*res = NULL;
		fprintf(stdout, "Got nil when performing operation (NODEKIND=%d)\n", nodekind);
		return ;
	}

	if(operation(l1, l2, &meta) == -1){
		fprintf(stderr, "SEMANTIC ERROR (%d):", yylineno);
		fprintf(stderr, "incompatible types for operator: ");
		fprintf(stderr, "LHS is '%s'", type_name(l1->type));
		fprintf(stderr, " and RHS is '%s'\n", type_name(l2->type));
		exit(1);
	}

	leftnode = op1;
	rightnode = op2;

	switch(meta.left){
		case CONV_I2F:
			kind = NODE_I2F;
			break;
		case CONV_I2C:
			kind = NODE_I2C;
			break;
		case CONV_C2I:
			kind = NODE_C2I;
			break;
		case CONV_C2F:
			kind = NODE_C2F;
			break;
		case CONV_F2I:
			kind = NODE_F2I;
			break;
		case CONV_F2C:
			kind = NODE_F2C;
			break;
		default:
			kind = NODE_NOCONV;
	}

	if(kind != NODE_NOCONV){
		leftnode = ast_new_subtree(kind, op1, NULL);
		ast_set_data(leftnode, ast_get_data(op1));
	}

	switch(meta.right){
		case CONV_I2F:
			kind = NODE_I2F;
			break;
		case CONV_I2C:
			kind = NODE_I2C;
			break;
		case CONV_C2I:
			kind = NODE_C2I;
			break;
		case CONV_C2F:
			kind = NODE_C2F;
			break;
		case CONV_F2I:
			kind = NODE_F2I;
			break;
		case CONV_F2C:
			kind = NODE_F2C;
			break;
		default:
			kind = NODE_NOCONV;
	}

	if(kind != NODE_NOCONV){
		rightnode = ast_new_subtree(kind, op2, NULL);
		ast_set_data(rightnode, ast_get_data(op2));
	}

	resdata.lit.type = meta.type;
	*res = ast_new_subtree(nodekind, leftnode, rightnode, NULL);
	ast_set_data(*res, resdata);
}

// void check_assignment(AST* op1, AST* res){
// 	UnaryOperationData meta;
	// Literal* l1;
	// Literal* l2;
	// l1 = ast_get_literal(op1);
	// l2 = ast_get_literal(res);
	// meta.type = l2.type;
	// if(l1 == NULL || l2 == NULL){
	// 	*res = NULL;
	// 	fprintf(stdout, "Got nil when performing operation %s\n", operator_symb);
	// 	return ;
	// }
// 	if(literal_assign(l1, &meta) == -1){
// 		printf("SEMANTIC ERROR (%d): ", yylineno);
// 		printf("incompatible types for operator '=', ");
// 		printf("LHS is '%s' and RHS is '%s'.\n", type_name(l2->type), type_name(op1->type));
// 	}
// }




/* ast related */

void ast_manager_add_declarator(AST** declarators, AST* declarator){
	static int declarators_defined = 0;
	if(declarators_defined == 0){
		*declarators = ast_new_node(NODE_DECLARATORS);
		declarators_defined = 1;
	}
	ast_add_child(*declarators, declarator); 
}

AST* ast_manager_new_node_literal(NodeKind kind, Literal lit){
	AST* node = ast_new_node(kind);
	NodeData data = {.lit = lit};
	ast_set_data(node, data);
	return node;
}

AST* ast_manager_new_node_id(char* name){
	if(var_check_declaration(name) == -1){
		return NULL;
	}
	Variable* var = var_search(name);
	AST* node = ast_new_node(NODE_VAR_USE);
	NodeData data = {.var = *var};
	ast_set_data(node, data);
	return node;
}

AST* ast_manager_new_node_function(NodeKind kind, char* name, enum Type ret){
	AST* node = NULL;
	NodeData data;
	switch(kind){
		case NODE_SCANF:
		case NODE_PRINTF:
		case NODE_FUNC_DECL:
			data.lit = (Literal) {.type =  ret };
			node = ast_new_node(kind);
			ast_set_data(node, data);
			break;
		default:
	}
	return node;
}

AST* ast_manager_new_node_fcall(char* name){
	AST* node = ast_new_node(NODE_FUNC_USE);
	NodeData data;
	data.lit = (Literal) {.type = function_get_returntype(name) };
	ast_set_data(node, data);
	return node;
}
