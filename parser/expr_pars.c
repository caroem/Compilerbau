/* $Id: Parser.c,v 1.54 2010/07/07 09:53:21 grosch Exp $ */

# include "expr_pars.h"
# include <stdio.h>
# include <stddef.h>

# ifndef EXTERN_C_BEGIN
#  define EXTERN_C_BEGIN
#  define EXTERN_C_END
# endif

EXTERN_C_BEGIN
# include "rString.h"
# include "rSystem.h"
# include "rMemory.h"
# include "General.h"
# include "DynArray.h"
# ifndef NO_RECOVER
#  include "Sets.h"
# endif
# if ! defined NO_RECOVER | defined YYReParse
#  include "Errors.h"
# endif
# if defined YYDEBUG | defined lex_interface
#  include "Position.h"
# endif
EXTERN_C_END

# ifdef lex_interface
#  define expr_scan_GetToken	yylex
     extern int yylex ARGS ((void));
#  ifndef AttributeDef
#   include "Position.h"
	   typedef struct { tPosition Position; } expr_scan_tScanAttribute;
	   expr_scan_tScanAttribute	expr_scan_Attribute = {{ 0, 0 }};
#  endif
#  ifndef ErrorAttributeDef
#   define expr_scan_ErrorAttribute(Token, RepairAttribute)
#  endif
#  ifndef yyGetAttribute
#   define yyGetAttribute(yyAttrStackPtr, a) * yyAttrStackPtr = yylval
#  endif
# else
#  include "expr_scan.h"
#  ifndef yyGetAttribute
#   define yyGetAttribute(yyAttrStackPtr, a) (yyAttrStackPtr)->Scan = a
#  endif
# endif

static void yyExit ARGS ((void)) { rExit (1); }

	void		(* expr_pars_Exit) ARGS ((void)) = yyExit;

static void yyCheckMemory
# ifdef HAVE_ARGS
   (char * ptr)
# else
   (ptr) char * ptr;
# endif
{
  if (! (ptr)) expr_pars_Exit ();
}

# define yyMakeArray(ptr, count, size)	 { MakeArray (ptr, count, size); \
					   yyCheckMemory (* ptr); }
# define yyExtendArray(ptr, count, size) { ExtendArray (ptr, count, size); \
					   yyCheckMemory (* ptr); }

typedef unsigned short	yyStateRange	;
typedef unsigned short	yySymbolRange	;
typedef struct { yyStateRange Check, Next; } yytComb;
typedef enum {
yyNT0_intern	= 28,
yyNTroot	= 29,
yyNTx	= 30,
yyNTname	= 31,
yyNTvar	= 32,
yyNTtypdef	= 33,
yyNTexec	= 34,
yyNTbody	= 35,
yyNTstatement	= 36,
yyNTvarallocation	= 37,
yyNTifstmt	= 38,
yyNTwhilestmt	= 39,
yyNTforstmt	= 40,
yyNTcondition	= 41,
yyNTexpr	= 42,
yyNTexprstring	= 43
} yytNonterminal;
typedef struct { short yyMode; rbool yyActions, yyMessages; } yytControl;

static	yytControl	yyControl	= { 0, rtrue, rtrue };
	rbool		expr_pars_Debug	= rfalse;

# define yyFirstTerminal	0
# define yyLastTerminal	27
# define yySetSize	28
# define yyFirstSymbol	0
# define yyLastSymbol	43
# define yyTTableMax	102
# define yyNTableMax	77
# define yyStartState	1
# define yyFirstReadState	1
# define yyLastReadState	52
# define yyFirstReadReduceState	53
# define yyLastReadReduceState	66
# define yyFirstReduceState	67
# define yyLastReduceState	104
# define yyLastState	104
# define yyLastStopState	67
# define YYTDefault
# define YYNDefault

# define yyFirstFinalState	yyFirstReadReduceState

# define ErrorMessages(Messages) yyControl.yyMessages = Messages
# define SemActions(Actions)	 yyControl.yyActions = Actions

# ifdef YYCSTree
#  include "cs_tree.h"
#  define CommonParsAttrs tCSTree Tree;
# endif

# ifdef YYGetLook

#  define GetLookahead(k)	yyGetLookahead ((k) - 1, yyTerminal)
#  define GetAttribute(k, a)	xxGetAttribute ((k) - 1, yyTerminal, a)

static int yyGetLookahead	ARGS ((int yyk, yySymbolRange yyToken));
static void xxGetAttribute	ARGS ((int yyk, yySymbolRange yyToken,
				expr_scan_tScanAttribute * yyAttribute));

# endif

/* line 8 "expr_pars.lrk" */


/* line 12 "expr.pars" */

 # include <stdio.h>


/* line 16 "expr_pars.lrk" */
# ifdef CommonParsAttrs
typedef struct { CommonParsAttrs } yytCommonParsAttrs;
#  define yyCommon yytCommonParsAttrs Common;
#  define yyCommonParsAttrs CommonParsAttrs
# else
#  define yyCommon
#  define yyCommonParsAttrs
# endif


typedef union {
expr_scan_tScanAttribute Scan;
yyCommon
} tParsAttribute;

/* line 160 "expr_pars.c" */

# if defined lex_interface & ! defined yylvalDef
     tParsAttribute yylval;
# endif

# ifndef yyInitStackSize
#  define yyInitStackSize	100
# endif
# define yyNoState		0
# define yystandard		1
# define yytrial		2
# define yybuffer		4
# define yyreparse		8
# define yyS			yySynAttribute
# define yyA			yyAttrStackPtr
# define YYACCEPT		goto yyAccept
# define YYABORT		goto yyAbort

# ifdef YYDEC_TABLE
#  define yyDecrement(x)
#  define yySetNT(x)
# else
#  define yyDecrement(x)	yyStateStackPtr -= x; yyAttrStackPtr -= x;
#  define yySetNT(x)		yyNonterminal = x;
# endif

# ifdef YYNDefault
#  define yytNComb yytComb
# else
#  define yytNComb yyStateRange
# endif

# if defined YYDEBUG | defined YYDCRP
static	long		yyCount		= 0;
static	FILE *		yyTrace		;

static	void		yyPrintState	ARGS ((yyStateRange yyState));
static	void		yyNl		ARGS ((void));

