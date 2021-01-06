#include "specific.h"
#include "parser_funcs.h"
#include "interpreter_funcs.h"
#include "../ADTS_and_general/stacks/stack_funcs.h"
#include "../ADTS_and_general/hash_map/hash_funcs.h"
#include "../ADTS_and_general/arrays/array_funcs.h"
#include "extension_flow.h"
#include "extension_funcs.h"

#define MAXTESTCAP 100
#define MAXTESTLEN 70
#define MAXARRLEN 50
#define EPSILON 0.00001


bool get_arr_identifier(word_cont* to_check,\
                        char arr_name[MAXARRLEN]);
bool valid_arr_identifier(word_cont* to_check);
bool valid_init_arr(word_cont* to_check);
void store_arr(word_cont* to_check,char arr_name[MAXARRLEN],\
               turt_arr* n_arr);
bool run_init_arr(word_cont* to_check);
word_cont* init_word_cont(void);

int main(void)
{
   word_cont* test_cont;
   line_cont* test_line_cont;
   char test_name[MAXARRLEN];
   turt_arr* test_arr;

   test_cont = init_word_cont();
   strcpy(test_cont->words[0],"a_myarr");
   assert(valid_arr_identifier(test_cont));
   assert(get_arr_identifier(test_cont,test_name));
   assert(strcmp(test_name,"a_myarr")==0);
   test_cont->position=0;

   strcpy(test_cont->words[0],"a_");
   assert(valid_arr_identifier(test_cont));
   assert(get_arr_identifier(test_cont,test_name));
   assert(strcmp(test_name,"a_")==0);
   test_cont->position=0;

   strcpy(test_cont->words[0],"a_bbbbbbbbbbbbbbbbbbbbbbbbbbbbbb");
   assert(valid_arr_identifier(test_cont));
   assert(get_arr_identifier(test_cont,test_name));
   assert(strcmp(test_name,"a_bbbbbbbbbbbbbbbbbbbbbbbbbbbbbb")==0);
   test_cont->position=0;


   strcpy(test_cont->words[0],"a_12334");
   assert(!valid_arr_identifier(test_cont));
   assert(!get_arr_identifier(test_cont,test_name));
   strcpy(test_cont->words[0],"abc");
   assert(!valid_arr_identifier(test_cont));
   assert(!get_arr_identifier(test_cont,test_name));
   strcpy(test_cont->words[0],"_abc");
   assert(!valid_arr_identifier(test_cont));
   assert(!get_arr_identifier(test_cont,test_name));
   strcpy(test_cont->words[0],"_");
   assert(!valid_arr_identifier(test_cont));
   assert(!get_arr_identifier(test_cont,test_name));
   strcpy(test_cont->words[0],"");
   assert(!valid_arr_identifier(test_cont));
   assert(!get_arr_identifier(test_cont,test_name));
   strcpy(test_cont->words[0],"a_myarrrrrrrrrrrrrrrrrrrrrrrrrrrrrramyarrrrrrrrrrrrrrrrrrrrrrrrrrrrrr");
   assert(!valid_arr_identifier(test_cont));
   assert(!get_arr_identifier(test_cont,test_name));

   strcpy(test_cont->words[0],"INITARR");
   strcpy(test_cont->words[1],"a_myarr");
   assert(valid_init_arr(test_cont));
   assert(test_cont->position==2);

   test_cont->position=0;
   strcpy(test_cont->words[0],"INITARR");
   strcpy(test_cont->words[1],"a_myarrrrrrrrrrrrrrrrrrrrrrrrrrrrrr");
   assert(valid_init_arr(test_cont));


   test_cont->position=0;
   strcpy(test_cont->words[0],"INITARR");
   strcpy(test_cont->words[1],"a_myarrrrrrrrrrrrrrrr_rrrrrrrrrrrrr");
   assert(!valid_init_arr(test_cont));

   test_cont->position=0;
   strcpy(test_cont->words[0],"INITARR");
   strcpy(test_cont->words[1],"a_myarrrrrrrr1rrrrrrrrrrrrr");
   assert(!valid_init_arr(test_cont));


   test_cont->position=0;
   strcpy(test_cont->words[0],"initarr");
   strcpy(test_cont->words[1],"a_myarrrrrrrrrrrrrrrrrrrrr");
   assert(!valid_init_arr(test_cont));

   test_cont->position=0;
   strcpy(test_cont->words[0],"");
   strcpy(test_cont->words[1],"a_myarrrrrrrrrrrrrrrrrrrrr");
   assert(!valid_init_arr(test_cont));


   free_word_cont(test_cont);

   test_cont = init_word_cont();

   strcpy(test_cont->words[0],"INITARR");
   strcpy(test_cont->words[1],"a_myarr");
   assert(run_init_arr(test_cont));
   test_arr=NULL;
   test_arr=assoc_lookup(test_cont->arr_map,"a_myarr");
   assert(test_arr);

   strcpy(test_cont->words[2],"INITARR");
   strcpy(test_cont->words[3],"a_myarragain");
   assert(run_init_arr(test_cont));
   return 0;
}


