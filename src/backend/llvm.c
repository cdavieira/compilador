//reference index
//https://llvm.org/docs/LangRef.html#instruction-reference

//on printf maybe?
//https://llvm.org/docs/LangRef.html#parameter-attributes
//https://llvm.org/docs/LangRef.html#module-structure

//on function syntax
//https://llvm.org/docs/LangRef.html#functions

//on function linkage
//https://llvm.org/docs/LangRef.html#linkage-types

//on identifiers
//https://llvm.org/docs/LangRef.html#identifiers

//on global identifiers
//https://llvm.org/docs/LangRef.html#global-variables

//on i8
//https://llvm.org/docs/LangRef.html#single-value-types

//on getelementptr
//https://llvm.org/docs/LangRef.html#constant-expressions

//on add, sub, mul, div and other
//https://llvm.org/docs/LangRef.html#binary-operations

//on alloca
//https://llvm.org/docs/LangRef.html#memory-access-and-addressing-operations

//on ptr
//https://llvm.org/docs/LangRef.html#pointer-type

//on conversion operations
//https://llvm.org/docs/LangRef.html#conversion-operations

//on writing strings
//https://stackoverflow.com/questions/14710427/what-really-is-0a-00-in-llvm-string

//on if stmts
//https://llvm.org/docs/LangRef.html#br-instruction


#include "backend/llvm.h"
// #include "parser/VarTable.h"
#include "types/AST.h"
#include "types/Function.h"
// #include "types/Scope.h"
#include "types/Type.h"
#include "parser/FuncTable.h"
#include "utils/Vector.h"



#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <stdarg.h>



#define LLVM_NEW_INT_REG() (registerID++)
#define llvm_comment1(fmt) llvm_iprint("; "fmt"\n")
#define llvm_comment2(fmt, ...) llvm_iprint("; "fmt"\n", __VA_ARGS__)



enum arghint {
      ARG_NOHINT = 1,
      ARG_FLT2DOUBLE = 2,
};



FILE* fdump;
int registerID = 1; //this should be unsigned actually
unsigned ifID = 1;
unsigned whileID = 1;
int indentLevel;
int *llvm_memory;
extern int var_count; // parser.y
extern Vector* stringliterals; // lexer.l
extern FuncTable* functable; // parser.y



/* core */
int llvm_genIR_recursive(AST* root);
void llvm_genIR_ftable(void);
void llvm_genIR_strtable(void);

/* expr */
int llvm_genIR_value(AST* node);
int llvm_IR_conv(AST* node);
int llvm_genIR_conv(const char* opcode, int fromreg, enum Type from, enum Type to);
int llvm_genIR_vardecl(AST* node);
int llvm_genIR_varuse(AST* node);
int llvm_genIR_assign(AST* node);

int llvm_IR_binlop(AST* ast, const char* opcode, const char* cond);
int llvm_genIR_binlop(AST* ast, const char* icode, const char* fcode);
int llvm_genIR_lt(AST* ast);
int llvm_genIR_gt(AST* ast);
int llvm_genIR_eq(AST* ast);
int llvm_genIR_ne(AST* ast);

int llvm_IR_binop(AST* ast, const char* opcode);
int llvm_genIR_binop(AST* ast, const char* icode, const char* fcode);
int llvm_genIR_and(AST* ast);
int llvm_genIR_or(AST* ast);
int llvm_genIR_sum(AST* ast);
int llvm_genIR_sub(AST* ast);
int llvm_genIR_mul(AST* ast);
int llvm_genIR_div(AST* ast);
int llvm_genIR_rem(AST* ast);

/* c stmt */
void llvm_genIR_if(AST* ast);
void llvm_genIR_while(AST* ast);

/* func */
void llvm_genIR_function_definition(AST* fnode);
int llvm_genIR_function_return(AST* ret);
int llvm_genIR_printlike_dummy(AST* ast, const char* fname);
int llvm_genIR_printf(AST* ast);
int llvm_genIR_scanf(AST* ast);
int llvm_genIR_fcall(AST* ast);
void llvm_genIR_fcall_arg(enum Type ttype, int reg, enum arghint hint);
void llvm_genIR_paramlist(AST* ast, size_t childCount, int argregs[childCount], enum arghint hint);

