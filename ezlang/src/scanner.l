%option noyywrap
%option nounput
%option noinput
%option yylineno

%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "vector.h"
#include "parser.h"
#include "Literal.h"
#define CMD(NAME) return NAME;

static Vector* ids;

void lexer_init(void);
void lexer_deinit(void);
void lexer_save_true(void);
void lexer_save_false(void);
void lexer_save_string(void);
void lexer_save_id(void);
void lexer_save_int(void);
void lexer_save_float(void);
%}

comentario      \{[^}]*\}
digito          [0-9]
naozero         [1-9]
identificador   [a-zA-Z]+{digito}*
numero          ({naozero}{digito}*)|0
float           {numero}\.[0-9]*
branco          [ \n\t]
str             \"([^"]|\\\")*\"
ignore          {comentario}|\n

%%
{ignore}+       {}
"program"       { CMD(PROGRAM) }
";"             { CMD(SEMI)    }
"var"           { CMD(VAR)     }
"int"           { CMD(INT)     }
"bool"          { CMD(BOOL)    }
"real"          { CMD(REAL)    }
"string"        { CMD(STRING)  }
"true"          { lexer_save_true();  CMD(TRUE) }
"false"         { lexer_save_false(); CMD(FALSE) }
"begin"         { CMD(BBEGIN) }
"read"          { CMD(READ)   }
"write"         { CMD(WRITE)  }
"if"            { CMD(IF)     }
"then"          { CMD(THEN)   }
"else"          { CMD(ELSE)   }
"repeat"        { CMD(REPEAT) }
"until"         { CMD(UNTIL)  }
"end"           { CMD(END)    }
":="            { lexer_save_id(); CMD(ASSIGN) }
"="             { lexer_save_id(); CMD(EQ) }
"<"             { lexer_save_id(); CMD(LT) }
"*"             { lexer_save_id(); CMD(TIMES) }
"+"             { lexer_save_id(); CMD(PLUS) }
"/"             { lexer_save_id(); CMD(OVER) }
"-"             { lexer_save_id(); CMD(MINUS) }
"("             { CMD(LPAR)   }
")"             { CMD(RPAR)   }
{branco}        { }
{numero}        { lexer_save_int();    CMD(INT_VAL)    }
{float}         { lexer_save_float();  CMD(FLOAT_VAL)  }
{str}           { lexer_save_string(); CMD(STRING_VAL) }
{identificador} { lexer_save_id();     CMD(ID) }
.               { printf("LEXICAL ERROR: Unknown symbols %s\n", yytext); }
%%
void lexer_init(void){
	ids = vector_new(32);
	literal_init();
}

void lexer_deinit(void){
	literal_deinit();
	vector_destroy(&ids, free);
}

void lexer_save_true(void){
	literal_str_to_boo(&yylval.l, 1);
}

void lexer_save_false(void){
	literal_str_to_boo(&yylval.l, 0);
}

void lexer_save_id(void){
	char* str = strdup(yytext);
	vector_append(ids, str);
	yylval.str = str;
}

void lexer_save_string(void){
	//yytext = \"something\" (11)
	//len = 10, because: something\" (10)
	unsigned len = strlen(yytext) - 1;

	//cpy = something\"
	char cpy[len];
	for(unsigned i=1; i<len; i++){
		cpy[i-1] = yytext[i];
	}

	//cpy = something
	cpy[len-1] = '\0';
	literal_str(&yylval.l, cpy);
}

void lexer_save_int(void){
	literal_str_to_int(&yylval.l, yytext);
}

void lexer_save_float(void){
	literal_str_to_flt(&yylval.l, yytext);
}
