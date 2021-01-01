#include "stack_funcs.h"

/*these funcs are very
similar to your stack funcs*/
stack* stack_init(void)
{
  stack* s;
  s=(stack*) safe_calloc(1,sizeof(stack));
  s->size=0;
  return s;
}


nodeptr init_node(double d)
{
   nodeptr n_node;
   n_node=(nodeptr)safe_calloc(1,sizeof(node));
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
      s->size--;
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
   char temp[MAXELELEN];
   if(s&&s->start)
   {
      str[0]='\0';
      current=s->start;
      while(current)
      {
          sprintf(temp, "%f", current->num);
          strcat(str, temp);
          strcat(str, "|");
          current=current->next;
      }
      str[strlen(str)-1] = '\0';
   }

}
