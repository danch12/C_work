#include "extension_arrays.h"
/*

#define MAXTESTCAP 100
#define MAXTESTLEN 70
#define MAXARRLEN 50
#define EPSILON 0.00001
*/







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
   if(change_helper(to_check,line_arr,&ind,&num,arr_name))
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

   return false;
}
/*gets info and checks for validity*/
bool change_helper(word_cont* to_check, line_cont* line_arr,\
                  int* ind,double* num,char arr_name[MAXARRLEN])
{
   if(get_arr_identifier(to_check,arr_name))
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


bool valid_delete_arr_val(word_cont* to_check)
{

   if(strcmp(to_check->words[to_check->position],"DEL")==0)
   {
      to_check->position++;
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
                  return true;
               }

            }
         }
      }
   }
   return false;
}

bool run_delete_arr_val(word_cont* to_check,line_cont* line_arr)
{
   char arr_name[MAXARRLEN];
   turt_arr* arr;

   int ind;
   if(delete_helper(to_check,line_arr,&ind,arr_name))
   {
      arr=get_arr(to_check,arr_name);
      if(!remove_val(arr,ind))
      {
         strcpy(to_check->err_message,"array not found - potentially not initalised yet");
         return false;
      }
      return true;
   }
   return false;

}

bool delete_helper(word_cont* to_check,line_cont* line_arr,\
                  int* ind,char arr_name[MAXARRLEN])
{
   if(strcmp(to_check->words[to_check->position],"DEL")==0)
   {
      to_check->position++;
      if(get_arr_identifier(to_check,arr_name))
      {
         if(strcmp(to_check->words[to_check->position],"[")==0)
         {
            to_check->position++;
            if(get_valid_ind(to_check,line_arr,ind))
            {
               if(strcmp(to_check->words[to_check->position],"]")==0)
               {
                  to_check->position++;
                  return true;
               }
            }
         }
      }
   }
   return false;
}


bool valid_access_val(word_cont* to_check)
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
               return true;
            }

         }
      }
   }
   return false;
}


bool run_access_val(word_cont* to_check,line_cont* line_arr,double* num)
{
   int ind;
   char arr_name[MAXARRLEN];
   turt_arr* arr;
   if(access_helper(to_check,line_arr,arr_name,&ind))
   {
      arr=get_arr(to_check,arr_name);
      if(!arr)
      {
         strcpy(to_check->err_message,"array not found - potentially not initalised yet");
         return false;
      }
      if(!get_arr_val(arr,ind,num))
      {
         strcpy(to_check->err_message,"index greater than size of array");
         return false;
      }
      return true;
   }
   return false;

}

bool access_helper(word_cont* to_check,line_cont* line_arr,\
                  char arr_name[MAXARRLEN],int* ind)
{
   if(get_arr_identifier(to_check,arr_name))
   {
      if(strcmp(to_check->words[to_check->position],"[")==0)
      {
         to_check->position++;
         if(get_valid_ind(to_check,line_arr,ind))
         {
            if(strcmp(to_check->words[to_check->position],"]")==0)
            {
               to_check->position++;
               return true;
            }
         }
      }
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
