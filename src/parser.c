#include "lexer.yy.h"
#include "parser.tab.h"
#include "backend/llvm.h"

void parser_init();
void parser_deinit();
void parser_print();
void lexer_init();
void lexer_deinit();

extern AST* root;

int main(void) {
	lexer_init();
	parser_init();

	if(yyparse() != 0){
		return -1;
	}

	parser_print();

#ifdef GEN_LLVM
	FILE* fpout;
	fpout = fopen("tmp.ll", "w");
	llvm_genIR(root, fpout);
	fclose(fpout);
#endif

	parser_deinit();
	lexer_deinit();

	yylex_destroy();

	return 0;
}

void yyerror(char const *s) {
	//https://stackoverflow.com/questions/6467166/bison-flex-print-erroneous-line
	fprintf(stderr, "SYNTAX ERROR (%d): %s\n", yylineno, s);
}
