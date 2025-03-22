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



#include "backend/llvm.h"
#include "types/AST.h"
#include "types/Function.h"
#include "types/Type.h"
#include "parser/FuncTable.h"
#include "utils/Vector.h"



#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>



#define LLVM_NEW_INT_REG() (registerID++)



enum arghint {
      ARG_NOHINT = 1,
      ARG_FLT2DOUBLE = 2,
};



FILE* fdump;
int registerID = 1;
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
int llvm_genIR_convdummy(const char* opcode, int fromreg, enum Type from, enum Type to);
int llvm_genIR_conv(AST* node);
int llvm_genIR_vardecl(AST* node);
int llvm_genIR_varuse(AST* node);
int llvm_genIR_assign(AST* node);
int llvm_genIR_binop(const char* opcode, const char* retname, int regL, int regR);
int llvm_genIR_binlop(const char* opcode, const char* cond, const char* retname, int regL, int regR);
int llvm_genIR_lt(AST* ast);
int llvm_genIR_gt(AST* ast);
int llvm_genIR_eq(AST* ast);
int llvm_genIR_ne(AST* ast);
int llvm_genIR_and(AST* ast);
int llvm_genIR_or(AST* ast);
int llvm_genIR_sum(AST* ast);
int llvm_genIR_sub(AST* ast);
int llvm_genIR_mul(AST* ast);
int llvm_genIR_div(AST* ast);
int llvm_genIR_rem(AST* ast);

/* func */
void llvm_genIR_function_definition(AST* fnode);
int llvm_genIR_function_return(AST* ret);
int llvm_genIR_printf(AST* ast);
int llvm_genIR_fcall(AST* ast);
void llvm_genIR_fcall_arg(enum Type ttype, int reg, enum arghint hint);
void llvm_genIR_paramlist(AST* ast, size_t childCount, int argregs[childCount], enum arghint hint);

/* helpers */
void indent();
size_t count_chars(const char* str);
const char* llvm_get_type(enum Type type);




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
			return llvm_genIR_conv(root);
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
			// indent();
			// fprintf(fdump, "; TODO: == operation\n\n");
			// return -1;
			return llvm_genIR_eq(root);
		case NODE_LT:
			// indent();
			// fprintf(fdump, "; TODO: < operation\n\n");
			// return -1;
			return llvm_genIR_lt(root);
		case NODE_GT:
			// indent();
			// fprintf(fdump, "; TODO: > operation\n\n");
			// return -1;
			return llvm_genIR_gt(root);
		case NODE_NE:
			// indent();
			// fprintf(fdump, "; TODO: != operation\n\n");
			// return -1;
			return llvm_genIR_ne(root);
		case NODE_OR:
			// indent();
			// fprintf(fdump, "; TODO: || operation\n\n");
			// return -1;
			return llvm_genIR_or(root);
		case NODE_AND:
			// indent();
			// fprintf(fdump, "; TODO: && operation\n\n");
			// return -1;
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
			indent();
			fprintf(fdump, "; TODO: fcall\n\n");
			return -1;
		case NODE_SCANF:
			indent();
			fprintf(fdump, "; TODO: scanf\n\n");
			return -1;
		case NODE_PRINTF:
			return llvm_genIR_printf(root);


		// Arrays (compound variables)
		case NODE_ARRAY_VAL:
			indent();
			fprintf(fdump, "; TODO: Loading array val\n\n");
			return -1;
		case NODE_PTR_VAL:
			indent();
			fprintf(fdump, "; TODO: Loading ptr val\n\n");
			return -1;


		// Node kinds handled exclusively by their parents
		case NODE_FUNC_PARAMLIST:
		case NODE_NOCONV: //this type of node does not get included in the AST
			break ;


		// Node kinds which don't need to be handled (only their children)
		case NODE_PROGRAM:
		case NODE_VAR_LIST:
		case NODE_BLOCK:
		case NODE_IF:
		case NODE_WHILE:
		case NODE_FUNC_BODY:
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
	fprintf(fdump, "; Dumping ftable\n");

	fprintf(fdump, "declare i32 @printf(i8*, ...)\n");
	fprintf(fdump, "declare i32 @scanf(i8*, ...)\n");

	fprintf(fdump, "\n");
}