static	char *		yyRule		[] = { 0,
"0_intern : root _EOF_ ",
"root : x ",
"x : '#' name ';' var exec ",
"name : identifier ",
"var : typdef name ';' ",
"var : var var ",
"typdef : 'int' ",
"typdef : 'float' ",
"typdef : 'String' ",
"exec : 'BEGIN' body 'END' ';' ",
"exec : ",
"body : varallocation ",
"body : statement ",
"body : body body ",
"statement : ifstmt ",
"statement : whilestmt ",
"statement : forstmt ",
"varallocation : name '=' expr ';' ",
"varallocation : name '=' exprstring ",
"ifstmt : 'if' condition 'then' body ",
"ifstmt : 'if' condition 'then' body 'else' body ",
"whilestmt : 'while' '(' expr ')' body ",
"forstmt : 'for' '(' int_const ')' body ",
"condition : '(' expr ')' ",
"expr : expr '+' expr ",
"expr : expr '-' expr ",
"expr : expr '*' expr ",
"expr : expr '/' expr ",
"expr : expr '<' expr ",
"expr : expr '>' expr ",
"expr : expr '==' expr ",
"expr : '(' expr ')' ",
"expr : int_const ",
"expr : float_const ",
"expr : identifier ",
"exprstring : identfier ",
"exprstring : string_const ",
"exprstring : exprstring '+' exprstring ",
""
};
# endif
	char *		expr_pars_TokenName	[yyLastTerminal + 2] = {
"_EOF_",
"int_const",
"float_const",
"string_const",
"identifier",
"#",
";",
"int",
"float",
"String",
"BEGIN",
"=",
"+",
"-",
"*",
"/",
"<",
">",
"==",
"(",
")",
"identfier",
"if",
"then",
"else",
"while",
"for",
"END",
""
};
static	yytComb		yyTComb		[yyTTableMax + 1] = {
{   9,   72}, {   6,   77}, {   3,   67}, {  19,   27}, {   2,   53}, 
{   7,   53}, {  28,   59}, {   5,   55}, {   5,   56}, {   5,   57}, 
{   9,   72}, {   6,    8}, {  28,   31}, {  28,   32}, {  28,   33}, 
{  28,   34}, {  28,   35}, {  28,   36}, {  28,   37}, {  26,   31}, 
{  26,   32}, {  26,   33}, {  26,   34}, {  26,   35}, {  26,   36}, 
{  26,   37}, {  29,   85}, {  26,   39}, {  49,   11}, {  45,   94}, 
{  22,   80}, {  49,   12}, {  49,   13}, {  22,   80}, {  29,   41}, 
{  45,   94}, {  45,   94}, {  45,   94}, {  45,   94}, {  34,   62}, 
{  34,   63}, {   1,    2}, {  34,   64}, {  45,   94}, {  29,   85}, 
{  44,   93}, {  29,   85}, {  29,   85}, {  29,   85}, {  29,   85}, 
{  41,   66}, {  44,   93}, {  44,   93}, {  44,   93}, {  44,   93}, 
{  42,   91}, {  46,   95}, {  34,   23}, {  43,   92}, {  44,   93}, 
{  48,   97}, {  42,   91}, {  42,   91}, {  47,   96}, {  43,   92}, 
{  43,   92}, {  20,   66}, {  10,   54}, {  41,   65}, {  42,   91}, 
{  46,   95}, {   4,    5}, {  43,   92}, {  52,   87}, {  48,   97}, 
{  50,   88}, {  52,   87}, {  47,   96}, {  50,   88}, {  38,   49}, 
{  51,   89}, {  21,   58}, {  38,   86}, {  51,   89}, {  20,   65}, 
{  14,   20}, {  13,   19}, {  11,   16}, {  12,   18}, {  24,   60}, 
{  27,   40}, {  30,   61}, {  17,   25}, {  15,   21}, {   0,    0}, 
{   0,    0}, {   0,    0}, {   0,    0}, {   0,    0}, {   0,    0}, 
{   0,    0}, {   0,    0}, {   0,    0}, 
};
static	yytNComb	yyNComb		[yyNTableMax - yyLastTerminal] = {
{   0,    0}, {   0,    0}, {   0,    0}, {  52,   14}, {   9,    9}, 
{   9,    7}, {   0,    0}, {  52,   22}, {  52,   79}, {  52,   78}, 
{  52,   81}, {  52,   82}, {  52,   83}, {   1,    3}, {   1,   68}, 
{  20,   28}, {  20,   29}, {  39,   50}, {  40,   51}, {  25,   38}, 
{   8,   15}, {  11,   17}, {  49,   52}, {  35,   46}, {   6,   69}, 
{  23,   30}, {   5,    6}, {  18,   26}, {   7,   10}, {  41,  104}, 
{  34,   45}, {  16,   24}, {  36,   47}, {  37,   48}, {  33,   44}, 
{  32,   43}, {  31,   42}, {   2,    4}, {   0,    0}, {   0,    0}, 
{   0,    0}, {   0,    0}, {   0,    0}, {   0,    0}, {   0,    0}, 
{   0,    0}, {   0,    0}, {   0,    0}, {   0,    0}, {   0,    0}, 
};
static	yytComb *	yyTBasePtr	[yyLastReadState + 1] = { 0,
& yyTComb [  36], & yyTComb [   0], & yyTComb [   2], & yyTComb [  65], 
& yyTComb [   0], & yyTComb [   1], & yyTComb [   1], & yyTComb [   0], 
& yyTComb [   0], & yyTComb [  61], & yyTComb [  68], & yyTComb [  69], 
& yyTComb [  67], & yyTComb [  74], & yyTComb [  66], & yyTComb [   0], 
& yyTComb [  69], & yyTComb [   0], & yyTComb [   2], & yyTComb [  63], 
& yyTComb [  75], & yyTComb [   6], & yyTComb [   0], & yyTComb [  69], 
& yyTComb [   0], & yyTComb [   7], & yyTComb [  70], & yyTComb [   0], 
& yyTComb [  22], & yyTComb [  71], & yyTComb [   0], & yyTComb [   0], 
& yyTComb [   0], & yyTComb [  38], & yyTComb [   0], & yyTComb [   0], 
& yyTComb [   0], & yyTComb [  55], & yyTComb [   0], & yyTComb [   0], 
& yyTComb [  47], & yyTComb [  49], & yyTComb [  52], & yyTComb [  39], 
& yyTComb [  23], & yyTComb [  50], & yyTComb [  57], & yyTComb [  54], 
& yyTComb [   6], & yyTComb [  51], & yyTComb [  56], & yyTComb [  49], 
};
static	yytNComb *	yyNBasePtr	[yyLastReadState + 1] = { 0,
& yyNComb [ -16], & yyNComb [   6], & yyNComb [ -28], & yyNComb [ -28], 
& yyNComb [  -6], & yyNComb [ -10], & yyNComb [  -3], & yyNComb [ -15], 
& yyNComb [ -28], & yyNComb [ -28], & yyNComb [ -20], & yyNComb [ -28], 
& yyNComb [ -28], & yyNComb [ -28], & yyNComb [ -28], & yyNComb [ -11], 
& yyNComb [ -28], & yyNComb [ -15], & yyNComb [ -28], & yyNComb [ -27], 
& yyNComb [ -28], & yyNComb [ -28], & yyNComb [ -17], & yyNComb [ -28], 
& yyNComb [ -16], & yyNComb [ -28], & yyNComb [ -28], & yyNComb [ -28], 
& yyNComb [ -28], & yyNComb [ -28], & yyNComb [  -6], & yyNComb [  -7], 
& yyNComb [  -8], & yyNComb [ -12], & yyNComb [ -19], & yyNComb [ -10], 
& yyNComb [  -9], & yyNComb [ -28], & yyNComb [ -18], & yyNComb [ -17], 
& yyNComb [ -14], & yyNComb [ -28], & yyNComb [ -28], & yyNComb [ -28], 
& yyNComb [ -28], & yyNComb [ -28], & yyNComb [ -28], & yyNComb [ -28], 
& yyNComb [ -13], & yyNComb [ -28], & yyNComb [ -28], & yyNComb [ -28], 
};
# ifdef YYTDefault
static	unsigned short	yyTDefault	[yyLastReadState + 1] = { 0,
    0,     0,     0,     0,     0,     9,     0,    49,     5,     0, 
    0,     0,     0,     0,    49,    34,     0,    34,     0,    34, 
    0,    51,    34,    26,    49,     0,     0,     0,     0,    26, 
   34,    34,    34,     0,    34,    34,    34,    51,    49,    49, 
    0,    48,    48,    48,    44,    48,    48,    26,     7,    51, 
   15,    51, 
};
# endif
# ifdef YYNDefault
static	unsigned short	yyNDefault	[yyLastReadState + 1] = { 0,
    0,     0,     0,     0,     9,     9,     0,    52,     0,     0, 
    0,     0,     0,     0,    52,     0,     0,     0,     0,     0, 
    0,    52,     0,     0,     8,     0,     0,     0,     0,     0, 
    0,     0,     0,     0,     0,     0,     0,    52,     8,     8, 
    0,     0,     0,     0,     0,     0,     0,     0,     8,    52, 
   52,     0, 
};
# endif
# if ! defined NO_RECOVER | defined YYDEC_TABLE | defined YYCSTree
static	unsigned char	yyLength	[yyLastReduceState - yyFirstReduceState
							+ 1] = {
    2,     1,     5,     1,     3,     2,     1,     1,     1,     4, 
    0,     1,     1,     2,     1,     1,     1,     4,     3,     4, 
    6,     5,     5,     3,     3,     3,     3,     3,     3,     3, 
    3,     3,     1,     1,     1,     1,     1,     3, 
};
# endif
# if ! defined NO_RECOVER | defined YYDEC_TABLE
static	yytNonterminal	yyLeftHandSide	[yyLastReduceState - yyFirstReduceState
							+ 1] = {
yyNT0_intern,
yyNTroot,
yyNTx,
yyNTname,
yyNTvar,
yyNTvar,
yyNTtypdef,
yyNTtypdef,
yyNTtypdef,
yyNTexec,
yyNTexec,
yyNTbody,
yyNTbody,
yyNTbody,
yyNTstatement,
yyNTstatement,
yyNTstatement,
yyNTvarallocation,
yyNTvarallocation,
yyNTifstmt,
yyNTifstmt,
yyNTwhilestmt,
yyNTforstmt,
yyNTcondition,
yyNTexpr,
yyNTexpr,
yyNTexpr,
yyNTexpr,
yyNTexpr,
yyNTexpr,
yyNTexpr,
yyNTexpr,
yyNTexpr,
yyNTexpr,
yyNTexpr,
yyNTexprstring,
yyNTexprstring,
yyNTexprstring,
};
# endif
# ifndef NO_RECOVER
static	yySymbolRange	yyContinuation	[yyLastReadState + 1] = { 0,
    5,     4,     0,     6,     9,     0,     4,     4,     0,     6, 
   19,    19,    19,    11,    27,     4,    23,     4,     1,     3, 
    6,    24,     4,    20,     4,    20,    20,     6,     4,    20, 
    4,     4,     4,     4,     4,     4,     4,    27,     4,     4, 
    3,     6,     6,     6,     6,     6,     6,     6,     4,    24, 
   24,    24, 
};
static	unsigned short	yyCondition	[yyLastState - yyLastReduceState + 1] =
{ 0,
};
# endif
static	unsigned short	yyFinalToProd	[yyLastReadReduceState -
						yyFirstReadReduceState + 2] = {
   70,    71,    73,    74,    75,    76,    84,    90,    98,    99, 
  100,   101,   102,   103, 
0
};
static	unsigned short	yyStartLine	[yyLastStopState - yyFirstReduceState
							+ 2] = { 0,
78,
};
# ifdef YYaccDefault

