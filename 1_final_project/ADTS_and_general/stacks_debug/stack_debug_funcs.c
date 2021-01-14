#include "stack_debug_funcs.h"

/*these funcs are very
similar to your stack funcs*/
dstack* dstack_init(void)
{
  dstack* s;
  s=(dstack*) safe_calloc(1,sizeof(dstack));
  s->size=0;
  return s;
}


dnodeptr init_dnode(void* d)
{
   dnodeptr n_node;
   n_node=(dnodeptr)safe_calloc(1,sizeof(dnode));
   n_node->data=d;
   n_node->next=NULL;
   return n_node;

}

void dstack_push(stack* s,loop_tracker* d)
{
  dnodeptr n_node;
  if(s)
  {
    n_node= init_dnode(d);
    n_node->next=s->start;
    s->start=n_node;
    s->size++;
  }
}

bool dstack_pop(stack* s, loop_tracker* new_d)
{
   dnodeptr temp;
   if(s&&s->start)
   {
      temp=s->start;
      s->start=s->start->next;
      *new_d=temp->data;
      free(temp);
      s->size--;
      return true;
   }
   return false;
}


bool dstack_peek(stack* s, loop_tracker* new_d)
{
   if(s)
   {
      if(s->start)
      {
         *new_d= s->start->data;
         return true;
      }
   }
   return false;
}


bool dstack_free(stack* s)
{
   dnodeptr temp,to_free;
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
