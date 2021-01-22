#include "debugger_funcs.h"

void draw_lines(line_cont* l_arr);


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
         case i_list:
         to_check->program->position++;
         return true;
         default:
         strcpy(to_check->program->err_message,"invalid opcode used");
      }
   }
   return false;
}

bool check_start(debugger* to_check)
{

   if(strcmp(to_check->program->words\
      [to_check->program->position],"{")!=0)
   {
      strcpy(to_check->info,"error in starting bracket");
      return false;
   }

   return true;
}

bool initial_checks(debugger* to_check)
{
   if(check_start(to_check))
   {
      if(!check_past_main(to_check))
      {
         strcpy(to_check->info,"code present after main, please fix before using debugger");
         return false;
      }
      to_check->program->position++;
      return true;
   }
   return false;
}

bool instruct_checks(debugger* to_check)
{
   /*end checks if we are at end of one iteration
   fin checks if we are at the end of all iters*/
   check_loop_end(to_check);
   while(check_loop_fin(to_check))
   {
      check_loop_end(to_check);
   }

   if(to_check->program->position>=\
      to_check->program->capacity-INDEX)
   {
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
                        end_pos=find_end_pos(to_check,START_BRACKET);
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
/*int find_end_pos(word_cont* to_check,int starting_brackets)
{
   int left_brackets,right_brackets;
   int end_pos;
   left_brackets=starting_brackets;
   right_brackets=0;
   end_pos=to_check->position;
   while(left_brackets!=right_brackets || left_brackets==0)
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
}*/

bool check_past_main(debugger* debug)
{
   if(find_end_pos(debug->program,0)<=\
      debug->program->capacity-INDEX)
   {
      return false;
   }
   return true;
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
   bool neg;
   neg=false;
   if(num< -MAXPOSLEN)
   {
      neg=true;
      num=fabs(num);
   }
   if(num>MAXPOSLEN)
   {
      while(num>MAXPOSLEN)
      {
         num=num/10;
      }
      if(neg)
      {
         num=num*-1;
      }
      sprintf(num_str,"= %d",(int)num);
      strcat(num_str,"... ");
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

/*will show 5 words behind and 5 ahead*/
void show_code_pos(debugger* to_check,char out_str[FULLARGSTRLEN])
{
   int from,to,pos,i;
   out_str[0]='\0';
   pos=to_check->program->position;
   from = (pos-SPREAD>0) ? pos-SPREAD : 0;
   to = (pos+SPREAD<to_check->program->capacity-INDEX) ? pos+SPREAD : \
                                       to_check->program->capacity-INDEX;
   for(i=from;i<=to;i++)
   {
      strcat(out_str,to_check->program->words[i]);
      if(i==pos)
      {
         strcat(out_str,"<- current pos");
      }
      strcat(out_str, "\n");
   }
}

/*will show last 5 coords */
void show_recent_coords(debugger* debug,char out_str[FULLARGSTRLEN])
{
   int i,start,max_ind;
   char temp[ONEARGLEN];
   out_str[0]='\0';
   max_ind=debug->output->size-1;
   start= (max_ind-SPREAD>0) ? max_ind-SPREAD : 0;
   for(i=start;i<=max_ind;i++)
   {
      sprintf(temp,"%3d y ",i);
      strcat(out_str,temp);
      str_num(debug->output->array[i]->end->y,temp);
      strcat(out_str,temp);
      strcat(out_str,"| x ");
      str_num(debug->output->array[i]->end->x,temp);
      strcat(out_str,temp);
      strcat(out_str,"\n");
   }
}

/*only want to use SDL in live version as
you cant really assert test it anyway
and it causes memory leaks*/
#ifdef LIVE_VERSION
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
      }
      return true;

      case to_mistake:
      if(!advance_to_mistake(to_check))
      {
         collate_instruct_messages(to_check,result_str);
      }
      else
      {
         strcpy(result_str,to_check->info);
      }
      return true;
      case show_vars:
      show_current_vars(to_check,result_str);
      return true;
      case show_pos:
      sprintf(result_str,"position = %d",to_check->program->position);
      return true;
      case show_code:
      show_code_pos(to_check,result_str);
      return true;
      case show_coords:
      show_recent_coords(to_check,result_str);
      return true;
      case suggestion:
      make_suggestion(to_check,result_str);
      return true;
      case visualize:
      draw_lines(to_check->output);
      return true;
      default:
      return false;
   }
}

#else

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
      }
      return true;

      case to_mistake:
      if(!advance_to_mistake(to_check))
      {
         collate_instruct_messages(to_check,result_str);
      }
      else
      {
         strcpy(result_str,to_check->info);
      }
      return true;
      case show_vars:
      show_current_vars(to_check,result_str);
      return true;
      case show_pos:
      sprintf(result_str,"position = %d",to_check->program->position);
      return true;
      case show_code:
      show_code_pos(to_check,result_str);
      return true;
      case show_coords:
      show_recent_coords(to_check,result_str);
      return true;
      case suggestion:
      make_suggestion(to_check,result_str);
      return true;
      default:
      return false;
   }
}

