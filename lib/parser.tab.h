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

#include "type/Literal.h"
#include "type/Type.h"
#include "management/AST.h"
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
    LONG = 267,                    /* LONG  */
    UNSIGNED = 268,                /* UNSIGNED  */
    AMP = 269,                     /* AMP  */
    STAR = 270,                    /* STAR  */
    RETURN = 271,                  /* RETURN  */
    RESTRICT = 272,                /* RESTRICT  */
    INLINE = 273,                  /* INLINE  */
    ENUM = 274,                    /* ENUM  */
    UNION = 275,                   /* UNION  */
    TYPEDEF = 276,                 /* TYPEDEF  */
    STRUCT = 277,                  /* STRUCT  */
    CHR = 278,                     /* CHR  */
    STRING = 279,                  /* STRING  */
    QUOTE = 280,                   /* QUOTE  */
    INTVAL = 281,                  /* INTVAL  */
    FLOATVAL = 282,                /* FLOATVAL  */
    PLUS = 283,                    /* PLUS  */
    MINUS = 284,                   /* MINUS  */
    DIVISION = 285,                /* DIVISION  */
    MODULUS = 286,                 /* MODULUS  */
    PLUSAUTO = 287,                /* PLUSAUTO  */
    MINUSAUTO = 288,               /* MINUSAUTO  */
    PLUS1 = 289,                   /* PLUS1  */
    MINUS1 = 290,                  /* MINUS1  */
    AND = 291,                     /* AND  */
    OR = 292,                      /* OR  */
    NOTEQ = 293,                   /* NOTEQ  */
    NOT = 294,                     /* NOT  */
    LTEQ = 295,                    /* LTEQ  */
    LT = 296,                      /* LT  */
    GTEQ = 297,                    /* GTEQ  */
    GT = 298,                      /* GT  */
    EQ = 299,                      /* EQ  */
    BITANDAUTO = 300,              /* BITANDAUTO  */
    RSHIFTAUTO = 301,              /* RSHIFTAUTO  */
    RSHIFT = 302,                  /* RSHIFT  */
    LSHIFTAUTO = 303,              /* LSHIFTAUTO  */
    LSHIFT = 304,                  /* LSHIFT  */
    BITXORAUTO = 305,              /* BITXORAUTO  */
    BITXOR = 306,                  /* BITXOR  */
    BITORAUTO = 307,               /* BITORAUTO  */
    BITOR = 308,                   /* BITOR  */
    BITNOTAUTO = 309,              /* BITNOTAUTO  */
    BITNOT = 310,                  /* BITNOT  */
    AUTO = 311,                    /* AUTO  */
    VOLATILE = 312,                /* VOLATILE  */
    STATIC = 313,                  /* STATIC  */
    REGISTER = 314,                /* REGISTER  */
    EXTERN = 315,                  /* EXTERN  */
    CONST = 316,                   /* CONST  */
    IF = 317,                      /* IF  */
    ELSE = 318,                    /* ELSE  */
    CASE = 319,                    /* CASE  */
    SWITCH = 320,                  /* SWITCH  */
    DEFAULT = 321,                 /* DEFAULT  */
    BREAK = 322,                   /* BREAK  */
    WHILE = 323,                   /* WHILE  */
    FOR = 324,                     /* FOR  */
    DO = 325,                      /* DO  */
    CONTINUE = 326,                /* CONTINUE  */
    GOTO = 327,                    /* GOTO  */
    LBRACKET = 328,                /* LBRACKET  */
    SIZEOF = 329,                  /* SIZEOF  */
    RPAR = 330,                    /* RPAR  */
    LPAR = 331,                    /* LPAR  */
    RCURLY = 332,                  /* RCURLY  */
    RBRACKET = 333,                /* RBRACKET  */
    DELI = 334,                    /* DELI  */
    COMMA = 335,                   /* COMMA  */
    LCURLY = 336,                  /* LCURLY  */
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