/* helpers */
void indent();
void llvm_print(const char* fmt, ...);
void llvm_iprint(const char* fmt, ...);
void llvm_IR_label(const char* label);
void llvm_IR_goto(const char* label);
void llvm_IR_condgoto(int reg, char* labeltrue, char* labelfalse);
size_t count_chars(const char* str);
const char* llvm_get_type(enum Type type);
int llvm_get_size(enum Type type);




/* CORE */

void llvm_genIR(AST* root, FILE* fdout){
	fdump = fdout;
	indentLevel = 0;
	llvm_memory = calloc(var_count, sizeof(int));
	llvm_genIR_strtable();
	llvm_genIR_ftable();
	llvm_genIR_recursive(root);
	indentLevel = 0;
	free(llvm_memory);
	fdump = NULL;
}

int llvm_genIR_recursive(AST* root){
	NodeKind kind = ast_get_kind(root);
	unsigned childCount = ast_get_children_count(root);

	// printf("Node %s visited\n", ast_kind2str(kind));

	switch(kind){
		// Expressions
		case NODE_FLT_VAL:
		case NODE_INT_VAL:
		case NODE_CHR_VAL:
		case NODE_STR_VAL:
			return llvm_genIR_value(root);
		case NODE_C2I:
		case NODE_F2I:
		case NODE_I2F:
		case NODE_C2F:
		case NODE_F2C:
		case NODE_I2C:
			return llvm_IR_conv(root);
		case NODE_PLUS:
			return llvm_genIR_sum(root);
		case NODE_MINUS:
			return llvm_genIR_sub(root);
		case NODE_TIMES:
			return llvm_genIR_mul(root);
		case NODE_OVER:
			return llvm_genIR_div(root);
		case NODE_MOD:
			return llvm_genIR_rem(root);
		case NODE_EQ:
			return llvm_genIR_eq(root);
		case NODE_LT:
			return llvm_genIR_lt(root);
		case NODE_GT:
			return llvm_genIR_gt(root);
		case NODE_NE:
			return llvm_genIR_ne(root);
		case NODE_OR:
			return llvm_genIR_or(root);
		case NODE_AND:
			return llvm_genIR_and(root);


		// Simple variables
		case NODE_ASSIGN:
			return llvm_genIR_assign(root);
		case NODE_VAR_USE:
			return llvm_genIR_varuse(root);
		case NODE_VAR_DECL:
			return llvm_genIR_vardecl(root);


		// Functions
		case NODE_FUNC:
			llvm_genIR_function_definition(root);
			break ;
		case NODE_FUNC_RET:
			return llvm_genIR_function_return(root);
		case NODE_FCALL:
			llvm_comment1("TODO: fcall\n");
			break;
		case NODE_SCANF:
			return llvm_genIR_scanf(root);
		case NODE_PRINTF:
			return llvm_genIR_printf(root);


		// Arrays (compound variables)
		case NODE_ARRAY_VAL:
			llvm_comment1("TODO: Loading array val\n");
			return -1;
		case NODE_PTR_VAL:
			llvm_comment1("TODO: Loading ptr val\n");
			return -1;


		// C statements
		case NODE_IF:
			llvm_genIR_if(root);
			return -1;
		case NODE_WHILE:
			llvm_genIR_while(root);
			return -1;


		// Node kinds handled exclusively by their parents
		case NODE_FUNC_PARAMLIST:
		case NODE_NOCONV: //this type of node does not get included in the AST
			break ;


		// Node kinds which don't need to be handled (only their children)
		case NODE_PROGRAM: //has 0 or 1 child
		case NODE_FUNC_BODY: //has 0 or 1 children
		case NODE_VAR_LIST: //has 1 or more children
		case NODE_BLOCK: //has 1 or more children
			// printf("Node %s: starting to transverse children...\n", ast_kind2str(kind));
			for(unsigned i=0; i<childCount; i++){
				llvm_genIR_recursive(ast_get_child(root, i));
			}
			break;
			// printf("Node %s: transverse completed\n", ast_kind2str(kind));
	}

	return -1;
}

