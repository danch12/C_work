#include <stdlib.h>
#include <stdio.h>
#include <assert.h>

#define SIZE 3

typedef int time;
typedef int button;

#define SECSINMIN 60
button button_presser(time start, time options[]);

void test(void);

int main(void)
{
   time start;
   time buttons[SIZE];
   buttons[0]= SECSINMIN*10;
   buttons[1]= SECSINMIN;
   buttons[2]= 10;

   printf("enter a time in seconds:\n");
   scanf("%d", &start);
   printf("need to press %d buttons\n",button_presser(start,buttons));
   return 0;

}

void test(void)
{
   time buttons[SIZE];
   buttons[0]= SECSINMIN*10;
   buttons[1]= SECSINMIN;
   buttons[2]= 10;
   assert(button_presser(600,buttons)==1);
   assert(button_presser(120,buttons)==2);
   assert(button_presser(130,buttons)==3);
   assert(button_presser(125,buttons)==3);
}

button button_presser(time start, time options[])
{

   button count;
   int i;

   count=0;
   for(i=0;i<SIZE;i++)
   {
      count += start/options[i];
      start = start%options[i];
   }
   if(start>0)
   {
      count+=1;
   }
   return count;

}
