#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

void int2string(int i, char s[]);
int get_digit_count(int i);

/*working out how many powers of 10 to divide by
-1 because already have first power*/
int get_divisor(int digit_count);

void test(void);

int main(void)
{
   int i;
   char s[256];
   test();
   printf("enter a number\n");
   if(scanf("%d", &i)!=1)
   {
      printf("number not entered\n");
      return 1;
   }
   int2string(i,s);
   printf("%s\n",s);
   return 0;
}

void test(void)
{
   int i;
   char test_arr[3];
   char test_arr_2[4];
   char test_arr_3[1];
   char target_arr[3]={'4','0','0'};
   char target_arr_2[4]={'-','4','0','0'};
   char target_arr_3[1]={'1'};


   assert(get_divisor(1)==1);
   assert(get_divisor(3)==100);
   assert(get_digit_count(245)==3);
   assert(get_digit_count(-50)==2);

   int2string(400,test_arr);
   int2string(-400,test_arr_2);
   int2string(1,test_arr_3);
   for(i=0;i<3;i++)
   {
      assert(test_arr[i]==target_arr[i]);
   }
   for(i=0;i<4;i++)
   {
      assert(test_arr_2[i]==target_arr_2[i]);
   }
   for(i=0;i<1;i++)
   {
      assert(test_arr_3[i]==target_arr_3[i]);
   }
}

int get_divisor(int digit_count)
{
   int j,divisor;
   divisor=1;
   /*working out how many powers of 10 to divide by
   -1 because already have first power*/
   for(j=0;j<digit_count-1;j++)
   {
      divisor=divisor*10;
   }
   return divisor;
}

void int2string(int i, char s[])
{
   int digit_count,j,divisor;
   digit_count=get_digit_count(i);
   divisor=get_divisor(digit_count);
   if(i<0)
   {
      s[0]='-';

      for(j=1;j<=digit_count;j++)
      {

         s[j]=  (abs(i)/divisor)+'0';
         i= abs(i)%divisor;
         divisor=divisor/10;
      }
   }
   else
   {
      for(j=0;j<digit_count;j++)
      {
         s[j]=  i/divisor+'0';
         i= abs(i)%divisor;
         divisor=divisor/10;
      }
   }

}

int get_digit_count(int i)
{
   int remainder,digit_count;
   remainder =i;
   digit_count=0;
   while(remainder!=0)
   {
      remainder/=10;
      digit_count+=1;

   }
   return digit_count;
}
