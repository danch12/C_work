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
/*store value into array*/
void store_arr(word_cont* to_check,char arr_name[MAXARRLEN],\
               turt_arr* n_arr);

/*guard aginst decimals being used as index*/
bool get_valid_ind(word_cont* to_check,line_cont* line_arr,int* ind);
bool run_init_arr(word_cont* to_check);
bool valid_append(word_cont* to_check);
bool run_append(word_cont* to_check,line_cont* line_arr);
turt_arr* get_arr(word_cont* to_check,char arr_name[MAXARRLEN]);

/*gets info and checks for validity*/
bool change_helper(word_cont* to_check, line_cont* line_arr,\
                  int* ind,double* num);
bool valid_change(word_cont* to_check);
bool run_change(word_cont* to_check,line_cont* line_arr);


bool compare_doubles(double d_1, double d_2);
word_cont* init_word_cont(void);

int main(void)
{
   word_cont* test_cont;
   line_cont* test_line_cont;
   char test_name[MAXARRLEN];
   turt_arr* test_arr;
   int test_int;
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
   assert(test_cont->position==2);

   strcpy(test_cont->words[2],"INITARR");
   strcpy(test_cont->words[3],"a_myarragain");
   assert(run_init_arr(test_cont));
   /*checking we can overwrite without memory leaks*/
   strcpy(test_cont->words[4],"INITARR");
   strcpy(test_cont->words[5],"a_myarragain");
   assert(run_init_arr(test_cont));
   test_arr=assoc_lookup(test_cont->arr_map,"a_myarragain");
   assert(test_arr);
   free_word_cont(test_cont);

   test_cont = init_word_cont();

   strcpy(test_cont->words[0],"INITARR");
   strcpy(test_cont->words[1],"");
   strcpy(test_cont->words[2],"a_myarr");
   assert(!run_init_arr(test_cont));
   test_cont->position=0;

   strcpy(test_cont->words[0],"INITARR");
   strcpy(test_cont->words[1],"dhfn");
   assert(!run_init_arr(test_cont));
   test_cont->position=0;

   strcpy(test_cont->words[0],"a_arr");
   strcpy(test_cont->words[1],"INITARR");
   assert(!run_init_arr(test_cont));
   test_cont->position=0;

   free_word_cont(test_cont);


   test_cont = init_word_cont();
   strcpy(test_cont->words[0],"a_myarr");
   strcpy(test_cont->words[1],"APPEND");
   strcpy(test_cont->words[2],"23");
   strcpy(test_cont->words[3],";");
   assert(valid_append(test_cont));
   assert(test_cont->position==4);

   test_cont->position=0;
   strcpy(test_cont->words[0],"a_myarr");
   strcpy(test_cont->words[1],"APPEND");
   strcpy(test_cont->words[2],"A");
   strcpy(test_cont->words[3],";");
   assert(valid_append(test_cont));

   test_cont->position=0;
   strcpy(test_cont->words[0],"a_myarr");
   strcpy(test_cont->words[1],"APPEND");
   strcpy(test_cont->words[2],"A");
   strcpy(test_cont->words[3],"1");
   strcpy(test_cont->words[4],"/");
   strcpy(test_cont->words[5],";");
   assert(valid_append(test_cont));



   test_cont->position=0;
   strcpy(test_cont->words[0],"a_myarr");
   strcpy(test_cont->words[1],"APPEND");
   strcpy(test_cont->words[2],"A");
   strcpy(test_cont->words[3],"B");
   strcpy(test_cont->words[4],"/");
   strcpy(test_cont->words[5],";");
   assert(valid_append(test_cont));

   test_cont->position=0;
   strcpy(test_cont->words[0],"a_myarr");
   strcpy(test_cont->words[1],"APPEND");
   strcpy(test_cont->words[2],"A");
   strcpy(test_cont->words[3],"abc");
   strcpy(test_cont->words[4],"{");
   strcpy(test_cont->words[5],"}");
   strcpy(test_cont->words[6],"/");
   strcpy(test_cont->words[7],";");
   assert(valid_append(test_cont));

   /*valid but wont run*/
   test_cont->position=0;
   strcpy(test_cont->words[0],"a_myarr");
   strcpy(test_cont->words[1],"APPEND");
   strcpy(test_cont->words[2],"/");
   strcpy(test_cont->words[3],"abc");
   strcpy(test_cont->words[4],"{");
   strcpy(test_cont->words[5],"}");
   strcpy(test_cont->words[6],"/");
   strcpy(test_cont->words[7],";");
   assert(valid_append(test_cont));

   test_cont->position=0;
   strcpy(test_cont->words[0],"kancdj");
   strcpy(test_cont->words[1],"APPEND");
   strcpy(test_cont->words[2],"/");
   strcpy(test_cont->words[3],"abc");
   strcpy(test_cont->words[4],"{");
   strcpy(test_cont->words[5],"}");
   strcpy(test_cont->words[6],"/");
   strcpy(test_cont->words[7],";");
   assert(!valid_append(test_cont));

   test_cont->position=0;
   strcpy(test_cont->words[0],"a_");
   strcpy(test_cont->words[1],"append");
   strcpy(test_cont->words[2],"/");
   strcpy(test_cont->words[3],"abc");
   strcpy(test_cont->words[4],"{");
   strcpy(test_cont->words[5],"}");
   strcpy(test_cont->words[6],"/");
   strcpy(test_cont->words[7],";");
   assert(!valid_append(test_cont));


   test_cont->position=0;
   strcpy(test_cont->words[0],"a_");
   strcpy(test_cont->words[1],"APPEND");
   strcpy(test_cont->words[2],"");
   strcpy(test_cont->words[3],"abc");
   strcpy(test_cont->words[4],"{");
   strcpy(test_cont->words[5],"}");
   strcpy(test_cont->words[6],"/");
   strcpy(test_cont->words[7],";");
   assert(!valid_append(test_cont));

   test_cont->position=0;
   strcpy(test_cont->words[0],"a_");
   strcpy(test_cont->words[1],"append");
   strcpy(test_cont->words[2],"FD");
   strcpy(test_cont->words[3],"abc");
   strcpy(test_cont->words[4],"{");
   strcpy(test_cont->words[5],"}");
   strcpy(test_cont->words[6],"/");
   strcpy(test_cont->words[7],";");
   assert(!valid_append(test_cont));

   free_word_cont(test_cont);


   test_cont = init_word_cont();
   test_line_cont= init_line_cont();
   strcpy(test_cont->words[0],"INITARR");
   strcpy(test_cont->words[1],"a_myarr");
   strcpy(test_cont->words[2],"a_myarr");
   strcpy(test_cont->words[3],"APPEND");
   strcpy(test_cont->words[4],"10");
   strcpy(test_cont->words[5],"16");
   strcpy(test_cont->words[6],"+");
   strcpy(test_cont->words[7],";");
   assert(run_init_arr(test_cont));
   assert(run_append(test_cont,test_line_cont));
   test_arr=assoc_lookup(test_cont->arr_map,"a_myarr");
   assert(compare_doubles(test_arr->head->data,26));

   free_word_cont(test_cont);


   test_cont = init_word_cont();
   strcpy(test_cont->words[0],"SET");
   strcpy(test_cont->words[1],"A");
   strcpy(test_cont->words[2],":=");
   strcpy(test_cont->words[3],"10");
   strcpy(test_cont->words[4],";");
   strcpy(test_cont->words[5],"INITARR");
   strcpy(test_cont->words[6],"a_myarr");
   strcpy(test_cont->words[7],"a_myarr");
   strcpy(test_cont->words[8],"APPEND");
   strcpy(test_cont->words[9],"A");
   strcpy(test_cont->words[10],"16");
   strcpy(test_cont->words[11],"+");
   strcpy(test_cont->words[12],";");
   assert(run_set(test_cont,test_line_cont));
   assert(run_init_arr(test_cont));
   assert(run_append(test_cont,test_line_cont));
   test_arr=assoc_lookup(test_cont->arr_map,"a_myarr");
   assert(compare_doubles(test_arr->head->data,26));

   free_word_cont(test_cont);


   test_cont = init_word_cont();
   strcpy(test_cont->words[0],"SETFUNC");
   strcpy(test_cont->words[1],"a");
   strcpy(test_cont->words[2],"{");
   strcpy(test_cont->words[3],"}");
   strcpy(test_cont->words[4],"{");
   strcpy(test_cont->words[5],"RETURN");
   strcpy(test_cont->words[6],"10");
   strcpy(test_cont->words[7],"}");
   strcpy(test_cont->words[8],"INITARR");
   strcpy(test_cont->words[9],"a_myarr");
   strcpy(test_cont->words[10],"a_myarr");
   strcpy(test_cont->words[11],"APPEND");
   strcpy(test_cont->words[12],"a");
   strcpy(test_cont->words[13],"{");
   strcpy(test_cont->words[14],"}");
   strcpy(test_cont->words[15],"16");
   strcpy(test_cont->words[16],"+");
   strcpy(test_cont->words[17],";");
   assert(run_funcset(test_cont));
   assert(run_init_arr(test_cont));
   assert(run_append(test_cont,test_line_cont));
   test_arr=assoc_lookup(test_cont->arr_map,"a_myarr");
   assert(compare_doubles(test_arr->head->data,26));
   free_word_cont(test_cont);


   test_cont = init_word_cont();

   strcpy(test_cont->words[0],"INITARR");
   strcpy(test_cont->words[1],"a_myarr");
   strcpy(test_cont->words[2],"a_");
   strcpy(test_cont->words[3],"APPEND");
   strcpy(test_cont->words[4],"10");
   strcpy(test_cont->words[5],"16");
   strcpy(test_cont->words[6],"+");
   strcpy(test_cont->words[7],";");
   assert(run_init_arr(test_cont));
   assert(!run_append(test_cont,test_line_cont));
   assert(strcmp(test_cont->err_message,"array not found - potentially not initalised yet")==0);
   free_word_cont(test_cont);

   test_cont = init_word_cont();
   strcpy(test_cont->words[0],"INITARR");
   strcpy(test_cont->words[1],"a_myarr");
   strcpy(test_cont->words[2],"a_myarr");
   strcpy(test_cont->words[3],"APPEND");
   strcpy(test_cont->words[4],"A");
   strcpy(test_cont->words[5],"16");
   strcpy(test_cont->words[6],"+");
   strcpy(test_cont->words[7],";");
   assert(run_init_arr(test_cont));
   assert(!run_append(test_cont,test_line_cont));
   free_word_cont(test_cont);


   test_cont = init_word_cont();
   strcpy(test_cont->words[0],"INITARR");
   strcpy(test_cont->words[1],"a_myarr");
   strcpy(test_cont->words[2],"a_myarr");
   strcpy(test_cont->words[3],"APPEND");
   strcpy(test_cont->words[4],"1");
   strcpy(test_cont->words[5],"16");
   strcpy(test_cont->words[6],"12");
   strcpy(test_cont->words[7],";");
   assert(run_init_arr(test_cont));
   assert(!run_append(test_cont,test_line_cont));
   free_word_cont(test_cont);



   test_cont = init_word_cont();
   strcpy(test_cont->words[0],"1.1");
   assert(!get_valid_ind(test_cont,test_line_cont,&test_int));
   test_cont->position=0;


   strcpy(test_cont->words[0],"-1");
   assert(!get_valid_ind(test_cont,test_line_cont,&test_int));
   test_cont->position=0;


   strcpy(test_cont->words[0],"1");
   assert(get_valid_ind(test_cont,test_line_cont,&test_int));
   test_cont->position=0;
   assert(test_int==1);
   strcpy(test_cont->words[0],"0");
   assert(get_valid_ind(test_cont,test_line_cont,&test_int));
   test_cont->position=0;
   assert(test_int==0);
   free_word_cont(test_cont);

   test_cont = init_word_cont();
   strcpy(test_cont->words[0],"a_bc");
   strcpy(test_cont->words[1],"[");
   strcpy(test_cont->words[2],"1");
   strcpy(test_cont->words[3],"]");
   strcpy(test_cont->words[4],":=");
   strcpy(test_cont->words[5],"10");
   strcpy(test_cont->words[6],";");
   assert(valid_change(test_cont));
   test_cont->position=0;
   strcpy(test_cont->words[5],"A");
   assert(valid_change(test_cont));
   test_cont->position=0;
   strcpy(test_cont->words[5],"1.1");
   assert(valid_change(test_cont));
   test_cont->position=0;
   strcpy(test_cont->words[5],"1");
   strcpy(test_cont->words[6],"1");
   strcpy(test_cont->words[7],"2");
   strcpy(test_cont->words[8],";");
   assert(valid_change(test_cont));

   free_word_cont(test_cont);

   test_cont = init_word_cont();
   strcpy(test_cont->words[0],"INITARR");
   strcpy(test_cont->words[1],"a_myarr");
   strcpy(test_cont->words[2],"a_myarr");
   strcpy(test_cont->words[3],"APPEND");
   strcpy(test_cont->words[4],"1");
   strcpy(test_cont->words[5],";");
   strcpy(test_cont->words[6],"a_myarr");
   strcpy(test_cont->words[7],"[");
   strcpy(test_cont->words[8],"0");
   strcpy(test_cont->words[9],"]");
   strcpy(test_cont->words[10],":=");
   strcpy(test_cont->words[11],"10");
   strcpy(test_cont->words[12],";");
   assert(run_init_arr(test_cont));
   assert(run_append(test_cont,test_line_cont));
   assert(run_change(test_cont,test_line_cont));
   assert(test_cont->position==13);
   test_arr=assoc_lookup(test_cont->arr_map,"a_myarr");
   assert(compare_doubles(test_arr->head->data,10));

   free_word_cont(test_cont);
   free_line_cont(test_line_cont);
   /*test  and run_change*/
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
   if(strcmp(to_check->words[to_check->position],"INITARR")==0)
   {
      to_check->position++;
      if(get_arr_identifier(to_check,arr_name))
      {
         n_arr=arr_init();
         store_arr(to_check,arr_name,n_arr);
         return true;
      }
   }
   free(arr_name);
   return false;
}

