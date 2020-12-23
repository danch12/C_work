#include "interpreter_funcs.h"



/*https://www.khanacademy.org/computing/pixar/sets/rotation/v/sets-8
found formulas for rotating a point */
bool rotate(double degrees,coord* to_rotate, coord* rotation_point)
{
   double radians;
   double n_y , n_x;

   if(to_rotate&&rotation_point)
   {

      radians = degrees * (PI/DEGTORAD);

      /*go to origin then rotate*/
      n_y= to_rotate->y - rotation_point->y;
      n_x= to_rotate->x -rotation_point->x;

      to_rotate->y = ((n_x *sin(radians)) + \
                     (n_y * cos(radians)))+ rotation_point->y;

      to_rotate->x = (n_x * cos(radians) - \
                     (n_y *sin(radians)))+rotation_point->x;
      return true;
   }
   return false;
}

coord* init_coords(double x, double y)
{
   coord* n_coord;
   n_coord=safe_calloc(1,sizeof(coord));
   n_coord->x=x;
   n_coord->y=y;
   return n_coord;
}

line* finish_line(line* prev_line,coord* endpoint)
{
   line* n_line;
   n_line= safe_calloc(1,sizeof(line));
   n_line->start=init_coords(prev_line->start->x,prev_line->start->y);

   n_line->rotation=prev_line->rotation;
   n_line->end=endpoint;
   return n_line;
}

line* init_origin(void)
{
   line* origin_line;
   origin_line= safe_calloc(1,sizeof(line));
   origin_line->start=init_coords(ORIGIN,ORIGIN);
   origin_line->rotation=0;
   origin_line->end=init_coords(ORIGIN,ORIGIN);
   return origin_line;
}

bool free_line(line* to_free)
{
   if(to_free)
   {
      free(to_free->start);
      free(to_free->end);

      free(to_free);

   }
   return true;
}



bool get_num(word_cont* to_check,double* num)
{

   char* ptr;
   if(to_check)
   {
      if(valid_number(to_check))
      {
         *num=strtod(to_check->words[to_check->position],&ptr);
         to_check->position++;
         return true;
      }
   }
   return false;
}


/*pretty much the same as parser except we
dont increase the position at the end*/
bool valid_number(word_cont* to_check)
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
   return true;
}


bool run_main(word_cont* to_check,line_cont* line_arr,\
            line* pending_line)
{
   if(to_check->position>=to_check->capacity)
   {
      return false;
   }
   if(strcmp(to_check->words[to_check->position],"{")==0)
   {
      to_check->position++;

      if(run_instruction_list(to_check,line_arr,pending_line))
      {
         return true;
      }
   }
   return false;
}



bool run_instruction_list(word_cont* to_check,line_cont* line_arr,\
                           line* pending_line)
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
      if(run_instruction(to_check,line_arr,pending_line))
      {
         if(run_instruction_list(to_check,line_arr,pending_line))
         {
            return true;
         }
      }
   }
   return false;
}




/*potentially add pending line in to word_cont -
as word cont deals with intermediate stages of instructions
which is basically what pending line is*/
bool run_instruction(word_cont* to_check,line_cont* line_arr,\
                     line* pending_line)
{
   int init_pos;
   init_pos=to_check->position;
   if(init_pos>=to_check->capacity)
   {
      return false;
   }
   if(get_rotation(to_check,pending_line))
   {
      return true;
   }
   to_check->position=init_pos;
   if(move_forward(to_check,pending_line,line_arr))
   {
      return true;
   }
   to_check->position=init_pos;
   if(run_set(to_check))
   {
      return true;
   }
   to_check->position=init_pos;
   if(run_do(to_check,line_arr,pending_line))
   {
      return true;
   }
   return false;
}



bool get_rotation(word_cont* to_check,line* pending_line)
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
         temp=num+pending_line->rotation;
      }
      else
      {
         temp= (DEGREES-num)+pending_line->rotation;
      }
      temp=fabs(fmod(temp,DEGREES));
      pending_line->rotation=temp;
      return true;
   }
   return false;
}


direction direction_helper(word_cont* to_check)
{
   direction dir;
   if(strcmp(to_check->words[to_check->position],"LT")==0)
   {
      to_check->position++;
      dir=left;
      return dir;
   }
   if(strcmp(to_check->words[to_check->position],"RT")==0)
   {
      to_check->position++;
      dir=right;
      return dir;
   }
   else
   {
      dir=invalid;
      return dir;
   }
}


/*to move forward we add a endpoint to the pending
line then we add that line to a collection then we create
a new line- may actually be easier to add a copy of the
line to the array so we dont have to deal with changing
addresses or anything
this is pretty much the meat and potatoes of the
whole thing since lines only appear when you move forward*/
bool move_forward(word_cont* to_check,line* pending_line,\
                  line_cont* l_arr)
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
         n_y=pending_line->start->y +num;
         end_coord=init_coords(pending_line->start->x,n_y);
         /*rotate returns true if sucessful even if it
         rotates by nothing*/
         if(rotate(pending_line->rotation,\
            end_coord,pending_line->start))
         {
            finished_line=finish_line(pending_line,end_coord);
            store_line(l_arr,finished_line);
            /*reusing pending_line over and over by just
             changing its start point to the last lines
             end point*/
            pending_line->start->x =finished_line->end->x;
            pending_line->start->y =finished_line->end->y;
            return true;
         }
      }
   }
   return false;
}


