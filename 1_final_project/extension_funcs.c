#include "extension_funcs.h"




word_cont* init_func_cont(void)
{
   word_cont* n_cont;
   int i;
   n_cont=(word_cont*)safe_calloc(1,sizeof(word_cont));
   n_cont->capacity=INITFUNCCAP;
   n_cont->words=(char**)safe_calloc(INITFUNCCAP,sizeof(char*));
   n_cont->position=0;
   for(i=0;i<NUMVARS;i++)
   {
      n_cont->var_array[i]=NULL;
      n_cont->arg_placer[i]=UNUSED;
   }
   n_cont->func_map=assoc_init();
   n_cont->stackptr=stack_init();
   n_cont->n_args=0;
   n_cont->err_message[0]='\0';
   return n_cont;
}



bool valid_funcvar(word_cont* to_check)
{
   int i;
   if(to_check->position>=to_check->capacity)
   {
      return false;
   }
   i=0;
   if(strlen(to_check->words[to_check->position])==0)
   {
      return false;
   }
   while(to_check->words[to_check->position][i])
   {
      if(!(to_check->words[to_check->position][i]>='a')||\
         !(to_check->words[to_check->position][i]<='z'))
      {
         return false;
      }
      i++;
   }
   return true;
}

bool set_funcvar(word_cont* to_check,char func_name[MAXFUNCLEN])
{
   if(valid_funcvar(to_check))
   {
      strcpy(func_name,to_check->words[to_check->position]);
      to_check->position++;
      return true;
   }
   return false;
}