void llvm_genIR_strtable(void){
	fprintf(fdump, "; Dumping string table\n");

	size_t count = vector_get_size(stringliterals);
	const char* str;
	size_t sz;
	size_t bytes;
	for(size_t i=0; i<count; i++){
		str = vector_get_item(stringliterals, i);
		sz = count_chars(str);
		bytes = strlen(str);
		fprintf(fdump, "@.str%lu = private unnamed_addr constant [%lu x i8] c\"", i, sz + 1);
		for(size_t j=0; j<bytes; j++){
		  if(str[j] == '\\'){
		    switch(str[j+1]){
		      case 'n':
			fprintf(fdump, "\\%02X", '\n');
			break;
		      case 't':
			fprintf(fdump, "\\%02X", '\t');
			break;
		      default:
			break;
		    }
		    j++;
		  }
		  else{
		    fprintf(fdump, "%c", str[j]);
		  }
		}
		fprintf(fdump, "\\00\", align 1\n");
	}

	fprintf(fdump, "\n");
}




/* EXPR */

int llvm_genIR_value(AST* node){
	enum Type vtype = ast_get_type(node);
	const char* llvmtype = llvm_get_type(vtype);
	Literal* l = ast_get_literal(node);
	int reg1 = -1;
	int reg2 = -1;

	indent();
	fprintf(fdump, "; Loading %s literal\n", llvmtype);
	switch(vtype){
		case TYPE_INT:
			reg1 = LLVM_NEW_INT_REG();
			reg2 = LLVM_NEW_INT_REG();

			indent();
			fprintf(fdump, "%%%d = alloca %s, align 4\n", reg1, llvmtype);

			indent();
			fprintf(fdump, "store %s %d, ptr %%%d, align 4\n", llvmtype, l->value.i, reg1);

			indent();
			fprintf(fdump, "%%%d = load %s, ptr %%%d, align 4\n", reg2, llvmtype, reg1);
			break;
		case TYPE_FLT:
			reg1 = LLVM_NEW_INT_REG();
			reg2 = LLVM_NEW_INT_REG();

			indent();
			fprintf(fdump, "%%%d = alloca %s, align 4\n", reg1, llvmtype);

			indent();
			fprintf(fdump, "store %s %f, ptr %%%d, align 4\n", llvmtype, l->value.f, reg1);

			indent();
			fprintf(fdump, "%%%d = load %s, ptr %%%d, align 4\n", reg2, llvmtype, reg1);
			break;
		case TYPE_CHAR:
			reg1 = LLVM_NEW_INT_REG();
			reg2 = LLVM_NEW_INT_REG();

			indent();
			fprintf(fdump, "%%%d = alloca %s, align 1\n", reg1, llvmtype);

			indent();
			fprintf(fdump, "store %s %d, ptr %%%d, align 1\n", llvmtype, l->value.c, reg1);

			indent();
			fprintf(fdump, "%%%d = load %s, ptr %%%d, align 1\n", reg2, llvmtype, reg1);
			break;
		case TYPE_STR:
			reg2 = l->value.i;
			indent();
			fprintf(fdump, "; \"%s\"@%d\n", (char*) vector_get_item(stringliterals, reg2), reg2);
			break;
		default:
			break;
	}

	fprintf(fdump, "\n");

	return reg2;
}

int llvm_genIR_conv(AST* node){
	NodeKind kind = ast_get_kind(node);
	int src = llvm_genIR_recursive(ast_get_child(node, 0));
	switch(kind){
		case NODE_C2F:
			return llvm_genIR_convdummy("uitofp", src, TYPE_CHAR, TYPE_FLT);
		case NODE_C2I:
			return llvm_genIR_convdummy("zext", src, TYPE_CHAR, TYPE_INT);
		case NODE_I2C:
			return llvm_genIR_convdummy("trunc", src, TYPE_INT, TYPE_CHAR);
		case NODE_I2F:
			return llvm_genIR_convdummy("sitofp", src, TYPE_INT, TYPE_FLT);
		case NODE_F2I:
			return llvm_genIR_convdummy("fptosi", src, TYPE_FLT, TYPE_INT);
		case NODE_F2C:
			return llvm_genIR_convdummy("fptoui", src, TYPE_FLT, TYPE_CHAR);
		default:
			assert(0);
			break;
	}
	return src;
}

