/* $Id: Scanner.c,v 2.67 2009/10/13 14:13:39 grosch Exp $ */

# define rbool		char
# define rtrue		1
# define rfalse		0
# define StdIn		0

# include "l_scan.h"

# ifndef EXTERN_C_BEGIN
# define EXTERN_C_BEGIN
# define EXTERN_C_END
# endif

EXTERN_C_BEGIN
#    include "l_scanSource.h"
#    include "rSystem.h"
#    include "General.h"
#    include "DynArray.h"
#    include "Position.h"
#    include "Errors.h"
EXTERN_C_END

# include <stdio.h>
# include <stdlib.h>
# include <stddef.h>

# define yyStart(State)	{ yyPreviousStart = yyStartState; yyStartState = State;}
# define yyPrevious	{ yyStateRange s = yyStartState; \
			yyStartState = yyPreviousStart; yyPreviousStart = s; }
# define yyEcho		{ (void) fwrite (l_scan_TokenPtr, sizeof (yytChar), \
			l_scan_TokenLength, stdout); }
# define yyEol(Column)	{ yyLineCount ++; \
			yyLineStart = (yytusChar *) l_scan_TokenPtr + \
			l_scan_TokenLength - 1 - (Column); }
# if l_scan_xxMaxCharacter < 256
#  define output(c)	(void) putchar ((int) c)
# else
#  define output(c)	(void) printf ("%lc", c)
# endif
# define yyColumn(Ptr)	((int) ((Ptr) - (yytChar *) yyLineStart))
# define yyOffset(Ptr)	(yyFileOffset + ((Ptr) - yyChBufferStart2))

# define yytChar	l_scan_xxtChar
# define yytusChar	l_scan_xxtusChar

# define yyDNoState		0
# define yyFirstCh	(yytusChar) '\0'
# define yyEolCh	(yytusChar) '\12'
# define yyEobCh	(yytusChar) '\177'
# define yyDStateCount	76
# define yyTableSize	957
# define yyEobState	31
# define yyDefaultState	32
# define yyToClassArraySize	0
# define STD	1
# define STRING	3
# define COMMENT	5
# define xxGetWord
# define xxinput

static void yyExit ARGS ((void))
{ rExit (1); }

typedef unsigned short	yyStateRange;
typedef struct { yyStateRange yyCheck, yyNext; } yyCombType;

	yytChar *	l_scan_TokenPtr	;
	int		l_scan_TokenLength	;
	l_scan_tScanAttribute	l_scan_Attribute	;
	void		(* l_scan_Exit)	ARGS ((void)) = yyExit;

static	void		yyInitialize	ARGS ((void));
static	void		yyErrorMessage	ARGS ((int yyErrorCode));
static	yytChar		input		ARGS ((void));
static	void		unput		ARGS ((yytChar));
static	void		yyLess		ARGS ((int));

