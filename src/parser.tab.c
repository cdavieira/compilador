/* A Bison parser, made by GNU Bison 3.8.2.  */

/* Bison implementation for Yacc-like parsers in C

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

/* C LALR(1) parser skeleton written by Richard Stallman, by
   simplifying the original so-called "semantic" parser.  */

/* DO NOT RELY ON FEATURES THAT ARE NOT DOCUMENTED in the manual,
   especially those whose name start with YY_ or yy_.  They are
   private implementation details that can be changed or removed.  */

/* All symbols defined below should begin with yy or YY, to avoid
   infringing on user name space.  This should be done even for local
   variables, as they might otherwise be expanded by user macros.
   There are some unavoidable exceptions within include files to
   define necessary library symbols; they are noted "INFRINGES ON
   USER NAME SPACE" below.  */

/* Identify Bison output, and Bison version.  */
#define YYBISON 30802

/* Bison version string.  */
#define YYBISON_VERSION "3.8.2"

/* Skeleton name.  */
#define YYSKELETON_NAME "yacc.c"

/* Pure parsers.  */
#define YYPURE 0

/* Push parsers.  */
#define YYPUSH 0

/* Pull parsers.  */
#define YYPULL 1




/* First part of user prologue.  */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "management/ScopeManager.h"
#include "management/VarTable.h"
#include "management/FuncTable.h"
#include "management/AST.h"
#include "type/Literal.h"
#include "container/Vector.h"

/* bison's internal variable that stores the current line number being processed. */
extern int yylineno;

/* if function_definition = 1, then we're currently reading a function definition block.
   This is useful for the parser to know if it is reading a declaration or a function definition. */
int function_definition;

/* params_count stores how mant parameters a function has. */
int params_count;

/* if fcall_active = 1, then all arguments parsed by the parser inside parenthesis get stored in 'funcargs' */
int fcall_active;

/* funcname stores the name of the current function.
   This is a trick to allow the parser to add a function declaration before start reading the function definition block (if it has one) */
char* funcname;

/* This is a trick to allow the parser to add a batch of variables from the same type.
   For example:
      int a, b, c, d; */
enum Type vartype;

/* This is also a trick which works together with 'funcname' for the same purpose, but to store the function return type. */
enum Type retvartype;

/* This is the variable which controls and stores all scopes in this program.
   It can find any variable definition. */
ScopeManager* scope_manager;

/* This is the variable which stores all functions in this program.
   Each functin has one scope associated with it, which always has access to the global scope. */
FuncTable* functable;

/* This variable stores any number of arguments that get passed to a function. */
Vector* funcargs;

AST* root;


/* bison mandatory */
int yylex(void);
void yyerror(char const *s);

/* internals */
void parser_init(void);
void parser_deinit(void);
void parser_info(void);

/* scope */
void scope_enter(void);
void scope_exit(void);

/* variables */
void var_add_declaration(char* name, enum Type type);
void var_check_declaration(char* name);
Variable* var_search(char* name);
void var_to_expr(char* name, Literal* exp);

/* functions */
int function_add_declaration(char* name, enum Type type);
void function_check_declaration(char* name);
int function_check_call(char* name);
int function_check_return(enum Type rettype);

/* misc */
void binary_operation(
  AST* op1, AST* op2, AST** res, NodeKind nodekind,
  int (*operation)(Literal*, Literal*, BinaryOperationData*));
void check_assignment(AST* op1, AST* res);
void check_if_while_condition(AST* exp);

/* ast related */
void ast_manager_add_declarator(AST** declarators, AST* declarator);
void ast_manager_new(NodeKind kind);



# ifndef YY_CAST
#  ifdef __cplusplus
#   define YY_CAST(Type, Val) static_cast<Type> (Val)
#   define YY_REINTERPRET_CAST(Type, Val) reinterpret_cast<Type> (Val)
#  else
#   define YY_CAST(Type, Val) ((Type) (Val))
#   define YY_REINTERPRET_CAST(Type, Val) ((Type) (Val))
#  endif
# endif
# ifndef YY_NULLPTR
#  if defined __cplusplus
#   if 201103L <= __cplusplus
#    define YY_NULLPTR nullptr
#   else
#    define YY_NULLPTR 0
#   endif
#  else
#   define YY_NULLPTR ((void*)0)
#  endif
# endif

#include "parser.tab.h"
/* Symbol kind.  */
enum yysymbol_kind_t
{
  YYSYMBOL_YYEMPTY = -2,
  YYSYMBOL_YYEOF = 0,                      /* "end of file"  */
  YYSYMBOL_YYerror = 1,                    /* error  */
  YYSYMBOL_YYUNDEF = 2,                    /* "invalid token"  */
  YYSYMBOL_ID = 3,                         /* ID  */
  YYSYMBOL_ASSIGN = 4,                     /* ASSIGN  */
  YYSYMBOL_INT = 5,                        /* INT  */
  YYSYMBOL_FLOAT = 6,                      /* FLOAT  */
  YYSYMBOL_CHAR = 7,                       /* CHAR  */
  YYSYMBOL_VOID = 8,                       /* VOID  */
  YYSYMBOL_SHORT = 9,                      /* SHORT  */
  YYSYMBOL_SIGNED = 10,                    /* SIGNED  */
  YYSYMBOL_DOUBLE = 11,                    /* DOUBLE  */
  YYSYMBOL_LONG = 12,                      /* LONG  */
  YYSYMBOL_UNSIGNED = 13,                  /* UNSIGNED  */
  YYSYMBOL_AMP = 14,                       /* AMP  */
  YYSYMBOL_STAR = 15,                      /* STAR  */
  YYSYMBOL_RETURN = 16,                    /* RETURN  */
  YYSYMBOL_RESTRICT = 17,                  /* RESTRICT  */
  YYSYMBOL_INLINE = 18,                    /* INLINE  */
  YYSYMBOL_ENUM = 19,                      /* ENUM  */
  YYSYMBOL_UNION = 20,                     /* UNION  */
  YYSYMBOL_TYPEDEF = 21,                   /* TYPEDEF  */
  YYSYMBOL_STRUCT = 22,                    /* STRUCT  */
  YYSYMBOL_CHR = 23,                       /* CHR  */
  YYSYMBOL_STRING = 24,                    /* STRING  */
  YYSYMBOL_QUOTE = 25,                     /* QUOTE  */
  YYSYMBOL_INTVAL = 26,                    /* INTVAL  */
  YYSYMBOL_FLOATVAL = 27,                  /* FLOATVAL  */
  YYSYMBOL_PLUS = 28,                      /* PLUS  */
  YYSYMBOL_MINUS = 29,                     /* MINUS  */
  YYSYMBOL_DIVISION = 30,                  /* DIVISION  */
  YYSYMBOL_MODULUS = 31,                   /* MODULUS  */
  YYSYMBOL_PLUSAUTO = 32,                  /* PLUSAUTO  */
  YYSYMBOL_MINUSAUTO = 33,                 /* MINUSAUTO  */
  YYSYMBOL_PLUS1 = 34,                     /* PLUS1  */
  YYSYMBOL_MINUS1 = 35,                    /* MINUS1  */
  YYSYMBOL_AND = 36,                       /* AND  */
  YYSYMBOL_OR = 37,                        /* OR  */
  YYSYMBOL_NOTEQ = 38,                     /* NOTEQ  */
  YYSYMBOL_NOT = 39,                       /* NOT  */
  YYSYMBOL_LTEQ = 40,                      /* LTEQ  */
  YYSYMBOL_LT = 41,                        /* LT  */
  YYSYMBOL_GTEQ = 42,                      /* GTEQ  */
  YYSYMBOL_GT = 43,                        /* GT  */
  YYSYMBOL_EQ = 44,                        /* EQ  */
  YYSYMBOL_BITANDAUTO = 45,                /* BITANDAUTO  */
  YYSYMBOL_RSHIFTAUTO = 46,                /* RSHIFTAUTO  */
  YYSYMBOL_RSHIFT = 47,                    /* RSHIFT  */
  YYSYMBOL_LSHIFTAUTO = 48,                /* LSHIFTAUTO  */
  YYSYMBOL_LSHIFT = 49,                    /* LSHIFT  */
  YYSYMBOL_BITXORAUTO = 50,                /* BITXORAUTO  */
  YYSYMBOL_BITXOR = 51,                    /* BITXOR  */
  YYSYMBOL_BITORAUTO = 52,                 /* BITORAUTO  */
  YYSYMBOL_BITOR = 53,                     /* BITOR  */
  YYSYMBOL_BITNOTAUTO = 54,                /* BITNOTAUTO  */
  YYSYMBOL_BITNOT = 55,                    /* BITNOT  */
  YYSYMBOL_AUTO = 56,                      /* AUTO  */
  YYSYMBOL_VOLATILE = 57,                  /* VOLATILE  */
  YYSYMBOL_STATIC = 58,                    /* STATIC  */
  YYSYMBOL_REGISTER = 59,                  /* REGISTER  */
  YYSYMBOL_EXTERN = 60,                    /* EXTERN  */
  YYSYMBOL_CONST = 61,                     /* CONST  */
  YYSYMBOL_IF = 62,                        /* IF  */
  YYSYMBOL_ELSE = 63,                      /* ELSE  */
  YYSYMBOL_CASE = 64,                      /* CASE  */
  YYSYMBOL_SWITCH = 65,                    /* SWITCH  */
  YYSYMBOL_DEFAULT = 66,                   /* DEFAULT  */
  YYSYMBOL_BREAK = 67,                     /* BREAK  */
  YYSYMBOL_WHILE = 68,                     /* WHILE  */
  YYSYMBOL_FOR = 69,                       /* FOR  */
  YYSYMBOL_DO = 70,                        /* DO  */
  YYSYMBOL_CONTINUE = 71,                  /* CONTINUE  */
  YYSYMBOL_GOTO = 72,                      /* GOTO  */
  YYSYMBOL_LBRACKET = 73,                  /* LBRACKET  */
  YYSYMBOL_SIZEOF = 74,                    /* SIZEOF  */
  YYSYMBOL_RPAR = 75,                      /* RPAR  */
  YYSYMBOL_LPAR = 76,                      /* LPAR  */
  YYSYMBOL_RCURLY = 77,                    /* RCURLY  */
  YYSYMBOL_RBRACKET = 78,                  /* RBRACKET  */
  YYSYMBOL_DELI = 79,                      /* DELI  */
  YYSYMBOL_COMMA = 80,                     /* COMMA  */
  YYSYMBOL_LCURLY = 81,                    /* LCURLY  */
  YYSYMBOL_PRINTF = 82,                    /* PRINTF  */
  YYSYMBOL_SCANF = 83,                     /* SCANF  */
  YYSYMBOL_YYACCEPT = 84,                  /* $accept  */
  YYSYMBOL_program = 85,                   /* program  */
  YYSYMBOL_declarators = 86,               /* declarators  */
  YYSYMBOL_declarator = 87,                /* declarator  */
  YYSYMBOL_declarator_var = 88,            /* declarator_var  */
  YYSYMBOL_init_declarator_list = 89,      /* init_declarator_list  */
  YYSYMBOL_init_declarator = 90,           /* init_declarator  */
  YYSYMBOL_declarator_func = 91,           /* declarator_func  */
  YYSYMBOL_92_1 = 92,                      /* $@1  */
  YYSYMBOL_93_2 = 93,                      /* $@2  */
  YYSYMBOL_declarator_func_end = 94,       /* declarator_func_end  */
  YYSYMBOL_opt_func_paramlist = 95,        /* opt_func_paramlist  */
  YYSYMBOL_func_paramlist = 96,            /* func_paramlist  */
  YYSYMBOL_97_3 = 97,                      /* $@3  */
  YYSYMBOL_func_param = 98,                /* func_param  */
  YYSYMBOL_func_block = 99,                /* func_block  */
  YYSYMBOL_100_4 = 100,                    /* $@4  */
  YYSYMBOL_block = 101,                    /* block  */
  YYSYMBOL_102_5 = 102,                    /* $@5  */
  YYSYMBOL_block_item_list = 103,          /* block_item_list  */
  YYSYMBOL_block_item = 104,               /* block_item  */
  YYSYMBOL_stmt = 105,                     /* stmt  */
  YYSYMBOL_jump_stmt = 106,                /* jump_stmt  */
  YYSYMBOL_while_stmt = 107,               /* while_stmt  */
  YYSYMBOL_if_stmt = 108,                  /* if_stmt  */
  YYSYMBOL_assign_stmt = 109,              /* assign_stmt  */
  YYSYMBOL_fcall_stmt = 110,               /* fcall_stmt  */
  YYSYMBOL_expr = 111,                     /* expr  */
  YYSYMBOL_fcall = 112,                    /* fcall  */
  YYSYMBOL_fcaller = 113,                  /* fcaller  */
  YYSYMBOL_exprs = 114,                    /* exprs  */
  YYSYMBOL_115_6 = 115,                    /* $@6  */
  YYSYMBOL_type_specifier = 116            /* type_specifier  */
};
typedef enum yysymbol_kind_t yysymbol_kind_t;




