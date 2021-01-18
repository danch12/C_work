#include "debugger_funcs.h"

/*need a way of checking miscellanious } as we skip past them
potentially use the existing bracket matching func*/

/*returns true if no mistakes else false*/
bool advance_to_mistake(debugger* to_check)
{
   while(step_instruction(to_check));
   if()
}

bool step_instruction(debugger* to_check)
{
   opcode current_op;

   if(instruct_checks(to_check))
   {
      current_op=get_opcode(to_check->program);
      switch(current_op)
      {
         case fd:
         if(move_forward(to_check->program,to_check->output))
         {
            return true;
         }
         break;
         case rot:
         if(get_rotation(to_check->program,to_check->output))
         {
            return true;
         }
         break;
         case do_loop:
         if(step_do(to_check))
         {
            return true;
         }
         break;
         case set:
         if(run_set(to_check->program))
         {
            return true;
         }
         break;
         default:
         strcpy(to_check->program->err_message,"invalid opcode used");
      }
   }
   return false;
}

/*not using instruct lists so will
just skip } checks for random
brackets will be done in another function
however after skipping however many brackets
we will need to recheck for loops*/
bool instruct_checks(debugger* to_check)
{
   int init_pos;
   init_pos=to_check->program->position;

   if(to_check->program->position>=\
      to_check->program->capacity)
   {
      return false;
   }
   /*end checks if we are at end of one iteration
   fin checks if we are at the end of all iters*/
   check_loop_end(to_check);
   while(check_loop_fin(to_check))
   {

      check_loop_end(to_check);
   }
   if(strcmp(to_check->program->words\
      [to_check->program->position],"}")==0)
   {
      to_check->program->position++;
   }
   if(init_pos!=to_check->program->position)
   {
      if(instruct_checks(to_check))
      {
         return true;
      }
      return false;
   }
   return true;
}


bool check_loop_fin(debugger* to_check)
{
   void* record;
   loop_tracker* cast_r;
   record=NULL;
   /*check if there is a loop*/
   if(stack_peek(to_check->loop_stack,&record))
   {
      cast_r=(loop_tracker*)record;
      /*check to see if we are in right position*/
      if(to_check->program->position==\
         cast_r->start_pos)
      {
         /*check to see if current val greater than
         the loop finish val or the finish flag is on*/
         if(*to_check->program->var_array[cast_r->var_pos]>\
            cast_r->end_val||cast_r->fin_flag)
         {
            to_check->program->position = cast_r->end_pos;
            stack_pop(to_check->loop_stack,&record);
            free(record);
            return true;
         }
      }
   }
   return false;
}


/*checks to see if we are at the end of a loop
and if so updates to position back to start*/
void check_loop_end(debugger* to_check)
{
   void* record;
   loop_tracker* cast_r;
   record=NULL;
   if(stack_peek(to_check->loop_stack,&record))
   {
      cast_r=(loop_tracker*)record;

      if(to_check->program->position>=\
         cast_r->end_pos)
      {
         to_check->program->position=cast_r->start_pos;
         if(*to_check->program->var_array[cast_r->var_pos]<\
            cast_r->end_val)
         {
            *to_check->program->var_array[cast_r->var_pos]+=1;
         }
         /*want to show we are finished with loop without
         going past finishing number as we dont go past this
         num in our actual code*/
         else
         {
            cast_r->fin_flag=true;
         }
      }
   }
}


bool step_do(debugger* debug)
{
   loop_tracker* record;
   record =step_do_helper(debug->program);
   if(record)
   {
      if(!debug->program->var_array[record->var_pos])
      {
         debug->program->var_array[record->var_pos]=(double*)safe_calloc(1,\
                                                      sizeof(double));
      }
      *debug->program->var_array[record->var_pos]=record->start_val;
      /*unused loop just check syntax*/
      if(record->start_val>record->end_val)
      {
         if(!valid_instructlist(debug->program))
         {
            strcpy(debug->program->err_message,\
            "syntax error in unused loop");
            return false;
         }
         debug->program->position=record->start_pos;
      }

      stack_push(debug->loop_stack,record);
      free(record);
      return true;
   }
   return false;
}


loop_tracker* step_do_helper(word_cont* to_check)
{
   loop_tracker* record;
   int end_pos;
   record=NULL;
   if(strcmp(to_check->words[to_check->position],"DO")==0)
   {
      record=(loop_tracker*)safe_calloc(1,sizeof(loop_tracker));
      to_check->position++;
      if(get_var_pos(to_check,&record->var_pos))
      {
         if(strcmp(to_check->words[to_check->position],"FROM")==0)
         {
            to_check->position++;
            if(get_varnum(to_check,&record->start_val))
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
                        record->fin_flag=false;
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
   if(record)
   {
      free(record);
   }
   return NULL;
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


debugger* init_debugger(void)
{
   debugger* n_debug;

   n_debug=(debugger*)safe_calloc(1,sizeof(debugger));
   n_debug->loop_stack=stack_init(sizeof(loop_tracker));
   return n_debug;
}

bool free_debugger(debugger* to_free)
{
   if(to_free)
   {
      if(to_free->program)
      {
         free_word_cont(to_free->program);
      }
      if(to_free->output)
      {
         free_line_cont(to_free->output);
      }
      stack_free(to_free->loop_stack);
      free(to_free);
   }
   return true;
}
