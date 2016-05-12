/* Project:  COCKTAIL training
 * Descr:    a simple scanner generated with rex
 * Kind:     REX scanner specification (solution)
 * Author:   Dr. Juergen Vollmer <juergen.vollmer@informatik-vollmer.de>
 * $Id: l.rex.in,v 1.10 2014/05/14 08:46:39 vollmer Exp $
 */

SCANNER l_scan

EXPORT {
/* code to be put into Scanner.h */

# include "Position.h"

/* Token Attributes.
 * For each token with user defined attributes, we need a typedef for the
 * token attributes.
 * The first struct-field must be of type tPosition!
 */
typedef struct {tPosition Pos; char* Value;} tint_const;
typedef struct {tPosition Pos; char* Value;} tfloat_const;
typedef struct {tPosition Pos; char* Value;} tidentifier;
typedef struct {tPosition Pos; char* Value;} tstring_const;

/* There is only one "actual" token, during scanning. Therfore
 * we use a UNION of all token-attributes as data type for that unique
 * token-attribute variable.
 * All token (with and without user defined attributes) have one
 * attribute: the source position:
 *     tPosition     Position;
 */
typedef union {
  tPosition     Position;
  tint_const    int_const;
  tfloat_const  float_const;
  tidentifier   identifier;
  tstring_const string_const;
} l_scan_tScanAttribute;

/* Tokens are coded as int's, with values >=0
 * The value 0 is reserved for the EofToken, which is defined automatically
 */
# define tok_int_const    1
# define tok_float_const  2
# define tok_identifier   3
# define tok_string_const 4

/* tokens without special attributes */
# define tok_BEGIN 	  5
# define tok_END 	  6
# define tok_PROCEDURE 	  7
# define tok_CASE	  8
} // EXPORT

GLOBAL {
  # include <stdlib.h>
  # include "rString.h"
} // GLOBAL

LOCAL {
 /* user-defined local variables of the generated GetToken routine */
# define MAX_STRING_LEN 2048
char string [MAX_STRING_LEN+1]; int len;
int NestingLevel = 0;
}  // LOCAL

DEFAULT {
  /* What happens if no scanner rule matches the input */
  WritePosition (stderr, l_scan_Attribute.Position);
  fprintf (stderr, " Illegal character [%c]\n", *l_scan_TokenPtr);
} // DEFAULT

EOF {
  /* What should be done if the end-of-input-file has been reached? */

  /* E.g.: check hat strings and comments are closed. */
  switch (yyStartState) {
  case STD:
    /* ok */
    break;
  case STRING:
    WritePosition (stderr, l_scan_Attribute.Position);
    fprintf (stderr, " Nicht abgeschlossene Stringkonstante\n");
   break;
  case COMMENT:
    WritePosition (stderr, l_scan_Attribute.Position);
    fprintf (stderr, " Nicht abgeschlossener C-Kommentar\n");
    break;
  case COMMENT2:
    WritePosition (stderr, l_scan_Attribute.Position);
    fprintf (stderr, " Nicht abgeschlossener Modula-2 Kommentar\n");
    break;
  default:
    Message ("OOPS: that should not happen!!",
	     xxFatal, l_scan_Attribute.Position);
    break;
  }

  /* implicit: return the EofToken */
} // EOF

