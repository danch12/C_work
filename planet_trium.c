#include <stdio.h>
#include<stdlib.h>

#define CONSIZE 22
#define VOWSIZE 6


int main(void)
{
   char letters[CONSIZE]="bcdfghjklmnpqrstvwxyz";
   char vowels[VOWSIZE]="aeiou";
   int i, j,k;
   for (i=0;i< CONSIZE-1 ;i++)
   {
      for(j=0;j<VOWSIZE-1;j++)
      {
         for(k=0;k<CONSIZE-1;k++)
         {
            printf("%c%c%c\n",letters[i], vowels[j],letters[k]);
         }
      }
   }
}