static	unsigned long *	yyDefaultLook	[yyLastReadState + 1] = { 0,
};
# endif


static	int	yyParse		ARGS ((yyStateRange, yySymbolRange, int));

# ifndef NO_RECOVER
static	yyStateRange yyNext	ARGS ((yyStateRange, yySymbolRange));
#  ifdef YYDEBUG
static	int	yyErrorRecovery	ARGS ((yySymbolRange, yySymbolRange *, long));
#  else
static	int	yyErrorRecovery	ARGS ((yySymbolRange, long));
#  endif
static	void	yyComputeContinuation	ARGS ((long, tSet *));
static	rbool	yyIsContinuation	ARGS ((yySymbolRange, long));
static	void	yyComputeRestartPoints	ARGS ((long, tSet *));
# endif

# if defined YYTrialParse | defined YYReParse | defined YYGetLook

# ifndef yyInitBufferSize
#  define yyInitBufferSize 100
# endif

typedef struct { yySymbolRange	yyToken;
		 expr_scan_tScanAttribute	yyAttribute;
# ifdef YYMemoParse
		 short		yyStart;
# endif
	       } yytBuffer;

static yytBuffer *	yyBuffer	= NULL;
static unsigned long	yyBufferSize	= yyInitBufferSize;
static long		yyBufferNext	= 1;
static long		yyBufferLast	= 1;
static rbool		yyBufferClear	= rtrue;
static unsigned short	yyParseLevel	= 0;

static void yyBufferSet
# ifdef HAVE_ARGS
   (yySymbolRange yyToken)
# else
   (yyToken) yySymbolRange yyToken;
# endif
{
   if (yyBufferNext == yyBufferLast) {
      if (yyBufferClear) yyBufferLast = 0;
      if (++ yyBufferLast >= (long) yyBufferSize) {
	 yyExtendArray ((char * *) & yyBuffer, & yyBufferSize,
			  (unsigned long) sizeof (yytBuffer));
# ifdef YYDEBUG
	 if (expr_pars_Debug) {
	    yyPrintState (0);
	    fprintf (yyTrace, "extend  token buffer from %ld to %ld",
		yyBufferSize / 2, yyBufferSize); yyNl ();
	 }
# endif
      }
      yyBuffer [yyBufferLast].yyToken	= yyToken;
      yyBuffer [yyBufferLast].yyAttribute= expr_scan_Attribute;
# ifdef YYMemoParse
      yyBuffer [yyBufferLast].yyStart	= 0;
# endif
      yyBufferNext = yyBufferLast;
   }
}

static int yyGetToken ARGS ((void))
{
   register yySymbolRange yyToken;

   if (yyBufferNext < yyBufferLast) {
      yyToken = yyBuffer [++ yyBufferNext].yyToken;
      expr_scan_Attribute = yyBuffer [yyBufferNext].yyAttribute;
   } else {
      yyToken = expr_scan_GetToken ();
      if ((yytrial | yybuffer) & yyControl.yyMode) {
	 if (++ yyBufferLast >= (long) yyBufferSize) {
	    yyExtendArray ((char * *) & yyBuffer, & yyBufferSize,
			     (unsigned long) sizeof (yytBuffer));
# ifdef YYDEBUG
	    if (expr_pars_Debug) {
	       yyPrintState (0);
	       fprintf (yyTrace, "extend  token buffer from %ld to %ld",
		  yyBufferSize / 2, yyBufferSize); yyNl ();
	    }
# endif
	 }
	 yyBuffer [yyBufferLast].yyToken = yyToken;
	 yyBuffer [yyBufferLast].yyAttribute = expr_scan_Attribute;
# ifdef YYMemoParse
	 yyBuffer [yyBufferLast].yyStart = 0;
# endif
	 yyBufferNext = yyBufferLast;
      }
   }
# ifdef TOKENOP
   TOKENOP
# endif
   return yyToken;
}

# else
# define yyGetToken expr_scan_GetToken
# endif

# ifdef YYGetLook

static int yyGetLookahead
# ifdef HAVE_ARGS
   (int yyk, yySymbolRange yyToken)
# else
   (yyk, yyToken) int yyk; yySymbolRange yyToken;
# endif
{
   if (yyk == 0) return yyToken;
   if (yyControl.yyMode == yystandard) yyBufferSet (yyToken);
   while (yyBufferNext + yyk > yyBufferLast) {
      if (yyBufferNext < yyBufferLast &&
	 yyBuffer [yyBufferLast].yyToken == expr_scan_EofToken)
	 return expr_scan_EofToken;
      if (++ yyBufferLast >= (long) yyBufferSize) {
	 yyExtendArray ((char * *) & yyBuffer, & yyBufferSize,
			  (unsigned long) sizeof (yytBuffer));
# ifdef YYDEBUG
	 if (expr_pars_Debug) {
	    yyPrintState (0);
	    fprintf (yyTrace, "extend  token buffer from %ld to %ld",
		yyBufferSize / 2, yyBufferSize); yyNl ();
	 }
# endif
      }
      yyBuffer [yyBufferLast].yyToken = expr_scan_GetToken ();
      yyBuffer [yyBufferLast].yyAttribute = expr_scan_Attribute;
# ifdef YYMemoParse
      yyBuffer [yyBufferLast].yyStart = 0;
# endif
   }
   expr_scan_Attribute = yyBuffer [yyBufferNext].yyAttribute;
   return yyBuffer [yyBufferNext + yyk].yyToken;
}

static void xxGetAttribute
# ifdef HAVE_ARGS
   (int yyk, yySymbolRange yyToken, expr_scan_tScanAttribute * yyAttribute)
# else
   (yyk, yyToken, yyAttribute)
   int yyk; yySymbolRange yyToken; expr_scan_tScanAttribute * yyAttribute;
# endif
{
   if (yyk == 0) * yyAttribute = expr_scan_Attribute;
   else {
      yyGetLookahead (yyk, yyToken);
      * yyAttribute =
	 yyBuffer [Min (yyBufferNext + yyk, yyBufferLast)].yyAttribute;
   }
}

# endif

# ifdef YYReParse

#  define BufferOn(Actions, Messages) yyBufferOn (Actions, Messages, yyTerminal)
#  define BufferPosition yyBufferNext

static yytControl yyPrevControl;

static long yyBufferOn
# ifdef HAVE_ARGS
   (rbool yyActions, rbool yyMessages, yySymbolRange yyToken)
# else
   (yyActions, yyMessages, yyToken)
   rbool yyActions, yyMessages; yySymbolRange yyToken;
# endif
{
   if (yyControl.yyMode == yystandard) {
      yyPrevControl		= yyControl;
      yyControl.yyMode		= yybuffer;
      yyControl.yyActions	= yyActions;
      yyControl.yyMessages	= yyMessages;
      yyBufferSet (yyToken);
      yyBufferClear		= rfalse;
   }
   return yyBufferNext;
}

static long BufferOff ARGS ((void))
{
   if (yyControl.yyMode == yybuffer) yyControl = yyPrevControl;
   return yyBufferNext;
}

static void BufferClear ARGS ((void))
{
   yyBufferClear = rtrue;
}

# endif

# if defined YYDEBUG | defined YYDCRP

static void yyNl ARGS ((void)) { putc ('\n', yyTrace); fflush (yyTrace); }

# endif

# ifdef YYDEBUG

static void yyPrintState
# ifdef HAVE_ARGS
   (yyStateRange yyState)
# else
   (yyState) yyStateRange yyState;
# endif
{
   fprintf (yyTrace, "%4ld:", ++ yyCount);
   WritePosition  (yyTrace, expr_scan_Attribute.Position);
   fprintf (yyTrace, ":%5d  %c  ", yyState, " ST-B---R" [yyControl.yyMode]);
# if defined YYTrialParse | defined YYReParse
   if (yyParseLevel > 0) {
      register int yyi = yyParseLevel;
      fprintf (yyTrace, "%2d  ", yyi);
      do fputs ("  ", yyTrace); while (-- yyi);
   } else
# endif
   fputs ("    ", yyTrace);
}

static rbool yyPrintResult
# ifdef HAVE_ARGS
   (yyStateRange yyState, int yyLine, rbool yyCondition)
