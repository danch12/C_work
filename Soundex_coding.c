#include <stdio.h>
#include<stdlib.h>
#include <assert.h>
#include <ctype.h>

#define ISLABIAL(x) ((x=='B'||x=='F'||x=='P'||x=='V') ? 1 : 0)
#define ISGUTTSANDSIBS(x) ((x=='C'||x=='G'||x=='J'||x=='K'||x=='Q'||x=='S'||x=='X'||x=='Z') ? 1 : 0)
#define ISDENTAL(x) ((x=='D'||x=='T') ? 1 : 0)
#define ISLONGLIQUID(x) ((x=='L') ? 1 : 0)
#define ISNASAL(x) ((x=='M'||x=='N') ? 1 : 0)
#define ISSHORTLIQUID(x) ((x=='R') ? 1:0)
#define ISSKIPCHAR(x) ((x=='H'||x=='W') ? 1: 0)

#define SKIPPREV 's'
#define PASS 'p'
#define START 'x'

void toSoundex(char* name);
char lettertype(char letter);
void str_toupper(char* str);

int main(int argc, char*argv[])
{
   assert(ISLABIAL('B')==1);
   assert(ISGUTTSANDSIBS('C')==1);
   assert(ISDENTAL('D')==1);
   assert(ISLONGLIQUID('L')==1);
   assert(ISNASAL('M')==1);
   assert(ISSHORTLIQUID('R')==1);
   if(argc==2)
   {
      toSoundex(argv[1]);
      return 0;
   }
   else
   {
      printf("error\n");
      return 1;
   }
}



void toSoundex(char* name)
{
   int count,i;
   char previous,current;

   printf("%c",toupper(name[0]));
   count=0;
   i=1;

   previous=START;
   while(count<3 && name[i]!='\0')
   {
      current= lettertype(toupper(name[i]));
      if(current!=SKIPPREV)
      {
         if(current!= PASS && current!=previous)
         {
            printf("%c",current);
            count+=1;
         }
         previous=current;
      }
      i++;
   }
   for(;count<3;count++)
   {
      printf("0");
   }
   printf("\n");
   assert(count==3);
}

char lettertype(char letter)
{
   if(ISLABIAL(letter))
   {
      return '1';

   }
   else if(ISGUTTSANDSIBS(letter))
   {
      return '2';

   }
   else if(ISDENTAL(letter))
   {
      return '3';

   }
   else if(ISLONGLIQUID(letter))
   {
      return '4';

   }
   else if(ISNASAL(letter))
   {
      return '5';
   }
   else if(ISSHORTLIQUID(letter))
   {
      return '6';
   }
   else if(ISSKIPCHAR(letter))
   {
      return SKIPPREV;
   }
   else
   {
      return PASS;
   }
}
