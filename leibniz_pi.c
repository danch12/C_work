#include <stdio.h>
#include<stdlib.h>
#include <assert.h>

#define MAXITER 10000000

float leibniz(int iters);

int main(void)
{
   printf("approx after %d iters = %f\n", MAXITER, leibniz(MAXITER));
   return 0;
}


float leibniz(int iters)
{
   int i;
   float start;

   start=4.0;
   /*want to alternate states for + -*/
   for(i=3;i<iters;i+=2)
   {
      if (i%2!=0)
      {
         start=start-((float) 4/ (float)i);
      }
      else
      {
         start=start+((float) 4/ (float)i);
      }

   }

   return start;
}
