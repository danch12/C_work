#include <stdlib.h>
#include <stdio.h>

typedef int money;

void nearest_20(money start);


int main(void)
{
   nearest_20(15);
   nearest_20(13);
   nearest_20(22);
   nearest_20(52);
   return 0;

}

void nearest_20(money start)
{
   money lower,upper;
   if(start%20==0)
   {
      printf("your money is a multiple of 20 so its cool\n");
   }
   else
   {
      lower = start - (start%20);
      upper = lower+20;
      printf("lower amount is %d\n", lower);
      printf("upper amount is %d\n", upper);
   }

}
