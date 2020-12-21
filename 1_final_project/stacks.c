#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <string.h>
#include <assert.h>

/*these funcs are pretty
similar to your stack funcs*/

#define MAXLEN 50
#define TESTEPSILON 0.000001
typedef enum bool {false, true} bool;
typedef struct node
{
   /*can store it as a number
   because we dont actually put
   operators on the stack*/
	double num;
  struct node* next;
}node;

typedef node* nodeptr;

typedef struct stack
{
  nodeptr start;
  int size;
}stack;

/*return void then cast when appropriate*/
void* safe_calloc(size_t nitems, size_t size);
stack* stack_init(void);
nodeptr init_node(double d);
void stack_push(stack* s,double d);
bool stack_pop(stack* s, double* new_d);
bool stack_free(stack* s);
void stack_tostr(stack* s, char* str);
bool stack_peek(stack* s, double* new_d);

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


/*return void then cast when appropriate*/
void* safe_calloc(size_t nitems, size_t size)
{
   void* ptr;
   ptr = calloc(nitems, size);
   if(ptr==NULL)
   {
      fprintf(stderr,"Not enough space\n");
   	exit(EXIT_FAILURE);
   }
   return ptr;
}


stack* stack_init(void)
{
  stack* s;
  s=(stack*) safe_calloc(1,sizeof(stack));
  return s;
}


nodeptr init_node(double d)
{
   nodeptr n_node;


   n_node=safe_calloc(1,sizeof(node));
   n_node->num=d;
   n_node->next=NULL;
   return n_node;

}

void stack_push(stack* s,double d)
{
   nodeptr n_node;
  if(s)
  {
    n_node= init_node(d);
    n_node->next=s->start;
    s->start=n_node;
    s->size++;
  }
}

bool stack_pop(stack* s, double* new_d)
{
   nodeptr temp;
   if(s&&s->start)
   {
      temp=s->start;
      s->start=s->start->next;
      *new_d=temp->num;
      free(temp);
      return true;
   }
   return false;
}


bool stack_peek(stack* s, double* new_d)
{
   if(s)
   {
      if(s->start)
      {
         *new_d= s->start->num;
         return true;
      }
   }
   return false;
}


bool stack_free(stack* s)
{
   nodeptr temp,to_free;
   if(s)
   {
      to_free= s->start;
      while(to_free)
      {
         temp=to_free->next;
         free(to_free);
         to_free=temp;
      }
      free(s);
   }
   return true;
}


void stack_tostr(stack* s, char* str)
{
   nodeptr current;
   char temp[MAXLEN];
   if(s&&s->start)
   {
      str[0]='\0';
      current=s->start;
      while(current)
      {
          sprintf(temp, "%lf", current->num);
          strcat(str, temp);
          strcat(str, "|");
          current=current->next;
      }
      str[strlen(str)-1] = '\0';
   }

}
