#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <string.h>
#include <assert.h>

typedef enum bool {false,true} bool;

int* sieve_of_e(int n);
int size_of_prime_arr(int* prime_arr);
void prime_factors(int n);
void stringify(int* array,int size);

void test(void);

int main(void)
{
   int* prime_arr;
   int i;
   i=0;
   /*prime_arr=sieve_of_e(100);
   while(prime_arr[i]>0)
   {
      printf("%d\n",prime_arr[i]);
      i++;
   }
   free(prime_arr);*/
   prime_factors(768);
}

void test(void)
{
   int primes_under20[8]={2,3, 5, 7, 11, 13, 17, 19};
   int primes_under30[8]={2,3, 5, 7, 11, 13, 17, 19,23, 29};
   
}



void prime_factors(int n)
{
   int* prime_arr;
   int i,pos;
   int size;
   int num,remainder;
   int final_arr[10000];
   num=n;
   prime_arr=sieve_of_e(n);
   size=size_of_prime_arr(prime_arr);
   size=size-1;
   pos=0;
   for(i=size;i>=0;i--)
   {

      while(num%prime_arr[i]==0)
      {
         final_arr[pos]=prime_arr[i];
         pos++;
         num/=prime_arr[i];
      }
   }
   for(i=0;i<pos;i++)
   {
      printf("%d\n",final_arr[i]);
   }
   stringify(final_arr,8);
   free(prime_arr);
}

void stringify(int* array,int size)
{
   int count;
   char inter_string[100];
   char final_string[1000]="";
   int i;
   for(i=0;i<size;i++)
   {
      count=1;
      sprintf(inter_string, "%d", array[i]);
      strcat(final_string,inter_string);

      while(array[i]==array[i+1])
      {
         count++;
         i++;
      }
      if(count>1)
      {
         strcat(final_string,"^");
         sprintf(inter_string, "%d",count);
         strcat(final_string,inter_string);
      }

      strcat(final_string,"x");
   }

   if(strncmp(&final_string[strlen(final_string)-1],"x",1)==0)
   {
      final_string[strlen(final_string)-1]='\0';
   }
   printf("%s\n",final_string);
}


int size_of_prime_arr(int* prime_arr)
{
   int count;
   count=0;
   /*added -1 at end of primes list*/
   while(prime_arr[count]>0)
   {
      count++;
   }
   /*dont want to include negative num*/
   count-=1;
   return count;
}


int* sieve_of_e(int n)
{
   bool *p_arr;
   int i;
   int p;
   int count;
   int* prime_arr;

   p_arr= (bool *) malloc(sizeof(bool)*(n+1));
   if(p_arr==NULL)
   {
      fprintf(stderr,"cant open\n");
      exit(EXIT_FAILURE);
   }
   /*set all vals to true*/
   for (i=0 ; i<(n+1); i++)
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
   /*count how many still true*/
   count=0;
   for (p=2;p<=n;p++)
   {
      if (p_arr[p])
      {
         count++;
      }
   }
   prime_arr=(int *)malloc(sizeof(int)*(count+1));
   i=0;
   for (p=2;p<=n;p++)
   {
      if (p_arr[p])
      {

         prime_arr[i]=p;
         i++;
      }
   }
   prime_arr[i]=-1;
   free(p_arr);
   return prime_arr;
}