static	yyCombType	yyComb		[yyTableSize   + 1] = {
{   0,    0}, {   0,    0}, {   0,    0}, {   0,    0}, {   0,    0}, 
{   0,    0}, {   0,    0}, {   0,    0}, {   0,    0}, {   1,   34}, 
{   1,   33}, {   0,    0}, {   3,   35}, {   3,   33}, {   0,    0}, 
{   5,   34}, {   5,   33}, {   0,    0}, {   0,    0}, {   0,    0}, 
{   0,    0}, {   0,    0}, {   0,    0}, {   0,    0}, {   0,    0}, 
{   0,    0}, {   0,    0}, {   0,    0}, {   0,    0}, {   0,    0}, 
{   0,    0}, {   0,    0}, {   1,   36}, {   0,    0}, {   1,   41}, 
{   3,   37}, {   0,    0}, {   3,   40}, {   5,   36}, {   0,    0}, 
{   1,   43}, {   1,   42}, {   1,   46}, {   1,   50}, {   0,    0}, 
{   1,   48}, {   1,   22}, {   1,   47}, {   1,    8}, {   1,    8}, 
{   1,    8}, {   1,    8}, {   1,    8}, {   1,    8}, {   1,    8}, 
{   1,    8}, {   1,    8}, {   1,    8}, {   2,   20}, {  20,   10}, 
{   1,   44}, {   1,   49}, {   1,   45}, {  24,   23}, {   0,    0}, 
{  24,   23}, {   1,   29}, {   1,   64}, {   0,    0}, {   1,   19}, 
{   1,   56}, {   0,    0}, {   0,    0}, {   1,   62}, {   9,   17}, 
{   0,    0}, {   0,    0}, {   0,    0}, {  16,   14}, {  17,   13}, 
{   1,   68}, {  18,   15}, {  21,   24}, {  19,   59}, {   1,   27}, 
{  19,   18}, {  25,   16}, {   1,   51}, {   0,    0}, {   0,    0}, 
{   0,    0}, {   0,    0}, {   0,    0}, {   0,    0}, {   0,    0}, 
{   3,   38}, {   0,    0}, {   0,    0}, {   1,   29}, {   1,   64}, 
{  26,    9}, {   1,   19}, {   1,   56}, {   0,    0}, {   0,    0}, 
{   1,   62}, {   9,   17}, {   0,    0}, {   0,    0}, {   0,    0}, 
{  16,   14}, {  17,   13}, {   1,   68}, {  18,   15}, {  21,   24}, 
{  19,   59}, {   1,   27}, {  19,   18}, {  25,   16}, {   1,   51}, 
{   7,    7}, {   7,    7}, {   7,    7}, {   7,    7}, {   7,    7}, 
{   7,    7}, {   7,    7}, {   7,    7}, {   7,    7}, {   7,    7}, 
{  30,   31}, {   7,    7}, {  26,    9}, {  38,   39}, {   7,    7}, 
{   7,    7}, {   7,    7}, {   7,    7}, {   7,    7}, {   7,    7}, 
{   7,    7}, {   7,    7}, {   7,    7}, {   7,    7}, {   7,    7}, 
{   7,    7}, {   7,    7}, {   7,    7}, {   7,    7}, {   7,    7}, 
{   7,    7}, {   7,    7}, {   7,    7}, {   7,    7}, {   0,    0}, 
{   7,    7}, {   7,    7}, {   7,    7}, {   7,    7}, {   7,    7}, 
{   7,    7}, {   7,    7}, {   7,    7}, {   7,    7}, {   7,    7}, 
{   7,    7}, {   7,    7}, {   7,    7}, {   7,    7}, {   7,    7}, 
{   7,    7}, {   7,    7}, {   7,    7}, {   7,    7}, {   7,    7}, 
{   7,    7}, {   7,    7}, {   7,    7}, {   7,    7}, {   7,    7}, 
{   7,    7}, {   7,    7}, {   7,    7}, {   7,    7}, {   7,    7}, 
{   7,    7}, {   7,    7}, {   7,    7}, {   7,    7}, {   7,    7}, 
{   7,    7}, {   7,    7}, {   7,    7}, {   7,    7}, {   7,    7}, 
{   7,    7}, {   7,    7}, {   7,    7}, {   7,    7}, {   7,    7}, 
{   7,    7}, {   7,    7}, {   7,    7}, {   7,    7}, {   7,    7}, 
{   7,    7}, {   7,    7}, {   7,    7}, {   7,    7}, {   7,    7}, 
{   7,    7}, {   7,    7}, {   0,    0}, {   7,    7}, {   7,    7}, 
{   7,    7}, {   7,    7}, {   7,    7}, {   7,    7}, {   7,    7}, 
{   7,    7}, {   7,    7}, {   7,    7}, {   7,    7}, {   7,    7}, 
{   7,    7}, {   7,    7}, {   7,    7}, {   7,    7}, {   7,    7}, 
{   7,    7}, {   7,    7}, {   7,    7}, {   7,    7}, {   7,    7}, 
{   7,    7}, {   7,    7}, {   7,    7}, {   7,    7}, {   7,    7}, 
{   7,    7}, {   7,    7}, {   7,    7}, {   7,    7}, {   7,    7}, 
{   7,    7}, {   7,    7}, {   0,    0}, {   7,    7}, {   7,    7}, 
{   7,    7}, {   7,    7}, {   7,    7}, {   7,    7}, {   7,    7}, 
{   7,    7}, {   7,    7}, {   7,    7}, {   7,    7}, {   7,    7}, 
{   7,    7}, {   7,    7}, {   7,    7}, {   7,    7}, {   7,    7}, 
{   7,    7}, {   7,    7}, {   7,    7}, {   7,    7}, {   7,    7}, 
{   7,    7}, {   7,    7}, {   7,    7}, {   7,    7}, {   7,    7}, 
{   7,    7}, {   7,    7}, {   7,    7}, {   7,    7}, {   7,    7}, 
{   7,    7}, {   7,    7}, {   7,    7}, {   7,    7}, {   7,    7}, 
{   7,    7}, {   7,    7}, {   7,    7}, {   7,    7}, {   7,    7}, 
{   7,    7}, {   7,    7}, {   7,    7}, {   7,    7}, {   7,    7}, 
{   7,    7}, {   7,    7}, {   7,    7}, {   7,    7}, {   7,    7}, 
{   7,    7}, {   7,    7}, {   7,    7}, {   7,    7}, {   7,    7}, 
{   7,    7}, {   7,    7}, {   7,    7}, {   7,    7}, {   7,    7}, 
{   7,    7}, {   7,    7}, {   7,    7}, {   7,    7}, {   7,    7}, 
{   7,    7}, {   7,    7}, {   7,    7}, {   7,    7}, {   7,    7}, 
{   7,    7}, {   7,    7}, {   7,    7}, {   7,    7}, {   7,    7}, 
{   7,    7}, {   7,    7}, {   7,    7}, {   7,    7}, {   7,    7}, 
{   7,    7}, {   7,    7}, {   7,    7}, {   7,    7}, {   7,    7}, 
{   7,    7}, {   7,    7}, {   7,    7}, {   7,    7}, {   7,    7}, 
{   7,    7}, {   7,    7}, {   7,    7}, {   7,    7}, {   7,    7}, 
{   7,    7}, {   7,    7}, {   7,    7}, {   7,    7}, {   7,    7}, 
{   7,    7}, {   7,    7}, {   7,    7}, {   7,    7}, {   7,    7}, 
{   7,    7}, {   7,    7}, {   7,    7}, {   7,    7}, {   7,    7}, 
{   7,    7}, {   7,    7}, {   7,    7}, {   7,    7}, {   7,    7}, 
{   7,    7}, {   7,    7}, {   7,    7}, {   7,    7}, {   7,    7}, 
{   7,    7}, {   7,    7}, {   7,    7}, {   7,    7}, {   7,    7}, 
{   7,    7}, {   8,   22}, {   0,    0}, {   8,    8}, {   8,    8}, 
{   8,    8}, {   8,    8}, {   8,    8}, {   8,    8}, {   8,    8}, 
{   8,    8}, {   8,    8}, {   8,    8}, {  10,   10}, {  10,   10}, 
{  10,   10}, {  10,   10}, {  10,   10}, {  10,   10}, {  10,   10}, 
{  10,   10}, {  10,   10}, {  10,   10}, {  10,   12}, {  10,   10}, 
{  10,   10}, {  10,   10}, {  10,   10}, {  10,   10}, {  10,   10}, 
{  10,   10}, {  10,   10}, {  10,   10}, {  10,   10}, {  10,   10}, 
{  10,   10}, {  10,   10}, {  10,   10}, {  10,   10}, {  10,   10}, 
{  10,   10}, {  10,   10}, {  10,   10}, {  10,   10}, {  10,   10}, 
{  10,   10}, {  10,   10}, {  10,   10}, {  10,   10}, {  10,   10}, 
{  10,   10}, {  10,   10}, {  10,   10}, {  10,   10}, {  10,   10}, 
{  10,   10}, {  10,   10}, {  10,   10}, {   0,    0}, {  10,   10}, 
{  10,   10}, {  10,   10}, {  10,   10}, {  10,   10}, {  10,   10}, 
{  10,   10}, {  10,   10}, {  10,   10}, {  10,   10}, {  10,   10}, 
{  10,   10}, {  10,   10}, {  10,   10}, {  10,   10}, {  10,   10}, 
{  10,   10}, {  10,   10}, {  10,   10}, {  10,   10}, {  10,   10}, 
{  10,   10}, {  10,   10}, {  10,   10}, {  10,   10}, {  10,   10}, 
{  10,   10}, {  10,   10}, {  10,   10}, {  10,   10}, {  10,   10}, 
{  10,   10}, {  10,   10}, {  10,   10}, {  10,   10}, {  10,   10}, 
{  10,   10}, {  10,   10}, {  10,   10}, {  10,   10}, {  10,   10}, 
{  10,   10}, {  10,   10}, {  10,   10}, {  10,   10}, {  10,   10}, 
{  10,   10}, {  10,   10}, {  10,   10}, {  10,   10}, {  10,   10}, 
{  10,   10}, {  10,   10}, {  10,   10}, {  10,   10}, {  10,   10}, 
{  10,   10}, {  10,   10}, {  10,   10}, {  10,   10}, {  10,   10}, 
{  10,   10}, {  10,   10}, {  10,   10}, {  10,   10}, {  10,   10}, 
{  10,   10}, {  10,   10}, {  10,   10}, {  10,   10}, {  10,   10}, 
{  10,   10}, {  10,   10}, {  10,   10}, {  10,   10}, {  10,   10}, 
{  10,   10}, {  10,   10}, {  10,   10}, {  10,   10}, {  10,   10}, 
{   0,    0}, {  10,   10}, {  10,   10}, {  10,   10}, {  10,   10}, 
{  10,   10}, {  10,   10}, {  10,   10}, {  10,   10}, {  10,   10}, 
{  10,   10}, {  10,   10}, {  10,   10}, {  10,   10}, {  10,   10}, 
{  10,   10}, {  10,   10}, {  10,   10}, {  10,   10}, {  10,   10}, 
{  10,   10}, {  10,   10}, {  10,   10}, {  10,   10}, {  10,   10}, 
{  10,   10}, {  10,   10}, {  10,   10}, {  10,   10}, {  10,   10}, 
{  10,   10}, {  10,   10}, {  10,   10}, {  10,   10}, {  10,   10}, 
{  10,   10}, {  10,   10}, {  10,   10}, {  10,   10}, {  10,   10}, 
{  10,   10}, {  10,   10}, {  10,   10}, {  10,   10}, {  10,   10}, 
{  10,   10}, {  10,   10}, {  10,   10}, {  10,   10}, {  10,   10}, 
{  10,   10}, {  10,   10}, {  10,   10}, {  10,   10}, {  10,   10}, 
{  10,   10}, {  10,   10}, {  10,   10}, {  10,   10}, {  10,   10}, 
{  10,   10}, {  10,   10}, {  10,   10}, {  10,   10}, {  10,   10}, 
{  10,   10}, {  10,   10}, {  10,   10}, {  10,   10}, {  10,   10}, 
{  10,   10}, {  10,   10}, {  10,   10}, {  10,   10}, {  10,   10}, 
{  10,   10}, {  10,   10}, {  10,   10}, {  10,   10}, {  10,   10}, 
{  10,   10}, {  10,   10}, {  10,   10}, {  10,   10}, {  10,   10}, 
{  10,   10}, {  10,   10}, {  10,   10}, {  10,   10}, {  10,   10}, 
{  10,   10}, {  10,   10}, {  10,   10}, {  10,   10}, {  10,   10}, 
{  10,   10}, {  10,   10}, {  10,   10}, {  10,   10}, {  10,   10}, 
{  10,   10}, {  10,   10}, {  10,   10}, {  10,   10}, {  10,   10}, 
{  10,   10}, {  10,   10}, {  10,   10}, {  10,   10}, {  10,   10}, 
{  10,   10}, {  10,   10}, {  10,   10}, {  10,   10}, {  10,   10}, 
{  10,   10}, {  10,   10}, {  10,   10}, {  10,   10}, {  10,   10}, 
{  10,   10}, {  10,   10}, {  10,   10}, {  10,   10}, {  10,   10}, 
{  10,   10}, {  10,   10}, {  10,   10}, {  10,   10}, {  11,   11}, 
{  11,   11}, {  11,   11}, {  11,   11}, {  11,   11}, {  11,   11}, 
{  11,   11}, {  11,   11}, {  11,   11}, {  11,   11}, {  22,   21}, 
{  22,   21}, {  22,   21}, {  22,   21}, {  22,   21}, {  22,   21}, 
{  22,   21}, {  22,   21}, {  22,   21}, {  22,   21}, {  27,   25}, 
{   0,    0}, {   0,    0}, {   0,    0}, {   0,    0}, {   0,    0}, 
{   0,    0}, {   0,    0}, {   0,    0}, {   0,    0}, {   0,    0}, 
{   0,    0}, {   0,    0}, {   0,    0}, {   0,    0}, {   0,    0}, 
{  28,   28}, {  28,   28}, {  28,   28}, {  28,   28}, {  28,   28}, 
{  28,   28}, {  28,   28}, {  28,   28}, {  28,   28}, {  28,   28}, 
{   0,    0}, {  29,   26}, {  51,   52}, {  52,   53}, {  53,   54}, 
{   0,    0}, {  27,   25}, {  28,   28}, {  28,   28}, {  28,   28}, 
{  28,   28}, {  28,   28}, {  28,   28}, {  28,   28}, {  28,   28}, 
{  28,   28}, {  28,   28}, {  28,   28}, {  28,   28}, {  28,   28}, 
{  28,   28}, {  28,   28}, {  28,   28}, {  28,   28}, {  28,   28}, 
{  28,   28}, {  28,   28}, {  28,   28}, {  28,   28}, {  28,   28}, 
{  28,   28}, {  28,   28}, {  28,   28}, {  29,   26}, {  51,   52}, 
{  52,   53}, {  53,   54}, {  28,   28}, {  54,   55}, {  28,   28}, 
{  28,   28}, {  28,   28}, {  28,   28}, {  28,   28}, {  28,   28}, 
{  28,   28}, {  28,   28}, {  28,   28}, {  28,   28}, {  28,   28}, 
{  28,   28}, {  28,   28}, {  28,   28}, {  28,   28}, {  28,   28}, 
{  28,   28}, {  28,   28}, {  28,   28}, {  28,   28}, {  28,   28}, 
{  28,   28}, {  28,   28}, {  28,   28}, {  28,   28}, {  28,   28}, 
{  56,   57}, {  57,   58}, {  59,   60}, {  60,   61}, {  62,   63}, 
{  54,   55}, {  64,   65}, {  65,   66}, {  66,   67}, {  68,   69}, 
{  69,   70}, {  70,   71}, {  71,   72}, {  72,   73}, {  73,   74}, 
{  74,   75}, {  75,   76}, {   0,    0}, {   0,    0}, {   0,    0}, 
{   0,    0}, {   0,    0}, {   0,    0}, {   0,    0}, {   0,    0}, 
{   0,    0}, {   0,    0}, {   0,    0}, {   0,    0}, {   0,    0}, 
{   0,    0}, {   0,    0}, {  56,   57}, {  57,   58}, {  59,   60}, 
{  60,   61}, {  62,   63}, {   0,    0}, {  64,   65}, {  65,   66}, 
{  66,   67}, {  68,   69}, {  69,   70}, {  70,   71}, {  71,   72}, 
{  72,   73}, {  73,   74}, {  74,   75}, {  75,   76}, {   0,    0}, 
{   0,    0}, {   0,    0}, {   0,    0}, {   0,    0}, {   0,    0}, 
{   0,    0}, {   0,    0}, {   0,    0}, {   0,    0}, {   0,    0}, 
{   0,    0}, {   0,    0}, {   0,    0}, {   0,    0}, {   0,    0}, 
{   0,    0}, {   0,    0}, {   0,    0}, {   0,    0}, {   0,    0}, 
{   0,    0}, {   0,    0}, {   0,    0}, {   0,    0}, {   0,    0}, 
{   0,    0}, {   0,    0}, {   0,    0}, {   0,    0}, {   0,    0}, 
{   0,    0}, {   0,    0}, {   0,    0}, {   0,    0}, {   0,    0}, 
{   0,    0}, {   0,    0}, {   0,    0}, {   0,    0}, {   0,    0}, 
{   0,    0}, {   0,    0}, {   0,    0}, {   0,    0}, {   0,    0}, 
{   0,    0}, {   0,    0}, {   0,    0}, {   0,    0}, {   0,    0}, 
{   0,    0}, {   0,    0}, {   0,    0}, {   0,    0}, {   0,    0}, 
{   0,    0}, {   0,    0}, {   0,    0}, {   0,    0}, {   0,    0}, 
{   0,    0}, {   0,    0}, {   0,    0}, {   0,    0}, {   0,    0}, 
{   0,    0}, {   0,    0}, {   0,    0}, {   0,    0}, {   0,    0}, 
{   0,    0}, {   0,    0}, {   0,    0}, {   0,    0}, {   0,    0}, 
{   0,    0}, {   0,    0}, {   0,    0}, {   0,    0}, {   0,    0}, 
{   0,    0}, {   0,    0}, {   0,    0}, {   0,    0}, {   0,    0}, 
{   0,    0}, {   0,    0}, {   0,    0}, {   0,    0}, {   0,    0}, 
{   0,    0}, {   0,    0}, {   0,    0}, {   0,    0}, {   0,    0}, 
{   0,    0}, {   0,    0}, {   0,    0}, {   0,    0}, {   0,    0}, 
{   0,    0}, {   0,    0}, {   0,    0}, {   0,    0}, {   0,    0}, 
{   0,    0}, {   0,    0}, {   0,    0}, {   0,    0}, {   0,    0}, 
{   0,    0}, {   0,    0}, {   0,    0}, {   0,    0}, {   0,    0}, 
{   0,    0}, {   0,    0}, {   0,    0}, {   0,    0}, {   0,    0}, 
{   0,    0}, {   0,    0}, {   0,    0}, {   0,    0}, {   0,    0}, 
{   0,    0}, {   0,    0}, {   0,    0}, {   0,    0}, {   0,    0}, 
{   0,    0}, {   0,    0}, {   0,    0}, {   0,    0}, {   0,    0}, 
{   0,    0}, {   0,    0}, {   0,    0}, {   0,    0}, {   0,    0}, 
{   0,    0}, {   0,    0}, {   0,    0}, {   0,    0}, {   0,    0}, 
{   0,    0}, {   0,    0}, {   0,    0}, {   0,    0}, {   0,    0}, 
{   0,    0}, {   0,    0}, {   0,    0}, 
};
static	yyCombType *	yyBasePtr	[yyDStateCount + 1] = { 0,
& yyComb [   0], & yyComb [  13], & yyComb [   3], & yyComb [   0], 
& yyComb [   6], & yyComb [   0], & yyComb [ 120], & yyComb [ 330], 
& yyComb [   1], & yyComb [ 388], & yyComb [ 596], & yyComb [   0], 
& yyComb [   0], & yyComb [   0], & yyComb [   0], & yyComb [   0], 
& yyComb [   1], & yyComb [  13], & yyComb [   7], & yyComb [  14], 
& yyComb [  13], & yyComb [ 606], & yyComb [   0], & yyComb [  20], 
& yyComb [  17], & yyComb [  29], & yyComb [ 592], & yyComb [ 632], 
& yyComb [ 622], & yyComb [   3], & yyComb [   0], & yyComb [   0], 
& yyComb [   0], & yyComb [   0], & yyComb [   0], & yyComb [   0], 
& yyComb [   0], & yyComb [  23], & yyComb [   0], & yyComb [   0], 
& yyComb [   0], & yyComb [   0], & yyComb [   0], & yyComb [   0], 
& yyComb [   0], & yyComb [   0], & yyComb [   0], & yyComb [   0], 
& yyComb [   0], & yyComb [   0], & yyComb [ 620], & yyComb [ 620], 
& yyComb [ 618], & yyComb [ 659], & yyComb [   0], & yyComb [ 676], 
& yyComb [ 674], & yyComb [   0], & yyComb [ 674], & yyComb [ 689], 
& yyComb [   0], & yyComb [ 689], & yyComb [   0], & yyComb [ 696], 
& yyComb [ 679], & yyComb [ 694], & yyComb [   0], & yyComb [ 682], 
& yyComb [ 686], & yyComb [ 699], & yyComb [ 698], & yyComb [ 700], 
& yyComb [ 684], & yyComb [ 688], & yyComb [ 702], & yyComb [   0], 
};
static	yyStateRange	yyDefault	[yyDStateCount + 1] = { 0,
   28,     1,     7,     3,    30,     5,    30,    30,    28,    20, 
   30,     0,    28,    28,    28,    28,    28,    28,    28,    30, 
   22,    30,    11,    11,    28,    28,    28,    30,    28,     0, 
    0,     0,     0,     0,     7,     0,     7,    30,     0,     0, 
    0,     0,     0,     0,     0,     0,     0,     0,     0,     0, 
   28,    28,    28,    28,    28,    28,    28,    28,    28,    28, 
   28,    28,    28,    28,    28,    28,    28,    28,    28,    28, 
   28,    28,    28,    28,    28,    28, 
};
static	yyStateRange	yyEobTrans	[yyDStateCount + 1] = { 0,
    0,     0,     7,     7,     0,     0,     7,     0,     0,    10, 
    0,     0,     0,     0,     0,     0,     0,     0,     0,     0, 
    0,     0,     0,     0,     0,     0,     0,     0,     0,     0, 
    0,     0,     0,     0,     7,     0,     7,     0,     0,     0, 
    0,     0,     0,     0,     0,     0,     0,     0,     0,     0, 
    0,     0,     0,     0,     0,     0,     0,     0,     0,     0, 
    0,     0,     0,     0,     0,     0,     0,     0,     0,     0, 
    0,     0,     0,     0,     0,     0, 
};

