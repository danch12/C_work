#include <stdio.h>
#include<stdlib.h>
#include <assert.h>
#include <ctype.h>
#include <string.h>

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
#define SIZE 4

char* toSoundex(char* name);
char lettertype(char letter);
void test(void);


int main(int argc, char*argv[])
{
   char *arabic_version;
   test();
   if(argc==2)
   {
      arabic_version=toSoundex(argv[1]);
      printf("%s",arabic_version);
      free(arabic_version);
      printf("\n");
      return 0;
   }
   else
   {
      printf("error need a arg\n");
      return 1;
   }
}

void test(void)
{
   int i;
   char* test_arr;
   char* test_arr_2;
   char* test_arr_3;

   char target_arr[SIZE]={'W','2','5','2'};
   char target_arr_2[SIZE]={'W','0','0','0'};
   char target_arr_3[SIZE]={'D','2','5','3'};

   assert(ISLABIAL('B')==1);
   assert(ISGUTTSANDSIBS('C')==1);
   assert(ISDENTAL('D')==1);
   assert(ISLONGLIQUID('L')==1);
   assert(ISNASAL('M')==1);
   assert(ISSHORTLIQUID('R')==1);
   assert(lettertype('B')=='1');
   assert(lettertype('C')=='2');
   assert(lettertype('D')=='3');
   assert(lettertype('H')==SKIPPREV);

   test_arr=toSoundex("Washington");
   test_arr_2=toSoundex("Wu");
   test_arr_3=toSoundex("DESMET");

   for(i=0;i<SIZE;i++)
   {
      assert(test_arr[i]==target_arr[i]);
      assert(test_arr_2[i]==target_arr_2[i]);
      assert(test_arr_3[i]==target_arr_3[i]);
   }
   free(test_arr);
   free(test_arr_2);
   free(test_arr_3);
}



char* toSoundex(char* name)
{
   int count,i;
   char previous,current;
   char* target;
   target=malloc(sizeof(char)*4);
   target[0]=toupper(name[0]);

   /*need to start i and count on one as already assigned first */
   i=1;
   count=1;

   previous=START;
   while(count<4 && name[i]!='\0')
   {
      current= lettertype(toupper(name[i]));
      if(current!=SKIPPREV)
      {
         if(current!= PASS && current!=previous)
         {
            target[count]= current;
            count+=1;
         }
         previous=current;
      }
      i++;
   }
   /*fill rest with 0s*/
   for(;count<4;count++)
   {
      target[count]='0';
   }
   assert(count==4);
   return target;
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
