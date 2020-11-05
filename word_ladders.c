#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <string.h>
#include <assert.h>

#define WORDLEN 6


typedef struct list{
   char       data[WORDLEN];
   struct list *next;
} element;

typedef element *link;

typedef enum bool {false,true} bool;

link make_links(char* filename);
int edit_distance(const char word_1[], const char word_2[],int wordlen);
void fill_ladder(char word_1[],char word_2[],link head,char word_list[WORDLEN][WORDLEN]);
void free_links(link head);

bool criteria_check(int word_len,char current[],\
                     char existing[],char target_word[]);
void test(void);

int main(void)
{
   link head;
   char word_list[WORDLEN][WORDLEN];
   int i;
   test();
   i=0;

   head=make_links("34words.txt");
   fill_ladder("cold","warm",head,word_list);
   while(strcmp(word_list[i],"xxx")!=0)
   {
      printf("%s\n",word_list[i]);
      i++;
   }
   free_links(head);
}

void test(void)
{
   int i;
   char word_list[WORDLEN][WORDLEN];
   int word_count,count;
   link head,current;
   char target_list[WORDLEN][WORDLEN]={"wold","word","worm","warm"};
   word_count=2891;
   count=0;

   assert(edit_distance("dan","man",3)==1);
   assert(edit_distance("dain","niad",4)==4);
   assert(edit_distance("dan","dan",3)==0);
   head=make_links("34words.txt");
   current=head;

   while(current!=NULL)
   {
      count++;
      current=current->next;
   }
   assert(strcmp(head->data,"work")==0);
   assert(count==word_count);
   fill_ladder("cold","warm",head,word_list);

   i=0;
   while(strcmp(word_list[i],"xxx")!=0)
   {
      assert(strcmp(word_list[i],target_list[i])==0);
      i++;
   }


   free_links(head);


   assert(criteria_check(4,"bean","mean","brow")==true);
   assert(criteria_check(4,"bran","mean","brow")==false);
   assert(criteria_check(4,"bean","mean","mrow")==false);
}



/*want to look for words that are 1 distance away from orig
and one closer to target*/
int edit_distance(const char word_1[], const char word_2[],int wordlen)
{
   int count,i;

   count=0;
   for(i=0;i<=wordlen;i++)
   {
      if(word_1[i]!=word_2[i])
      {
         count++;
      }
   }
   return count;
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

void fill_ladder(char word_1[],char word_2[],link head,char word_list[WORDLEN][WORDLEN])
{
   link current;
   char temp[6];
   int word_len;
   int i;

   i=0;
   word_len=strlen(word_1);
   current=head;
   strcpy(temp,word_1);
   while(current!=NULL&&(strcmp(temp,word_2)!=0))
   {

      if(criteria_check(word_len,current->data,temp,word_2))
      {
         strcpy(temp,current->data);
         strcpy(word_list[i],temp);
         i++;
         current=head;
      }
      else
      {
         current=current->next;
      }
   }
   strcpy(word_list[i],"xxx");
}


bool criteria_check(int word_len,char current[],\
                     char existing[],char target_word[])
{
   int distance_new,distance_old,between;
   if((int)strlen(current)==word_len)
   {
      /*if new distance is smaller than old and theres only 1 difference
      between new and old*/
      distance_new=edit_distance(current,target_word,word_len);
      distance_old=edit_distance(existing,target_word,word_len);
      between= edit_distance(existing,current,word_len);
      if(distance_new<distance_old&&between==1)
      {
         return true;
      }

   }
   return false;
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