# if l_scan_xxMaxCharacter < 256
#  define yyGetLine	l_scan_GetLine
# else
#  define yyGetLine	l_scan_GetWLine
# endif

# if yyToClassArraySize == 0
#  define yyToClass(x) (x)
# else
typedef	unsigned short	yytCharClass;

static	yytCharClass	yyToClassArray	[yyToClassArraySize] = {
};
#  if l_scan_xxMaxCharacter < yyToClassArraySize
#   define yyToClass(x) (yyToClassArray [x])
#  else
#   define yyToClass(x) \
	((x) < yyToClassArraySize ? yyToClassArray [x] : yyToClassFunction (x))

static	yytusChar	yyToClassUpb	[yyToClassRangeSize] = {
};
static	yytCharClass	yyToClassClass	[yyToClassRangeSize] = {
};

static yytCharClass yyToClassFunction
# ifdef HAVE_ARGS
   (yytusChar yyCh)
# else
   (yyCh) yytusChar yyCh;
# endif
{
   register int yyLower = 1, yyUpper = yyToClassRangeSize;
   while (yyUpper - yyLower > 1) {			/* binary search */
      register int yyMiddle = (yyLower + yyUpper) / 2;
      if (yyToClassUpb [yyMiddle] < yyCh)
	 yyLower = yyMiddle;
      else
	 yyUpper = yyMiddle;
   }
   if (yyToClassUpb [yyLower - 1] < yyCh && yyCh <= yyToClassUpb [yyLower])
      return yyToClassClass [yyLower];
   else
      return yyToClassClass [yyUpper];
}

