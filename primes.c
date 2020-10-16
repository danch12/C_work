#include <stdlib.h>
#include <stdio.h>
#include <math.h>

typedef enum { false, true } bool;


void primes(int amount);
void primesrecc(int amount, int start);
void sieve_of_e(int n);

int main(void)
{
    int amount;
    printf("enter amount of primes you want\n");
    scanf("%d",&amount);
    primes(amount);
    printf("recc-\n");
    primesrecc(amount,2);
    printf("sieve with 50- \n");
    sieve_of_e(50);
}


void primes(int amount)
{
    /*want to start on 3 so just printing 2*/
    int i;
    int count;
    int start;
    int state;
    printf("2\n");

    count =0;

    start=3;
    state=true;
    while (count<amount-1)
    {
        for(i=2;i<start;i++)
        {

            if(start%i==0)
            {
                state=false;
            }
        }
        if(state==true)
        {
           printf("%d\n",start);
           count+=1;
        }
        start+=1;
        state=true;
    }
}

void primesrecc(int amount, int start)
{
   int i;
    if(amount==0)
    {
      return;
    }
    else
    {
      if (start==2)
      {
        printf("2\n");
        primesrecc(amount-1,start+1);
        return;
      }
      else
         {
            for(i=2;i<start;i++)
         {
            if(start%i==0)
            {
                 primesrecc(amount, start+1);
                 return;
            }
         }

         printf("%d\n",start);
         primesrecc(amount-1,start+1);

    }
    }
}

void sieve_of_e(int n)
{
   bool *p_arr;
   int i;
   int p;
   p_arr= (bool *) malloc(sizeof(bool)*(n+1));

   if(p_arr==NULL)
   {
      printf("malloc failed\n");
      return;
   }
   for (i=0 ; i<=(n+1); i++)
   {
      p_arr[i]=true;
   }

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
   for (p=2;p<=n;p++)
   {
      if (p_arr[p])
      {
         printf("%d\n",p);
      }
   }
}