void store_arr(word_cont* to_check,char arr_name[MAXARRLEN],turt_arr* n_arr)
{
   /*find mains hash map for arrays as arrays are global*/
   turt_arr* existing;

   while(to_check->parent)
   {
      to_check=to_check->parent;
   }
   existing=assoc_lookup(to_check->arr_map,arr_name);
   if(existing)
   {
      free_arr(existing);
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


bool valid_append(word_cont* to_check)
{
   if(valid_arr_identifier(to_check))
   {
      to_check->position++;
      if(strcmp(to_check->words[to_check->position],"APPEND")==0)
      {
         to_check->position++;
         if(valid_polish(to_check))
         {
            return true;
         }
      }
   }
   return false;
}


bool run_append(word_cont* to_check,line_cont* line_arr)
{
   char arr_name[MAXARRLEN];
   turt_arr* arr;
   double num;
   if(get_arr_identifier(to_check,arr_name))
   {
      if(strcmp(to_check->words[to_check->position],"APPEND")==0)
      {
         to_check->position++;
         /*returns null if not found*/
         arr=get_arr(to_check,arr_name);
         if(run_polish(to_check,&num,line_arr))
         {
            /*returns false if null passed in*/
            if(!append_arr(num,arr))
            {
               strcpy(to_check->err_message,"array not found - potentially not initalised yet");
               return false;
            }
            return true;
         }
      }
   }
   return false;
}


bool valid_change(word_cont* to_check)
{
   if(valid_arr_identifier(to_check))
   {
      to_check->position++;
      if(strcmp(to_check->words[to_check->position],"[")==0)
      {
         to_check->position++;
         if(valid_varnum(to_check))
         {
            if(strcmp(to_check->words[to_check->position],"]")==0)
            {
               to_check->position++;
               if(strcmp(to_check->words[to_check->position],":=")==0)
               {
                  to_check->position++;
                  if(valid_polish(to_check))
                  {
                     return true;
                  }
               }
            }

         }
      }
   }
   return false;
}


bool run_change(word_cont* to_check,line_cont* line_arr)
{
   char arr_name[MAXARRLEN];
   turt_arr* arr;
   double num;
   int ind;
   if(get_arr_identifier(to_check,arr_name))
   {
      if(change_helper(to_check,line_arr,&ind,&num))
      {
         arr=get_arr(to_check,arr_name);
         if(!arr)
         {
            strcpy(to_check->err_message,"array not found - potentially not initalised yet");
            return false;
         }
         if(change_val_arr(num,ind,arr))
         {
            return true;
         }
         strcpy(to_check->err_message,"potentially trying to change a position greater than the size of array");

      }
   }
   return false;
}
/*gets info and checks for validity*/
bool change_helper(word_cont* to_check, line_cont* line_arr,\
                  int* ind,double* num)
{
   if(strcmp(to_check->words[to_check->position],"[")==0)
   {
      to_check->position++;
      {
         if(get_valid_ind(to_check,line_arr,ind))
         {
            if(strcmp(to_check->words[to_check->position],"]")==0)
            {
               to_check->position++;
               if(strcmp(to_check->words[to_check->position],":=")==0)
               {
                  to_check->position++;
                  if(run_polish(to_check,num,line_arr))
                  {
                     return true;
                  }
               }
            }
         }
      }
   }
   return false;
}

/*guard aginst decimals being used as index*/
bool get_valid_ind(word_cont* to_check,line_cont* line_arr,int* ind)
{
   double num;
   int i_num;
   if(get_varnum(to_check,&num,line_arr))
   {
      if(num<0)
      {
         strcpy(to_check->err_message,"negative indexes not allowed");
         return false;
      }
      i_num=num;
      if(!((num-(double)i_num)<EPSILON))
      {
         strcpy(to_check->err_message,"decimal indexes not allowed");
         return false;
      }
      *ind=i_num;
      return true;
   }
   return false;
}


turt_arr* get_arr(word_cont* to_check,char arr_name[MAXARRLEN])
{
   while(to_check->parent)
   {
      to_check=to_check->parent;
   }
   return assoc_lookup(to_check->arr_map,arr_name);
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



bool compare_doubles(double d_1, double d_2)
{
   double temp;
   temp= d_1 - d_2;
   if(temp<EPSILON && temp > -EPSILON)
   {
      return true;
   }
   return false;
}
