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

//on array constants
//https://llvm.org/docs/LangRef.html#complex-constants

//on array type
//https://llvm.org/docs/LangRef.html#array-type

//ona array access
//https://llvm.org/docs/LangRef.html#getelementptr-instruction

#include "backend/llvm.h"
// #include "parser/VarTable.h"
#include "parser/VarTable.h"
#include "types/AST.h"
#include "types/Function.h"
// #include "types/Scope.h"
#include "types/Scope.h"
#include "types/Type.h"
#include "parser/FuncTable.h"
#include "parser/ScopeManager.h"
#include "utils/Vector.h"
#include "utils/Stack.h"



#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <stdarg.h>



#define LLVM_NEW_INT_REG() (registerID++)
#define llvm_comment1(fmt) llvm_iprint("; "fmt"\n")
#define llvm_comment2(fmt, ...) llvm_iprint("; "fmt"\n", __VA_ARGS__)

//Frame Context
typedef struct BlockFrame {
      int* addrs;
      int size;
} BlockFrame;

/* DONT STOP READING
 *
 * How should this FrameManager struct work:
 * 1. Each element is a funcframe;
 * 2. Each funcframe is associated with one function.
 * 3. A funcframe is a stack of frames.
 * 4. A frame corresponds to a scratch-space memory that gets created every time a function gets called
 * 5. KEEP READING
 *
 * OBS1: From 1) and 2) we can imply that the FrameManager instance would then
 * store a number of elements equal to the number of USER-DEFINED functions in
 * the program
 *
 * OK, BUT THAT'S JUST NOT HOW THE FOLLOWING FRAMEMANAGER STRUCT WAS IMPLEMENTED...
 * WHY?
 *
 * Well, it would definitely possible to do that, but it's convenient to take
 * a different approach (which happens to be more flexible) because of how the
 * 'ScopeManager' struct works.
 *
 * For this parser, the ScopeManager takes care of the creation of all scopes
 * found in the C file during the parse phase
 *
 * The ScopeManager creates one scope for each block found in the code,
 * instead of for each function. Therefore, it has a list of block scopes
 * rather than function scopes, although it keeps track of how each scope
 * relates to each other. By that, i mean that it is possible to find all
 * scopes that belong to an arbitrary function, when using an underlying
 * datastructure appropriate for that purpose (UNION-FIND).
 *
 * Since i would have to implement the union-find scheme and additional code for
 * that, i decided to take a different approach for the FrameManager, which
 * works with blockframes instead of funcframes.
 *
 * I've thought about it for a while and it really works. The only difference
 * is that instead of saving the context (memory and other stuff) when a
 * function gets activated/called, we do the same thing but for each block.
 *
 * Since each block is unique and always begin and end (there's nothing like
 * entering in a block and never quitting (with the exception being for
 * infinite loops and infinite stuff)), this approach is possible.
 *
 * So, for this implementation of FrameManager, the following points are
 * valid instead:
 * 1. Each element is a blockframe;
 * 2. Each blockframe is associated with one block.
 * 3. A blockframe is a stack of blocks.
 * 4. A blockframe corresponds to a scratch-space memory that gets created every time a block gets activated
 *
 * OBS1: From 1) and 2) we can imply that the FrameManager instance stores a
 * number of elements equal to the number of blocks in the program
 *
 * OBS2: 'blockframes' is a Vector of Stacks
 *
 * OBS3: each blockframe is a Stack, where the top element is the current blockframe underexecution.
 *
 * OBS4: if a blockframe is empty, that means that that block is deactivated,
 * that is, it hasn't been called yet.
 * */
typedef struct FrameManager {
      Stack** blockframes;
      int nblocks;
      int currentblock;
} FrameManager ;

enum arghint {
      ARG_NOHINT = 1,
      ARG_FLT2DOUBLE = 2,
};



FILE* fdump;
int registerID = 1; //this should be unsigned actually
unsigned ifID = 1;
unsigned whileID = 1;
unsigned paramID = 1;
int indentLevel;
extern int var_count; // parser.y
extern Vector* stringliterals; // lexer.l
extern FuncTable* functable; // parser.y
extern ScopeManager* scope_manager; // parser.y
FrameManager llvm_memory;



