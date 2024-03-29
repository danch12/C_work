#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <string.h>
#include <assert.h>


typedef enum bool {false,true} bool;

int* sieve_of_e(int n);
int size_of_prime_arr(int* prime_arr);
char* prime_factors(int n);
char* stringify(int* array,int size);
void swap(int* a, int* b);
int* prime_factors_sdl(int n);
void simple_sort(int arr[],int size);

void test(void);

int main(void)
{

   int i;
   char* word;
   i=0;
   test();
   /*prime_arr=sieve_of_e(100);
   while(prime_arr[i]>0)
   {
      printf("%d\n",prime_arr[i]);
      i++;
   }
   free(prime_arr);*/

   word=prime_factors(27000);
   printf("%s\n",word);
   free(word);
   return 0;
}

void test(void)
{
   int i;
   int* sieve_under20;
   int* sieve_under30;
   int* sdl_prime;
   int primes_under20[8]={2,3, 5, 7, 11, 13, 17, 19};
   int primes_under30[10]={2,3, 5, 7, 11, 13, 17, 19,23, 29};
   int test_list[6]={0,1,2,3,4,-1};
   int test_stringify[6]={3,4,4,5,5,5};
   char* test_string;
   /*int* sieve_of_e(int n)*/
   sieve_under20=sieve_of_e(20);
   sieve_under30=sieve_of_e(30);
   for(i=0;i<8;i++)
   {
      assert(sieve_under20[i]==primes_under20[i]);
   }
   for(i=0;i<10;i++)
   {
      assert(sieve_under30[i]==primes_under30[i]);
   }
   free(sieve_under20);
   free(sieve_under30);

   assert(size_of_prime_arr(test_list)==5);
   /*int size_of_prime_arr(int* prime_arr)*/

   test_string=stringify(test_stringify,6);
   printf("%s\n",test_string);
   free(test_string);
   simple_sort(test_stringify,6);
   printf("sorted\n");
   for(i=0;i<6;i++)
   {
      printf("%d\n",test_stringify[i]);
   }

   sdl_prime=prime_factors_sdl(117);
   for(i=0;i<3;i++)
   {
      printf("%d\n",sdl_prime[i]);
   }
   free(sdl_prime);

}

void one_num_sdl(int arr[])
{

}


int* prime_factors_sdl(int n)
{
   int* prime_arr;
   int i,pos,count;
   int size;
   int num;
   int* final_arr;
   num=n;
   prime_arr=sieve_of_e(n);
   size=size_of_prime_arr(prime_arr);
   size=size-1;
   count=0;
   for(i=size;i>=0;i--)
   {

      while(num%prime_arr[i]==0)
      {
         count++;
         num/=prime_arr[i];
      }
   }
   printf("count =%d\n",count);
   final_arr=(int*)malloc(sizeof(int)*count);
   pos=0;
   num=n;
   for(i=size;i>=0;i--)
   {

      while(num%prime_arr[i]==0)
      {
         final_arr[pos]=prime_arr[i];
         pos++;
         num/=prime_arr[i];
      }
   }
   simple_sort(final_arr,pos);
   free(prime_arr);
   return final_arr;
}



void swap(int* a, int* b)
{
   int temp;
   temp= *a;
   *a= *b;
   *b=temp;
}

void simple_sort(int arr[],int size)
{
   int swap_count,i;

   do
   {
      swap_count=0;
      for(i=0;i<size-1;i++)
      {
         if(arr[i]<arr[i+1])
         {
            swap(&arr[i],&arr[i+1]);
            swap_count++;
         }
      }

   } while(swap_count!=0);

}

char* prime_factors(int n)
{
   int* prime_arr;
   int i,pos;
   int size;
   int num;
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
   /*for(i=0;i<pos;i++)
   {
      printf("%d\n",final_arr[i]);
   }*/

   free(prime_arr);
   return stringify(final_arr,9);
}

char* stringify(int* array,int size)
{
   int count;
   char inter_string[100];
   char final_string[1000];
   char* final_string_point;
   int i;
   strcpy(final_string,"");
   for(i=0;i<size;i++)
   {
      count=1;
      sprintf(inter_string, "%d", array[i]);
      strcat(final_string,inter_string);

      while(i<size-1&&array[i]==array[i+1])
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

   final_string_point=malloc(strlen(final_string)+1);
   strcpy(final_string_point,final_string);
   final_string_point[strlen(final_string)]='\0';
   return final_string_point;
}


int size_of_prime_arr(int* prime_arr)
{
   int count;
   count=0;
   /*added -1 at end of primes list*/
   while(prime_arr[count]>=0)
   {
      count++;
   }
   /*dont want to include negative num*/

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