void store_line(line_cont* l_arr, line* to_add)
{
   if(to_add)
   {
      l_arr->array[l_arr->size]=to_add;
      l_arr->size+=1;
      if(l_arr->size==l_arr->capacity)
      {
         l_arr->array=(line**)realloc(l_arr->array, \
            sizeof(line*)*l_arr->capacity*SCALEFACTOR);
         if(l_arr->array==NULL)
         {
            fprintf(stderr,"Ran out of space\n");
      	 	exit(EXIT_FAILURE);
         }
         l_arr->capacity=l_arr->capacity*SCALEFACTOR;
      }
   }
}


line_cont* init_line_cont(void)
{
   line_cont* l_arr;
   l_arr=(line_cont* )safe_calloc(1,sizeof(line_cont));
   l_arr->array=(line**)safe_calloc(INITSIZE,sizeof(line*));
   l_arr->capacity=INITSIZE;
   l_arr->size=0;
   return l_arr;
}

bool free_line_cont(line_cont* to_free)
{
   int i;
   if(to_free)
   {
      for(i=0;i<to_free->size;i++)
      {
         free_line(to_free->array[i]);
      }
      free(to_free->array);
      free(to_free);
   }
   return true;

}


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

   n_cont->stackptr=stack_init();
   for(i=0;i<NUMVARS;i++)
   {
      n_cont->var_array[i]=NULL;
   }
   n_cont->err_message[0]='\0';
   return n_cont;
}*/

/*bool free_word_cont(word_cont* to_free)
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
      stack_free(to_free->stackptr);
      free(to_free);
   }

   return true;
}*/



/*get op v similar to parser except we dont increase position
and we return a op instead of bool*/
op get_op(word_cont* to_check)
{
   if(to_check->position>=to_check->capacity)
   {
      return invalid_op;
   }
   if(strcmp(to_check->words[to_check->position],"+")==0)
   {
      return plus;
   }
   if(strcmp(to_check->words[to_check->position],"-")==0)
   {
      return minus;
   }
   if(strcmp(to_check->words[to_check->position],"*")==0)
   {
      return mult;
   }
   if(strcmp(to_check->words[to_check->position],"/")==0)
   {
      return divide;
   }
   return invalid_op;
}
/* we need to be careful that we dont pop
a number off and leave it off - im not sure in
what cases this would happen but its easy to safe guard
against*/
bool do_operation(word_cont* to_check)
{
   op operator;
   double result, num1, num2;
   operator=get_op(to_check);

   if(operator==invalid_op)
   {
      return false;
   }
   if(to_check->stackptr->size<2)
   {
      return false;
   }
   if(!stack_pop(to_check->stackptr, &num2) || \
      !stack_pop(to_check->stackptr, &num1))
   {
      return false;
   }
   switch(operator)
   {
      case plus:
      result= num1 + num2;
      break;

      case minus:
      result= num1 - num2;
      break;

      case mult:
      result= num1 * num2;
      break;

      case divide:
      result =  num1 / num2;
      break;

      default:
      fprintf(stderr,"unexpected operator\n");
      exit(EXIT_FAILURE);
   }
   stack_push(to_check->stackptr,result);
   to_check->position++;
   return true;
}

/*add variables option here*/
bool polish_num(word_cont* to_check)
{
   double num;
   if(get_varnum(to_check,&num))
   {
      stack_push(to_check->stackptr,num);
      return true;
   }
   return false;

}

/*need to get remaining num and check only one num there*/
bool finish_polish(word_cont* to_check,double* result)
{
   /*if no number something wrong*/
   if(!stack_pop(to_check->stackptr,result))
   {
      strcpy(to_check->err_message,"no numbers on the stack at end of expr");
      return false;
   }
   /*if numbers left over then something is wrong*/
   if(stack_peek(to_check->stackptr,result))
   {
      strcpy(to_check->err_message,"more than one number left on stack at end of expr");
      return false;
   }
   return true;

}

/*num is going to be passed in by set function*/
bool run_polish(word_cont* to_check,double* num)
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

            if(run_polish(to_check,&to_set))
            {
               if(!to_check->var_array[var_p])
               {
                  to_check->var_array[var_p]=safe_calloc(1,\
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


bool valid_variable(word_cont* to_check)
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
         return true;
      }
   }
   return false;
}

/*pass position back through var_p
*we see vars as A- Z but really they could be 0-25*/
bool get_var_pos(word_cont* to_check,int* var_p)
{
   if(valid_variable(to_check))
   {
      *var_p= to_check->words[to_check->position][0]-LETTER_TO_NUM;
      to_check->position++;
      return true;
   }
   return false;
}



bool get_var(word_cont* to_check,double* num)
{
   int pos;
   if(get_var_pos(to_check,&pos))
   {
      /*if we have initialised something*/
      if(to_check->var_array[pos])
      {
         *num= *to_check->var_array[pos];
         /*get var pos increases position*/
         return true;
      }
      else
      {
         /*here we have checked its a valid position
         but also the space is empty- this indicates
         var not been set*/
         strcpy(to_check->err_message,"potentially haven't set variable before calling it");
      }

   }
   return false;
}

bool get_varnum(word_cont* to_check,double* num)
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
   return false;
}


bool run_do(word_cont* to_check,line_cont* line_arr,\
                           line* pending_line)
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
            to_check->var_array[var_pos]=safe_calloc(1,sizeof(double));
         }
         *to_check->var_array[var_pos]=i;
         to_check->position=beg_loop;
         if(!run_instruction_list(to_check,line_arr,pending_line))
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
