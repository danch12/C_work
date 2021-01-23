#include "../src/unity.h"
#include "../ADTS_and_general/stacks/stack_funcs.h"

#define EPSILON 0.000001


typedef struct test_struct
{
   int id;
}test_struct;
/*tests similar to the ones in your
 github just with void pointers now*/

void setUp(void)
{

}

void tearDown(void)
{

}

void test_stack_push_one(void)
{
   stack* test_stack;
   double test_dub;
   void* test_ptr;
   test_stack=stack_init(sizeof(double));
   TEST_ASSERT_TRUE(test_stack);
   TEST_ASSERT_TRUE(test_stack->size==0);
   TEST_ASSERT_TRUE(test_stack->start==NULL);
   test_dub=1.01;
   stack_push(test_stack,&test_dub);
   TEST_ASSERT_TRUE(test_stack->size==1);
   TEST_ASSERT_EQUAL_DOUBLE(*(double*)test_stack->start->data,1.01);
   test_ptr=NULL;
   TEST_ASSERT_TRUE(stack_peek(test_stack,&test_ptr));
   TEST_ASSERT_EQUAL_DOUBLE(*(double*)test_ptr,1.01);
   test_dub=0;
   TEST_ASSERT_TRUE(stack_pop(test_stack,&test_ptr));
   TEST_ASSERT_EQUAL_DOUBLE(*(double*)test_ptr,1.01);
   free(test_ptr);
   TEST_ASSERT_TRUE(test_stack->size==0);

   TEST_ASSERT_TRUE(!stack_pop(test_stack,&test_ptr));
   TEST_ASSERT_TRUE(!stack_peek(test_stack,&test_ptr));
   TEST_ASSERT_TRUE(!stack_pop(NULL,&test_ptr));
   TEST_ASSERT_TRUE(!stack_peek(NULL,&test_ptr));
   stack_free(test_stack);
}

void test_stack_push_many(void)
{

   stack* test_stack;
   char test_str[10000];
   double test_dub;
   void* test_ptr;
   test_ptr=NULL;
   test_stack=stack_init(sizeof(double));
   test_dub=1.1;
   stack_push(test_stack,&test_dub);
   test_dub=2.2;
   stack_push(test_stack,&test_dub);
   test_dub=3.3;
   stack_push(test_stack,&test_dub);
   test_dub=4.4;
   stack_push(test_stack,&test_dub);
   test_dub=5.5;
   stack_push(test_stack,&test_dub);
   test_dub=6.6;
   stack_push(test_stack,&test_dub);
   test_dub=7.7;
   stack_push(test_stack,&test_dub);

   stack_tostr(test_stack,test_str);

   TEST_ASSERT_TRUE(strcmp(test_str,"7.700000|6.600000|5.500000|4.400000|3.300000|2.200000|1.100000")==0);
   TEST_ASSERT_TRUE(stack_pop(test_stack,&test_ptr));
   TEST_ASSERT_EQUAL_DOUBLE(*(double*)test_ptr,7.7);
   free(test_ptr);
   stack_tostr(test_stack,test_str);
   TEST_ASSERT_TRUE(strcmp(test_str,"6.600000|5.500000|4.400000|3.300000|2.200000|1.100000")==0);

   TEST_ASSERT_TRUE(stack_peek(test_stack,&test_ptr));
   TEST_ASSERT_EQUAL_DOUBLE(*(double*)test_ptr,6.6);

   TEST_ASSERT_TRUE(stack_free(test_stack));
}

void test_stack_stress(void)
{
   stack* test_stack;
   void* test_ptr;
   double test_dub2;
   test_stack=stack_init(sizeof(double));

   for(test_dub2=0;test_dub2<100;test_dub2+=0.1)
   {

      stack_push(test_stack,&test_dub2);
   }
   for(test_dub2=100;test_dub2>=0;test_dub2-=0.1)
   {
      TEST_ASSERT_TRUE(stack_pop(test_stack,&test_ptr));
      TEST_ASSERT_DOUBLE_WITHIN(EPSILON,*(double*)test_ptr,test_dub2);
      free(test_ptr);
   }
   TEST_ASSERT_TRUE(stack_free(test_stack));
}


void test_stack_struct(void)
{
   stack* test_stack;
   void* test_ptr;
   test_struct* test_s;
   int i;

   test_stack=stack_init(sizeof(test_struct));
   test_s=safe_calloc(1,sizeof(test_struct));
   test_s->id =10;
   stack_push(test_stack,test_s);
   TEST_ASSERT_TRUE(stack_peek(test_stack,&test_ptr));
   ((test_struct*)test_ptr)->id=5;
   TEST_ASSERT_TRUE(stack_pop(test_stack,&test_ptr));
   TEST_ASSERT_TRUE(((test_struct*)test_ptr)->id==5);
   free(test_ptr);
   stack_free(test_stack);
   free(test_s);
   test_stack=stack_init(sizeof(test_struct));

   test_s=safe_calloc(1,sizeof(test_struct));
   test_s->id =10;
   stack_push(test_stack,test_s);
   TEST_ASSERT_TRUE(stack_pop(test_stack,&test_ptr));
   free(test_ptr);
   for(i=0;i<1000;i++)
   {
      test_s->id =i;
      stack_push(test_stack,test_s);
   }

   for(i=999;i>=0;i--)
   {
      TEST_ASSERT_TRUE(stack_pop(test_stack,&test_ptr));

      TEST_ASSERT_TRUE(((test_struct*)test_ptr)->id==i);
      free(test_ptr);
   }
   free(test_s);
   TEST_ASSERT_TRUE(stack_free(test_stack));
}

int main(void)
{
   UNITY_BEGIN();
   RUN_TEST(test_stack_push_one);
   RUN_TEST(test_stack_push_many);
   RUN_TEST(test_stack_stress);
   RUN_TEST(test_stack_struct);
   return UNITY_END();
}
