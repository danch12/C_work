#include "debugger_funcs.h"



/*returns true if no mistakes else false*/
bool advance_to_mistake(debugger* to_check)
{
   while(step_instruction(to_check));
   return check_step_end(to_check);
}



bool step_instruction(debugger* to_check)
{
   opcode current_op;
   int init_pos;

   if(instruct_checks(to_check))
   {

      current_op=get_opcode(to_check->program);

      init_pos=to_check->program->position;
      switch(current_op)
      {
         case fd:
         if(move_forward(to_check->program,to_check->output))
         {
            return true;
         }
         to_check->program->position=init_pos;
         break;

         case rot:
         if(get_rotation(to_check->program,to_check->output))
         {
            return true;
         }
         to_check->program->position=init_pos;
         break;
         case do_loop:
         /*dont want to reset position for do loop as want
          to see where in loop we go wrong if needed*/
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
         to_check->program->position=init_pos;
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
   /*end checks if we are at end of one iteration
   fin checks if we are at the end of all iters*/

   check_loop_end(to_check);
   while(check_loop_fin(to_check))
   {
      check_loop_end(to_check);
   }

   if(to_check->program->position>=\
      to_check->program->capacity)
   {
      return false;
   }
   init_pos=to_check->program->position;

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
            free(record);
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

      if(end_pos>to_check->capacity)
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

/*going to have the vars like A = 1.453 to two dps to start with
this means for each var there is at least 10 chars although maybe more
so we want to give space for wiggle room- going to say that we only want
4 digits above the decimal point so will cut off after that. Also want to
format it nicely so will have 5 rows of 5 and 1 row of 1 at the end
(26 is an annoying number)*/
void show_current_vars(debugger* debug,char out_str[FULLARGSTRLEN])
{
   char temp[ONEARGLEN];
   char temp_num[ONEARGLEN];
   int i;

   out_str[0]='\0';
   for(i=0;i<NUMVARS;i++)
   {
      if(i%5==0&&i!=0)
      {
         strcat(out_str,"\n");
      }
      if(debug->program->var_array[i])
      {
         temp[0]='\0';
         str_num(*debug->program->var_array[i],temp_num);
         sprintf(temp,"%c",i+TOALPHA);
         strcat(temp,temp_num);
      }
      else
      {
         temp[0]='\0';
         sprintf(temp_num,"= NA ");
         sprintf(temp,"%c",i+TOALPHA);
         strcat(temp,temp_num);
      }
      strcat(out_str,temp);

   }
}

void str_num(double num,char num_str[ONEARGLEN])
{
   int i_num;
   if(num>MAXPOSLEN)
   {
      while(num>MAXPOSLEN)
      {
         num=num/10;
      }
      sprintf(num_str,"= %d",(int)num);
      strcat(num_str,".. ");
   }
   else
   {
      /*dont want it to round*/
      num=num*TWODP;
      i_num=num;
      num= (double)i_num/TWODP;
      sprintf(num_str,"= %.2f ",num);
   }
}


/*void show_coords(debugger* debug)*/

/*returns false if weird action entered*/
bool run_action(debugger* to_check, char action_str[MAXACTIONLEN],\
               char result_str[FULLARGSTRLEN])
{
   action curr_action;
   curr_action=get_action(action_str);
   switch(curr_action)
   {
      case s_step:
      if(!step_instruction(to_check))
      {
         collate_instruct_messages(to_check,result_str);
         to_check->stop=true;
      }
      if(to_check->program->position>=\
         to_check->program->capacity)
      {
         to_check->stop=true;
      }
      return true;

      case to_mistake:
      if(!advance_to_mistake(to_check))
      {
         collate_instruct_messages(to_check,result_str);
      }
      to_check->stop=true;
      return true;

      case show_vars:
      show_current_vars(to_check,result_str);
      return true;
      case show_pos:
      sprintf(result_str,"position = %d",to_check->program->position);
      return true;
      default:
      return false;
   }
}


void collate_instruct_messages(debugger* to_check,\
                              char result_str[FULLARGSTRLEN])
{
   if(check_step_end(to_check))
   {
      strcpy(result_str,to_check->info);
   }
   else
   {

      sprintf(result_str,"error around word %d %s\n",\
      to_check->program->position,\
      to_check->program->words[to_check->program->position]);
      if(strlen(to_check->info)>0)
      {
         strcat(result_str,to_check->info);
         strcat(result_str,"\n");
      }
      if(strlen(to_check->program->err_message)>0)
      {
         strcat(result_str,to_check->program->err_message);
         strcat(result_str,"\n");
      }
   }
}


bool check_step_end(debugger* to_check)
{
   if(to_check->program->position==to_check->program->capacity)
   {
      if(strcmp(to_check->program->words[to_check->program->position],\
         "}")==0)
      {
         if(to_check->loop_stack->size==0)
         {
            strcpy(to_check->info,"no mistakes found. At end of program");
            return true;
         }

      }
   }
   if(to_check->program->position>=to_check->program->capacity)
   {
      strcpy(to_check->info,"missing bracket in code");
   }
   return false;
}



action get_action(char action_str[MAXACTIONLEN])
{
   if(strcmp(action_str,"single step\n")==0)
   {
      return s_step;
   }
   if(strcmp(action_str,"advance to error\n")==0)
   {
      return to_mistake;
   }
   if(strcmp(action_str,"show vars\n")==0)
   {
      return show_vars;
   }
   if(strcmp(action_str,"show pos\n")==0)
   {
      return show_pos;
   }
   return invalid_act;
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
