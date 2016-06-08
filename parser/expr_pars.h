# ifndef yyexpr_pars
# define yyexpr_pars

/* $Id: Parser.h,v 1.8 2002/09/25 14:26:49 grosch rel $ */

# include "ratc.h"

/* line 2 "expr_pars.lrk" */


/* line 12 "expr_pars.h" */
/* line 5 "expr_pars.lrk" */


/* line 16 "expr_pars.h" */

# ifdef yacc_interface
# define expr_pars			yyparse
# define yyInitStackSize	YYMAXDEPTH
# endif
# define rbool		char
# define rtrue		1
# define rfalse		0
					/* named constants for start symbols */
					/* named constants for tokens */

extern	rbool	expr_pars_Debug;
extern	char *	expr_pars_TokenName	[];
extern	void	(* expr_pars_Exit)	ARGS ((void));

extern	void	Beginexpr_pars	ARGS ((void));
extern	int	expr_pars	ARGS ((void));
extern	int	expr_pars2	ARGS ((int yyStartSymbol));
extern	void	Resetexpr_pars	ARGS ((void));
extern	void	Closeexpr_pars	ARGS ((void));

# endif
