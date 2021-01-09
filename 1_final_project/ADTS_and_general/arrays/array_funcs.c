#include "array_funcs.h"

t_node* init_t_node(double data);

turt_arr* arr_init(void)
{
   turt_arr* n_arr;
   n_arr=(turt_arr*)safe_calloc(1,sizeof(turt_arr));
   n_arr->size=0;
   n_arr->head=NULL;
   n_arr->tail=NULL;
   return n_arr;
}



bool append_arr(double data,turt_arr* t_arr)
{
   t_node* n_node;
   if(t_arr)
   {
      n_node=init_t_node(data);
      if(t_arr->head==NULL)
      {
         t_arr->head=n_node;
         t_arr->tail=n_node;
         t_arr->size++;

      }
      else
      {
         t_arr->tail->next=n_node;
         t_arr->tail=n_node;
         t_arr->size++;
      }
      return true;
   }
   return false;
}
/*if position bigger than size then just puts val at end*/
bool insert_arr(double data,turt_arr* t_arr,int position)
{
   t_node* n_node;
   t_node* temp,*previous;
   if(t_arr)
   {
      n_node=init_t_node(data);
      if(t_arr->size==0)
      {
         t_arr->head=n_node;
         t_arr->tail=n_node;
         t_arr->size++;
         return true;
      }
      if(position==0)
      {
         n_node->next=t_arr->head;
         t_arr->head=n_node;
         t_arr->size++;
         return true;
      }
      temp=t_arr->head;
      previous=temp;
      while((position>0)&&temp)
      {
         previous=temp;
         temp=temp->next;
         position--;
      }
      n_node->next=temp;
      previous->next=n_node;
      t_arr->size++;
      return true;
   }
   return false;
}



t_node* init_t_node(double data)
{
   t_node* n_node;
   n_node=(t_node*)safe_calloc(1, sizeof(t_node));
   n_node->data= data;
   n_node->next=NULL;
   return n_node;
}


/*zero indexed */
bool remove_val(turt_arr* t_arr,int position)
{
   t_node* temp;
   t_node* prev;
   if(t_arr)
   {
      if((unsigned int)position>=t_arr->size)
      {
         return true;
      }
      if(position==0)
      {
         temp=t_arr->head;
         /*if size if 1 this will set head to NULL*/
         t_arr->head= t_arr->head->next;
         free(temp);
         t_arr->size--;
      }
      else
      {
         temp= t_arr->head;
         prev=temp;
         while(position>0)
         {
            prev=temp;
            temp=temp->next;
            position--;
         }
         prev->next=temp->next;
         /*if temp next is null it means
         that it was the last in the list*/
         if(!temp->next)
         {
            t_arr->tail=prev;
         }
         free(temp);
         t_arr->size--;
      }
      return true;
   }
   return false;

}


bool change_val_arr(double n_data,int position,\
                  turt_arr* t_arr)
{
   t_node* temp;
   if(t_arr)
   {
      if((position<0) || ((unsigned int)position>=t_arr->size))
      {
         return false;
      }
      temp=t_arr->head;
      while(position>0)
      {
         temp=temp->next;
         position--;
      }
      temp->data=n_data;
      return true;
   }
   return false;
}



bool get_arr_val(turt_arr* t_arr,int position,double* val)
{
   t_node* temp;
   if(t_arr)
   {
      if((unsigned int)position>=t_arr->size)
      {
         return false;
      }
      temp=t_arr->head;
      while(position)
      {
         temp=temp->next;
         position--;
      }
      *val =temp->data;
      return true;
   }
   return false;
}

bool free_arr(turt_arr* to_free)
{
   t_node* current;
   t_node* prev;
   if(to_free)
   {
      current=to_free->head;
      while(current)
      {
         prev=current;
         current=current->next;
         free(prev);
      }
      free(to_free);
   }
   return true;
}