/* MEMORY OF REGISTERS */
void framemanager_init(void);
void framemanager_destroy(void);
int framemanager_read(AST* var);
void framemanager_write(AST* var, int reg);
void framemanager_push(AST* block);
void framemanager_pop(AST* block);
int framemanager_current_block(void);
int framemanager_print(void);
int in_global_scope();
int var_in_global_scope(AST* var);
BlockFrame* frame_new(unsigned sz);
void frame_destroy(void* frame);

/* core */
int llvm_genIR_recursive(AST* root);
void llvm_genIR_extfuncs(void);
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
void llvm_genIR_fcall_arg(AST* arg, int reg, enum arghint hint);
void llvm_genIR_block(AST* block);

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
	framemanager_init();
	llvm_genIR_strtable();
	llvm_genIR_extfuncs();
	llvm_genIR_recursive(root);
	indentLevel = 0;
	framemanager_destroy();
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


		// variables (simple and arrays)
		case NODE_ASSIGN:
			return llvm_genIR_assign(root);
		case NODE_VAR_USE:
			return llvm_genIR_varuse(root);
		case NODE_VAR_DECL:
			return llvm_genIR_vardecl(root);
		case NODE_PTR_VAL:
			llvm_comment1("TODO: Loading ptr val\n");
			return -1;



		// Functions
		case NODE_FUNC:
			llvm_genIR_function_definition(root);
			break ;
		case NODE_FUNC_RET:
			return llvm_genIR_function_return(root);
		case NODE_FCALL:
			return llvm_genIR_fcall(root);
		case NODE_SCANF:
			return llvm_genIR_scanf(root);
		case NODE_PRINTF:
			return llvm_genIR_printf(root);
		case NODE_BLOCK: //its a long story
			llvm_genIR_block(root);
			return -1;


		// C statements
		case NODE_IF:
			llvm_genIR_if(root);
			return -1;
		case NODE_WHILE:
			llvm_genIR_while(root);
			return -1;


		// Node kinds handled exclusively by their parents
		case NODE_ARRAY_VAL:
		case NODE_NOCONV: //this type of node does not get included in the AST
		case NODE_FUNC_PARAMLIST:
		case NODE_FUNC_BODY:
			break ;


		// Node kinds which don't need to be handled (only their children)
		case NODE_PROGRAM: //has 0 or 1 child
		case NODE_VAR_LIST: //has 1 or more children
			// printf("Node %s: starting to transverse children...\n", ast_kind2str(kind));
			for(unsigned i=0; i<childCount; i++){
				llvm_genIR_recursive(ast_get_child(root, i));
			}
			break;
			// printf("Node %s: transverse completed\n", ast_kind2str(kind));
	}

	return -1;
}

