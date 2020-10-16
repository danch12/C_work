#include <stdio.h>
#include<stdlib.h>
#include <assert.h>
#include <time.h>

#define MAXITER 1000000
#define SQLEN 5

typedef int length;
typedef int coordinate;

length random_gen(length upper, length lower);
int dart_throw(length x, length y, length r);

int main(void)
{
   int count,i;
   length r;
   count=0;

   /*making random lengths for square */
   r=SQLEN;
   assert(r>0);

   for(i=0;i<MAXITER;i++)
   {
      count+= dart_throw(r,r,r);
      printf("pi approx= %f\n", (4 * ((float) count/i)));
   }
   return 0;

}

coordinate random_gen(length upper, length lower)
{
   srand(time(NULL));
   return (coordinate) rand() % upper + lower;
}

int dart_throw(length x, length y, length r)
{
   coordinate x_cord,y_cord;
   x_cord= random_gen(x,0);
   y_cord= random_gen(y,0);
   if((x_cord*x_cord)+(y_cord*y_cord)<=(r*r))
   {
      return 1;
   }
   else
   {
      return 0;
   }
}
