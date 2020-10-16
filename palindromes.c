#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include <assert.h>
#define LIMIT 100
#define CASEDIFF ('a' - 'A')

typedef enum bool {false, true} bool;


char* to_lower(char sent[],int size);
bool is_palindrome_cased(char sent[],int size);
bool is_palindrome(char sent[],int size);

int main(void)
{
   char user_input[LIMIT];
   int size;
   char test[]= "kayak";
   char test_2[]="kayfjb";
   char test_3[]= "a man, A plan, a Canal: panama!";
   assert(is_palindrome(test,6)==true);
   assert(is_palindrome(test_2,6)==false);
   assert(is_palindrome(test_3,32)==true);

   printf("enter a palindrome\n");
   if(scanf("%s",user_input)!=1)
   {
      printf("error\n");
      return 1;
   }
   size= sizeof(user_input)/sizeof(user_input[0]);
   
   if(is_palindrome(user_input,size)==true)
   {
      printf("it is a palindrome\n");
   }
   else
   {
      printf("it isnt a palindrome\n");
   }

   return 0;
}

bool is_palindrome_cased(char sent[],int size)
{
   int start, end;
   start =0;
   end = size-2;
   while(start<=end)
   {
      while(!isalpha(sent[start]))
      {
         start+=1;
      }
      while(!isalpha(sent[end]))
      {
         end-=1;
      }

      if(sent[start]!=sent[end])
      {
         return false;
      }
      start+=1;
      end-=1;
   }
   return true;
}


char* to_lower(char sent[],int size)
{
   int i;
   for(i=0;i<size;i++)
   {
      if(!islower(sent[i]) && isalpha(sent[i]))
      {
         sent[i]= sent[i]+CASEDIFF;
      }

   }
   return sent;
}

bool is_palindrome(char sent[],int size)
{
   char* lowered;
   lowered = to_lower(sent,size);
   return is_palindrome_cased(lowered,size);
}
