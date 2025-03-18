
#include <stdio.h>
#include <stdlib.h>
#include "interpreter.h"
#include "ast.h"
#include "tables.h"
#include "types.h"
#include <assert.h>
#include <string.h>

// ----------------------------------------------------------------------------

extern StrTable *st;
extern VarTable *vt;

typedef union {
    int   as_int;
    float as_float;
} Word;

// ----------------------------------------------------------------------------

// Data stack -----------------------------------------------------------------

#define STACK_SIZE 100

Word stack[STACK_SIZE];
int sp; // stack pointer

// All these ops should have a boundary check, buuuut... X_X

void pushi(int x) {
    stack[++sp].as_int = x;
}

int popi() {
    return stack[sp--].as_int;
}

void pushf(float x) {
    stack[++sp].as_float = x;
}

float popf() {
    return stack[sp--].as_float;
}

void init_stack() {
    for (int i = 0; i < STACK_SIZE; i++) {
        stack[i].as_int = 0;
    }
    sp = -1;
}

void print_stack() {
    printf("*** STACK: ");
    for (int i = 0; i <= sp; i++) {
        printf("%d ", stack[i].as_int);
    }
    printf("\n");
}

// ----------------------------------------------------------------------------

// Variables memory -----------------------------------------------------------

#define MEM_SIZE 100

Word mem[MEM_SIZE];

void storei(int addr, int val) {
    mem[addr].as_int = val;
}

int loadi(int addr) {
    return mem[addr].as_int;
}

void storef(int addr, float val) {
    mem[addr].as_float = val;
}

float loadf(int addr) {
    return mem[addr].as_float;
}

void init_mem() {
    for (int addr = 0; addr < MEM_SIZE; addr++) {
        mem[addr].as_int = 0;
    }
}

// ----------------------------------------------------------------------------

// #define TRACE
#ifdef TRACE
#define trace(msg) printf("TRACE: %s\n", msg)
#else
#define trace(msg)
#endif

#define MAX_STR_SIZE 128
static char str_buf[MAX_STR_SIZE];
#define clear_str_buf() str_buf[0] = '\0'

void rec_run_ast(AST *ast);

void read_int(int var_idx) {
    int x;
    printf("read (int): ");
    scanf("%d", &x);
    storei(var_idx, x);
}

void read_real(int var_idx) {
    float x;
    printf("read (real): ");
    scanf("%f", &x);
    storef(var_idx, x);
}

void read_bool(int var_idx) {
    int x;
    do {
        printf("read (bool - 0 = false, 1 = true): ");
        scanf("%d", &x);
    } while (x != 0 && x != 1);
    storei(var_idx, x);
}

void read_str(int var_idx) {
    printf("read (str): ");
    clear_str_buf();
    scanf("%s", str_buf);   // Did anyone say Buffer Overflow..? ;P
    storei(var_idx, add_string(st, str_buf));
}

void write_int() {
    printf("%d\n", popi());
}

void write_real() {
    printf("%f\n", popf());
}

void write_bool() {
    popi() == 0 ? printf("false\n") : printf("true\n");
}

// Helper function to write strings.
void escape_str(const char* s, char *n) {
    int i = 0, j = 0;
    char c;
    while ((c = s[i++]) != '\0') {
        if (c == '"') { continue; }
        else if (c == '\\' && s[i] == 'n') {
            n[j++] = '\n';
            i++;
        } else {
            n[j++] = c;
        }
    }
    n[j] = '\0';
}

void write_str() {
    int s = popi(); // String pointer
    clear_str_buf();
    escape_str(get_string(st, s), str_buf);
    printf("%s", str_buf); // Weird language semantics, if printing a string, no new line.
}

// ----------------------------------------------------------------------------


void run_program(AST *ast) {
    trace("program");
    rec_run_ast(get_child(ast, 0)); // run var_list
    rec_run_ast(get_child(ast, 1)); // run block
}

void run_var_list(AST *ast) {
    trace("var_list");
    // Nothing to do, memory was already cleared upon initialization.
}

void run_var_decl(AST *ast) {
    trace("var_decl");
    // Nothing to do, memory was already cleared upon initialization.
}


// DONE
void run_block(AST *ast) {
    int child_count = get_child_count(ast);
    for(int i=0; i<child_count; i++){
        rec_run_ast(get_child(ast, i));
    }
}

