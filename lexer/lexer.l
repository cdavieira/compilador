%option noyywrap
%option nounput
%option noinput
%option yylineno

%{
#include <stdio.h>
#include <string.h>

#include "parser.tab.h"
#include "container/Vector.h"

#define RETURN(NAME)    return NAME
#define PRINT(NAME)     printf("%d: %s -> %s\n", yylineno, yytext, #NAME)
#define CMD(NAME)       RETURN(NAME);
#define ERRO            printf("LEXICAL ERROR (%d): Unknown symbol %s\n", yylineno, yytext);

static Vector* tokens;

static char* alloc_str(char* src);
static void save_id(void);
static void save_string(void);
static void save_int(void);
static void save_flt(void);
static void save_chr(void);

void lexer_init(void);
void lexer_deinit(void);

%}

unicomment      "//".*
multicomment    "/*"[^*]*[^/]*"*/"
comment         {unicomment}|{multicomment}
blank           [ \n\t]
ignore          {blank}|{comment}

dec             [0-9]
nonzero         [1-9]
hex             [0-9a-fA-F]
decval          ({nonzero}{dec}*)|0
hexval          0x{hex}+
intval          {decval}|{hexval}
floatval        {decval}\.[0-9]*
val             {intval}|{floatval}

str             \"([^"]|\\\")*\"
chr             \'[^'\n]{1}\'|\'\\[0ntvbr]{1}\'
identifier      ([a-zA-Z_]+{dec}*)+
macro           ^#.*

%%
{ignore}       {}
{macro}        {}
"{"            {CMD(LCURLY)}
"}"            {CMD(RCURLY)}
"["            {CMD(LBRACKET)}
"]"            {CMD(RBRACKET)}
"("            {CMD(LPAR)}
")"            {CMD(RPAR)}
";"            {CMD(DELI)}
","            {CMD(COMMA)}
"auto"         {CMD(AUTO)}
"break"        {CMD(BREAK)}
"case"         {CMD(CASE)}
"char"         {CMD(CHAR)}
"const"        {CMD(CONST)}
"continue"     {CMD(CONTINUE)}
"default"      {CMD(DEFAULT)}
"do"           {CMD(DO)}
"double"       {CMD(DOUBLE)}
"else"         {CMD(ELSE)}
"enum"         {CMD(ENUM)}
"extern"       {CMD(EXTERN)}
"float"        {CMD(FLOAT)}
"for"          {CMD(FOR)}
"goto"         {CMD(GOTO)}
"if"           {CMD(IF)}
"inline"       {CMD(INLINE)}
"int"          {CMD(INT)}
"long"         {CMD(LONG)}
"printf"       {save_id(); CMD(PRINTF)}
"register"     {CMD(REGISTER)}
"restrict"     {CMD(RESTRICT)}
"return"       {CMD(RETURN)}
"scanf"        {save_id(); CMD(SCANF)}
"short"        {CMD(SHORT)}
"signed"       {CMD(SIGNED)}
"sizeof"       {CMD(SIZEOF)}
"static"       {CMD(STATIC)}
"struct"       {CMD(STRUCT)}
"switch"       {CMD(SWITCH)}
"typedef"      {CMD(TYPEDEF)}
"union"        {CMD(UNION)}
"unsigned"     {CMD(UNSIGNED)}
"void"         {CMD(VOID)}
"volatile"     {CMD(VOLATILE)}
"while"        {CMD(WHILE)}
"="            {CMD(ASSIGN)}
"<"            {CMD(LT)}
"<="           {CMD(LTEQ)}
">"            {CMD(GT)}
">="           {CMD(GTEQ)}
"!"            {CMD(NOT)}
"!="           {CMD(NOTEQ)}
"~"            {CMD(BITNOT)}
"~="           {CMD(BITNOTAUTO)}
"^"            {CMD(BITXOR)}
"^="           {CMD(BITXORAUTO)}
"|"            {CMD(BITOR)}
"|="           {CMD(BITORAUTO)}
"&="           {CMD(BITANDAUTO)}
"&&"           {CMD(AND)}
"||"           {CMD(OR)}
"=="           {CMD(EQ)}
"<<"           {CMD(LSHIFT)}
"<<="          {CMD(LSHIFTAUTO)}
">>"           {CMD(RSHIFT)}
">>="          {CMD(RSHIFTAUTO)}
"+"            {CMD(PLUS)}
"+="           {CMD(PLUSAUTO)}
"++"           {CMD(PLUS1)}
"-"            {CMD(MINUS)}
"%"            {CMD(MODULUS)}
"-="           {CMD(MINUSAUTO)}
"--"           {CMD(MINUS1)}
"/"            {CMD(DIVISION)}
"&"            {CMD(AMP)}
"\*"           {CMD(STAR)}
{identifier}   {save_id();     CMD(ID)}
{intval}       {save_int();    CMD(INTVAL)}
{floatval}     {save_flt();    CMD(FLOATVAL)}
{str}          {save_string(); CMD(STRING)}
"'"            {CMD(QUOTE)}
{chr}          {save_chr();    CMD(CHR)}
.              { ERRO }
%%
void lexer_init(void){
	tokens = vector_new(32); //all tokens dynamically alloc'd by the lexer
}

void lexer_deinit(void){
	vector_destroy(&tokens, free);
}

static char* alloc_str(char* src){
	char* string = strdup(src);
	vector_append(tokens, string);
	return string;
}

static void save_id(void){
	char* str = alloc_str(yytext);
	yylval.str = str;
}

static void save_string(void){
	char* str = alloc_str(yytext);
	yylval.l.type = TYPE_STR;
	yylval.l.value.s = str;
}

static void save_int(void){
	yylval.l.type = TYPE_INT;
	yylval.l.value.i = atoi(yytext);
}

static void save_flt(void){
	yylval.l.type = TYPE_FLT;
	yylval.l.value.f = atof(yytext);
}

static void save_chr(void){
	yylval.l.type = TYPE_CHAR;
	yylval.l.value.c = yytext[0];
}
