//https://llvm.org/docs/LangRef.html#instruction-reference
//https://llvm.org/docs/LangRef.html#module-structure
//https://llvm.org/docs/tutorial/MyFirstLanguageFrontend/LangImpl03.html
//https://llvm.org/docs/LangRef.html#memory-access-and-addressing-operations
//https://llvm.org/docs/LangRef.html#pointer-type

#include "backend/llvm.h"
#include "types/AST.h"
#include "types/Function.h"
#include "types/Type.h"

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#define LLVM_NEW_INT_REG() (registerID++)

FILE* fdump;
int registerID = 1;
int indentLevel;
extern int var_count;
int *llvm_memory;

/* internal */
void indent();
const char* llvm_get_type(enum Type type);
int llvm_genIR_recursive(AST* root);

/* expr */
int llvm_genIR_vardecl(AST* node);
int llvm_genIR_varuse(AST* node);
int llvm_genIR_value(AST* node);
int llvm_genIR_binop(AST* ast, const char* opcode);
int llvm_genIR_sum(AST* ast);
int llvm_genIR_sub(AST* ast);
int llvm_genIR_mul(AST* ast);
int llvm_genIR_div(AST* ast);
int llvm_genIR_mod(AST* ast);

/* func */
void llvm_genIR_function_definition(AST* fnode);
int llvm_genIR_function_return(AST* ret);

int llvm_genIR_recursive(AST* root);

void llvm_genIR(AST* root, FILE* fdout){
	fdump = fdout;
	indentLevel = 0;
	llvm_memory = calloc(var_count, sizeof(int));
	llvm_genIR_recursive(root);
	indentLevel = 0;
	free(llvm_memory);
	fdump = NULL;
}

/* internal */
void indent(){
	for(int i=0; i<indentLevel; i++){
		fprintf(fdump, "\t");
	}
}

const char* llvm_get_type(enum Type type){
	switch(type){
		case TYPE_INT:
			return "i32";
		default:
			return "";
	}
}

int llvm_genIR_recursive(AST* root){
	NodeKind kind = ast_get_kind(root);
	/* printf("Entering %s\n", ast_kind2str(kind)); */

	switch(kind){
		// Expressions
		case NODE_FLT_VAL:
			break;
		case NODE_INT_VAL:
			return llvm_genIR_value(root);
		case NODE_STR_VAL:
			break;
		case NODE_CHR_VAL:
			break;
		case NODE_C2I:
			break;
		case NODE_F2I:
			break;
		case NODE_I2F:
			break;
		case NODE_C2F:
			break;
		case NODE_F2C:
			break;
		case NODE_I2C:
			break;
		case NODE_NOCONV:
			break;
		case NODE_PLUS:
			return llvm_genIR_sum(root);
		case NODE_MINUS:
			return llvm_genIR_sub(root);
		case NODE_TIMES:
			return llvm_genIR_mul(root);
		case NODE_OVER:
			return llvm_genIR_div(root);
		case NODE_MOD:
			return llvm_genIR_mod(root);
		case NODE_EQ:
			break;
		case NODE_LT:
			break;
		case NODE_GT:
			break;
		case NODE_NE:
			break;
		case NODE_OR:
			break;
		case NODE_AND:
			break;

		// Simple variables
		case NODE_ASSIGN:
			break;
		case NODE_VAR_USE:
			return llvm_genIR_varuse(root);
		case NODE_VAR_LIST:
			break;
		case NODE_VAR_DECL:
			return llvm_genIR_vardecl(root);

		// Functions
		case NODE_FUNC:
			llvm_genIR_function_definition(root);
			return -1;
		case NODE_FUNC_PARAMLIST:
			break;
		case NODE_FUNC_BODY:
			break;
		case NODE_FUNC_RET:
			llvm_genIR_function_return(root);
			return -1;
		case NODE_FUNC_USE:
			break;
		case NODE_FCALL:
			break;
		case NODE_SCANF:
			break;
		case NODE_PRINTF:
			break;

		// Arrays (compound variables)
		case NODE_ARRAY_VAL:
			break;

		// Other
		case NODE_PROGRAM:
			break;
		case NODE_BLOCK:
			break;
		case NODE_IF:
			break;
		case NODE_WHILE:
			break;
	}

	unsigned childCount = ast_get_children_count(root);
	for(unsigned i=0; i<childCount; i++){
		llvm_genIR_recursive(ast_get_child(root, i));
	}
	/* printf("Exiting %s\n", ast_kind2str(kind)); */

	return -1;
}