#ifdef short
# undef short
#endif

/* On compilers that do not define __PTRDIFF_MAX__ etc., make sure
   <limits.h> and (if available) <stdint.h> are included
   so that the code can choose integer types of a good width.  */

#ifndef __PTRDIFF_MAX__
# include <limits.h> /* INFRINGES ON USER NAME SPACE */
# if defined __STDC_VERSION__ && 199901 <= __STDC_VERSION__
#  include <stdint.h> /* INFRINGES ON USER NAME SPACE */
#  define YY_STDINT_H
# endif
#endif

/* Narrow types that promote to a signed type and that can represent a
   signed or unsigned integer of at least N bits.  In tables they can
   save space and decrease cache pressure.  Promoting to a signed type
   helps avoid bugs in integer arithmetic.  */

#ifdef __INT_LEAST8_MAX__
typedef __INT_LEAST8_TYPE__ yytype_int8;
#elif defined YY_STDINT_H
typedef int_least8_t yytype_int8;
#else
typedef signed char yytype_int8;
#endif

#ifdef __INT_LEAST16_MAX__
typedef __INT_LEAST16_TYPE__ yytype_int16;
#elif defined YY_STDINT_H
typedef int_least16_t yytype_int16;
#else
typedef short yytype_int16;
#endif

/* Work around bug in HP-UX 11.23, which defines these macros
   incorrectly for preprocessor constants.  This workaround can likely
   be removed in 2023, as HPE has promised support for HP-UX 11.23
   (aka HP-UX 11i v2) only through the end of 2022; see Table 2 of
   <https://h20195.www2.hpe.com/V2/getpdf.aspx/4AA4-7673ENW.pdf>.  */
#ifdef __hpux
# undef UINT_LEAST8_MAX
# undef UINT_LEAST16_MAX
# define UINT_LEAST8_MAX 255
# define UINT_LEAST16_MAX 65535
#endif

#if defined __UINT_LEAST8_MAX__ && __UINT_LEAST8_MAX__ <= __INT_MAX__
typedef __UINT_LEAST8_TYPE__ yytype_uint8;
#elif (!defined __UINT_LEAST8_MAX__ && defined YY_STDINT_H \
       && UINT_LEAST8_MAX <= INT_MAX)
typedef uint_least8_t yytype_uint8;
#elif !defined __UINT_LEAST8_MAX__ && UCHAR_MAX <= INT_MAX
typedef unsigned char yytype_uint8;
#else
typedef short yytype_uint8;
#endif

#if defined __UINT_LEAST16_MAX__ && __UINT_LEAST16_MAX__ <= __INT_MAX__
typedef __UINT_LEAST16_TYPE__ yytype_uint16;
#elif (!defined __UINT_LEAST16_MAX__ && defined YY_STDINT_H \
       && UINT_LEAST16_MAX <= INT_MAX)
typedef uint_least16_t yytype_uint16;
#elif !defined __UINT_LEAST16_MAX__ && USHRT_MAX <= INT_MAX
typedef unsigned short yytype_uint16;
#else
typedef int yytype_uint16;
#endif

#ifndef YYPTRDIFF_T
# if defined __PTRDIFF_TYPE__ && defined __PTRDIFF_MAX__
#  define YYPTRDIFF_T __PTRDIFF_TYPE__
#  define YYPTRDIFF_MAXIMUM __PTRDIFF_MAX__
# elif defined PTRDIFF_MAX
#  ifndef ptrdiff_t
#   include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  endif
#  define YYPTRDIFF_T ptrdiff_t
#  define YYPTRDIFF_MAXIMUM PTRDIFF_MAX
# else
#  define YYPTRDIFF_T long
#  define YYPTRDIFF_MAXIMUM LONG_MAX
# endif
#endif

#ifndef YYSIZE_T
# ifdef __SIZE_TYPE__
#  define YYSIZE_T __SIZE_TYPE__
# elif defined size_t
#  define YYSIZE_T size_t
# elif defined __STDC_VERSION__ && 199901 <= __STDC_VERSION__
#  include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  define YYSIZE_T size_t
# else
#  define YYSIZE_T unsigned
# endif
#endif

#define YYSIZE_MAXIMUM                                  \
  YY_CAST (YYPTRDIFF_T,                                 \
           (YYPTRDIFF_MAXIMUM < YY_CAST (YYSIZE_T, -1)  \
            ? YYPTRDIFF_MAXIMUM                         \
            : YY_CAST (YYSIZE_T, -1)))

#define YYSIZEOF(X) YY_CAST (YYPTRDIFF_T, sizeof (X))


/* Stored state numbers (used for stacks). */
typedef yytype_uint8 yy_state_t;

/* State numbers in computations.  */
typedef int yy_state_fast_t;

#ifndef YY_
# if defined YYENABLE_NLS && YYENABLE_NLS
#  if ENABLE_NLS
#   include <libintl.h> /* INFRINGES ON USER NAME SPACE */
#   define YY_(Msgid) dgettext ("bison-runtime", Msgid)
#  endif
# endif
# ifndef YY_
#  define YY_(Msgid) Msgid
# endif
#endif


#ifndef YY_ATTRIBUTE_PURE
# if defined __GNUC__ && 2 < __GNUC__ + (96 <= __GNUC_MINOR__)
#  define YY_ATTRIBUTE_PURE __attribute__ ((__pure__))
# else
#  define YY_ATTRIBUTE_PURE
# endif
#endif

#ifndef YY_ATTRIBUTE_UNUSED
# if defined __GNUC__ && 2 < __GNUC__ + (7 <= __GNUC_MINOR__)
#  define YY_ATTRIBUTE_UNUSED __attribute__ ((__unused__))
# else
#  define YY_ATTRIBUTE_UNUSED
# endif
#endif

/* Suppress unused-variable warnings by "using" E.  */
#if ! defined lint || defined __GNUC__
# define YY_USE(E) ((void) (E))
#else
# define YY_USE(E) /* empty */
#endif

/* Suppress an incorrect diagnostic about yylval being uninitialized.  */
#if defined __GNUC__ && ! defined __ICC && 406 <= __GNUC__ * 100 + __GNUC_MINOR__
# if __GNUC__ * 100 + __GNUC_MINOR__ < 407
#  define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN                           \
    _Pragma ("GCC diagnostic push")                                     \
    _Pragma ("GCC diagnostic ignored \"-Wuninitialized\"")
# else
#  define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN                           \
    _Pragma ("GCC diagnostic push")                                     \
    _Pragma ("GCC diagnostic ignored \"-Wuninitialized\"")              \
    _Pragma ("GCC diagnostic ignored \"-Wmaybe-uninitialized\"")
# endif
# define YY_IGNORE_MAYBE_UNINITIALIZED_END      \
    _Pragma ("GCC diagnostic pop")
#else
# define YY_INITIAL_VALUE(Value) Value
#endif
#ifndef YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
# define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
# define YY_IGNORE_MAYBE_UNINITIALIZED_END
#endif
#ifndef YY_INITIAL_VALUE
# define YY_INITIAL_VALUE(Value) /* Nothing. */
#endif

#if defined __cplusplus && defined __GNUC__ && ! defined __ICC && 6 <= __GNUC__
# define YY_IGNORE_USELESS_CAST_BEGIN                          \
    _Pragma ("GCC diagnostic push")                            \
    _Pragma ("GCC diagnostic ignored \"-Wuseless-cast\"")
# define YY_IGNORE_USELESS_CAST_END            \
    _Pragma ("GCC diagnostic pop")
#endif
#ifndef YY_IGNORE_USELESS_CAST_BEGIN
# define YY_IGNORE_USELESS_CAST_BEGIN
# define YY_IGNORE_USELESS_CAST_END
#endif


#define YY_ASSERT(E) ((void) (0 && (E)))

#if 1

/* The parser invokes alloca or malloc; define the necessary symbols.  */

# ifdef YYSTACK_ALLOC
   /* Pacify GCC's 'empty if-body' warning.  */
#  define YYSTACK_FREE(Ptr) do { /* empty */; } while (0)
#  ifndef YYSTACK_ALLOC_MAXIMUM
    /* The OS might guarantee only one guard page at the bottom of the stack,
       and a page size can be as small as 4096 bytes.  So we cannot safely
       invoke alloca (N) if N exceeds 4096.  Use a slightly smaller number
       to allow for a few compiler-allocated temporary stack slots.  */
#   define YYSTACK_ALLOC_MAXIMUM 4032 /* reasonable circa 2006 */
#  endif
# else
#  define YYSTACK_ALLOC YYMALLOC
#  define YYSTACK_FREE YYFREE
#  ifndef YYSTACK_ALLOC_MAXIMUM
#   define YYSTACK_ALLOC_MAXIMUM YYSIZE_MAXIMUM
#  endif
#  if (defined __cplusplus && ! defined EXIT_SUCCESS \
       && ! ((defined YYMALLOC || defined malloc) \
             && (defined YYFREE || defined free)))
#   include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#   ifndef EXIT_SUCCESS
#    define EXIT_SUCCESS 0
#   endif
#  endif
#  ifndef YYMALLOC
#   define YYMALLOC malloc
#   if ! defined malloc && ! defined EXIT_SUCCESS
void *malloc (YYSIZE_T); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
#  ifndef YYFREE
#   define YYFREE free
#   if ! defined free && ! defined EXIT_SUCCESS
void free (void *); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
# endif
# define YYCOPY_NEEDED 1
#endif /* 1 */

#if (! defined yyoverflow \
     && (! defined __cplusplus \
         || (defined YYSTYPE_IS_TRIVIAL && YYSTYPE_IS_TRIVIAL)))

/* A type that is properly aligned for any stack member.  */
union yyalloc
{
  yy_state_t yyss_alloc;
  YYSTYPE yyvs_alloc;
};

/* The size of the maximum gap between one aligned stack and the next.  */
# define YYSTACK_GAP_MAXIMUM (YYSIZEOF (union yyalloc) - 1)

/* The size of an array large to enough to hold all stacks, each with
   N elements.  */
# define YYSTACK_BYTES(N) \
     ((N) * (YYSIZEOF (yy_state_t) + YYSIZEOF (YYSTYPE)) \
      + YYSTACK_GAP_MAXIMUM)

# define YYCOPY_NEEDED 1

/* Relocate STACK from its old location to the new one.  The
   local variables YYSIZE and YYSTACKSIZE give the old and new number of
   elements in the stack, and YYPTR gives the new location of the
   stack.  Advance YYPTR to a properly aligned location for the next
   stack.  */
# define YYSTACK_RELOCATE(Stack_alloc, Stack)                           \
    do                                                                  \
      {                                                                 \
        YYPTRDIFF_T yynewbytes;                                         \
        YYCOPY (&yyptr->Stack_alloc, Stack, yysize);                    \
        Stack = &yyptr->Stack_alloc;                                    \
        yynewbytes = yystacksize * YYSIZEOF (*Stack) + YYSTACK_GAP_MAXIMUM; \
        yyptr += yynewbytes / YYSIZEOF (*yyptr);                        \
      }                                                                 \
    while (0)

#endif

#if defined YYCOPY_NEEDED && YYCOPY_NEEDED
/* Copy COUNT objects from SRC to DST.  The source and destination do
   not overlap.  */
# ifndef YYCOPY
#  if defined __GNUC__ && 1 < __GNUC__
#   define YYCOPY(Dst, Src, Count) \
      __builtin_memcpy (Dst, Src, YY_CAST (YYSIZE_T, (Count)) * sizeof (*(Src)))
#  else
#   define YYCOPY(Dst, Src, Count)              \
      do                                        \
        {                                       \
          YYPTRDIFF_T yyi;                      \
          for (yyi = 0; yyi < (Count); yyi++)   \
            (Dst)[yyi] = (Src)[yyi];            \
        }                                       \
      while (0)
#  endif
# endif
#endif /* !YYCOPY_NEEDED */

/* YYFINAL -- State number of the termination state.  */
#define YYFINAL  11
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   451

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  84
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  33
/* YYNRULES -- Number of rules.  */
#define YYNRULES  94
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  168

/* YYMAXUTOK -- Last valid token kind.  */
#define YYMAXUTOK   338


/* YYTRANSLATE(TOKEN-NUM) -- Symbol number corresponding to TOKEN-NUM
   as returned by yylex, with out-of-bounds checking.  */
#define YYTRANSLATE(YYX)                                \
  (0 <= (YYX) && (YYX) <= YYMAXUTOK                     \
   ? YY_CAST (yysymbol_kind_t, yytranslate[YYX])        \
   : YYSYMBOL_YYUNDEF)

/* YYTRANSLATE[TOKEN-NUM] -- Symbol number corresponding to TOKEN-NUM
   as returned by yylex.  */
