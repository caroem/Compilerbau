/* Project:  COCKTAIL training
 * Descr:    A simple scanner generated with rex
 * Kind:     C-main function, fully implemented solution
 * Author:   Dr. Juergen Vollmer <juergen.vollmer@informatik-vollmer.d
 * $Id: main.c.in,v 1.4 2010/04/15 12:52:49 vollmer Exp $
 */

# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include "Position.h"
# include "l_scan.h"

int main (int argc, char *argv[]){
	int Token = 0;
	int Count = 0;
	if(argc == 2){
		if(strcmp(argv[1], "-h") == 0){
			fprintf(stderr,
				"usage: scan [-h] [file]\n"
				"  simple scanner, reads `file' or stdin\n"
				"  -h: Help\n"
			);
			exit(0);
		 }
		 l_scan_BeginFile(argv[1]);
		 /* Read from file argv[1].
			* If this routine is not called, stdin is read.
			*/
	}
	for(Token =  l_scan_GetToken();
		Token != l_scan_EofToken;
		Token = l_scan_GetToken()
	){
		Count++;
		WritePosition(stdout, l_scan_Attribute.Position);
		printf(" ");
		switch(Token){
			case tok_int_const:
				printf("int_const    : %s\n",l_scan_Attribute.int_const.Value);
				break;
			/* add code to emit your tokens here */
			case tok_float_const:
				printf("float_const: %s\n",l_scan_Attribute.float_const.Value);
				break;
			case tok_identifier:
				printf("identifier: %s\n",l_scan_Attribute.identifier.Value);
				break;
			case tok_string_const:
				printf("string_const: %s\n",l_scan_Attribute.string_const.Value);
				break;
			case tok_BEGIN:
				puts("BEGIN");
				break;
			case tok_END:
				puts("END");
				break;
			case tok_PROCEDURE:
				puts("PROCEDURE");
				break;
			case tok_IF:
				puts("IF");
				break;
			case tok_THEN:
				puts("THEN");
				break;
			case tok_ELSE:
				puts("ELSE");
				break;
			case tok_WHILE:
				puts("WHILE");
				break;
			case tok_FOR:
				puts("FOR");
				break;
			case tok_PLUS:
				puts("+");
				break;
			case tok_EQUALS:
				puts("=");
				break;
			case tok_MINUS:
				puts("-");
				break;
			case tok_DIVIDE:
			        puts("/");
				break;
			case tok_MULT:
			        puts("*");
				break;
			case tok_BRACKETOP:
			        puts("(");
				break;
			case tok_BRACKETCL:
			        puts(")");
				break;
			case tok_LT:
			        puts("<");
				break;
			case tok_GT:
			        puts(">");
				break;
			default:
				fprintf(stderr, "FATAL ERROR, unknown token\n");
		}
	}
	printf("Token count: %d\n", Count);
	return 0;
}
