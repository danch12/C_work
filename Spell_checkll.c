#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <string.h>
#include <assert.h>

#define MAXWORDSIZE 50
#define BUFFERSIZE 1000

typedef enum bool {false,true} bool;

typedef struct list{
   char       data[MAXWORDSIZE];
   struct list *next;
} element;

typedef element *link;


link make_links(char* filename);
void print_ll(link head);
void free_links(link head);
link add_link(char word[],link head);

int main(void)
{
   link head;
   head=make_links("test_word_list.txt");
   print_ll(head);
   free_links(head);
   return 0;
}


link make_links(char* filename)
{
   FILE *ifp;
   char str_char[MAXWORDSIZE];
   link head;
   link current;


   head=(link)malloc(sizeof(element));
   strcpy(head->data,"*start*");
   head->next=NULL;
   if((ifp=fopen(filename,"r"))==NULL)
   {
      fprintf(stderr,"cant open\n");
      exit(EXIT_FAILURE);
   }

   while((fscanf(ifp,"%s",str_char))!=EOF)
   {
      printf("hello\n");
      head=add_link(str_char,head);
   }
   return head;
}


link add_link(char word[],link head)
{
   link temp;
   link previous;
   link current;

   temp=(link)malloc(sizeof(element));
   strcpy(temp->data,word);

   if(strcmp(word,head->data)<0)
   {
      printf("%s\n",word);
      temp->next= head;
      head=temp;
      return head;
   }

   current=head;
   while(current->next!=NULL)
   {
      printf("current =%s\n",current->data);
      if(strcmp(word,current->data)<0)
      {
         previous->next=temp;
         temp->next=current;
         return head;
      }
      previous=current;
      current=current->next;
   }
   current->next=temp;
   temp->next=NULL;
   return head;
}


void print_ll(link head)
{

   while(head!=NULL)
   {
      printf("%s ",head->data);

      head=head->next;
   }
}


void free_links(link head)
{
   link previous;
   while(head!=NULL)
   {
      previous =head;
      head=head->next;
      free(previous);
   }

}