static const yytype_int8 yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     1,     2,     3,     4,
       5,     6,     7,     8,     9,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    21,    22,    23,    24,
      25,    26,    27,    28,    29,    30,    31,    32,    33,    34,
      35,    36,    37,    38,    39,    40,    41,    42,    43,    44,
      45,    46,    47,    48,    49,    50,    51,    52,    53,    54,
      55,    56,    57,    58,    59,    60,    61,    62,    63,    64,
      65,    66,    67,    68,    69,    70,    71,    72,    73,    74,
      75,    76,    77,    78,    79,    80,    81,    82,    83
};

#if YYDEBUG
/* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
static const yytype_int16 yyrline[] =
{
       0,   251,   251,   255,   256,   260,   261,   265,   269,   270,
     274,   275,   276,   277,   278,   279,   283,   283,   283,   297,
     298,   302,   303,   307,   308,   308,   312,   313,   314,   315,
     316,   317,   324,   325,   325,   333,   334,   334,   338,   339,
     343,   344,   345,   349,   350,   351,   352,   353,   357,   358,
     359,   360,   364,   368,   369,   370,   374,   375,   379,   383,
     384,   385,   386,   387,   388,   389,   390,   391,   392,   393,
     394,   395,   396,   397,   398,   399,   400,   401,   402,   403,
     404,   405,   406,   410,   411,   415,   416,   417,   421,   422,
     422,   427,   428,   429,   430
};
#endif

/** Accessing symbol of state STATE.  */
#define YY_ACCESSING_SYMBOL(State) YY_CAST (yysymbol_kind_t, yystos[State])

#if 1
/* The user-facing name of the symbol whose (internal) number is
   YYSYMBOL.  No bounds checking.  */
static const char *yysymbol_name (yysymbol_kind_t yysymbol) YY_ATTRIBUTE_UNUSED;

/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const yytname[] =
{
  "\"end of file\"", "error", "\"invalid token\"", "ID", "ASSIGN", "INT",
  "FLOAT", "CHAR", "VOID", "SHORT", "SIGNED", "DOUBLE", "LONG", "UNSIGNED",
  "AMP", "STAR", "RETURN", "RESTRICT", "INLINE", "ENUM", "UNION",
  "TYPEDEF", "STRUCT", "CHR", "STRING", "QUOTE", "INTVAL", "FLOATVAL",
  "PLUS", "MINUS", "DIVISION", "MODULUS", "PLUSAUTO", "MINUSAUTO", "PLUS1",
  "MINUS1", "AND", "OR", "NOTEQ", "NOT", "LTEQ", "LT", "GTEQ", "GT", "EQ",
  "BITANDAUTO", "RSHIFTAUTO", "RSHIFT", "LSHIFTAUTO", "LSHIFT",
  "BITXORAUTO", "BITXOR", "BITORAUTO", "BITOR", "BITNOTAUTO", "BITNOT",
  "AUTO", "VOLATILE", "STATIC", "REGISTER", "EXTERN", "CONST", "IF",
  "ELSE", "CASE", "SWITCH", "DEFAULT", "BREAK", "WHILE", "FOR", "DO",
  "CONTINUE", "GOTO", "LBRACKET", "SIZEOF", "RPAR", "LPAR", "RCURLY",
  "RBRACKET", "DELI", "COMMA", "LCURLY", "PRINTF", "SCANF", "$accept",
  "program", "declarators", "declarator", "declarator_var",
  "init_declarator_list", "init_declarator", "declarator_func", "$@1",
  "$@2", "declarator_func_end", "opt_func_paramlist", "func_paramlist",
  "$@3", "func_param", "func_block", "$@4", "block", "$@5",
  "block_item_list", "block_item", "stmt", "jump_stmt", "while_stmt",
  "if_stmt", "assign_stmt", "fcall_stmt", "expr", "fcall", "fcaller",
  "exprs", "$@6", "type_specifier", YY_NULLPTR
};

static const char *
yysymbol_name (yysymbol_kind_t yysymbol)
{
  return yytname[yysymbol];
}
#endif

#define YYPACT_NINF (-104)

#define yypact_value_is_default(Yyn) \
  ((Yyn) == YYPACT_NINF)

#define YYTABLE_NINF (-90)

#define yytable_value_is_error(Yyn) \
  0

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
static const yytype_int16 yypact[] =
{
     131,  -104,  -104,  -104,  -104,    29,  -104,   131,  -104,  -104,
      27,  -104,  -104,    21,   -37,    -7,   176,    53,     6,  -104,
      93,   -14,    98,  -104,  -104,  -104,  -104,    37,    41,   176,
    -104,  -104,   394,  -104,    26,    28,  -104,    16,  -104,   176,
    -104,  -104,  -104,  -104,  -104,   340,   176,   176,   176,   176,
     176,   176,   176,   176,   176,   176,   176,   105,   101,   131,
     287,  -104,  -104,    13,    13,  -104,  -104,   411,   411,   147,
     147,   147,   147,  -104,   196,    32,   -21,    36,  -104,    34,
      -1,  -104,    35,  -104,  -104,   132,    19,    46,    39,   106,
     176,  -104,    50,  -104,    64,  -104,  -104,   131,   140,     1,
    -104,  -104,  -104,    42,  -104,  -104,   304,    55,    66,    17,
     168,    68,    69,    71,    70,    73,  -104,  -104,     3,  -104,
    -104,  -104,  -104,  -104,  -104,  -104,    74,    93,  -104,    76,
     131,   176,   176,  -104,   226,   176,  -104,   176,  -104,  -104,
      42,  -104,  -104,  -104,   131,    82,   250,   323,  -104,   359,
     376,    10,    90,  -104,  -104,   166,    91,    91,  -104,  -104,
     176,   110,  -104,   270,   -35,  -104,  -104,  -104
};

/* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
   Performed when YYTABLE does not specify something else to do.  Zero
   means the default is an error.  */