#  endif
# endif

static	yyStateRange	yyStartState	= STD;
static	yyStateRange	yyPreviousStart	= STD;
static	int		yySourceFile	= StdIn;
static	rbool		yyEof		= rfalse;
static	long		yyBytesRead	= 0;
static	long		yyFileOffset	= 0;
static	unsigned int	yyLineCount	= 1;
static	yytusChar *	yyLineStart	;
static	yytChar *	yyChBufferStart2;

					/* Start State Stack: StStSt	*/

# if defined xxyyPush | defined xxyyPop
# define		yyInitStStStackSize	16

static	yyStateRange *	yyStStStackPtr	;
static	unsigned long	yyStStStackSize	= 0;
static	unsigned int	yyStStStackIdx	= 0;
# endif

# ifdef xxyyPush
static void yyPush
# ifdef HAVE_ARGS
   (yyStateRange yyState)
# else
   (yyState) yyStateRange yyState;
# endif
{
   if (yyStStStackIdx == yyStStStackSize) {
      if (yyStStStackSize == 0) {
	 yyStStStackSize = yyInitStStStackSize;
	 MakeArray ((char * *) & yyStStStackPtr, & yyStStStackSize,
			(unsigned long) sizeof (yyStateRange));
      } else {
	 ExtendArray ((char * *) & yyStStStackPtr, & yyStStStackSize,
			(unsigned long) sizeof (yyStateRange));
      }
      if (yyStStStackPtr == NULL) yyErrorMessage (xxScannerOutOfMemory);
   }
   yyStStStackPtr [yyStStStackIdx ++] = yyStartState;
   yyStart (yyState);
}
# endif

# ifdef xxyyPop
static void yyPop ARGS ((void))
{
   yyPreviousStart = yyStartState;
   if (yyStStStackIdx > 0)
      yyStartState = yyStStStackPtr [-- yyStStStackIdx];
   else
      yyErrorMessage (xxStartStackUnderflow);
}
# endif

static void yyTab1	ARGS ((int yya));

# define yyTab		yyTab1 (0)
# define yyTab2(a,b)	yyTab1 (a)

/* line 47 "expr.rex" */

	# include <stdlib.h>
	# include "rString.h"

/* line 429 "l_scan.c" */

# ifndef yySetPosition
# define yySetPosition l_scan_Attribute.Position.Line = yyLineCount; \
l_scan_Attribute.Position.Column = (int) ((yytusChar *) l_scan_TokenPtr - yyLineStart);
# endif

# undef yyTab
# undef yyTab2

# ifndef yyInitBufferSize
# define yyInitBufferSize	1024 * 8 + 256
# endif
# ifndef yyInitFileStackSize
# define yyInitFileStackSize	8
# endif
# ifndef yyTabSpace
# define yyTabSpace		8
# endif

static void yyTab1
# ifdef HAVE_ARGS
   (int yya)
# else
   (yya) int yya;
# endif
   { yyLineStart -= (yyTabSpace - 1 - ((yytusChar *) l_scan_TokenPtr -
	yyLineStart + yya - 1)) & (yyTabSpace - 1); }

# define yyTab		yyLineStart -= (yyTabSpace - 1 - \
((yytusChar *) l_scan_TokenPtr - yyLineStart - 1)) & (yyTabSpace - 1)
# define yyTab1(a)	yyLineStart -= (yyTabSpace - 1 - \
((yytusChar *) l_scan_TokenPtr - yyLineStart + (a) - 1)) & (yyTabSpace - 1)
# define yyTab2(a,b)	yyLineStart -= (yyTabSpace - 1 - \
((yytusChar *) l_scan_TokenPtr - yyLineStart + (a) - 1)) & (yyTabSpace - 1)

# ifndef EBCDIC
#  if l_scan_xxMaxCharacter < 256
#   include <ctype.h>
#   define yyToUpper(x)	toupper (x)
#   define yyToLower(x)	tolower (x)
#  else
#   include <wctype.h>
#   define yyToUpper(x)	towupper (x)
#   define yyToLower(x)	towlower (x)
#  endif
# else
#  define yyToLower(x)	yyToLowerArray [x]
#  define yyToUpper(x)	yyToUpperArray [x]

# ifdef xxGetLower
static	yytusChar	yyToLowerArray	[] = {
'\x00', '\x01', '\x02', '\x03', '\x04', '\x05', '\x06', '\x07',
'\x08', '\x09', '\x0A', '\x0B', '\x0C', '\x0D', '\x0E', '\x0F',
'\x10', '\x11', '\x12', '\x13', '\x14', '\x15', '\x16', '\x17',
'\x18', '\x19', '\x1A', '\x1B', '\x1C', '\x1D', '\x1E', '\x1F',
'\x20', '\x21', '\x22', '\x23', '\x24', '\x25', '\x26', '\x27',
'\x28', '\x29', '\x2A', '\x2B', '\x2C', '\x2D', '\x2E', '\x2F',
'\x30', '\x31', '\x32', '\x33', '\x34', '\x35', '\x36', '\x37',
'\x38', '\x39', '\x3A', '\x3B', '\x3C', '\x3D', '\x3E', '\x3F',
'\x40', '\x41', '\x42', '\x43', '\x44', '\x45', '\x46', '\x47',
'\x48', '\x49', '\x4A', '\x4B', '\x4C', '\x4D', '\x4E', '\x4F',
'\x50', '\x51', '\x52', '\x53', '\x54', '\x55', '\x56', '\x57',
'\x58', '\x59', '\x5A', '\x5B', '\x5C', '\x5D', '\x5E', '\x5F',
'\x60', '\x61', '\x62', '\x63', '\x64', '\x65', '\x66', '\x67',
'\x68', '\x69', '\x6A', '\x6B', '\x6C', '\x6D', '\x6E', '\x6F',
'\x70', '\x71', '\x72', '\x73', '\x74', '\x75', '\x76', '\x77',
'\x78', '\x79', '\x7A', '\x7B', '\x7C', '\x7D', '\x7E', '\x7F',
'\x80',    'a',    'b',    'c',    'd',    'e',    'f',    'g',
   'h',    'i', '\x8A', '\x8B', '\x8C', '\x8D', '\x8E', '\x8F',
'\x90',    'j',    'k',    'l',    'm',    'n',    'o',    'p',
   'q',    'r', '\x9A', '\x9B', '\x9C', '\x9D', '\x9E', '\x9F',
'\xA0', '\xA1',    's',    't',   'u',     'v',    'w',    'x',
   'y',    'z', '\xAA', '\xAB', '\xAC', '\xAD', '\xAE', '\xAF',
'\xB0', '\xB1', '\xB2', '\xB3', '\xB4', '\xB5', '\xB6', '\xB7',
'\xB8', '\xB9', '\xBA', '\xBB', '\xBC', '\xBD', '\xBE', '\xBF',
'\xC0',    'a',    'b',    'c',    'd',    'e',    'f',    'g',
   'h',    'i', '\xCA', '\xCB', '\xCC', '\xCD', '\xCE', '\xCF',
'\xD0',    'j',    'k',    'l',    'm',    'n',    'o',    'p',
   'q',    'r', '\xDA', '\xDB', '\xDC', '\xDD', '\xDE', '\xDF',
'\xE0', '\xE1',    's',    't',    'u',    'v',    'w',    'x',
   'y',    'z', '\xEA', '\xEB', '\xEC', '\xED', '\xEE', '\xEF',
'\xF0', '\xF1', '\xF2', '\xF3', '\xF4', '\xF5', '\xF6', '\xF7',
'\xF8', '\xF9', '\xFA', '\xFB', '\xFC', '\xFD', '\xFE', '\xFF'
};
# endif

