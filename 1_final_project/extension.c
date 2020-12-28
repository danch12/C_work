#include "extension.h"

#define MAXTESTCAP 100
#define MAXTESTLEN 50
bool get_funcvar(word_cont* to_check,char func_name[MAXFUNCLEN]);
bool valid_funcvar(word_cont* to_check);

bool get_argset(word_cont* to_check,func_cont* n_func);
bool get_arg(word_cont* to_check,func_cont* n_func);
bool get_func_info(word_cont* to_check,func_cont* n_func);

func_cont* init_func_cont(void);
bool free_func_cont(func_cont* to_free);
word_cont* init_word_cont(void);
void resize(func_cont* n_func);




void test(void);

int main(void)
{
   word_cont* test_cont;
   func_cont* test_func;
   int i;
   char word[MAXTESTLEN];


   test_cont=init_word_cont();
   strcpy(test_cont->words[0],"aaabbbccc");
   assert(valid_funcvar(test_cont));
   strcpy(test_cont->words[0],"aaAaa");
   assert(!valid_funcvar(test_cont));
   strcpy(test_cont->words[0],"aa|aa");
   assert(!valid_funcvar(test_cont));
   strcpy(test_cont->words[0],"");
   assert(!valid_funcvar(test_cont));
   strcpy(test_cont->words[0],"z");
   assert(valid_funcvar(test_cont));

   strcpy(test_cont->words[0],"aaabbbccc");
   assert(get_funcvar(test_cont,word));
   assert(strcmp(word,"aaabbbccc")==0);
   assert(test_cont->position==1);

   strcpy(test_cont->words[0],"aaaAaa");
   test_cont->position=0;
   assert(!get_funcvar(test_cont,word));
   strcpy(test_cont->words[0],"aa|aa");
   assert(!get_funcvar(test_cont,word));
   strcpy(test_cont->words[0],"");
   assert(!get_funcvar(test_cont,word));

   strcpy(test_cont->words[0],"z");
   assert(get_funcvar(test_cont,word));
   assert(strcmp(word,"z")==0);

   test_func=init_func_cont();
   for(i=0;i<NUMVARS;i++)
   {
      assert(test_func->arg_placer[i]==UNUSED);
   }
   strcpy(test_cont->words[0],"A");
   test_cont->position=0;
   assert(get_arg(test_cont,test_func));
   assert(test_func->arg_placer[0]==0);
   assert(test_cont->position==1);
   test_cont->position=0;
   assert(!get_arg(test_cont,test_func));
   assert(strcmp(test_cont->err_message,"used same var twice when defining function")==0);
   free_func_cont(test_func);

   test_func=init_func_cont();
   strcpy(test_cont->words[0],"Z");
   assert(get_arg(test_cont,test_func));
   assert(test_func->arg_placer[0]==25);
   assert(test_cont->position==1);
   test_cont->position=0;
   strcpy(test_cont->words[0],"B");
   assert(get_arg(test_cont,test_func));
   assert(test_func->arg_placer[1]==1);

   test_cont->position=0;
   strcpy(test_cont->words[0],"a");
   assert(!get_arg(test_cont,test_func));

   test_cont->position=0;
   strcpy(test_cont->words[0],"@");
   assert(!get_arg(test_cont,test_func));

   test_cont->position=0;
   strcpy(test_cont->words[0],"");
   assert(!get_arg(test_cont,test_func));

   test_cont->position=0;
   strcpy(test_cont->words[0],"AA");
   assert(!get_arg(test_cont,test_func));

   free_func_cont(test_func);
   free_word_cont(test_cont);

   test_func=init_func_cont();
   test_cont=init_word_cont();

   strcpy(test_cont->words[0],"A");
   strcpy(test_cont->words[1],"B");
   strcpy(test_cont->words[2],"C");
   strcpy(test_cont->words[3],"}");

   assert(get_argset(test_cont,test_func));
   assert(test_func->arg_placer[0]==0);
   assert(test_func->arg_placer[1]==1);
   assert(test_func->arg_placer[2]==2);
   assert(test_cont->position==4);

   free_func_cont(test_func);
   test_func=init_func_cont();

   test_cont->position=0;
   strcpy(test_cont->words[3],"D");
   assert(!get_argset(test_cont,test_func));


   free_func_cont(test_func);
   test_func=init_func_cont();

   test_cont->position=0;
   strcpy(test_cont->words[2],"B");
   strcpy(test_cont->words[3],"}");
   assert(!get_argset(test_cont,test_func));
   assert(strcmp(test_cont->err_message,"used same var twice when defining function")==0);

   test_cont->position=0;
   free_func_cont(test_func);
   test_func=init_func_cont();
   strcpy(test_cont->words[0],"A");
   strcpy(test_cont->words[1],"DD");
   strcpy(test_cont->words[2],"C");
   strcpy(test_cont->words[3],"}");
   assert(!get_argset(test_cont,test_func));

   free_func_cont(test_func);

   test_func=init_func_cont();
   test_func->words[0]=safe_calloc(10,1);
   strcpy(test_func->words[0],"hello");

   test_func->words[2]=safe_calloc(10,1);
   strcpy(test_func->words[2],"goodbye");

   resize(test_func);
   assert(strcmp(test_func->words[0],"hello")==0);
   assert(strcmp(test_func->words[2],"goodbye")==0);
   free_func_cont(test_func);
   free_word_cont(test_cont);

   test_func=init_func_cont();
   test_cont=init_word_cont();

   strcpy(test_cont->words[0],"FD");
   strcpy(test_cont->words[1],"30");
   strcpy(test_cont->words[2],"LT");
   strcpy(test_cont->words[3],"20");
   strcpy(test_cont->words[4],"}");

   assert(get_func_info(test_cont,test_func));
   assert(strcmp(test_func->words[0],"FD")==0);
   assert(strcmp(test_func->words[4],"}")==0);
   assert(test_func->position==0);
   assert(test_cont->position==5);

   free_func_cont(test_func);
   free_word_cont(test_cont);


   test_func=init_func_cont();
   test_cont=init_word_cont();

   strcpy(test_cont->words[0],"FD");
   strcpy(test_cont->words[1],"30");
   strcpy(test_cont->words[2],"LT");
   strcpy(test_cont->words[3],"LT");
   strcpy(test_cont->words[4],"}");

   assert(!get_func_info(test_cont,test_func));
   test_cont->position=0;
   strcpy(test_cont->words[3],"30");
   strcpy(test_cont->words[4],"30");
   assert(!get_func_info(test_cont,test_func));
   test_cont->position=0;
   strcpy(test_cont->words[3],"");
   strcpy(test_cont->words[4],"}");
   assert(!get_func_info(test_cont,test_func));
   test_cont->position=0;

   strcpy(test_cont->words[0],"FD");
   strcpy(test_cont->words[1],"-30");
   strcpy(test_cont->words[2],"LT");
   strcpy(test_cont->words[3],"30");
   strcpy(test_cont->words[4],"}");
   assert(get_func_info(test_cont,test_func));
   free_func_cont(test_func);
   free_word_cont(test_cont);

   test_func=init_func_cont();
   test_cont=init_word_cont();
   strcpy(test_cont->words[0],"FD");
   strcpy(test_cont->words[1],"-30.907");
   strcpy(test_cont->words[2],"LT");
   strcpy(test_cont->words[3],"30");
   strcpy(test_cont->words[4],"}");
   assert(get_func_info(test_cont,test_func));
   free_func_cont(test_func);
   free_word_cont(test_cont);


   test_cont=init_word_cont();
   strcpy(test_cont->words[0],"SETFUNC");
   strcpy(test_cont->words[1],"abc");
   strcpy(test_cont->words[2],"{");
   strcpy(test_cont->words[3],"A");
   strcpy(test_cont->words[4],"}");
   strcpy(test_cont->words[5],"{");
   strcpy(test_cont->words[6],"FD");
   strcpy(test_cont->words[7],"A");
   strcpy(test_cont->words[8],"}");
   assert(run_funcset(test_cont));
   

   return 0;
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


   return n_cont;
}


