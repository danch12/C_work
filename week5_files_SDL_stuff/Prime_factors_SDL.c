#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <string.h>
#include <assert.h>
#include <math.h>
#include "neillsdl2.h"

#define RADIUSOUTER 5
#define RADIUS 5
#define PI 3.14159265

typedef enum bool {false,true} bool;

typedef struct coords{
   int x;
   int y;
}coords;

int* sieve_of_e(int n);
int size_of_prime_arr(int* prime_arr);
char* prime_factors(int n);
char* stringify(int* array,int size);
void swap(int* a, int* b);
void prime_factors_sdl(int n);
void simple_sort(int arr[],int size);
bool been_in_area(coords seen_locs[],int arr_size,coords new_loc,int radius);
void one_num_sdl(int num);
void make_circle(int num, SDL_Simplewin sw,int start_row, int start_col);
void make_square(int first_num,int second_num,\
               SDL_Simplewin sw,int start_col, int start_row);
void two_num_sdl(int arr[]);
void three_four_num_sdl(int arr[],int num);
void run_sdl(int arr[], int size);
void test(void);

int main(void)
{

   int i;
   char* word;
   i=0;
   test();
   /*prime_arr=sieve_of_e(100);
   while(prime_arr[i]>0)
   {
      printf("%d\n",prime_arr[i]);
      i++;
   }
   free(prime_arr);*/

   word=prime_factors(27000);
   printf("%s\n",word);
   free(word);
   return 0;
}

void test(void)
{
   int i;
   int* sieve_under20;
   int* sieve_under30;

   int primes_under20[8]={2,3, 5, 7, 11, 13, 17, 19};
   int primes_under30[10]={2,3, 5, 7, 11, 13, 17, 19,23, 29};
   int test_list[6]={0,1,2,3,4,-1};
   int test_stringify[6]={3,4,4,5,5,5};
   int test_sdl[2]={4,10};
   char* test_string;
   /*int* sieve_of_e(int n)*/
   sieve_under20=sieve_of_e(20);
   sieve_under30=sieve_of_e(30);
   for(i=0;i<8;i++)
   {
      assert(sieve_under20[i]==primes_under20[i]);
   }
   for(i=0;i<10;i++)
   {
      assert(sieve_under30[i]==primes_under30[i]);
   }
   free(sieve_under20);
   free(sieve_under30);

   assert(size_of_prime_arr(test_list)==5);
   /*int size_of_prime_arr(int* prime_arr)*/

   test_string=stringify(test_stringify,6);
   printf("%s\n",test_string);
   free(test_string);
   simple_sort(test_stringify,6);
   printf("sorted\n");
   for(i=0;i<6;i++)
   {
      printf("%d\n",test_stringify[i]);
   }

   prime_factors_sdl(18);


   /*one_num_sdl(20);*/

}

void one_num_sdl(int num)
{
   SDL_Simplewin sw;
   Neill_SDL_Init(&sw);
   make_circle(num,sw,WWIDTH/2, WHEIGHT/2);

   do
   {
     Neill_SDL_Events(&sw);
   }while(!sw.finished);

   SDL_Quit();
   atexit(SDL_Quit);
}

void three_four_num_sdl(int arr[],int num)
{
   int i,temp;
   assert(num==3||num==4);
   temp=arr[1];
   for(i=2;i<=num;i++)
   {
      temp=temp*arr[i];
   }
   arr[1]=temp;
   two_num_sdl(arr);
}

void two_num_sdl(int arr[])
{
   double first, second,i,third,start_col,base_row,base_col;
   SDL_Simplewin sw;
   first=arr[0];
   second=arr[1];
   base_col=WWIDTH/first;
   base_row=WHEIGHT/first;
   if((int)second%2==0)
   {
      second=second/2;
      third=2;
   }
   else
   {
      third=1;
   }
   Neill_SDL_Init(&sw);
   for(i=0;i<first;i++)
   {
      /*start_row=base_row*i +WHEIGHT/first;*/
      start_col= base_col*i+ ((WWIDTH/(first))/2);
      make_square(second,third,sw,start_col,base_row);
   }
   do
   {
     Neill_SDL_Events(&sw);
   }while(!sw.finished);

   SDL_Quit();
   atexit(SDL_Quit);
}

void make_circle(int num, SDL_Simplewin sw,int start_col, int start_row)
{
   double i,radius_outer,x,y,intervals;

   radius_outer = RADIUSOUTER*num;
   intervals= 2*PI/num;

   for(i=0;i<2*PI;i+=intervals)
   {

      x= radius_outer *cos(i)+start_col;
      y= radius_outer * sin(i)+start_row;

      Neill_SDL_SetDrawColour(&sw, rand()%255,
                                   rand()%255,
                                   rand()%255);

      Neill_SDL_RenderFillCircle(sw.renderer,x,y,RADIUS);
      Neill_SDL_UpdateScreen(&sw);
   }
}

