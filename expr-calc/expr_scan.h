# ifndef yyexpr_scan
# define yyexpr_scan

/* $Id: Scanner.h,v 2.18 2005/03/24 17:43:29 grosch rel $ */

# include "config.h"
# if HAVE_WCHAR_H
#  include <wchar.h>
# endif
# include "ratc.h"

/* line 11 "expr.rex" */

/* code to be put intp Scanner.h */
# include "Position.h"

/* Token Attributes.
 * For each token with user defined attributes, we need a typedef for the
 * token attributes.
 * LPP extracts the token-attribute declaration from the parser specification.
 * They are inserted here.
 */

/* line 23 "expr.rex" */
# ifndef CommonScanAttrs
# define CommonScanAttrs
# endif

# define zzCommonScanAttrs tPosition Position; CommonScanAttrs

typedef struct { zzCommonScanAttrs long Value; } zz_expr_scan_int_const;
typedef struct { zzCommonScanAttrs double Value; } zz_expr_scan_float_const;

typedef union {
tPosition Position;
struct { zzCommonScanAttrs } Common;
zz_expr_scan_int_const int_const;
zz_expr_scan_float_const float_const;
} expr_scan_tScanAttribute;

# undef zzCommonScanAttrs

extern void expr_scan_ErrorAttribute ARGS((int Token, expr_scan_tScanAttribute * pAttribute));
/* line 43 "expr.rex" */


/* line 48 "expr_scan.h" */

# define expr_scan_EofToken	0
# define expr_scan_xxMaxCharacter	255

# ifndef expr_scan_xxtChar
#  if expr_scan_xxMaxCharacter < 256
#   define expr_scan_xxtChar	char
#  else
#   define expr_scan_xxtChar	wchar_t
#  endif
# endif

# ifndef expr_scan_xxtusChar
#  if expr_scan_xxMaxCharacter < 256
#   define expr_scan_xxtusChar	unsigned char
#  else
#   define expr_scan_xxtusChar	wchar_t
#  endif
# endif

# ifdef lex_interface
#  define expr_scan_GetToken	yylex
#  define expr_scan_TokenLength	yyleng
# endif

extern expr_scan_xxtChar *	expr_scan_TokenPtr	;
extern int		expr_scan_TokenLength	;
extern expr_scan_tScanAttribute	expr_scan_Attribute	;
extern void		(* expr_scan_Exit) ARGS ((void));

extern void	expr_scan_BeginScanner	ARGS ((void));
extern void	expr_scan_BeginFile	ARGS ((char * yyFileName));
# if HAVE_WCHAR_T
extern void	expr_scan_BeginFileW	ARGS ((wchar_t * yyFileName));
# endif
extern void	expr_scan_BeginMemory	ARGS ((void * yyInputPtr));
extern void	expr_scan_BeginMemoryN	ARGS ((void * yyInputPtr, int yyLength));
extern void	expr_scan_BeginGeneric	ARGS ((void * yyInputPtr));
extern int	expr_scan_GetToken	ARGS ((void));
extern int	expr_scan_GetWord 	ARGS ((expr_scan_xxtChar * yyWord));
extern int	expr_scan_GetLower	ARGS ((expr_scan_xxtChar * yyWord));
extern int	expr_scan_GetUpper	ARGS ((expr_scan_xxtChar * yyWord));
extern void	expr_scan_CloseFile	ARGS ((void));
extern void	expr_scan_CloseScanner	ARGS ((void));
extern void	expr_scan_ResetScanner	ARGS ((void));

# endif
