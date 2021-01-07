#include "array_funcs.h"

#define EPSILON 0.00001


bool compare_doubles(double d_1, double d_2);

int main(void)
{
   turt_arr* test_arr;
   double test_d;
   int i;
   test_arr=arr_init();
   assert(append_arr(1.1,test_arr));
   assert(compare_doubles(test_arr->head->data,1.1));
   assert(append_arr(2.2,test_arr));
   assert(compare_doubles(test_arr->head->data,1.1));
   assert(compare_doubles(test_arr->tail->data,2.2));
   assert(append_arr(3.3,test_arr));
   assert(compare_doubles(test_arr->head->data,1.1));
   assert(compare_doubles(test_arr->head->next->data,2.2));
   assert(compare_doubles(test_arr->tail->data,3.3));
   assert(remove_val(test_arr,0));
   assert(test_arr->size==2);
   assert(compare_doubles(test_arr->head->data,2.2));

   assert(append_arr(1.1,test_arr));
   assert(remove_val(test_arr,2));
   assert(compare_doubles(test_arr->tail->data,3.3));
   assert(test_arr->size==2);
   assert(remove_val(test_arr,1));
   assert(compare_doubles(test_arr->tail->data,2.2));
   assert(test_arr->size==1);
   assert(remove_val(test_arr,0));
   assert(test_arr->size==0);
   assert(!test_arr->head);
   assert(append_arr(1.1,test_arr));
   assert(remove_val(test_arr,0));
   assert(remove_val(test_arr,0));



   assert(free_arr(test_arr));

   test_arr=arr_init();

   assert(append_arr(1.1,test_arr));
   assert(insert_arr(2,test_arr,0));
   assert(compare_doubles(test_arr->head->data,2));
   assert(insert_arr(3,test_arr,1));
   assert(compare_doubles(test_arr->head->next->data,3));
   assert(get_arr_val(test_arr,1,&test_d));
   assert(compare_doubles(test_d,3));

   assert(get_arr_val(test_arr,0,&test_d));
   assert(compare_doubles(test_d,2));

   assert(insert_arr(7,test_arr,10000));
   assert(get_arr_val(test_arr,3,&test_d));
   assert(compare_doubles(test_d,7));


   assert(change_val_arr(90,3,test_arr));
   assert(get_arr_val(test_arr,3,&test_d));
   assert(compare_doubles(test_d,90));

   assert(!change_val_arr(90,1000,test_arr));
   assert(free_arr(test_arr));


   test_arr=arr_init();

   for(i=0;i<10000;i++)
   {
      assert(test_arr->size==(unsigned int)i);
      assert(append_arr(i,test_arr));
   }
   for(i=0;i<10000;i++)
   {
      assert(get_arr_val(test_arr,i,&test_d));
      assert(compare_doubles(test_d,(double)i));

   }
   assert(free_arr(test_arr));
   assert(!append_arr(10,NULL));
   assert(!insert_arr(0,NULL,0));
   assert(!get_arr_val(NULL,0,&test_d));
   assert(!remove_val(NULL,0));
   assert(!change_val_arr(1,3,NULL));
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
