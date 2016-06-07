/* $Id: ScanDrv.c,v 2.9 1998/06/24 15:52:15 grosch rel $ */

# include <stdio.h>
# include "Position.h"
# include "l_scan.h"

# define WordSize 2048

int main ARGS ((void))
{
   int Token, Count = 0;
   char Word [WordSize];

   l_scan_BeginScanner ();
   do {
      Token = l_scan_GetToken ();
      Count ++;
# ifdef Debug
      if (Token != l_scan_EofToken) {
	 if (l_scan_TokenLength < WordSize) {
	    (void) l_scan_GetWord (Word);
	 } else {
	    strncpy (Word, l_scan_TokenPtr, WordSize - 1);
	    Word [WordSize - 1] = '\0';
	 }
      } else {
	 Word [0] = '\0';
      }
      WritePosition (stdout, l_scan_Attribute.Position);
      (void) printf ("%5d %s\n", Token, Word);
# endif
   } while (Token != l_scan_EofToken);
   l_scan_CloseScanner ();
   (void) printf ("%d\n", Count);
   return 0;
}
