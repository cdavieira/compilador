#include "lexer.yy.h"
#include "parser.tab.h"
#include "backend/llvm.h"
#include <string.h>

void parser_init();
void parser_deinit();
void parser_debug();
void parser_print_ast();
void lexer_init();
void lexer_deinit();

extern AST* root;

int main(int argc, char* argv[]) {
	lexer_init();
	parser_init();

	if(yyparse() != 0){
		return -1;
	}

	parser_debug();

	if(argc > 1){
	  int dot_generated = 0;
	  int ll_generated = 0;
	  for(int i=1; i<argc; i++){
	    if((dot_generated == 0) && (strcmp(argv[i], "dot") == 0)){
	      dot_generated = 1;
	      parser_print_ast("tmp.dot");
	      continue;
	    }
	    if((ll_generated == 0) && (strcmp(argv[i], "ll") == 0)){
	      ll_generated = 1;
	      FILE* fpout;
	      fpout = fopen("tmp.ll", "w");
	      llvm_genIR(root, fpout);
	      fclose(fpout);
	      continue;
	    }
	  }
	}

	parser_deinit();
	lexer_deinit();

	yylex_destroy();

	return 0;
}

void yyerror(char const *s) {
	//https://stackoverflow.com/questions/6467166/bison-flex-print-erroneous-line
	fprintf(stderr, "SYNTAX ERROR (%d): %s\n", yylineno, s);
}
