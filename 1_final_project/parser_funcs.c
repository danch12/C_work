#include "parser_funcs.h"


/*are we allowing negative numbers???*/

/*pretty much just for testing so we dont
deal with files until end*/
/*word_cont* init_word_cont(void)
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
   return n_cont;
}


bool concat_word_cont(word_cont* to_concat, char target[MAXLEN])
{
   int i;
   if(to_concat)
   {
      target[0]='\0';
      for(i=0;i<to_concat->capacity;i++)
      {
         strcat(target,to_concat->words[i]);
      }
      return true;
   }

   return false;
}*/

bool free_word_cont(word_cont* to_free)
{
   int i;
   if(to_free)
   {
      for(i=0;i<to_free->capacity;i++)
      {
         free(to_free->words[i]);
      }
      free(to_free->words);
      free(to_free);
   }
   return true;
}



bool valid_num(word_cont* to_check)
{
   int i;
   int len;
   if(to_check->position>=to_check->capacity)
   {
      return false;
   }
   len=strlen(to_check->words[to_check->position]);
   /*blank string not number*/
   if(len==0)
   {
      return false;
   }
   for(i=0;i<len;i++)
   {
      if(!isdigit(to_check->words[to_check->position][i])\
         &&(to_check->words[to_check->position][i]!='.'))
      {
         return false;
      }
   }
   to_check->position++;
   return true;
}

bool valid_var(word_cont* to_check)
{
   if(to_check->position>=to_check->capacity)
   {
      return false;
   }
   if(strlen(to_check->words[to_check->position])==1)
   {
      if(to_check->words[to_check->position][0]>='A'&&\
         to_check->words[to_check->position][0]<='Z')
      {
         to_check->position++;
         return true;
      }
   }
   return false;
}

bool valid_varnum(word_cont* to_check)
{
   if(to_check->position>=to_check->capacity)
   {
      return false;
   }
   if(valid_num(to_check))
   {
      return true;
   }
   if(valid_var(to_check))
   {
      return true;
   }
   return false;
}


bool valid_mv(word_cont* to_check,char move[INSTRUCTLEN])
{
   if(to_check->position>=to_check->capacity)
   {
      return false;
   }
   if(strcmp(to_check->words[to_check->position],move)==0)
   {
      to_check->position++;
      if(valid_varnum(to_check))
      {
         return true;
      }
   }
   return false;
}


/*return back to original position after each try
so we dont try to start reading in the middle of a
command*/
bool valid_instruct(word_cont* to_check)
{
   int i;
   int init_pos;
   char instructions[NUMINSTRUCTIONS][INSTRUCTLEN]= {"FD", "LT","RT"};
   init_pos=to_check->position;
   if(init_pos>=to_check->capacity)
   {
      return false;
   }
   for(i=0;i<NUMINSTRUCTIONS;i++)
   {
      if(valid_mv(to_check,instructions[i]))
      {
         return true;
      }
      else
      {
         to_check->position=init_pos;
      }
   }
   if(valid_set(to_check))
   {
      return true;
   }
   to_check->position=init_pos;

   if(valid_do(to_check))
   {
      return true;
   }

   return false;
}



bool valid_instructlist(word_cont* to_check)
{
   if(to_check->position>=to_check->capacity)
   {
      return false;
   }
   if(strcmp(to_check->words[to_check->position],"}")==0)
   {
      /*doesnt matter for end but for do loops important
      to increase position*/
      to_check->position++;

      return true;
   }
   else
   {
      if(valid_instruct(to_check))
      {
         if(valid_instructlist(to_check))
         {
            return true;
         }
      }
   }
   return false;
}


bool valid_main(word_cont* to_check)
{
   if(to_check->position>=to_check->capacity)
   {
      return false;
   }
   if(strcmp(to_check->words[to_check->position],"{")==0)
   {
      to_check->position++;

      if(valid_instructlist(to_check))
      {
         return true;
      }
   }
   return false;
}


bool valid_op(word_cont* to_check)
{
   if(to_check->position>=to_check->capacity)
   {
      return false;
   }
   if(strcmp(to_check->words[to_check->position],"+")==0)
   {
      to_check->position++;
      return true;
   }
   if(strcmp(to_check->words[to_check->position],"-")==0)
   {
      to_check->position++;
      return true;
   }
   if(strcmp(to_check->words[to_check->position],"*")==0)
   {
      to_check->position++;
      return true;
   }
   if(strcmp(to_check->words[to_check->position],"/")==0)
   {
      to_check->position++;
      return true;
   }
   return false;
}


bool valid_polish(word_cont* to_check)
{
   if(to_check->position>=to_check->capacity)
   {
      return false;
   }
   if(strcmp(to_check->words[to_check->position],";")==0)
   {
      to_check->position++;
      return true;
   }
   if(valid_op(to_check))
   {
      if(valid_polish(to_check))
      {
         return true;
      }
   }
   if(valid_varnum(to_check))
   {
      if(valid_polish(to_check))
      {
         return true;
      }
   }
   return false;
}


bool valid_set(word_cont* to_check)
{
   if(to_check->position>=to_check->capacity)
   {
      return false;
   }
   if(strcmp(to_check->words[to_check->position],"SET")==0)
   {
      to_check->position++;
      if(valid_var(to_check))
      {
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
   return false;
}

bool valid_do(word_cont* to_check)
{
   if(to_check->position>=to_check->capacity)
   {
      return false;
   }
   if(strcmp(to_check->words[to_check->position],"DO")==0)
   {
      to_check->position++;
      if(valid_var(to_check))
      {
         if(strcmp(to_check->words[to_check->position],"FROM")==0)
         {
            to_check->position++;
            if(valid_varnum(to_check))
            {
               if(strcmp(to_check->words[to_check->position],"TO")==0)
               {
                  to_check->position++;
                  if(valid_varnum(to_check))
                  {
                     if(strcmp(to_check->words[to_check->position],"{")==0)
                     {
                        to_check->position++;
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
   }
   return false;
}


word_cont* read_in_file(char* filename)
{
   FILE* fp;
   word_cont* n_cont;
   int num_lines,count;
   char buffer[MAXLEN];
   num_lines=0;
   n_cont=(word_cont*)safe_calloc(1,sizeof(word_cont));
   fp=get_file_words(filename,&num_lines);
   n_cont->capacity=num_lines;
   n_cont->position=0;
   n_cont->words= (char**)safe_calloc(num_lines,sizeof(char*));
   count=0;
   while(fscanf(fp,"%s",buffer)==1)
   {
      n_cont->words[count]=(char*)safe_calloc(strlen(buffer)+1,\
                                             sizeof(char));
      strcpy(n_cont->words[count],buffer);
      count++;
   }
   fclose(fp);
   return n_cont;
}




FILE* get_file_words(char* filename,int* lines)
{
   FILE *fp;
   int num_lines;
   char buffer[MAXLEN];
   num_lines=0;
   fp= safe_fopen(filename);

   while(fscanf(fp,"%s",buffer)==1)
   {
      num_lines++;
   }
   *lines=num_lines;
   rewind(fp);
   return fp;
}