# else
   (yyState, yyLine, yyCondition)
   yyStateRange	yyState;
   int		yyLine;
   rbool	yyCondition;
# endif
{
   if (expr_pars_Debug) {
      yyPrintState (yyState);
      fprintf (yyTrace, "check   predicate in line %d, result = %d",
	 yyLine, yyCondition); yyNl ();
   }
   return yyCondition;
}

# else
#  define yyPrintResult(State, Line, Condition) Condition
# endif

# if defined YYDEBUG | defined YYDEC_TABLE
#  define yyGotoReduce(State, Rule)	{ yyState = State; goto yyReduce; }
#  define yyGotoRead(State)		{ yyState = State; goto yyRead; }
# else
#  define yyGotoReduce(State, Rule)	goto Rule;
#  define yyGotoRead(State)		{ yyState = State; goto yyRead; }
# endif

static unsigned long	yyStateStackSize	= yyInitStackSize;
static yyStateRange *	yyStateStack		= NULL;
static yyStateRange *	yyEndOfStack		;
static unsigned long	yyAttrStackSize		= yyInitStackSize;
static tParsAttribute * yyAttributeStack	= NULL;
# if defined YYTrialParse | defined YYReParse
static yyStateRange *	yyStateStackPtr		;
static tParsAttribute * yyAttrStackPtr		;
# endif
# ifndef NO_RECOVER
static yyStateRange *	yyIsContStackPtr	= NULL;
static unsigned long	yyIsContStackSize	= yyInitStackSize;
static yyStateRange *	yyCompResStackPtr	= NULL;
static unsigned long	yyCompResStackSize	= yyInitStackSize;
# endif

void Resetexpr_pars ARGS ((void))
   {
      if (yyStateStack) {
	 ReleaseArray ((char * *) & yyStateStack, & yyStateStackSize,
			(unsigned long) sizeof (yyStateRange));
	 yyStateStack = NULL;
      }
      if (yyAttributeStack) {
	 ReleaseArray ((char * *) & yyAttributeStack, & yyAttrStackSize,
			(unsigned long) sizeof (tParsAttribute));
	 yyAttributeStack = NULL;
      }
# ifndef NO_RECOVER
      if (yyIsContStackPtr) {
	 ReleaseArray ((char * *) & yyIsContStackPtr, & yyIsContStackSize,
			(unsigned long) sizeof (yyStateRange));
	 yyIsContStackPtr = NULL;
      }
      if (yyCompResStackPtr) {
	 ReleaseArray ((char * *) & yyCompResStackPtr, & yyCompResStackSize,
			(unsigned long) sizeof (yyStateRange));
	 yyCompResStackPtr = NULL;
      }
# endif
# if defined YYTrialParse | defined YYReParse | defined YYGetLook
      if (yyBuffer) {
	 ReleaseArray ((char * *) & yyBuffer, & yyBufferSize,
			(unsigned long) sizeof (yytBuffer));
	 yyBuffer = NULL;
      }
# endif
   }

int expr_pars ARGS ((void))
   {
      return expr_pars2 (yyStartState);
   }

int expr_pars2
# ifdef HAVE_ARGS
   (int yyStartSymbol)
# else
   (yyStartSymbol) int yyStartSymbol;
# endif
   {
      int		yyErrorCount;
# if defined YYDEBUG | defined YYDCRP
      yyTrace		= stdout;
# endif
      Beginexpr_pars ();
      yyStateStackSize	= yyInitStackSize;
      yyMakeArray ((char * *) & yyStateStack, & yyStateStackSize,
		     (unsigned long) sizeof (yyStateRange));
      yyAttrStackSize	= yyInitStackSize;
      yyMakeArray ((char * *) & yyAttributeStack, & yyAttrStackSize,
		     (unsigned long) sizeof (tParsAttribute));
# ifndef NO_RECOVER
      yyIsContStackSize	= yyInitStackSize;
      yyMakeArray ((char * *) & yyIsContStackPtr, & yyIsContStackSize,
		     (unsigned long) sizeof (yyStateRange));
      yyCompResStackSize= yyInitStackSize;
      yyMakeArray ((char * *) & yyCompResStackPtr, & yyCompResStackSize,
		     (unsigned long) sizeof (yyStateRange));
# endif
# if defined YYTrialParse | defined YYReParse | defined YYGetLook
      yyBufferSize	= yyInitBufferSize;
      yyMakeArray ((char * *) & yyBuffer, & yyBufferSize,
		     (unsigned long) sizeof (yytBuffer));
# endif
      yyEndOfStack	= & yyStateStack [yyStateStackSize - 1];
# if defined YYTrialParse | defined YYReParse
      yyStateStackPtr	= yyStateStack;
      yyAttrStackPtr	= yyAttributeStack;
      yyBufferNext	= 1;
      yyBufferLast	= 1;
      yyBufferClear	= rtrue;
      yyParseLevel	= 0;
# endif
# ifdef YYDEBUG
      if (expr_pars_Debug) {
	 yyCount = 0;
	 fprintf (yyTrace,
      "  # |Position|State|Mod|Lev|Action |Terminal and Lookahead or Rule\n");
	 yyNl ();
      }
# endif
      yyControl.yyMode		= yystandard;
      yyControl.yyActions	= rtrue;
      yyControl.yyMessages	= rtrue;
      yyErrorCount = yyParse ((yyStateRange) yyStartSymbol,
	 (yySymbolRange) yyGetToken (), (int) yyStartLine [yyStartSymbol]);
      Resetexpr_pars ();
      return yyErrorCount;
   }

# ifdef YYTrialParse

# ifdef YYMemoParse
#  define MemoryClear(Position) yyBuffer [Position].yyStart = 0
# endif

static int yyTrialParse
# ifdef HAVE_ARGS
   (yyStateRange yyStartSymbol, yySymbolRange yyToken, int yyLine)
# else
   (yyStartSymbol, yyToken, yyLine)
   yyStateRange		yyStartSymbol	;
   yySymbolRange	yyToken		;
   int			yyLine		;
# endif
   {
      int	yyErrorCount		;
      ptrdiff_t	yyPrevStateStackPtr	= yyStateStackPtr - yyStateStack;
      ptrdiff_t	yyPrevAttrStackPtr	= yyAttrStackPtr - yyAttributeStack;
      long	yyPrevBufferNext	;
      yytControl yyPrevControl		;

# ifdef BEFORE_TRIAL
      BEFORE_TRIAL
# endif
# ifdef YYMemoParse
      if (yyBuffer [yyBufferNext].yyStart ==   yyStartSymbol) return 0;
      if (yyBuffer [yyBufferNext].yyStart == - yyStartSymbol) return 1;
# endif
      yyPrevControl		= yyControl;
      yyStateStackPtr		++;
      yyAttrStackPtr		++;
      yyParseLevel		++;
      if (yyControl.yyMode == yystandard) yyBufferSet (yyToken);
      yyPrevBufferNext		= yyBufferNext;
      yyControl.yyMode		= yytrial;
      yyControl.yyActions	= rfalse;
      yyControl.yyMessages	= rfalse;
      yyErrorCount		= yyParse (yyStartSymbol, yyToken, yyLine);
# ifdef YYMemoParse
      yyBuffer [yyPrevBufferNext].yyStart = yyErrorCount ?
					- yyStartSymbol : yyStartSymbol;
# endif
      yyStateStackPtr		= yyStateStack + yyPrevStateStackPtr;
      yyAttrStackPtr		= yyAttributeStack + yyPrevAttrStackPtr;
      yyBufferNext		= yyPrevBufferNext;
      yyControl			= yyPrevControl;
      yyParseLevel		--;
      expr_scan_Attribute		= yyBuffer [yyBufferNext].yyAttribute;
# ifdef AFTER_TRIAL
      AFTER_TRIAL
# endif
      return yyErrorCount;
   }

# endif

# ifdef YYReParse

static int ReParse
# ifdef HAVE_ARGS
   (yyStateRange yyStartSymbol, int yyFrom, int yyTo, rbool yyActions,
      rbool yyMessages)
# else
   (yyStartSymbol, yyFrom, yyTo, yyActions, yyMessages)
   yyStateRange	yyStartSymbol		;
   int		yyFrom, yyTo		;
   rbool	yyActions, yyMessages	;