void llvm_genIR_ftable(void){
	llvm_print("; Dumping ftable\n");

	llvm_print("declare i32 @printf(i8*, ...)\n");
	llvm_print("declare i32 @scanf(i8*, ...)\n");

	// unsigned count = func_table_get_size(functable);
	// Function* f;
	// Scope* scope;
	// VarTable* vt;
	// Variable* var;
	// const char* fname;
	// const char* llvmtypename;
	// int nparams;
	// for(unsigned i=2; i<count; i++){
	// 	f = func_table_get(functable, i);

	// 	nparams = func_get_nparams(f);

	// 	fname = func_get_name(f);
	// 	llvmtypename = llvm_get_type(func_get_return(f));
	// 	scope = func_get_scope(f);
	// 	vt = scope_get_vartable(scope);

	// 	llvm_print("declare %s @%s(", llvmtypename, fname);
	// 	if(nparams != 0){
	// 		var = vartable_idx(vt, 0);
	// 		llvm_print("%s", llvm_get_type(var->type));
	// 		for(int j=1; j<nparams; j++){
	// 			llvm_print(", %s", llvm_get_type(var->type));
	// 		}
	// 	}
	// 	llvm_print(")\n");
	// }

	llvm_print("\n");
}

void llvm_genIR_strtable(void){
	llvm_comment1("Dumping string table");

	size_t count = vector_get_size(stringliterals);
	const char* str;
	size_t sz;
	size_t bytes;
	for(size_t i=0; i<count; i++){
		str = vector_get_item(stringliterals, i);
		sz = count_chars(str);
		bytes = strlen(str);
		llvm_print("@.str%lu = private unnamed_addr constant [%lu x i8] c\"", i, sz + 1);
		for(size_t j=0; j<bytes; j++){
			if(str[j] == '\\'){
				switch(str[j+1]){
					case 'n':
						llvm_print("\\%02X", '\n');
						break;
					case 't':
						llvm_print("\\%02X", '\t');
						break;
					default:
						break;
				}
				j++;
			}
			else{
				llvm_print("%c", str[j]);
			}
		}
		llvm_print("\\00\", align 1\n");
	}

	llvm_print("\n");
}




/* EXPR */

int llvm_genIR_value(AST* node){
	enum Type vtype = ast_get_type(node);
	const char* llvmtype = llvm_get_type(vtype);
	Literal* l = ast_get_literal(node);
	int reg1 = -1;
	int reg2 = -1;

	llvm_comment2("Loading %s literal", llvmtype);
	switch(vtype){
		case TYPE_INT:
			reg1 = LLVM_NEW_INT_REG();
			reg2 = LLVM_NEW_INT_REG();
			llvm_iprint("%%%d = alloca %s, align 4\n", reg1, llvmtype);
			llvm_iprint("store %s %d, ptr %%%d, align 4\n", llvmtype, l->value.i, reg1);
			llvm_iprint("%%%d = load %s, ptr %%%d, align 4\n", reg2, llvmtype, reg1);
			break;
		case TYPE_FLT:
			reg1 = LLVM_NEW_INT_REG();
			reg2 = LLVM_NEW_INT_REG();
			llvm_iprint("%%%d = alloca %s, align 4\n", reg1, llvmtype);
			llvm_iprint("store %s %f, ptr %%%d, align 4\n", llvmtype, l->value.f, reg1);
			llvm_iprint("%%%d = load %s, ptr %%%d, align 4\n", reg2, llvmtype, reg1);
			break;
		case TYPE_CHAR:
			reg1 = LLVM_NEW_INT_REG();
			reg2 = LLVM_NEW_INT_REG();
			llvm_iprint("%%%d = alloca %s, align 1\n", reg1, llvmtype);
			llvm_iprint("store %s %d, ptr %%%d, align 1\n", llvmtype, l->value.c, reg1);
			llvm_iprint("%%%d = load %s, ptr %%%d, align 1\n", reg2, llvmtype, reg1);
			break;
		case TYPE_STR:
			reg2 = l->value.i;
			llvm_comment2("\"%s\"@%d", (char*) vector_get_item(stringliterals, reg2), reg2);
			break;
		default:
			break;
	}

	llvm_print("\n");
	return reg2;
}

