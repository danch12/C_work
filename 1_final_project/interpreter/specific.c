#include "specific.h"
/*just the headers of functions that are defined in other places*/
bool get_rotation(word_cont* to_check,line_cont* line_arr);
bool move_forward(word_cont* to_check,line_cont* l_arr);
bool run_set(word_cont* to_check);
bool run_do(word_cont* to_check,line_cont* line_arr);
bool valid_mv(word_cont* to_check,char move[INSTRUCTLEN]);
bool valid_set(word_cont* to_check);
bool valid_do(word_cont* to_check);



line* finish_line(line* prev_line,coord* endpoint);
coord* init_coords(double x, double y);
bool rotate(double degrees,coord* to_rotate, coord* rotation_point);
bool get_num(word_cont* to_check,double* num);
bool run_instruction_list(word_cont* to_check,\
                        line_cont* line_arr);

void store_line(line_cont* l_arr, line* to_add);
direction direction_helper(word_cont* to_check);
bool valid_instructlist(word_cont* to_check);
bool finish_polish(word_cont* to_check,double* result);
bool get_var_pos(word_cont* to_check,int* var_p);
bool get_var(word_cont* to_check,double* num);
bool do_operation(word_cont* to_check);
op get_op(word_cont* to_check);
bool valid_var(word_cont* to_check);
bool valid_num(word_cont* to_check);

/*going to pretty much just try to emulate how cython do opcodes
with a big switch statement - cant emulate it perfectly because
we have to do the strcmp first but still speeds up the code
compared to just brute force running all the functions and seeing
which one returns true*/


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
   /*for debugger in the interpreting stage these will
      get eaten by the switch default*/
   if(strcmp(to_check->words[to_check->position],"}")==0)
   {
      return i_list;
   }

   return inv_opcode;
}

bool run_instruction(word_cont* to_check,line_cont* line_arr)
{
   opcode current_op;
   if(to_check->position>to_check->capacity)
   {
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
      if(run_set(to_check))
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



bool free_word_cont(word_cont* to_free)
{
   int i;
   if(to_free)
   {
      for(i=0;i<=to_free->capacity;i++)
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
      stack_free(to_free->stackptr);
      free(to_free);
   }

   return true;
}

word_cont* read_in_file(char* filename)
{
   FILE* fp;
   word_cont* n_cont;
   int num_lines,count,i;
   char buffer[MAXLEN];
   num_lines=0;
   n_cont=(word_cont*)safe_calloc(1,sizeof(word_cont));
   fp=get_file_words(filename,&num_lines);
   /*need to minus 1 for indexing*/
   n_cont->capacity=num_lines-1;
   n_cont->position=0;
   n_cont->words= (char**)safe_calloc(num_lines,sizeof(char*));
   n_cont->err_message[0]='\0';

   count=0;
   while(fscanf(fp,"%s",buffer)==1)
   {
      n_cont->words[count]=(char*)safe_calloc(strlen(buffer)+1,\
                                             sizeof(char));
      strcpy(n_cont->words[count],buffer);
      count++;
   }
   fclose(fp);

   n_cont->stackptr=stack_init(sizeof(double));
   for(i=0;i<NUMVARS;i++)
   {
      n_cont->var_array[i]=NULL;
   }
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
      default:
      strcpy(to_check->err_message,"invalid opcode used");
      return false;
   }
   return false;
}

bool polish_num(word_cont* to_check)
{
   double num;
   if(get_varnum(to_check,&num))
   {
      stack_push(to_check->stackptr,&num);
      return true;
   }
   return false;

}


bool move_forward(word_cont* to_check,line_cont* l_arr)
{
   double num;
   double n_y;
   coord* end_coord;
   line* finished_line;
   if(strcmp(to_check->words[to_check->position],"FD")==0)
   {
      to_check->position++;
      if(get_varnum(to_check,&num))
      {
         n_y=l_arr->pending_line->start->y +num;
         end_coord=init_coords(l_arr->pending_line->start->x,n_y);
         /*rotate returns true if sucessful even if it
         rotates by nothing*/
         if(rotate(l_arr->pending_line->rotation,\
            end_coord,l_arr->pending_line->start))
         {
            finished_line=finish_line(l_arr->pending_line,end_coord);
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
   if(get_varnum(to_check,&num))
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




bool run_polish(word_cont* to_check,double* num)
{
   if(to_check->position>to_check->capacity)
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
      if(run_polish(to_check,num))
      {
         return true;
      }
   }
   if(polish_num(to_check))
   {
      if(run_polish(to_check,num))
      {
         return true;
      }
   }
   return false;
}


bool run_set(word_cont* to_check)
{
   double to_set;
   int var_p;
   if(to_check->position>to_check->capacity)
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

bool get_varnum(word_cont* to_check,double* num)
{

   if(get_var(to_check,num))
   {
      return true;
   }
   if(get_num(to_check,num))
   {
      return true;
   }
   return false;
}


/*still can use variable after the do loop
bit like in c*/
bool run_do(word_cont* to_check,line_cont* line_arr)
{
   int var_pos;
   int beg_loop;
   double start,end,i;
   if(do_helper(to_check,&var_pos,&start,&end))
   {
      beg_loop=to_check->position;

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



bool do_helper(word_cont* to_check,int* var_pos,\
               double* start,double* end)
{
   int loop_start;
   if(to_check->position>to_check->capacity)
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
            if(get_varnum(to_check,start))
            {
               if(strcmp(to_check->words[to_check->position],"TO")==0)
               {
                  to_check->position++;
                  if(get_varnum(to_check,end))
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