# endif
   {
      int yyErrorCount = 1;

      if (1 <= yyFrom && yyFrom <= yyTo && yyTo <= yyBufferLast) {
	 ptrdiff_t yyPrevStateStackPtr	= yyStateStackPtr - yyStateStack;
	 ptrdiff_t yyPrevAttrStackPtr	= yyAttrStackPtr - yyAttributeStack;
	 long	yyPrevBufferNext	= yyBufferNext;
	 int	yyToToken		= yyBuffer [yyTo].yyToken;
	 yytControl yyPrevControl	;

	 yyPrevControl		= yyControl;
	 yyStateStackPtr	++;
	 yyAttrStackPtr		++;
	 yyParseLevel		++;
	 yyBufferNext		= yyFrom - 1;
	 yyBuffer [yyTo].yyToken= expr_scan_EofToken;
	 yyControl.yyMode	= yyreparse;
	 yyControl.yyActions	= yyActions;
	 yyControl.yyMessages	= yyMessages;
	 yyErrorCount		= yyParse (yyStartSymbol,
	    (yySymbolRange) yyGetToken (), (int) yyStartLine [yyStartSymbol]);
	 yyStateStackPtr	= yyStateStack + yyPrevStateStackPtr;
	 yyAttrStackPtr		= yyAttributeStack + yyPrevAttrStackPtr;
	 yyBufferNext		= yyPrevBufferNext;
	 yyControl		= yyPrevControl;
	 yyParseLevel		--;
	 yyBuffer [yyTo].yyToken= yyToToken;
	 expr_scan_Attribute		= yyBuffer [yyBufferNext].yyAttribute;
      } else {
      ErrorMessage (xxInvalidCallOfReParse, xxError, expr_scan_Attribute.Position);
      }
      return yyErrorCount;
   }

# endif

static char * yyGetTokenName
# ifdef HAVE_ARGS
   (yySymbolRange yyTerminal)
# else
   (yyTerminal) yySymbolRange yyTerminal;
# endif
   {
      if (yyTerminal <= yyLastTerminal && expr_pars_TokenName [yyTerminal])
	 return expr_pars_TokenName [yyTerminal];
      else
	 return "_unknown_";
   }

static void yyExtendStack
# ifdef HAVE_ARGS
   (yyStateRange * *	yyStateStackPtr,
    tParsAttribute * *	yyAttrStackPtr,
    yyStateRange	yyState)
# else
   (yyStateStackPtr, yyAttrStackPtr, yyState)
   yyStateRange * *	yyStateStackPtr;
   tParsAttribute * *	yyAttrStackPtr;
   yyStateRange		yyState;
# endif
   {
      ptrdiff_t yyyStateStackPtr = * yyStateStackPtr - yyStateStack;
      ptrdiff_t yyyAttrStackPtr = * yyAttrStackPtr - yyAttributeStack;
      yyExtendArray ((char * *) & yyStateStack, & yyStateStackSize,
		       (unsigned long) sizeof (yyStateRange));
      yyExtendArray ((char * *) & yyAttributeStack, & yyAttrStackSize,
		       (unsigned long) sizeof (tParsAttribute));
      * yyStateStackPtr	= yyStateStack + yyyStateStackPtr;
      * yyAttrStackPtr	= yyAttributeStack + yyyAttrStackPtr;
      yyEndOfStack	= & yyStateStack [yyStateStackSize - 1];
# ifdef YYDEBUG
      if (expr_pars_Debug) {
	 yyPrintState (yyState);
	 fprintf (yyTrace, "extend  stack from %ld to %ld",
	    yyStateStackSize / 2, yyStateStackSize); yyNl ();
      }
# endif
   }

static int yyParse
# ifdef HAVE_ARGS
   (yyStateRange yyStartSymbol, yySymbolRange yyToken, int yyLine)
# else
   (yyStartSymbol, yyToken, yyLine)
   yyStateRange		yyStartSymbol	;
   yySymbolRange	yyToken		;
   int			yyLine		;