# ifdef xxGetUpper
static	yytusChar	yyToUpperArray	[] = {
'\x00', '\x01', '\x02', '\x03', '\x04', '\x05', '\x06', '\x07',
'\x08', '\x09', '\x0A', '\x0B', '\x0C', '\x0D', '\x0E', '\x0F',
'\x10', '\x11', '\x12', '\x13', '\x14', '\x15', '\x16', '\x17',
'\x18', '\x19', '\x1A', '\x1B', '\x1C', '\x1D', '\x1E', '\x1F',
'\x20', '\x21', '\x22', '\x23', '\x24', '\x25', '\x26', '\x27',
'\x28', '\x29', '\x2A', '\x2B', '\x2C', '\x2D', '\x2E', '\x2F',
'\x30', '\x31', '\x32', '\x33', '\x34', '\x35', '\x36', '\x37',
'\x38', '\x39', '\x3A', '\x3B', '\x3C', '\x3D', '\x3E', '\x3F',
'\x40', '\x41', '\x42', '\x43', '\x44', '\x45', '\x46', '\x47',
'\x48', '\x49', '\x4A', '\x4B', '\x4C', '\x4D', '\x4E', '\x4F',
'\x50', '\x51', '\x52', '\x53', '\x54', '\x55', '\x56', '\x57',
'\x58', '\x59', '\x5A', '\x5B', '\x5C', '\x5D', '\x5E', '\x5F',
'\x60', '\x61', '\x62', '\x63', '\x64', '\x65', '\x66', '\x67',
'\x68', '\x69', '\x6A', '\x6B', '\x6C', '\x6D', '\x6E', '\x6F',
'\x70', '\x71', '\x72', '\x73', '\x74', '\x75', '\x76', '\x77',
'\x78', '\x79', '\x7A', '\x7B', '\x7C', '\x7D', '\x7E', '\x7F',
'\x80',    'A',    'B',    'C',    'D',    'E',    'F',    'G',
   'H',    'I', '\x8A', '\x8B', '\x8C', '\x8D', '\x8E', '\x8F',
'\x90',    'J',    'K',    'L',    'M',    'N',    'O',    'P',
   'Q',    'R', '\x9A', '\x9B', '\x9C', '\x9D', '\x9E', '\x9F',
'\xA0', '\xA1',    'S',    'T',    'U',    'V',    'W',    'X',
   'Y',    'Z', '\xAA', '\xAB', '\xAC', '\xAD', '\xAE', '\xAF',
'\xB0', '\xB1', '\xB2', '\xB3', '\xB4', '\xB5', '\xB6', '\xB7',
'\xB8', '\xB9', '\xBA', '\xBB', '\xBC', '\xBD', '\xBE', '\xBF',
'\xC0',    'A',    'B',    'C',    'D',    'E',    'F',    'G',
   'H',    'I', '\xCA', '\xCB', '\xCC', '\xCD', '\xCE', '\xCF',
'\xD0',    'J',    'K',    'L',    'M',    'N',    'O',    'P',
   'Q',    'R', '\xDA', '\xDB', '\xDC', '\xDD', '\xDE', '\xDF',
'\xE0', '\xE1',    'S',    'T',    'U',    'V',    'W',    'X',
   'Y',    'Z', '\xEA', '\xEB', '\xEC', '\xED', '\xEE', '\xEF',
'\xF0', '\xF1', '\xF2', '\xF3', '\xF4', '\xF5', '\xF6', '\xF7',
'\xF8', '\xF9', '\xFA', '\xFB', '\xFC', '\xFD', '\xFE', '\xFF'
};
# endif

# endif

static	yyStateRange	yyInitStateStack [4] = { yyDefaultState };
static	yyStateRange *	yyStateStack	= yyInitStateStack;
static	unsigned long	yyStateStackSize= 0;

static	yytusChar	yyInitChBuffer [] = {
   '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0',
   '\0', '\0', '\0', '\0', '\0', '\0', '\0', yyEolCh, yyEobCh, '\0', };
static	yytusChar *	yyChBufferPtr	= yyInitChBuffer;
static	unsigned long	yyChBufferSize	= 0;
static	yytusChar *	yyChBufferStart	= & yyInitChBuffer [16];
static	yytusChar *	yyChBufferIndex	= & yyInitChBuffer [16];

# if yyInitFileStackSize != 0
typedef	struct {
	int		yySourceFile	;
	rbool		yyEof		;
	yytusChar *	yyChBufferPtr	;
	yytusChar *	yyChBufferStart	;
	unsigned long	yyChBufferSize	;
	yytusChar *	yyChBufferIndex	;
	long		yyBytesRead	;
	long		yyFileOffset	;
	unsigned int	yyLineCount	;
	yytusChar *	yyLineStart	;
	}		yytFileStack	;

static	yytFileStack *	yyFileStack	;
static	unsigned long	yyFileStackSize	= 0;
static	yytFileStack *	yyFileStackPtr	;
# endif

