#include "../src/unity.h"
#include "../ADTS_and_general/stacks/stack_funcs.h"

#define EPSILON 0.000001

/*tests similar to the ones in your github*/

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

   test_stack=stack_init();
   TEST_ASSERT_TRUE(test_stack);
   TEST_ASSERT_TRUE(test_stack->size==0);
   TEST_ASSERT_TRUE(test_stack->start==NULL);
   stack_push(test_stack,1.01);
   TEST_ASSERT_TRUE(test_stack->size==1);
   TEST_ASSERT_EQUAL_DOUBLE(test_stack->start->num,1.01);

   TEST_ASSERT_TRUE(stack_peek(test_stack,&test_dub));
   TEST_ASSERT_EQUAL_DOUBLE(test_dub,1.01);
   test_dub=0;
   TEST_ASSERT_TRUE(stack_pop(test_stack,&test_dub));
   TEST_ASSERT_EQUAL_DOUBLE(test_dub,1.01);
   TEST_ASSERT_TRUE(test_stack->size==0);

   TEST_ASSERT_TRUE(!stack_pop(test_stack,&test_dub));
   TEST_ASSERT_TRUE(!stack_peek(test_stack,&test_dub));
   TEST_ASSERT_TRUE(!stack_pop(NULL,&test_dub));
   TEST_ASSERT_TRUE(!stack_peek(NULL,&test_dub));
}

void test_stack_push_many(void)
{

   stack* test_stack;
   char test_str[10000];
   double test_dub;
   test_stack=stack_init();
   stack_push(test_stack,1.1);
   stack_push(test_stack,2.2);
   stack_push(test_stack,3.3);
   stack_push(test_stack,4.4);
   stack_push(test_stack,5.5);
   stack_push(test_stack,6.6);
   stack_push(test_stack,7.7);

   stack_tostr(test_stack,test_str);
   TEST_ASSERT_TRUE(strcmp(test_str,"7.700000|6.600000|5.500000|4.400000|3.300000|2.200000|1.100000")==0);
   TEST_ASSERT_TRUE(stack_pop(test_stack,&test_dub));
   TEST_ASSERT_EQUAL_DOUBLE(test_dub,7.7);
   stack_tostr(test_stack,test_str);
   TEST_ASSERT_TRUE(strcmp(test_str,"6.600000|5.500000|4.400000|3.300000|2.200000|1.100000")==0);

   TEST_ASSERT_TRUE(stack_peek(test_stack,&test_dub));
   TEST_ASSERT_EQUAL_DOUBLE(test_dub,6.6);

   TEST_ASSERT_TRUE(stack_free(test_stack));
}

void test_stack_stress(void)
{
   stack* test_stack;

   double test_dub,test_dub2;
   test_stack=stack_init();

   for(test_dub2=0;test_dub2<100;test_dub2+=0.1)
   {

      stack_push(test_stack,test_dub2);
   }
   for(test_dub2=100;test_dub2>=0;test_dub2-=0.1)
   {
      TEST_ASSERT_TRUE(stack_pop(test_stack,&test_dub));
      TEST_ASSERT_DOUBLE_WITHIN(EPSILON,test_dub,test_dub2);
   }
   TEST_ASSERT_TRUE(stack_free(test_stack));
}

int main(void)
{
   UNITY_BEGIN();
   RUN_TEST(test_stack_push_one);
   RUN_TEST(test_stack_push_many);
   RUN_TEST(test_stack_stress);
   return UNITY_END();
}