static const yytype_int8 yydefact[] =
{
       0,    91,    93,    92,    94,     0,     2,     3,     5,     6,
       0,     1,     4,    10,     0,     8,     0,     0,     0,     7,
       0,    60,     0,    64,    63,    61,    62,     0,     0,     0,
      86,    87,    11,    80,     0,     0,    17,    10,     9,     0,
      81,    77,    79,    76,    78,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    12,    21,
       0,    59,    67,    65,    66,    68,    69,    71,    70,    73,
      74,    75,    72,    83,    88,     0,     0,     0,    22,    23,
      26,    82,     0,    84,    15,     0,     0,     0,    27,     0,
       0,    13,     0,    19,    33,    18,    20,     0,     0,     0,
      90,    14,    32,     0,    25,    28,     0,     0,     0,    85,
       0,     0,     0,     0,     0,    36,    40,    42,     0,    38,
      41,    46,    45,    44,    43,    47,     0,     0,    29,     0,
       0,     0,     0,    50,     0,     0,    49,     0,    48,    35,
       0,    34,    39,    58,     0,     0,     0,     0,    51,     0,
       0,     0,     0,    30,    56,     0,     0,     0,    37,    31,
       0,    53,    52,     0,     0,    57,    54,    55
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -104,  -104,   167,  -104,     5,   165,  -104,  -104,  -104,  -104,
    -104,  -104,   -75,  -104,  -104,  -104,  -104,  -103,  -104,    49,
     -95,  -104,  -104,  -104,    22,  -104,  -104,   -15,   -52,  -104,
     -33,  -104,     0
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_uint8 yydefgoto[] =
{
       0,     5,     6,     7,   116,    14,    15,     9,    18,    59,
      95,    77,    78,    87,    79,    96,   103,   117,   140,   118,
     119,   120,   121,   122,   123,   124,   125,    74,    33,    34,
      75,    82,    80
};

/* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule whose
   number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int16 yytable[] =
{
      10,    32,    88,    84,   107,     8,   109,    10,     1,     2,
       3,     4,     8,   109,    45,     1,     2,     3,     4,   110,
      16,   131,   104,   142,    60,    16,   110,   111,    46,    11,
      13,    62,    63,    64,    65,    66,    67,    68,    69,    70,
      71,    72,    19,    49,    50,   109,   115,     1,     2,     3,
       4,   126,    92,   161,   162,   145,   142,   100,   110,    39,
      85,   166,   -85,    41,    42,   111,   126,    43,    44,   152,
     112,   113,   111,    20,   114,    89,   108,   112,   113,    35,
     141,   114,    36,   106,   115,    30,    31,   158,   126,    17,
     132,   115,    30,    31,    17,   134,    37,   -16,    93,   126,
      94,    40,    57,   127,   111,    76,    58,    83,    21,   112,
     113,    86,    98,   114,   -24,    90,   146,   147,   127,    22,
     149,    99,   150,   115,    30,    31,    97,   101,    23,    24,
     129,    25,    26,    27,    28,    21,     1,     2,     3,     4,
     127,   102,   130,    21,   135,   163,    22,   137,   136,   138,
     139,   127,   144,   143,    22,    23,    24,   153,    25,    26,
      27,    28,    46,    23,    24,   159,    25,    26,    27,    28,
     160,    21,   115,   164,    12,    47,    48,    49,    50,    21,
      73,    29,    22,    51,    52,    38,   167,    30,    31,   151,
      22,    23,    24,     0,    25,    26,    27,    28,     0,    23,
      24,     0,    25,    26,    27,    28,     0,     0,    29,    91,
       0,    46,     0,     0,    30,    31,    29,     0,   105,     0,
       0,     0,    30,    31,    47,    48,    49,    50,     0,     0,
       0,     0,    51,    52,    53,     0,     0,    54,     0,    55,
      56,    46,     0,     0,    29,     0,     0,   133,     0,     0,
      30,    31,    29,     0,    47,    48,    49,    50,    30,    31,
       0,     0,    51,    52,    53,    46,     0,    54,     0,    55,
      56,     0,     0,     0,     0,     0,   -89,     0,    47,    48,
      49,    50,     0,     0,     0,    46,    51,    52,    53,     0,
       0,    54,     0,    55,    56,     0,     0,     0,    47,    48,
      49,    50,    46,     0,     0,   148,    51,    52,    53,     0,
       0,    54,     0,    55,    56,    47,    48,    49,    50,    46,
       0,     0,     0,    51,    52,    53,     0,     0,    54,   154,
      55,    56,    47,    48,    49,    50,     0,     0,    46,     0,
      51,    52,    53,     0,     0,    54,     0,    55,    56,   165,
       0,    47,    48,    49,    50,    46,     0,     0,     0,    51,
      52,    53,     0,     0,    54,    81,    55,    56,    47,    48,
      49,    50,     0,     0,    46,     0,    51,    52,    53,     0,
       0,    54,   128,    55,    56,     0,     0,    47,    48,    49,
      50,    46,     0,     0,     0,    51,    52,    53,     0,     0,
      54,   155,    55,    56,    47,    48,    49,    50,     0,    46,
       0,     0,    51,    52,    53,    61,     0,    54,     0,    55,
      56,     0,    47,    48,    49,    50,    46,     0,     0,     0,
      51,    52,    53,     0,   156,    54,     0,    55,    56,    47,
      48,    49,    50,     0,     0,     0,     0,     0,     0,     0,
       0,   157
};

static const yytype_int16 yycheck[] =
{
       0,    16,     3,    24,     3,     0,     3,     7,     5,     6,
       7,     8,     7,     3,    29,     5,     6,     7,     8,    16,
       4,     4,    97,   118,    39,     4,    16,    62,    15,     0,
       3,    46,    47,    48,    49,    50,    51,    52,    53,    54,
      55,    56,    79,    30,    31,     3,    81,     5,     6,     7,
       8,   103,    85,   156,   157,   130,   151,    90,    16,    73,
      81,   164,    76,    26,    27,    62,   118,    26,    27,   144,
      67,    68,    62,    80,    71,    76,    75,    67,    68,    26,
      77,    71,    76,    98,    81,    82,    83,    77,   140,    73,
      73,    81,    82,    83,    73,   110,     3,    76,    79,   151,
      81,     3,    76,   103,    62,     4,    78,    75,     3,    67,
      68,    75,    73,    71,    80,    80,   131,   132,   118,    14,
     135,    15,   137,    81,    82,    83,    80,    77,    23,    24,
      75,    26,    27,    28,    29,     3,     5,     6,     7,     8,
     140,    77,    76,     3,    76,   160,    14,    76,    79,    79,
      77,   151,    76,    79,    14,    23,    24,    75,    26,    27,
      28,    29,    15,    23,    24,    75,    26,    27,    28,    29,
       4,     3,    81,    63,     7,    28,    29,    30,    31,     3,
      75,    76,    14,    36,    37,    20,   164,    82,    83,   140,
      14,    23,    24,    -1,    26,    27,    28,    29,    -1,    23,
      24,    -1,    26,    27,    28,    29,    -1,    -1,    76,    77,
      -1,    15,    -1,    -1,    82,    83,    76,    -1,    78,    -1,
      -1,    -1,    82,    83,    28,    29,    30,    31,    -1,    -1,
      -1,    -1,    36,    37,    38,    -1,    -1,    41,    -1,    43,
      44,    15,    -1,    -1,    76,    -1,    -1,    79,    -1,    -1,
      82,    83,    76,    -1,    28,    29,    30,    31,    82,    83,
      -1,    -1,    36,    37,    38,    15,    -1,    41,    -1,    43,
      44,    -1,    -1,    -1,    -1,    -1,    80,    -1,    28,    29,
      30,    31,    -1,    -1,    -1,    15,    36,    37,    38,    -1,
      -1,    41,    -1,    43,    44,    -1,    -1,    -1,    28,    29,
      30,    31,    15,    -1,    -1,    79,    36,    37,    38,    -1,
      -1,    41,    -1,    43,    44,    28,    29,    30,    31,    15,
      -1,    -1,    -1,    36,    37,    38,    -1,    -1,    41,    79,
      43,    44,    28,    29,    30,    31,    -1,    -1,    15,    -1,
      36,    37,    38,    -1,    -1,    41,    -1,    43,    44,    79,
      -1,    28,    29,    30,    31,    15,    -1,    -1,    -1,    36,
      37,    38,    -1,    -1,    41,    78,    43,    44,    28,    29,
      30,    31,    -1,    -1,    15,    -1,    36,    37,    38,    -1,
      -1,    41,    78,    43,    44,    -1,    -1,    28,    29,    30,
      31,    15,    -1,    -1,    -1,    36,    37,    38,    -1,    -1,
      41,    78,    43,    44,    28,    29,    30,    31,    -1,    15,
      -1,    -1,    36,    37,    38,    75,    -1,    41,    -1,    43,
      44,    -1,    28,    29,    30,    31,    15,    -1,    -1,    -1,
      36,    37,    38,    -1,    75,    41,    -1,    43,    44,    28,
      29,    30,    31,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    75
};

/* YYSTOS[STATE-NUM] -- The symbol kind of the accessing symbol of
   state STATE-NUM.  */
static const yytype_int8 yystos[] =
{
       0,     5,     6,     7,     8,    85,    86,    87,    88,    91,
     116,     0,    86,     3,    89,    90,     4,    73,    92,    79,
      80,     3,    14,    23,    24,    26,    27,    28,    29,    76,
      82,    83,   111,   112,   113,    26,    76,     3,    89,    73,
       3,    26,    27,    26,    27,   111,    15,    28,    29,    30,
      31,    36,    37,    38,    41,    43,    44,    76,    78,    93,
     111,    75,   111,   111,   111,   111,   111,   111,   111,   111,
     111,   111,   111,    75,   111,   114,     4,    95,    96,    98,
     116,    78,   115,    75,    24,    81,    75,    97,     3,    76,
      80,    77,   114,    79,    81,    94,    99,    80,    73,    15,
     114,    77,    77,   100,    96,    78,   111,     3,    75,     3,
      16,    62,    67,    68,    71,    81,    88,   101,   103,   104,
     105,   106,   107,   108,   109,   110,   112,   116,    78,    75,
      76,     4,    73,    79,   111,    76,    79,    76,    79,    77,
     102,    77,   104,    79,    76,    96,   111,   111,    79,   111,
     111,   103,    96,    75,    79,    78,    75,    75,    77,    75,
       4,   101,   101,   111,    63,    79,   101,   108
};

/* YYR1[RULE-NUM] -- Symbol kind of the left-hand side of rule RULE-NUM.  */
static const yytype_int8 yyr1[] =
{
       0,    84,    85,    86,    86,    87,    87,    88,    89,    89,
      90,    90,    90,    90,    90,    90,    92,    93,    91,    94,
      94,    95,    95,    96,    97,    96,    98,    98,    98,    98,
      98,    98,    99,   100,    99,   101,   102,   101,   103,   103,
     104,   104,   104,   105,   105,   105,   105,   105,   106,   106,
     106,   106,   107,   108,   108,   108,   109,   109,   110,   111,
     111,   111,   111,   111,   111,   111,   111,   111,   111,   111,
     111,   111,   111,   111,   111,   111,   111,   111,   111,   111,
     111,   111,   111,   112,   112,   113,   113,   113,   114,   115,
     114,   116,   116,   116,   116
};

/* YYR2[RULE-NUM] -- Number of symbols on the right-hand side of rule RULE-NUM.  */
static const yytype_int8 yyr2[] =
{
       0,     2,     1,     1,     2,     1,     1,     3,     1,     3,
       1,     3,     4,     7,     8,     6,     0,     0,     8,     1,
       1,     0,     1,     1,     0,     4,     1,     2,     4,     5,
       7,     8,     2,     0,     4,     2,     0,     4,     1,     2,
       1,     1,     1,     1,     1,     1,     1,     1,     2,     2,
       2,     3,     5,     5,     7,     7,     4,     7,     2,     3,
       1,     1,     1,     1,     1,     3,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     3,     2,     2,     2,     2,
       1,     2,     4,     3,     4,     1,     1,     1,     1,     0,
       4,     1,     1,     1,     1
};


enum { YYENOMEM = -2 };

#define yyerrok         (yyerrstatus = 0)
#define yyclearin       (yychar = YYEMPTY)

#define YYACCEPT        goto yyacceptlab
#define YYABORT         goto yyabortlab
#define YYERROR         goto yyerrorlab
#define YYNOMEM         goto yyexhaustedlab


#define YYRECOVERING()  (!!yyerrstatus)

#define YYBACKUP(Token, Value)                                    \
  do                                                              \
    if (yychar == YYEMPTY)                                        \
      {                                                           \
        yychar = (Token);                                         \
        yylval = (Value);                                         \
        YYPOPSTACK (yylen);                                       \
        yystate = *yyssp;                                         \
        YY_LAC_DISCARD ("YYBACKUP");                              \
        goto yybackup;                                            \
      }                                                           \
    else                                                          \
      {                                                           \
        yyerror (YY_("syntax error: cannot back up")); \
        YYERROR;                                                  \
      }                                                           \
  while (0)

/* Backward compatibility with an undocumented macro.
   Use YYerror or YYUNDEF. */
#define YYERRCODE YYUNDEF


/* Enable debugging if requested.  */
#if YYDEBUG

# ifndef YYFPRINTF
#  include <stdio.h> /* INFRINGES ON USER NAME SPACE */
#  define YYFPRINTF fprintf
# endif

# define YYDPRINTF(Args)                        \
do {                                            \
  if (yydebug)                                  \
    YYFPRINTF Args;                             \
} while (0)




# define YY_SYMBOL_PRINT(Title, Kind, Value, Location)                    \
do {                                                                      \
  if (yydebug)                                                            \
    {                                                                     \
      YYFPRINTF (stderr, "%s ", Title);                                   \
      yy_symbol_print (stderr,                                            \
                  Kind, Value); \
      YYFPRINTF (stderr, "\n");                                           \
    }                                                                     \
} while (0)


/*-----------------------------------.
| Print this symbol's value on YYO.  |
`-----------------------------------*/

static void
yy_symbol_value_print (FILE *yyo,
                       yysymbol_kind_t yykind, YYSTYPE const * const yyvaluep)
{
  FILE *yyoutput = yyo;
  YY_USE (yyoutput);
  if (!yyvaluep)
    return;
  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  YY_USE (yykind);
  YY_IGNORE_MAYBE_UNINITIALIZED_END
}


/*---------------------------.
| Print this symbol on YYO.  |
`---------------------------*/

static void
yy_symbol_print (FILE *yyo,
                 yysymbol_kind_t yykind, YYSTYPE const * const yyvaluep)
{
  YYFPRINTF (yyo, "%s %s (",
             yykind < YYNTOKENS ? "token" : "nterm", yysymbol_name (yykind));

  yy_symbol_value_print (yyo, yykind, yyvaluep);
  YYFPRINTF (yyo, ")");
}

/*------------------------------------------------------------------.
| yy_stack_print -- Print the state stack from its BOTTOM up to its |
| TOP (included).                                                   |
`------------------------------------------------------------------*/

static void
yy_stack_print (yy_state_t *yybottom, yy_state_t *yytop)
{
  YYFPRINTF (stderr, "Stack now");
  for (; yybottom <= yytop; yybottom++)
    {
      int yybot = *yybottom;
      YYFPRINTF (stderr, " %d", yybot);
    }
  YYFPRINTF (stderr, "\n");
}

# define YY_STACK_PRINT(Bottom, Top)                            \
do {                                                            \
  if (yydebug)                                                  \
    yy_stack_print ((Bottom), (Top));                           \
} while (0)


/*------------------------------------------------.
| Report that the YYRULE is going to be reduced.  |
`------------------------------------------------*/

static void
yy_reduce_print (yy_state_t *yyssp, YYSTYPE *yyvsp,
                 int yyrule)
{
  int yylno = yyrline[yyrule];
  int yynrhs = yyr2[yyrule];
  int yyi;
  YYFPRINTF (stderr, "Reducing stack by rule %d (line %d):\n",
             yyrule - 1, yylno);
  /* The symbols being reduced.  */
  for (yyi = 0; yyi < yynrhs; yyi++)
    {
      YYFPRINTF (stderr, "   $%d = ", yyi + 1);
      yy_symbol_print (stderr,
                       YY_ACCESSING_SYMBOL (+yyssp[yyi + 1 - yynrhs]),
                       &yyvsp[(yyi + 1) - (yynrhs)]);
      YYFPRINTF (stderr, "\n");
    }
}

# define YY_REDUCE_PRINT(Rule)          \
do {                                    \
  if (yydebug)                          \
    yy_reduce_print (yyssp, yyvsp, Rule); \
} while (0)

/* Nonzero means print parse trace.  It is left uninitialized so that
   multiple parsers can coexist.  */
int yydebug;
#else /* !YYDEBUG */
# define YYDPRINTF(Args) ((void) 0)
# define YY_SYMBOL_PRINT(Title, Kind, Value, Location)
# define YY_STACK_PRINT(Bottom, Top)
# define YY_REDUCE_PRINT(Rule)
#endif /* !YYDEBUG */


/* YYINITDEPTH -- initial size of the parser's stacks.  */
#ifndef YYINITDEPTH
# define YYINITDEPTH 200
#endif

/* YYMAXDEPTH -- maximum size the stacks can grow to (effective only
   if the built-in stack extension method is used).

   Do not make this value too large; the results are undefined if
   YYSTACK_ALLOC_MAXIMUM < YYSTACK_BYTES (YYMAXDEPTH)
   evaluated with infinite-precision integer arithmetic.  */

#ifndef YYMAXDEPTH
# define YYMAXDEPTH 10000
#endif


/* Given a state stack such that *YYBOTTOM is its bottom, such that
   *YYTOP is either its top or is YYTOP_EMPTY to indicate an empty
   stack, and such that *YYCAPACITY is the maximum number of elements it
   can hold without a reallocation, make sure there is enough room to
   store YYADD more elements.  If not, allocate a new stack using
   YYSTACK_ALLOC, copy the existing elements, and adjust *YYBOTTOM,
   *YYTOP, and *YYCAPACITY to reflect the new capacity and memory
   location.  If *YYBOTTOM != YYBOTTOM_NO_FREE, then free the old stack
   using YYSTACK_FREE.  Return 0 if successful or if no reallocation is
   required.  Return YYENOMEM if memory is exhausted.  */
static int
yy_lac_stack_realloc (YYPTRDIFF_T *yycapacity, YYPTRDIFF_T yyadd,
#if YYDEBUG
                      char const *yydebug_prefix,
                      char const *yydebug_suffix,
#endif
                      yy_state_t **yybottom,
                      yy_state_t *yybottom_no_free,
                      yy_state_t **yytop, yy_state_t *yytop_empty)
{
  YYPTRDIFF_T yysize_old =
    *yytop == yytop_empty ? 0 : *yytop - *yybottom + 1;
  YYPTRDIFF_T yysize_new = yysize_old + yyadd;
  if (*yycapacity < yysize_new)
    {
      YYPTRDIFF_T yyalloc = 2 * yysize_new;
      yy_state_t *yybottom_new;
      /* Use YYMAXDEPTH for maximum stack size given that the stack
         should never need to grow larger than the main state stack
         needs to grow without LAC.  */
      if (YYMAXDEPTH < yysize_new)
        {
          YYDPRINTF ((stderr, "%smax size exceeded%s", yydebug_prefix,
                      yydebug_suffix));
          return YYENOMEM;
        }
      if (YYMAXDEPTH < yyalloc)
        yyalloc = YYMAXDEPTH;
      yybottom_new =
        YY_CAST (yy_state_t *,
                 YYSTACK_ALLOC (YY_CAST (YYSIZE_T,
                                         yyalloc * YYSIZEOF (*yybottom_new))));
      if (!yybottom_new)
        {
          YYDPRINTF ((stderr, "%srealloc failed%s", yydebug_prefix,
                      yydebug_suffix));
          return YYENOMEM;
        }
      if (*yytop != yytop_empty)
        {
          YYCOPY (yybottom_new, *yybottom, yysize_old);
          *yytop = yybottom_new + (yysize_old - 1);
        }
      if (*yybottom != yybottom_no_free)
        YYSTACK_FREE (*yybottom);
      *yybottom = yybottom_new;
      *yycapacity = yyalloc;
    }
  return 0;
}

/* Establish the initial context for the current lookahead if no initial
   context is currently established.

   We define a context as a snapshot of the parser stacks.  We define
   the initial context for a lookahead as the context in which the
   parser initially examines that lookahead in order to select a
   syntactic action.  Thus, if the lookahead eventually proves
   syntactically unacceptable (possibly in a later context reached via a
   series of reductions), the initial context can be used to determine
   the exact set of tokens that would be syntactically acceptable in the
   lookahead's place.  Moreover, it is the context after which any
   further semantic actions would be erroneous because they would be
   determined by a syntactically unacceptable token.

   YY_LAC_ESTABLISH should be invoked when a reduction is about to be
   performed in an inconsistent state (which, for the purposes of LAC,
   includes consistent states that don't know they're consistent because
   their default reductions have been disabled).  Iff there is a
   lookahead token, it should also be invoked before reporting a syntax
   error.  This latter case is for the sake of the debugging output.

   For parse.lac=full, the implementation of YY_LAC_ESTABLISH is as
   follows.  If no initial context is currently established for the
   current lookahead, then check if that lookahead can eventually be
   shifted if syntactic actions continue from the current context.
   Report a syntax error if it cannot.  */
#define YY_LAC_ESTABLISH                                                \
do {                                                                    \
  if (!yy_lac_established)                                              \
    {                                                                   \
      YYDPRINTF ((stderr,                                               \
                  "LAC: initial context established for %s\n",          \
                  yysymbol_name (yytoken)));                            \
      yy_lac_established = 1;                                           \
      switch (yy_lac (yyesa, &yyes, &yyes_capacity, yyssp, yytoken))    \
        {                                                               \
        case YYENOMEM:                                                  \
          YYNOMEM;                                                      \
        case 1:                                                         \
          goto yyerrlab;                                                \
        }                                                               \
    }                                                                   \
} while (0)

/* Discard any previous initial lookahead context because of Event,
   which may be a lookahead change or an invalidation of the currently
   established initial context for the current lookahead.

   The most common example of a lookahead change is a shift.  An example
   of both cases is syntax error recovery.  That is, a syntax error
   occurs when the lookahead is syntactically erroneous for the
   currently established initial context, so error recovery manipulates
   the parser stacks to try to find a new initial context in which the
   current lookahead is syntactically acceptable.  If it fails to find
   such a context, it discards the lookahead.  */
#if YYDEBUG
# define YY_LAC_DISCARD(Event)                                           \
do {                                                                     \
  if (yy_lac_established)                                                \
    {                                                                    \
      YYDPRINTF ((stderr, "LAC: initial context discarded due to "       \
                  Event "\n"));                                          \
      yy_lac_established = 0;                                            \
    }                                                                    \
} while (0)
#else
# define YY_LAC_DISCARD(Event) yy_lac_established = 0
#endif

/* Given the stack whose top is *YYSSP, return 0 iff YYTOKEN can
   eventually (after perhaps some reductions) be shifted, return 1 if
   not, or return YYENOMEM if memory is exhausted.  As preconditions and
   postconditions: *YYES_CAPACITY is the allocated size of the array to
   which *YYES points, and either *YYES = YYESA or *YYES points to an
   array allocated with YYSTACK_ALLOC.  yy_lac may overwrite the
   contents of either array, alter *YYES and *YYES_CAPACITY, and free
   any old *YYES other than YYESA.  */
static int
yy_lac (yy_state_t *yyesa, yy_state_t **yyes,
        YYPTRDIFF_T *yyes_capacity, yy_state_t *yyssp, yysymbol_kind_t yytoken)
{
  yy_state_t *yyes_prev = yyssp;
  yy_state_t *yyesp = yyes_prev;
  /* Reduce until we encounter a shift and thereby accept the token.  */
  YYDPRINTF ((stderr, "LAC: checking lookahead %s:", yysymbol_name (yytoken)));
  if (yytoken == YYSYMBOL_YYUNDEF)
    {
      YYDPRINTF ((stderr, " Always Err\n"));
      return 1;
    }
  while (1)
    {
      int yyrule = yypact[+*yyesp];
      if (yypact_value_is_default (yyrule)
          || (yyrule += yytoken) < 0 || YYLAST < yyrule
          || yycheck[yyrule] != yytoken)
        {
          /* Use the default action.  */
          yyrule = yydefact[+*yyesp];
          if (yyrule == 0)
            {
              YYDPRINTF ((stderr, " Err\n"));
              return 1;
            }
        }
      else
        {
          /* Use the action from yytable.  */
          yyrule = yytable[yyrule];
          if (yytable_value_is_error (yyrule))
            {
              YYDPRINTF ((stderr, " Err\n"));
              return 1;
            }
          if (0 < yyrule)
            {
              YYDPRINTF ((stderr, " S%d\n", yyrule));
              return 0;
            }
          yyrule = -yyrule;
        }
      /* By now we know we have to simulate a reduce.  */
      YYDPRINTF ((stderr, " R%d", yyrule - 1));
      {
        /* Pop the corresponding number of values from the stack.  */
        YYPTRDIFF_T yylen = yyr2[yyrule];
        /* First pop from the LAC stack as many tokens as possible.  */
        if (yyesp != yyes_prev)
          {
            YYPTRDIFF_T yysize = yyesp - *yyes + 1;
            if (yylen < yysize)
              {
                yyesp -= yylen;
                yylen = 0;
              }
            else
              {
                yyesp = yyes_prev;
                yylen -= yysize;
              }
          }
        /* Only afterwards look at the main stack.  */
        if (yylen)
          yyesp = yyes_prev -= yylen;
      }
      /* Push the resulting state of the reduction.  */
      {
        yy_state_fast_t yystate;
        {
          const int yylhs = yyr1[yyrule] - YYNTOKENS;
          const int yyi = yypgoto[yylhs] + *yyesp;
          yystate = (0 <= yyi && yyi <= YYLAST && yycheck[yyi] == *yyesp
                     ? yytable[yyi]
                     : yydefgoto[yylhs]);
        }
        if (yyesp == yyes_prev)
          {
            yyesp = *yyes;
            YY_IGNORE_USELESS_CAST_BEGIN
            *yyesp = YY_CAST (yy_state_t, yystate);
            YY_IGNORE_USELESS_CAST_END
          }
        else
          {
            if (yy_lac_stack_realloc (yyes_capacity, 1,
#if YYDEBUG
                                      " (", ")",
#endif
                                      yyes, yyesa, &yyesp, yyes_prev))
              {
                YYDPRINTF ((stderr, "\n"));
                return YYENOMEM;
              }
            YY_IGNORE_USELESS_CAST_BEGIN
            *++yyesp = YY_CAST (yy_state_t, yystate);
            YY_IGNORE_USELESS_CAST_END
          }
        YYDPRINTF ((stderr, " G%d", yystate));
      }
    }
}

/* Context of a parse error.  */
typedef struct
{
  yy_state_t *yyssp;
  yy_state_t *yyesa;
  yy_state_t **yyes;
  YYPTRDIFF_T *yyes_capacity;
  yysymbol_kind_t yytoken;
} yypcontext_t;

/* Put in YYARG at most YYARGN of the expected tokens given the
   current YYCTX, and return the number of tokens stored in YYARG.  If
   YYARG is null, return the number of expected tokens (guaranteed to
   be less than YYNTOKENS).  Return YYENOMEM on memory exhaustion.
   Return 0 if there are more than YYARGN expected tokens, yet fill
   YYARG up to YYARGN. */
static int
yypcontext_expected_tokens (const yypcontext_t *yyctx,
                            yysymbol_kind_t yyarg[], int yyargn)
{
  /* Actual size of YYARG. */
  int yycount = 0;

  int yyx;
  for (yyx = 0; yyx < YYNTOKENS; ++yyx)
    {
      yysymbol_kind_t yysym = YY_CAST (yysymbol_kind_t, yyx);
      if (yysym != YYSYMBOL_YYerror && yysym != YYSYMBOL_YYUNDEF)
        switch (yy_lac (yyctx->yyesa, yyctx->yyes, yyctx->yyes_capacity, yyctx->yyssp, yysym))
          {
          case YYENOMEM:
            return YYENOMEM;
          case 1:
            continue;
          default:
            if (!yyarg)
              ++yycount;
            else if (yycount == yyargn)
              return 0;
            else
              yyarg[yycount++] = yysym;
          }
    }
  if (yyarg && yycount == 0 && 0 < yyargn)
    yyarg[0] = YYSYMBOL_YYEMPTY;
  return yycount;
}




#ifndef yystrlen
# if defined __GLIBC__ && defined _STRING_H
#  define yystrlen(S) (YY_CAST (YYPTRDIFF_T, strlen (S)))
# else
/* Return the length of YYSTR.  */
static YYPTRDIFF_T
yystrlen (const char *yystr)
{
  YYPTRDIFF_T yylen;
  for (yylen = 0; yystr[yylen]; yylen++)
    continue;
  return yylen;
}
# endif
#endif

#ifndef yystpcpy
# if defined __GLIBC__ && defined _STRING_H && defined _GNU_SOURCE
#  define yystpcpy stpcpy
# else
/* Copy YYSRC to YYDEST, returning the address of the terminating '\0' in
   YYDEST.  */
static char *
yystpcpy (char *yydest, const char *yysrc)
{
  char *yyd = yydest;
  const char *yys = yysrc;

  while ((*yyd++ = *yys++) != '\0')
    continue;

  return yyd - 1;
}
# endif
#endif

#ifndef yytnamerr
/* Copy to YYRES the contents of YYSTR after stripping away unnecessary
   quotes and backslashes, so that it's suitable for yyerror.  The
   heuristic is that double-quoting is unnecessary unless the string
   contains an apostrophe, a comma, or backslash (other than
   backslash-backslash).  YYSTR is taken from yytname.  If YYRES is
   null, do not copy; instead, return the length of what the result
   would have been.  */
static YYPTRDIFF_T
yytnamerr (char *yyres, const char *yystr)
{
  if (*yystr == '"')
    {
      YYPTRDIFF_T yyn = 0;
      char const *yyp = yystr;
      for (;;)
        switch (*++yyp)
          {
          case '\'':
          case ',':
            goto do_not_strip_quotes;

          case '\\':
            if (*++yyp != '\\')
              goto do_not_strip_quotes;
            else
              goto append;

          append:
          default:
            if (yyres)
              yyres[yyn] = *yyp;
            yyn++;
            break;

          case '"':
            if (yyres)
              yyres[yyn] = '\0';
            return yyn;
          }
    do_not_strip_quotes: ;
    }

  if (yyres)
    return yystpcpy (yyres, yystr) - yyres;
  else
    return yystrlen (yystr);
}
#endif


static int
yy_syntax_error_arguments (const yypcontext_t *yyctx,
                           yysymbol_kind_t yyarg[], int yyargn)
{
  /* Actual size of YYARG. */
  int yycount = 0;
  /* There are many possibilities here to consider:
     - If this state is a consistent state with a default action, then
       the only way this function was invoked is if the default action
       is an error action.  In that case, don't check for expected
       tokens because there are none.
     - The only way there can be no lookahead present (in yychar) is if
       this state is a consistent state with a default action.  Thus,
       detecting the absence of a lookahead is sufficient to determine
       that there is no unexpected or expected token to report.  In that
       case, just report a simple "syntax error".
     - Don't assume there isn't a lookahead just because this state is a
       consistent state with a default action.  There might have been a
       previous inconsistent state, consistent state with a non-default
       action, or user semantic action that manipulated yychar.
       In the first two cases, it might appear that the current syntax
       error should have been detected in the previous state when yy_lac
       was invoked.  However, at that time, there might have been a
       different syntax error that discarded a different initial context
       during error recovery, leaving behind the current lookahead.
  */
  if (yyctx->yytoken != YYSYMBOL_YYEMPTY)
    {
      int yyn;
      YYDPRINTF ((stderr, "Constructing syntax error message\n"));
      if (yyarg)
        yyarg[yycount] = yyctx->yytoken;
      ++yycount;
      yyn = yypcontext_expected_tokens (yyctx,
                                        yyarg ? yyarg + 1 : yyarg, yyargn - 1);
      if (yyn == YYENOMEM)
        return YYENOMEM;
      else if (yyn == 0)
        YYDPRINTF ((stderr, "No expected tokens.\n"));
      else
        yycount += yyn;
    }
  return yycount;
}

/* Copy into *YYMSG, which is of size *YYMSG_ALLOC, an error message
   about the unexpected token YYTOKEN for the state stack whose top is
   YYSSP.  In order to see if a particular token T is a
   valid looakhead, invoke yy_lac (YYESA, YYES, YYES_CAPACITY, YYSSP, T).

   Return 0 if *YYMSG was successfully written.  Return -1 if *YYMSG is
   not large enough to hold the message.  In that case, also set
   *YYMSG_ALLOC to the required number of bytes.  Return YYENOMEM if the
   required number of bytes is too large to store or if
   yy_lac returned YYENOMEM.  */
static int
yysyntax_error (YYPTRDIFF_T *yymsg_alloc, char **yymsg,
                const yypcontext_t *yyctx)
{
  enum { YYARGS_MAX = 5 };
  /* Internationalized format string. */
  const char *yyformat = YY_NULLPTR;
  /* Arguments of yyformat: reported tokens (one for the "unexpected",
     one per "expected"). */
  yysymbol_kind_t yyarg[YYARGS_MAX];
  /* Cumulated lengths of YYARG.  */
  YYPTRDIFF_T yysize = 0;

  /* Actual size of YYARG. */
  int yycount = yy_syntax_error_arguments (yyctx, yyarg, YYARGS_MAX);
  if (yycount == YYENOMEM)
    return YYENOMEM;

  switch (yycount)
    {
#define YYCASE_(N, S)                       \
      case N:                               \
        yyformat = S;                       \
        break
    default: /* Avoid compiler warnings. */
      YYCASE_(0, YY_("syntax error"));
      YYCASE_(1, YY_("syntax error, unexpected %s"));
      YYCASE_(2, YY_("syntax error, unexpected %s, expecting %s"));
      YYCASE_(3, YY_("syntax error, unexpected %s, expecting %s or %s"));
      YYCASE_(4, YY_("syntax error, unexpected %s, expecting %s or %s or %s"));
      YYCASE_(5, YY_("syntax error, unexpected %s, expecting %s or %s or %s or %s"));
#undef YYCASE_
    }

  /* Compute error message size.  Don't count the "%s"s, but reserve
     room for the terminator.  */
  yysize = yystrlen (yyformat) - 2 * yycount + 1;
  {
    int yyi;
    for (yyi = 0; yyi < yycount; ++yyi)
      {
        YYPTRDIFF_T yysize1
          = yysize + yytnamerr (YY_NULLPTR, yytname[yyarg[yyi]]);
        if (yysize <= yysize1 && yysize1 <= YYSTACK_ALLOC_MAXIMUM)
          yysize = yysize1;
        else
          return YYENOMEM;
      }
  }

  if (*yymsg_alloc < yysize)
    {
      *yymsg_alloc = 2 * yysize;
      if (! (yysize <= *yymsg_alloc
             && *yymsg_alloc <= YYSTACK_ALLOC_MAXIMUM))
        *yymsg_alloc = YYSTACK_ALLOC_MAXIMUM;
      return -1;
    }

  /* Avoid sprintf, as that infringes on the user's name space.
     Don't have undefined behavior even if the translation
     produced a string with the wrong number of "%s"s.  */
  {
    char *yyp = *yymsg;
    int yyi = 0;
    while ((*yyp = *yyformat) != '\0')
      if (*yyp == '%' && yyformat[1] == 's' && yyi < yycount)
        {
          yyp += yytnamerr (yyp, yytname[yyarg[yyi++]]);
          yyformat += 2;
        }
      else
        {
          ++yyp;
          ++yyformat;
        }
  }
  return 0;
}


/*-----------------------------------------------.
| Release the memory associated to this symbol.  |
`-----------------------------------------------*/

static void
yydestruct (const char *yymsg,
            yysymbol_kind_t yykind, YYSTYPE *yyvaluep)
{
  YY_USE (yyvaluep);
  if (!yymsg)
    yymsg = "Deleting";
  YY_SYMBOL_PRINT (yymsg, yykind, yyvaluep, yylocationp);

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  YY_USE (yykind);
  YY_IGNORE_MAYBE_UNINITIALIZED_END
}


/* Lookahead token kind.  */
int yychar;

/* The semantic value of the lookahead symbol.  */
YYSTYPE yylval;
/* Number of syntax errors so far.  */
int yynerrs;




/*----------.
| yyparse.  |
`----------*/

int
yyparse (void)
{
    yy_state_fast_t yystate = 0;
    /* Number of tokens to shift before error messages enabled.  */
    int yyerrstatus = 0;

    /* Refer to the stacks through separate pointers, to allow yyoverflow
       to reallocate them elsewhere.  */

    /* Their size.  */
    YYPTRDIFF_T yystacksize = YYINITDEPTH;

    /* The state stack: array, bottom, top.  */
    yy_state_t yyssa[YYINITDEPTH];
    yy_state_t *yyss = yyssa;
    yy_state_t *yyssp = yyss;

    /* The semantic value stack: array, bottom, top.  */
    YYSTYPE yyvsa[YYINITDEPTH];
    YYSTYPE *yyvs = yyvsa;
    YYSTYPE *yyvsp = yyvs;

    yy_state_t yyesa[20];
    yy_state_t *yyes = yyesa;
    YYPTRDIFF_T yyes_capacity = 20 < YYMAXDEPTH ? 20 : YYMAXDEPTH;

  /* Whether LAC context is established.  A Boolean.  */
  int yy_lac_established = 0;
  int yyn;
  /* The return value of yyparse.  */
  int yyresult;
  /* Lookahead symbol kind.  */
  yysymbol_kind_t yytoken = YYSYMBOL_YYEMPTY;
  /* The variables used to return semantic value and location from the
     action routines.  */
  YYSTYPE yyval;

  /* Buffer for error messages, and its allocated size.  */
  char yymsgbuf[128];
  char *yymsg = yymsgbuf;
  YYPTRDIFF_T yymsg_alloc = sizeof yymsgbuf;

#define YYPOPSTACK(N)   (yyvsp -= (N), yyssp -= (N))

  /* The number of symbols on the RHS of the reduced rule.
     Keep to zero when no symbol should be popped.  */
  int yylen = 0;

  YYDPRINTF ((stderr, "Starting parse\n"));

  yychar = YYEMPTY; /* Cause a token to be read.  */

  goto yysetstate;


/*------------------------------------------------------------.
| yynewstate -- push a new state, which is found in yystate.  |
`------------------------------------------------------------*/
yynewstate:
  /* In all cases, when you get here, the value and location stacks
     have just been pushed.  So pushing a state here evens the stacks.  */
  yyssp++;


/*--------------------------------------------------------------------.
| yysetstate -- set current state (the top of the stack) to yystate.  |
`--------------------------------------------------------------------*/
yysetstate:
  YYDPRINTF ((stderr, "Entering state %d\n", yystate));
  YY_ASSERT (0 <= yystate && yystate < YYNSTATES);
  YY_IGNORE_USELESS_CAST_BEGIN
  *yyssp = YY_CAST (yy_state_t, yystate);
  YY_IGNORE_USELESS_CAST_END
  YY_STACK_PRINT (yyss, yyssp);

  if (yyss + yystacksize - 1 <= yyssp)
#if !defined yyoverflow && !defined YYSTACK_RELOCATE
    YYNOMEM;
#else
    {
      /* Get the current used size of the three stacks, in elements.  */
      YYPTRDIFF_T yysize = yyssp - yyss + 1;

# if defined yyoverflow
      {
        /* Give user a chance to reallocate the stack.  Use copies of
           these so that the &'s don't force the real ones into
           memory.  */
        yy_state_t *yyss1 = yyss;
        YYSTYPE *yyvs1 = yyvs;

        /* Each stack pointer address is followed by the size of the
           data in use in that stack, in bytes.  This used to be a
           conditional around just the two extra args, but that might
           be undefined if yyoverflow is a macro.  */
        yyoverflow (YY_("memory exhausted"),
                    &yyss1, yysize * YYSIZEOF (*yyssp),
                    &yyvs1, yysize * YYSIZEOF (*yyvsp),
                    &yystacksize);
        yyss = yyss1;
        yyvs = yyvs1;
      }
# else /* defined YYSTACK_RELOCATE */
      /* Extend the stack our own way.  */
      if (YYMAXDEPTH <= yystacksize)
        YYNOMEM;
      yystacksize *= 2;
      if (YYMAXDEPTH < yystacksize)
        yystacksize = YYMAXDEPTH;

      {
        yy_state_t *yyss1 = yyss;
        union yyalloc *yyptr =
          YY_CAST (union yyalloc *,
                   YYSTACK_ALLOC (YY_CAST (YYSIZE_T, YYSTACK_BYTES (yystacksize))));
        if (! yyptr)
          YYNOMEM;
        YYSTACK_RELOCATE (yyss_alloc, yyss);
        YYSTACK_RELOCATE (yyvs_alloc, yyvs);
#  undef YYSTACK_RELOCATE
        if (yyss1 != yyssa)
          YYSTACK_FREE (yyss1);
      }
# endif

      yyssp = yyss + yysize - 1;
      yyvsp = yyvs + yysize - 1;

      YY_IGNORE_USELESS_CAST_BEGIN
      YYDPRINTF ((stderr, "Stack size increased to %ld\n",
                  YY_CAST (long, yystacksize)));
      YY_IGNORE_USELESS_CAST_END

      if (yyss + yystacksize - 1 <= yyssp)
        YYABORT;
    }
#endif /* !defined yyoverflow && !defined YYSTACK_RELOCATE */


  if (yystate == YYFINAL)
    YYACCEPT;

  goto yybackup;


/*-----------.
| yybackup.  |
`-----------*/
yybackup:
  /* Do appropriate processing given the current state.  Read a
     lookahead token if we need one and don't already have one.  */

  /* First try to decide what to do without reference to lookahead token.  */
  yyn = yypact[yystate];
  if (yypact_value_is_default (yyn))
    goto yydefault;

  /* Not known => get a lookahead token if don't already have one.  */

  /* YYCHAR is either empty, or end-of-input, or a valid lookahead.  */
  if (yychar == YYEMPTY)
    {
      YYDPRINTF ((stderr, "Reading a token\n"));
      yychar = yylex ();
    }

  if (yychar <= YYEOF)
    {
      yychar = YYEOF;
      yytoken = YYSYMBOL_YYEOF;
      YYDPRINTF ((stderr, "Now at end of input.\n"));
    }
  else if (yychar == YYerror)
    {
      /* The scanner already issued an error message, process directly
         to error recovery.  But do not keep the error token as
         lookahead, it is too special and may lead us to an endless
         loop in error recovery. */
      yychar = YYUNDEF;
      yytoken = YYSYMBOL_YYerror;
      goto yyerrlab1;
    }
  else
    {
      yytoken = YYTRANSLATE (yychar);
      YY_SYMBOL_PRINT ("Next token is", yytoken, &yylval, &yylloc);
    }

  /* If the proper action on seeing token YYTOKEN is to reduce or to
     detect an error, take that action.  */
  yyn += yytoken;
  if (yyn < 0 || YYLAST < yyn || yycheck[yyn] != yytoken)
    {
      YY_LAC_ESTABLISH;
      goto yydefault;
    }
  yyn = yytable[yyn];
  if (yyn <= 0)
    {
      if (yytable_value_is_error (yyn))
        goto yyerrlab;
      yyn = -yyn;
      YY_LAC_ESTABLISH;
      goto yyreduce;
    }

  /* Count tokens shifted since error; after three, turn off error
     status.  */
  if (yyerrstatus)
    yyerrstatus--;

  /* Shift the lookahead token.  */
  YY_SYMBOL_PRINT ("Shifting", yytoken, &yylval, &yylloc);
  yystate = yyn;
  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END

  /* Discard the shifted token.  */
  yychar = YYEMPTY;
  YY_LAC_DISCARD ("shift");
  goto yynewstate;


/*-----------------------------------------------------------.
| yydefault -- do the default action for the current state.  |
`-----------------------------------------------------------*/
yydefault:
  yyn = yydefact[yystate];
  if (yyn == 0)
    goto yyerrlab;
  goto yyreduce;


/*-----------------------------.
| yyreduce -- do a reduction.  |
`-----------------------------*/
yyreduce:
  /* yyn is the number of a rule to reduce with.  */
  yylen = yyr2[yyn];

  /* If YYLEN is nonzero, implement the default value of the action:
     '$$ = $1'.

     Otherwise, the following line sets YYVAL to garbage.
     This behavior is undocumented and Bison
     users should not rely upon it.  Assigning to YYVAL
     unconditionally makes the parser a bit smaller, and it avoids a
     GCC warning that YYVAL may be used uninitialized.  */
  yyval = yyvsp[1-yylen];


  YY_REDUCE_PRINT (yyn);
  {
    int yychar_backup = yychar;
    switch (yyn)
      {
  case 2: /* program: declarators  */
              { root = ast_new_subtree(NODE_PROGRAM, (yyvsp[0].ast), NULL); }
    break;

  case 3: /* declarators: declarator  */
                         { ast_manager_add_declarator(&(yyval.ast), (yyvsp[0].ast)); }
    break;

  case 4: /* declarators: declarator declarators  */
                         { ast_manager_add_declarator(&(yyval.ast), (yyvsp[-1].ast)); }
    break;

  case 5: /* declarator: declarator_var  */
                  { (yyval.ast) = (yyvsp[0].ast); }
    break;

  case 6: /* declarator: declarator_func  */
                  { (yyval.ast) = (yyvsp[0].ast); }
    break;

  case 10: /* init_declarator: ID  */
                                                            { var_add_declaration((yyvsp[0].str), vartype); }
    break;

  case 11: /* init_declarator: ID ASSIGN expr  */
                                                            { var_add_declaration((yyvsp[-2].str), vartype); }
    break;

  case 12: /* init_declarator: ID LBRACKET INTVAL RBRACKET  */
                                                            { var_add_declaration((yyvsp[-3].str), vartype); }
    break;

  case 13: /* init_declarator: ID LBRACKET INTVAL RBRACKET ASSIGN LCURLY RCURLY  */
                                                            { var_add_declaration((yyvsp[-6].str), vartype); }
    break;

  case 14: /* init_declarator: ID LBRACKET INTVAL RBRACKET ASSIGN LCURLY exprs RCURLY  */
                                                            { var_add_declaration((yyvsp[-7].str), vartype); }
    break;

  case 15: /* init_declarator: ID LBRACKET INTVAL RBRACKET ASSIGN STRING  */
                                                            { var_add_declaration((yyvsp[-5].str), TYPE_STR); }
    break;

  case 16: /* $@1: %empty  */
                    { retvartype = vartype; funcname = (yyvsp[0].str); }
    break;

  case 17: /* $@2: %empty  */
                                                                  {
	params_count = 0;
	scope_enter();
  }
    break;

  case 18: /* declarator_func: type_specifier ID $@1 LPAR $@2 opt_func_paramlist RPAR declarator_func_end  */
                                              {
	if(function_add_declaration((yyvsp[-6].str), (yyvsp[-7].ast)) == -1){
		exit(1);
	}
	scope_exit();
  }
    break;

  case 19: /* declarator_func_end: DELI  */
              { function_definition = 0; }
    break;

  case 20: /* declarator_func_end: func_block  */
              { function_definition = 1; }
    break;

  case 23: /* func_paramlist: func_param  */
                { params_count++; }
    break;

  case 24: /* $@3: %empty  */
                { params_count++; }
    break;

  case 27: /* func_param: type_specifier ID  */
                                                          { var_add_declaration((yyvsp[0].str), vartype); }
    break;

  case 28: /* func_param: type_specifier ID LBRACKET RBRACKET  */
                                                          { var_add_declaration((yyvsp[-2].str), vartype); }
    break;

  case 29: /* func_param: type_specifier ID LBRACKET expr RBRACKET  */
                                                          { var_add_declaration((yyvsp[-3].str), vartype); }
    break;

  case 33: /* $@4: %empty  */
         {
	function_definition = 0;
	if(function_add_declaration(funcname, retvartype) == -1){
	}
	function_definition = 1;
  }
    break;

  case 36: /* $@5: %empty  */
         { scope_enter(); }
    break;

  case 37: /* block: LCURLY $@5 block_item_list RCURLY  */
                                                   { scope_exit(); }
    break;

  case 50: /* jump_stmt: RETURN DELI  */
                     { function_check_return(TYPE_VOID); }
    break;

  case 51: /* jump_stmt: RETURN expr DELI  */
                     { function_check_return((yyvsp[-1].ast).type);  }
    break;

  case 52: /* while_stmt: WHILE LPAR expr RPAR block  */
                               { check_if_while_condition(&(yyvsp[-2].ast)); }
    break;

  case 53: /* if_stmt: IF LPAR expr RPAR block  */
                                       { check_if_while_condition(&(yyvsp[-2].ast)); }
    break;

  case 54: /* if_stmt: IF LPAR expr RPAR block ELSE block  */
                                       { check_if_while_condition(&(yyvsp[-4].ast)); }
    break;

  case 55: /* if_stmt: IF LPAR expr RPAR block ELSE if_stmt  */
                                       { check_if_while_condition(&(yyvsp[-4].ast)); }
    break;

  case 56: /* assign_stmt: ID ASSIGN expr DELI  */
                                               { var_check_declaration((yyvsp[-3].str)); }
    break;

  case 57: /* assign_stmt: ID LBRACKET expr RBRACKET ASSIGN expr DELI  */
                                               { var_check_declaration((yyvsp[-6].str)); }
    break;

  case 59: /* expr: LPAR expr RPAR  */
                            { (yyval.ast) = (yyvsp[-1].ast) ; }
    break;

  case 60: /* expr: ID  */
                            { var_check_declaration((yyvsp[0].str)); var_to_expr((yyvsp[0].str), &(yyval.ast)); }
    break;

  case 61: /* expr: INTVAL  */
                            { (yyval.ast) = ast_new_node(NODE_INT_VAL); ast_set_data((yyval.ast), ); }
    break;

  case 62: /* expr: FLOATVAL  */
                            { (yyval.ast) = (yyvsp[0].ast); }
    break;

  case 63: /* expr: STRING  */
                            { (yyval.ast) = (yyvsp[0].ast); }
    break;

  case 64: /* expr: CHR  */
                            { (yyval.ast) = (yyvsp[0].ast); }
    break;

  case 65: /* expr: expr PLUS expr  */
                            { binary_operation((yyvsp[-2].ast), (yyvsp[0].ast), &(yyval.ast), NODE_SUM, literal_sum); }
    break;

  case 66: /* expr: expr MINUS expr  */
                            { binary_operation((yyvsp[-2].ast), (yyvsp[0].ast), &(yyval.ast), NODE_MINUS, literal_sub); }
    break;

  case 67: /* expr: expr STAR expr  */
                            { binary_operation((yyvsp[-2].ast), (yyvsp[0].ast), &(yyval.ast), NODE_MULT, literal_mul); }
    break;

  case 68: /* expr: expr DIVISION expr  */
                            { binary_operation((yyvsp[-2].ast), (yyvsp[0].ast), &(yyval.ast), NODE_DIV, literal_div); }
    break;

  case 69: /* expr: expr MODULUS expr  */
                            {  }
    break;

  case 70: /* expr: expr OR expr  */
                            { binary_operation((yyvsp[-2].ast), (yyvsp[0].ast), &(yyval.ast), NODE_OR, literal_or);  }
    break;

  case 71: /* expr: expr AND expr  */
                            { binary_operation((yyvsp[-2].ast), (yyvsp[0].ast), &(yyval.ast), NODE_AND, literal_an); }
    break;

  case 72: /* expr: expr EQ expr  */
                            { binary_operation((yyvsp[-2].ast), (yyvsp[0].ast), &(yyval.ast), NODE_EQ, literal_eq);  }
    break;

  case 73: /* expr: expr NOTEQ expr  */
                            { binary_operation((yyvsp[-2].ast), (yyvsp[0].ast), &(yyval.ast), NODE_NE, literal_ne);  }
    break;

  case 74: /* expr: expr LT expr  */
                            { binary_operation((yyvsp[-2].ast), (yyvsp[0].ast), &(yyval.ast), NODE_LT, literal_lt);  }
    break;

  case 75: /* expr: expr GT expr  */
                            { binary_operation((yyvsp[-2].ast), (yyvsp[0].ast), &(yyval.ast), NODE_GT, literal_gt);  }
    break;

  case 76: /* expr: MINUS INTVAL  */
                            { (yyval.ast) = (yyvsp[0].ast); }
    break;

  case 77: /* expr: PLUS INTVAL  */
                            { (yyval.ast) = (yyvsp[0].ast); }
    break;

  case 78: /* expr: MINUS FLOATVAL  */
                            { (yyval.ast) = (yyvsp[0].ast); }
    break;

  case 79: /* expr: PLUS FLOATVAL  */
                            { (yyval.ast) = (yyvsp[0].ast); }
    break;

  case 80: /* expr: fcall  */
                            {  }
    break;

  case 81: /* expr: AMP ID  */
                            { var_check_declaration((yyvsp[0].str)); }
    break;

  case 82: /* expr: ID LBRACKET expr RBRACKET  */
                            { var_check_declaration((yyvsp[-3].str)); (yyval.ast).type = vartype; }
    break;

  case 83: /* fcall: fcaller LPAR RPAR  */
                            { function_check_call((yyvsp[-2].str)); fcall_active = 0; }
    break;

  case 84: /* fcall: fcaller LPAR exprs RPAR  */
                            { function_check_call((yyvsp[-3].str)); fcall_active = 0; }
    break;

  case 85: /* fcaller: ID  */
          { function_check_declaration((yyvsp[0].str)); vector_clear(funcargs); fcall_active = 1; (yyval.str) = (yyvsp[0].str); }
    break;

  case 86: /* fcaller: PRINTF  */
          { vector_clear(funcargs); fcall_active = 1; (yyval.str) = (yyvsp[0].ast); }
    break;

  case 87: /* fcaller: SCANF  */
          { vector_clear(funcargs); fcall_active = 1; (yyval.str) = (yyvsp[0].ast); }
    break;

  case 88: /* exprs: expr  */
       { if(fcall_active == 1){ vector_append(funcargs, &(yyvsp[0].ast)) ; }; }
    break;

  case 89: /* $@6: %empty  */
       { if(fcall_active == 1){ vector_append(funcargs, &(yyvsp[0].ast)) ; }; }
    break;

  case 91: /* type_specifier: INT  */
          { vartype = TYPE_INT;  }
    break;

  case 92: /* type_specifier: CHAR  */
          { vartype = TYPE_CHAR; }
    break;

  case 93: /* type_specifier: FLOAT  */
          { vartype = TYPE_FLT;  }
    break;

  case 94: /* type_specifier: VOID  */
          { vartype = TYPE_VOID; }
    break;



        default: break;
      }
    if (yychar_backup != yychar)
      YY_LAC_DISCARD ("yychar change");
  }
  /* User semantic actions sometimes alter yychar, and that requires
     that yytoken be updated with the new translation.  We take the
     approach of translating immediately before every use of yytoken.
     One alternative is translating here after every semantic action,
     but that translation would be missed if the semantic action invokes
     YYABORT, YYACCEPT, or YYERROR immediately after altering yychar or
     if it invokes YYBACKUP.  In the case of YYABORT or YYACCEPT, an
     incorrect destructor might then be invoked immediately.  In the
     case of YYERROR or YYBACKUP, subsequent parser actions might lead
     to an incorrect destructor call or verbose syntax error message
     before the lookahead is translated.  */
  YY_SYMBOL_PRINT ("-> $$ =", YY_CAST (yysymbol_kind_t, yyr1[yyn]), &yyval, &yyloc);

  YYPOPSTACK (yylen);
  yylen = 0;

  *++yyvsp = yyval;

  /* Now 'shift' the result of the reduction.  Determine what state
     that goes to, based on the state we popped back to and the rule
     number reduced by.  */
  {
    const int yylhs = yyr1[yyn] - YYNTOKENS;
    const int yyi = yypgoto[yylhs] + *yyssp;
    yystate = (0 <= yyi && yyi <= YYLAST && yycheck[yyi] == *yyssp
               ? yytable[yyi]
               : yydefgoto[yylhs]);
  }

  goto yynewstate;


/*--------------------------------------.
| yyerrlab -- here on detecting error.  |
`--------------------------------------*/
yyerrlab:
  /* Make sure we have latest lookahead translation.  See comments at
     user semantic actions for why this is necessary.  */
  yytoken = yychar == YYEMPTY ? YYSYMBOL_YYEMPTY : YYTRANSLATE (yychar);
  /* If not already recovering from an error, report this error.  */
  if (!yyerrstatus)
    {
      ++yynerrs;
      {
        yypcontext_t yyctx
          = {yyssp, yyesa, &yyes, &yyes_capacity, yytoken};
        char const *yymsgp = YY_("syntax error");
        int yysyntax_error_status;
        if (yychar != YYEMPTY)
          YY_LAC_ESTABLISH;
        yysyntax_error_status = yysyntax_error (&yymsg_alloc, &yymsg, &yyctx);
        if (yysyntax_error_status == 0)
          yymsgp = yymsg;
        else if (yysyntax_error_status == -1)
          {
            if (yymsg != yymsgbuf)
              YYSTACK_FREE (yymsg);
            yymsg = YY_CAST (char *,
                             YYSTACK_ALLOC (YY_CAST (YYSIZE_T, yymsg_alloc)));
            if (yymsg)
              {
                yysyntax_error_status
                  = yysyntax_error (&yymsg_alloc, &yymsg, &yyctx);
                yymsgp = yymsg;
              }
            else
              {
                yymsg = yymsgbuf;
                yymsg_alloc = sizeof yymsgbuf;
                yysyntax_error_status = YYENOMEM;
              }
          }
        yyerror (yymsgp);
        if (yysyntax_error_status == YYENOMEM)
          YYNOMEM;
      }
    }

  if (yyerrstatus == 3)
    {
      /* If just tried and failed to reuse lookahead token after an
         error, discard it.  */

      if (yychar <= YYEOF)
        {
          /* Return failure if at end of input.  */
          if (yychar == YYEOF)
            YYABORT;
        }
      else
        {
          yydestruct ("Error: discarding",
                      yytoken, &yylval);
          yychar = YYEMPTY;
        }
    }

  /* Else will try to reuse lookahead token after shifting the error
     token.  */
  goto yyerrlab1;


/*---------------------------------------------------.
| yyerrorlab -- error raised explicitly by YYERROR.  |
`---------------------------------------------------*/
yyerrorlab:
  /* Pacify compilers when the user code never invokes YYERROR and the
     label yyerrorlab therefore never appears in user code.  */
  if (0)
    YYERROR;
  ++yynerrs;

  /* Do not reclaim the symbols of the rule whose action triggered
     this YYERROR.  */
  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);
  yystate = *yyssp;
  goto yyerrlab1;


/*-------------------------------------------------------------.
| yyerrlab1 -- common code for both syntax error and YYERROR.  |
`-------------------------------------------------------------*/
yyerrlab1:
  yyerrstatus = 3;      /* Each real token shifted decrements this.  */

  /* Pop stack until we find a state that shifts the error token.  */
  for (;;)
    {
      yyn = yypact[yystate];
      if (!yypact_value_is_default (yyn))
        {
          yyn += YYSYMBOL_YYerror;
          if (0 <= yyn && yyn <= YYLAST && yycheck[yyn] == YYSYMBOL_YYerror)
            {
              yyn = yytable[yyn];
              if (0 < yyn)
                break;
            }
        }

      /* Pop the current state because it cannot handle the error token.  */
      if (yyssp == yyss)
        YYABORT;


      yydestruct ("Error: popping",
                  YY_ACCESSING_SYMBOL (yystate), yyvsp);
      YYPOPSTACK (1);
      yystate = *yyssp;
      YY_STACK_PRINT (yyss, yyssp);
    }

  /* If the stack popping above didn't lose the initial context for the
     current lookahead token, the shift below will for sure.  */
  YY_LAC_DISCARD ("error recovery");

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END


  /* Shift the error token.  */
  YY_SYMBOL_PRINT ("Shifting", YY_ACCESSING_SYMBOL (yyn), yyvsp, yylsp);

  yystate = yyn;
  goto yynewstate;


/*-------------------------------------.
| yyacceptlab -- YYACCEPT comes here.  |
`-------------------------------------*/
yyacceptlab:
  yyresult = 0;
  goto yyreturnlab;


/*-----------------------------------.
| yyabortlab -- YYABORT comes here.  |
`-----------------------------------*/
yyabortlab:
  yyresult = 1;
  goto yyreturnlab;


/*-----------------------------------------------------------.
| yyexhaustedlab -- YYNOMEM (memory exhaustion) comes here.  |
`-----------------------------------------------------------*/
yyexhaustedlab:
  yyerror (YY_("memory exhausted"));
  yyresult = 2;
  goto yyreturnlab;


/*----------------------------------------------------------.
| yyreturnlab -- parsing is finished, clean up and return.  |
`----------------------------------------------------------*/
yyreturnlab:
  if (yychar != YYEMPTY)
    {
      /* Make sure we have latest lookahead translation.  See comments at
         user semantic actions for why this is necessary.  */
      yytoken = YYTRANSLATE (yychar);
      yydestruct ("Cleanup: discarding lookahead",
                  yytoken, &yylval);
    }
  /* Do not reclaim the symbols of the rule whose action triggered
     this YYABORT or YYACCEPT.  */
  YYPOPSTACK (yylen);
  YY_STACK_PRINT (yyss, yyssp);
  while (yyssp != yyss)
    {
      yydestruct ("Cleanup: popping",
                  YY_ACCESSING_SYMBOL (+*yyssp), yyvsp);
      YYPOPSTACK (1);
    }
#ifndef yyoverflow
  if (yyss != yyssa)
    YYSTACK_FREE (yyss);
#endif
  if (yyes != yyesa)
    YYSTACK_FREE (yyes);
  if (yymsg != yymsgbuf)
    YYSTACK_FREE (yymsg);
  return yyresult;
}