int l_scan_GetToken ARGS ((void))
{
   register	yyStateRange	yyState;
   register	yyStateRange *	yyStatePtr;
   register	yytusChar *	yyChBufferIndexReg;
   register	yyCombType * *	yyBasePtrReg = yyBasePtr;
/* line 52 "expr.rex" */

	/* user-defined local variables of the generated GetToken routine */
	# define MAX_STRING_LEN 2048
	char string [MAX_STRING_LEN+1];
	int len;
	
	/* Variable zur Ueberpruefung auf korrekt geklammerte Ausdruecke */
	int bracketCounter = 0;

/* line 602 "l_scan.c" */

yyBegin:
   yyState		= yyStartState;		/* initialize */
   yyStatePtr		= & yyStateStack [1];
   yyChBufferIndexReg	= yyChBufferIndex;
   l_scan_TokenPtr		= (yytChar *) yyChBufferIndexReg;
   if (yyChBufferIndexReg [-1] == yyEolCh) yyState ++;

   /* ASSERT yyChBuffer [yyChBufferIndex] == first character */

yyContinue:		/* continue after sentinel or skipping blanks */
   for (;;) {		/* execute as many state transitions as possible */
			/* determine next state and get next character */
      register yyCombType * yyTablePtr =
		      yyBasePtrReg [yyState] + yyToClass (* yyChBufferIndexReg);
      if (yyTablePtr->yyCheck == yyState) {
	 yyState = yyTablePtr->yyNext;		/* determine next state */
	 * yyStatePtr ++ = yyState;		/* push state */
	 yyChBufferIndexReg ++;			/* get next character */
	 goto yyContinue;
      }
      if ((yyState = yyDefault [yyState]) == yyDNoState) break;
   }

   for (;;) {				/* search for last final state */
      l_scan_TokenLength =
	    (int) (yyChBufferIndexReg - (yytusChar *) l_scan_TokenPtr);
      yyChBufferIndex = yyChBufferIndexReg;
switch (* -- yyStatePtr) {
case 8:;
yySetPosition
/* line 106 "expr.rex" */
{
		l_scan_Attribute.int_const.Value = malloc(l_scan_TokenLength+1);
		l_scan_GetWord(l_scan_Attribute.int_const.Value);
		return tok_int_const;
	
/* line 640 "l_scan.c" */
} goto yyBegin;
case 11:;
case 21:;
yySetPosition
/* line 115 "expr.rex" */
{
		l_scan_Attribute.float_const.Value = malloc(l_scan_TokenLength+1);
		l_scan_GetWord(l_scan_Attribute.float_const.Value);
		return tok_float_const;
	
/* line 651 "l_scan.c" */
} goto yyBegin;
case 13:;
yySetPosition
/* line 121 "expr.rex" */
{return tok_BEGIN;
/* line 657 "l_scan.c" */
} goto yyBegin;
case 15:;
yySetPosition
/* line 122 "expr.rex" */
{return tok_END;
/* line 663 "l_scan.c" */
} goto yyBegin;
case 76:;
yySetPosition
/* line 123 "expr.rex" */
{return tok_PROCEDURE;
/* line 669 "l_scan.c" */
} goto yyBegin;
case 67:;
yySetPosition
/* line 124 "expr.rex" */
{return tok_CASE;
/* line 675 "l_scan.c" */
} goto yyBegin;
case 63:;
yySetPosition
/* line 126 "expr.rex" */
{return tok_IF;
/* line 681 "l_scan.c" */
} goto yyBegin;
case 14:;
yySetPosition
/* line 127 "expr.rex" */
{return tok_THEN;
/* line 687 "l_scan.c" */
} goto yyBegin;
case 61:;
yySetPosition
/* line 128 "expr.rex" */
{return tok_ELSE;
/* line 693 "l_scan.c" */
} goto yyBegin;
case 58:;
yySetPosition
/* line 129 "expr.rex" */
{return tok_FOR;
/* line 699 "l_scan.c" */
} goto yyBegin;
case 55:;
yySetPosition
/* line 130 "expr.rex" */
{return tok_WHILE;
/* line 705 "l_scan.c" */
} goto yyBegin;
case 50:;
yySetPosition
/* line 132 "expr.rex" */
{return tok_PLUS;
/* line 711 "l_scan.c" */
} goto yyBegin;
case 49:;
yySetPosition
/* line 133 "expr.rex" */
{return tok_EQUALS;
/* line 717 "l_scan.c" */
} goto yyBegin;
case 20:;
case 48:;
yySetPosition
/* line 135 "expr.rex" */
{return tok_MINUS;
/* line 724 "l_scan.c" */
} goto yyBegin;
case 47:;
yySetPosition
/* line 136 "expr.rex" */
{return tok_DIVIDE;
/* line 730 "l_scan.c" */
} goto yyBegin;
case 46:;
yySetPosition
/* line 137 "expr.rex" */
{return tok_MULT;
/* line 736 "l_scan.c" */
} goto yyBegin;
case 45:;
yySetPosition
/* line 138 "expr.rex" */
{return tok_GT;
/* line 742 "l_scan.c" */
} goto yyBegin;
case 44:;
yySetPosition
/* line 139 "expr.rex" */
{return tok_LT;
/* line 748 "l_scan.c" */
} goto yyBegin;
case 43:;
yySetPosition
/* line 143 "expr.rex" */
{
		bracketCounter = bracketCounter+1;
		return tok_BRACKETOP;
	
/* line 757 "l_scan.c" */
} goto yyBegin;
case 42:;
yySetPosition
/* line 149 "expr.rex" */
{
		bracketCounter = bracketCounter-1;
		return tok_BRACKETCL;
	
/* line 766 "l_scan.c" */
} goto yyBegin;
case 9:;
case 16:;
case 17:;
case 18:;
case 19:;
case 25:;
case 26:;
case 27:;
case 28:;
case 29:;
case 51:;
case 52:;
case 53:;
case 54:;
case 56:;
case 57:;
case 59:;
case 60:;
case 62:;
case 64:;
case 65:;
case 66:;
case 68:;
case 69:;
case 70:;
case 71:;
case 72:;
case 73:;
case 74:;
case 75:;
yySetPosition
/* line 156 "expr.rex" */
{
		l_scan_Attribute.identifier.Value = malloc(l_scan_TokenLength+1);
		l_scan_GetWord(l_scan_Attribute.identifier.Value);
		return tok_identifier;
	
/* line 805 "l_scan.c" */
} goto yyBegin;
case 12:;
yyChBufferIndex -= 1;
l_scan_TokenLength -= 1;
yySetPosition
/* line 164 "expr.rex" */
{ 
		WritePosition(stdout, l_scan_Attribute.Position);
		printf(" -- comment\n");
	
/* line 816 "l_scan.c" */
} goto yyBegin;
case 41:;
yySetPosition
/* line 171 "expr.rex" */
{ 
		yyStart(STRING);
		len = 0;
	
/* line 825 "l_scan.c" */
} goto yyBegin;
case 7:;
case 35:;
case 37:;
yySetPosition
/* line 176 "expr.rex" */
{
		if(len + l_scan_TokenLength+1 >= MAX_STRING_LEN){
			WritePosition(stderr, l_scan_Attribute.Position);
			len = 0;
		}
		len += l_scan_GetWord(&string[len]);
	
/* line 839 "l_scan.c" */
} goto yyBegin;
case 40:;
yySetPosition
/* line 184 "expr.rex" */
{ 
		yyStart(STD);
		string[len] = '\0';
		l_scan_Attribute.string_const.Value = malloc(len);
		strcpy(l_scan_Attribute.string_const.Value, string);
		return tok_string_const;
	
/* line 851 "l_scan.c" */
} goto yyBegin;
case 39:;
yySetPosition
/* line 192 "expr.rex" */
{ 
		if(len + 1 >= MAX_STRING_LEN){
			WritePosition(stderr, l_scan_Attribute.Position);
			fprintf(stderr, " tl\n");
		}
		string[len++] = '\n';
	
/* line 863 "l_scan.c" */
} goto yyBegin;
case 38:;
yySetPosition
/* line 200 "expr.rex" */
{
		if(len + 1 >= MAX_STRING_LEN){
			WritePosition(stderr, l_scan_Attribute.Position);
			fprintf(stderr, " tl\n");
		}
		string[len++] = '\\';
	
/* line 875 "l_scan.c" */
} goto yyBegin;
case 36:;
{/* BlankAction */
while (* yyChBufferIndexReg ++ == ' ') ;
l_scan_TokenPtr = (yytChar *) -- yyChBufferIndexReg;
yyState = yyStartState;
yyStatePtr = & yyStateStack [1];
goto yyContinue;
/* line 884 "l_scan.c" */
} goto yyBegin;
case 34:;
{/* TabAction */
yyTab;
/* line 889 "l_scan.c" */
} goto yyBegin;
case 33:;
{/* EolAction */
yyEol (0);
/* line 894 "l_scan.c" */
} goto yyBegin;
case 1:;
case 2:;
case 3:;
case 4:;
case 5:;
case 6:;
case 10:;
case 22:;
case 23:;
case 24:;
case 30:;
	 /* non final states */
	 yyChBufferIndexReg --;			/* return character */
	 break;

case 32:
	 yySetPosition
      l_scan_TokenLength   = 1;
	 yyChBufferIndex = ++ yyChBufferIndexReg;
	 {
/* line 62 "expr.rex" */

	/* What happens if no scanner rule matches the input */
	WritePosition(stderr, l_scan_Attribute.Position);
	fprintf(stderr, " Illegal character [%c]\n", *l_scan_TokenPtr);

/* line 922 "l_scan.c" */
	 }
	 goto yyBegin;

      case yyDNoState:
	 goto yyBegin;

case 31:
	 yyChBufferIndex = -- yyChBufferIndexReg; /* undo last state transit */
	 if (-- l_scan_TokenLength == 0) {		/* get previous state */
	    yyState = yyStartState;
  	    if (yyChBufferIndexReg [-1] == yyEolCh) yyState ++;
	 } else {
	    yyState = * (yyStatePtr - 1);
	 }

	 if (yyChBufferIndex != & yyChBufferStart [yyBytesRead]) {
					/* end of buffer sentinel in buffer */
	    if ((yyState = yyEobTrans [yyState]) == yyDNoState) continue;
	    yyChBufferIndexReg ++;
	    * yyStatePtr ++ = yyState;		/* push state */
	    goto yyContinue;
	 }
						/* end of buffer reached */
	 if (! yyEof) {
	    register yytChar * yySource;
	    register yytChar * yyTarget;
	    unsigned long yyChBufferFree;

	    if (yyChBufferSize == 0) {
	       yyStateRange * yyOldStateStack = yyStateStack;
	       yyInitialize ();
	       yyStatePtr += yyStateStack - yyOldStateStack;
	       yyChBufferIndexReg = yyChBufferIndex;
	    }
	    yySource = l_scan_TokenPtr - 1;
	    yyTarget = (yytChar *) & yyChBufferPtr
		[(yyMaxAlign - 1 - l_scan_TokenLength) & (yyMaxAlign - 1)];
	    yyChBufferFree = Exp2 (Log2 (yyChBufferSize - 4 -
		yyMaxAlign - l_scan_TokenLength));
		/* copy initial part of token in front of the input buffer */
	    if (yySource > yyTarget) {
	       l_scan_TokenPtr = yyTarget + 1;
	       do * yyTarget ++ = * yySource ++;
	       while (yySource < (yytChar *) yyChBufferIndexReg);
	       yyLineStart += (yytusChar *) yyTarget - yyChBufferStart -
				yyBytesRead;
	       yyChBufferStart = (yytusChar *) yyTarget;
	    } else {
	       yyChBufferStart = yyChBufferIndexReg;
	    }
	    yyChBufferStart2 = (yytChar *) yyChBufferStart;
						/* extend buffer if necessary */
	    if (yyChBufferFree < yyChBufferSize >> 3 /* / 8 */ ) {
	       register ptrdiff_t yyDelta;
	       register yytusChar * yyOldChBufferPtr = yyChBufferPtr;
	       ExtendArray ((char * *) & yyChBufferPtr, & yyChBufferSize,
				(unsigned long) sizeof (yytChar));
	       if (yyChBufferPtr == NULL) yyErrorMessage (xxScannerOutOfMemory);
	       yyDelta = yyChBufferPtr - yyOldChBufferPtr;
	       yyChBufferStart	+= yyDelta;
	       yyLineStart	+= yyDelta;
	       l_scan_TokenPtr	+= yyDelta;
	       yyChBufferStart2	 = (yytChar *) yyChBufferStart;
	       yyChBufferFree = Exp2 (Log2 (yyChBufferSize - 4 -
			yyMaxAlign - l_scan_TokenLength));
	       if (yyStateStackSize < yyChBufferSize) {
		  yyStateRange * yyOldStateStack = yyStateStack;
		  ExtendArray ((char * *) & yyStateStack, & yyStateStackSize,
				   (unsigned long) sizeof (yyStateRange));
		  if (yyStateStack == NULL)
		     yyErrorMessage (xxScannerOutOfMemory);
		  yyStatePtr	+= yyStateStack - yyOldStateStack;
	       }
	    }
						/* read buffer and restart */
	    yyChBufferIndex = yyChBufferIndexReg = yyChBufferStart;
	    yyFileOffset += yyBytesRead;
	    yyBytesRead = yyGetLine (yySourceFile, (yytChar *) yyChBufferIndex,
	       (int) yyChBufferFree);
	    if (yyBytesRead <= 0) { yyBytesRead = 0; yyEof = rtrue; }
	    yyChBufferStart [yyBytesRead    ] = yyEobCh;
	    yyChBufferStart [yyBytesRead + 1] = '\0';
	    goto yyContinue;
	 }

	 if (l_scan_TokenLength == 0) {		/* end of file reached */
	    if (yyChBufferSize == 0) return l_scan_EofToken;
	    yySetPosition
	    l_scan_CloseFile ();
# if yyInitFileStackSize != 0
	    if (yyFileStackPtr == yyFileStack) {
/* line 68 "expr.rex" */

	/* What should be done if the end-of-input-file has been reached? */

	/* E.g.: check hat strings and comments are closed. */
	if (bracketCounter > 1) {
		printf(" Klammerung falsch");
	}
	
	switch(yyStartState){
		case STD:
			
		
			break;
		case STRING:
			WritePosition(stderr, l_scan_Attribute.Position);
			fprintf(stderr, " Anfuehrungszeichen am Ende Fehlen\n");
		break;
		
		default:
			Message("OOPS: that should not happen!!",
				 xxFatal, l_scan_Attribute.Position);
			break;
	}

	/* implicit: return the EofToken */

/* line 1041 "l_scan.c" */
	    }
	    if (yyFileStackPtr == yyFileStack) {
	       l_scan_ResetScanner ();
	       return l_scan_EofToken;
	    }
	    goto yyBegin;
# else
/* line 68 "expr.rex" */

	/* What should be done if the end-of-input-file has been reached? */

	/* E.g.: check hat strings and comments are closed. */
	if (bracketCounter > 1) {
		printf(" Klammerung falsch");
	}
	
	switch(yyStartState){
		case STD:
			
		
			break;
		case STRING:
			WritePosition(stderr, l_scan_Attribute.Position);
			fprintf(stderr, " Anfuehrungszeichen am Ende Fehlen\n");
		break;
		
		default:
			Message("OOPS: that should not happen!!",
				 xxFatal, l_scan_Attribute.Position);
			break;
	}

	/* implicit: return the EofToken */

/* line 1076 "l_scan.c" */
	    l_scan_ResetScanner ();
	    return l_scan_EofToken;
# endif
	 }
	 break;

      default:
	 yyErrorMessage (xxScannerInternalError);
      }
   }
}

