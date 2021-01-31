#include "specific.h"
#include "../extension_funcs.h"
#include "../interpreter_funcs.h"
#include "../extension_flow.h"
#include "../extension_arrays.h"
#include "../parser_funcs.h"

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
   n_cont->words= (char**)safe_calloc(num_lines,\
                                    sizeof(char*));
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
   if(safe_samestr(to_check,"FD"))
   {
      return fd;
   }
   if((safe_samestr(to_check,"LT"))||\
      (safe_samestr(to_check,"RT")))
   {
      return rot;
   }
   if(safe_samestr(to_check,"DO"))
   {
      return do_loop;
   }
   if(safe_samestr(to_check,"SET"))
   {
      return set;
   }
   if(safe_samestr(to_check,"SETFUNC"))
   {
      return set_func;
   }
   if(safe_samestr(to_check,"RETURN"))
   {
      return return_val;
   }
   if(safe_samestr(to_check,"IF"))
   {
      return flowstate;
   }
   if(safe_samestr(to_check,"INITARR"))
   {
      return initarr;
   }
   if(safe_samestr(to_check,"APPEND"))
   {
      return append;
   }
   if(safe_samestr(to_check,"CHANGE"))
   {
      return change;
   }
   if(safe_samestr(to_check,"DEL"))
   {
      return del;
   }
   if(safe_samestr(to_check,"LOAD"))
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
   /*if we have already seen a return then we do
    not execute any more commands just check syntax*/
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



bool get_varnum(word_cont* to_check,double* num,\
               line_cont* line_arr)
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
   Neill_SDL_UpdateScreen(&to_check->sw);
   Neill_SDL_Events(&to_check->sw);

}
#endif

bool move_forward(word_cont* to_check,line_cont* l_arr)
{
   double num;
   double n_y;
   coord* end_coord;
   line* finished_line;
   if(safe_samestr(to_check,"FD"))
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
            finished_line=finish_line(l_arr->pending_line,\
                                       end_coord);
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
   double new_rot;
   direction dir;
   dir=direction_helper(to_check);
   if(dir==invalid)
   {
      return false;
   }
   if(get_varnum(to_check,&num,line_arr))
   {
      if(dir==right)
      {
         new_rot=num+line_arr->pending_line->rotation;
      }
      else
      {
         new_rot= (DEGREES-num)+\
                  line_arr->pending_line->rotation;
      }
      new_rot=wrap_around(new_rot,DEGREES);

      line_arr->pending_line->rotation=new_rot;
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
      /*if we dont run loop check syntax and go to end*/
      if(start>end)
      {
         if(valid_instructlist(to_check))
         {
            return true;
         }
         return false;
      }
      for(i=start;i<=end;i++)
      {
         if(!to_check->var_array[var_pos])
         {
            to_check->var_array[var_pos]=(double*)safe_calloc(1,\
                                                   sizeof(double));
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
   if(safe_samestr(to_check,"DO"))
   {
      to_check->position++;
      if(get_var_pos(to_check,var_pos))
      {
         if(safe_samestr(to_check,"FROM"))
         {
            to_check->position++;
            if(get_varnum(to_check,start,line_arr))
            {
               if(safe_samestr(to_check,"TO"))
               {
                  to_check->position++;
                  if(get_varnum(to_check,end,line_arr))
                  {
                     if(safe_samestr(to_check,"{"))
                     {
                        to_check->position++;
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
bool run_polish(word_cont* to_check,double* num,\
               line_cont* line_arr)
{
   if(safe_samestr(to_check,";"))
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

   if(safe_samestr(to_check,"SET"))
   {
      to_check->position++;
      if(get_var_pos(to_check,&var_p))
      {
         if(safe_samestr(to_check,":="))
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
