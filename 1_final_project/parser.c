#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <string.h>
#include <assert.h>
#include <ctype.h>
#include <math.h>

#define MAXNUM 1000
#define MAXLEN 100
#define NUMINSTRUCTIONS 3
#define INSTRUCTLEN 3
typedef enum bool {false, true} bool;

/*add in checker that prevents looking over bounds*/
typedef struct word_container
{
   char words[MAXNUM][MAXLEN];
   int position;

}word_cont;


void test(void);
/*return void then cast when appropriate*/
void* safe_calloc(size_t nitems, size_t size);
bool valid_num(word_cont* to_check);
bool valid_instruct(word_cont* to_check);
bool valid_mv(word_cont* to_check,char move[INSTRUCTLEN]);
bool valid_instructlist(word_cont* to_check);
bool valid_main(word_cont* to_check);
bool valid_op(word_cont* to_check);
bool valid_var(word_cont* to_check);
bool valid_varnum(word_cont* to_check);
bool valid_polish(word_cont* to_check);
bool valid_set(word_cont* to_check);
bool valid_do(word_cont* to_check);
word_cont* init_word_cont(void);


int main(void)
{
   test();
   return 0;
}



void test(void)
{
   word_cont* test_cont;
   test_cont=init_word_cont();


   /*assumption for all the below funcs that
   they are within bigger funcs that check
   for null containers etc*/
   strcpy(test_cont->words[0],"1234");
   assert(valid_num(test_cont));
   assert(test_cont->position==1);
   test_cont->position=0;
   assert(valid_varnum(test_cont));
   test_cont->position=0;

   strcpy(test_cont->words[0],"123.4");
   assert(!valid_num(test_cont));
   assert(!valid_varnum(test_cont));
   strcpy(test_cont->words[0],"");
   assert(!valid_num(test_cont));
   assert(!valid_varnum(test_cont));
   strcpy(test_cont->words[0],"1a");
   assert(!valid_num(test_cont));
   strcpy(test_cont->words[0],"a1");
   assert(!valid_varnum(test_cont));
   assert(!valid_num(test_cont));
   strcpy(test_cont->words[0],"1a1");
   assert(!valid_num(test_cont));
   strcpy(test_cont->words[0]," ");
   assert(!valid_num(test_cont));
   assert(test_cont->position==0);
   free(test_cont);


   test_cont=init_word_cont();
   strcpy(test_cont->words[0],"RT");
   strcpy(test_cont->words[1],"1234");
   assert(valid_mv(test_cont,"RT"));
   test_cont->position=0;

   strcpy(test_cont->words[1],"RT");
   assert(!valid_mv(test_cont,"RT"));
   test_cont->position=0;


   strcpy(test_cont->words[1],"90RT");
   assert(!valid_mv(test_cont,"RT"));
   test_cont->position=0;

   strcpy(test_cont->words[1],"");
   assert(!valid_mv(test_cont,"RT"));
   test_cont->position=0;

   strcpy(test_cont->words[1],"12");
   strcpy(test_cont->words[0],"RG");
   assert(!valid_mv(test_cont,"RT"));
   test_cont->position=0;

   strcpy(test_cont->words[0],"LRG");
   assert(!valid_mv(test_cont,"RT"));
   test_cont->position=0;

   strcpy(test_cont->words[0],"LT");
   assert(!valid_mv(test_cont,"RT"));
   test_cont->position=0;


   strcpy(test_cont->words[0],"LT");
   assert(valid_mv(test_cont,"LT"));
   test_cont->position=0;

   strcpy(test_cont->words[0],"FD");
   assert(valid_mv(test_cont,"FD"));
   test_cont->position=0;

   assert(valid_instruct(test_cont));
   assert(test_cont->position=2);
   test_cont->position=0;

   strcpy(test_cont->words[0],"FG");
   assert(!valid_instruct(test_cont));
   assert(test_cont->position==0);

   strcpy(test_cont->words[1],"121RT");
   assert(!valid_instruct(test_cont));
   assert(test_cont->position==0);

   strcpy(test_cont->words[1],"121RT");
   assert(!valid_instruct(test_cont));
   assert(test_cont->position==0);


   test_cont->position=0;
   strcpy(test_cont->words[0],"FD");
   strcpy(test_cont->words[1],"2654");
   strcpy(test_cont->words[2],"}");
   assert(valid_instructlist(test_cont));
   assert(test_cont->position==3);


   test_cont->position=0;
   strcpy(test_cont->words[2],"FD");
   strcpy(test_cont->words[3],"2654");
   strcpy(test_cont->words[4],"}");
   assert(valid_instructlist(test_cont));
   assert(test_cont->position==5);

   test_cont->position=1;
   assert(!valid_instructlist(test_cont));

   test_cont->position=2;
   assert(valid_instructlist(test_cont));

   test_cont->position=0;
   strcpy(test_cont->words[2],"FD");
   strcpy(test_cont->words[3],"");
   strcpy(test_cont->words[4],"}");
   assert(!valid_instructlist(test_cont));

   test_cont->position=0;
   strcpy(test_cont->words[3],"2654");
   strcpy(test_cont->words[4],"2");
   assert(!valid_instructlist(test_cont));

   test_cont->position=0;
   strcpy(test_cont->words[2],"LT");
   strcpy(test_cont->words[3],"2654");
   strcpy(test_cont->words[4],"}");
   assert(valid_instructlist(test_cont));
   assert(test_cont->position==5);

   test_cont->position=0;
   strcpy(test_cont->words[1],"}");
   assert(!valid_instructlist(test_cont));
   strcpy(test_cont->words[1],"2654");

   strcpy(test_cont->words[0],"}");
   assert(valid_instructlist(test_cont));
   strcpy(test_cont->words[0],"FD");
   assert(test_cont->position==1);

   test_cont->position=0;
   assert(!valid_main(test_cont));

   test_cont->position=0;
   strcpy(test_cont->words[0],"{");
   strcpy(test_cont->words[1],"FD");
   strcpy(test_cont->words[2],"2654");
   strcpy(test_cont->words[3],"}");
   assert(valid_main(test_cont));
   assert(test_cont->position==4);

   test_cont->position=0;
   strcpy(test_cont->words[1],"}");
   assert(valid_main(test_cont));

   test_cont->position=0;
   strcpy(test_cont->words[0],"");
   strcpy(test_cont->words[1],"FD");
   assert(!valid_main(test_cont));

   test_cont->position=0;
   strcpy(test_cont->words[0],"{");
   strcpy(test_cont->words[3],"");
   strcpy(test_cont->words[4],"}");
   assert(!valid_main(test_cont));
   free(test_cont);

   test_cont=init_word_cont();
   strcpy(test_cont->words[0],"+");
   assert(valid_op(test_cont));
   assert(test_cont->position==1);
   test_cont->position=0;
   strcpy(test_cont->words[0],"++");
   assert(!valid_op(test_cont));
   strcpy(test_cont->words[0],"+-");
   assert(!valid_op(test_cont));
   strcpy(test_cont->words[0],"/-");
   assert(!valid_op(test_cont));
   assert(!valid_varnum(test_cont));

   strcpy(test_cont->words[0],"AA");
   assert(!valid_var(test_cont));
   strcpy(test_cont->words[0],"A");
   assert(valid_var(test_cont));
   assert(test_cont->position==1);
   test_cont->position=0;
   strcpy(test_cont->words[0],"Aa");
   assert(!valid_var(test_cont));
   assert(!valid_varnum(test_cont));
   strcpy(test_cont->words[0],"a");
   assert(!valid_var(test_cont));
   strcpy(test_cont->words[0],"Z");
   assert(valid_var(test_cont));
   assert(test_cont->position==1);
   test_cont->position=0;
   assert(valid_varnum(test_cont));
   free(test_cont);

   test_cont=init_word_cont();
   strcpy(test_cont->words[0],"/");
   strcpy(test_cont->words[1],";");
   assert(valid_polish(test_cont));
   assert(test_cont->position==2);
   test_cont->position=0;
   /*doesnt have to make sense to be valid*/
   strcpy(test_cont->words[1],"2");
   strcpy(test_cont->words[2],";");
   assert(valid_polish(test_cont));
   assert(test_cont->position==3);
   test_cont->position=0;

   strcpy(test_cont->words[1],"+");
   assert(valid_polish(test_cont));
   assert(test_cont->position==3);
   test_cont->position=0;

   strcpy(test_cont->words[3],";");
   assert(valid_polish(test_cont));
   assert(test_cont->position==3);
   assert(valid_polish(test_cont));
   test_cont->position=0;

   strcpy(test_cont->words[1],"A");
   assert(valid_polish(test_cont));
   test_cont->position=0;
   strcpy(test_cont->words[2],"A");
   assert(valid_polish(test_cont));
   test_cont->position=0;


   strcpy(test_cont->words[2],"");
   assert(!valid_polish(test_cont));
   strcpy(test_cont->words[2],"A");
   test_cont->position=0;
   strcpy(test_cont->words[1],"FD");
   assert(!valid_polish(test_cont));
   strcpy(test_cont->words[1],"1A");
   assert(!valid_polish(test_cont));
   strcpy(test_cont->words[1],"+1");
   assert(!valid_polish(test_cont));
   strcpy(test_cont->words[1],";;");
   assert(!valid_polish(test_cont));
   /*no semi colon*/
   strcpy(test_cont->words[1],"A");
   strcpy(test_cont->words[3],"A");
   assert(!valid_polish(test_cont));
   free(test_cont);

   test_cont=init_word_cont();
   strcpy(test_cont->words[0],"SET");
   strcpy(test_cont->words[1],"A");
   strcpy(test_cont->words[2],":=");
   strcpy(test_cont->words[3],"/");
   strcpy(test_cont->words[4],";");
   assert(valid_set(test_cont));
   assert(test_cont->position==5);
   test_cont->position=0;
   assert(valid_instruct(test_cont));

   test_cont->position=0;
   strcpy(test_cont->words[0],"SET");
   strcpy(test_cont->words[1],"A");
   strcpy(test_cont->words[2],":=");
   strcpy(test_cont->words[3],"A");
   strcpy(test_cont->words[4],"/");
   strcpy(test_cont->words[5],"A");
   strcpy(test_cont->words[6],";");
   assert(valid_set(test_cont));
   assert(test_cont->position==7);
   test_cont->position=0;
   assert(valid_instruct(test_cont));
   assert(test_cont->position==7);
   test_cont->position=0;

   strcpy(test_cont->words[0],"SET");
   strcpy(test_cont->words[1],"A");
   strcpy(test_cont->words[2],":=");
   strcpy(test_cont->words[3],";");
   strcpy(test_cont->words[4],"/");
   strcpy(test_cont->words[5],"A");
   strcpy(test_cont->words[6],";");
   assert(valid_set(test_cont));
   assert(test_cont->position==4);
   test_cont->position=0;
   assert(valid_instruct(test_cont));
   assert(test_cont->position==4);
   test_cont->position=0;

   strcpy(test_cont->words[0],"SET");
   strcpy(test_cont->words[1],"A");
   strcpy(test_cont->words[2],":=");
   strcpy(test_cont->words[3],"1");
   strcpy(test_cont->words[4],"/");
   strcpy(test_cont->words[5],"A");
   strcpy(test_cont->words[6],";");
   assert(valid_set(test_cont));
   assert(test_cont->position==7);
   test_cont->position=0;
   assert(valid_instruct(test_cont));
   assert(test_cont->position==7);
   test_cont->position=0;

   strcpy(test_cont->words[0],"SET");
   strcpy(test_cont->words[1],"A");
   strcpy(test_cont->words[2],":=");
   strcpy(test_cont->words[3],"a");
   strcpy(test_cont->words[4],"/");
   strcpy(test_cont->words[5],"A");
   strcpy(test_cont->words[6],";");
   assert(!valid_set(test_cont));
   test_cont->position=0;
   assert(!valid_instruct(test_cont));
   test_cont->position=0;

   strcpy(test_cont->words[0],"set");
   strcpy(test_cont->words[1],"A");
   strcpy(test_cont->words[2],":=");
   strcpy(test_cont->words[3],"A");
   strcpy(test_cont->words[4],"/");
   strcpy(test_cont->words[5],"A");
   strcpy(test_cont->words[6],";");
   assert(!valid_set(test_cont));
   test_cont->position=0;
   assert(!valid_instruct(test_cont));
   test_cont->position=0;

   strcpy(test_cont->words[0],"SET");
   strcpy(test_cont->words[1],"aa");
   assert(!valid_set(test_cont));
   test_cont->position=0;
   assert(!valid_instruct(test_cont));
   test_cont->position=0;


   strcpy(test_cont->words[1],"A");
   strcpy(test_cont->words[2],"=:");
   assert(!valid_set(test_cont));
   test_cont->position=0;
   assert(!valid_instruct(test_cont));
   test_cont->position=0;

   strcpy(test_cont->words[2],"A");
   assert(!valid_set(test_cont));
   test_cont->position=0;
   assert(!valid_instruct(test_cont));
   test_cont->position=0;

   strcpy(test_cont->words[2],"");
   assert(!valid_set(test_cont));
   test_cont->position=0;
   assert(!valid_instruct(test_cont));
   test_cont->position=0;
   free(test_cont);

   test_cont=init_word_cont();

   strcpy(test_cont->words[0],"DO");
   strcpy(test_cont->words[1],"A");
   strcpy(test_cont->words[2],"FROM");
   strcpy(test_cont->words[3],"3");
   strcpy(test_cont->words[4],"TO");
   strcpy(test_cont->words[5],"9");
   strcpy(test_cont->words[6],"{");
   strcpy(test_cont->words[7],"}");
   assert(valid_do(test_cont));
   assert(test_cont->position==8);
   test_cont->position=0;
   assert(valid_instruct(test_cont));
   test_cont->position=0;

   strcpy(test_cont->words[7],"FD");
   strcpy(test_cont->words[8],"40");
   strcpy(test_cont->words[9],"}");
   assert(valid_do(test_cont));
   assert(test_cont->position==10);
   test_cont->position=0;
   assert(valid_instruct(test_cont));
   test_cont->position=0;


   strcpy(test_cont->words[7],"DO");
   strcpy(test_cont->words[8],"A");
   strcpy(test_cont->words[9],"FROM");
   strcpy(test_cont->words[10],"3");
   strcpy(test_cont->words[11],"TO");
   strcpy(test_cont->words[12],"9");
   strcpy(test_cont->words[13],"{");
   strcpy(test_cont->words[14],"FD");
   strcpy(test_cont->words[15],"40");
   strcpy(test_cont->words[16],"}");
   strcpy(test_cont->words[17],"}");
   assert(valid_do(test_cont));
   assert(test_cont->position==18);
   test_cont->position=0;
   assert(valid_instruct(test_cont));
   test_cont->position=0;

   strcpy(test_cont->words[3],"10");
   assert(valid_do(test_cont));
   assert(test_cont->position==18);
   test_cont->position=0;

   strcpy(test_cont->words[14],"LT");
   assert(valid_do(test_cont));
   assert(test_cont->position==18);
   test_cont->position=0;
   assert(valid_instruct(test_cont));
   test_cont->position=0;

   strcpy(test_cont->words[17],"");
   assert(!valid_do(test_cont));
   test_cont->position=0;
   assert(!valid_instruct(test_cont));
   test_cont->position=0;

   strcpy(test_cont->words[17],"}");


   strcpy(test_cont->words[16],"");
   assert(!valid_do(test_cont));
   test_cont->position=0;
   assert(!valid_instruct(test_cont));
   test_cont->position=0;
   strcpy(test_cont->words[16],"}");

   strcpy(test_cont->words[13],"}");
   assert(!valid_do(test_cont));
   test_cont->position=0;
   assert(!valid_instruct(test_cont));
   test_cont->position=0;
   strcpy(test_cont->words[13],"{");

   strcpy(test_cont->words[8],"2");
   assert(!valid_do(test_cont));
   test_cont->position=0;
   assert(!valid_instruct(test_cont));
   test_cont->position=0;
   strcpy(test_cont->words[8],"A");

   strcpy(test_cont->words[7],"OD");
   assert(!valid_do(test_cont));
   test_cont->position=0;
   assert(!valid_instruct(test_cont));
   test_cont->position=0;
   strcpy(test_cont->words[7],"DO");
   free(test_cont);


   test_cont=init_word_cont();

   strcpy(test_cont->words[0],"{");
   strcpy(test_cont->words[1],"FD");
   strcpy(test_cont->words[2],"40");
   strcpy(test_cont->words[3],"SET");
   strcpy(test_cont->words[4],"A");
   strcpy(test_cont->words[5],":=");
   strcpy(test_cont->words[6],"30");
   strcpy(test_cont->words[7],"/");
   strcpy(test_cont->words[8],";");

   strcpy(test_cont->words[9],"DO");
   strcpy(test_cont->words[10],"B");
   strcpy(test_cont->words[11],"FROM");
   strcpy(test_cont->words[12],"A");
   strcpy(test_cont->words[13],"TO");
   strcpy(test_cont->words[14],"30");
   strcpy(test_cont->words[15],"{");

   strcpy(test_cont->words[16],"}");
   strcpy(test_cont->words[17],"}");

   assert(valid_main(test_cont));
   assert(test_cont->position==18);
   test_cont->position=0;

   strcpy(test_cont->words[17],"DO");
   strcpy(test_cont->words[18],"C");
   strcpy(test_cont->words[19],"FROM");
   strcpy(test_cont->words[20],"A");
   strcpy(test_cont->words[21],"TO");
   strcpy(test_cont->words[22],"30");
   strcpy(test_cont->words[23],"{");
   strcpy(test_cont->words[24],"LT");
   strcpy(test_cont->words[25],"30");
   strcpy(test_cont->words[26],"FD");
   strcpy(test_cont->words[27],"30");
   strcpy(test_cont->words[28],"}");
   strcpy(test_cont->words[29],"}");
   strcpy(test_cont->words[30],"}");
   assert(valid_main(test_cont));
   assert(test_cont->position==30);
   test_cont->position=0;
}

