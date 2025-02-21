#include "lexer.yy.h"
#include "parser.tab.h"

void parser_init();
void parser_deinit();
void parser_info();
void lexer_init();
void lexer_deinit();

int main(void) {
	lexer_init();
	parser_init();

	if(yyparse() != 0){
		return -1;
	}

	parser_info();

	parser_deinit();
	lexer_deinit();

	yylex_destroy();

	return 0;
}

void yyerror(char const *s) {
	//https://stackoverflow.com/questions/6467166/bison-flex-print-erroneous-line
	fprintf(stderr, "SYNTAX ERROR (%d): %s\n", yylineno, s);
}
