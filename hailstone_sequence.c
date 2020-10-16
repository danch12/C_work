#include <stdio.h>
#include<stdlib.h>
#include <assert.h>

#define MAX 50000

typedef struct{
   int max;
   int len;
} counter;

counter hailstone(int start);
int hailstone_recc(int start, int l_num);
int hailstone_recc_long(int start, int long_num);

int main(void)
{
   int i;
   int max_len,max_num;
   int max_len_start , max_num_start;
   counter temp;
   max_len=0;
   max_num=0;
   for(i=2;i<MAX;i++)
   {
      temp = hailstone(i);
      if(temp.max>max_num)
      {
         max_num = temp.max;
         max_num_start = i;
      }
      if(temp.len>max_len)
      {
         max_len=temp.len;
         max_len_start= i;
      }

   }
   printf("%d creates longest hailstone seq with %d numbers\n",max_len_start,max_len );
   printf("%d creates the largest number : %d\n",max_num_start,max_num );
}


counter hailstone(int start)
{
   counter count;
   count.max=start;
   count.len=1;

   while(start!=1)
   {
      if(start>count.max)
      {
         count.max=start;
      }
      count.len+=1;
      if(start%2==0)
      {
         start=start/2;
      }
      else
      {
         start= start*3+1;
      }
   }

   return count;
}

int hailstone_recc(int start, int l_num)
{
   if(start==1)
   {
      return l_num;
   }
   else
   {
      if (start%2==0)
      {
         if(start>l_num)
         {
         return hailstone_recc(start/2,start);
         }
         else
         {
            return hailstone_recc(start/2,l_num);
         }

      }
      else
      {
            if(start>l_num)
            {
               return hailstone_recc(start*3+1,start);
            }
            else
            {
               return hailstone_recc(start*3+1,l_num);
            }
      }
   }
}



int hailstone_recc_long(int start, int long_num)
{
   if(start==1)
   {
      return long_num;
   }
   else
   {
      if (start%2==0)
      {
         return hailstone_recc(start/2,long_num+1);
      }
      else
      {
         return hailstone_recc(start*3+1,long_num+1);
      }
   }
}
