#include <stdio.h>

extern int yylineno;

int yyparse(void);
int yylex(void);
void yyerror(char const *s);
void yylex_destroy(void);

void lexer_init(void);
void lexer_deinit(void);
void parser_init(void);
void parser_deinit(void);
void parser_info(void);

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
