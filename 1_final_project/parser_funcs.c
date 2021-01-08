#include "parser_funcs.h"





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
         &&(to_check->words[to_check->position][i]!='.')\
         &&(to_check->words[to_check->position][i]!='-'))
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
