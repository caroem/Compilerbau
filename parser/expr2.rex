#@ line 1
/* Project:  COCKTAIL training
 * Descr:    LR parser for an expression language
 * Kind:     Scanner specification
 * Author:   Dr. Juergen Vollmer <Juergen.Vollmer@informatik-vollmer.de>
 * $Id: expr.scan,v 1.1 2007/06/01 11:37:53 vollmer draft $
 */

SCANNER expr_scan

EXPORT {
/* code to be put intp Scanner.h */
# include "Position.h"

/* Token Attributes.
 * For each token with user defined attributes, we need a typedef for the
 * token attributes.
 * LPP extracts the token-attribute declaration from the parser specification.
 * They are inserted here.
 */

#@ line 23
# ifndef CommonScanAttrs
# define CommonScanAttrs
# endif

# define zzCommonScanAttrs tPosition Position; CommonScanAttrs

typedef struct { zzCommonScanAttrs long Value; } zz_expr_scan_int_const;
typedef struct { zzCommonScanAttrs double Value; } zz_expr_scan_float_const;
typedef struct { zzCommonScanAttrs tIdent Value; } zz_expr_scan_string_const;

typedef union {
tPosition Position;
struct { zzCommonScanAttrs } Common;
zz_expr_scan_int_const int_const;
zz_expr_scan_float_const float_const;
zz_expr_scan_string_const string_const;
} expr_scan_tScanAttribute;

# undef zzCommonScanAttrs

extern void expr_scan_ErrorAttribute ARGS((int Token, expr_scan_tScanAttribute * pAttribute));
#@ line 20

}

GLOBAL {
/* code to be put into Scanner.c */
# include <stdlib.h>
# include "Errors.h"

/* Insert the routine computing "error-values" of attributes, in case the
 * parser decides during error repair to insert a token.
 */

#@ line 58
void expr_scan_ErrorAttribute
# ifdef HAVE_ARGS
 (int Token, expr_scan_tScanAttribute * pAttribute)
# else
 (Token, pAttribute) int Token; expr_scan_tScanAttribute * pAttribute;
# endif
{
 pAttribute->Position = expr_scan_Attribute.Position;
 switch (Token) {
 case /* int_const */ 1: 
pAttribute->int_const.Value = 0;
   break;
 case /* float_const */ 2: 
pAttribute->float_const.Value = 0.0;
 break;
 case /* string_const */ 3: 
pAttribute->string_const.Value = NoIdent;
 break;
 }
}
#@ line 31

}

LOCAL {
/* user-defined local variables of the generated GetToken routine */
  # define MAX_STRING_LEN 2048
  char string [MAX_STRING_LEN];
  int stringLength = 0;
  int commentLevel = 0;
}

DEFAULT {
  /* What happens if no scanner rule matches the input */
  MessageI ("Illegal character",
	    xxError, expr_scan_Attribute.Position,
	    xxCharacter, expr_scan_TokenPtr);
}

EOF {
  /* What should be done if the end-of-input-file has been reached? */

  /* E.g.: check hat strings and comments are closed. */
  switch (yyStartState) {
  case STD:
    /* ok */
    break;
  case STRING:
    Message ("unclosed String",
	     xxError, expr_scan_Attribute.Position);
    break;
  case COMMENT:
    Message ("unclosed comment",
	     xxError, expr_scan_Attribute.Position);
    break;
  default:
    Message ("OOPS: that should not happen!!",
	     xxFatal, expr_scan_Attribute.Position);
    break;
  }

  /* implicit: return the EofToken */
}

DEFINE /* some abbreviations */
  digit  = {0-9}       .
  letter = {a-zA-Z}.
  string = - {"\\\n\r\f} .

/* define start states, note STD is defined by default */
START STRING, COMMENT

RULES
/* Comments */
#STD,COMMENT# "<!--" :
{ 
    yyStart (COMMENT);
    commentLevel ++;
}

#COMMENT# ANY :
{
}

#COMMENT# "-->" :
{ 
    commentLevel --;
    if (commentLevel == 0) {
	yyStart (STD);
    }
}

#STD# ("int" | "float" | "string") :
{
    expr_scan_GetWord (string);
    expr_scan_Attribute.key.Value = atol (string);
    return key;
}

/* Integers */
#STD# digit+ :
{
    expr_scan_GetWord (string);
    expr_scan_Attribute.int_const.Value = atol (string);
    return 1;
}

/* Float*/
#STD# digit + "." digit * (("E"|"e") ("+"|"-") ? digit +) ? :
{
    expr_scan_GetWord (string);
    expr_scan_Attribute.float_const.Value = atof (string);
    return 2;
}

/* identifier */
#STD# letter (letter | digit)* :
{ 
    expr_scan_Attribute.identifier.Value =
    MakeIdent (expr_scan_TokenPtr, expr_scan_TokenLength);
    return identifier;
}

/* String */
#STD# \" :
{
    yyStart (STRING);
    stringLength = 0;
}
    
#STRING# string* :
{
    stringLength += expr_scan_TokenLength;
}
    
#STRING# \" :
{
    yyStart(STD);
    string[stringLength] = '\0';
    expr_scan_Attribute.string_const.Value =
    MakeIdent (string, stringLength);
    return 3;
}

#STRING# \\ \" :
{
    string[stringLength++] = '"';
}