int llvm_genIR_convdummy(const char* opcode, int fromreg, enum Type from, enum Type to){
	const char* fromname = llvm_get_type(from);
	const char* toname = llvm_get_type(to);
	int reg = LLVM_NEW_INT_REG();

	indent();
	fprintf(fdump, "; Converting from %s to %s\n", fromname, toname);

	indent();
	fprintf(fdump, "%%%d = %s %s %%%d to %s\n", reg, opcode, fromname, fromreg, toname);

	fprintf(fdump, "\n");

	return reg;
}

int llvm_genIR_vardecl(AST* node){
	enum Type vtype = ast_get_type(node);
	Variable* v = ast_get_variable(node);
	unsigned count = ast_get_children_count(node);
	int has_val = count != 0;
	AST* ininode = has_val ? ast_get_child(node, 0) : NULL;
	int reg1 = has_val ? llvm_genIR_recursive(ininode) : -1;
	const char* llvmtype = llvm_get_type(vtype);
	int reg2 = LLVM_NEW_INT_REG();

	indent();
	fprintf(fdump, "; Declaring %s variable\n", llvmtype);

	switch(vtype){
		case TYPE_FLT:
		case TYPE_INT:
			indent();
			fprintf(fdump, "%%%d = alloca %s, align 4\n", reg2, llvmtype);
			llvm_memory[v->addr] = reg2;
			if(has_val){
				indent();
				fprintf(fdump, "store %s %%%d, ptr %%%d, align 4\n", llvmtype, reg1, reg2);
			}
			break;
		case TYPE_CHAR:
			indent();
			fprintf(fdump, "%%%d = alloca %s, align 1\n", reg2, llvmtype);
			llvm_memory[v->addr] = reg2;
			if(has_val){
				indent();
				fprintf(fdump, "store %s %%%d, ptr %%%d, align 1\n", llvmtype, reg1, reg2);
			}
			break;
		default:
			assert(0);
			break;
	}

	fprintf(fdump, "\n");

	return reg2;
}

int llvm_genIR_varuse(AST* node){
	enum Type vtype = ast_get_type(node);
	const char* llvmtype = llvm_get_type(vtype);
	Variable* v = ast_get_variable(node);
	int reg1 = LLVM_NEW_INT_REG();

	indent();
	fprintf(fdump, "; Reading %s variable from memory\n", llvmtype);

	switch(vtype){
		case TYPE_FLT:
		case TYPE_INT:
			indent();
			fprintf(fdump, "%%%d = load %s, ptr %%%d, align 4\n", reg1, llvmtype, llvm_memory[v->addr]);
			break;
		case TYPE_CHAR:
			indent();
			fprintf(fdump, "%%%d = load %s, ptr %%%d, align 1\n", reg1, llvmtype, llvm_memory[v->addr]);
			break;
		default:
			assert(0);
			break;
	}

	fprintf(fdump, "\n");

	return reg1;
}

int llvm_genIR_assign(AST* node){
	AST* lhs = ast_get_child(node, 0);
	AST* rhs = ast_get_child(node, 1);
	enum Type vtype = ast_get_type(lhs);
	Variable* var = ast_get_variable(lhs);
	const char* llvmtype = llvm_get_type(vtype);

	indent();
	fprintf(fdump, "; Writing to %s variable (%s) in memory\n", llvmtype, var->name);

	int reg1 = llvm_genIR_recursive(rhs);
	int reg2 = LLVM_NEW_INT_REG();

	switch(vtype){
		case TYPE_FLT:
		case TYPE_INT:
			indent();
			fprintf(fdump, "%%%d = alloca %s, align 4\n", reg2, llvmtype);

			indent();
			fprintf(fdump, "store %s %%%d, ptr %%%d, align 4\n", llvmtype, reg1, reg2);

			llvm_memory[var->addr] = reg2;
			break;
		case TYPE_CHAR:
			indent();
			fprintf(fdump, "%%%d = alloca %s, align 1\n", reg2, llvmtype);

			indent();
			fprintf(fdump, "store %s %%%d, ptr %%%d, align 1\n", llvmtype, reg1, reg2);

			llvm_memory[var->addr] = reg2;
			break;
		default:
			assert(0);
			break;
	}

	fprintf(fdump, "\n");
	return reg2;
}

