#include <stdio.h>

extern int yylineno;

int yylex(void);
void yyerror(char const *s);
void yylex_destroy(void);

void lexer_init(void);
void lexer_deinit(void);

int main(void) {
	lexer_init();
	lexer_deinit();
	int ret = yylex() == -1? -1 : 0;
	yylex_destroy();
	return ret;
}

void yyerror(char const *s) {
  //https://stackoverflow.com/questions/6467166/bison-flex-print-erroneous-line
  fprintf(stderr, "LEXICAL ERROR (%d): %s\n", yylineno, s);
}
