#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <string.h>
#include <assert.h>


int main(void)
{

}


bool inbounds(int x,int max_size)
{
   if(x>=0&&x<max_size)
   {
      return true;
   }
   return false;
}

void make_hist(const char word[],int letter_hist[],int hist_size)
{
   int i;

   for(i=0;i<hist_size;i++)
   {
      letter_hist[i]=0;
   }
   i=0;
   while(word[i]!='\0')
   {
      letter_hist[word[i]-'a']++;
      i++;
   }
}

/*stringify single digit array*/
void stringify_array(int arr[],int arr_size,char string[])
{
   int i;
   for(i=0;i<arr_size;i++)
   {
      string[i]=arr[i]+'0';
   }
   string[i]='\0';
}


void stringify_array_2d(int arr[],int num_rows,int num_cols,char string[])
{
   int i,j,pos;
   pos=0;
   for(i=0;i<num_rows;i++)
   {
      for(j=0;j<num_cols;j++)
      {
         string[pos]=arr[i][j]+'0';
         pos++;
      }
      string[pos]='\n';
      pos++;
   }
   string[i]='\0';
}

bool same_array(const int arr_1[],\
    const int arr_2[],int size)
{
   int i;
   for(i=0;i<size;i++)
   {
      if(arr_1[i]!=arr_2[i])
      {
         return false;
      }
   }
   return true;
}