/*return void then cast when appropriate*/
void* safe_calloc(size_t nitems, size_t size)
{
   void* ptr;
   ptr = calloc(nitems, size);
   if(ptr==NULL)
   {
      fprintf(stderr,"Not enough space\n");
   	exit(EXIT_FAILURE);
   }
   return ptr;
}


word_cont* init_word_cont(void)
{
   word_cont* n_cont;
   int i;
   n_cont=safe_calloc(1,sizeof(word_cont));
   for(i=0;i<MAXNUM;i++)
   {
      n_cont->words[i][0]='\0';
   }
   n_cont->position=0;
   return n_cont;
}


bool valid_num(word_cont* to_check)
{
   int i;
   int len;
   len=strlen(to_check->words[to_check->position]);
   /*blank string not number*/
   if(len==0)
   {
      return false;
   }
   for(i=0;i<len;i++)
   {
      if(!isdigit(to_check->words[to_check->position][i]))
      {
         return false;
      }
   }
   to_check->position++;
   return true;
}

bool valid_var(word_cont* to_check)
{

   if(strlen(to_check->words[to_check->position])==1)
   {
      if(to_check->words[to_check->position][0]>='A'&&\
         to_check->words[to_check->position][0]<='Z')
      {
         to_check->position++;
         return true;
      }
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
   return false;
}


bool valid_mv(word_cont* to_check,char move[INSTRUCTLEN])
{
   if(strcmp(to_check->words[to_check->position],move)==0)
   {
      to_check->position++;
      if(valid_num(to_check))
      {
         return true;
      }
   }
   return false;
}



bool valid_instruct(word_cont* to_check)
{
   int i;
   int init_pos;
   char instructions[NUMINSTRUCTIONS][INSTRUCTLEN]= {"FD", "LT","RT"};
   init_pos=to_check->position;

   for(i=0;i<NUMINSTRUCTIONS;i++)
   {
      if(valid_mv(to_check,instructions[i]))
      {
         return true;
      }
      else
      {
         to_check->position=init_pos;
      }
   }
   if(valid_set(to_check))
   {
      return true;
   }
   to_check->position=init_pos;

   if(valid_do(to_check))
   {
      return true;
   }

   return false;
}



bool valid_instructlist(word_cont* to_check)
{
   if(strcmp(to_check->words[to_check->position],"}")==0)
   {
      /*doesnt matter for end but for do loops important
      to increase position*/
      to_check->position++;
      return true;
   }
   else
   {
      if(valid_instruct(to_check))
      {
         if(valid_instructlist(to_check))
         {
            return true;
         }
      }
   }
   return false;
}


bool valid_main(word_cont* to_check)
{
   if(strcmp(to_check->words[to_check->position],"{")==0)
   {
      to_check->position++;
      if(valid_instructlist(to_check))
      {
         return true;
      }
   }
   return false;
}


bool valid_op(word_cont* to_check)
{
   if(strcmp(to_check->words[to_check->position],"+")==0)
   {
      to_check->position++;
      return true;
   }
   if(strcmp(to_check->words[to_check->position],"-")==0)
   {
      to_check->position++;
      return true;
   }
   if(strcmp(to_check->words[to_check->position],"*")==0)
   {
      to_check->position++;
      return true;
   }
   if(strcmp(to_check->words[to_check->position],"/")==0)
   {
      to_check->position++;
      return true;
   }
   return false;
}


bool valid_polish(word_cont* to_check)
{
   if(strcmp(to_check->words[to_check->position],";")==0)
   {
      to_check->position++;
      return true;
   }
   if(valid_op(to_check))
   {
      if(valid_polish(to_check))
      {
         return true;
      }
   }
   if(valid_varnum(to_check))
   {
      if(valid_polish(to_check))
      {
         return true;
      }
   }
   return false;
}


bool valid_set(word_cont* to_check)
{
   if(strcmp(to_check->words[to_check->position],"SET")==0)
   {
      to_check->position++;
      if(valid_var(to_check))
      {
         if(strcmp(to_check->words[to_check->position],":=")==0)
         {
            to_check->position++;
            if(valid_polish(to_check))
            {
               return true;
            }
         }
      }
   }
   return false;
}

bool valid_do(word_cont* to_check)
{
   if(strcmp(to_check->words[to_check->position],"DO")==0)
   {
      to_check->position++;
      if(valid_var(to_check))
      {
         if(strcmp(to_check->words[to_check->position],"FROM")==0)
         {
            to_check->position++;
            if(valid_varnum(to_check))
            {
               if(strcmp(to_check->words[to_check->position],"TO")==0)
               {
                  to_check->position++;
                  if(valid_varnum(to_check))
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
            }
         }
      }
   }
   return false;
}