void llvm_genIR_extfuncs(void){
	llvm_print("; Dumping external funcs\n");
	llvm_print("declare i32 @printf(i8*, ...)\n");
	llvm_print("declare i32 @scanf(i8*, ...)\n");
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
			if(str[j] != '\\'){
				llvm_print("%c", str[j]);
				continue;
			}
			switch(str[++j]){
				case 'n':
					llvm_print("\\%02X", '\n');
					break;
				case 't':
					llvm_print("\\%02X", '\t');
					break;
				default:
					break;
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
	unsigned count = ast_get_children_count(node);
	enum Type node_type = ast_get_type(node);
	const char* llvm_type = llvm_get_type(node_type);
	int llvm_size = llvm_get_size(node_type);

	int has_val = count != 0;
	AST* ininode = has_val ? ast_get_child(node, 0) : NULL;

	int inglobalscope = in_global_scope();
	int isarray = has_val && (ast_get_kind(ininode) == NODE_ARRAY_VAL);
	int array_has_init = isarray ? ast_get_children_count(ininode) > 0 : 0;

	int reginit = !inglobalscope && has_val ? llvm_genIR_recursive(ininode) : -1;
	int regres = LLVM_NEW_INT_REG();

	llvm_comment2("Declaring %s variable (array=%d, global=%d)", llvm_type, isarray, inglobalscope);

	if(node_type != TYPE_FLT && node_type != TYPE_INT && node_type != TYPE_CHAR){
	    assert(0);
	}

	framemanager_write(node, regres);
	if(isarray && inglobalscope){ //array in global scope
		NodeData adata = ast_get_data(ininode);
		unsigned nelems = adata.var.var.qualifier;
		if(array_has_init){ //not supported for now
			//@Z = global [2 x ptr] [ ptr @X, ptr @Y ]
			assert(0);
		}
		else{
			llvm_iprint("@g%d = global [%u x %s] zeroinitializer, align %d\n", regres, nelems, llvm_type, llvm_size);
		}
	}
	else if(isarray && !inglobalscope){ //array in local scope
		NodeData adata = ast_get_data(ininode);
		unsigned nelems = adata.var.var.qualifier;
		if(array_has_init){ //not supported for now
			assert(0);
		}
		else{
			llvm_iprint("%%%d = alloca [%u x %s], align %d\n", regres, nelems, llvm_type, llvm_size);
		}
	}
	else if(inglobalscope){ //simple variable in global scope
		if(has_val && ast_has_literal(ininode)){ //we only initialize it if the initializer is a literal
			Literal* l = ast_get_literal(ininode);;
			switch(node_type){
				case TYPE_INT:
					llvm_iprint("@g%d = global %s %d, align %d\n", regres, llvm_type, l->value.i, llvm_size);
					break;
				case TYPE_FLT:
					llvm_iprint("@g%d = global %s %f, align %d\n", regres, llvm_type, l->value.f, llvm_size);
					break;
				case TYPE_CHAR:
					llvm_iprint("@g%d = global %s %c, align %d\n", regres, llvm_type, l->value.c, llvm_size);
					break;
				default:
					llvm_iprint("@g%d = global %s zeroinitializer, align %d\n", regres, llvm_type, llvm_size);
					break;
			}
		}
		else{
			llvm_iprint("@g%d = global %s zeroinitializer, align %d\n", regres, llvm_type, llvm_size);
		}
	}
	else{ //simple variable in local scope
		llvm_iprint("%%%d = alloca %s, align %d\n", regres, llvm_type, llvm_size);
		if(has_val){
			llvm_iprint("store %s %%%d, ptr %%%d, align %d\n", llvm_type, reginit, regres, llvm_size);
		}
	}

	llvm_print("\n");

	return regres;
}

int llvm_genIR_varuse(AST* node){
	enum Type node_type = ast_get_type(node);
	const char* llvm_type = llvm_get_type(node_type);
	int llvm_size = llvm_get_size(node_type);
	int indexedvariable = ast_get_children_count(node) > 0;
	int regres = -1;

	if(node_type != TYPE_FLT && node_type != TYPE_INT && node_type != TYPE_CHAR){
		assert(0);
	}

	if(ast_is_array(node)){
		regres = LLVM_NEW_INT_REG();
		llvm_comment2("Reading %s array from memory", llvm_type);
		if(var_in_global_scope(node)){
			llvm_iprint("%%%d = getelementptr inbounds [%d x %s], ptr @g%d, i64 0, i64 0\n", regres, ast_get_data(node).var.var.qualifier, llvm_type, framemanager_read(node));
		}
		else{
			llvm_iprint("%%%d = getelementptr inbounds [%d x %s], ptr %%%d, i64 0, i64 0\n", regres, ast_get_data(node).var.var.qualifier, llvm_type, framemanager_read(node));
		}
	}
	else if(indexedvariable){
		int idxreg = llvm_genIR_recursive(ast_get_child(node, 0));
		int elemreg = LLVM_NEW_INT_REG();
		regres = LLVM_NEW_INT_REG();

		llvm_comment2("Reading %s array from memory", llvm_type);
		if(var_in_global_scope(node)){
			llvm_iprint("%%%d = getelementptr inbounds %s, ptr @g%d, i32 %%%d\n", elemreg, llvm_type, framemanager_read(node), idxreg);
		}
		else{
			llvm_iprint("%%%d = getelementptr inbounds %s, ptr %%%d, i32 %%%d\n", elemreg, llvm_type, framemanager_read(node), idxreg);
		}
		llvm_iprint("%%%d = load %s, ptr %%%d, align %d\n", regres, llvm_type, elemreg, llvm_size);
	}
	else{

		llvm_comment2("Reading %s variable from memory", llvm_type);
		regres = LLVM_NEW_INT_REG();
		if(var_in_global_scope(node)){
			llvm_iprint("%%%d = load %s, ptr @g%d, align %d\n", regres, llvm_type, framemanager_read(node), llvm_size);
		}
		else{
			llvm_iprint("%%%d = load %s, ptr %%%d, align %d\n", regres, llvm_type, framemanager_read(node), llvm_size);
		}
	}
	llvm_print("\n");

	return regres;
}

int llvm_genIR_assign(AST* node){
	AST* lhs = ast_get_child(node, 0);
	AST* rhs = ast_get_child(node, 1);
	enum Type node_type = ast_get_type(lhs);
	Variable* var = ast_get_variable(lhs);
	const char* llvm_type = llvm_get_type(node_type);
	int llvm_size = llvm_get_size(node_type);
	int isarray = ast_get_children_count(lhs) > 0;
	int rhsreg = -1;
	int lhsreg = -1;
	if(node_type != TYPE_FLT && node_type != TYPE_INT && node_type != TYPE_CHAR){
		assert(0);
	}

	if(isarray){
		rhsreg = llvm_genIR_recursive(rhs);
		AST* idx = ast_get_child(lhs, 0);
		int idxreg = llvm_genIR_recursive(idx);
		lhsreg = framemanager_read(lhs);
		int elemreg = LLVM_NEW_INT_REG();

		llvm_comment2("Writing to %s variable (%s, array=%d) in memory", llvm_type, var->name, isarray);
		if(var_in_global_scope(lhs)){
			llvm_iprint("%%%d = getelementptr inbounds %s, ptr @g%d, i32 %%%d\n", elemreg, llvm_type, lhsreg, idxreg);
			llvm_iprint("store %s %%%d, ptr %%%d, align %d\n", llvm_type, rhsreg, elemreg, llvm_size);
		}
		else{
			llvm_iprint("%%%d = getelementptr inbounds %s, ptr %%%d, i32 %%%d\n", elemreg, llvm_type, lhsreg, idxreg);
			llvm_iprint("store %s %%%d, ptr %%%d, align %d\n", llvm_type, rhsreg, elemreg, llvm_size);
		}
	}
	else{
		rhsreg = llvm_genIR_recursive(rhs);
		lhsreg = framemanager_read(lhs);

		llvm_comment2("Writing to %s variable (%s, array=%d) in memory", llvm_type, var->name, isarray);
		if(var_in_global_scope(lhs)){
			llvm_iprint("store %s %%%d, ptr @g%d, align %d\n", llvm_type, rhsreg, lhsreg, llvm_size);
		}
		else{
			llvm_iprint("store %s %%%d, ptr %%%d, align %d\n", llvm_type, rhsreg, lhsreg, llvm_size);
		}
	}

	llvm_print("\n");
	return lhsreg;
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
	if(ifchild){
		//since its the 'block' non terminal token (from parser.y)
		//which gives birth to 'ifchild', 'ifchild' could be NULL
		//(because 'block' can be NULL). This is true for 'elsechild'
		//as well and any other AST node where that's the case. Sorry
		//for only pointing that out here lmao (if this interests you
		//for any reason).
		llvm_genIR_recursive(ifchild);
	}
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
	llvm_genIR_block(block);

	llvm_comment1("looping");
	llvm_IR_goto(condlabel);

	llvm_IR_label(escapelabel);
	llvm_comment1("--- while");
}



/* FUNCTIONS */

void llvm_genIR_function_definition(AST* fnode){
	registerID = 1;

	Function* f = ast_get_data(fnode).func.func;
	if(!func_is_defined(f)){
		return ;
	}

	enum Type fret = func_get_return(f);
	const char* fname = func_get_name(f);
	const char* llvm_ftype = llvm_get_type(fret);


	llvm_comment1("Writing function");

	//func name
	llvm_print("define %s @%s", llvm_ftype, fname);

	//func paramlist
	AST* paramlist = ast_get_child(fnode, 0);
	size_t nparams = ast_get_children_count(paramlist);
	AST* child;

	if(nparams == 0){
		llvm_print("()");
	}
	else{
		child = ast_get_child(paramlist, 0);
		llvm_print("(%s %%arg0", ast_is_pointerlike(child) ? "ptr" : llvm_get_type(ast_get_type(child)));
		for(size_t i=1; i<nparams; i++){
			child = ast_get_child(paramlist, i);
			llvm_print(", %s %%arg%lu", ast_is_pointerlike(child) ? "ptr" : llvm_get_type(ast_get_type(child)), i);
		}
		llvm_print(")");
	}

	//func body
	AST* fbody = ast_get_child(fnode, 1);
	AST* fblock = ast_get_child(fbody, 0);
	if(fblock == NULL){
		//the function return type is void when it is defined but it
		//doesn't have a block
		llvm_print("{\n");
		indentLevel++;
		llvm_iprint("ret void\n");
		indentLevel--;
		llvm_print("}\n\n");
	}
	else if(nparams == 0){
		llvm_print("{\n");
		indentLevel++;
		llvm_genIR_block(fblock);
		indentLevel--;
		llvm_print("}\n\n");
	}
	else{
		llvm_print("{\n");
		indentLevel++;

		llvm_comment1("Generating arglist glue");
		int glueregs[nparams];
		enum Type argtype;
		for(size_t i=0; i<nparams; i++){
			//%par0, %par1, %par2, ...
			glueregs[i] = LLVM_NEW_INT_REG();
			child = ast_get_child(paramlist, i);
			argtype = ast_get_type(child);
			if(ast_is_pointerlike(child)){
				llvm_iprint("%%%d = alloca ptr, align 8\n", glueregs[i]);
				llvm_iprint("store ptr %%arg%lu, ptr %%%d, align 8\n", i, glueregs[i]);
				int oldreg = glueregs[i];
				glueregs[i] = LLVM_NEW_INT_REG();
				llvm_iprint("%%%d = load ptr, ptr %%%d, align 8\n\n", glueregs[i], oldreg);
			}
			else{
				llvm_iprint("%%%d = alloca %s, align %d\n", glueregs[i], llvm_get_type(argtype), llvm_get_size(argtype));
				llvm_iprint("store %s %%arg%lu, ptr %%%d, align %d\n\n", llvm_get_type(argtype), i, glueregs[i], llvm_get_size(argtype));
			}
		}
		llvm_print("\n");

		//We have to perform a 'fake' execution of a function in order
		//to build its body. The reason for that is because the
		//generation of the funcbody code depends on registers
		//which only exist during the function execution. More
		//specifically, the code generation itself references
		//registers which are obtained from the function Frame. For
		//that reason, we first push a frame for this function,
		//generate the code and then pop that frame.

		framemanager_push(fblock);

		for(size_t i=0; i<nparams; i++){
			child = ast_get_child(paramlist, i);
			framemanager_write(child, glueregs[i]);
		}

		unsigned childCount = ast_get_children_count(fblock);
		for(unsigned i=0; i<childCount; i++){
			llvm_genIR_recursive(ast_get_child(fblock, i));
		}

		framemanager_pop(fblock);

		indentLevel--;
		llvm_print("}\n\n");
	}
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

	//well, apparently this is needed for recursive calls.
	//in other words, if we return 'reg', then the return value for this
	//fcall won't be in an independet/separate temporary register, which
	//will violate the SSA policy (don't ask my why, this was proven to be
	//true after testing this a lot)
	int resreg = LLVM_NEW_INT_REG();

	return resreg;
}

int llvm_genIR_printlike_dummy(AST* ast, const char* fname){
	//printf call: %1 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([15 x i8], [15 x i8]* @.str, i64 0, i64 0))
	size_t childCount = ast_get_children_count(ast);
	assert(childCount != 0);

	llvm_comment2("+++ %s()\n", fname);

	// generating the registers for the parameters prior to writing the paramlist
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

	// funcname
	int reg1 = LLVM_NEW_INT_REG();
	llvm_iprint("%%%d = call i32 (i8*, ...) @%s", reg1, fname);

	// writing arglist
	llvm_print("(");
	llvm_genIR_fcall_arg(ast_get_child(ast, 0), argregs[0], ARG_FLT2DOUBLE);
	if(childCount > 1){
		for(size_t i=1; i<childCount; i++){
			llvm_print(", ");
			llvm_genIR_fcall_arg(ast_get_child(ast, i), argregs[i], ARG_FLT2DOUBLE);
		}
	}
	llvm_print(")\n");

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
	size_t childCount = ast_get_children_count(ast);
	NodeData data = ast_get_data(ast);
	Function* f = data.func.func;
	const char* fname = func_get_name(f);
	enum Type fret = func_get_return(f);
	const char* fretname = llvm_get_type(fret);
	int regret = -1;

	llvm_comment2("+++ %s()", fname);

	if(childCount > 0){
		AST* child;
		int argregs[childCount];

		/* expanding arguments */
		for(size_t i=0; i<childCount; i++){
			child = ast_get_child(ast, i);
			argregs[i] = llvm_genIR_recursive(child);
			// if(ast_is_array(child)){
			// 	argregs[i] = LLVM_NEW_INT_REG();
			// 	llvm_iprint("%%%d = alloca [%d x %s]\n", argregs[i], ast_get_data(child).var.var.qualifier, llvm_get_type(ast_get_type(child)));
			// 	argregs[i] = LLVM_NEW_INT_REG();
			// 	llvm_iprint("%%%d = load [%d x %s], \n", argregs[i], ast_get_data(child).var.var.qualifier, llvm_get_type(ast_get_type(child)));
			// }
		}

		/* writing llvm ir funccall */
		if(fret != TYPE_VOID){
			regret = LLVM_NEW_INT_REG();
			llvm_iprint("%%%d = call %s @%s(", regret, fretname, fname);
		}
		else{
			llvm_iprint("call %s @%s(", fretname, fname);
		}

		//writing paramlist
		llvm_genIR_fcall_arg(ast_get_child(ast, 0), argregs[0], ARG_NOHINT);
		if(childCount > 1){
			for(size_t i=1; i<childCount; i++){
				llvm_print(", ");
				llvm_genIR_fcall_arg(ast_get_child(ast, i), argregs[i], ARG_NOHINT);
			}
		}
		llvm_print(")\n");
	}
	else{
		if(fret != TYPE_VOID){
			regret = LLVM_NEW_INT_REG();
			llvm_iprint("%%%d = call %s @%s()\n", regret, fretname, fname);
		}
		else{
			llvm_iprint("call %s @%s()\n", fretname, fname);
		}
	}

	llvm_comment2("--- %s()", fname);
	return regret;
}

void llvm_genIR_fcall_arg(AST* arg, int reg, enum arghint hint){
	enum Type ttype = ast_get_type(arg);
	const char* llvmtname = llvm_get_type(ttype);
	size_t chcount;
	if(ast_is_array(arg)){
		// llvm_print(
		//     "%s* getelementptr inbounds ("
		//     "[%lu x %s], "
		//     "ptr %%%d, "
		//     "i64 0, "
		//     "i64 0)",
		//     llvmtname, ast_get_data(arg).var.var.qualifier, llvmtname, reg
		// );
		llvm_print(
		    "ptr noundef %%%d", reg
		);
		return ;
	}
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
			    "i8* getelementptr inbounds ("
			    "[%lu x i8], "
			    "[%lu x i8]* @.str%d, "
			    "i64 0, "
			    "i64 0)",
			    chcount, chcount, reg
			);
			break;
		default:
			break;
	}
}

