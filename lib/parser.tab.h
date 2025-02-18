/* A Bison parser, made by GNU Bison 3.8.2.  */

/* Bison interface for Yacc-like parsers in C

   Copyright (C) 1984, 1989-1990, 2000-2015, 2018-2021 Free Software Foundation,
   Inc.

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <https://www.gnu.org/licenses/>.  */

/* As a special exception, you may create a larger work that contains
   part or all of the Bison parser skeleton and distribute that work
   under terms of your choice, so long as that work isn't itself a
   parser generator using the skeleton or a modified version thereof
   as a parser skeleton.  Alternatively, if you modify or redistribute
   the parser skeleton itself, you may (at your option) remove this
   special exception, which will cause the skeleton and the resulting
   Bison output files to be licensed under the GNU General Public
   License without this special exception.

   This special exception was added by the Free Software Foundation in
   version 2.2 of Bison.  */

/* DO NOT RELY ON FEATURES THAT ARE NOT DOCUMENTED in the manual,
   especially those whose name start with YY_ or yy_.  They are
   private implementation details that can be changed or removed.  */

#ifndef YY_YY_LIB_PARSER_TAB_H_INCLUDED
# define YY_YY_LIB_PARSER_TAB_H_INCLUDED
/* Debug traces.  */
#ifndef YYDEBUG
# define YYDEBUG 1
#endif
#if YYDEBUG
extern int yydebug;
#endif
/* "%code requires" blocks.  */

#include "Literal.h"
#include "AST.h"
union Token {
	Literal l;
	enum Type t;
	char* str;
	AST* ast;
};


/* Token kinds.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
  enum yytokentype
  {
    YYEMPTY = -2,
    YYEOF = 0,                     /* "end of file"  */
    YYerror = 256,                 /* error  */
    YYUNDEF = 257,                 /* "invalid token"  */
    ID = 258,                      /* ID  */
    ASSIGN = 259,                  /* ASSIGN  */
    INT = 260,                     /* INT  */
    FLOAT = 261,                   /* FLOAT  */
    CHAR = 262,                    /* CHAR  */
    VOID = 263,                    /* VOID  */
    SHORT = 264,                   /* SHORT  */
    SIGNED = 265,                  /* SIGNED  */
    DOUBLE = 266,                  /* DOUBLE  */
    UNSIGNED = 267,                /* UNSIGNED  */
    LONG = 268,                    /* LONG  */
    AMP = 269,                     /* AMP  */
    INLINE = 270,                  /* INLINE  */
    RESTRICT = 271,                /* RESTRICT  */
    RETURN = 272,                  /* RETURN  */
    STAR = 273,                    /* STAR  */
    ENUM = 274,                    /* ENUM  */
    STRUCT = 275,                  /* STRUCT  */
    TYPEDEF = 276,                 /* TYPEDEF  */
    UNION = 277,                   /* UNION  */
    CHR = 278,                     /* CHR  */
    FLOATVAL = 279,                /* FLOATVAL  */
    INTVAL = 280,                  /* INTVAL  */
    QUOTE = 281,                   /* QUOTE  */
    STRING = 282,                  /* STRING  */
    DIVISION = 283,                /* DIVISION  */
    MINUS = 284,                   /* MINUS  */
    MINUS1 = 285,                  /* MINUS1  */
    MINUSAUTO = 286,               /* MINUSAUTO  */
    MODULUS = 287,                 /* MODULUS  */
    PLUS = 288,                    /* PLUS  */
    PLUS1 = 289,                   /* PLUS1  */
    PLUSAUTO = 290,                /* PLUSAUTO  */
    AND = 291,                     /* AND  */
    EQ = 292,                      /* EQ  */
    GT = 293,                      /* GT  */
    GTEQ = 294,                    /* GTEQ  */
    LT = 295,                      /* LT  */
    LTEQ = 296,                    /* LTEQ  */
    NOT = 297,                     /* NOT  */
    NOTEQ = 298,                   /* NOTEQ  */
    OR = 299,                      /* OR  */
    BITANDAUTO = 300,              /* BITANDAUTO  */
    BITNOT = 301,                  /* BITNOT  */
    BITNOTAUTO = 302,              /* BITNOTAUTO  */
    BITOR = 303,                   /* BITOR  */
    BITORAUTO = 304,               /* BITORAUTO  */
    BITXOR = 305,                  /* BITXOR  */
    BITXORAUTO = 306,              /* BITXORAUTO  */
    LSHIFT = 307,                  /* LSHIFT  */
    LSHIFTAUTO = 308,              /* LSHIFTAUTO  */
    RSHIFT = 309,                  /* RSHIFT  */
    RSHIFTAUTO = 310,              /* RSHIFTAUTO  */
    AUTO = 311,                    /* AUTO  */
    CONST = 312,                   /* CONST  */
    EXTERN = 313,                  /* EXTERN  */
    REGISTER = 314,                /* REGISTER  */
    STATIC = 315,                  /* STATIC  */
    VOLATILE = 316,                /* VOLATILE  */
    IF = 317,                      /* IF  */
    ELSE = 318,                    /* ELSE  */
    CASE = 319,                    /* CASE  */
    DEFAULT = 320,                 /* DEFAULT  */
    SWITCH = 321,                  /* SWITCH  */
    BREAK = 322,                   /* BREAK  */
    CONTINUE = 323,                /* CONTINUE  */
    DO = 324,                      /* DO  */
    FOR = 325,                     /* FOR  */
    WHILE = 326,                   /* WHILE  */
    GOTO = 327,                    /* GOTO  */
    LBRACKET = 328,                /* LBRACKET  */
    LCURLY = 329,                  /* LCURLY  */
    COMMA = 330,                   /* COMMA  */
    DELI = 331,                    /* DELI  */
    RBRACKET = 332,                /* RBRACKET  */
    RCURLY = 333,                  /* RCURLY  */
    LPAR = 334,                    /* LPAR  */
    RPAR = 335,                    /* RPAR  */
    SIZEOF = 336,                  /* SIZEOF  */
    PRINTF = 337,                  /* PRINTF  */
    SCANF = 338                    /* SCANF  */
  };
  typedef enum yytokentype yytoken_kind_t;
#endif

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
typedef  union Token  YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif


extern YYSTYPE yylval;


int yyparse (void);


#endif /* !YY_YY_LIB_PARSER_TAB_H_INCLUDED  */