bool valid_init_arr(word_cont* to_check)
{
   if(strcmp(to_check->words[to_check->position],"INITARR")==0)
   {
      to_check->position++;
      if(valid_arr_identifier(to_check))
      {
         to_check->position++;
         return true;
      }
   }
   return false;
}

bool run_init_arr(word_cont* to_check)
{
   char* arr_name;
   turt_arr* n_arr;
   arr_name=(char*)safe_calloc(MAXARRLEN,sizeof(char));
   n_arr=arr_init();
   if(strcmp(to_check->words[to_check->position],"INITARR")==0)
   {
      to_check->position++;
      if(get_arr_identifier(to_check,arr_name))
      {
         store_arr(to_check,arr_name,n_arr);
         return true;
      }
   }
   return false;
}

void store_arr(word_cont* to_check,char arr_name[MAXARRLEN],turt_arr* n_arr)
{
   /*find mains hash map for arrays as arrays are global*/
   while(to_check->parent)
   {
      to_check=to_check->parent;
   }
   assoc_insert(&to_check->arr_map,arr_name,n_arr);
}


bool get_arr_identifier(word_cont* to_check,char arr_name[MAXARRLEN])
{
   if(valid_arr_identifier(to_check))
   {
      strcpy(arr_name,to_check->words[to_check->position]);
      to_check->position++;
      return true;
   }
   return false;
}



bool valid_arr_identifier(word_cont* to_check)
{
   int i;
   if(to_check->position>=to_check->capacity)
   {
      return false;
   }
   if(strlen(to_check->words[to_check->position])<2)
   {
      return false;
   }
   /* all arrays should start with a_ */
   if((to_check->words[to_check->position][0]!='a')||
      (to_check->words[to_check->position][1]!='_'))
   {
      return false;
   }
   i=2;
   while(to_check->words[to_check->position][i])
   {
      if(i>=MAXARRLEN)
      {
         return false;
      }
      if(!(to_check->words[to_check->position][i]>='a')||\
         !(to_check->words[to_check->position][i]<='z'))
      {
         return false;
      }
      i++;
   }
   return true;
}



word_cont* init_word_cont(void)
{
   word_cont* n_cont;
   int i;
   n_cont=(word_cont*)safe_calloc(1,sizeof(word_cont));
   n_cont->capacity=MAXTESTCAP;
   n_cont->words= (char**)safe_calloc(MAXTESTCAP,sizeof(char*));
   for(i=0;i<MAXTESTCAP;i++)
   {
      n_cont->words[i]=(char*)safe_calloc(MAXTESTLEN,sizeof(char));
      n_cont->words[i][0]='\0';
   }
   n_cont->position=0;

   n_cont->stackptr=stack_init();
   for(i=0;i<NUMVARS;i++)
   {
      n_cont->var_array[i]=NULL;
   }
   n_cont->err_message[0]='\0';
   n_cont->func_map=assoc_init();
   n_cont->arr_map=assoc_init();
   return n_cont;
}
