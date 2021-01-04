#include "function_stack_funcs.h"

#define EPSILON 0.000001

typedef struct test_struct
{
   double num;
}test_struct;

bool compare_doubles(double d_1, double d_2);


int main(void)
{
   func_stack* test_stack;
   test_struct* test_s;
   void* voidptr;
   int i;
   test_stack=func_stack_init();
   assert(test_stack);
   assert(test_stack->size==0);
   assert(test_stack->start==NULL);
   test_s=safe_calloc(1, sizeof(test_struct));
   test_s->num =1.01;
   
   func_stack_push(test_stack,test_s);
   assert(test_stack->size==1);
   assert(compare_doubles(((test_struct*)test_stack->start->data)->num,1.01));
   assert(func_stack_peek(test_stack,&voidptr));
   assert(compare_doubles(((test_struct*)voidptr)->num,1.01));
   voidptr=NULL;
   assert(func_stack_pop(test_stack,&voidptr));
   assert(compare_doubles(((test_struct*)voidptr)->num,1.01));
   free(test_s);
   assert(!func_stack_pop(test_stack,&voidptr));
   assert(!func_stack_peek(test_stack,&voidptr));
   assert(!func_stack_pop(NULL,&voidptr));
   assert(!func_stack_peek(NULL,&voidptr));


   for(i=0;i<10;i++)
   {
      test_s=safe_calloc(1, sizeof(test_struct));
      test_s->num=i;
      func_stack_push(test_stack,test_s);
   }

   for(i=9;i>=0;i--)
   {
      assert(func_stack_pop(test_stack,&voidptr));
      assert(compare_doubles(((test_struct*)voidptr)->num,i));
      free(voidptr);
   }
   assert(func_stack_free(test_stack));
   /*im probably going to put a cap on the amount of
   functions we can put on the stack so dont have to worry
   too much about stress testing to a crazy amount*/
   test_stack=func_stack_init();
   for(i=0;i<1000;i++)
   {
      test_s=safe_calloc(1, sizeof(test_struct));
      test_s->num=i;
      assert(test_stack->size==i);
      func_stack_push(test_stack,test_s);

   }
   for(i=999;i>=0;i--)
   {

      assert(func_stack_pop(test_stack,&voidptr));
      assert(test_stack->size==i);
      assert(compare_doubles(((test_struct*)voidptr)->num,i));
      free(voidptr);
   }
   assert(func_stack_free(test_stack));
   return 0;
}



bool compare_doubles(double d_1, double d_2)
{
   double temp;
   temp= d_1 - d_2;
   if(temp<EPSILON && temp > -EPSILON)
   {
      return true;
   }
   return false;
}
