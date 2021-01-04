#include "function_stack_funcs.h"

/*these funcs are very
similar to your stack funcs*/
func_stack* func_stack_init(void)
{
  func_stack* s;
  s=(func_stack*) safe_calloc(1,sizeof(func_stack));
  s->size=0;
  s->return_val=NULL;
  return s;
}


func_nodeptr init_func_node(void* d)
{
   func_nodeptr n_node;
   n_node=(func_nodeptr)safe_calloc(1,sizeof(func_node));
   n_node->data=d;
   n_node->next=NULL;
   return n_node;

}

void func_stack_push(func_stack* s,void* d)
{
   func_nodeptr n_node;
  if(s)
  {
    n_node= init_func_node(d);
    n_node->next=s->start;
    s->start=n_node;
    s->size++;
  }
}

bool func_stack_pop(func_stack* s, void** new_d)
{
   func_nodeptr temp;
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


bool func_stack_peek(func_stack* s, void** new_d)
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


bool func_stack_free(func_stack* s)
{
   func_nodeptr temp,to_free;
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

/*
void func_stack_tostr(stack* s, char* str)
{
   nodeptr current;
   char temp[MAXELELEN];
   if(s&&s->start)
   {
      str[0]='\0';
      current=s->start;
      while(current)
      {
          sprintf(temp, "%f", current->data);
          strcat(str, temp);
          strcat(str, "|");
          current=current->next;
      }
      str[strlen(str)-1] = '\0';
   }

}
*/
