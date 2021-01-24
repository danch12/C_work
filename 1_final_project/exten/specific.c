#include "specific.h"



bool run_funcset(word_cont* to_check);
bool run_funcrun(word_cont* to_check,line_cont* line_arr,double** return_val);
bool run_flowstate(word_cont* to_check,line_cont* line_arr);
bool run_return(word_cont* to_check,line_cont* line_arr);
bool valid_funcvar(word_cont* to_check);
bool valid_mv(word_cont* to_check,char move[INSTRUCTLEN]);
bool valid_set(word_cont* to_check);
bool valid_do(word_cont* to_check);
bool valid_funcrun(word_cont* to_check);
bool valid_funcset(word_cont* to_check);
bool valid_flowstate(word_cont* to_check);
bool valid_return(word_cont* to_check);
bool valid_var(word_cont* to_check);
bool valid_num(word_cont* to_check);
bool finish_polish(word_cont* to_check,double* result);
bool get_var_pos(word_cont* to_check,int* var_p);
bool get_var(word_cont* to_check,double* num);
bool do_operation(word_cont* to_check);
op get_op(word_cont* to_check);
void store_line(line_cont* l_arr, line* to_add);
direction direction_helper(word_cont* to_check);
bool valid_instructlist(word_cont* to_check);


line* finish_line(line* prev_line,coord* endpoint);
coord* init_coords(double x, double y);
bool rotate(double degrees,coord* to_rotate, coord* rotation_point);
bool get_num(word_cont* to_check,double* num);
bool run_instruction_list(word_cont* to_check,\
                        line_cont* line_arr);
bool get_func_val(word_cont* to_check,line_cont* line_arr,\
                     double* num);

bool valid_init_arr(word_cont* to_check);
bool valid_append(word_cont* to_check);
bool valid_change(word_cont* to_check);
bool valid_delete_arr_val(word_cont* to_check);
bool valid_access_val(word_cont* to_check);
bool run_access_val(word_cont* to_check,line_cont* line_arr,double* num);
bool run_init_arr(word_cont* to_check);
bool run_append(word_cont* to_check,line_cont* line_arr);
bool run_delete_arr_val(word_cont* to_check,line_cont* line_arr);
bool run_change(word_cont* to_check,line_cont* line_arr);
bool run_len(word_cont* to_check,double* num);
bool valid_len(word_cont* to_check);
bool run_file_to_array(word_cont* to_check);
bool valid_file_to_array(word_cont* to_check);


void deep_free_assoc(assoc* a,assoc_type t)
{
   unsigned int i;
   if(a)
   {
      for(i=0;i<a->capacity;i++)
      {
         if(a->arr[i])
         {
            free(a->arr[i]->key);
            if(a->arr[i]->value)
            {
               if(t==function)
               {
                  free_word_cont(a->arr[i]->value);
               }
               if(t==array)
               {
                  free_arr(a->arr[i]->value);
               }
            }

            free(a->arr[i]);
         }
      }
      free(a->arr);
      free(a);
   }
}






bool free_word_cont(word_cont* to_free)
{
   int i;
   if(to_free)
   {
      for(i=0;i<to_free->capacity;i++)
      {

         free(to_free->words[i]);

      }
      for(i=0;i<NUMVARS;i++)
      {
         if(to_free->var_array[i])
         {
            free(to_free->var_array[i]);
         }
      }
      free(to_free->words);
      free(to_free->return_val);
      stack_free(to_free->stackptr);
      deep_free_assoc(to_free->arr_map,array);
      deep_free_assoc(to_free->func_map,function);
      free(to_free);
   }

   return true;
}


void read_words(char* filename,word_cont* n_cont)
{
   int num_lines,count;
   FILE* fp;
   char buffer[MAXLEN];
   num_lines=0;
   fp=get_file_words(filename,&num_lines);
   n_cont->capacity=num_lines;
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
}