# endif
   {
      register	yyStateRange	yyState		= yyStartSymbol;
      register	yySymbolRange	yyTerminal	= yyToken;
      register	rbool		yyIsRepairing	= rfalse;
		tParsAttribute	yySynAttribute	;   /* synthesized attribute */
		int		yyErrorCount	= 0;
# if ! (defined YYTrialParse | defined YYReParse)
		yyStateRange *	yyStateStackPtr	= yyStateStack;
		tParsAttribute *yyAttrStackPtr	= yyAttributeStack;
# endif
# ifdef YYDEBUG
		long		yyStartCount	= yyCount + 1;
		yySymbolRange	yyPrevTerminal	= yyToken;
# endif
# ifdef YYGetLook
		yySymbolRange	yy2;
# endif

/* line 32 "expr_pars.lrk" */


/* line 982 "expr_pars.c" */

# ifdef YYDEBUG
      if (expr_pars_Debug) {
	 yyPrintState (yyStartSymbol);
	 fprintf (yyTrace, "parse   for predicate in line %d, lookahead: %s",
	    yyLine, yyGetTokenName (yyTerminal)); yyNl ();
      }
# endif

      memset (& yySynAttribute, 0, sizeof yySynAttribute);

      for (;;) {
	 if (yyStateStackPtr >= yyEndOfStack) {
	    yyExtendStack (& yyStateStackPtr, & yyAttrStackPtr, yyState);
	 }
	 * yyStateStackPtr = yyState;

	 for (;;) { /* SPEC State = Next (State, Terminal); terminal transit */
	    register yytComb * yyTCombPtr = yyTBasePtr [yyState] + yyTerminal;
# if defined YYTDefault & defined YYaccDefault
	    register unsigned long * yylp;
# endif
	    if (yyTCombPtr->Check == yyState) {
	       yyState = yyTCombPtr->Next; break;
	    }
# ifdef YYTDefault
#  ifdef YYaccDefault
	    if ((yylp = yyDefaultLook [yyState]) &&
	       (yylp [yyTerminal >> 5] >> (yyTerminal & 0x1f)) & 1) {
	       yyState = yyTDefault [yyState]; break;
	    }
	    yyState = yyNoState; break;
#  else
	    if ((yyState = yyTDefault [yyState]) == yyNoState) break;
#  endif
# endif
	 }

	 if (yyState == yyNoState ||			/* syntax error ? */
	    yyIsRepairing && yyState >= yyFirstReduceState) { /* repair mode */
	    yySymbolRange yyRepairToken;

	    if (yyState == yyNoState && ! yyIsRepairing) { /* syntax error */
# ifdef ERROR
	       for (;;) {		/* be compatible with error trick */
		  yySymbolRange yyyTerminal = (yySymbolRange) yyTerminal;
		  ERROR
		  goto yyRecover;
	       }
	       goto yyFinal;
   yyRecover:
# endif
# ifdef ERROR2
	       ERROR2
# endif
# ifdef YYTrialParse
	       if (yyControl.yyMode == yytrial) YYABORT;
# endif
# ifndef NO_RECOVER
	       yyErrorCount ++;				/* report and recover */
	       yyTerminal = yyErrorRecovery (yyTerminal,
#  ifdef YYDEBUG
			& yyPrevTerminal,
#  endif
			(long) (yyStateStackPtr - yyStateStack));
	       yyIsRepairing = rtrue;
	       yyState = * yyStateStackPtr;
	       continue;
# else
	       YYABORT;
# endif
	    }

# ifndef NO_RECOVER
	/* SPEC State = Next (Top (), Continuation [yyState]); repair transit */
	    yyState = * yyStateStackPtr;
	    yyRepairToken = yyContinuation [yyState];	/* repair */
	    yyState = yyNext (yyState, yyRepairToken);
	    if (yyState > yyLastReduceState) {		/* dynamic ? */
	       yyState = yyCondition [yyState - yyLastReduceState];
	    }
	    if (yyState <= yyLastReadReduceState) { /* read or read reduce ? */
	       expr_scan_tScanAttribute yyRepairAttribute;
	       expr_scan_ErrorAttribute ((int) yyRepairToken, & yyRepairAttribute);
	       if (yyControl.yyMessages)
		  ErrorMessageI (xxTokenInserted, xxRepair,
		     expr_scan_Attribute.Position, xxString,
		     yyGetTokenName (yyRepairToken));
#  ifdef YYDEBUG
	       if (expr_pars_Debug) {
		  yyPrintState (* yyStateStackPtr);
		  fprintf (yyTrace, "insert  %s",
		     yyGetTokenName (yyRepairToken)); yyNl ();
		  yyPrintState (* yyStateStackPtr);
		  fprintf (yyTrace, "shift   %s, lookahead: %s",
		     yyGetTokenName (yyRepairToken),
		     yyGetTokenName (yyTerminal)); yyNl ();
	       }
#  endif
	       if (yyStateStackPtr >= yyEndOfStack) {
		  yyExtendStack (& yyStateStackPtr, & yyAttrStackPtr, yyState);
	       }
	       yyStateStackPtr ++;
	       yyGetAttribute (yyAttrStackPtr ++, yyRepairAttribute);
	       if (yyState >= yyFirstFinalState) {	/* read reduce */
		  * yyStateStackPtr = yyStateStackPtr [-1];
		  yyState = yyFinalToProd [yyState - yyFirstReadReduceState];
	       } else {					/* read */
		  * yyStateStackPtr = yyState;
	       }
	    }
	    if (yyState <= yyLastReadState) continue;	/* read ? */
# endif
	 }

   yyFinal:
	 if (yyState >= yyFirstFinalState) {		/* final state ? */
	    if (yyState <= yyLastReadReduceState) {	/* read reduce ? */
	       yyStateStackPtr ++;
	       yyGetAttribute (yyAttrStackPtr ++, expr_scan_Attribute);
	       yyTerminal = yyGetToken ();
# ifdef YYDEBUG
	       if (expr_pars_Debug) {
		  yyStateStackPtr [0] = yyStateStackPtr [-1];
		  yyPrintState (* yyStateStackPtr);
		  fprintf (yyTrace, "shift   %s, lookahead: %s",
		     yyGetTokenName (yyPrevTerminal),
		     yyGetTokenName (yyTerminal)); yyNl ();
		  yyPrevTerminal = yyTerminal;
	       }
# endif
	       yyIsRepairing = rfalse;
	    }

	    for (;;) {
	       register yytNonterminal yyNonterminal;	/* left-hand side */

   yyReduce:
# ifdef YYDEBUG
	       if (expr_pars_Debug) {
		  if (yyState <= yyLastReadReduceState)	/* read reduce ? */
		     yyState = yyFinalToProd [yyState - yyFirstReadReduceState];
		  yyPrintState (* yyStateStackPtr);
		  if (yyState <= yyLastReduceState) {
		     fprintf (yyTrace, "reduce  %s",
			yyRule [yyState - yyLastReadReduceState]); yyNl ();
		  } else {
		     fprintf (yyTrace, "dynamic decision %d",
			yyState - yyLastReduceState); yyNl ();
		  }
	       }
# endif
# ifdef YYCSTree
	       if (yyControl.yyActions) {
		 yyStateRange yystate = yyState;
		 if (yystate <= yyLastReadReduceState) { /* read reduce ? */
		   yystate = yyFinalToProd [yystate - yyFirstReadReduceState];
		 }
		 if (yystate <= yyLastStopState) {		/* accept */
		   CSTree = NewNonterminal (yystate - yyLastReadReduceState,
		     yyAttrStackPtr [-1].Common.Tree);
		 } else if (yystate <= yyLastReduceState) {	/* reduce */
		   int yyrule = yystate - yyLastReadReduceState;
		   int yyleng = yyLength [yyrule - 1], i;
		   tCSTree yynode = NULL, yynext = NULL;
		   for (i = yyleng - 1; i >= 0; i --) {
		     unsigned short yysymbol = yyRules [yyrule] [i];
		     tParsAttribute * yyap = & yyAttrStackPtr [i - yyleng];
		     if (yysymbol < yyEpsilon) {		/* terminal */
		       yynode = NewTerminal (yyap->Scan.Position,
			 StGetCStr (yyap->Scan.Common.Text));
		     } else {					/* nonterminal*/
		       yynode = yyap->Common.Tree;
		     }
		     yynode->Next = yynext;
		     if (yynext) yynext->Previous = yynode;
		     yynext = yynode;
		   }
		   yySynAttribute.Common.Tree = NewNonterminal (yyrule, yynode);
		 }
	       }
# endif
# ifdef YYDEC_TABLE
	       if (yyLastStopState < yyState && yyState <= yyLastReduceState) {
		  register int yyd = yyLength [yyState - yyFirstReduceState];
		  yyStateStackPtr -= yyd;
		  yyAttrStackPtr  -= yyd;
		  yyNonterminal = yyLeftHandSide [yyState - yyFirstReduceState];
	       }
# endif
switch (yyState) {
case 67:
YYACCEPT;
case 68: /* root : x */
yyDecrement (1) yySetNT (yyNTroot) {
/* line 1178 "expr_pars.c" */
} break;
case 69: /* x : '#' name ';' var exec */
yyDecrement (5) yySetNT (yyNTx) {
/* line 1182 "expr_pars.c" */
} break;
case 70:
case 53: /* name : identifier */
yyDecrement (1) yySetNT (yyNTname) {
/* line 1187 "expr_pars.c" */
} break;
case 71:
case 54: /* var : typdef name ';' */
yyDecrement (3) yySetNT (yyNTvar) {
/* line 1192 "expr_pars.c" */
} break;
case 72: /* var : var var */
yyDecrement (2) yySetNT (yyNTvar) {
/* line 1196 "expr_pars.c" */
} break;
case 73:
case 55: /* typdef : 'int' */
yyDecrement (1) yySetNT (yyNTtypdef) {
/* line 1201 "expr_pars.c" */
} break;
case 74:
case 56: /* typdef : 'float' */
yyDecrement (1) yySetNT (yyNTtypdef) {
/* line 1206 "expr_pars.c" */
} break;
case 75:
case 57: /* typdef : 'String' */
yyDecrement (1) yySetNT (yyNTtypdef) {
/* line 1211 "expr_pars.c" */
} break;
case 76:
case 58: /* exec : 'BEGIN' body 'END' ';' */
yyDecrement (4) yySetNT (yyNTexec) {
/* line 1216 "expr_pars.c" */
} break;
case 77: /* exec : */
yySetNT (yyNTexec) {
/* line 1220 "expr_pars.c" */
} break;
case 78: /* body : varallocation */
yyDecrement (1) yySetNT (yyNTbody) {
/* line 1224 "expr_pars.c" */
} break;
case 79: /* body : statement */
yyDecrement (1) yySetNT (yyNTbody) {
/* line 1228 "expr_pars.c" */
} break;
case 80: /* body : body body */
yyDecrement (2) yySetNT (yyNTbody) {
/* line 1232 "expr_pars.c" */
} break;
case 81: /* statement : ifstmt */
yyDecrement (1) yySetNT (yyNTstatement) {
/* line 1236 "expr_pars.c" */
} break;
case 82: /* statement : whilestmt */
yyDecrement (1) yySetNT (yyNTstatement) {
/* line 1240 "expr_pars.c" */
} break;
case 83: /* statement : forstmt */
yyDecrement (1) yySetNT (yyNTstatement) {
/* line 1244 "expr_pars.c" */
} break;
case 84:
case 59: /* varallocation : name '=' expr ';' */
yyDecrement (4) yySetNT (yyNTvarallocation) {
/* line 1249 "expr_pars.c" */
} break;
case 85: /* varallocation : name '=' exprstring */
yyDecrement (3) yySetNT (yyNTvarallocation) {
/* line 1253 "expr_pars.c" */
} break;
case 86: /* ifstmt : 'if' condition 'then' body */
yyDecrement (4) yySetNT (yyNTifstmt) {
/* line 1257 "expr_pars.c" */
} break;
case 87: /* ifstmt : 'if' condition 'then' body 'else' body */
yyDecrement (6) yySetNT (yyNTifstmt) {
/* line 1261 "expr_pars.c" */
} break;
case 88: /* whilestmt : 'while' '(' expr ')' body */
yyDecrement (5) yySetNT (yyNTwhilestmt) {
/* line 1265 "expr_pars.c" */
} break;
case 89: /* forstmt : 'for' '(' int_const ')' body */
yyDecrement (5) yySetNT (yyNTforstmt) {
/* line 1269 "expr_pars.c" */
} break;
case 90:
case 60: /* condition : '(' expr ')' */
yyDecrement (3) yySetNT (yyNTcondition) {
/* line 1274 "expr_pars.c" */
} break;
case 91: /* expr : expr '+' expr */
yyDecrement (3) yySetNT (yyNTexpr) {
/* line 1278 "expr_pars.c" */
} break;
case 92: /* expr : expr '-' expr */
yyDecrement (3) yySetNT (yyNTexpr) {
/* line 1282 "expr_pars.c" */
} break;
case 93: /* expr : expr '*' expr */
yyDecrement (3) yySetNT (yyNTexpr) {
/* line 1286 "expr_pars.c" */
} break;
case 94: /* expr : expr '/' expr */
yyDecrement (3) yySetNT (yyNTexpr) {
/* line 1290 "expr_pars.c" */
} break;
case 95: /* expr : expr '<' expr */
yyDecrement (3) yySetNT (yyNTexpr) {
/* line 1294 "expr_pars.c" */
} break;
case 96: /* expr : expr '>' expr */
yyDecrement (3) yySetNT (yyNTexpr) {
/* line 1298 "expr_pars.c" */
} break;
case 97: /* expr : expr '==' expr */
yyDecrement (3) yySetNT (yyNTexpr) {
/* line 1302 "expr_pars.c" */
} break;
case 98:
case 61: /* expr : '(' expr ')' */
yyDecrement (3) yySetNT (yyNTexpr) {
/* line 1307 "expr_pars.c" */
} break;
case 99:
case 62: /* expr : int_const */
yyDecrement (1) yySetNT (yyNTexpr) {
/* line 1312 "expr_pars.c" */
} break;
case 100:
case 63: /* expr : float_const */
yyDecrement (1) yySetNT (yyNTexpr) {
/* line 1317 "expr_pars.c" */
} break;
case 101:
case 64: /* expr : identifier */
yyDecrement (1) yySetNT (yyNTexpr) {
/* line 1322 "expr_pars.c" */
} break;
case 102:
case 65: /* exprstring : identfier */
yyDecrement (1) yySetNT (yyNTexprstring) {
/* line 1327 "expr_pars.c" */
} break;
case 103:
case 66: /* exprstring : string_const */
yyDecrement (1) yySetNT (yyNTexprstring) {
/* line 1332 "expr_pars.c" */
} break;
case 104: /* exprstring : exprstring '+' exprstring */
yyDecrement (3) yySetNT (yyNTexprstring) {
/* line 1336 "expr_pars.c" */
} break;
default: switch (yyState) {
case 1: goto yyAbort;
case 2: goto yyRead;
case 3: goto yyReduce;
}
}

	/* SPEC State = Next (Top (), Nonterminal); nonterminal transition */
# ifdef YYNDefault
	       yyState = * yyStateStackPtr ++;
	       for (;;) {
		  register yytComb * yyNCombPtr =
				yyNBasePtr [yyState] + (int) yyNonterminal;
		  if (yyNCombPtr->Check == yyState) {
		     yyState = yyNCombPtr->Next; break;
		  }
		  yyState = yyNDefault [yyState];
	       }
# else
	       yyState = yyNBasePtr [* yyStateStackPtr ++] [yyNonterminal];
# endif
	       * yyAttrStackPtr ++ = yySynAttribute;
	       if (yyState < yyFirstFinalState) break;	/* read nonterminal ? */
# ifdef YYDEBUG
	       yyStateStackPtr [0] = yyStateStackPtr [-1];
# endif
	    }

	 } else {					/* read */
   yyRead:  yyStateStackPtr ++;
	    yyGetAttribute (yyAttrStackPtr ++, expr_scan_Attribute);
	    yyTerminal = yyGetToken ();
# ifdef YYDEBUG
	    if (expr_pars_Debug) {
	       yyPrintState (yyStateStackPtr [-1]);
	       fprintf (yyTrace, "shift   %s, lookahead: %s",
		  yyGetTokenName (yyPrevTerminal),
		  yyGetTokenName (yyTerminal)); yyNl ();
	       yyPrevTerminal = yyTerminal;
	    }
# endif
	    yyIsRepairing = rfalse;
	 }
      }

   yyAbort:
# ifdef YYDEBUG
      if (expr_pars_Debug) {
	 yyPrintState (* yyStateStackPtr);
	 fprintf (yyTrace, "fail    parse started at %ld", yyStartCount);
	 yyNl ();
      }
# endif
      return ++ yyErrorCount;

   yyAccept:
# ifdef YYDEBUG
      if (expr_pars_Debug) {
	 yyPrintState (* yyStateStackPtr);
	 fprintf (yyTrace, "accept  parse started at %ld", yyStartCount);
	 yyNl ();
      }
# endif
      return yyErrorCount;
   }

