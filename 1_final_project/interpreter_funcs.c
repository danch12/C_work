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
   n_coord=(coord*)safe_calloc(1,sizeof(coord));
   n_coord->x=x;
   n_coord->y=y;
   return n_coord;
}

line* finish_line(line* prev_line,coord* endpoint)
{
   line* n_line;
   n_line= (line*)safe_calloc(1,sizeof(line));
   n_line->start=init_coords(prev_line->start->x,\
                             prev_line->start->y);

   n_line->rotation=prev_line->rotation;
   n_line->end=endpoint;
   return n_line;
}

line* init_origin(void)
{
   line* origin_line;
   origin_line= (line*)safe_calloc(1,sizeof(line));
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
   int len;
   if(to_check->position>=to_check->capacity)
   {
      return false;
   }
   len=strlen(to_check->words[to_check->position]);
   switch(len)
   {
      /*blank string not number*/
      case BLANK:
      return false;
      /*if single number then has to actually be a number*/
      case 1:
      if(!isdigit(to_check->words[to_check->position][0]))
      {
         return false;
      }
      return true;
      default:
      if(valid_larger_than_one(to_check,len))
      {
         return true;
      }
   }
   return false;
}



bool run_main(word_cont* to_check,line_cont* line_arr)
{
   if(to_check->position>=to_check->capacity)
   {
      return false;
   }
   if(strcmp(to_check->words[to_check->position],"{")==0)
   {
      to_check->position++;

      if(run_instruction_list(to_check,line_arr))
      {
         return true;
      }
   }
   return false;
}



bool run_instruction_list(word_cont* to_check,\
                        line_cont* line_arr)
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
      if(run_instruction(to_check,line_arr))
      {
         if(run_instruction_list(to_check,line_arr))
         {
            return true;
         }
      }
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
   l_arr->pending_line=init_origin();
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
      free_line(to_free->pending_line);
      free(to_free);
   }
   return true;

}


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
   double result;
   void* num1, *num2;
   num1=NULL;
   num2=NULL;
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
      result= *(double*)num1 + *(double*)num2;
      break;

      case minus:
      result= *(double*)num1 - *(double*)num2;
      break;

      case mult:
      result= *(double*)num1 * *(double*)num2;
      break;

      case divide:
      if(fabs(*(double*)num2)<EPSILON)
      {
         strcpy(to_check->err_message,"trying to divde by 0");
         free(num1);
         free(num2);
         return false;
      }
      result =  *(double*)num1 / *(double*)num2;
      break;

      default:
      fprintf(stderr,"unexpected operator not caught by first check\n");
      exit(EXIT_FAILURE);
   }
   free(num1);
   free(num2);
   stack_push(to_check->stackptr,&result);
   to_check->position++;
   return true;
}



/*need to get remaining num and check only one num there*/
bool finish_polish(word_cont* to_check,double* result)
{
   /*if no number something wrong*/
   void* ptr_result;
   void* leftover;
   ptr_result=NULL;
   leftover=NULL;
   if(!stack_pop(to_check->stackptr,&ptr_result))
   {

      strcpy(to_check->err_message,"no numbers on the stack at end of expr");
      return false;
   }
   /*if numbers left over then something is wrong*/
   if(stack_peek(to_check->stackptr,&leftover))
   {
      if(ptr_result)
      {
         free(ptr_result);
      }
      strcpy(to_check->err_message,"more than one number left on stack at end of expr");
      return false;
   }
   *result = *(double*)ptr_result;
   free(ptr_result);
   return true;

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

/*uninitialised variables will throw a error*/
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