int llvm_IR_conv(AST* node){
	NodeKind kind = ast_get_kind(node);
	int src = llvm_genIR_recursive(ast_get_child(node, 0));
	switch(kind){
		case NODE_C2F:
			return llvm_genIR_conv("uitofp", src, TYPE_CHAR, TYPE_FLT);
		case NODE_C2I:
			return llvm_genIR_conv("zext", src, TYPE_CHAR, TYPE_INT);
		case NODE_I2C:
			return llvm_genIR_conv("trunc", src, TYPE_INT, TYPE_CHAR);
		case NODE_I2F:
			return llvm_genIR_conv("sitofp", src, TYPE_INT, TYPE_FLT);
		case NODE_F2I:
			return llvm_genIR_conv("fptosi", src, TYPE_FLT, TYPE_INT);
		case NODE_F2C:
			return llvm_genIR_conv("fptoui", src, TYPE_FLT, TYPE_CHAR);
		default:
			assert(0);
			break;
	}
	return src;
}

int llvm_genIR_conv(const char* opcode, int fromreg, enum Type from, enum Type to){
	const char* fromname = llvm_get_type(from);
	const char* toname = llvm_get_type(to);
	int reg = LLVM_NEW_INT_REG();

	llvm_comment2("Converting from %s to %s", fromname, toname);
	llvm_iprint("%%%d = %s %s %%%d to %s\n\n", reg, opcode, fromname, fromreg, toname);

	return reg;
}

int llvm_genIR_vardecl(AST* node){
	Variable* v = ast_get_variable(node);
	unsigned count = ast_get_children_count(node);
	enum Type vtype = ast_get_type(node);

	const char* llvmtype = llvm_get_type(vtype);
	int llvmsize = llvm_get_size(vtype);
	int has_val = count != 0;
	AST* ininode = has_val ? ast_get_child(node, 0) : NULL;
	int reg1 = has_val ? llvm_genIR_recursive(ininode) : -1;
	int reg2 = LLVM_NEW_INT_REG();

	llvm_comment2("Declaring %s variable", llvmtype);

	switch(vtype){
		case TYPE_FLT:
		case TYPE_INT:
		case TYPE_CHAR:
			llvm_iprint("%%%d = alloca %s, align %d\n", reg2, llvmtype, llvmsize);
			llvm_memory[v->addr] = reg2;
			if(has_val){
				llvm_iprint("store %s %%%d, ptr %%%d, align %d\n", llvmtype, reg1, reg2, llvmsize);
			}
			break;
		default:
			assert(0);
			break;
	}

	llvm_print("\n");

	return reg2;
}

int llvm_genIR_varuse(AST* node){
	Variable* v = ast_get_variable(node);
	enum Type vtype = ast_get_type(node);
	const char* llvmtype = llvm_get_type(vtype);
	int llvmsize = llvm_get_size(vtype);
	int reg1 = LLVM_NEW_INT_REG();

	llvm_comment2("Reading %s variable from memory", llvmtype);
	switch(vtype){
		case TYPE_FLT:
		case TYPE_INT:
		case TYPE_CHAR:
			llvm_iprint("%%%d = load %s, ptr %%%d, align %d\n", reg1, llvmtype, llvm_memory[v->addr], llvmsize);
			break;
		default:
			assert(0);
			break;
	}
	llvm_print("\n");

	return reg1;
}

int llvm_genIR_assign(AST* node){
	AST* lhs = ast_get_child(node, 0);
	AST* rhs = ast_get_child(node, 1);
	enum Type vtype = ast_get_type(lhs);
	Variable* var = ast_get_variable(lhs);
	const char* llvmtype = llvm_get_type(vtype);
	int llvmsize = llvm_get_size(vtype);

	llvm_comment2("Writing to %s variable (%s) in memory", llvmtype, var->name);

	int reg1 = llvm_genIR_recursive(rhs);
	int reg2 = llvm_memory[var->addr];
	switch(vtype){
		case TYPE_FLT:
		case TYPE_INT:
		case TYPE_CHAR:
			llvm_iprint("store %s %%%d, ptr %%%d, align %d\n", llvmtype, reg1, reg2, llvmsize);
			break;
		default:
			assert(0);
			break;
	}

	llvm_print("\n");
	return reg2;
}

