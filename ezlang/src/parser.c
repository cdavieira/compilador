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
#line 4 "parser/parser.y"

#include <stdio.h>
#include "Literal.h"
#include "tables.h"
#include "scope.h"
#include "ast.h"

extern char* yytext;
extern int yylineno;
static ScopeManager* scope_manager;
AST* root;
Type TYPE_DECL;
int decl_list_created;
int stmt_list_created;

int yylex(void);
void yyerror(char const *s);

void parser_init(void);
void parser_deinit(void);
void enter_scope(void);
void exit_scope(void);
void unary_operation(
	Literal* op1,
	Literal* res,
	int (*operation)(void*, void*),
	const char* operator);
void binary_operation(
	AST* op1,
	AST* op2,
	AST** res,
	int (*operation)(Literal*, Literal*, BinaryOperationData*),
	const char* operator,
	NodeKind nodekind);
void add_var_declaration(char* name, enum Type type);
int check_var_declaration(char* name);
int check_assignment(char* name, enum Type type);
int check_if_repeat_expr(enum Type type);
void save_token_to_var(Literal* props, char* varname);
void load_var_to_token(char* varname, AST** res);
void parser_get_var(char* name, Variable* res);

#line 114 "src/parser.c"

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

#include "parser.h"
/* Symbol kind.  */
enum yysymbol_kind_t
{
  YYSYMBOL_YYEMPTY = -2,
  YYSYMBOL_YYEOF = 0,                      /* "end of file"  */
  YYSYMBOL_YYerror = 1,                    /* error  */
  YYSYMBOL_YYUNDEF = 2,                    /* "invalid token"  */
  YYSYMBOL_PROGRAM = 3,                    /* PROGRAM  */
  YYSYMBOL_SEMI = 4,                       /* SEMI  */
  YYSYMBOL_VAR = 5,                        /* VAR  */
  YYSYMBOL_INT = 6,                        /* INT  */
  YYSYMBOL_BOOL = 7,                       /* BOOL  */
  YYSYMBOL_REAL = 8,                       /* REAL  */
  YYSYMBOL_STRING = 9,                     /* STRING  */
  YYSYMBOL_BBEGIN = 10,                    /* BBEGIN  */
  YYSYMBOL_READ = 11,                      /* READ  */
  YYSYMBOL_WRITE = 12,                     /* WRITE  */
  YYSYMBOL_IF = 13,                        /* IF  */
  YYSYMBOL_THEN = 14,                      /* THEN  */
  YYSYMBOL_ELSE = 15,                      /* ELSE  */
  YYSYMBOL_REPEAT = 16,                    /* REPEAT  */
  YYSYMBOL_UNTIL = 17,                     /* UNTIL  */
  YYSYMBOL_END = 18,                       /* END  */
  YYSYMBOL_LPAR = 19,                      /* LPAR  */
  YYSYMBOL_RPAR = 20,                      /* RPAR  */
  YYSYMBOL_FLOAT_VAL = 21,                 /* FLOAT_VAL  */
  YYSYMBOL_INT_VAL = 22,                   /* INT_VAL  */
  YYSYMBOL_STRING_VAL = 23,                /* STRING_VAL  */
  YYSYMBOL_TRUE = 24,                      /* TRUE  */
  YYSYMBOL_FALSE = 25,                     /* FALSE  */
  YYSYMBOL_ASSIGN = 26,                    /* ASSIGN  */
  YYSYMBOL_EQ = 27,                        /* EQ  */
  YYSYMBOL_LT = 28,                        /* LT  */
  YYSYMBOL_TIMES = 29,                     /* TIMES  */
  YYSYMBOL_PLUS = 30,                      /* PLUS  */
  YYSYMBOL_OVER = 31,                      /* OVER  */
  YYSYMBOL_MINUS = 32,                     /* MINUS  */
  YYSYMBOL_ID = 33,                        /* ID  */
  YYSYMBOL_YYACCEPT = 34,                  /* $accept  */
  YYSYMBOL_program = 35,                   /* program  */
  YYSYMBOL_vars_sect = 36,                 /* vars_sect  */
  YYSYMBOL_opt_var_decl = 37,              /* opt_var_decl  */
  YYSYMBOL_var_decl_list = 38,             /* var_decl_list  */
  YYSYMBOL_var_decl = 39,                  /* var_decl  */
  YYSYMBOL_type_spec = 40,                 /* type_spec  */
  YYSYMBOL_stmt_sect = 41,                 /* stmt_sect  */
  YYSYMBOL_stmt_list = 42,                 /* stmt_list  */
  YYSYMBOL_stmt = 43,                      /* stmt  */
  YYSYMBOL_if_stmt = 44,                   /* if_stmt  */
  YYSYMBOL_if_stmt_pt2 = 45,               /* if_stmt_pt2  */
  YYSYMBOL_repeat_stmt = 46,               /* repeat_stmt  */
  YYSYMBOL_assign_stmt = 47,               /* assign_stmt  */
  YYSYMBOL_read_stmt = 48,                 /* read_stmt  */
  YYSYMBOL_write_stmt = 49,                /* write_stmt  */
  YYSYMBOL_expr = 50                       /* expr  */
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
typedef yytype_int8 yy_state_t;

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
#define YYFINAL  4
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   140

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  34
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  17
/* YYNRULES -- Number of rules.  */
#define YYNRULES  41
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  75

/* YYMAXUTOK -- Last valid token kind.  */
#define YYMAXUTOK   288


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
      25,    26,    27,    28,    29,    30,    31,    32,    33
};