/* parser internals */

void parser_init(void){
	functable = func_table_new();
	scope_manager = scope_manager_new();
	funcargs = vector_new(12);
}

void parser_deinit(void){
	scope_manager_destroy(&scope_manager);
	func_table_destroy(&functable);
	vector_destroy(&funcargs, NULL);
}

void parser_info(void){
#ifdef DEBUG_PARSER
	scope_manager_print(scope_manager);
	func_table_print(functable);
#endif
}


/* scope */

void scope_enter(void){
	scope_manager_enter(scope_manager);
}

void scope_exit(void){
	scope_manager_exit(scope_manager);
}



/* variables */

void var_add_declaration(char* name, enum Type type){
	Scope* scope = scope_manager_get_current_scope(scope_manager);
	if(scope_add(scope, name, yylineno, type) == -1){
		Variable* var = scope_search_by_name(scope, name);
		printf("SEMANTIC ERROR (%d): variable %s has been declared already\n", var->line, name);
		exit(1);
	}
	if(type == TYPE_VOID){
		Variable* var = scope_search_by_name(scope, name);
		printf("SEMANTIC ERROR (%d): variable %s has been declared with type VOID\n", var->line, name);
		exit(1);
	}
#ifdef DEBUG_SCOPE
	printf("Variable %s added to scope %d\n", name, scope_get_id(scope));
#endif
}