static void yyInitialize ARGS ((void))
   {
      if (yyStateStackSize == 0) {
	 yyStateStackSize = yyInitBufferSize;
	 MakeArray ((char * *) & yyStateStack, & yyStateStackSize,
		       (unsigned long) sizeof (yyStateRange));
	 if (yyStateStack == NULL) yyErrorMessage (xxScannerOutOfMemory);
	 yyStateStack [0] = yyDefaultState;
      }
# if yyInitFileStackSize != 0
      if (yyFileStackSize == 0) {
	 yyFileStackSize = yyInitFileStackSize;
	 MakeArray ((char * *) & yyFileStack, & yyFileStackSize,
			(unsigned long) sizeof (yytFileStack));
	 if (yyFileStack == NULL) yyErrorMessage (xxScannerOutOfMemory);
	 yyFileStackPtr = yyFileStack;
      }

      if (yyFileStackPtr >= yyFileStack + yyFileStackSize - 1) {
	 ptrdiff_t yyyFileStackPtr = yyFileStackPtr - yyFileStack;
	 ExtendArray ((char * *) & yyFileStack, & yyFileStackSize,
			   (unsigned long) sizeof (yytFileStack));
	 if (yyFileStack == NULL) yyErrorMessage (xxScannerOutOfMemory);
	 yyFileStackPtr = yyFileStack + yyyFileStackPtr;
      }
      yyFileStackPtr ++;			/* push file */
      yyFileStackPtr->yySourceFile	= yySourceFile		;
      yyFileStackPtr->yyEof		= yyEof			;
      yyFileStackPtr->yyChBufferPtr	= yyChBufferPtr		;
      yyFileStackPtr->yyChBufferStart	= yyChBufferStart	;
      yyFileStackPtr->yyChBufferSize	= yyChBufferSize	;
      yyFileStackPtr->yyChBufferIndex	= yyChBufferIndex	;
      yyFileStackPtr->yyBytesRead	= yyBytesRead		;
      yyFileStackPtr->yyFileOffset	= yyFileOffset		;
      yyFileStackPtr->yyLineCount	= yyLineCount		;
      yyFileStackPtr->yyLineStart	= yyLineStart		;
# endif
						/* initialize file state */
      yyChBufferSize	   = yyInitBufferSize;
      MakeArray ((char * *) & yyChBufferPtr, & yyChBufferSize,
			(unsigned long) sizeof (yytChar));
      if (yyChBufferPtr == NULL) yyErrorMessage (xxScannerOutOfMemory);
      yyChBufferStart	   = & yyChBufferPtr [yyMaxAlign];
      yyChBufferStart2	   = (yytChar *) yyChBufferStart;
      yyChBufferStart [-1] = yyEolCh;		/* begin of line indicator */
      yyChBufferStart [ 0] = yyEobCh;		/* end of buffer sentinel */
      yyChBufferStart [ 1] = '\0';
      yyChBufferIndex	   = yyChBufferStart;
      l_scan_TokenPtr	   = (yytChar *) yyChBufferStart;
      yyEof		   = rfalse;
      yyBytesRead	   = 0;
      yyFileOffset	   = 0;
      yyLineCount	   = 1;
      yyLineStart	   = & yyChBufferStart [-1];
# ifdef HAVE_FILE_NAME
      if (l_scan_Attribute.Position.FileName == 0)
	 l_scan_Attribute.Position.FileName = 1; /* NoIdent */
# endif
   }

