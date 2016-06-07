/* Project:  COCKTAIL training
 * Descr:    A simple pocket computer (scanner, parser, evaluator)
 * Kind:     C-main program
 * Author:   Prof. Dr. Juergen Vollmer <vollmer@dhbw-karlsruhe.de>
 * $Id: main.c,v 1.6 2014/12/14 14:49:55 vollmer Exp $
 */

# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include "Position.h"
# include "Errors.h"
# include "expr_scan.h"
# include "expr_pars.h"

int main (int argc, char *argv[])
{
   int errors = 0;
   if (argc == 2) {
     if (strcmp (argv[1], "-h") == 0) {
       fprintf (stderr,
		"usage: %s [-h] [file]\n"
		"  expression LR based parser, reads `file' or stdin\n"
		"  -h: Help\n", argv[0]);
       exit (0);
     }

     expr_scan_Attribute.Position.FileName = MakeIdent1 (argv[1]);
     expr_scan_BeginFile (argv[1]);
     /* Read from file argv[1].
      * If this routine is not called, stdin is read.
      */
   }

   errors = expr_pars ();  /* the parser */

   printf ("parser returned: %d number of errors: %d\n",
	   errors,GetCount (xxError));

   return (errors == 0)? 0 : 1;
}
