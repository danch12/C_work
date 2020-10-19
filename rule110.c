#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <assert.h>

#define LOWER 1
#define SIZE 30
#define ITERS 10
#define TOCHAR 48

typedef enum state {off, on} state;
typedef enum choice {gen_random,only_one} choice;
typedef int rule;

state* generate_random_row(int length);
state* generate_random_1(int length);

/*determines if a cell is on or off given the previous row and a rule*/
state on_or_off(int position,state previous[],state (*rule)(state,state,state));

/* fills row based on previous row and specified rule*/
void fill_row(state above[],state tofill[],int fill_size, \
               state (*rule)(state,state,state));

void copy_array(state to_copy[], state target[],int size);
void print_arr(state arr[],int size);


/*different rules for generating new rows*/
state rule110(state start, state mid, state end);
state rule30(state start, state mid, state end);

void test(void);

int main(void)
{
   choice decision;
   rule rulechoice;
   int i;
   state* current;
   state* previous;
   state (*rule_ptr)(state,state,state);

   previous = malloc(sizeof(int)*SIZE);

   srand(time(NULL));
   test();

   printf("enter a choice 0 for random or 1 for only one\n");
   if(scanf("%d",&decision)!=1)
   {
      printf("invalid number\n");
      return 1;
   }

   printf("enter a choice for rule you want 1 for 110 and 3 for 30\n");
   if(scanf("%d", &rulechoice)!=1)
   {
      printf("invalid number\n");
      return 1;
   }
   /*put this in a function*/
   if(rulechoice==1)
   {
      rule_ptr=&rule110;
   }
   else if(rulechoice==3)
   {
      rule_ptr=&rule30;
   }
   else
   {
      printf("invalid rule\n");
      return 1;
   }

   if(decision==gen_random)
   {
      current = generate_random_row(SIZE);
   }
   else if(decision==only_one)
   {
      current= generate_random_1(SIZE);
   }
   else
   {
      printf("wrong number entered\n");
      return 1;
   }
   for(i=0;i<ITERS;i++)
   {
      print_arr(current,SIZE);
      copy_array(current,previous,SIZE);
      fill_row(previous,current,SIZE,rule_ptr);
   }

   free(current);
   free(previous);


   return 0;
}

void test(void)
{
   int i,on_count;
   state* test_row;
   state on_or_off_test[]={on,on,on,off,on,off,off,on};
   state copy_arr_test[8]={off};

   /*check to see if weird numbers pop up in gen of first row*/
   test_row=generate_random_row(SIZE);
   for(i=0;i<SIZE;i++)
   {
      assert(test_row[i]==off || test_row[i] ==on);
   }
   free(test_row);

   test_row=generate_random_1(SIZE);
   for(i=0;i<SIZE;i++)
   {
      assert(test_row[i]==off || test_row[i] ==on);
   }

   /*make sure only one on counted*/
   on_count=0;

   for(i=0;i<SIZE;i++)
   {
      if(test_row[i]==on)
      {
         on_count+=1;
      }
   }
   assert(on_count==1);
   free(test_row);

   assert(rule30(1,1,1)==off);
   assert(rule30(1,0,0)==on);
   assert(rule30(1,0,1)==off);
   assert(rule30(0,0,1)==on);
   assert(rule110(1,1,1)==off);
   assert(rule110(1,1,0)==on);
   assert(rule110(0,0,0)==off);

   /*not going to test the first or last position as
   will have an assert statement within function that fills array*/
   assert(on_or_off(1,on_or_off_test,rule110)==off);
   assert(on_or_off(2,on_or_off_test,rule110)==on);
   assert(on_or_off(3,on_or_off_test,rule110)==on);

   assert(on_or_off(1,on_or_off_test,rule30)==off);
   assert(on_or_off(6,on_or_off_test,rule30)==on);
   assert(on_or_off(5,on_or_off_test,rule30)==on);

   copy_array(on_or_off_test, copy_arr_test,8);
   for(i=0;i<8;i++)
   {
      assert(on_or_off_test[i]==copy_arr_test[i]);
   }

}

state* generate_random_row(int length)
{
   state* arr;
   int num,i;
   arr= (state *)malloc(sizeof(state)*length);
   for(i=0;i<length;i++)
   {
   num = rand();
   if(num%2==0)
   {
      arr[i]=0;
   }
   else
   {
      arr[i]=1;
   }
   }
   return arr;

}


state* generate_random_1(int length)
{
   state* arr;
   int num,i;
   arr= (state *)malloc(sizeof(state)*length);
   for(i=0;i<length;i++)
   {
      arr[i]=0;
   }
   num =rand()% length-1 +LOWER;
   arr[num]=1;
   return arr;

}


state on_or_off(int position,state previous[],state (*rule)(state,state,state))
{

   state start, mid, end;
   start = previous[position-1];
   mid = previous[position];
   end = previous[position+1];
   return rule(start,mid,end);
}

state rule30(state start, state mid, state end)
{
   char bin_arr[3];
   char *ptr;
   long result;
   bin_arr[0]= (char) start+TOCHAR;
   bin_arr[1]=(char) mid+TOCHAR;
   bin_arr[2]=(char) end+TOCHAR;
   result = strtol(bin_arr,&ptr,2);
   if(result==7||result==5||result==0||result==6)
   {
      return off;
   }
   else
   {
      return on;
   }
}

state rule110(state start, state mid, state end)
{
   char bin_arr[3];
   char *ptr;
   long result;
   bin_arr[0]= (char) start+TOCHAR;
   bin_arr[1]=(char) mid+TOCHAR;
   bin_arr[2]=(char) end+TOCHAR;
   result = strtol(bin_arr,&ptr,2);
   if(result==7||result==4 || result==0)
   {
      return off;
   }
   else
   {
      return on;
   }
}




void fill_row(state above[],state tofill[],int fill_size, \
               state (*rule)(state,state,state))
{
   int i;
   /*first and last cant be filled normally*/
   tofill[0]=0;
   tofill[fill_size-1]=0;
   for(i=1;i<fill_size-1;i++)
   {
      tofill[i]= on_or_off(i,above,rule);
   }
}

void copy_array(state to_copy[], state target[],int size)
{
   /*copys up to length of target array*/
   /*assumes to copy is at least as big*/
   int i;
   for(i=0;i<size;i++)
   {
      target[i]=to_copy[i];
   }
}

void print_arr(state arr[],int size)
{
   int i;
   /*want to print on same line for this*/
   for(i=0;i<size;i++)
   {
      printf("%d ", arr[i]);
   }
   printf("\n");
}