void var_check_declaration(char* name){
	if(scope_manager_search_by_name(scope_manager, name) == NULL){
		printf("SEMANTIC ERROR (%d): variable %s hasn't been declared yet\n", yylineno, name);
	}
}

Variable* var_search(char* name){
	Scope* scope = scope_manager_search_by_name(scope_manager, name);
  	if(scope == NULL){
		return NULL;
  	}
	return scope_search_by_name(scope, name);
}

void var_to_expr(char* name, Literal* exp){
	Variable* var = var_search(name);
	if(var == NULL){
		fprintf(stderr, "SEMANTIC ERROR (%d): variable '%s' not found in the current scope\n", yylineno, name);
		return ;
	}
	exp->type = var->token.type;
	exp->value = var->token.value;
}



/* functions */

int function_add_declaration(char* name, enum Type type){
	Scope* scope = scope_manager_get_current_scope(scope_manager);
	if(func_table_add(functable, name, scope, type, params_count, function_definition) == -1){
		if(function_definition){
			printf("SEMANTIC ERROR (%d): function %s has been defined already\n", yylineno, name);
			return -1;
		}

		printf("SEMANTIC WARNING (%d): function %s has been declared already\n", yylineno, name);
	}
	return 0;
}

void function_check_declaration(char* name){
	int missing_function_declared = func_table_search(functable, name) == NULL;
	//int missing_func_ptr = 0;
	if(missing_function_declared){
		printf("SEMANTIC ERROR (%d): function %s hasn't been declared yet\n", yylineno, name);
	}
}

