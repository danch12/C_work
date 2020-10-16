#include <stdlib.h>
#include <stdio.h>

float find_max(void);
int find_max_odd(void);

int main(void)
{
 printf("max= %f\n",find_max());
 printf("max odd = %d\n", find_max_odd());
 return 0;
}

float find_max(void)
{
   int i;
   int amount;
   float max;
   float *arr;
   printf("How many numbers do you wish to enter ?\n");
   scanf("%d",&amount);
   arr=(float *)malloc(sizeof(float)*amount);
   for(i=0;i<amount;i++)
   {
      printf("enter number %d\n",i+1);
      scanf("%f",&arr[i]);
   }
   max = arr[0];
   for(i=1;i<amount;i++)
   {

      if (max<arr[i])
      {
         max=arr[i];
      }
   }

   return max;
}

int find_max_odd(void)
{
   int i;
   int amount;
   int max;
   int *arr;
   printf("How many numbers do you wish to enter ?\n");
   scanf("%d",&amount);
   arr=(int *)malloc(sizeof(int)*amount);
   for(i=0;i<amount;i++)
   {
      printf("enter number %d\n",i+1);
      scanf("%d",&arr[i]);
   }
   max = arr[0];
   for(i=1;i<amount;i++)
   {

      if (max<arr[i]&&arr[i]%2!=0)
      {
         max=arr[i];
      }
   }

   return max;
}
