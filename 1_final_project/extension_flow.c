#include "extension_flow.h"


bool run_flowstate(word_cont* to_check,line_cont* line_arr)
{

   bool result;

   if(do_comparison(to_check,&result,line_arr))
   {
      if(result==true)
      {
         if(run_true(to_check,line_arr))
         {
            return true;
         }
      }
      else
      {
         if(run_false(to_check,line_arr))
         {
            return true;
         }
      }
   }
   return false;
}

bool run_true(word_cont* to_check,line_cont* line_arr)
{
   if(run_main(to_check,line_arr))
   {
      if(strcmp(to_check->words[to_check->position],"ELSE")!=0)
      {
         return true;
      }
      if(valid_else(to_check))
      {
         return true;
      }
   }
   return false;
}

bool run_false(word_cont* to_check,line_cont* line_arr)
{
   if(valid_main(to_check))
   {
      if(strcmp(to_check->words[to_check->position],"ELSE")!=0)
      {
         return true;
      }
      if(run_else(to_check,line_arr))
      {
         return true;
      }
   }
   return false;
}

bool run_else(word_cont* to_check,line_cont* line_arr)
{
   if(strcmp(to_check->words[to_check->position],"ELSE")==0)
   {
      to_check->position++;
      if(strcmp(to_check->words[to_check->position],"{")==0)
      {
         to_check->position++;

         if(run_instruction_list(to_check,line_arr))
         {
            return true;
         }
      }
   }
   return false;
}

bool do_comparison(word_cont* to_check,bool* result,line_cont* line_arr)
{
   double vn_1, vn_2;
   comparator cmp;
   if(strcmp(to_check->words[to_check->position],"IF")==0)
   {
      to_check->position++;
      if(get_varnum(to_check,&vn_1,line_arr))
      {
         cmp=get_comparator(to_check);
         if(cmp!=inv_op)
         {
            to_check->position++;
            if(get_varnum(to_check,&vn_2,line_arr))
            {

               *result = do_comparison_helper(vn_1,vn_2,cmp);

               return true;
            }
         }
      }
   }
   return false;
}

bool do_comparison_helper(double vn_1,double vn_2,comparator cmp)
{
   switch(cmp)
   {
      case equal:
      return comp_doubles(vn_1,vn_2);
      case less:
      return vn_1<vn_2;
      case greater:
      return vn_1>vn_2;
      default:
      fprintf(stderr,"error- comparator not recognised\n");
      exit(EXIT_FAILURE);
   }
}
/*if its meant to be an else but theres a spelling
mistake or something the error will be picked up by other
functions not recognising it this is also why we
do not increase the position- we want to recheck this
word*/
bool valid_flowstate(word_cont* to_check)
{
   if(strcmp(to_check->words[to_check->position],"IF")==0)
   {
      to_check->position++;
      if(valid_expression(to_check))
      {
         if(strcmp(to_check->words[to_check->position],"{")==0)
         {
            to_check->position++;
            if(valid_instructlist(to_check))
            {
               if(strcmp(to_check->words[to_check->position],"ELSE")!=0)
               {
                  return true;
               }
               else
               {
                  if(valid_else(to_check))
                  {
                     return true;
                  }
               }
            }
         }
      }
   }
   return false;
}

bool valid_else(word_cont* to_check)
{

   if(strcmp(to_check->words[to_check->position],"ELSE")==0)
   {
      to_check->position++;
      if(strcmp(to_check->words[to_check->position],"{")==0)
      {
         to_check->position++;
         if(valid_instructlist(to_check))
         {
            return true;
         }
      }
   }
   return false;
}

bool valid_expression(word_cont* to_check)
{
   if(valid_varnum(to_check))
   {
      if(get_comparator(to_check)!=inv_op)
      {
         to_check->position++;
         if(valid_varnum(to_check))
         {
            return true;
         }
      }
   }
   return false;
}



comparator get_comparator(word_cont* to_check)
{
   if(strcmp(to_check->words[to_check->position],"==")==0)
   {
      return equal;
   }
   if(strcmp(to_check->words[to_check->position],"<")==0)
   {
      return less;
   }
   if(strcmp(to_check->words[to_check->position],">")==0)
   {
      return greater;
   }
   return inv_op;
}




bool comp_doubles(double d_1, double d_2)
{
   double temp;
   temp= d_1 - d_2;
   if(temp<EPSILON && temp > -EPSILON)
   {
      return true;
   }
   return false;
}
