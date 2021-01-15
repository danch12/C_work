#include "stack_funcs.h"

/*these funcs are very
similar to your stack funcs with
a couple of extra bits added on*/
stack* stack_init(size_t datasize)
{
  stack* s;
  s=(stack*) safe_calloc(1,sizeof(stack));
  s->size=0;
  s->data_size=datasize;
  return s;
}

/*using memcpy makes life so much easier
because you dont have to worry about the
value in the stack changing*/
nodeptr init_node(void* d,size_t datasize)
{
   nodeptr n_node;
   n_node=(nodeptr)safe_calloc(1,sizeof(node));

   n_node->data=(void*)safe_calloc(1,datasize);

   memcpy(n_node->data,d,datasize);
   n_node->next=NULL;
   return n_node;

}

void stack_push(stack* s,void* d)
{
   nodeptr n_node;
   if(s)
   {
    n_node= init_node(d,s->data_size);
    n_node->next=s->start;
    s->start=n_node;
    s->size++;
   }
}

bool stack_pop(stack* s, void** new_d)
{
   nodeptr temp;
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


bool stack_peek(stack* s, void** new_d)
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


bool stack_free(stack* s)
{
   nodeptr temp,to_free;
   if(s)
   {
      to_free= s->start;
      while(to_free)
      {
         temp=to_free->next;
         if(to_free->data)
         {
            free(to_free->data);
         }
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
          sprintf(temp, "%f", *(double*)current->data);
          strcat(str, temp);
          strcat(str, "|");
          current=current->next;
      }
      str[strlen(str)-1] = '\0';
   }

}