word_cont* read_in_file(char* filename)
{
   word_cont* n_cont;
   int i;
   n_cont=(word_cont*)safe_calloc(1,sizeof(word_cont));
   n_cont->position=0;
   read_words(filename,n_cont);
   n_cont->err_message[0]='\0';
   n_cont->stackptr=stack_init(sizeof(double));
   for(i=0;i<NUMVARS;i++)
   {
      n_cont->var_array[i]=NULL;
   }
   n_cont->arr_map=assoc_init();
   n_cont->func_map=assoc_init();
   n_cont->n_args=UNUSED;
   n_cont->parent=NULL;
   n_cont->return_val=NULL;
   #ifdef INTERP_PRODUCTION
   Neill_SDL_Init(&n_cont->sw);
   Neill_SDL_SetDrawColour(&n_cont->sw,WHITE,WHITE,WHITE);
   Neill_SDL_UpdateScreen(&n_cont->sw);
   #endif
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



opcode get_opcode(word_cont* to_check)
{
   if(strcmp(to_check->words[to_check->position],"FD")==0)
   {
      return fd;
   }
   if((strcmp(to_check->words[to_check->position],"LT")==0)||\
      (strcmp(to_check->words[to_check->position],"RT")==0))
   {
      return rot;
   }
   if(strcmp(to_check->words[to_check->position],"DO")==0)
   {
      return do_loop;
   }
   if(strcmp(to_check->words[to_check->position],"SET")==0)
   {
      return set;
   }
   if(strcmp(to_check->words[to_check->position],"SETFUNC")==0)
   {
      return set_func;
   }
   if(strcmp(to_check->words[to_check->position],"RETURN")==0)
   {
      return return_val;
   }
   if(strcmp(to_check->words[to_check->position],"IF")==0)
   {
      return flowstate;
   }
   if(strcmp(to_check->words[to_check->position],"INITARR")==0)
   {
      return initarr;
   }
   if(strcmp(to_check->words[to_check->position],"APPEND")==0)
   {
      return append;
   }
   if(strcmp(to_check->words[to_check->position],"CHANGE")==0)
   {
      return change;
   }
   if(strcmp(to_check->words[to_check->position],"DEL")==0)
   {
      return del;
   }
   if(strcmp(to_check->words[to_check->position],"LOAD")==0)
   {
      return load;
   }
   if(valid_funcvar(to_check))
   {
      return run_func;
   }

   return inv_opcode;
}


/*taking ideas from python with the massive switch
statement*/
bool run_instruction(word_cont* to_check,line_cont* line_arr)
{
   double* placeholder;
   opcode current_op;
   placeholder=NULL;

   if(to_check->position>=to_check->capacity)
   {
      return false;
   }
   /*if we have already seen a return then we do not execute any more
   commands just check syntax*/
   if(to_check->return_val)
   {
      if(valid_instruct(to_check))
      {
         return true;
      }
      return false;
   }
   current_op=get_opcode(to_check);
   switch(current_op)
   {
      case fd:
      if(move_forward(to_check,line_arr))
      {
         return true;
      }
      break;
      case rot:
      if(get_rotation(to_check,line_arr))
      {
         return true;
      }
      break;
      case do_loop:
      if(run_do(to_check,line_arr))
      {
         return true;
      }
      break;
      case set:
      if(run_set(to_check,line_arr))
      {
         return true;
      }
      break;
      case set_func:
      if(run_funcset(to_check))
      {
         return true;
      }
      break;
      case run_func:
      /*not using return value here */
      if(run_funcrun(to_check,line_arr,&placeholder))
      {
         if(placeholder)
         {
            free(placeholder);
         }
         return true;
      }
      break;
      case return_val:
      if(run_return(to_check,line_arr))
      {
         return true;
      }
      break;
      case flowstate:
      if(run_flowstate(to_check,line_arr))
      {
         return true;
      }
      break;
      case initarr:
      if(run_init_arr(to_check))
      {
         return true;
      }
      break;
      case append:
      if(run_append(to_check,line_arr))
      {
         return true;
      }
      break;
      case change:
      if(run_change(to_check,line_arr))
      {
         return true;
      }
      break;
      case del:
      if(run_delete_arr_val(to_check,line_arr))
      {
         return true;
      }
      break;
      case load:
      if(run_file_to_array(to_check))
      {
         return true;
      }
      break;
      default:
      strcpy(to_check->err_message,"invalid opcode used");
      return false;
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
   if(valid_funcrun(to_check))
   {
      return true;
   }
   if(valid_access_val(to_check))
   {
      return true;
   }
   if(valid_len(to_check))
   {
      return true;
   }
   return false;
}



bool valid_instruct(word_cont* to_check)
{
   opcode current_op;
   int init_pos;
   init_pos=to_check->position;
   current_op=get_opcode(to_check);
   switch(current_op)
   {
      case fd:
      if(valid_mv(to_check,"FD"))
      {
         return true;
      }
      break;
      case rot:
      if(valid_mv(to_check,"LT"))
      {
         return true;
      }
      to_check->position=init_pos;
      if(valid_mv(to_check,"RT"))
      {
         return true;
      }
      break;
      case do_loop:
      if(valid_do(to_check))
      {
         return true;
      }
      break;
      case set:
      if(valid_set(to_check))
      {
         return true;
      }
      break;
      case set_func:
      if(valid_funcset(to_check))
      {
         return true;
      }
      break;
      case run_func:

      if(valid_funcrun(to_check))
      {
         return true;
      }
      break;
      case return_val:
      if(valid_return(to_check))
      {
         return true;
      }
      break;
      case flowstate:
      if(valid_flowstate(to_check))
      {
         return true;
      }
      break;
      case initarr:
      if(valid_init_arr(to_check))
      {
         return true;
      }
      break;
      case append:
      if(valid_append(to_check))
      {
         return true;
      }
      break;
      case change:
      if(valid_change(to_check))
      {
         return true;
      }
      break;
      case del:
      if(valid_delete_arr_val(to_check))
      {
         return true;
      }
      break;
      case load:
      if(valid_file_to_array(to_check))
      {
         return true;
      }
      break;
      default:
      strcpy(to_check->err_message,"invalid opcode used");
      return false;
   }
   return false;
}



bool get_varnum(word_cont* to_check,double* num,line_cont* line_arr)
{
   /*will return false before increaing position*/
   if(get_var(to_check,num))
   {
      return true;
   }
   if(get_num(to_check,num))
   {
      return true;
   }

   if(get_func_val(to_check,line_arr,num))
   {

      return true;
   }
   if(run_access_val(to_check,line_arr,num))
   {
      return true;
   }
   if(run_len(to_check,num))
   {
      return true;
   }
   return false;
}

#ifdef INTERP_PRODUCTION
void simple_draw(word_cont* to_check,line* to_draw)
{
   while(to_check->parent)
   {
      to_check=to_check->parent;
   }
   SDL_RenderDrawLine(to_check->sw.renderer, \
                     (int)to_draw->start->x+MIDWIDTH,\
                     (int)to_draw->start->y+MIDHEIGHT, \
                      (int)to_draw->end->x+MIDWIDTH,\
                   (int)to_draw->end->y+MIDHEIGHT);
   Neill_SDL_Events(&to_check->sw);
   Neill_SDL_UpdateScreen(&to_check->sw);

}
#endif

bool move_forward(word_cont* to_check,line_cont* l_arr)
{
   double num;
   double n_y;
   coord* end_coord;
   line* finished_line;
   if(strcmp(to_check->words[to_check->position],"FD")==0)
   {
      to_check->position++;
      if(get_varnum(to_check,&num,l_arr))
      {
         n_y=l_arr->pending_line->start->y +num;
         end_coord=init_coords(l_arr->pending_line->start->x,n_y);
         /*rotate returns true if sucessful even if it
         rotates by nothing*/
         if(rotate(l_arr->pending_line->rotation,\
            end_coord,l_arr->pending_line->start))
         {
            finished_line=finish_line(l_arr->pending_line,end_coord);
            #ifdef INTERP_PRODUCTION
            simple_draw(to_check,finished_line);
            #endif
            store_line(l_arr,finished_line);

            /*reusing pending_line over and over by just
             changing its start point to the last lines
             end point*/
            l_arr->pending_line->start->x =finished_line->end->x;
            l_arr->pending_line->start->y =finished_line->end->y;
            return true;
         }
      }
   }
   return false;
}

double wrap_around(double i, double i_max)
{

   i = fmod(i,i_max);
   if(i<0)
   {
      return  i+i_max;
   }
   return i;
}

bool get_rotation(word_cont* to_check,line_cont* line_arr)
{
   double num;
   double temp;
   direction dir;
   dir=direction_helper(to_check);
   if(dir==invalid)
   {
      return false;
   }
   if(get_varnum(to_check,&num,line_arr))
   {
      if(dir==left)
      {
         temp=num+line_arr->pending_line->rotation;
      }
      else
      {
         temp= (DEGREES-num)+line_arr->pending_line->rotation;
      }
      temp=wrap_around(temp,DEGREES);

      line_arr->pending_line->rotation=temp;
      return true;
   }
   return false;
}



bool run_do(word_cont* to_check,line_cont* line_arr)
{
   int var_pos;
   int beg_loop;
   double start,end,i;
   if(do_helper(to_check,&var_pos,&start,&end,line_arr))
   {
      beg_loop=to_check->position;
      for(i=start;i<=end;i++)
      {
         if(!to_check->var_array[var_pos])
         {
            to_check->var_array[var_pos]=(double*)safe_calloc(1,sizeof(double));
         }
         *to_check->var_array[var_pos]=i;
         to_check->position=beg_loop;
         if(!run_instruction_list(to_check,line_arr))
         {
            return false;
         }
         /*in case we change the value of the iteratable
         in our loop*/
         i=*to_check->var_array[var_pos];
      }
      return true;
   }
   return false;
}

/*grabs info and checks validity*/
bool do_helper(word_cont* to_check,int* var_pos,\
               double* start,double* end,line_cont* line_arr)
{
   int loop_start;
   if(to_check->position>=to_check->capacity)
   {
      return false;
   }
   if(strcmp(to_check->words[to_check->position],"DO")==0)
   {
      to_check->position++;
      if(get_var_pos(to_check,var_pos))
      {
         if(strcmp(to_check->words[to_check->position],"FROM")==0)
         {
            to_check->position++;
            if(get_varnum(to_check,start,line_arr))
            {
               if(strcmp(to_check->words[to_check->position],"TO")==0)
               {
                  to_check->position++;
                  if(get_varnum(to_check,end,line_arr))
                  {
                     if(strcmp(to_check->words[to_check->position],"{")==0)
                     {
                        to_check->position++;
                        loop_start=to_check->position;
                        if(valid_instructlist(to_check))
                        {
                           to_check->position=loop_start;
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


/*add variables option here*/
bool polish_num(word_cont* to_check,line_cont* line_arr)
{
   double num;
   if(get_varnum(to_check,&num,line_arr))
   {
      stack_push(to_check->stackptr,&num);
      return true;
   }
   return false;

}

/*num is going to be passed in by set function*/
bool run_polish(word_cont* to_check,double* num,line_cont* line_arr)
{
   if(to_check->position>=to_check->capacity)
   {
      return false;
   }
   if(strcmp(to_check->words[to_check->position],";")==0)
   {
      /*need to get num and check only one num here*/
      if(finish_polish(to_check,num))
      {
         to_check->position++;
         return true;
      }
   }
   if(do_operation(to_check))
   {
      if(run_polish(to_check,num,line_arr))
      {
         return true;
      }
   }
   if(polish_num(to_check,line_arr))
   {
      if(run_polish(to_check,num,line_arr))
      {
         return true;
      }
   }
   return false;
}


bool run_set(word_cont* to_check,line_cont* line_arr)
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

            if(run_polish(to_check,&to_set,line_arr))
            {
               if(!to_check->var_array[var_p])
               {
                  to_check->var_array[var_p]=(double*)safe_calloc(1,\
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