int llvm_genIR_binlop(const char* opcode, const char* cond, const char* retname, int regL, int regR){
	//%0 = add i32 %X, %X 
	int tmpVal = LLVM_NEW_INT_REG();
	int resVal = LLVM_NEW_INT_REG();

	indent();
	fprintf(fdump, "; Performing %s\n", opcode);

	indent();
	fprintf(fdump, "%%%d = %s %s %s %%%d, %%%d\n", tmpVal, opcode, cond, retname, regL, regR);

	indent();
	fprintf(fdump, "%%%d = zext i1 %%%d to i32\n", resVal, tmpVal);

	fprintf(fdump, "\n");

	return resVal;
}

int llvm_genIR_lt(AST* ast){
	AST* L = ast_get_child(ast, 0);
	AST* R = ast_get_child(ast, 1);
	int regL = llvm_genIR_recursive(L);
	int regR = llvm_genIR_recursive(R);
	const char* retname = llvm_get_type(ast_get_type(L));
	switch(ast_get_type(L)){
	  case TYPE_INT:
		  return llvm_genIR_binlop("icmp", "slt", retname, regL, regR);
	  case TYPE_FLT:
		  return llvm_genIR_binlop("fcmp", "olt", retname, regL, regR);
	  default:
		  assert(0);
	}
	return -1;
}

int llvm_genIR_gt(AST* ast){
	AST* L = ast_get_child(ast, 0);
	AST* R = ast_get_child(ast, 1);
	int regL = llvm_genIR_recursive(L);
	int regR = llvm_genIR_recursive(R);
	const char* retname = llvm_get_type(ast_get_type(L));
	switch(ast_get_type(L)){
	  case TYPE_INT:
		  return llvm_genIR_binlop("icmp", "sgt", retname, regL, regR);
	  case TYPE_FLT:
		  return llvm_genIR_binlop("fcmp", "ogt", retname, regL, regR);
	  default:
		  assert(0);
	}
	return -1;
}

int llvm_genIR_eq(AST* ast){
	AST* L = ast_get_child(ast, 0);
	AST* R = ast_get_child(ast, 1);
	int regL = llvm_genIR_recursive(L);
	int regR = llvm_genIR_recursive(R);
	const char* retname = llvm_get_type(ast_get_type(L));
	switch(ast_get_type(L)){
	  case TYPE_INT:
		  return llvm_genIR_binlop("icmp", "eq", retname, regL, regR);
	  case TYPE_FLT:
		  return llvm_genIR_binlop("fcmp", "oeq", retname, regL, regR);
	  default:
		  assert(0);
	}
	return -1;
}

int llvm_genIR_ne(AST* ast){
	AST* L = ast_get_child(ast, 0);
	AST* R = ast_get_child(ast, 1);
	int regL = llvm_genIR_recursive(L);
	int regR = llvm_genIR_recursive(R);
	const char* retname = llvm_get_type(ast_get_type(L));
	switch(ast_get_type(L)){
	  case TYPE_INT:
		  return llvm_genIR_binlop("icmp", "ne", retname, regL, regR);
	  case TYPE_FLT:
		  return llvm_genIR_binlop("fcmp", "one", retname, regL, regR);
	  default:
		  assert(0);
	}
	return -1;
}

int llvm_genIR_and(AST* ast){
	AST* L = ast_get_child(ast, 0);
	AST* R = ast_get_child(ast, 1);
	int regL = llvm_genIR_recursive(L);
	int regR = llvm_genIR_recursive(R);
	enum Type rettype = ast_get_type(ast);
	const char* retname = llvm_get_type(rettype);
	return llvm_genIR_binop("and", retname, regL, regR);
}

int llvm_genIR_or(AST* ast){
	AST* L = ast_get_child(ast, 0);
	AST* R = ast_get_child(ast, 1);
	int regL = llvm_genIR_recursive(L);
	int regR = llvm_genIR_recursive(R);
	enum Type rettype = ast_get_type(ast);
	const char* retname = llvm_get_type(rettype);
	return llvm_genIR_binop("or", retname, regL, regR);
}

