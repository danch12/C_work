#include <stdio.h>
#include<stdlib.h>
#include <assert.h>

void triangle_num(int num);

int main(void)
{
   /*assert(triangle_num(5)==15);
   assert(triangle_num(4)==10);*/
   triangle_num(10);

   return 0;
}

void triangle_num(int num)
{
   int sum, i;
   sum=0;
   for(i=1;i<=num;i++)
   {
      sum+=i;
      printf("the %d triangle number is %d\n", i, sum);
   }

}