void llvm_genIR_block(AST* block){
	if(block == NULL){
		return ;
	}
	framemanager_push(block);
	llvm_comment2("+++ Block %d entered\n", framemanager_current_block());

	unsigned childCount = ast_get_children_count(block);
	for(unsigned i=0; i<childCount; i++){
		llvm_genIR_recursive(ast_get_child(block, i));
	}

	llvm_comment2("--- Block %d exited\n", framemanager_current_block());
	framemanager_pop(block);
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
	int testreg = LLVM_NEW_INT_REG();

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





/* MEMORY */

void framemanager_init(void){
	llvm_memory.nblocks = scope_manager_get_size(scope_manager);
	llvm_memory.blockframes = calloc(llvm_memory.nblocks, sizeof(void*));
	for(unsigned i=0; i<llvm_memory.nblocks; i++){
	      llvm_memory.blockframes[i] = stack_new(8);
	}

	//initializing global blockframe
	const Scope* globalscope = scope_manager_get_scope(scope_manager, 0);
	VarTable* vt = scope_get_vartable(globalscope);
	Stack* blockframe = llvm_memory.blockframes[0];
	llvm_memory.currentblock = 0;
	unsigned sz = vartable_get_size(vt);
	BlockFrame* frame = frame_new(sz);
	stack_push(blockframe, frame);
}

void framemanager_destroy(void){
	if(llvm_memory.blockframes){
	      for(unsigned i=0; i<llvm_memory.nblocks; i++){
		    stack_destroy(llvm_memory.blockframes + i, frame_destroy);
	      }
	      free(llvm_memory.blockframes);
	}
	llvm_memory.nblocks = 0;
	llvm_memory.currentblock = -1;
	llvm_memory.blockframes = NULL;
}

int framemanager_read(AST* var){
	Scope* scope = ast_get_scope(var);
	int scopeid = scope_get_id(scope);
	Stack* blockframe = llvm_memory.blockframes[scopeid];
	BlockFrame* frame = stack_top(blockframe);
	Variable* v = ast_get_variable(var);
	// printf("var %s (scope=%d) should be at %d... using reg %d\n", v->name, scopeid, v->reladdr, frame->addrs[v->reladdr]);
	return frame->addrs[v->reladdr];
}

void framemanager_write(AST* var, int reg){
	Scope* scope = ast_get_scope(var);
	int scopeid = scope_get_id(scope);
	Stack* blockframe = llvm_memory.blockframes[scopeid];
	BlockFrame* frame = stack_top(blockframe);

	Variable* v = ast_get_variable(var);
	frame->addrs[v->reladdr] = reg;
	// printf("var %s (scope=%d) should be at %d... writing reg %d\n", v->name, scopeid, v->reladdr, frame->addrs[v->reladdr]);
}

void framemanager_push(AST* block){
	Scope* scope = ast_get_scope(block);
	VarTable* vt = scope_get_vartable(scope);
	int scopeid = scope_get_id(scope);
	llvm_memory.currentblock = scopeid;
	Stack* blockframe = llvm_memory.blockframes[scopeid];
	BlockFrame* frame = frame_new(vartable_get_size(vt));
	stack_push(blockframe, frame);
}

void framemanager_pop(AST* block){
	Scope* scope = ast_get_scope(block);
	int scopeid = scope_get_id(scope);
	llvm_memory.currentblock = scope_get_parent(scope);
	Stack* blockframe = llvm_memory.blockframes[scopeid];
	BlockFrame* frame = stack_pop(blockframe);
	frame_destroy(frame);
}

int framemanager_current_block(void){
	return llvm_memory.currentblock;
}

int in_global_scope(){
	return llvm_memory.currentblock == 0;
}

int var_in_global_scope(AST* var){
	Scope* scope = ast_get_scope(var);
	assert(scope);
	return scope_get_id(scope) == 0;
}

BlockFrame* frame_new(unsigned sz){
	BlockFrame* frame = calloc(1, sizeof(BlockFrame));
	frame->size = sz;
	frame->addrs = calloc(sz, sizeof(int));
	return frame;
}

void frame_destroy(void* frame){
	BlockFrame* bframe = frame;
	free(bframe->addrs);
	free(bframe);
}
