# include <stdio.h>
# include <math.h>
# define GAP 'a'-1
typedef enum vowel{a=1,e=5,i=9,o=15,u=21} vowel;
typedef enum boolean{False,True} boolean;

int isPrime(int n);
int isVowel(int n);

int main()
{
   int i,j;
   for(i=1;i<=26;i++)
   {
      for(j=1;j<=26;j++)
      {
         if((isPrime(2*i+j)&&isVowel(i)&&!isVowel(j))||(isPrime(2*i+j)&&!isVowel(i)&&isVowel(j)))
            {
               printf("%c%c%c\n",i+GAP,j+GAP,i+GAP);
            }
      }
   }
   return 0;
}

int isPrime(int n)
{
   int i;
   if(n==1)
   {
      return False;
   }
   for(i=2;i<=sqrt(n);i++)
   {
      if(n%i==0)
      {
            return False;
      }
   }
   return True;
}

int isVowel(int n)
{
   if(n==a||n==e||n==i||n==o||n==u)
   {
      return True;
   }
   else
   {
      return False;
   }
}