void make_square(int first_num,int second_num,\
               SDL_Simplewin sw,int start_col, int start_row)
{
   int i,j,x,y,prev;
   int pos_count;
   pos_count=0;
   prev=0;
   for(i=0;i<first_num;i++)
   {
      y= start_row + pos_count;
      for(j=0;j<second_num;j++)
      {

         Neill_SDL_SetDrawColour(&sw, rand()%255,
                                      rand()%255,
                                      rand()%255);

         x= start_col+(j*RADIUS*3);

         Neill_SDL_RenderFillCircle(sw.renderer,x,y,RADIUS);
         Neill_SDL_UpdateScreen(&sw);

      }
      pos_count+=RADIUS*2;
   }

}





void prime_factors_sdl(int n)
{
   int* prime_arr;
   int i,pos,count;
   int size;
   int num;
   int* final_arr;
   num=n;
   prime_arr=sieve_of_e(n);
   size=size_of_prime_arr(prime_arr);
   size=size-1;
   count=0;
   for(i=size;i>=0;i--)
   {

      while(num%prime_arr[i]==0)
      {
         count++;
         num/=prime_arr[i];
      }
   }
   final_arr=(int*)malloc(sizeof(int)*count);
   pos=0;
   num=n;
   for(i=size;i>=0;i--)
   {

      while(num%prime_arr[i]==0)
      {
         final_arr[pos]=prime_arr[i];
         pos++;
         num/=prime_arr[i];
      }
   }
   simple_sort(final_arr,pos);
   free(prime_arr);
   run_sdl(final_arr,pos);

}

void run_sdl(int arr[], int size)
{
   switch(size)
   {
      case 1:
      one_num_sdl(arr[0]);
      break;

      case 2:
      two_num_sdl(arr);
      break;

      case 3:
      case 4:
      three_four_num_sdl(arr,size);
      break;
      default:
      fprintf(stderr,"wrong size arr sorry not everything supported\
                     in this iteration of code\n");
      exit(EXIT_FAILURE);

   }
}

void swap(int* a, int* b)
{
   int temp;
   temp= *a;
   *a= *b;
   *b=temp;
}

void simple_sort(int arr[],int size)
{
   int swap_count,i;

   do
   {
      swap_count=0;
      for(i=0;i<size-1;i++)
      {
         if(arr[i]<arr[i+1])
         {
            swap(&arr[i],&arr[i+1]);
            swap_count++;
         }
      }

   } while(swap_count!=0);

}

char* prime_factors(int n)
{
   int* prime_arr;
   int i,pos;
   int size;
   int num;
   int final_arr[10000];
   num=n;
   prime_arr=sieve_of_e(n);
   size=size_of_prime_arr(prime_arr);
   size=size-1;
   pos=0;
   for(i=size;i>=0;i--)
   {

      while(num%prime_arr[i]==0)
      {
         final_arr[pos]=prime_arr[i];
         pos++;
         num/=prime_arr[i];
      }
   }
   /*for(i=0;i<pos;i++)
   {
      printf("%d\n",final_arr[i]);
   }*/

   free(prime_arr);
   return stringify(final_arr,9);
}

char* stringify(int* array,int size)
{
   int count;
   char inter_string[100];
   char final_string[1000];
   char* final_string_point;
   int i;
   strcpy(final_string,"");
   for(i=0;i<size;i++)
   {
      count=1;
      sprintf(inter_string, "%d", array[i]);
      strcat(final_string,inter_string);

      while(i<size-1&&array[i]==array[i+1])
      {
         count++;
         i++;
      }
      if(count>1)
      {
         strcat(final_string,"^");
         sprintf(inter_string, "%d",count);
         strcat(final_string,inter_string);
      }

      strcat(final_string,"x");
   }

   if(strncmp(&final_string[strlen(final_string)-1],"x",1)==0)
   {
      final_string[strlen(final_string)-1]='\0';
   }

   final_string_point=malloc(strlen(final_string)+1);
   strcpy(final_string_point,final_string);
   final_string_point[strlen(final_string)]='\0';
   return final_string_point;
}


int size_of_prime_arr(int* prime_arr)
{
   int count;
   count=0;
   /*added -1 at end of primes list*/
   while(prime_arr[count]>=0)
   {
      count++;
   }
   /*dont want to include negative num*/

   return count;
}


int* sieve_of_e(int n)
{
   bool *p_arr;
   int i;
   int p;
   int count;
   int* prime_arr;

   p_arr= (bool *) malloc(sizeof(bool)*(n+1));
   if(p_arr==NULL)
   {
      fprintf(stderr,"cant open\n");
      exit(EXIT_FAILURE);
   }
   /*set all vals to true*/
   for (i=0 ; i<(n+1); i++)
   {
      p_arr[i]=true;
   }
   /*if val is true go through and turn all of its powers false*/
   for(p=2;p*p<=n;p++)
   {
      if(p_arr[p]==true)
      {
         for(i=p*p; i<=n;i+=p)
         {
            p_arr[i]=false;
         }
      }
   }
   /*count how many still true*/
   count=0;
   for (p=2;p<=n;p++)
   {
      if (p_arr[p])
      {
         count++;
      }
   }
   prime_arr=(int *)malloc(sizeof(int)*(count+1));
   i=0;
   for (p=2;p<=n;p++)
   {
      if (p_arr[p])
      {

         prime_arr[i]=p;
         i++;
      }
   }
   prime_arr[i]=-1;
   free(p_arr);
   return prime_arr;
}