int llvm_IR_binlop(AST* ast, const char* opcode, const char* cond){
	AST* L = ast_get_child(ast, 0);
	AST* R = ast_get_child(ast, 1);
	int regL = llvm_genIR_recursive(L);
	int regR = llvm_genIR_recursive(R);
	const char* retname = llvm_get_type(ast_get_type(L));

	int tmpreg = LLVM_NEW_INT_REG();
	int resreg = LLVM_NEW_INT_REG();

	llvm_comment2("Performing %s", opcode);
	llvm_iprint("%%%d = %s %s %s %%%d, %%%d\n", tmpreg, opcode, cond, retname, regL, regR);
	llvm_iprint("%%%d = zext i1 %%%d to i32\n\n", resreg, tmpreg);

	return resreg;
}

int llvm_genIR_binlop(AST* ast, const char* icode, const char* fcode){
	switch(ast_get_type(ast_get_child(ast, 0))){
		case TYPE_INT:
			return llvm_IR_binlop(ast, "icmp", icode);
		case TYPE_FLT:
			return llvm_IR_binlop(ast, "fcmp", fcode);
		default:
			assert(0);
	}
	return -1;
}

int llvm_genIR_lt(AST* ast){
	return llvm_genIR_binlop(ast, "slt", "olt");
}

int llvm_genIR_gt(AST* ast){
	return llvm_genIR_binlop(ast, "sgt", "ogt");
}

int llvm_genIR_eq(AST* ast){
	return llvm_genIR_binlop(ast, "eq", "oeq");
}

int llvm_genIR_ne(AST* ast){
	return llvm_genIR_binlop(ast, "ne", "one");
}

int llvm_IR_binop(AST* ast, const char* opcode){
	enum Type rettype = ast_get_type(ast);
	AST* L = ast_get_child(ast, 0);
	AST* R = ast_get_child(ast, 1);
	int regL = llvm_genIR_recursive(L);
	int regR = llvm_genIR_recursive(R);
	int reg = LLVM_NEW_INT_REG();
	const char* retname = llvm_get_type(rettype);

	llvm_comment2("Performing %s", opcode);
	llvm_iprint("%%%d = %s %s %%%d, %%%d\n\n", reg, opcode, retname, regL, regR);

	return reg;
}

int llvm_genIR_binop(AST* ast, const char* icode, const char* fcode){
	switch(ast_get_type(ast)){
	  case TYPE_INT:
		  return llvm_IR_binop(ast, icode);
	  case TYPE_FLT:
		  return llvm_IR_binop(ast, fcode);
	  default:
		  assert(0);
	}
	return -1;
}

int llvm_genIR_and(AST* ast){
	return llvm_IR_binop(ast, "and");
}

int llvm_genIR_or(AST* ast){
	return llvm_IR_binop(ast, "or");
}

int llvm_genIR_sum(AST* ast){
	return llvm_genIR_binop(ast, "add", "fadd");
}

int llvm_genIR_sub(AST* ast){
	return llvm_genIR_binop(ast, "sub", "fsub");
}

int llvm_genIR_mul(AST* ast){
	return llvm_genIR_binop(ast, "mul", "fmul");
}

int llvm_genIR_div(AST* ast){
	return llvm_genIR_binop(ast, "sdiv", "fdiv");
}

int llvm_genIR_rem(AST* ast){
	return llvm_genIR_binop(ast, "srem", "frem");
}



/* C STMTS */

void llvm_genIR_if(AST* ast){
	llvm_comment1("+++ if");

	AST* expr = ast_get_child(ast, 0);
	AST* ifchild = ast_get_child(ast, 1);
	AST* elsechild = ast_get_child(ast, 2);
	int reg = llvm_genIR_recursive(expr);
	char iflabel[16];
	char elselabel[16];
	char escapelabel[16];

	sprintf(iflabel, "ifif%u", ifID);
	sprintf(elselabel, "ifelse%u", ifID);
	sprintf(escapelabel, "ifesc%u", ifID++);

	llvm_IR_condgoto(reg, iflabel, elsechild ? elselabel : escapelabel);
	llvm_IR_label(iflabel);
	llvm_genIR_recursive(ifchild);
	llvm_IR_goto(escapelabel);

	if(elsechild){
		llvm_IR_label(elselabel);
		llvm_genIR_recursive(elsechild);
		llvm_IR_goto(escapelabel);
	}
	llvm_IR_label(escapelabel);

	llvm_comment1("--- if");
}