int function_check_call(char* name){
#define STRING_EQ(name1, name2) (strcmp(name1, name2) == 0)
	if(STRING_EQ(name, "printf") || STRING_EQ(name, "scanf")){
		return 0;
	}
#undef STRING_EQ
	Function* f = func_table_search(functable, name);
	if(f == NULL){
		return -1;
	}

	Scope* scope = func_get_scope(f);
	VarTable* vt = scope_get_vartable(scope);
	const int nparams = func_get_nparams(f);
	const int nargs = vector_get_size(funcargs);
	Variable* param = NULL;
	Literal* arg = NULL;
	
	if(nargs != nparams){
		fprintf(stderr, "TYPE ERROR (%d); function %s called with insufficient number of arguments\n", yylineno, name);
		return -1;
	}

	for(int i=0; i<nparams; i++){
		param = vartable_idx(vt, i);
		arg = vector_get_item(funcargs, i);
		if(arg->type != param->token.type){
			fprintf(stderr, "TYPE ERROR (%d); function %s called with argument of wrong type; '%s' should be '%s'\n", yylineno, name, literal_get_typename(arg), literal_get_typename(&param->token));
			return -1;
		}
	}

	return 0;
}

int function_check_return(enum Type rettype){
	if(retvartype != rettype){
		fprintf(stderr, "TYPE ERROR (%d); function %s with wrong return type; '%s' should be '%s'\n", yylineno, funcname, type_get_name(rettype), type_get_name(retvartype));
		return -1;
	}
	return 0;
}



