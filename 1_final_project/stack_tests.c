#include "stack_funcs.h"
#define TESTEPSILON 0.000001


int main(void)
{
   stack* test_stack;
   char test_str[10000];
   double test_dub,test_dub2;


   test_stack=stack_init();
   assert(test_stack);
   assert(test_stack->size==0);
   assert(test_stack->start==NULL);

   stack_push(test_stack,1.01);
   assert(test_stack->size==1);
   assert((test_stack->start->num-1.01)<TESTEPSILON);

   assert(stack_peek(test_stack,&test_dub));
   assert((test_dub-1.01)<TESTEPSILON);
   test_dub=0;
   assert(stack_pop(test_stack,&test_dub));
   assert((test_dub-1.01)<TESTEPSILON);


   assert(!stack_pop(test_stack,&test_dub));
   assert(!stack_peek(test_stack,&test_dub));
   assert(!stack_pop(NULL,&test_dub));
   assert(!stack_peek(NULL,&test_dub));

   stack_push(test_stack,1.1);
   stack_push(test_stack,2.2);
   stack_push(test_stack,3.3);
   stack_push(test_stack,4.4);
   stack_push(test_stack,5.5);
   stack_push(test_stack,6.6);
   stack_push(test_stack,7.7);

   stack_tostr(test_stack,test_str);
   assert(strcmp(test_str,"7.700000|6.600000|5.500000|4.400000|3.300000|2.200000|1.100000")==0);
   assert(stack_pop(test_stack,&test_dub));
   assert((test_dub-7.7)<TESTEPSILON);
   stack_tostr(test_stack,test_str);
   assert(strcmp(test_str,"6.600000|5.500000|4.400000|3.300000|2.200000|1.100000")==0);

   assert(stack_peek(test_stack,&test_dub));
   assert((test_dub-6.6)<TESTEPSILON);

   assert(stack_free(test_stack));
   test_stack=stack_init();

   for(test_dub2=0;test_dub2<100;test_dub2+=0.1)
   {

      stack_push(test_stack,test_dub2);
   }
   for(test_dub2=100;test_dub2>=0;test_dub2-=0.1)
   {


      assert(stack_pop(test_stack,&test_dub));
      assert(test_dub-test_dub2<TESTEPSILON);
   }
   assert(stack_free(test_stack));
   return 0;
}