# ifndef NO_RECOVER
static int yyErrorRecovery
# ifdef HAVE_ARGS
   (yySymbolRange yyTerminal,
#  ifdef YYDEBUG
    yySymbolRange * yyPrevTerminal,
#  endif
    long yyStackPtr)
# else
   (yyTerminal,
#  ifdef YYDEBUG
    yyPrevTerminal,
#  endif
    yyStackPtr)
   yySymbolRange	yyTerminal	;
#  ifdef YYDEBUG
   yySymbolRange *	yyPrevTerminal	;
#  endif
   long			yyStackPtr	;
# endif
   {
#     define	yyContinueSize	5000
      rbool	yyTokensSkipped	;
      tSet	yyContinueSet	;
      tSet	yyRestartSet	;
      int	yyLength	;
      char	yyContinueString [yyContinueSize + 2];

      if (yyControl.yyMessages) {
   /* 1. report an error */
	 ErrorMessage (xxSyntaxError, xxError, expr_scan_Attribute.Position);

   /* 2. report the offending token */
	 strcpy (yyContinueString, yyGetTokenName (yyTerminal));
# ifdef SPELLING
	 if (strncmp (yyContinueString, expr_scan_TokenPtr, expr_scan_TokenLength)) {
	    yyContinueString [yyLength = strlen (yyContinueString)] = ' ';
	    expr_scan_GetWord (& yyContinueString [++ yyLength]);
	 }
# endif
	 ErrorMessageI (xxTokenFound, xxInformation, expr_scan_Attribute.Position,
	    xxString, yyContinueString);

   /* 3. report the set of expected terminal symbols */
	 MakeSet (& yyContinueSet, yyLastTerminal);
	 yyComputeContinuation (yyStackPtr, & yyContinueSet);
	 yyLength = 0;
	 yyContinueString [0] = '\0';
	 while (! IsEmpty (& yyContinueSet)) {
	    char * yyTokenString =
	       yyGetTokenName ((yySymbolRange) Extract (& yyContinueSet));
	    int yyl = strlen (yyTokenString);
	    if (yyLength + yyl >= yyContinueSize) break;
	    strcpy (& yyContinueString [yyLength], yyTokenString);
	    yyLength += yyl;
	    yyContinueString [yyLength ++] = ' ';
	 }
	 yyContinueString [-- yyLength] = '\0';
	 ErrorMessageI (xxExpectedTokens, xxInformation, expr_scan_Attribute.Position,
	    xxString, yyContinueString);
	 ReleaseSet (& yyContinueSet);
      }

   /* 4. compute the set of terminal symbols for restart of the parse */
      MakeSet (& yyRestartSet, yyLastTerminal);
      yyComputeRestartPoints (yyStackPtr, & yyRestartSet);

   /* 5. skip terminal symbols until a restart point is reached */
      yyTokensSkipped = rfalse;
      while (! IsElement (yyTerminal, & yyRestartSet)) {
	 yyTerminal = yyGetToken ();
	 yyTokensSkipped = rtrue;
# ifdef YYDEBUG
	 if (expr_pars_Debug) {
	    yyPrintState (yyStateStack [yyStackPtr]);
	    fprintf (yyTrace, "skip    %s, lookahead: %s",
	       yyGetTokenName (* yyPrevTerminal),
	       yyGetTokenName (yyTerminal)); yyNl ();
	    * yyPrevTerminal = yyTerminal;
	 }
# endif
      }
      ReleaseSet (& yyRestartSet);

   /* 6. report the restart point */
      if (yyTokensSkipped & yyControl.yyMessages)
	 ErrorMessage (xxRestartPoint, xxInformation, expr_scan_Attribute.Position);

      return yyTerminal;
   }

/*
   compute the set of terminal symbols that can be accepted (read)
   in a given stack configuration (eventually after reduce actions)
*/

static void yyComputeContinuation
# ifdef HAVE_ARGS
   (long yyStackPtr, tSet * yyContinueSet)
# else
   (yyStackPtr, yyContinueSet)
   long			yyStackPtr	;
   tSet *		yyContinueSet	;
# endif
   {
      register yySymbolRange	yyTerminal;
      register yyStateRange	yyState = yyStateStack [yyStackPtr];

      AssignEmpty (yyContinueSet);
      for (yyTerminal = yyFirstTerminal; yyTerminal <= yyLastTerminal;
							yyTerminal ++) {
	 yyStateRange yyNextState = yyNext (yyState, yyTerminal);
	 if (yyNextState != yyNoState && (yyNextState <= yyLastStopState ||
	    yyIsContinuation (yyTerminal, yyStackPtr))) {
	    Include (yyContinueSet, yyTerminal);
	 }
      }
   }

/*
   check whether a given terminal symbol can be accepted (read)
   in a certain stack configuration (eventually after reduce actions)
*/

static rbool yyIsContinuation
# ifdef HAVE_ARGS
   (yySymbolRange yyTerminal, long yyStackPtr)
# else
   (yyTerminal, yyStackPtr)
   yySymbolRange	yyTerminal	;
   long			yyStackPtr	;