/* misc */

void check_if_while_condition(Literal* exp){
	switch(exp->type){
		case TYPE_INT:
			break;
		case TYPE_VOID:
		case TYPE_FLT:
		case TYPE_STR:
		case TYPE_CHAR:
		default:
		fprintf(stderr, "SEMANTIC ERROR (%d): conditional expression in IF/WHILE", yylineno);
		fprintf(stderr, " is '%s' instead of 'int'\n", literal_get_typename(exp));
			break;
	}
}

void binary_operation(
  AST* op1, AST* op2, AST** res, NodeKind nodekind,
  int (*operation)(Literal*, Literal*, BinaryOperationData*))
{
	Literal* l1;
	Literal* l2;
	BinaryOperationData meta;
	AST* leftnode;
	AST* rightnode;
	NodeKind kind;
	NodeData resdata;

	l1 = ast_get_literal(op1);
	l2 = ast_get_literal(op2);

	if(l1 == NULL || l2 == NULL){
		*res = NULL;
		fprintf(stdout, "Got nil when performing operation %s\n", operator_symb);
		return ;
	}

	if(operation(op1, op2, &meta) == -1){
		fprintf(stderr, "SEMANTIC ERROR (%d):", yylineno);
		fprintf(stderr, "incompatible types for operator: ");
		fprintf(stderr, "LHS is '%s'", literal_get_typename(op1));
		fprintf(stderr, " and RHS is '%s'\n", literal_get_typename(op2));
		exit(1);
	}

	leftnode = op1;
	rightnode = op2;
	NodeKind kind;

	switch(meta.left){
		case CONV_B2I:
			kind = B2I_NODE;
			break;
		case CONV_B2R:
			kind = B2R_NODE;
			break;
		case CONV_B2S:
			kind = B2S_NODE;
			break;
		case CONV_I2R:
			kind = I2R_NODE;
			break;
		case CONV_I2S:
			kind = I2S_NODE;
			break;
		case CONV_R2S:
			kind = R2S_NODE;
			break;
		default:
			kind = NOCONV_NODE;
	}

	if(kind != NOCONV_NODE){
		leftnode = ast_new_subtree(kind, ast_get_data(op1), op1, NULL);
	}

	switch(meta.right){
		case CONV_B2I:
			kind = B2I_NODE;
			break;
		case CONV_B2R:
			kind = B2R_NODE;
			break;
		case CONV_B2S:
			kind = B2S_NODE;
			break;
		case CONV_I2R:
			kind = I2R_NODE;
			break;
		case CONV_I2S:
			kind = I2S_NODE;
			break;
		case CONV_R2S:
			kind = R2S_NODE;
			break;
		default:
			kind = NOCONV_NODE;
	}

	if(kind != NOCONV_NODE){
		rightnode = ast_new_subtree(kind, ast_get_data(op2), op2, NULL);
	}

	resdata.lit.type = meta.type;
	*res = ast_new_subtree(nodekind, resdata, leftnode, rightnode, NULL);
}

void check_assignment(Literal* op1, Literal* res){
	if(literal_assign(op1, res) == -1){
		printf("SEMANTIC ERROR (%d): ", yylineno);
		printf("incompatible types for operator '=', ");
		printf("LHS is '%s' and RHS is '%s'.\n", literal_get_typename(res), literal_get_typename(op1));
	}
}




/* ast related */

void ast_manager_add_declarator(AST** declarators, AST* declarator){
	static int declarators_defined = 0;
	if(declarators_defined == 0){
		*declarators = ast_new_node(NODE_DECLARATORS);
		declarators_defined = 1;
	}
	ast_add_child(declarators, declarator); 
}

AST* ast_manager_new(NodeKind kind){
	AST* node = ast_new_node(kind);
	NodeData data;
	switch(kind){
	        case NODE_INT_VAL:
			data.var.literal.value.
			break;
	        case NODE_FLOAT_VAL:
			break;
	        case NODE_STR_VAL:
			break;
		default:
			break;
	}
	return node;
}
