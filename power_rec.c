#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <string.h>
#include <assert.h>


int power_rec(unsigned int a, unsigned int b);
int power_rec_better(unsigned int a, unsigned int b);
int main(void)
{
   printf("%d\n",power_rec(2,3));
   printf("%d\n",power_rec_better(2,3));
   return 0;
}


int power_rec(unsigned int a, unsigned int b)
{
   if(b==1)
   {
      return a;
   }
   else
   {
      return a * power_rec(a,b-1);
   }
}

int power_rec_better(unsigned int a, unsigned int b)
{
   int t;
   if(a==0)
   {
      return 0;
   }
   if(b==0)
   {
      return 1;
   }
   if(b==1)
   {
      return a;
   }
   if(b%2==0)
   {
      t= power_rec_better(a, b/2)
      return t*t;
   }
   else
   {
      return a * power_rec_better(a,b-1);
   }
}
