#@ line 1
/* Project:  COCKTAIL training
 * Descr:    a simple scanner generated with rex
 * Kind:     REX scanner specification 
 * Author:   Dr. Juergen Vollmer <juergen.vollmer@informatik-vollmer.de>
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
	
} // EXPORT

GLOBAL {
	# include <stdlib.h>
	# include "rString.h"
} // GLOBAL

LOCAL {
	/* user-defined local variables of the generated GetToken routine */
	# define MAX_STRING_LEN 2048
	char string [MAX_STRING_LEN+1];
	int len;
	
	/* Variable zur Ueberpruefung auf korrekt geklammerte Ausdruecke */
	int bracketCounter = 0;
}  // LOCAL

DEFAULT {
	/* What happens if no scanner rule matches the input */
	WritePosition(stderr, l_scan_Attribute.Position);
	fprintf(stderr, " Illegal character [%c]\n", *l_scan_TokenPtr);
} // DEFAULT

EOF {
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
} // EOF

DEFINE  /* some abbreviations */
	digit  = {0-9}         .
	letter = {a-z_A-Z}     .
	string = - {"\\\n\r\f} .

/* define start states, note STD is defined by default, separate several states by a comma */
/* START STRING */
START STRING, COMMENT
RULE
/* Integers */
#STD# digit+ :
	{
		l_scan_Attribute.int_const.Value = malloc(l_scan_TokenLength+1);
		l_scan_GetWord(l_scan_Attribute.int_const.Value);
		return tok_int_const;
	}

/* Please add rules for: (don't forget to adapt main()) */
/* Float numbers */
#STD# digit * "." digit + ({eE} ("+"|"-")? digit+)? :
	{
		l_scan_Attribute.float_const.Value = malloc(l_scan_TokenLength+1);
		l_scan_GetWord(l_scan_Attribute.float_const.Value);
		return tok_float_const;
	}
/* case insensitive keywords: BEGIN PROCEDURE END CASE */
#STD# {Bb} {Ee} {Gg} {Ii} {Nn}                    : {return tok_BEGIN;}
#STD# {Ee} {Nn} {Dd}                              : {return tok_END;}
#STD# {Pp} {Rr} {Oo} {Cc} {Ee} {Dd} {Uu} {Rr} {Ee}: {return tok_PROCEDURE;}
#STD# {Cc} {Aa} {Ss} {Ee}                         : {return tok_CASE;}

#STD# {Ii} {Ff}                                   : {return tok_IF;}
#STD# {Tt} {Hh} {Ee} {Nn}                         : {return tok_THEN;}
#STD# {Ee} {Ll} {Ss} {Ee}                         : {return tok_ELSE;}
#STD# {Ff} {Oo} {Rr}                              : {return tok_FOR;}
#STD# {Ww} {Hh} {Ii} {Ll} {Ee}                    : {return tok_WHILE;}

#STD# {+} 					  : {return tok_PLUS;}
#STD# {=}					  : {return tok_EQUALS;}

#STD# "-"					  : {return tok_MINUS;}
#STD# {/}				          : {return tok_DIVIDE;}
#STD# {*}					  : {return tok_MULT;}
#STD# ">"					  : {return tok_GT;}
#STD# "<" 					  : {return tok_LT;}


#STD# "(" :
	{
		bracketCounter = bracketCounter+1;
		return tok_BRACKETOP;
	}
	
#STD# ")" :
	{
		bracketCounter = bracketCounter-1;
		return tok_BRACKETCL;
	}

/* identifiers */
#STD# letter (letter | digit) * :
	{
		l_scan_Attribute.identifier.Value = malloc(l_scan_TokenLength+1);
		l_scan_GetWord(l_scan_Attribute.identifier.Value);
		return tok_identifier;
	}

/* comment up to end of line */
#STD# < "--" ANY * > :
	{ 
		WritePosition(stdout, l_scan_Attribute.Position);
		printf(" -- comment\n");
	}


#STD# \" :
	{ 
		yyStart(STRING);
		len = 0;
	}
#STRING# string* :
	{
		if(len + l_scan_TokenLength+1 >= MAX_STRING_LEN){
			WritePosition(stderr, l_scan_Attribute.Position);
			len = 0;
		}
		len += l_scan_GetWord(&string[len]);
	}
#STRING# \" :
	{ 
		yyStart(STD);
		string[len] = '\0';
		l_scan_Attribute.string_const.Value = malloc(len);
		strcpy(l_scan_Attribute.string_const.Value, string);
		return tok_string_const;
	}
#STRING# \\ n :
	{ 
		if(len + 1 >= MAX_STRING_LEN){
			WritePosition(stderr, l_scan_Attribute.Position);
			fprintf(stderr, " tl\n");
		}
		string[len++] = '\n';
	}
#STRING# \\ :
	{
		if(len + 1 >= MAX_STRING_LEN){
			WritePosition(stderr, l_scan_Attribute.Position);
			fprintf(stderr, " tl\n");
		}
		string[len++] = '\\';
	}
/**********************************************************************/
