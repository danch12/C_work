#include <stdio.h>
#include<stdlib.h>
#include <assert.h>
#include <time.h>

#define LOWER 1
#define SIZE 10

typedef int song;
typedef enum bool {false,true} bool;
void test(void);

song* make_empty(int num);
void order_array(song arr[],int size);
song* order_gen(int num);
void swap(song* p, song* q);
void print_arr(song* arr, int size);

int main(void)
{

   song* random_selection;
   song second_test[SIZE]={1,2,3,4,5,6,7,8,9,10};

   srand(time(NULL));
   test();
   random_selection=order_gen(6);
   order_array(second_test,SIZE);
   print_arr(second_test,SIZE);
   printf("and another one\n");
   print_arr(random_selection,6);
   free(random_selection);
   return 0;

}

void test(void)
{
   int i,j;
   song* test_arr;
   song order_array_test[SIZE]={1,2,3,4,5,6,7,8,9,10};


   test_arr=order_gen(SIZE);

   /* testing that values stay within bounds */
   for(i=0;i<SIZE;i++)
   {
      assert(test_arr[i]<SIZE+1 &&test_arr[i]>0);
   }
   /* testing for duplicates*/
   for(i=0;i<SIZE;i++)
   {
      for(j=i+1;j<SIZE;j++)
      {
         assert(test_arr[i]!=test_arr[j]);
      }
   }
   free(test_arr);

   order_array(order_array_test,SIZE);
   for(i=0;i<SIZE;i++)
   {
      assert(order_array_test[i]<=10&& order_array_test[i]>=1);
   }

   for(i=0;i<SIZE;i++)
   {
      for(j=i+1;j<SIZE;j++)
      {
         assert(order_array_test[i]!=order_array_test[j]);
      }
   }

}


song* make_empty(int num)
{
   song *arr;
   int i;
   arr=(song *) malloc(sizeof(song)*num);
   if(arr==NULL)
   {
      printf("array could not be created\n");
      exit(1);
   }
   for(i=0;i<num;i++)
   {
      arr[i]=0;
   }
   return arr;
}


song* order_gen(int num)
{
   song *arr;
   song position;
   int count,i;
   bool state;
   state=true;
   arr= make_empty(num);
   count=0;
   while(count<num)
   {
   position= rand() % num + LOWER;
   for(i=0;i<num;i++)
   {
      if(arr[i]==position)
      {
         state=false;
      }
   }
   if(state==true)
   {
      arr[count]= position;
      count+=1;
   }

   state=true;
   }

   return arr;
}

/* swaps values in array randomly*/
void order_array(song arr[],int size)
{
   int i;
   song j,k;
   for(i=0;i<size;i++)
   {
      j=rand() %size;
      k=rand() %size;
      swap(&arr[k],&arr[j]);
   }

}

void swap(song* p, song* q)
{
   song temp;
   temp= *q;
   *q= *p;
   *p = temp;
}

void print_arr(song* arr, int size)
{
   int i;
   for(i=0;i<size;i++)
   {
      printf("%d\n",arr[i]);
   }
}
