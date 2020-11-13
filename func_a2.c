#include "common.h"
#include <ctype.h>




void strtogglealpha(char* s, const char c)
{
   int i;


   for(i=0;i<(int)strlen(s);i++)
   {

      if(s[i]==tolower(c))
      {

         s[i]=toupper(s[i]);


      }
      else if(s[i]==toupper(c))
      {
         s[i]=tolower(s[i]);
      }
   }

}