func_cont* init_func_cont(void)
{
   func_cont* n_cont;
   int i;
   n_cont=(func_cont*)safe_calloc(1,sizeof(func_cont));
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
   return n_cont;
}

bool free_func_cont(func_cont* to_free)
{
   int i;
   if(to_free)
   {
      for(i=0;i<to_free->capacity;i++)
      {
         if(to_free->words[i])
         {
            free(to_free->words[i]);
         }
      }
      /*need to change at some point*/
      assoc_free(to_free->func_map);
      stack_free(to_free->stackptr);
      free(to_free->words);
      free(to_free);
   }
   return true;
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

bool get_funcvar(word_cont* to_check,char func_name[MAXFUNCLEN])
{
   if(valid_funcvar(to_check))
   {
      strcpy(func_name,to_check->words[to_check->position]);
      to_check->position++;
      return true;
   }
   return false;
}

/*downside is that we will only see errors like using undeclared vars
when the function is called not when it is set*/
bool get_func_info(word_cont* to_check,func_cont* n_func)
{
   int initial_pos;
   initial_pos=to_check->position;
   if(valid_instructlist(to_check))
   {

      to_check->position=initial_pos;
      /*now we know it is valid we can just copy over*/
      while(strcmp(to_check->words[to_check->position],"}"))
      {
         n_func->words[n_func->position]=(char*)safe_calloc(strlen(to_check->words[to_check->position])+1,\
                     sizeof(char));
         strcpy(n_func->words[n_func->position],\
               to_check->words[to_check->position]);
         n_func->position++;
         to_check->position++;
         if(n_func->position>=n_func->capacity)
         {
            resize(n_func);
         }
      }
      /*copy over the } and move word_cont on*/
      n_func->words[n_func->position]=(char*)safe_calloc(strlen(to_check->words[to_check->position])+1,\
                  sizeof(char));
      strcpy(n_func->words[n_func->position],\
            to_check->words[to_check->position]);
      to_check->position++;
      /*set func position to zero*/
      n_func->position=0;
      return true;
   }
   return false;
}


void resize(func_cont* n_func)
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





/*need to test setting loads of functions see if resizing
breaks it*/


bool run_funcset(word_cont* to_check)
{
   func_cont* n_func;
   char* func_name;
   func_name=(char*)safe_calloc(MAXFUNCLEN,sizeof(char));
   n_func=init_func_cont();
   if(strcmp(to_check->words[to_check->position],"SETFUNC"))
   {
      to_check->position++;
      if(get_funcvar(to_check,func_name))
      {
         if(strcmp(to_check->words[to_check->position],"{"))
         {
            to_check->position++;
            if(get_argset(to_check,n_func))
            {
               if(strcmp(to_check->words[to_check->position],"{"))
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
   free_func_cont(n_func);
   free(func_name);
   return false;
}


bool get_argset(word_cont* to_check,func_cont* n_func)
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

bool get_arg(word_cont* to_check,func_cont* n_func)
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
      to_check->position++;
      return true;
   }
   return false;
}






/*
bool run_set(word_cont* to_check)
{
   double to_set;
   int var_p;
   if(to_check->position>=to_check->capacity)
   {
      return false;
   }
   if(strcmp(to_check->words[to_check->position],"SET")==0)
   {
      to_check->position++;
      if(get_var_pos(to_check,&var_p))
      {
         if(strcmp(to_check->words[to_check->position],":=")==0)
         {
            to_check->position++;

            if(run_polish(to_check,&to_set))
            {
               if(!to_check->var_array[var_p])
               {
                  to_check->var_array[var_p]=safe_calloc(1,\
                                             sizeof(double));
               }
               *to_check->var_array[var_p]=to_set;
               return true;
            }
         }
      }
   }
   return false;
}
*/
