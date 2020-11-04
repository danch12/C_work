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
link create_anagram_links(link head,int orig_hist[],char orig_word[]);
int size_of_ll(link head);
void group_anagrams(link head);
bool in_list(char word[],char* used_words[],int size);
int add_to_list(link head,char* seen_list[],int position);
link two_word_anagrams(link head,int orig_hist[],char orig_word[]);
void test(void);

int main(int argc, char* argv[])
{
   int user_hist[ALPHASIZE];
   link head,anagrams,two_word;
   test();
   if(argc<2)
   {
      fprintf(stderr,"not valid cell\n");
      return 1;
   }
   else
   {
      make_hist(argv[1],user_hist,ALPHASIZE);
      head=make_links("34words.txt");
      /*group_anagrams(head);*/
      anagrams=create_anagram_links(head,user_hist,argv[1]);
      printf("%d\n",size_of_ll(anagrams));
      two_word=two_word_anagrams(head,user_hist,argv[1]);
      /*head=remove_non_anagrams(head,user_hist,argv[1]);*/
      print_ll(two_word);
      print_ll(anagrams);
      free_links(head);
      free_links(anagrams);
      free_links(two_word);
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
      printf("%s ",head->data);

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



link remove_anagrams(link head,int orig_hist[],char orig_word[])
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
      if((same_hist(comparison_hist,orig_hist,26))||\
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

link create_anagram_links(link head,int orig_hist[],char orig_word[])
{
   link top,temp;
   int comparison_hist[26];
   temp=(element *)malloc(sizeof(element));
   if(temp==NULL)
   {
      fprintf(stderr,"not enough space\n");
      exit(EXIT_FAILURE);
   }

   strcpy(temp->data," ");
   temp->next=NULL;
   top=temp;
   while(head!=NULL)
   {

      make_hist(head->data,comparison_hist,26);
      if((same_hist(comparison_hist,orig_hist,26))&&\
           (!(strcmp(head->data,orig_word)==0)))
      {

         temp->next=(element *)malloc(sizeof(element));
         if(temp==NULL)
         {
            fprintf(stderr,"not enough space\n");
            exit(EXIT_FAILURE);
         }
         temp=temp->next;
         strcpy(temp->data,head->data);
         temp->next=NULL;
         head=head->next;

      }
      else
      {
         head=head->next;

      }
   }
   temp = top;
   top=top->next;
   free(temp);
   return top;

}


bool in_list(char word[],char* used_words[],int size)
{
   int i;
   for(i=0;i<size;i++)
   {
      if(strcmp(used_words[i],word)==0)
      {
         return true;
      }
   }
   return false;
}
void group_anagrams(link head)
{
   /*want to first check that word hasnt apeared yet
   then if it hasnt print group of anagrams */
   int list_size,position;
   link current_anagrams,top,temp;
   char** seen_list;
   int i;
   int head_hist[ALPHASIZE];
   /*create list of pointers big enough for all the words
   have to make it a list of double pointers bc not sure
   how big each word going to be*/
   list_size=size_of_ll(head);
   seen_list=malloc(sizeof(char *)* list_size);
   if(seen_list==NULL)
   {
      fprintf(stderr,"not enough space\n");
      exit(EXIT_FAILURE);
   }
   /*keeping a pointer at top of linked list*/
   top=head;
   temp=top;
   position=0;

   while(head!=NULL)
   {
      if(!in_list(head->data,seen_list,position))
      {
         /*copy over head word*/
         seen_list[position]=malloc(sizeof(char)*strlen(head->data)+1);
         if(seen_list[position]==NULL)
         {
            fprintf(stderr,"not enough space\n");
            exit(EXIT_FAILURE);
         }
         strcpy(seen_list[position],head->data);
         position++;
         make_hist(head->data,head_hist,26);
         /*create linked list of anagrams and print then free*/
         current_anagrams=create_anagram_links(temp,head_hist,head->data);
         printf("%d %s ",size_of_ll(current_anagrams),head->data);
         print_ll(current_anagrams);
         /*copy over rest of anagrams*/
         position=add_to_list(current_anagrams,seen_list,position);
         printf("\n");

         temp=top;
      }
      head=head->next;
   }
   /*free seen list*/
   for(i=0;i<position;i++)
   {
      free(seen_list[i]);
   }
   free(seen_list);
}


int add_to_list(link head,char* seen_list[],int position)
{
   link previous;
   while(head!=NULL)
   {
      previous=head;
      seen_list[position]=malloc(sizeof(char)*strlen(previous->data)+1);
      if(seen_list[position]==NULL)
      {
         fprintf(stderr,"not enough space\n");
         exit(EXIT_FAILURE);
      }
      strcpy(seen_list[position],previous->data);
      head=head->next;
      free(previous);
      position++;
   }
   return position;
}

int size_of_ll(link head)
{
   int i;
   i=0;
   while(head!=NULL)
   {
      i++;
      head=head->next;
   }
   return i;
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


link two_word_anagrams(link head,int orig_hist[],char orig_word[])
{
   link top,temp,follower;
   int comparison_hist[26];
   char combo_word[100];
   temp=(element *)malloc(sizeof(element));

   if(temp==NULL)
   {
      fprintf(stderr,"not enough space\n");
      exit(EXIT_FAILURE);
   }

   strcpy(temp->data," ");
   temp->next=NULL;
   top=temp;

   while(head!=NULL)
   {
      follower=head;
      while(follower!=NULL)
      {
         strcpy(combo_word,head->data);
         strcat(combo_word,follower->data);

         make_hist(combo_word,comparison_hist,26);
         if((same_hist(comparison_hist,orig_hist,26))&&\
              (!(strcmp(combo_word,orig_word)==0)))
         {
            printf("%s\n",combo_word);
            temp->next=(element *)malloc(sizeof(element));
            if(temp->next==NULL)
            {
               fprintf(stderr,"not enough space\n");
               exit(EXIT_FAILURE);
            }
            temp=temp->next;
            strcpy(temp->data,combo_word);
            temp->next=NULL;
            follower=follower->next;

         }
         else
         {
            follower=follower->next;

         }
      }

      head=head->next;
   }


   temp = top;
   top=top->next;
   free(temp);
   return top;
}