void llvm_genIR_while(AST* ast){
	llvm_comment1("+++ while");

	AST* expr = ast_get_child(ast, 0);
	AST* block = ast_get_child(ast, 1);
	char condlabel[16];
	char bodylabel[16];
	char escapelabel[16];
	int cmpreg = -1;

	sprintf(condlabel, "whilecond%u", whileID);
	sprintf(bodylabel, "whilebody%u", whileID);
	sprintf(escapelabel, "whileesc%u", whileID++);

	llvm_IR_goto(condlabel);
	llvm_IR_label(condlabel);

	cmpreg = llvm_genIR_recursive(expr);
	llvm_IR_condgoto(cmpreg, bodylabel, escapelabel);

	llvm_IR_label(bodylabel);
	llvm_genIR_recursive(block);

	llvm_comment1("looping");
	llvm_IR_goto(condlabel);

	llvm_IR_label(escapelabel);
	llvm_comment1("--- while");
}



/* FUNCTIONS */

void llvm_genIR_function_definition(AST* fnode){
	Function* f = ast_get_data(fnode).func.func;
	const char* fname = func_get_name(f);
	enum Type fret = func_get_return(f);
	const char* llvm_ftype = llvm_get_type(fret);

	llvm_comment1("Writing function");

	//func name
	llvm_print("define %s @%s", llvm_ftype, fname);

	//func paramlist
	AST* paramlist = ast_get_child(fnode, 0);
	size_t childCount = ast_get_children_count(paramlist);
	if(childCount == 0){
		llvm_print("()");
	}
	else{
		llvm_print("(%s", llvm_get_type(ast_get_type(ast_get_child(paramlist, 0))));
		for(size_t i=1; i<childCount; i++){
			llvm_print(", %s", llvm_get_type(ast_get_type(ast_get_child(paramlist, i))));
		}
		llvm_print(")");
	}

	//func body
	llvm_print("{\n");
	indentLevel++;
	llvm_genIR_recursive(ast_get_child(fnode, 1));
	indentLevel--;
	llvm_print("}\n\n");
}

int llvm_genIR_function_return(AST* ret){
	enum Type rettype = ast_get_type(ret);
	const char* llvmtypename = llvm_get_type(rettype);
	unsigned count = ast_get_children_count(ret);
	if(count == 0){
		llvm_iprint("ret void\n");
		return -1;
	}

	int reg = llvm_genIR_recursive(ast_get_child(ret, 0));
	llvm_iprint("ret %s %%%d\n\n", llvmtypename, reg);
	return reg;
}

int llvm_genIR_printlike_dummy(AST* ast, const char* fname){
	//  %1 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([15 x i8], [15 x i8]* @.str, i64 0, i64 0))
	size_t childCount = ast_get_children_count(ast);
	assert(childCount != 0);

	llvm_comment2("+++ %s()\n", fname);

	AST* child;
	int argregs[childCount];
	for(size_t i=0; i<childCount; i++){
		child = ast_get_child(ast, i);
		argregs[i] = llvm_genIR_recursive(child);
		if(ast_get_type(child) == TYPE_FLT){
			int tmp = LLVM_NEW_INT_REG();
			llvm_iprint("%%%d = fpext float %%%d to double\n", tmp, argregs[i]);
			argregs[i] = tmp;
		}
	}

	int reg1 = LLVM_NEW_INT_REG();
	llvm_iprint("%%%d = call i32 (i8*, ...) @%s", reg1, fname);
	llvm_genIR_paramlist(ast, childCount, argregs, ARG_FLT2DOUBLE);
	llvm_comment2("--- %s()\n", fname);
	return reg1;
}

int llvm_genIR_printf(AST* ast){
	return llvm_genIR_printlike_dummy(ast, "printf");
}

int llvm_genIR_scanf(AST* ast){
	return llvm_genIR_printlike_dummy(ast, "scanf");
}