DEFINE  /* some abbreviations */
  digit  = {0-9}       .
  letter = {a-z_A-Z}   .
  string = - {"\\\n\r\f} .

/* define start states, note STD is defined by default, separate several states by a comma */
START STRING, COMMENT, COMMENT2

RULE

/* Integers */
#STD# digit+ :
	{l_scan_Attribute.int_const.Value = malloc (l_scan_TokenLength+1);
	 l_scan_GetWord (l_scan_Attribute.int_const.Value);
	 return tok_int_const;
	}

/* Please add rules for: (don't forget to adapt main()) */
/* Float numbers */
#STD# digit + "." digit * (("E"|"e") ("+"|"-") ? digit +) ? :
	{l_scan_Attribute.float_const.Value = malloc (l_scan_TokenLength+1);
	 l_scan_GetWord (l_scan_Attribute.float_const.Value);
	 return tok_float_const;
	}

/* case insensitive keywords: BEGIN PROCEDURE END CASE */
#STD# {Bb} {Ee} {Gg} {Ii} {Nn}                    : {return tok_BEGIN;}
#STD# {Ee} {Nn} {Dd}                              : {return tok_END;}
#STD# {Pp} {Rr} {Oo} {Cc} {Ee} {Dd} {Uu} {Rr} {Ee}: {return tok_PROCEDURE;}
#STD# {Cc} {Aa} {Ss} {Ee}                         : {return tok_CASE;}

/* identifiers */
#STD# letter (letter | digit) * :
	{l_scan_Attribute.identifier.Value = malloc (l_scan_TokenLength+1);
	 l_scan_GetWord (l_scan_Attribute.identifier.Value);
	 return tok_identifier;
	}

/* comment up to end of line */
#STD# < "--" ANY * > :
        { /* comment up to end of line, nothing to do */
	  WritePosition (stdout, l_scan_Attribute.Position);
	  printf (" -- comment\n");
        }

/* C-style comment */
#STD# "/*" :
	{ /**/
	  yyStart (COMMENT);
	  WritePosition (stdout, l_scan_Attribute.Position);
	  printf (" start of a C-comment\n");
	}

#COMMENT# - {*\n\t} + | "*" :
        {}

#COMMENT# "*/" :
        {yyStart (STD);
	 WritePosition (stdout, l_scan_Attribute.Position);
	 printf (" Ende   des C-Kommentars\n");
	}

/* Modula2-style nested comment */
#STD,COMMENT2# "(*" :
	{ yyStart (COMMENT2);
	  NestingLevel ++;
	  WritePosition (stdout, l_scan_Attribute.Position);
	  printf (" start of a Modula-2 comment, nesting level = %d\n", NestingLevel);
	}

#COMMENT2# - {*(\n\t} + | "*" | "(" :
        {}

#COMMENT2# "*)" :
        { WritePosition (stdout, l_scan_Attribute.Position);
	  printf (" end   of a Modula-2 comment, nesting level = %d\n", NestingLevel);
	  NestingLevel --;
	  if (NestingLevel == 0) {
	    yyStart (STD);
	  }
	}

/* double quote delimited strings */
#STD# \" :
	{ /* start of string */
          yyStart (STRING);
	  int len = 0;
	}
#STRING# string* :
	{ /* we're inside the string */
          if (len + l_scan_TokenLength+1 >= MAX_STRING_LEN) {
	    WritePosition (stderr, l_scan_Attribute.Position);
	    fprintf (stderr, " String zu lang\n");
	    len = 0;
	  }
	  len += l_scan_GetWord (&string[len]);
        }
#STRING# \" :
	{ /* end of string */
          yyStart(STD);
	  string[len] = '\0';
	  l_scan_Attribute.string_const.Value = malloc (len);
	  strcpy (l_scan_Attribute.string_const.Value, string);
	  return tok_string_const;
        }
#STRING# \\ \" :
	{ /* transform the \" characters to a single " */
          if (len + 1 >= MAX_STRING_LEN) {
	    WritePosition (stderr, l_scan_Attribute.Position);
	    fprintf (stderr, " String too long\n");
	  }
	  string[len++] = '"';
	}
#STRING# \\ n :
	{ /* transform the \n characters to a single " */
          if (len + 1 >= MAX_STRING_LEN) {
	    WritePosition (stderr, l_scan_Attribute.Position);
	    fprintf (stderr, " String too long\n");
	  }
	  string[len++] = '\n';
	}
#STRING# \\ :
	{ /* special treatment of the \-character */
          if (len + 1 >= MAX_STRING_LEN) {
	    WritePosition (stderr, l_scan_Attribute.Position);
	    fprintf (stderr, " String too long\n");
	  }
	  string[len++] = '\\';
	}
#STRING# \n :
	{ /* error handling of typical errors:
	   *  a string not closed at the end of a line
           */
          yyStart(STD);
	  yyEol (0);
	  WritePosition (stderr, l_scan_Attribute.Position);
	  fprintf (stderr, " String not closed\n");
	  l_scan_Attribute.string_const.Value = "";
          return tok_string_const;
        }
/**********************************************************************/
