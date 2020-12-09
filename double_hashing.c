#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <string.h>
#include <assert.h>
#include <ctype.h>

typedef enum bool {false, true} bool;




int sieve_of_e(int n);

/*return void then cast when appropriate*/
void* safe_calloc(size_t nitems, size_t size);


void test(void);

int main(void)
{
   test();
   return 0;
}


void test(void)
{
   printf("%d\n",sieve_of_e(10));

}

/*return void then cast when appropriate*/
void* safe_calloc(size_t nitems, size_t size)
{
   void* ptr;
   ptr = calloc(nitems, size);
   if(ptr==NULL)
   {
      fprintf(stderr,"Not enough space\n");
   	exit(EXIT_FAILURE);
   }
   return ptr;
}



/*returns prime that is closest (lower) with value given*/
int sieve_of_e(int n)
{
   bool *p_arr;
   int i;
   int p;
   int count;
   p_arr= (bool *)safe_calloc(n+1,sizeof(bool));
   /*set all vals to true*/
   for (i=0 ; i<n; i++)
   {
      p_arr[i]=true;
   }
   /*if val is true go through and turn all of its powers false*/
   for(p=2;p*p<=n;p++)
   {
      if(p_arr[p]==true)
      {
         for(i=p*p; i<=n;i+=p)
         {
            p_arr[i]=false;
         }
      }
   }
   for(p=n;p>0;p--)
   {
      if(p_arr[p]==true)
      {
         return p;
      }
   }
   /*no primes found*/
   return -1;
}