#if YYDEBUG
/* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
static const yytype_int16 yyrline[] =
{
       0,    73,    73,    87,    93,    94,   100,   109,   121,   129,
     130,   131,   132,   136,   137,   141,   155,   172,   173,   174,
     175,   176,   180,   188,   189,   193,   201,   224,   238,   268,
     269,   270,   271,   272,   273,   274,   275,   276,   277,   278,
     279,   280
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
  "\"end of file\"", "error", "\"invalid token\"", "PROGRAM", "SEMI",
  "VAR", "INT", "BOOL", "REAL", "STRING", "BBEGIN", "READ", "WRITE", "IF",
  "THEN", "ELSE", "REPEAT", "UNTIL", "END", "LPAR", "RPAR", "FLOAT_VAL",
  "INT_VAL", "STRING_VAL", "TRUE", "FALSE", "ASSIGN", "EQ", "LT", "TIMES",
  "PLUS", "OVER", "MINUS", "ID", "$accept", "program", "vars_sect",
  "opt_var_decl", "var_decl_list", "var_decl", "type_spec", "stmt_sect",
  "stmt_list", "stmt", "if_stmt", "if_stmt_pt2", "repeat_stmt",
  "assign_stmt", "read_stmt", "write_stmt", "expr", YY_NULLPTR
};

static const char *
yysymbol_name (yysymbol_kind_t yysymbol)
{
  return yytname[yysymbol];
}
#endif

#define YYPACT_NINF (-32)

#define yypact_value_is_default(Yyn) \
  ((Yyn) == YYPACT_NINF)

#define YYTABLE_NINF (-1)

#define yytable_value_is_error(Yyn) \
  0

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
static const yytype_int8 yypact[] =
{
      14,   -31,    32,    34,   -32,    35,     4,    36,   -32,   -32,
     -32,   -32,   -32,     4,   -32,     9,     3,   -32,   -32,    40,
      12,   107,   107,    86,   -32,    29,    60,   -32,   -32,   -32,
     -32,   -32,   -32,   -32,    62,   107,   -32,   -32,   -32,   -32,
     -32,   -32,    -1,    93,    70,   107,   -32,   -32,   -32,    85,
     -32,   107,   107,   107,   107,   107,   107,    86,   107,    30,
     -32,    -7,    -7,   -32,     8,   -32,     8,    52,   -23,   -32,
      86,   -32,   -32,    78,   -32
};

/* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
   Performed when YYTABLE does not specify something else to do.  Zero
   means the default is an error.  */
