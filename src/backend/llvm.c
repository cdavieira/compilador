#include "backend/llvm.h"
#include "types/AST.h"
#include "types/Function.h"
#include "types/Type.h"

#include <stdio.h>

//https://llvm.org/docs/LangRef.html#instruction-reference
//https://llvm.org/docs/LangRef.html#module-structure
//https://llvm.org/docs/tutorial/MyFirstLanguageFrontend/LangImpl03.html

FILE* fdump;

void llvm_genIR_recursive(AST* root);

void llvm_genIR(AST* root, FILE* fdout){
	fdump = fdout;
}

const char* llvm_get_type(enum Type type){
	switch(type){
		case TYPE_INT:
			return "i32";
		default:
			return "";
	}
}

void llvm_genIR_function_return(AST* ret){
	fprintf(fdump, "");
}

void llvm_genIR_function_definition(AST* fnode){
	Function* f = ast_get_data(fnode).func.func;
	const char* fname = func_get_name(f);
	enum Type fret = func_get_return(f);

	const char* llvm_ftype = llvm_get_type(fret);
	fprintf(fdump, "define %s @%s{", llvm_ftype, fname);
	for(int i=0; i<3; i++){
		llvm_genIR_recursive(fnode);
	}
	fprintf(fdump, "}");
}

void llvm_genIR_plus(){

}

void llvm_genIR_recursive(AST* root){
	NodeKind kind = ast_get_kind(root);
	switch(kind){
		// Expressions
		case NODE_FLT_VAL:
		case NODE_INT_VAL:
		case NODE_STR_VAL:
		case NODE_CHR_VAL:
		case NODE_C2I:
		case NODE_F2I:
		case NODE_I2F:
		case NODE_C2F:
		case NODE_F2C:
		case NODE_I2C:
		case NODE_NOCONV:
		case NODE_PLUS:
		case NODE_MINUS:
		case NODE_TIMES:
		case NODE_OVER:
		case NODE_MOD:
		case NODE_EQ:
		case NODE_LT:
		case NODE_GT:
		case NODE_NE:
		case NODE_OR:
		case NODE_AND:

		// Simple variables
		case NODE_ASSIGN:
		case NODE_VAR_USE:
		case NODE_VAR_LIST:
		case NODE_VAR_DECL:

		// Functions
		case NODE_FUNC:
		case NODE_FUNC_PARAMLIST:
		case NODE_FUNC_BODY:
		case NODE_FUNC_RET:
		case NODE_FUNC_USE:
		case NODE_FCALL:
		case NODE_SCANF:
		case NODE_PRINTF:

		// Arrays (compound variables)
		case NODE_ARRAY_VAL:

		// Other
		case NODE_PROGRAM:
		case NODE_BLOCK:
		case NODE_IF:
		case NODE_WHILE:
			break;
	}
	
}