int llvm_genIR_binop(const char* opcode, const char* retname, int regL, int regR){
	//%0 = add i32 %X, %X 
	int resVal = LLVM_NEW_INT_REG();

	indent();
	fprintf(fdump, "; Performing %s\n", opcode);

	indent();
	fprintf(fdump, "%%%d = %s %s %%%d, %%%d\n", resVal, opcode, retname, regL, regR);

	fprintf(fdump, "\n");

	return resVal;
}

int llvm_genIR_sum(AST* ast){
	AST* L = ast_get_child(ast, 0);
	AST* R = ast_get_child(ast, 1);
	int regL = llvm_genIR_recursive(L);
	int regR = llvm_genIR_recursive(R);
	enum Type rettype = ast_get_type(ast);
	const char* retname = llvm_get_type(rettype);

	switch(rettype){
	  case TYPE_INT:
		  return llvm_genIR_binop("add", retname, regL, regR);
	  case TYPE_FLT:
		  return llvm_genIR_binop("fadd", retname, regL, regR);
	  default:
		  assert(0);
	}
	return -1;
}

int llvm_genIR_sub(AST* ast){
	AST* L = ast_get_child(ast, 0);
	AST* R = ast_get_child(ast, 1);
	int regL = llvm_genIR_recursive(L);
	int regR = llvm_genIR_recursive(R);
	enum Type rettype = ast_get_type(ast);
	const char* retname = llvm_get_type(rettype);
	switch(rettype){
	  case TYPE_INT:
	    return llvm_genIR_binop("sub", retname, regL, regR);
	  case TYPE_FLT:
	    return llvm_genIR_binop("fsub", retname, regL, regR);
	  default:
	    assert(0);
	}
	return -1;
}

int llvm_genIR_mul(AST* ast){
	AST* L = ast_get_child(ast, 0);
	AST* R = ast_get_child(ast, 1);
	int regL = llvm_genIR_recursive(L);
	int regR = llvm_genIR_recursive(R);
	enum Type rettype = ast_get_type(ast);
	const char* retname = llvm_get_type(rettype);
	switch(rettype){
	  case TYPE_INT:
	    return llvm_genIR_binop("mul", retname, regL, regR);
	  case TYPE_FLT:
	    return llvm_genIR_binop("fmul", retname, regL, regR);
	  default:
	    assert(0);
	}
	return -1;
}

int llvm_genIR_div(AST* ast){
	AST* L = ast_get_child(ast, 0);
	AST* R = ast_get_child(ast, 1);
	int regL = llvm_genIR_recursive(L);
	int regR = llvm_genIR_recursive(R);
	enum Type rettype = ast_get_type(ast);
	const char* retname = llvm_get_type(rettype);
	switch(rettype){
	  case TYPE_INT:
	    return llvm_genIR_binop("sdiv", retname, regL, regR);
	  case TYPE_FLT:
	    return llvm_genIR_binop("fdiv", retname, regL, regR);
	  default:
	    assert(0);
	}
	return -1;
}

int llvm_genIR_rem(AST* ast){
	AST* L = ast_get_child(ast, 0);
	AST* R = ast_get_child(ast, 1);
	int regL = llvm_genIR_recursive(L);
	int regR = llvm_genIR_recursive(R);
	enum Type rettype = ast_get_type(ast);
	const char* retname = llvm_get_type(rettype);
	switch(rettype){
	  case TYPE_INT:
	    return llvm_genIR_binop("srem", retname, regL, regR);
	  case TYPE_FLT:
	    return llvm_genIR_binop("frem", retname, regL, regR);
	  default:
	    assert(0);
	}
	return -1;
}




/* FUNCTIONS */

void llvm_genIR_function_definition(AST* fnode){
	Function* f = ast_get_data(fnode).func.func;
	const char* fname = func_get_name(f);
	enum Type fret = func_get_return(f);
	const char* llvm_ftype = llvm_get_type(fret);

	indent();
	fprintf(fdump, "; Writing function\n");

	//func name
	fprintf(fdump, "define %s @%s", llvm_ftype, fname);

	//func paramlist
	AST* paramlist = ast_get_child(fnode, 0);
	size_t childCount = ast_get_children_count(paramlist);
	if(childCount == 0){
		fprintf(fdump, "()");
	}
	else{
		llvm_ftype = llvm_get_type(ast_get_type(ast_get_child(paramlist, 0)));
		fprintf(fdump, "(");
		fprintf(fdump, "%s", llvm_ftype);
		for(size_t i=1; i<childCount; i++){
			llvm_ftype = llvm_get_type(ast_get_type(ast_get_child(paramlist, i)));
			fprintf(fdump, ", %s", llvm_ftype);
		}
		fprintf(fdump, ")");
	}

	//func body
	fprintf(fdump, "{\n");
	indentLevel++;
	llvm_genIR_recursive(ast_get_child(fnode, 1));
	indentLevel--;

	indent();
	fprintf(fdump, "}\n");

	indent();
	fprintf(fdump, "\n");
}

