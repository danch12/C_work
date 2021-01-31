#include "extension_arrays.h"





bool valid_init_arr(word_cont* to_check)
{
   if(safe_samestr(to_check,"INITARR"))
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
   if(safe_samestr(to_check,"INITARR"))
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

void store_arr(word_cont* to_check,char arr_name[MAXARRLEN],\
               turt_arr* n_arr)
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


bool get_arr_identifier(word_cont* to_check,\
                     char arr_name[MAXARRLEN])
{
   if(valid_arr_identifier(to_check))
   {
      strcpy(arr_name,to_check->words[to_check->position]);
      to_check->position++;
      return true;
   }
   if(strlen(to_check->err_message)==0)
   {
      strcpy(to_check->err_message,"invalid array name - they have to start with a_");
   }
   return false;
}



bool valid_arr_identifier(word_cont* to_check)
{
   int i;
   if(strlen(to_check->words[to_check->position])<MINARRLEN)
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
   if(safe_samestr(to_check,"APPEND"))
   {
      to_check->position++;
      if(valid_arr_identifier(to_check))
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

   if(safe_samestr(to_check,"APPEND"))
   {
      to_check->position++;
      if(get_arr_identifier(to_check,arr_name))
      {
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
   if(safe_samestr(to_check,"CHANGE"))
   {
      to_check->position++;
      if(valid_arr_identifier(to_check))
      {
         to_check->position++;
         if(safe_samestr(to_check,"["))
         {
            to_check->position++;
            if(valid_varnum(to_check))
            {
               if(safe_samestr(to_check,"]"))
               {
                  to_check->position++;
                  if(safe_samestr(to_check,":="))
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
   if(safe_samestr(to_check,"CHANGE"))
   {
      to_check->position++;
      if(get_arr_identifier(to_check,arr_name))
      {
         if(safe_samestr(to_check,"["))
         {
            to_check->position++;
            {
               if(get_valid_ind(to_check,line_arr,ind))
               {
                  if(safe_samestr(to_check,"]"))
                  {
                     to_check->position++;
                     if(safe_samestr(to_check,":="))
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
   }
   return false;
}

/*guard aginst decimals and negative nums being used as index*/
bool get_valid_ind(word_cont* to_check,line_cont* line_arr,\
                  int* ind)
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

   if(safe_samestr(to_check,"DEL"))
   {
      to_check->position++;
      if(valid_arr_identifier(to_check))
      {
         to_check->position++;
         if(safe_samestr(to_check,"["))
         {
            to_check->position++;
            if(valid_varnum(to_check))
            {
               if(safe_samestr(to_check,"]"))
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

bool run_delete_arr_val(word_cont* to_check,\
                        line_cont* line_arr)
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
   if(safe_samestr(to_check,"DEL"))
   {
      to_check->position++;
      if(get_arr_identifier(to_check,arr_name))
      {
         if(safe_samestr(to_check,"["))
         {
            to_check->position++;
            if(get_valid_ind(to_check,line_arr,ind))
            {
               if(safe_samestr(to_check,"]"))
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
      if(safe_samestr(to_check,"["))
      {
         to_check->position++;
         if(valid_varnum(to_check))
         {
            if(safe_samestr(to_check,"]"))
            {
               to_check->position++;
               return true;
            }

         }
      }
   }
   return false;
}


bool run_access_val(word_cont* to_check,line_cont* line_arr,\
                     double* num)
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

/*basically checks syntax*/
bool access_helper(word_cont* to_check,line_cont* line_arr,\
                  char arr_name[MAXARRLEN],int* ind)
{
   if(get_arr_identifier(to_check,arr_name))
   {
      if(safe_samestr(to_check,"["))
      {
         to_check->position++;
         if(get_valid_ind(to_check,line_arr,ind))
         {
            if(safe_samestr(to_check,"]"))
            {
               to_check->position++;
               return true;
            }
         }
      }
   }
   return false;
}

/*https://en.wikipedia.org/wiki/Filename  going off this stuff it
looks like almost everything is valid - but will need to check
that we are using txt files so can check for that*/
bool valid_filepath(word_cont* to_check)
{
   char *last_dot;
   last_dot = NULL;
   /*find position of last dot*/
   last_dot = strrchr(to_check->words[to_check->position], '.');
   if(last_dot)
   {
      if(strcmp(last_dot,".txt")==0)
      {
         return true;
      }
   }
   strcpy(to_check->err_message,"not valid file path, make sure it's a txt file");
   return false;
}

bool valid_len(word_cont* to_check)
{
   if(safe_samestr(to_check,"LEN"))
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

bool run_len(word_cont* to_check,double* num)
{
   char arr_name[MAXARRLEN];
   turt_arr* arr;
   int size;
   if(safe_samestr(to_check,"LEN"))
   {
      to_check->position++;
      if(get_arr_identifier(to_check,arr_name))
      {
         arr=get_arr(to_check,arr_name);
         if(!arr_len(arr,&size))
         {
            strcpy(to_check->err_message,"array not found\n");
            return false;
         }
         *num =(double)size;
         return true;
      }
   }
   return false;
}

bool valid_file_to_array(word_cont* to_check)
{
   if(safe_samestr(to_check,"LOAD"))
   {
      to_check->position++;
      if(valid_arr_identifier(to_check))
      {
         to_check->position++;
         if(valid_filepath(to_check))
         {
            to_check->position++;
            return true;
         }
      }
   }

   return false;
}




bool run_file_to_array(word_cont* to_check)
{
   char arr_name[MAXARRLEN];
   turt_arr* arr;

   if(safe_samestr(to_check,"LOAD"))
   {
      to_check->position++;
      if(get_arr_identifier(to_check,arr_name))
      {
         if(valid_filepath(to_check))
         {

            arr=get_arr(to_check,arr_name);
            if(!arr)
            {
               strcpy(to_check->err_message,"array not found - potentially not initalised yet");
               return false;
            }
            if(load_in(to_check,arr,\
            to_check->words[to_check->position]))
            {
               to_check->position++;
               return true;
            }
         }
      }
   }


   return false;
}

/*reads numbers from file and loads them into linked list*/
bool load_in(word_cont* to_check,turt_arr* arr,\
            char* filepath)
{
   FILE* fp;
   double num;
   char buffer[MAXLEN];
   fp= fopen(filepath,"r");
   if(fp==NULL)
   {
      strcpy(to_check->err_message,"error while opening file- does it exist?");
      return false;
   }

   while(fscanf(fp,"%s",buffer)==1)
   {
      if(sscanf(buffer,"%lf",&num)!=1)
      {
         fclose(fp);
         strcpy(to_check->err_message,"error reading data in file");
         return false;
      }
      append_arr(num,arr);
   }

   fclose(fp);
   return true;
}



turt_arr* get_arr(word_cont* to_check,\
            char arr_name[MAXARRLEN])
{
   /*find "main" word_cont*/
   while(to_check->parent)
   {
      to_check=to_check->parent;
   }
   return assoc_lookup(to_check->arr_map,arr_name);
}
