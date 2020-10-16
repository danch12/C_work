#include <stdio.h>
#include<stdlib.h>
#include <ctype.h>
#define INVERT 25

int scode(int a);

int main(void)
{
   char input;


   while(scanf(" %c",&input)==1)
   {
      printf("%c",scode((int) input));
      printf("\n");
      if(input=='!')
      {
         return 0;
      }
   }
   printf("\n");
   return 0;
}


int scode(int a)
{
   if(islower((char) a))
   {
   return a + INVERT - (2*( a- (int) 'a' ));
   }
   else if((char) a >= 'A' && (char) a <='Z')
   {
      return a + INVERT -(2*(a-(int) 'A'));
   }
   else
   {
      printf("not a letter");
      return 33;
   }

}