void l_scan_BeginFile
# ifdef HAVE_ARGS
   (char * yyFileName)
# else
   (yyFileName) char * yyFileName;
# endif
   {
      yyInitialize ();
      yySourceFile = yyFileName == NULL ? StdIn :
# ifdef SOURCE_VER
	 l_scan_BeginSourceFile (yyFileName);
# else
	 l_scan_BeginSource (yyFileName);
# endif
      if (yySourceFile < 0) yyErrorMessage (xxCannotOpenInputFile);
   }

# ifdef SOURCE_VER

# if HAVE_WCHAR_T

void l_scan_BeginFileW
# ifdef HAVE_ARGS
   (wchar_t * yyFileName)
# else
   (yyFileName) wchar_t * yyFileName;
# endif
   {
      yyInitialize ();
      yySourceFile = yyFileName == NULL ? StdIn :
	 l_scan_BeginSourceFileW (yyFileName);
      if (yySourceFile < 0) yyErrorMessage (xxCannotOpenInputFile);
   }

# endif

void l_scan_BeginMemory
# ifdef HAVE_ARGS
   (void * yyInputPtr)
# else
   (yyInputPtr) void * yyInputPtr;
# endif
   {
      yyInitialize ();
      l_scan_BeginSourceMemory (yyInputPtr);
   }

void l_scan_BeginMemoryN
# ifdef HAVE_ARGS
   (void * yyInputPtr, int yyLength)
# else
   (yyInputPtr, yyLength) void * yyInputPtr; int yyLength;
# endif
   {
      yyInitialize ();
      l_scan_BeginSourceMemoryN (yyInputPtr, yyLength);
   }

void l_scan_BeginGeneric
# ifdef HAVE_ARGS
   (void * yyInputPtr)
# else
   (yyInputPtr) void * yyInputPtr;
# endif
   {
      yyInitialize ();
      l_scan_BeginSourceGeneric (yyInputPtr);
   }

# endif

void l_scan_CloseFile ARGS ((void))
   {
# if yyInitFileStackSize != 0
      if (yyFileStackPtr == yyFileStack) yyErrorMessage (xxFileStackUnderflow);
# endif
      l_scan_CloseSource (yySourceFile);
      ReleaseArray ((char * *) & yyChBufferPtr, & yyChBufferSize,
			(unsigned long) sizeof (yytChar));
# if yyInitFileStackSize != 0
						/* pop file */
      yySourceFile	= yyFileStackPtr->yySourceFile		;
      yyEof		= yyFileStackPtr->yyEof			;
      yyChBufferPtr	= yyFileStackPtr->yyChBufferPtr		;
      yyChBufferStart	= yyFileStackPtr->yyChBufferStart	;
      yyChBufferStart2	= (yytChar *) yyChBufferStart		;
      yyChBufferSize	= yyFileStackPtr->yyChBufferSize	;
      yyChBufferIndex	= yyFileStackPtr->yyChBufferIndex	;
      yyBytesRead	= yyFileStackPtr->yyBytesRead		;
      yyFileOffset	= yyFileStackPtr->yyFileOffset		;
      yyLineCount	= yyFileStackPtr->yyLineCount		;
      yyLineStart	= yyFileStackPtr->yyLineStart		;
      yyFileStackPtr --;
# endif
   }

# ifdef xxGetWord
int l_scan_GetWord
# ifdef HAVE_ARGS
   (yytChar * yyWord)
# else
   (yyWord) yytChar * yyWord;
# endif
   {
      register yytChar * yySource		= l_scan_TokenPtr;
      register yytChar * yyTarget		= yyWord;
      register yytChar * yyChBufferIndexReg	= (yytChar *) yyChBufferIndex;

      while (yySource < yyChBufferIndexReg)
	 * yyTarget ++ = * yySource ++;
      * yyTarget = '\0';
      return (int) (yyChBufferIndexReg - l_scan_TokenPtr);
   }
# endif

# ifdef xxGetLower
int l_scan_GetLower
# ifdef HAVE_ARGS
   (yytChar * yyWord)
# else
   (yyWord) yytChar * yyWord;
# endif
   {
      register yytusChar * yySource	= (yytusChar *) l_scan_TokenPtr;
      register yytusChar * yyTarget	= (yytusChar *) yyWord;
      register yytusChar * yyChBufferIndexReg = yyChBufferIndex;

      while (yySource < yyChBufferIndexReg)
	 * yyTarget ++ = yyToLower (* yySource ++);
      * yyTarget = '\0';
      return (int) (yyChBufferIndexReg - (yytusChar *) l_scan_TokenPtr);
   }
# endif

# ifdef xxGetUpper
int l_scan_GetUpper
# ifdef HAVE_ARGS
   (yytChar * yyWord)
# else
   (yyWord) yytChar * yyWord;
# endif
   {
      register yytusChar * yySource	= (yytusChar *) l_scan_TokenPtr;
      register yytusChar * yyTarget	= (yytusChar *) yyWord;
      register yytusChar * yyChBufferIndexReg = yyChBufferIndex;

      while (yySource < yyChBufferIndexReg)
	 * yyTarget ++ = yyToUpper (* yySource ++);
      * yyTarget = '\0';
      return (int) (yyChBufferIndexReg - (yytusChar *) l_scan_TokenPtr);
   }
# endif

# ifdef xxinput
static yytChar input ARGS ((void))
   {
      if (yyChBufferIndex == & yyChBufferStart [yyBytesRead]) {
	 if (! yyEof) {
	    yyLineStart -= yyBytesRead;
	    yyChBufferIndex = yyChBufferStart = & yyChBufferPtr [yyMaxAlign];
	    yyChBufferStart2 = (yytChar *) yyChBufferStart;
	    yyFileOffset += yyBytesRead;
	    yyBytesRead = yyGetLine (yySourceFile, (yytChar *) yyChBufferIndex,
	       (int) Exp2 (Log2 (yyChBufferSize)));
	    if (yyBytesRead <= 0) { yyBytesRead = 0; yyEof = rtrue; }
	    yyChBufferStart [yyBytesRead    ] = yyEobCh;
	    yyChBufferStart [yyBytesRead + 1] = '\0';
	 }
      }
      if (yyChBufferIndex == & yyChBufferStart [yyBytesRead]) return '\0';
      else return * yyChBufferIndex ++;
   }
# endif

static void unput
# ifdef HAVE_ARGS
   (yytChar yyc)
# else
   (yyc) yytChar yyc;
# endif
   { * (-- yyChBufferIndex) = yyc; }

static void yyLess
# ifdef HAVE_ARGS
   (int yyn)
# else
   (yyn) int yyn;
# endif
   { yyChBufferIndex -= l_scan_TokenLength - yyn; l_scan_TokenLength = yyn; }

void l_scan_BeginScanner ARGS ((void))
   {
   }

void l_scan_CloseScanner ARGS ((void))
   {
   }

void l_scan_ResetScanner ARGS ((void))
   {
      yyChBufferPtr	= yyInitChBuffer;
      yyChBufferSize	= 0;
      yyChBufferStart	= & yyInitChBuffer [16];
      yyChBufferIndex	= & yyInitChBuffer [16];
      if (yyStateStackSize != 0)
	 ReleaseArray ((char * *) & yyStateStack, & yyStateStackSize,
			(unsigned long) sizeof (yyStateRange));
      yyStateStack	= yyInitStateStack;
      yyStateStackSize	= 0;
# if yyInitFileStackSize != 0
      if (yyFileStackSize != 0)
	 ReleaseArray ((char * *) & yyFileStack, & yyFileStackSize,
			(unsigned long) sizeof (yytFileStack));
      yyFileStackSize	= 0;
# endif
# if defined xxyyPush | defined xxyyPop
      if (yyStStStackSize != 0)
	 ReleaseArray ((char * *) & yyStStStackPtr, & yyStStStackSize,
			(unsigned long) sizeof (yyStateRange));
      yyStStStackSize	= 0;
      yyStStStackIdx	= 0;
# endif
      yyStartState	= STD;
      yyPreviousStart	= STD;
      yySourceFile	= StdIn;
   }

static void yyErrorMessage
# ifdef HAVE_ARGS
   (int yyErrorCode)
# else
   (yyErrorCode) int yyErrorCode;
# endif
   {
      ErrorMessageI (yyErrorCode, xxFatal, l_scan_Attribute.Position,
	 xxString, "l_scan");
      l_scan_Exit ();
   }
