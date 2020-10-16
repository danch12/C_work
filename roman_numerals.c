#include <stdio.h>
#include<stdlib.h>
#include <assert.h>

int romanToArabic(char *roman);
int individual_roman(char roman);
void test(void);

int main(int argc, char**argv)
{
   if(argc==2)
   {
      printf("roman numeral %s is equal to %d\n",argv[1],romanToArabic(argv[1]));
   }
   else
   {
      printf("error\n");
   }
   return 0;
}

void test(void)
{
   assert(individual_roman('I')==1);
   assert(individual_roman('X')==10);
   assert(individual_roman('D')==500);
   assert(individual_roman('M')==1000);
   assert(romanToArabic("MCMXCIX")==1999);
   assert(romanToArabic("MCMLXVII")==1967);
   assert(romanToArabic("MCDXCI")==1491);
}

int romanToArabic(char *roman)
{
   int total,i,count;
   int *roman_conversion;

   i=0;
   count=0;
   while(roman[i]!='\0')
   {
      count+=1;
      i++;
   }
   roman_conversion=(int *)malloc(sizeof(int)*count);
   for(i=0;roman[i]!='\0';i++)
   {
      roman_conversion[i]= individual_roman(roman[i]);
   }

   total=0;
   for(i=0;i<count;i++)
   {
      printf("%dth number =%d\n",i,roman_conversion[i]);
      if(roman_conversion[i]<roman_conversion[i+1])
         {
            total+=roman_conversion[i+1]-roman_conversion[i];
            i++;
         }
      else
         {
            total+=roman_conversion[i];
         }
      }


   free(roman_conversion);
   return total;
}

int individual_roman(char roman)
{
   int num;

   switch(roman)
   {
      case('I'):
      num=1;
      break;

      case('V'):
      num=5;
      break;

      case('X'):
      num=10;
      break;

      case('L'):
      num=50;
      break;

      case('C'):
      num=100;
      break;

      case('D'):
      num=500;
      break;

      case('M'):
      num=1000;
      break;

      default:
      num=-1;
   }
   assert(num>0 && "Not a roman numeral\n");
   return num;
}
