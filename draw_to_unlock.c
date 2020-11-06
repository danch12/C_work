#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <string.h>
#include <assert.h>

#define BEHIND -1
#define INFRONT 2
#define SCREENLEN 5
#define LENGTH 25

typedef enum bool {false,true} bool;



int count_moves(char screen[SCREENLEN][SCREENLEN],int x,int y,char string[],\
                  char* arr_to_fill[],int start);
bool inbounds(int x);
bool not_been(char n,char been[]);
void test(void);
bool coord_been(char screen[SCREENLEN][SCREENLEN],int x, int y,char been[]);

int main(void)
{
   int i,num_patterns;
   char string[1000]="";
   char* patterns[50];
   char end_string[5]="END";
   /*char screen[SCREENLEN][SCREENLEN]={{'1','2','3'},
                                       {'4','5','6'},
                                       {'7','8','9'}};*/
   char screen[SCREENLEN][SCREENLEN]={{'1','2','3','4','5'},
                                       {'6','7','8','9','a'},
                                       {'b','c','d','e','f'},
                                       {'g','h','i','j','k'},
                                       {'l','m','n','o','p'}};
   /*test();*/
   i=0;
   num_patterns=count_moves(screen,0,0,string,patterns,0);
   /*patterns[num_patterns]=(char*)malloc(sizeof(char)*strlen(end_string)+1);
   strcpy(patterns[num_patterns],end_string);
   while(strcmp(patterns[i],end_string)!=0)
   {
      printf("%d\n",i+1);
      printf("%s\n",patterns[i]);
      free(patterns[i]);
      i++;
   }*/
   return 0;
}


void test(void)
{
   int i;
   int num_patterns_test;
   char test_string[1000]="";
   char* pattern_test[100];
   char end_string[5]="END";
   char test_screen[SCREENLEN][SCREENLEN]={{'1','2','3'},
                                          {'4','5','6'},
                                          {'7','8','9'}};

   char target_strings[10][10]={"123654789",
                                 "123698547",
                                 "123698745",
                                 "125478963",
                                 "145236987",
                                 "147852369",
                                 "147896325",
                                 "147896523"};

   assert(inbounds(50)==false);
   assert(inbounds(-1)==false);
   assert(inbounds(1)==true);
   assert(inbounds(0)==true);
   assert(not_been('1',"123")==false);
   assert(not_been('2',"123")==false);
   assert(not_been('9',"1238")==true);
   num_patterns_test=count_moves(test_screen,0,0,test_string,pattern_test,0);
   for(i=0;i<num_patterns_test;i++)
   {
      assert(strcmp(target_strings[i],pattern_test[i])==0);
      free(pattern_test[i]);
   }

}



int count_moves(char screen[SCREENLEN][SCREENLEN],int x,int y,char string[],\
                  char* arr_to_fill[],int start)
{
   int i,j;
   static int count;
   count=start;
   for(i=BEHIND;i<INFRONT;i++)
   {
      for(j=BEHIND;j<INFRONT;j++)
      {
         /*no diagonals*/
         if(i==0||j==0)
         {
            /*dont go off phone*/
            if(inbounds(x+i)&&inbounds(y+j))
            {
               if(not_been(screen[x+i][y+j],string))
               {

                  strncat(string,&screen[x+i][y+j],1);
                  count_moves(screen,x+i,y+j,string,arr_to_fill,count);
                  /*then remove again so different path taken
                  dont want to remove first char tho*/
                  if(strlen(string)>1)
                  {
                     string[strlen(string)-1]='\0';
                  }




               }


            }

         }
      }
   }
   if(strlen(string)==LENGTH)
   {
      printf("%s\n",string);
      /*arr_to_fill[count]=(char*)malloc(sizeof(char)*strlen(string)+1);
      strcpy(arr_to_fill[count],string);*/
      count+=1;


   }


   return count;

}


bool inbounds(int x)
{
   if(x>=0&&x<SCREENLEN)
   {
      return true;
   }
   return false;
}

bool not_been(char n,char been[])
{
   int i;
   for(i=0;been[i]!='\0';i++)
   {
      if(been[i]==n)
      {
         return false;
      }
   }
   return true;
}
