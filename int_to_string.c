#include <stdio.h>
#include<stdlib.h>

void int2string(int i, char s[]);
int get_digit_count(int i);
int main(void)
{
   int i;
   char s[256];
   scanf("%d", &i);
   int2string(i,s);
   printf("%s\n",s);
   return 0;
}

void int2string(int i, char s[])
{
   int digit_count,j,divisor;
   digit_count=get_digit_count(i);
   divisor=10;
   for(j=0;j<digit_count-2;j++)
   {
      divisor=divisor*10;
   }

   if(i<0)
   {
      s[0]='-';

      for(j=1;j<=digit_count;j++)
      {

         s[j]=  (abs(i)/divisor)+'0';
         i= abs(i)%divisor;
         divisor=divisor/10;
      }
   }
   else
   {
      for(j=0;j<digit_count;j++)
      {
         s[j]=  i/divisor+'0';
         i= abs(i)%divisor;
         divisor=divisor/10;
      }
   }

}

int get_digit_count(int i)
{
   int remainder,digit_count;
   remainder =i;
   digit_count=0;
   while(remainder!=0)
   {
      remainder/=10;
      digit_count+=1;

   }
   return digit_count;
}
