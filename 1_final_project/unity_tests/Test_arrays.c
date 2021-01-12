#include "../src/unity.h"
#include "../ADTS_and_general/arrays/array_funcs.h"


void setUp(void)
{

}

void tearDown(void)
{

}

void test_append(void)
{
   turt_arr* test_arr;

   test_arr=arr_init();
   TEST_ASSERT_TRUE(append_arr(1.1,test_arr));
   TEST_ASSERT_EQUAL_DOUBLE(test_arr->head->data,1.1);
   TEST_ASSERT_TRUE(append_arr(2.2,test_arr));
   TEST_ASSERT_EQUAL_DOUBLE(test_arr->head->data,1.1);
   TEST_ASSERT_EQUAL_DOUBLE(test_arr->tail->data,2.2);
   TEST_ASSERT_TRUE(append_arr(3.3,test_arr));
   TEST_ASSERT_EQUAL_DOUBLE(test_arr->head->data,1.1);
   TEST_ASSERT_EQUAL_DOUBLE(test_arr->head->next->data,2.2);
   TEST_ASSERT_EQUAL_DOUBLE(test_arr->tail->data,3.3);

   TEST_ASSERT_TRUE(free_arr(test_arr));

}

void test_remove(void)
{
   turt_arr* test_arr;

   test_arr=arr_init();
   TEST_ASSERT_TRUE(append_arr(1.1,test_arr));
   TEST_ASSERT_EQUAL_DOUBLE(test_arr->head->data,1.1);
   TEST_ASSERT_TRUE(append_arr(2.2,test_arr));
   TEST_ASSERT_EQUAL_DOUBLE(test_arr->head->data,1.1);
   TEST_ASSERT_EQUAL_DOUBLE(test_arr->tail->data,2.2);
   TEST_ASSERT_TRUE(append_arr(3.3,test_arr));
   TEST_ASSERT_EQUAL_DOUBLE(test_arr->head->data,1.1);
   TEST_ASSERT_EQUAL_DOUBLE(test_arr->head->next->data,2.2);
   TEST_ASSERT_EQUAL_DOUBLE(test_arr->tail->data,3.3);
   TEST_ASSERT_TRUE(remove_val(test_arr,0));
   TEST_ASSERT_TRUE(test_arr->size==2);
   TEST_ASSERT_EQUAL_DOUBLE(test_arr->head->data,2.2);

   TEST_ASSERT_TRUE(append_arr(1.1,test_arr));
   TEST_ASSERT_TRUE(remove_val(test_arr,2));
   TEST_ASSERT_EQUAL_DOUBLE(test_arr->tail->data,3.3);
   TEST_ASSERT_TRUE(test_arr->size==2);
   TEST_ASSERT_TRUE(remove_val(test_arr,1));
   TEST_ASSERT_EQUAL_DOUBLE(test_arr->tail->data,2.2);
   TEST_ASSERT_TRUE(test_arr->size==1);
   TEST_ASSERT_TRUE(remove_val(test_arr,0));
   TEST_ASSERT_TRUE(test_arr->size==0);
   TEST_ASSERT_TRUE(!test_arr->head);
   TEST_ASSERT_TRUE(append_arr(1.1,test_arr));
   TEST_ASSERT_TRUE(remove_val(test_arr,0));
   TEST_ASSERT_TRUE(remove_val(test_arr,0));
   TEST_ASSERT_TRUE(free_arr(test_arr));
}

void test_get_val(void)
{
   turt_arr* test_arr;
   double test_d;
   int i;
   int y;
   test_arr=arr_init();
   y=0;
   for(i=0;i<10000;i++)
   {
      TEST_ASSERT_TRUE(arr_len(test_arr,&y));
      TEST_ASSERT_TRUE(y==i);
      TEST_ASSERT_TRUE(append_arr(i,test_arr));
   }
   for(i=0;i<10000;i++)
   {
      TEST_ASSERT_TRUE(get_arr_val(test_arr,i,&test_d));
      TEST_ASSERT_EQUAL_DOUBLE(test_d,(double)i);

   }
   TEST_ASSERT_TRUE(free_arr(test_arr));
}

void test_insert(void)
{
   turt_arr* test_arr;
   double test_d;
   test_arr=arr_init();

   TEST_ASSERT_TRUE(append_arr(1.1,test_arr));
   TEST_ASSERT_TRUE(insert_arr(2,test_arr,0));
   TEST_ASSERT_EQUAL_DOUBLE(test_arr->head->data,2);
   TEST_ASSERT_TRUE(insert_arr(3,test_arr,1));
   TEST_ASSERT_EQUAL_DOUBLE(test_arr->head->next->data,3);
   TEST_ASSERT_TRUE(get_arr_val(test_arr,1,&test_d));
   TEST_ASSERT_EQUAL_DOUBLE(test_d,3);

   TEST_ASSERT_TRUE(get_arr_val(test_arr,0,&test_d));
   TEST_ASSERT_EQUAL_DOUBLE(test_d,2);

   TEST_ASSERT_TRUE(insert_arr(7,test_arr,10000));
   TEST_ASSERT_TRUE(get_arr_val(test_arr,3,&test_d));
   TEST_ASSERT_EQUAL_DOUBLE(test_d,7);
   TEST_ASSERT_TRUE(free_arr(test_arr));
}


void test_change_val(void)
{
   turt_arr* test_arr;
   double test_d;
   test_arr=arr_init();

   TEST_ASSERT_TRUE(append_arr(1.1,test_arr));

   TEST_ASSERT_TRUE(append_arr(2.2,test_arr));

   TEST_ASSERT_TRUE(change_val_arr(90,0,test_arr));
   TEST_ASSERT_TRUE(get_arr_val(test_arr,0,&test_d));
   TEST_ASSERT_EQUAL_DOUBLE(test_d,90);

   TEST_ASSERT_TRUE(change_val_arr(100,1,test_arr));
   TEST_ASSERT_TRUE(get_arr_val(test_arr,1,&test_d));
   TEST_ASSERT_EQUAL_DOUBLE(test_d,100);

   TEST_ASSERT_TRUE(!change_val_arr(90,1000,test_arr));
   TEST_ASSERT_TRUE(free_arr(test_arr));
}

void test_nulls(void)
{
   double test_d;
   TEST_ASSERT_TRUE(!append_arr(10,NULL));
   TEST_ASSERT_TRUE(!insert_arr(0,NULL,0));
   TEST_ASSERT_TRUE(!get_arr_val(NULL,0,&test_d));
   TEST_ASSERT_TRUE(!remove_val(NULL,0));
   TEST_ASSERT_TRUE(!change_val_arr(1,3,NULL));
}

int main(void)
{
   UNITY_BEGIN();
   RUN_TEST(test_append);
   RUN_TEST(test_remove);
   RUN_TEST(test_get_val);
   RUN_TEST(test_insert);
   RUN_TEST(test_change_val);
   RUN_TEST(test_nulls);

   return UNITY_END();
}