// DONE
void run_assign(AST *ast) {
    AST* lhs = get_child(ast, 0);
    int symbtable_idx = get_data(lhs);
    int addr = symbtable_idx;

    AST* rhs = get_child(ast, 1);
    rec_run_ast(rhs);
    
    //Usar stack para 'reduzir' a expressa a direita até sobrar um unico No da AST
    
    Word word;
    word.as_int = 0;
    switch(get_node_type(rhs)){
        case INT_TYPE:
        case BOOL_TYPE:
	    word.as_int = popi();
            printf("run_assign: Storing %d at 0x%d (vartable)\n", word.as_int, addr);
            storei(addr, word.as_int);
            break;
        case REAL_TYPE:
	    word.as_float = popf();
            printf("run_assign: Storing %f at 0x%d (vartable)\n", word.as_float, addr);
            storef(addr, word.as_float);
            break;
        case STR_TYPE:
            printf("run_assign: String %s at 0x%d (strtable)\n", get_string(st, addr), addr);
        default:
        case NO_TYPE:
            break;
    }
}

// DONE
void run_bool_val(AST *ast) {
    printf("run_bool: Pushing %d (bool) to virtual stack...\n", get_data(ast));
    pushi(get_data(ast));  //if NODE_INT_VAL, then we push the actual int to the virtual stack
}

// DONE
void run_eq(AST *ast) {
    AST* left = get_child(ast, 0);
    AST* right = get_child(ast, 0);
    rec_run_ast(left);
    rec_run_ast(right);
    int res;
    switch(get_node_type(left)){
      case INT_TYPE:
      case BOOL_TYPE:
	res = get_data(left) == get_data(right);
	break;
      case REAL_TYPE:
	res = get_float_data(left) == get_float_data(right);
	break;
      case STR_TYPE:
	res = !strcmp(get_string(st, get_data(left)), get_string(st, get_data(right)));
	break;
      case NO_TYPE:
      default:
	printf("error: run_eq: NO_TYPE or undefined\n");
	exit(1);
    }
    printf("run_eq: Pushing %d to virtual stack...\n", res);
    pushi(res);
}

// DONE
void run_if(AST *ast) {
    AST* expr = get_child(ast, 0);
    AST* etrue = get_child(ast, 1);
    AST* efalse = get_child(ast, 2);
    rec_run_ast(expr);
    if(popi()){
      rec_run_ast(etrue);
    }
    else if(efalse){
      rec_run_ast(efalse);
    }
}

// DONE
void run_int_val(AST *ast) {
    printf("run_int: Pushing %d to virtual stack...\n", get_data(ast));
    pushi(get_data(ast));  //if NODE_INT_VAL, then we push the actual int to the virtual stack
}

// TODO
void run_lt(AST *ast) {
    AST* op1 = get_child(ast, 0);
    AST* op2 = get_child(ast, 1);

    rec_run_ast(op1);
    rec_run_ast(op2);

    Word i1;
    Word i2;
    switch(get_node_type(op1)){
        case INT_TYPE:
            i1.as_int = popi();
            i2.as_int = popi();
	    pushi(i1.as_int < i2.as_int);
	    printf("run_lt: pushing %d onto Virtual Stack\n", i1.as_int < i2.as_int);
            break;
        default:
            break;
    }
}

// TODO
void run_minus(AST *ast) {

}

// TODO
void run_over(AST *ast) {

}

// TODO
void run_plus(AST *ast) {
    AST* op1 = get_child(ast, 0);
    AST* op2 = get_child(ast, 1);

    rec_run_ast(op1);
    rec_run_ast(op2);

    Word i1;
    Word i2;
    switch(get_node_type(op1)){
        case BOOL_TYPE:
        case INT_TYPE:
            i1.as_int = popi();
            i2.as_int = popi();
	    printf("run_plus: pushing %d onto Virtual Stack\n", i1.as_int + i2.as_int);
	    pushi(i1.as_int + i2.as_int);
            break;
        default:
            break;
    }
}


// TODO
void run_read(AST *ast) {

}

// DONE
void run_real_val(AST *ast) {
    printf("run_real: Pushing %f to virtual stack...\n", get_float_data(ast));
    pushf(get_float_data(ast));  //if NODE_REAL_VAL, then we push the actual float to the virtual stack
}

