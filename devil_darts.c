#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <string.h>
#include <assert.h>

#define MAXNUM 20

void fill_array(int arr[]);
int sum_three_sq(int a,int b,int c);
void stringify(int arr[],char string[],int arr_size);
int sum_array(int arr[],int size);
void random_swap(int arr[],int size);
void swap(int* i, int* p);
void random_swap_loop(int arr[],int size);
int wrap_around(int max_num,int position);
void test(void);

int main(void)
{
   int board[20];
   srand(time(NULL));
   test();
   fill_array(board);
   random_swap_loop(board,20);
   return 0;

}

void test(void)
{
   int i;
   int test_arr[20]={20,1,18,4,13,6,10,15,2,17,3,19,7,16,8,11,14,9,12,5};
   int one_arr[20]={1,1,1};
   printf("%d\n",sum_array(test_arr,20));
   swap(&one_arr[0],&one_arr[10]);
   random_swap(one_arr,20);

}



void fill_array(int arr[])
{
   int i;

   for(i=1;i<=MAXNUM;i++)
   {
      arr[i-1]=i;

   }
}

int sum_three_sq(int a,int b,int c)
{
   return ((a+b+c)*(a+b+c));
}


int sum_array(int arr[],int size)
{
   int i;
   int total;
   int pos_1,pos_2,pos_3;
   total=0;
   for(i=0;i<size;i++)
   {

      pos_1=wrap_around(size,i);
      pos_2=wrap_around(size,i+1);
      pos_3=wrap_around(size,i+2);

      total+=sum_three_sq(arr[pos_1],arr[pos_2],arr[pos_3]);
   }
   return total;
}

void swap(int* i, int* p)
{
   int temp;
   temp=*i;
   *i= *p;
   *p=temp;
}

void random_swap(int arr[],int size)
{
   int pos_1,pos_2;
   pos_1= rand()%size;
   pos_2= rand()%size;
   swap(&arr[pos_1],&arr[pos_2]);
}


void random_swap_loop(int arr[],int size)
{
   int i;
   int pos_1,pos_2;
   int best;
   int temp;
   best= sum_array(arr,size);
   for(i=0;i<5000000;i++)
   {
      pos_1= rand()%size;
      pos_2= rand()%size;
      swap(&arr[pos_1],&arr[pos_2]);
      temp= sum_array(arr,size);
      if(temp>best)
      {
         swap(&arr[pos_1],&arr[pos_2]);
      }
      else
      {
         best=temp;
      }
   }
   printf("%d is best total\n",best);
   for(i=0;i<size;i++)
   {
      printf("%d\n",arr[i]);
   }
}


int wrap_around(int max_num,int position)
{
   if(position>=max_num)
   {
      return abs(max_num-position);
   }
   else
   {
      return position;
   }
}
