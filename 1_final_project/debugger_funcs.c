#include "debugger_funcs.h"




bool step_instruction(debugger* to_check)
{
   int init_pos;
   init_pos=to_check->program->position;
   if(get_rotation(to_check->program,to_check->output))
   {
      return true;
   }
   to_check->position=init_pos;
   if(move_forward(to_check->program,to_check->output))
   {
      return true;
   }
   return false;
}


bool step_do(debugger* debug)
{
   word_cont* to_check;
   loop_tracker* record;
   int end_pos;
   to_check = debug->program;
   if(strcmp(to_check->words[to_check->position],"DO")==0)
   {
      record=(loop_tracker*)safe_calloc(1,sizeof(loop_tracker));
      if(get_var_pos(to_check,&record->var_pos))
      {
         if(strcmp(to_check->words[to_check->position],"FROM")==0)
         {
            to_check->position++;
            if(get_varnum(to_check,&record->current_val))
            {
               if(strcmp(to_check->words[to_check->position],"TO")==0)
               {
                  to_check->position++;
                  if(get_varnum(to_check,&record->end_val))
                  {
                     if(strcmp(to_check->words[to_check->position],"{")==0)
                     {
                        to_check->position++;
                        record->start_pos=to_check->position;
                        end_pos=find_end_pos(to_check);
                        if(end_pos==NOTFOUND)
                        {
                           return false;
                        }
                        record->end_pos=end_pos;
                     }
                  }
               }
            }
         }
      }
   }
}


loop_tracker* step_do_helper(word_cont* to_check)
{
   loop_tracker* record;
   if(strcmp(to_check->words[to_check->position],"DO")==0)
   {
      record=(loop_tracker*)safe_calloc(1,sizeof(loop_tracker));
      to_check->position++;
      if(strcmp(to_check->words[to_check->position],"FROM")==0)
      {
         to_check->position++;
         if(get_varnum(to_check,&record->current_val))
         {
            if(strcmp(to_check->words[to_check->position],"TO")==0)
            {
               to_check->position++;
               if(get_varnum(to_check,&record->end_val))
               {
                  if(strcmp(to_check->words[to_check->position],"{")==0)
                  {
                     to_check->position++;
                     record->start_pos=to_check->position;
                     end_pos=find_end_pos(to_check);
                     if(end_pos==NOTFOUND)
                     {
                        free(record);
                        return NULL;
                     }
                     record->end_pos=end_pos;
                     return record;
                  }
               }
            }
         }
      }
   }
}


/*cant just run valid instruct list
 as that wont work for invalid ones*/
int find_end_pos(word_cont* to_check)
{
   int left_brackets,right_brackets;
   int end_pos;
   left_brackets=START_BRACKET;
   right_brackets=0;
   end_pos=to_check->position;
   while(left_brackets!=right_brackets)
   {
      if(end_pos>=to_check->capacity)
      {
         return NOTFOUND;
      }
      if(strcmp(to_check->words[end_pos],"}")==0)
      {
         right_brackets++;
      }
      if(strcmp(to_check->words[end_pos],"{")==0)
      {
         left_brackets++;
      }
      end_pos++;
   }
   return end_pos;

}