// TODO
void run_repeat(AST *ast) {
    AST* block = get_child(ast, 0);
    AST* expr = get_child(ast, 1);

    do {
      printf("run_repeat:+1\n");
      rec_run_ast(block);
      rec_run_ast(expr);
    } while(popi() == 0);
}

// DONE
void run_str_val(AST *ast) {
    trace("str_val");
    pushi(get_data(ast)); //if NODE_STR_VAL, then we push the string idx to the virtual stack
}

// TODO
void run_times(AST *ast) {

}


// TODO
void run_var_use(AST *ast) {
    int idx = get_data(ast);
    int addr = idx;
    switch(get_node_type(ast)){
      case INT_TYPE:
      case REAL_TYPE:
      case BOOL_TYPE:
	printf("run_var_use: Pushing %d to virtual stack...\n", loadi(addr));
	pushi(loadi(addr));
	break;
      case STR_TYPE:
	printf("run_var_use: Pushing %d to virtual stack...\n", idx);
	pushi(idx);
	break;
      case NO_TYPE:
      default:
	break;
    }
}

// TODO
void run_write(AST *ast) {
    AST* toplevel = get_child(ast, 0);
    rec_run_ast(toplevel);
    
    switch(get_node_type(toplevel)){
        case INT_TYPE:
	case BOOL_TYPE:
            write_int();
            break;
	case STR_TYPE:
	    write_str();
	    break;
	case REAL_TYPE:
	    write_real();
	    break;
	case NO_TYPE:
	default:
	    break;
    }
}

// DONE
void run_b2i(AST* ast) {
    rec_run_ast(get_child(ast, 0));
}

// TODO
void run_b2r(AST* ast) {

}

void run_b2s(AST* ast) {
    rec_run_ast(get_child(ast, 0));
    clear_str_buf();
    popi() == 0 ? sprintf(str_buf, "false") : sprintf(str_buf, "true");
    pushi(add_string(st, str_buf));
}

// TODO
void run_i2r(AST* ast) {

}

void run_i2s(AST* ast) {
    rec_run_ast(get_child(ast, 0));
    clear_str_buf();
    sprintf(str_buf, "%d", popi());
    pushi(add_string(st, str_buf));
}

void run_r2s(AST* ast) {
    rec_run_ast(get_child(ast, 0));
    clear_str_buf();
    sprintf(str_buf, "%f", popf());
    pushi(add_string(st, str_buf));
}



void rec_run_ast(AST *ast) {
    switch(get_kind(ast)) {
        case ASSIGN_NODE:   run_assign(ast);    break;
        case EQ_NODE:       run_eq(ast);        break;
        case BLOCK_NODE:    run_block(ast);     break;
        case BOOL_VAL_NODE: run_bool_val(ast);  break;
        case IF_NODE:       run_if(ast);        break;
        case INT_VAL_NODE:  run_int_val(ast);   break;
        case LT_NODE:       run_lt(ast);        break;
        case MINUS_NODE:    run_minus(ast);     break;
        case OVER_NODE:     run_over(ast);      break;
        case PLUS_NODE:     run_plus(ast);      break;
        case PROGRAM_NODE:  run_program(ast);   break;
        case READ_NODE:     run_read(ast);      break;
        case REAL_VAL_NODE: run_real_val(ast);  break;
        case REPEAT_NODE:   run_repeat(ast);    break;
        case STR_VAL_NODE:  run_str_val(ast);   break;
        case TIMES_NODE:    run_times(ast);     break;
        case VAR_DECL_NODE: run_var_decl(ast);  break;
        case VAR_LIST_NODE: run_var_list(ast);  break;
        case VAR_USE_NODE:  run_var_use(ast);   break;
        case WRITE_NODE:    run_write(ast);     break;

        case B2I_NODE:      run_b2i(ast);       break;
        case B2R_NODE:      run_b2r(ast);       break;
        case B2S_NODE:      run_b2s(ast);       break;
        case I2R_NODE:      run_i2r(ast);       break;
        case I2S_NODE:      run_i2s(ast);       break;
        case R2S_NODE:      run_r2s(ast);       break;

        default:
            fprintf(stderr, "Invalid kind: %s!\n", kind2str(get_kind(ast)));
            exit(EXIT_FAILURE);
    }
}

// ----------------------------------------------------------------------------

void run_ast(AST *ast) {
    init_stack();
    init_mem();
    rec_run_ast(ast);
}