int llvm_genIR_function_return(AST* ret){
	enum Type rettype = ast_get_type(ret);
	const char* llvmtypename = llvm_get_type(rettype);
	unsigned count = ast_get_children_count(ret);
	assert(count != 0);

	int reg = llvm_genIR_recursive(ast_get_child(ret, 0));

	indent();
	fprintf(fdump, "; Writing function return\n");

	indent();
	fprintf(fdump, "ret %s %%%d\n", llvmtypename, reg);

	fprintf(fdump, "\n");

	return reg;
}

int llvm_genIR_printf(AST* ast){
	//  %1 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([15 x i8], [15 x i8]* @.str, i64 0, i64 0))
	size_t childCount = ast_get_children_count(ast);
	assert(childCount != 0);

	indent();
	fprintf(fdump, ";; Calling printf\n");

	AST* child;
	int argregs[childCount];
	for(size_t i=0; i<childCount; i++){
		child = ast_get_child(ast, i);
		argregs[i] = llvm_genIR_recursive(child);
		if(ast_get_type(child) == TYPE_FLT){
			int tmp = LLVM_NEW_INT_REG();
			indent();
			fprintf(fdump, "%%%d = fpext float %%%d to double\n", tmp, argregs[i]);
			argregs[i] = tmp;
		}
	}

	int reg1 = LLVM_NEW_INT_REG();

	indent();
	fprintf(fdump, "%%%d = call i32 (i8*, ...) @printf", reg1);
	llvm_genIR_paramlist(ast, childCount, argregs, ARG_FLT2DOUBLE);

	indent();
	fprintf(fdump, ";; End of printf\n\n");

	return reg1;
}

int llvm_genIR_fcall(AST* ast){
	//  %1 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([15 x i8], [15 x i8]* @.str, i64 0, i64 0))
	// size_t childCount = ast_get_children_count(ast);
	int reg1 = LLVM_NEW_INT_REG();
	NodeData data = ast_get_data(ast);
	Function* f = data.func.func;
	// enum Type fret = func_get_return(f);
	const char* fname = func_get_name(f);

	indent();
	fprintf(fdump, "; Calling function %s\n", fname);

	fprintf(fdump, "\n");

	return reg1;
}

void llvm_genIR_paramlist(AST* ast, size_t childCount, int argregs[childCount], enum arghint hint){
	fprintf(fdump, "(");
	enum Type ttype = ast_get_type(ast_get_child(ast, 0));
	llvm_genIR_fcall_arg(ttype, argregs[0], hint);
	if(childCount > 1){
		for(size_t i=1; i<childCount; i++){
			fprintf(fdump, ", ");
			ttype = ast_get_type(ast_get_child(ast, i));
			llvm_genIR_fcall_arg(ttype, argregs[i], hint);
		}
	}
	fprintf(fdump, ")\n");
}

void llvm_genIR_fcall_arg(enum Type ttype, int reg, enum arghint hint){
	//  i8* getelementptr inbounds ([15 x i8], [15 x i8]* @.str, i64 0, i64 0)
	//  <result> = getelementptr <ty>, <N x ptr> <ptrval>, <vector index type> <idx>
	const char* llvmtname = llvm_get_type(ttype);
	size_t chcount;
	switch(ttype){
		case TYPE_FLT:
			if(hint == ARG_FLT2DOUBLE){
			      fprintf(fdump, "double %%%d", reg);
			      break;
			}
		case TYPE_INT:
		case TYPE_CHAR:
			fprintf(fdump, "%s %%%d", llvmtname, reg);
			break;
		case TYPE_STR:
			chcount = count_chars(vector_get_item(stringliterals, reg));
			fprintf(fdump,
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
		default:
			return "";
	}
}
