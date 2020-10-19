#include <stdio.h>
#include<stdlib.h>
#include <assert.h>

int romanToArabic(char *roman);
int individual_roman(char roman);
int get_len(char *string);
int* to_arabic_array(char *roman,int count);

void test(void);

int main(int argc, char**argv)
{
   if(argc==2)
   {
      printf("roman numeral %s is equal to %d\n",argv[1],romanToArabic(argv[1]));
   }
   else
   {
      printf("error enter a roman numeral next time pls\n");
   }
   return 0;
}

void test(void)
{
   int* test_arr;
   assert(individual_roman('I')==1);
   assert(individual_roman('X')==10);
   assert(individual_roman('D')==500);
   assert(individual_roman('M')==1000);
   assert(get_len("1234")==4);
   assert(get_len("12345")==5);
   assert(get_len("1234567")==7);

   test_arr= to_arabic_array("MCX",3);
   assert(test_arr[0]==1000);
   assert(test_arr[1]==100);
   assert(test_arr[2]==10);
   free(test_arr);

   assert(romanToArabic("MCMXCIX")==1999);
   assert(romanToArabic("MCMLXVII")==1967);
   assert(romanToArabic("MCDXCI")==1491);
}

int get_len(char *string)
{
   int i,count;
   count=0;
   for(i=0;string[i]!='\0';i++)
   {
      count+=1;
   }
   return count;
}

int* to_arabic_array(char *roman,int count)
{
   int i;
   int* roman_conversion;
   roman_conversion=(int *)malloc(sizeof(int)*count);
   for(i=0;roman[i]!='\0';i++)
   {
      roman_conversion[i]= individual_roman(roman[i]);
   }
   return roman_conversion;
}

int romanToArabic(char *roman)
{
   int total,i,count;
   int *roman_conversion;
   count= get_len(roman);
   roman_conversion=to_arabic_array(roman,count);
   total=0;
   for(i=0;i<count;i++)
   {

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