#endif



void collate_instruct_messages(debugger* to_check,\
                              char result_str[FULLARGSTRLEN])
{
   if(check_step_end(to_check))
   {
      strcpy(result_str,to_check->info);
   }
   else
   {
      /*bug around here*/
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
   if(to_check->program->position==to_check->program->capacity-INDEX)
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
   if(to_check->program->position>=to_check->program->capacity-INDEX)
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
   if(strcmp(action_str,"show code\n")==0)
   {
      return show_code;
   }
   if(strcmp(action_str,"show coords\n")==0)
   {
      return show_coords;
   }
   if(strcmp(action_str,"give suggestion\n")==0)
   {
      return suggestion;
   }
   if(strcmp(action_str,"visualize\n")==0)
   {
      return visualize;
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

/*levenshtein matrix is 1 indexed so have to add
1 to a lot of things. put some links in my extension
file on levenshtein distance that i used*/
int levenshtein(char* word_1,char* word_2)
{
   int word_1_len,word_2_len;
   int** matrix;
   int x,y,answer;
   word_1_len=strlen(word_1)+LEVIND;
   word_2_len=strlen(word_2)+LEVIND;
   matrix=(int**)safe_calloc_2d(word_1_len,word_2_len,\
                                 sizeof(int));
   for(y=0;y<word_1_len;y++)
   {
      matrix[y][0]=y;
   }
   for(x=0;x<word_2_len;x++)
   {
      matrix[0][x]=x;
   }
   for(y=LEVIND;y<word_1_len;y++)
   {
      for(x=LEVIND;x<word_2_len;x++)
      {
         if(word_1[y-LEVIND]==word_2[x-LEVIND])
         {
            matrix[y][x]=min_three(matrix[y-LEVIND][x]+LEVIND,
                                 matrix[y-LEVIND][x-LEVIND],
                                 matrix[y][x-LEVIND]+LEVIND);
         }
         else
         {
            matrix[y][x]=min_three(matrix[y-LEVIND][x]+LEVIND,
                                 matrix[y-LEVIND][x-LEVIND]+LEVIND,
                                 matrix[y][x-LEVIND]+LEVIND);
         }
      }
   }
   answer= matrix[word_1_len-LEVIND][word_2_len-LEVIND];
   free_2d(matrix,word_1_len);
   return answer;
}


int min_two(int a,int b)
{
   return (a<b) ? a:b;
}

int min_three(int a,int b, int c)
{
   return min_two(min_two(a,b),c);
}


void make_suggestion(debugger* to_check,\
            char result_str[FULLARGSTRLEN])
{
   char orig[MAXLEN];
   strcpy(orig,to_check->program->words\
               [to_check->program->position]);
   suggest_keyword(orig,result_str);
}

/*will not make suggestions for things like hfhjfhdkhdjfhfhffzzzzz
because there is no telling what they wanted there */
void suggest_keyword(char orig[MAXLEN],\
                  char suggestion[FULLARGSTRLEN])
{
   char keywords[NWORDS][MWORDLEN]={"TO","FROM","DO",\
                                    "FD","LT","RT",\
                                    "SET",":="};
   int lev_distance[NWORDS];
   char upper_orig[MAXLEN];
   int i,lowest_pos;
   if(strlen(orig)==0)
   {
      strcpy(suggestion,"no suggestion possible:blank string?");
      return;
   }
   str_to_upper(orig,upper_orig);
   for(i=0;i<NWORDS;i++)
   {
      lev_distance[i]=levenshtein(upper_orig,keywords[i]);
   }
   lowest_pos=find_low(lev_distance);
   if(lowest_pos==NOTFOUND)
   {
      strcpy(suggestion,"no suggestion possible");
   }
   else
   {
      strcpy(suggestion,keywords[lowest_pos]);
   }
}


void str_to_upper(char orig[MAXLEN],char upper[MAXLEN])
{
   unsigned int i;

   for(i=0;i<strlen(orig);i++)
   {
      upper[i]=toupper(orig[i]);
   }
   upper[i]='\0';
}


int find_low(int lev_distance[NWORDS])
{
   int i, lowest_val,lowest_pos;
   lowest_val=LARGESTART;
   lowest_pos=NOTFOUND;
   for(i=0;i<NWORDS;i++)
   {
      if(lev_distance[i]<lowest_val)
      {
         lowest_val=lev_distance[i];
         lowest_pos=i;
      }
   }
   return lowest_pos;
}
