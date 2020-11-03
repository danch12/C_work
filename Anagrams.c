#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <string.h>
#include <assert.h>

typedef enum bool {false,true} bool;

typedef struct list{
   char       data[50];
   struct list *next;
} element;

#define ALPHASIZE 26


typedef element *link;

void make_hist(const char word[],int letter_hist[],int hist_size);
void stringify_array(int arr[],int arr_size,char string[]);
bool same_hist(const int hist_1[], const int hist_2[],int hist_size);
link make_links(char* filename);
void print_ll(link head);
void free_links(link head);
link remove_non_anagrams(link head,int orig_hist[],char orig_word[]);
void stringify_linkedl(link head,char string[]);
void test(void);

int main(int argc, char* argv[])
{
   int user_hist[ALPHASIZE];
   link head;
   test();
   if(argc<2)
   {
      fprintf(stderr,"not valid cell\n");
      return 1;
   }
   else
   {
      make_hist(argv[1],user_hist,ALPHASIZE);
      head=make_links("eng_370k_shuffle.txt");
      head=remove_non_anagrams(head,user_hist,argv[1]);
      print_ll(head);
      free_links(head);
   }

   return 0;
}

void test(void)
{
   link head;
   int test_hist_1[26]={0};
   int test_hist_2[26]={0};
   char test_str[27];
   char test_str_ll[10000];
   make_hist("abet",test_hist_1,26);
   make_hist("abbbaa",test_hist_2,26);
   stringify_array(test_hist_2,26,test_str);
   assert(strcmp(test_str,"33000000000000000000000000")==0);
   assert(same_hist(test_hist_1,test_hist_2,26)==false);
   assert(same_hist(test_hist_1,test_hist_1,26)==true);
   head=make_links("test_word_list.txt");
   stringify_linkedl(head,test_str_ll);
   assert(strcmp(test_str_ll," abetbeatbetaalertalter"));
   head=remove_non_anagrams(head,test_hist_1,"abet");
   stringify_linkedl(head,test_str_ll);
   assert(strcmp(test_str_ll," abetbeatbeta"));
   free_links(head);


}


void make_hist(const char word[],int letter_hist[],int hist_size)
{
   int i;

   for(i=0;i<hist_size;i++)
   {
      letter_hist[i]=0;
   }
   i=0;
   while(word[i]!='\0')
   {
      letter_hist[word[i]-'a']++;
      i++;
   }
}

void stringify_array(int arr[],int arr_size,char string[])
{
   int i;
   for(i=0;i<arr_size;i++)
   {
      string[i]=arr[i]+'0';
   }
   string[i]='\0';
}

bool same_hist(const int hist_1[],\
    const int hist_2[],int hist_size)
{
   int i;
   for(i=0;i<hist_size;i++)
   {
      if(hist_1[i]!=hist_2[i])
      {
         return false;
      }
   }
   return true;
}


link make_links(char* filename)
{
   FILE *ifp;
   char str_char[50];
   link head;
   link current;
   head = (element *)malloc(sizeof(element));

   current=head;

   if((ifp=fopen(filename,"r"))==NULL)
   {
      fprintf(stderr,"cant open\n");
      exit(EXIT_FAILURE);
   }
   while((fscanf(ifp,"%s",str_char))!=EOF)
   {
      current=current->next=(element *)malloc(sizeof(element));
      strcpy(current->data,str_char);
      current->next =NULL;
    }

    fclose(ifp);
    current=head;
    head=head->next;
    free(current);
    return head;
}

void print_ll(link head)
{

   while(head!=NULL)
   {
      printf("%s",head->data);
      printf("\n");
      head=head->next;
   }
}

/*really dont know the length of the dictionary
this is why we use linked list in first place
however for testing going to only have a couple of words*/
void stringify_linkedl(link head,char string[])
{
   if(head)
   {
      strcpy(string,"");
      while(head!=NULL)
      {
         strcat(string,head->data);
         head=head->next;
      }
   }

}

link remove_non_anagrams(link head,int orig_hist[],char orig_word[])
{
   link previous,top,temp;
   int comparison_hist[26];

   top=malloc(sizeof(element));
   strcpy(top->data," ");
   top->next=head;
   previous=top;
   while(head!=NULL)
   {
      make_hist(head->data,comparison_hist,26);
      if((!same_hist(comparison_hist,orig_hist,26))||\
           strcmp(head->data,orig_word)==0)
      {
         temp=head;
         previous->next=head->next;
         head=head->next;
         free(temp);
      }
      else
      {
         head=head->next;
         previous=previous->next;
      }
   }
   temp = top;
   top=top->next;
   free(temp);
   return top;

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