/* expr */
int llvm_genIR_value(AST* node){
	enum Type vtype = ast_get_type(node);
	const char* llvmtype = llvm_get_type(vtype);
	Literal* l = ast_get_literal(node);
	int reg1 = LLVM_NEW_INT_REG();
	int reg2 = LLVM_NEW_INT_REG();
	switch(vtype){
		case TYPE_INT:
  			// %1 = alloca i32, align 4
			indent();
			fprintf(fdump, "%%%d = alloca %s, align 4\n", reg1, llvmtype);

  			// store i32 0, i32* %1, align 4
			indent();
			fprintf(fdump, "store %s %d, ptr %%%d, align 4\n", llvmtype, l->value.i, reg1);

			//%4 = load i32, ptr %2, align 4
			indent();
			fprintf(fdump, "%%%d = load %s, ptr %%%d, align 4\n", reg2, llvmtype, reg1);
			break;
		default:
			break;
	}
	return reg2;
}

int llvm_genIR_vardecl(AST* node){
	enum Type vtype = ast_get_type(node);
	const char* llvmtype = llvm_get_type(vtype);
	Variable* v = ast_get_variable(node);
	int reg1 = LLVM_NEW_INT_REG();
	unsigned count = ast_get_children_count(node);
	int has_val = count != 0;
	AST* ininode;
	Literal* l;
	switch(vtype){
		case TYPE_INT:
  			// %1 = alloca i32, align 4
			indent();
			fprintf(fdump, "%%%d = alloca %s, align 4\n", reg1, llvmtype);
			llvm_memory[v->addr] = reg1;
			if(has_val){
				// store i32 0, i32* %1, align 4
				ininode = ast_get_child(node, 0);
				l = ast_get_literal(ininode);
				indent();
				fprintf(fdump, "store %s %d, ptr %%%d, align 4\n", llvmtype, l->value.i, reg1);
			}
			break;
		default:
			break;
	}
	return reg1;
}

int llvm_genIR_varuse(AST* node){
	enum Type vtype = ast_get_type(node);
	const char* llvmtype = llvm_get_type(vtype);
	Variable* v = ast_get_variable(node);
	int reg1 = LLVM_NEW_INT_REG();
	switch(vtype){
		case TYPE_INT:
			indent();
			fprintf(fdump, "%%%d = load %s, ptr %%%d, align 4\n", reg1, llvmtype, llvm_memory[v->addr]);
			break;
		default:
			break;
	}
	return reg1;
}

int llvm_genIR_binop(AST* ast, const char* opcode){
	//%0 = add i32 %X, %X 
	AST* L = ast_get_child(ast, 0);
	AST* R = ast_get_child(ast, 1);
	int Lval = llvm_genIR_recursive(L);
	int Rval = llvm_genIR_recursive(R);
	int resVal = LLVM_NEW_INT_REG();
	const char* retname = llvm_get_type(ast_get_type(ast));

	indent();
	fprintf(fdump, "%%%d = %s %s %%%d, %%%d\n", resVal, opcode, retname, Lval, Rval);
	
	return resVal;
}

int llvm_genIR_sum(AST* ast){
	return llvm_genIR_binop(ast, "sum");
}

int llvm_genIR_sub(AST* ast){
	return llvm_genIR_binop(ast, "sub");
}

int llvm_genIR_mul(AST* ast){
	return llvm_genIR_binop(ast, "mul");
}

int llvm_genIR_div(AST* ast){
	return llvm_genIR_binop(ast, "sdiv");
}

int llvm_genIR_mod(AST* ast){
	return llvm_genIR_binop(ast, "srem");
}

/* func */
int llvm_genIR_function_return(AST* ret){
	enum Type rettype = ast_get_type(ret);
	const char* llvmtypename = llvm_get_type(rettype);
	unsigned count = ast_get_children_count(ret);
	if(count == 0){
		assert(0);
		indent();
		fprintf(fdump, "ret %s\n", llvmtypename);
		return LLVM_NEW_INT_REG();
	}

	int reg = llvm_genIR_recursive(ast_get_child(ret, 0));
	switch(rettype){
		case TYPE_INT:
			indent();
			fprintf(fdump, "ret %s %%%d\n", llvmtypename, reg);
			return reg;
		case TYPE_FLT:
			break;
		case TYPE_CHAR:
			break;
		case TYPE_STR:
			break;
		default:
			break;
	}

	return -1;
}

void llvm_genIR_function_definition(AST* fnode){
	Function* f = ast_get_data(fnode).func.func;
	const char* fname = func_get_name(f);
	enum Type fret = func_get_return(f);
	const char* llvm_ftype = llvm_get_type(fret);
	// unsigned count = ast_get_children_count(fnode);

	//func name
	fprintf(fdump, "define %s @%s", llvm_ftype, fname);

	//func paramlist
	fprintf(fdump, "(");
	llvm_genIR_recursive(ast_get_child(fnode, 0));
	fprintf(fdump, ")");

	//func body
	fprintf(fdump, "{\n");
	indentLevel++;
	llvm_genIR_recursive(ast_get_child(fnode, 1));
	indentLevel--;

	fprintf(fdump, "}\n");
}