bool valid_funcset(word_cont* to_check)
{
   if(strcmp(to_check->words[to_check->position],"SETFUNC")==0)
   {
      to_check->position++;
      if(valid_funcvar(to_check))
      {
         to_check->position++;
         if(strcmp(to_check->words[to_check->position],"{")==0)
         {
            to_check->position++;
            if(valid_argset(to_check))
            {
               if(strcmp(to_check->words[to_check->position],"{")==0)
               {
                  to_check->position++;
                  {
                     if(valid_instructlist(to_check))
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

bool get_funcvar(word_cont* to_check,word_cont** to_get)
{
   if(valid_funcvar(to_check))
   {
      *to_get=assoc_lookup(to_check->func_map,\
                           to_check->words[to_check->position]);
      if(*to_get==NULL)
      {
         strcpy(to_check->err_message,"function name not found, has it been set?");
         return false;
      }
      to_check->position++;
      return true;
   }
   return false;
}

/*downside is that we will only see errors like using undeclared vars
when the function is called not when it is set*/
bool get_func_info(word_cont* to_check,word_cont* n_func)
{
   int initial_pos;
   initial_pos=to_check->position;
   if(valid_instructlist(to_check))
   {

      to_check->position=initial_pos;
      if(copy_over(to_check,n_func))
      {
         n_func->position=0;
         return true;
      }
      n_func->position=0;
      return true;
   }
   return false;
}

/*there are better ways to balance brackets out there
but this will be adequate*/
bool copy_over(word_cont* to_check,word_cont* n_func)
{

   int left_brackets,right_brackets;
   left_brackets=START_BRACKET;
   right_brackets=0;
   while(left_brackets!=right_brackets)
   {
      if(to_check->position>=to_check->capacity)
      {
         return false;
      }
      if(strcmp(to_check->words[to_check->position],"}")==0)
      {
         right_brackets++;
      }
      if(strcmp(to_check->words[to_check->position],"{")==0)
      {
         left_brackets++;
      }
      n_func->words[n_func->position]=(char*)safe_calloc(\
            strlen(to_check->words[to_check->position])+1,sizeof(char));

      strcpy(n_func->words[n_func->position],\
            to_check->words[to_check->position]);

      n_func->position++;
      to_check->position++;
      if(n_func->position>=n_func->capacity)
      {
         resize(n_func);
      }
   }

   return true;
}

void resize(word_cont* n_func)
{
   char** n_words;
   char** temp;
   int i;
   n_words=(char**)safe_calloc(n_func->capacity*FUNCSCALE,\
                              sizeof(char*));
   for(i=0;i<n_func->capacity;i++)
   {
      if(n_func->words[i])
      {
         n_words[i]=(char*)safe_calloc(strlen(n_func->words[i])+1,\
                                       sizeof(char));
         strcpy(n_words[i],n_func->words[i]);
      }
   }
   temp=n_func->words;
   n_func->words=n_words;

   for(i=0;i<n_func->capacity;i++)
   {
      if(temp[i])
      {
         free(temp[i]);
      }

   }
   n_func->capacity=n_func->capacity*FUNCSCALE;
   free(temp);
}






bool run_funcset(word_cont* to_check)
{
   word_cont* n_func;
   char* func_name;
   func_name=(char*)safe_calloc(MAXFUNCLEN,sizeof(char));
   n_func=init_func_cont();
   if(strcmp(to_check->words[to_check->position],"SETFUNC")==0)
   {

      to_check->position++;
      if(set_funcvar(to_check,func_name))
      {

         if(strcmp(to_check->words[to_check->position],"{")==0)
         {

            to_check->position++;
            if(get_argset(to_check,n_func))
            {
               if(strcmp(to_check->words[to_check->position],"{")==0)
               {
                  to_check->position++;
                  if(get_func_info(to_check,n_func))
                  {
                     assoc_insert(&to_check->func_map,func_name,\
                                 n_func);
                     return true;
                  }
               }
            }
         }
      }
   }
   free_word_cont(n_func);
   free(func_name);
   return false;
}


bool valid_argset(word_cont* to_check)
{
   if(strcmp(to_check->words[to_check->position],"}")==0)
   {
      to_check->position++;
      return true;
   }
   if(valid_variable(to_check))
   {
      to_check->position++;
      if(valid_argset(to_check))
      {
         return true;
      }
   }
   return false;
}


bool get_argset(word_cont* to_check,word_cont* n_func)
{
   if(strcmp(to_check->words[to_check->position],"}")==0)
   {
      to_check->position++;
      return true;
   }
   if(get_arg(to_check,n_func))
   {
      if(get_argset(to_check,n_func))
      {
         return true;
      }
   }
   return false;
}

bool get_arg(word_cont* to_check,word_cont* n_func)
{

   int i;
   int var_pos;

   if(valid_variable(to_check))
   {
      i=0;
      var_pos=to_check->words[to_check->position][0]-LETTER_TO_NUM;
      /*find first empty position in arg array*/
      while(n_func->arg_placer[i]!=UNUSED)
      {
         /*if var already used in arguments*/
         if(n_func->arg_placer[i]==var_pos)
         {
            strcpy(to_check->err_message,"used same var twice when defining function");
            return false;
         }
         i++;
      }
      n_func->arg_placer[i]= var_pos;
      n_func->n_args++;
      to_check->position++;
      return true;
   }
   return false;
}


bool place_arg(word_cont* to_check,word_cont* n_func,\
               int position)
{
   double to_place;
   int func_var;
   if(get_varnum(to_check,&to_place))
   {
      func_var= n_func->arg_placer[position];
      if(func_var==UNUSED)
      {
         strcpy(to_check->err_message,"too many arguments for function");
         return false;
      }
      n_func->var_array[func_var]=(double*)safe_calloc(1,\
                                 sizeof(double));
      *n_func->var_array[func_var]=to_place;
      return true;
   }
   return false;
}


bool place_all_args(word_cont* to_check,word_cont* n_func,int pos)
{

   if(strcmp(to_check->words[to_check->position],"}")==0)
   {

      if(pos!=n_func->n_args)
      {
         strcpy(to_check->err_message,"incorrect amount of args passed to function");
         return false;
      }
      to_check->position++;
      return true;
   }
   if(place_arg(to_check,n_func,pos))
   {
      if(place_all_args(to_check,n_func,pos+1))
      {
         return true;
      }
   }
   return false;
}


bool run_funcrun(word_cont* to_check,line_cont* line_arr)
{
   word_cont* to_run;
   to_run=NULL;
   if(get_funcvar(to_check,&to_run))
   {
      if(strcmp(to_check->words[to_check->position],"{")==0)
      {
         to_check->position++;
         if(place_all_args(to_check,to_run,START))
         {
            if(run_instruction_list(to_run,line_arr))
            {
               /*set back to beginning*/
               reset_func(to_run);
               return true;
            }
         }
      }
   }
   return false;
}


bool valid_argsrun(word_cont* to_check)
{
   if(strcmp(to_check->words[to_check->position],"}")==0)
   {
      to_check->position++;
      return true;
   }
   if(valid_varnum(to_check))
   {
      if(valid_argsrun(to_check))
      {
         return true;
      }
   }
   return false;
}

bool valid_funcrun(word_cont* to_check)
{
   if(valid_funcvar(to_check))
   {
      to_check->position++;
      if(strcmp(to_check->words[to_check->position],"{")==0)
      {
         to_check->position++;
         if(valid_argsrun(to_check))
         {
            return true;
         }
      }
   }
   return false;

}

void reset_func(word_cont* func)
{
   int i;
   func->position=0;
   for(i=0;i<NUMVARS;i++)
   {
      if(func->var_array[i])
      {
         free(func->var_array[i]);
         func->var_array[i]=NULL;
      }
   }
}