# endif
   {
      register yyStateRange	yState		;
      register yytNonterminal	yyNonterminal	;

      while (yyStackPtr >= yyIsContStackSize)	       /* pass Stack by value */
	 yyExtendArray ((char * *) & yyIsContStackPtr, & yyIsContStackSize,
			  (unsigned long) sizeof (yyStateRange));
      memcpy ((char *) yyIsContStackPtr, (char *) yyStateStack,
		(int) sizeof (yyStateRange) * (yyStackPtr + 1));

      yState = yyIsContStackPtr [yyStackPtr];
      for (;;) {
	 yyIsContStackPtr [yyStackPtr] = yState;
	 yState = yyNext (yState, yyTerminal);
	 if (yState == yyNoState) return rfalse;

	 do {						/* reduce */
	    if (yState > yyLastReduceState) {		/* dynamic ? */
	       yState = yyCondition [yState - yyLastReduceState];
	    }
	    if (yState <= yyLastStopState) { /* read, read reduce, or accept? */
	       return rtrue;
	    } else {					/* reduce */
	       yyStackPtr -= yyLength [yState - yyFirstReduceState];
	       yyNonterminal = yyLeftHandSide [yState - yyFirstReduceState];
	    }

	    yState = yyNext (yyIsContStackPtr [yyStackPtr],
				(yySymbolRange) yyNonterminal);
	    yyStackPtr ++;
	    if (yyStackPtr >= yyIsContStackSize) {
	       yyExtendArray ((char * *) & yyIsContStackPtr,
		  & yyIsContStackSize, (unsigned long) sizeof (yyStateRange));
	       if (yyStackPtr >= 1000) return rfalse; /* EMERGENCY break !!! */
	    }
	 } while (yState >= yyFirstFinalState);
      }
   }

/*
   compute the set of terminal symbols that can be accepted (read)
   directly in a given stack configuration
*/

static void yyCompContinuation
# ifdef HAVE_ARGS
   (yyStateRange yyState, tSet * yyContinueSet)
# else
   (yyState, yyContinueSet)
   yyStateRange		yyState		;
   tSet *		yyContinueSet	;
# endif
   {
      register yySymbolRange	yyTerminal;

      AssignEmpty (yyContinueSet);
      for (yyTerminal = yyFirstTerminal; yyTerminal <= yyLastTerminal;
							yyTerminal ++) {
	 yyStateRange yyNextState = yyNext (yyState, yyTerminal);
	 if (yyNextState != yyNoState && yyNextState <= yyLastStopState) {
	    Include (yyContinueSet, yyTerminal);
	 }
      }
   }

/*
   compute a set of terminal symbols that can be used to restart
   parsing in a given stack configuration. We simulate parsing until
   end of file using a suffix program synthesized by the function (array)
   yyContinuation. All symbols acceptable in the states reached during
   the simulation can be used to restart parsing.
*/

static void yyComputeRestartPoints
# ifdef HAVE_ARGS
   (long yyStackPtr, tSet * yyRestartSet)
# else
   (yyStackPtr, yyRestartSet)
   long			yyStackPtr	;
   tSet *		yyRestartSet	;
# endif
   {
      register yyStateRange	yState		;
      register yytNonterminal	yyNonterminal	;
	       tSet		yyContinueSet	;

      while (yyStackPtr >= yyCompResStackSize)	       /* pass Stack by value */
	 yyExtendArray ((char * *) & yyCompResStackPtr, & yyCompResStackSize,
			  (unsigned long) sizeof (yyStateRange));
      memcpy ((char *) yyCompResStackPtr, (char *) yyStateStack,
		(int) sizeof (yyStateRange) * (yyStackPtr + 1));

      MakeSet (& yyContinueSet, yyLastTerminal);
      AssignEmpty (yyRestartSet);
      yState = yyCompResStackPtr [yyStackPtr];

      for (;;) {
	 if (yyStackPtr >= yyCompResStackSize)
	    yyExtendArray ((char * *) & yyCompResStackPtr, & yyCompResStackSize,
			     (unsigned long) sizeof (yyStateRange));
	 yyCompResStackPtr [yyStackPtr] = yState;
	 yyCompContinuation (yState, & yyContinueSet);
	 Union (yyRestartSet, & yyContinueSet);
# ifdef YYDCRP
	 {
	    int yyLength = 0;
	    char yyContinueString [yyContinueSize + 2];
	    yyContinueString [0] = '\0';
	    while (! IsEmpty (& yyContinueSet)) {
	       char * yyTokenString =
		  yyGetTokenName ((yySymbolRange) Extract (& yyContinueSet));
	       int yyl = strlen (yyTokenString);
	       if (yyLength + yyl >= yyContinueSize) break;
	       strcpy (& yyContinueString [yyLength], yyTokenString);
	       yyLength += yyl;
	       yyContinueString [yyLength ++] = ' ';
	    }
	    yyContinueString [-- yyLength] = '\0';
	    fprintf (yyTrace, "%5d union %s", yState, yyContinueString);
	    yyNl ();
	 }
# endif
	 yState = yyNext (yState, yyContinuation [yState]);

	 if (yState >= yyFirstFinalState) {		/* final state ? */
	    if (yState <= yyLastReadReduceState) {	/* read reduce ? */
	       yyStackPtr ++;
	       yState = yyFinalToProd [yState - yyFirstReadReduceState];
# ifdef YYDCRP
	       yyCompResStackPtr [yyStackPtr] =
					yyCompResStackPtr [yyStackPtr - 1];
	       fprintf (yyTrace, "%5d shift   %s",
		  yyCompResStackPtr [yyStackPtr], yyGetTokenName
		  (yyContinuation [yyCompResStackPtr [yyStackPtr]])); yyNl ();
# endif
	    }

	    do {					/* reduce */
	       if (yState > yyLastReduceState) {	/* dynamic ? */
# ifdef YYDCRP
		  fprintf (yyTrace, "%5d dynamic decision %d",
		    yyCompResStackPtr [yyStackPtr], yState - yyLastReduceState);
		  yyNl ();
# endif
		  yState = yyCondition [yState - yyLastReduceState];
	       }
	       if (yyFirstReduceState <= yState &&
		   yState <= yyLastStopState) {		/* accept */
# ifdef YYDCRP
		  fprintf (yyTrace, "%5d accept",
		     yyCompResStackPtr [yyStackPtr]); yyNl ();
# endif
		  ReleaseSet (& yyContinueSet);
		  return;
	       } else if (yState < yyFirstFinalState) {	/* read */
		  goto yyRead;
	       } else {					/* reduce */
# ifdef YYDCRP
		  fprintf (yyTrace, "%5d reduce  %s",
		     yyCompResStackPtr [yyStackPtr],
		     yyRule [yState - yyLastReadReduceState]); yyNl ();
# endif
		  yyStackPtr -= yyLength [yState - yyFirstReduceState];
		  yyNonterminal = yyLeftHandSide [yState - yyFirstReduceState];
	       }

	       yState = yyNext (yyCompResStackPtr [yyStackPtr],
				(yySymbolRange) yyNonterminal);
	       yyStackPtr ++;
	    } while (yState >= yyFirstFinalState);
	 } else {					/* read */
yyRead:
# ifdef YYDCRP
	    fprintf (yyTrace, "%5d shift   %s",
	       yyCompResStackPtr [yyStackPtr], yyGetTokenName
	       (yyContinuation [yyCompResStackPtr [yyStackPtr]])); yyNl ();
# endif
	    yyStackPtr ++;
	 }
      }
   }

/* access the parse table:   Next : State x Symbol -> Action */

static yyStateRange yyNext
# ifdef HAVE_ARGS
   (yyStateRange yyState, yySymbolRange yySymbol)
# else
   (yyState, yySymbol) yyStateRange yyState; yySymbolRange yySymbol;
# endif
   {
      if (yySymbol <= yyLastTerminal) {
	 for (;;) {
	    register yytComb * yyTCombPtr = yyTBasePtr [yyState] + yySymbol;
# if defined YYTDefault & defined YYaccDefault
	    register unsigned long * yylp;
# endif
	    if (yyTCombPtr->Check == yyState) return yyTCombPtr->Next;
# ifdef YYTDefault
#  ifdef YYaccDefault
	    return (yylp = yyDefaultLook [yyState]) &&
	       (yylp [yySymbol >> 5] >> (yySymbol & 0x1f)) & 1 ?
		  yyTDefault [yyState] : yyNoState;
#  else
	    if ((yyState = yyTDefault [yyState]) == yyNoState) return yyNoState;
#  endif
# else
	    return yyNoState;
# endif
	 }
      }
# ifdef YYNDefault
      for (;;) {
	 register yytComb * yyNCombPtr = yyNBasePtr [yyState] + yySymbol;
	 if (yyNCombPtr->Check == yyState) return yyNCombPtr->Next;
	 yyState = yyNDefault [yyState];
      }
# else
      return yyNBasePtr [yyState] [yySymbol];
# endif
   }
# endif

void Beginexpr_pars ARGS ((void))
   {
/* line 35 "expr_pars.lrk" */


/* line 1764 "expr_pars.c" */
   }

void Closeexpr_pars ARGS ((void))
   {
/* line 38 "expr_pars.lrk" */


/* line 1772 "expr_pars.c" */
   }

