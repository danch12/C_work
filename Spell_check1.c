#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <string.h>
#include <assert.h>


#define MAXWORDSIZE 50
#define MAXWORDS 400000
#define BUFFERSIZE 1000

typedef enum bool {false,true} bool;

void move_up_one(int start_pos,char word_list[][MAXWORDSIZE],int size);
void put_word_in(char word[],char word_list[][MAXWORDSIZE],int size);
void read_file_in(char word_array[][MAXWORDSIZE],int arr_size, char* filename);
bool in_list(char word[],char word_array[][MAXWORDSIZE],int arr_size);
int find_start(const char k_word[], char word_array[][MAXWORDSIZE],int lf,int r);
int word_score(const char word[]);
bool interp_search(const char word[],char word_array[][MAXWORDSIZE],int lf, int r);
void test(void);

int main(int argc,char* argv[])
{
   int i;
   char word_arr[100000][MAXWORDSIZE]={""};
   test();
   if(argc<2)
   {
      fprintf(stderr,"not enough args\n");
      exit(EXIT_FAILURE);
   }
   else
   {
      read_file_in(word_arr,100000,argv[1]);
      for(i=0;i<10;i++)
      {
         printf("%s\n",word_arr[i]);
      }
      printf("%d\n",in_list("beat",word_arr,100000));

      printf("%d\n",in_list("alter",word_arr,100000));
   }


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
   assert(in_list("alter",target_test,5)==true);

   assert(word_score(test_arr[0])==207);
   assert(word_score("a")==97);

   assert(interp_search("beat",target_test,0,4)==true);
   assert(interp_search("notin",target_test,0,4)==false);

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
   /*go past blanks if there are any*/
   while(strcmp(word_array[rt],"")==0)
   {
      rt--;
   }
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


bool interp_search(const char new_word[],char word_array[][MAXWORDSIZE],int lf, int r)
{
   int m,k;

   k=word_score(new_word);

   while(lf<=r)
   {
      m=find_start(new_word,word_array,lf,r);


      if(strcmp(new_word,word_array[m])==0)
      {
         return true;
      }
      else
      {
         if(strcmp(new_word,word_array[m])>0)
         {
            lf=m+1;
         }
         if(strcmp(new_word,word_array[m])<0)
         {
            r=m-1;
         }
      }
   }
   return false;
}


int word_score(const char word[])
{
   if(strlen(word)>1)
   {
      return (int) word[0] + (int) word[1];
   }
   else
   {
      return (int) word[0];
   }

}


int find_start(const char k_word[], char word_array[][MAXWORDSIZE],int lf,int r)
{
   double md,l_first,l_last;
   int m,first_word,last_word,k;
   k=word_score(k_word);
   l_first=word_score(word_array[lf]);
   l_last=word_score(word_array[r]);

   md= (double)(lf)+(((double)k-(double)l_first)/\
      ((double)l_last-(double)l_first)*((double)r-(double)lf));
   m=0.5+md;
   return m;
}