int llvm_genIR_fcall(AST* ast){
	int reg1 = LLVM_NEW_INT_REG();
	NodeData data = ast_get_data(ast);
	Function* f = data.func.func;
	const char* fname = func_get_name(f);

	llvm_comment2("+++ %s()\n", fname);
	llvm_comment2("--- %s()\n", fname);
	llvm_print("\n");

	return reg1;
}

void llvm_genIR_paramlist(AST* ast, size_t childCount, int argregs[childCount], enum arghint hint){
	llvm_print("(");
	enum Type ttype = ast_get_type(ast_get_child(ast, 0));
	llvm_genIR_fcall_arg(ttype, argregs[0], hint);
	if(childCount > 1){
		for(size_t i=1; i<childCount; i++){
			llvm_print(", ");
			ttype = ast_get_type(ast_get_child(ast, i));
			llvm_genIR_fcall_arg(ttype, argregs[i], hint);
		}
	}
	llvm_print(")\n");
}

void llvm_genIR_fcall_arg(enum Type ttype, int reg, enum arghint hint){
	//  i8* getelementptr inbounds ([15 x i8], [15 x i8]* @.str, i64 0, i64 0)
	//  <result> = getelementptr <ty>, <N x ptr> <ptrval>, <vector index type> <idx>
	const char* llvmtname = llvm_get_type(ttype);
	size_t chcount;
	switch(ttype){
		case TYPE_FLT:
			if(hint == ARG_FLT2DOUBLE){
			      llvm_print("double %%%d", reg);
			      break;
			}
		case TYPE_INT:
		case TYPE_CHAR:
			llvm_print("%s %%%d", llvmtname, reg);
			break;
		case TYPE_STR:
			chcount = count_chars(vector_get_item(stringliterals, reg));
			llvm_print(
			    "%s getelementptr inbounds ("
			    "[%lu x i8], "
			    "[%lu x i8]* @.str%d, "
			    "i64 0, "
			    "i64 0)",
			    llvmtname, chcount, chcount, reg
			);
			break;
		default:
			break;
	}
}



/* HELPERS */

void indent(){
	for(int i=0; i<indentLevel; i++){
		fprintf(fdump, "\t");
	}
}

void llvm_print(const char* fmt, ...){
	va_list args;
	va_start(args, fmt);
	vfprintf(fdump, fmt, args);
	va_end(args);
}

void llvm_iprint(const char* fmt, ...){
	va_list args;
	va_start(args, fmt);
	indent();
	vfprintf(fdump, fmt, args);
	va_end(args);
}

void llvm_IR_label(const char* label){
	llvm_print("%s:\n", label);
}

void llvm_IR_goto(const char* label){
	llvm_iprint("br label %%%s\n", label);
}

void llvm_IR_condgoto(int reg, char* labeltrue, char* labelfalse){
	// int testreg = LLVM_NEW_INT_REG();
	// llvm_iprint("br i1 %%%d, label %%%s, label %%%s\n", testreg, iterlabel, escapelabel);

	int testreg = LLVM_NEW_INT_REG();

	// llvm_iprint("%%%d = trunc i32 %%%d to i1\n", testreg, reg);
	llvm_iprint("%%%d = icmp ne i32 %%%d, 1\n", testreg, reg);

	llvm_iprint("br i1 %%%d, label %%%s, label %%%s\n", testreg, labelfalse, labeltrue);
}

size_t count_chars(const char* str){
	size_t count = 0;
	size_t bytes = strlen(str);
	for(size_t i=0; i<bytes; i++){
		if(str[i] == '\\'){
			switch(str[i+1]){
				case 'n':
				case 't':
					i++;
					break;
				default:
					break;
			}
		}
		count++;
	}
	return count;
}

const char* llvm_get_type(enum Type type){
	switch(type){
		case TYPE_INT:
			return "i32";
		case TYPE_CHAR:
			return "i8";
		case TYPE_FLT:
			return "float";
		case TYPE_STR:
			return "i8*";
		case TYPE_VOID:
			return "void";
		default:
			assert(0);
	}
	return "";
}

int llvm_get_size(enum Type type){
	switch(type){
		case TYPE_INT:
		case TYPE_FLT:
			return 4;
		case TYPE_CHAR:
			return 1;
		default:
			assert(0);
	}
	return -1;
}
