#include "debugger_funcs.h"




bool step_instruction(debugger* to_check)
{
   opcode current_op;
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
      default:
      strcpy(to_check->program->err_message,"invalid opcode used");
   }
   return false;
}

/*will need to do if its equal or greater than end pos*/
void check_loop(debugger* to_check)
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
         /*increment variable*/
         *to_check->program->var_array[cast_r->var_pos]+=1;
         /*if we have got to the end val loop is in its
         final iteration and we can pop it off*/
         if(comp_doubles(*to_check->program->var_array[cast_r->var_pos],\
                        cast_r->end_val))
         {
            stack_pop(to_check->loop_stack,&record);
         }
      }
   }
}


bool step_do(debugger* debug)
{
   loop_tracker* record;
   record =step_do_helper(debug->program,debug->output);
   if(record)
   {
      if(!debug->program->var_array[record->var_pos])
      {
         debug->program->var_array[record->var_pos]=(double*)safe_calloc(1,\
                                                      sizeof(double));
      }
      *debug->program->var_array[record->var_pos]=record->start_val;
      stack_push(debug->loop_stack,record);
      return true;
   }
   return false;
}


loop_tracker* step_do_helper(word_cont* to_check,line_cont* line_arr)
{
   loop_tracker* record;
   int end_pos;
   if(strcmp(to_check->words[to_check->position],"DO")==0)
   {
      record=(loop_tracker*)safe_calloc(1,sizeof(loop_tracker));
      to_check->position++;
      if(get_var_pos(to_check,&record->var_pos))
      {
         if(strcmp(to_check->words[to_check->position],"FROM")==0)
         {
            to_check->position++;
            if(get_varnum(to_check,&record->start_val,line_arr))
            {
               if(strcmp(to_check->words[to_check->position],"TO")==0)
               {
                  to_check->position++;
                  if(get_varnum(to_check,&record->end_val,line_arr))
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
