#include "extension_flow.h"
#define MAXTESTCAP 100
#define MAXTESTLEN 50
#define EPSILON 0.00001

typedef enum comparator {equal,greater,less,inv_op} comparator;

comparator get_comparator(word_cont* to_check);
bool valid_expression(word_cont* to_check);
bool valid_flowstate(word_cont* to_check);
bool valid_if(word_cont* to_check);
bool valid_ifelse(word_cont* to_check);

bool compare_doubles(double d_1, double d_2);
word_cont* init_word_cont(void);



int main(void)
{
   word_cont* test_cont;


   test_cont=init_word_cont();
   strcpy(test_cont->words[0],"==");
   assert(get_comparator(test_cont)==equal);
   strcpy(test_cont->words[0],"<");
   assert(get_comparator(test_cont)==less);
   strcpy(test_cont->words[0],">");
   assert(get_comparator(test_cont)==greater);
   strcpy(test_cont->words[0],"=");
   assert(get_comparator(test_cont)==inv_op);

   free_word_cont(test_cont);
   test_cont=init_word_cont();
   strcpy(test_cont->words[0],"10");
   strcpy(test_cont->words[1],"==");
   strcpy(test_cont->words[2],"A");
   assert(valid_expression(test_cont));

   test_cont->position=0;
   strcpy(test_cont->words[0],"10");
   strcpy(test_cont->words[1],"==");
   strcpy(test_cont->words[2],"10");
   assert(valid_expression(test_cont));

   test_cont->position=0;
   strcpy(test_cont->words[0],"10");
   strcpy(test_cont->words[1],">");
   strcpy(test_cont->words[2],"10");
   assert(valid_expression(test_cont));

   test_cont->position=0;
   strcpy(test_cont->words[0],"10");
   strcpy(test_cont->words[1],"<");
   strcpy(test_cont->words[2],"10");
   assert(valid_expression(test_cont));

   test_cont->position=0;
   strcpy(test_cont->words[0],"B");
   strcpy(test_cont->words[1],"<");
   strcpy(test_cont->words[2],"A");
   assert(valid_expression(test_cont));

   test_cont->position=0;
   strcpy(test_cont->words[0],"<");
   strcpy(test_cont->words[1],"<");
   strcpy(test_cont->words[2],"A");
   assert(!valid_expression(test_cont));

   test_cont->position=0;
   strcpy(test_cont->words[0],"<");
   strcpy(test_cont->words[1],"10");
   strcpy(test_cont->words[2],"A");
   assert(!valid_expression(test_cont));

   test_cont->position=0;
   strcpy(test_cont->words[0],"10");
   strcpy(test_cont->words[1],"");
   strcpy(test_cont->words[2],"A");
   assert(!valid_expression(test_cont));

   test_cont->position=0;
   strcpy(test_cont->words[0],"10");
   strcpy(test_cont->words[1],"=");
   strcpy(test_cont->words[2],"A");
   assert(!valid_expression(test_cont));


   test_cont->position=0;
   strcpy(test_cont->words[0],"IF");
   strcpy(test_cont->words[1],"10");
   strcpy(test_cont->words[2],"==");
   strcpy(test_cont->words[3],"A");
   strcpy(test_cont->words[4],"{");
   strcpy(test_cont->words[5],"FD");
   strcpy(test_cont->words[6],"10");
   strcpy(test_cont->words[7],"}");

   assert(valid_flowstate(test_cont));

   test_cont->position=0;
   strcpy(test_cont->words[0],"IF");
   strcpy(test_cont->words[1],"10");
   strcpy(test_cont->words[2],"==");
   strcpy(test_cont->words[3],"A");
   strcpy(test_cont->words[4],"{");
   strcpy(test_cont->words[5],"IF");
   strcpy(test_cont->words[6],"7");
   strcpy(test_cont->words[7],"==");
   strcpy(test_cont->words[8],"8");
   strcpy(test_cont->words[9],"{");
   strcpy(test_cont->words[10],"LT");
   strcpy(test_cont->words[11],"30");
   strcpy(test_cont->words[12],"}");
   strcpy(test_cont->words[13],"}");


   assert(valid_flowstate(test_cont));

   return 0;
}

/*this isnt very beautiful - lets have a think see what
we can do to improve it surely some way of combining the two*/
bool valid_flowstate(word_cont* to_check)
{
   int initial_pos;
   initial_pos =to_check->position;

   if(valid_ifelse(to_check))
   {
      return true;
   }
   to_check->position=initial_pos;

   if(valid_if(to_check))
   {
      return true;
   }
   return false;
}



bool valid_if(word_cont* to_check)
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
               return true;
            }
         }
      }
   }
   return false;
}


bool valid_ifelse(word_cont* to_check)
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
            }
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


word_cont* init_word_cont(void)
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
   n_cont->func_map=assoc_init();


   return n_cont;
}



bool compare_doubles(double d_1, double d_2)
{
   double temp;
   temp= d_1 - d_2;
   if(temp<EPSILON && temp > -EPSILON)
   {
      return true;
   }
   return false;
}
