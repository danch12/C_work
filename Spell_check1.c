#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <string.h>
#include <assert.h>


#define MAXWORDSIZE 50
#define BUFFERSIZE 1000

typedef enum bool {false,true} bool;

void move_up_one(int start_pos,char word_list[][MAXWORDSIZE],int size);
void put_word_in(char word[],char word_list[][MAXWORDSIZE],int size);
void read_file_in(char word_array[][MAXWORDSIZE],int arr_size, char* filename);
bool in_list(char word[],char word_array[][MAXWORDSIZE],int arr_size);
void test(void);

int main(void)
{
   test();
   return 0;
}


void test(void)
{
   int i;
   char test_arr[4][MAXWORDSIZE]={"first","sec","third",""};
   char test_arr_2[6][MAXWORDSIZE]={"abc","bbc","dbc",""};
   char large_test[10000][MAXWORDSIZE]={""};
   char target_test[5][MAXWORDSIZE]={"abet","alert","alter","beat","beta"};
   move_up_one(0,test_arr,4);
   assert(strcmp(test_arr[3],"third")==0);
   put_word_in("bac",test_arr_2,5);

   assert(strcmp(test_arr_2[1],"bac")==0);

   put_word_in("aac",test_arr_2,5);
   assert(strcmp(test_arr_2[0],"aac")==0);
   assert(strcmp(test_arr_2[2],"bac")==0);

   read_file_in(large_test,10000,"test_word_list.txt");
   for(i=0;i<5;i++)
   {
      assert(strcmp(large_test[i],target_test[i])==0);
   }
   assert(in_list("alert",target_test,5)==true);
   assert(in_list("dan",target_test,5)==false);
   assert(in_list("beat",target_test,5)==true);
}


void move_up_one(int start_pos,char word_list[][MAXWORDSIZE],int size)
{
   int i;
   for(i=size-1;i>start_pos;i--)
   {
      strcpy(word_list[i],word_list[i-1]);
   }
}


void put_word_in(char word[],char word_list[][MAXWORDSIZE],int size)
{
   int i;
   i=0;

   while(strcmp(word,word_list[i])>0)
   {
      if(strcmp(word_list[i],"")==0)
      {
         strcpy(word_list[i],word);
         return;
      }
      i++;
   }

   move_up_one(i,word_list,size);
   strcpy(word_list[i],word);
}





void read_file_in(char word_array[][MAXWORDSIZE],int arr_size, char* filename)
{
   FILE *ifp;
   int slen;
   char word[MAXWORDSIZE];

   if((ifp=fopen(filename,"r"))==NULL)
   {
      fprintf(stderr,"cant open\n");
      exit(EXIT_FAILURE);
   }
   while(fgets(word,BUFFERSIZE,ifp)!=NULL)
   {
      slen=strlen(word);
      if(word[slen-1]=='\n')
      {
         word[slen-1]='\0';
      }
      put_word_in(word,word_array,arr_size);
   }
   if(fclose(ifp)!=0)
   {
      fprintf(stderr,"not exited\n");
      exit(EXIT_FAILURE);
   }

}



bool in_list(char word[],char word_array[][MAXWORDSIZE],int arr_size)
{
   int lf,rt,mid;
   lf=0;
   rt=arr_size-1;

   while(lf<=rt)
   {
      mid=(lf+rt)/2;
      if(strcmp(word,word_array[mid])==0)
      {
         return true;
      }
      else
      {
         if(strcmp(word,word_array[mid])>0)
         {
            lf=mid+1;
         }
         if(strcmp(word,word_array[mid])<0)
         {
            rt=mid-1;
         }
      }

   }
   return false;
}
