#include <stdio.h>
#include<stdlib.h>
#include <assert.h>
#include <string.h>


#define ALPHASIZE 26
#define ALPHASTART 'a'-1

typedef enum { false, true } bool;
typedef enum {vowel, consonant} lettertype;

bool isprime(char a,char b, char c);
void cvc_gen(void);
void vcv_gen(void);
void general_gen(lettertype a);
bool is_vowel(char c);

int main(void)
{
   assert(is_vowel('d')==false);
   assert(is_vowel('a')==true);
   assert(isprime('a','a','a')==true);
   assert(isprime('b','b','b')==false);


   general_gen(consonant);
   printf("old\n\n");
   cvc_gen();
   return 0;
}

bool isprime(char a,char b, char c)
{
   int total,i;
   /* do i need to cast all of these? */
   total= (int) a+ (int) b +(int) c - ((int) ALPHASTART *3);
   for(i=2;i<total; i++)
   {
      if (total%i==0)
      {
         return false;
      }
   }
   return true;
}


void general_gen(lettertype a)
{
   char start ='a';
   int i, j,k;

   if(a==vowel)
   {
      for (i=0;i< ALPHASIZE ;i++)
      {
         for(j=0;j<ALPHASIZE;j++)
         {
            for(k=0;k<ALPHASIZE;k++)
            {
               if(start+i==start+k && isprime(start+i,start+j,start+k)
                  && is_vowel(start+i) && !is_vowel(start+j))
               {

                  printf("%c%c%c\n",start+i,start+j,start+k);
               }

         }
      }
   }
   else if (a==consonant)
   {
      for (i=0;i<ALPHASIZE ;i++)
      {
         for(j=0;j<ALPHASIZE;j++)
         {
            for(k=0;k<ALPHASIZE;k++)
            {
               if(start+i==start+k && isprime(start+i,start+j,start+k)
               && !is_vowel(start+i) && is_vowel(start+j))
               {
                  printf("%c%c%c\n",start+i,start+j,start+k);

               }
            }
         }
      }

   }
   else
   {
      printf("input error\n");
   }

}

bool is_vowel(char c)
{
   if(c=='a'||c=='e'||c=='i'||c=='o'||c=='u')
   {
      return true;
   }
   else
   {
      return false;
   }
}