static const yytype_int8 yydefact[] =
{
       0,     0,     0,     0,     1,     0,     4,     0,    10,     9,
      11,    12,     3,     5,     7,     0,     0,     2,     6,     0,
       0,     0,     0,     0,    14,     0,     0,    16,    17,    18,
      19,    20,    21,     8,     0,     0,    39,    38,    40,    36,
      37,    41,     0,     0,     0,     0,    13,    15,    27,     0,
      28,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      35,    30,    29,    33,    31,    34,    32,     0,    25,    26,
       0,    23,    22,     0,    24
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int8 yypgoto[] =
{
     -32,   -32,   -32,   -32,   -32,    56,   -32,   -32,   -22,   -26,
     -32,   -32,   -32,   -32,   -32,   -32,    -2
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int8 yydefgoto[] =
{
       0,     2,     7,    12,    13,    14,    15,    17,    26,    27,
      28,    72,    29,    30,    31,    32,    42
};

/* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule whose
   number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int8 yytable[] =
{
      47,    44,     3,    50,    51,    52,    53,    54,    55,    56,
       8,     9,    10,    11,    20,    21,    22,     1,    47,    23,
      43,    24,    53,    54,    55,    56,    51,    52,    53,    54,
      55,    56,     4,    49,    69,    67,    25,    53,     5,    55,
       6,    47,    19,    59,    33,    34,    16,    47,    73,    61,
      62,    63,    64,    65,    66,    45,    68,    51,    52,    53,
      54,    55,    56,    20,    21,    22,    48,    70,    23,    18,
      71,    20,    21,    22,     0,     0,    23,     0,    46,     0,
       0,    20,    21,    22,     0,    25,    23,    58,     0,    20,
      21,    22,     0,    25,    23,     0,    74,    20,    21,    22,
       0,     0,    23,    25,     0,    60,     0,    57,     0,     0,
       0,    25,    51,    52,    53,    54,    55,    56,     0,    25,
      51,    52,    53,    54,    55,    56,    35,     0,    36,    37,
      38,    39,    40,     0,     0,     0,     0,     0,     0,     0,
      41
};

static const yytype_int8 yycheck[] =
{
      26,    23,    33,     4,    27,    28,    29,    30,    31,    32,
       6,     7,     8,     9,    11,    12,    13,     3,    44,    16,
      22,    18,    29,    30,    31,    32,    27,    28,    29,    30,
      31,    32,     0,    35,     4,    57,    33,    29,     4,    31,
       5,    67,    33,    45,     4,    33,    10,    73,    70,    51,
      52,    53,    54,    55,    56,    26,    58,    27,    28,    29,
      30,    31,    32,    11,    12,    13,     4,    15,    16,    13,
      18,    11,    12,    13,    -1,    -1,    16,    -1,    18,    -1,
      -1,    11,    12,    13,    -1,    33,    16,    17,    -1,    11,
      12,    13,    -1,    33,    16,    -1,    18,    11,    12,    13,
      -1,    -1,    16,    33,    -1,    20,    -1,    14,    -1,    -1,
      -1,    33,    27,    28,    29,    30,    31,    32,    -1,    33,
      27,    28,    29,    30,    31,    32,    19,    -1,    21,    22,
      23,    24,    25,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      33
};

/* YYSTOS[STATE-NUM] -- The symbol kind of the accessing symbol of
   state STATE-NUM.  */
static const yytype_int8 yystos[] =
{
       0,     3,    35,    33,     0,     4,     5,    36,     6,     7,
       8,     9,    37,    38,    39,    40,    10,    41,    39,    33,
      11,    12,    13,    16,    18,    33,    42,    43,    44,    46,
      47,    48,    49,     4,    33,    19,    21,    22,    23,    24,
      25,    33,    50,    50,    42,    26,    18,    43,     4,    50,
       4,    27,    28,    29,    30,    31,    32,    14,    17,    50,
      20,    50,    50,    50,    50,    50,    50,    42,    50,     4,
      15,    18,    45,    42,    18
};

/* YYR1[RULE-NUM] -- Symbol kind of the left-hand side of rule RULE-NUM.  */
static const yytype_int8 yyr1[] =
{
       0,    34,    35,    36,    37,    37,    38,    38,    39,    40,
      40,    40,    40,    41,    41,    42,    42,    43,    43,    43,
      43,    43,    44,    45,    45,    46,    47,    48,    49,    50,
      50,    50,    50,    50,    50,    50,    50,    50,    50,    50,
      50,    50
};

/* YYR2[RULE-NUM] -- Number of symbols on the right-hand side of rule RULE-NUM.  */
static const yytype_int8 yyr2[] =
{
       0,     2,     5,     2,     0,     1,     2,     1,     3,     1,
       1,     1,     1,     3,     2,     2,     1,     1,     1,     1,
       1,     1,     5,     1,     3,     4,     4,     3,     3,     3,
       3,     3,     3,     3,     3,     3,     1,     1,     1,     1,
       1,     1
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
  case 2: /* program: PROGRAM ID SEMI vars_sect stmt_sect  */
#line 73 "parser/parser.y"
                                             {
		NodeData data;
		data.nil = NULL;
		root = ast_new_node(PROGRAM_NODE, data);
		if((yyvsp[-1].ast) != NULL){
			ast_add_child(root, (yyvsp[-1].ast));
		}
		if((yyvsp[0].ast) != NULL){
			ast_add_child(root, (yyvsp[0].ast));			
		}
	}
#line 1726 "src/parser.c"
    break;

  case 3: /* vars_sect: VAR opt_var_decl  */
#line 87 "parser/parser.y"
                         {
		(yyval.ast) = (yyvsp[0].ast);
	}
#line 1734 "src/parser.c"
    break;

  case 4: /* opt_var_decl: %empty  */
#line 93 "parser/parser.y"
                       { (yyval.ast) = NULL; }
#line 1740 "src/parser.c"
    break;

  case 5: /* opt_var_decl: var_decl_list  */
#line 94 "parser/parser.y"
                       {
		(yyval.ast) = (yyvsp[0].ast);
	}
#line 1748 "src/parser.c"
    break;

  case 6: /* var_decl_list: var_decl_list var_decl  */
#line 100 "parser/parser.y"
                               {
		if(decl_list_created == 0){
			NodeData data;
			data.nil = NULL;
			(yyval.ast) = ast_new_node(VAR_LIST_NODE, data);
			decl_list_created = 1;
		}
		ast_add_child((yyval.ast), (yyvsp[0].ast));
	}
#line 1762 "src/parser.c"
    break;

  case 7: /* var_decl_list: var_decl  */
#line 109 "parser/parser.y"
                 { 
		if(decl_list_created == 0){
			NodeData data;
			data.nil = NULL;
			(yyval.ast) = ast_new_node(VAR_LIST_NODE, data);
			decl_list_created = 1;
		}
		ast_add_child((yyval.ast), (yyvsp[0].ast));
	}
#line 1776 "src/parser.c"
    break;

  case 8: /* var_decl: type_spec ID SEMI  */
#line 121 "parser/parser.y"
                          {
		add_var_declaration((yyvsp[-1].str), TYPE_DECL);
		// NodeData data;
		load_var_to_token((yyvsp[-1].str), &(yyval.ast));
	}
#line 1786 "src/parser.c"
    break;

  case 9: /* type_spec: BOOL  */
#line 129 "parser/parser.y"
               { TYPE_DECL = TYPE_BOO; }
#line 1792 "src/parser.c"
    break;

  case 10: /* type_spec: INT  */
#line 130 "parser/parser.y"
               { TYPE_DECL = TYPE_INT; }
#line 1798 "src/parser.c"
    break;

  case 11: /* type_spec: REAL  */
#line 131 "parser/parser.y"
               { TYPE_DECL = TYPE_FLT; }
#line 1804 "src/parser.c"
    break;

  case 12: /* type_spec: STRING  */
#line 132 "parser/parser.y"
               { TYPE_DECL = TYPE_STR; }
#line 1810 "src/parser.c"
    break;

  case 13: /* stmt_sect: BBEGIN stmt_list END  */
#line 136 "parser/parser.y"
                             { (yyval.ast) = (yyvsp[-1].ast); }
#line 1816 "src/parser.c"
    break;

  case 14: /* stmt_sect: BBEGIN END  */
#line 137 "parser/parser.y"
                             { (yyval.ast) = NULL; }
#line 1822 "src/parser.c"
    break;

  case 15: /* stmt_list: stmt_list stmt  */
#line 141 "parser/parser.y"
                        {
		if(stmt_list_created == 0){
			NodeData data;
			data.nil = NULL;
			(yyval.ast) = ast_new_node(BLOCK_NODE, data);
			stmt_list_created = 1;
		}
		if((yyvsp[0].ast) != NULL){
			ast_add_child((yyval.ast), (yyvsp[0].ast));
		}
		else{
			printf("Can't add a child to stmt list (2)\n");
		}
	}
#line 1841 "src/parser.c"
    break;

  case 16: /* stmt_list: stmt  */
#line 155 "parser/parser.y"
             {
		if(stmt_list_created == 0){
			NodeData data;
			data.nil = NULL;
			(yyval.ast) = ast_new_node(BLOCK_NODE, data);
			stmt_list_created = 1;
		}
		if((yyvsp[0].ast) != NULL){
			ast_add_child((yyval.ast), (yyvsp[0].ast));
		}
		else{
			printf("Can't add a child to stmt list (1)\n");
		}
	}
#line 1860 "src/parser.c"
    break;

  case 17: /* stmt: if_stmt  */
#line 172 "parser/parser.y"
                    { (yyval.ast) = (yyvsp[0].ast); }
#line 1866 "src/parser.c"
    break;

  case 18: /* stmt: repeat_stmt  */
#line 173 "parser/parser.y"
                    { (yyval.ast) = (yyvsp[0].ast); }
#line 1872 "src/parser.c"
    break;

  case 19: /* stmt: assign_stmt  */
#line 174 "parser/parser.y"
                    { (yyval.ast) = (yyvsp[0].ast); }
#line 1878 "src/parser.c"
    break;

  case 20: /* stmt: read_stmt  */
#line 175 "parser/parser.y"
                    { (yyval.ast) = (yyvsp[0].ast); }
#line 1884 "src/parser.c"
    break;

  case 21: /* stmt: write_stmt  */
#line 176 "parser/parser.y"
                    { (yyval.ast) = (yyvsp[0].ast); }
#line 1890 "src/parser.c"
    break;

  case 22: /* if_stmt: IF expr THEN stmt_list if_stmt_pt2  */
#line 180 "parser/parser.y"
                                           {
		/* check_if_repeat_expr($2.type); */
		fprintf(stdout, "IF NODE INCOMPLETE\n");
		(yyval.ast) = (yyvsp[-3].ast);
	}
#line 1900 "src/parser.c"
    break;

  case 25: /* repeat_stmt: REPEAT stmt_list UNTIL expr  */
#line 193 "parser/parser.y"
                                    {
		/* check_if_repeat_expr($4.type);
		fprintf(stdout, "REPEAT NODE INCOMPLETE\n");
		$$ = $4;
	 */ }
#line 1910 "src/parser.c"
    break;

  case 26: /* assign_stmt: ID ASSIGN expr SEMI  */
#line 201 "parser/parser.y"
                            { 
	    Literal* lit = ast_get_literal((yyvsp[-1].ast));
	    if(lit != NULL){
		    save_token_to_var(lit, (yyvsp[-3].str));

			NodeData data;
			data.lit = *lit;
			(yyval.ast) = ast_new_node(ASSIGN_NODE, data);

			Variable var;
			parser_get_var((yyvsp[-3].str), &var);
			data.var = var;
			AST* ass = ast_new_node(VAR_USE_NODE, data);
			ast_add_child((yyval.ast), ass);
			ast_add_child((yyval.ast), (yyvsp[-1].ast));
	    }
		else{
			(yyval.ast) = NULL;
		}
	}
#line 1935 "src/parser.c"
    break;

  case 27: /* read_stmt: READ ID SEMI  */
#line 224 "parser/parser.y"
                     {
		Variable var;
		parser_get_var((yyvsp[-1].str), &var);
		NodeData data;
		data.lit = var.literal;
		(yyval.ast) = ast_new_node(READ_NODE, data);

		AST* varread = NULL;
		load_var_to_token((yyvsp[-1].str), &varread);
		ast_add_child((yyval.ast), varread);
	}
#line 1951 "src/parser.c"
    break;

  case 28: /* write_stmt: WRITE expr SEMI  */
#line 238 "parser/parser.y"
                        { 
	    Literal* lit = ast_get_literal((yyvsp[-1].ast));
		if(lit != NULL){
			NodeData data;
			data.lit = *lit;
			(yyval.ast) = ast_new_node(WRITE_NODE, data);
			ast_add_child((yyval.ast), (yyvsp[-1].ast));
		}
		else{
			fprintf(stdout, "Error write!\n");
		}
		// switch(ast_get_type($2)){
		// 	case TYPE_INT:
		// 		printf("write writes %d\n", $2.value.i);
		// 		break;
		// 	case TYPE_FLT:
		// 		printf("write writes %f\n", $2.value.f);
		// 		break;
		// 	case TYPE_BOO:
		// 		printf("write writes %d\n", $2.value.b);
		// 		break;
		// 	case TYPE_STR:
		// 		printf("write writes %s\n", $2.value.s);
		// 		break;
		// }
	}
#line 1982 "src/parser.c"
    break;

  case 29: /* expr: expr LT expr  */
#line 268 "parser/parser.y"
                         { binary_operation((yyvsp[-2].ast), (yyvsp[0].ast), &(yyval.ast), literal_lt, (yyvsp[-1].str), LT_NODE); }
#line 1988 "src/parser.c"
    break;

  case 30: /* expr: expr EQ expr  */
#line 269 "parser/parser.y"
                         { binary_operation((yyvsp[-2].ast), (yyvsp[0].ast), &(yyval.ast), literal_eq, (yyvsp[-1].str), EQ_NODE); }
#line 1994 "src/parser.c"
    break;

  case 31: /* expr: expr PLUS expr  */
#line 270 "parser/parser.y"
                         { binary_operation((yyvsp[-2].ast), (yyvsp[0].ast), &(yyval.ast), literal_sum, (yyvsp[-1].str), PLUS_NODE); }
#line 2000 "src/parser.c"
    break;

  case 32: /* expr: expr MINUS expr  */
#line 271 "parser/parser.y"
                         { binary_operation((yyvsp[-2].ast), (yyvsp[0].ast), &(yyval.ast), literal_minus, (yyvsp[-1].str), MINUS_NODE); }
#line 2006 "src/parser.c"
    break;

  case 33: /* expr: expr TIMES expr  */
#line 272 "parser/parser.y"
                         { binary_operation((yyvsp[-2].ast), (yyvsp[0].ast), &(yyval.ast), literal_mult, (yyvsp[-1].str), TIMES_NODE); }
#line 2012 "src/parser.c"
    break;

  case 34: /* expr: expr OVER expr  */
#line 273 "parser/parser.y"
                         { binary_operation((yyvsp[-2].ast), (yyvsp[0].ast), &(yyval.ast), literal_div, (yyvsp[-1].str), OVER_NODE); }
#line 2018 "src/parser.c"
    break;

  case 35: /* expr: LPAR expr RPAR  */
#line 274 "parser/parser.y"
                         { (yyval.ast) = (yyvsp[-1].ast); }
#line 2024 "src/parser.c"
    break;

  case 36: /* expr: TRUE  */
#line 275 "parser/parser.y"
                         { NodeData data; data.lit = (yyvsp[0].l); (yyval.ast) = ast_new_node(BOOL_VAL_NODE, data); }
#line 2030 "src/parser.c"
    break;

  case 37: /* expr: FALSE  */
#line 276 "parser/parser.y"
                         { NodeData data; data.lit = (yyvsp[0].l); (yyval.ast) = ast_new_node(BOOL_VAL_NODE, data); }
#line 2036 "src/parser.c"
    break;

  case 38: /* expr: INT_VAL  */
#line 277 "parser/parser.y"
                         { NodeData data; data.lit = (yyvsp[0].l); (yyval.ast) = ast_new_node(INT_VAL_NODE, data); }
#line 2042 "src/parser.c"
    break;

  case 39: /* expr: FLOAT_VAL  */
#line 278 "parser/parser.y"
                         { NodeData data; data.lit = (yyvsp[0].l); (yyval.ast) = ast_new_node(REAL_VAL_NODE, data); }
#line 2048 "src/parser.c"
    break;

  case 40: /* expr: STRING_VAL  */
#line 279 "parser/parser.y"
                         { NodeData data; data.lit = (yyvsp[0].l); (yyval.ast) = ast_new_node(STR_VAL_NODE, data); }
#line 2054 "src/parser.c"
    break;

  case 41: /* expr: ID  */
#line 280 "parser/parser.y"
                         { load_var_to_token((yyvsp[0].str), &(yyval.ast)); }
#line 2060 "src/parser.c"
    break;


#line 2064 "src/parser.c"

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

#line 282 "parser/parser.y"

#include <stdio.h>
#include "scanner.h"

void lexer_init(void);
void lexer_deinit(void);

int main(void) {
	lexer_init();
	parser_init();

	if(yyparse() == 0){
		printf("PARSE SUCCESSFUL!\n");
		Scope* scope = scope_manager_search_by_name(scope_manager, "xxxx");
		VarTable* vartable = scope_get_vartable(scope);
		print_dot(root, vartable);
	}
	else{
		printf("PARSE FAILED!\n");
	}

	parser_deinit();
	lexer_deinit();
	yylex_destroy();

	return 0;
}

void yyerror(char const *s) {
	fprintf(stderr, "%s\n", s);
}

void parser_init(void){
	scope_manager = scope_manager_new();
}

void parser_deinit(void){
	scope_manager_destroy(&scope_manager);
	ast_free(root);
	root = NULL;
}

void enter_scope(void){
	scope_manager_enter(scope_manager);
}

void exit_scope(void){
	scope_manager_exit(scope_manager);
}

void add_var_declaration(char* name, enum Type type){
	Scope* scope = scope_manager_get_current_scope(scope_manager);
	if(scope_add(scope, name, yylineno, type) == -1){
		VarTable* vartable = scope_get_vartable(scope);
		int idx = scope_search_by_name(scope, name);
		printf("SEMANTIC ERROR (%d): variable %s has been declared already at line %d\n", yylineno, name, get_line(vartable, idx));
	}
}

int check_var_declaration(char* name){
	if(scope_manager_search_by_name(scope_manager, name) == NULL){
		printf("SEMANTIC ERROR (%d): variable %s hasn't been declared yet\n", yylineno, name);
		return -1;
	}
	return 0;
}

int check_assignment(char* name, enum Type type){
	if(check_var_declaration(name) == -1){
		return -1;
	}

	Scope* scope = scope_manager_search_by_name(scope_manager, name);
	VarTable* table = scope_get_vartable(scope);
	int idx = lookup_var(table, name, scope_get_level(scope));
	if(idx == -1){
		fprintf(stderr, "check_assignment: idx error\n");
		return -1;
	}

	/*
	enum Type vartype = get_type(table, idx);
	fprintf(
		stderr, "%s == %s?\n",
		type_name(vartype),
		type_name(type)
	);
	if(vartype == type){
		fprintf(stderr, "%s matches the assignment type!\n", name);
	}
	else{
		fprintf(stderr, "%s DOESN'T match the assignment type!\n", name);
	}
	*/

	return 0;
}

int check_if_repeat_expr(enum Type type){
	int ret = type != TYPE_BOO;
	if(ret != 0){
		fprintf(stderr, "SEMANTIC ERROR (%d): conditional expression in IF/REPEAT", yylineno);
		fprintf(stderr, " is '%s' instead of 'bool'\n", type_name(type));
	}
	return ret;
}

void unary_operation(
	Literal* op1,
	Literal* res,
	int (*operation)(void*, void*),
	const char* operator)
{
	int fail = operation(op1, res);
	if(fail){
		fprintf(stderr, "SEMANTIC ERROR (%d):", yylineno);
		fprintf(stderr, "incompatible types for operator '%s': ", operator);
		fprintf(stderr, "LHS is '%s'", type_name(res->type));
		fprintf(stderr, " and RHS is '%s'\n", type_name(op1->type));
	}
}

void binary_operation(
	AST* op1,
	AST* op2,
	AST** res,
	int (*operation)(Literal*, Literal*, BinaryOperationData*),
	const char* operator_symb,
	NodeKind nodekind)
{
	Literal* l1 = ast_get_literal(op1);
	Literal* l2 = ast_get_literal(op2);
	if(l1 == NULL || l2 == NULL || *res == NULL){
		*res = NULL;
		fprintf(stdout, "Got nil when performing operation %s\n", operator_symb);
		return ;
	}

	BinaryOperationData meta;
	int fail = operation(l1, l2, &meta);
	if(fail){
		fprintf(stderr, "SEMANTIC ERROR (%d):", yylineno);
		fprintf(stderr, "incompatible types for operator '%s': ", operator_symb);
		fprintf(stderr, "LHS is '%s'", type_name(ast_get_type(op1)));
		fprintf(stderr, " and RHS is '%s'\n", type_name(ast_get_type(op2)));
	}

	AST* leftnode = op1;
	AST* rightnode = op2;
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

	NodeData resdata;
	resdata.lit.type = meta.type;
	*res = ast_new_subtree(nodekind, resdata, leftnode, rightnode, NULL);
}

void save_token_to_var(Literal* props, char* varname){
	if(check_var_declaration(varname) == -1){
		return ;
	}
	if(check_assignment(varname, props->type) == -1){
		return ;
	}

	/*
	switch($3.type){
		case TYPE_INT:
			printf("Assignment: var '%s' = %d\n", varname, props->value.i);
			break;
		case TYPE_FLT:
			printf("Assignment: var '%s' = %f\n", varname, props->value.f);
			break;
		case TYPE_BOO:
			printf("Assignment: var '%s' = %d\n", varname, props->value.b);
			break;
		case TYPE_STR:
			printf("Assignment: var '%s' = %s\n", varname, props->value.s);
			break;
	}
	*/

	Scope* scope = scope_manager_search_by_name(scope_manager, varname);
	VarTable* table = scope_get_vartable(scope);
	int idx = lookup_var(table, varname, scope_get_level(scope));
	set_val(table, idx, props->value);
}

void load_var_to_token(char* varname, AST** res){
	if(check_var_declaration(varname) == -1){
		return ;
	}
	Scope* scope = scope_manager_search_by_name(scope_manager, varname);
	VarTable* table = scope_get_vartable(scope);
	int idx = lookup_var(table, varname, scope_get_level(scope));
	Variable var = get_var(table, idx);
	NodeData data = {.var = var};
	*res = ast_new_node(VAR_USE_NODE, data);
}

void parser_get_var(char* varname, Variable* res){
	if(check_var_declaration(varname) == -1){
		return ;
	}
	Scope* scope = scope_manager_search_by_name(scope_manager, varname);
	VarTable* table = scope_get_vartable(scope);
	int idx = lookup_var(table, varname, scope_get_level(scope));
	Variable var = get_var(table, idx);
	*res = var;
}
