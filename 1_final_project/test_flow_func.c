#include "extension_funcs.h"
#include "extension_flow.h"

#define MAXTESTCAP 100
#define MAXTESTLEN 50
#define EPSILON 0.00001

bool compare_doubles(double d_1, double d_2);
word_cont* init_word_cont(void);

int main(void)
{
   word_cont* test_cont;
   word_cont* test_func;
   int i;

   line_cont* test_line_cont;


   test_cont=init_word_cont();
   test_line_cont=init_line_cont();

   strcpy(test_cont->words[0],"{");

   strcpy(test_cont->words[1],"SETFUNC");
   strcpy(test_cont->words[2],"abc");
   strcpy(test_cont->words[3],"{");
   strcpy(test_cont->words[4],"A");
   strcpy(test_cont->words[5],"}");

   strcpy(test_cont->words[6],"{");
   strcpy(test_cont->words[7],"SET");
   strcpy(test_cont->words[8],"A");
   strcpy(test_cont->words[9],":=");
   strcpy(test_cont->words[10],"10");
   strcpy(test_cont->words[11],"A");
   strcpy(test_cont->words[12],"*");
   strcpy(test_cont->words[13],";");

   strcpy(test_cont->words[14],"RETURN");
   strcpy(test_cont->words[15],"A");
   strcpy(test_cont->words[16],"}");

   strcpy(test_cont->words[17],"IF");
   strcpy(test_cont->words[18],"abc");
   strcpy(test_cont->words[19],"{");
   strcpy(test_cont->words[20],"100");
   strcpy(test_cont->words[21],"}");
   strcpy(test_cont->words[22],">");
   strcpy(test_cont->words[23],"200");
   strcpy(test_cont->words[24],"{");
   strcpy(test_cont->words[25],"FD");
   strcpy(test_cont->words[26],"50");
   strcpy(test_cont->words[27],"}");
   strcpy(test_cont->words[28],"}");
   assert(run_main(test_cont,test_line_cont));
   assert(test_line_cont->size==1);
   free_word_cont(test_cont);
   free_line_cont(test_line_cont);



   test_cont=init_word_cont();
   test_line_cont=init_line_cont();

   strcpy(test_cont->words[0],"{");

   strcpy(test_cont->words[1],"SETFUNC");
   strcpy(test_cont->words[2],"abc");
   strcpy(test_cont->words[3],"{");
   strcpy(test_cont->words[4],"A");
   strcpy(test_cont->words[5],"}");

   strcpy(test_cont->words[6],"{");
   strcpy(test_cont->words[7],"SET");
   strcpy(test_cont->words[8],"A");
   strcpy(test_cont->words[9],":=");
   strcpy(test_cont->words[10],"10");
   strcpy(test_cont->words[11],"A");
   strcpy(test_cont->words[12],"*");
   strcpy(test_cont->words[13],";");

   strcpy(test_cont->words[14],"RETURN");
   strcpy(test_cont->words[15],"A");
   strcpy(test_cont->words[16],"}");

   strcpy(test_cont->words[17],"IF");
   strcpy(test_cont->words[18],"abc");
   strcpy(test_cont->words[19],"{");
   strcpy(test_cont->words[20],"100");
   strcpy(test_cont->words[21],"}");
   strcpy(test_cont->words[22],">");
   strcpy(test_cont->words[23],"200");
   strcpy(test_cont->words[24],"{");
   strcpy(test_cont->words[25],"FD");
   strcpy(test_cont->words[26],"abc");
   strcpy(test_cont->words[27],"{");
   strcpy(test_cont->words[28],"7");
   strcpy(test_cont->words[29],"}");
   strcpy(test_cont->words[30],"}");
   strcpy(test_cont->words[31],"}");
   assert(run_main(test_cont,test_line_cont));
   assert(test_line_cont->size==1);
   assert(compare_doubles(test_line_cont->array[0]->end->y,70));
   free_word_cont(test_cont);
   free_line_cont(test_line_cont);

   test_cont=init_word_cont();
   test_line_cont=init_line_cont();

   strcpy(test_cont->words[0],"{");

   strcpy(test_cont->words[1],"SETFUNC");
   strcpy(test_cont->words[2],"abc");
   strcpy(test_cont->words[3],"{");
   strcpy(test_cont->words[4],"A");
   strcpy(test_cont->words[5],"}");

   strcpy(test_cont->words[6],"{");
   strcpy(test_cont->words[7],"SET");
   strcpy(test_cont->words[8],"A");
   strcpy(test_cont->words[9],":=");
   strcpy(test_cont->words[10],"10");
   strcpy(test_cont->words[11],"A");
   strcpy(test_cont->words[12],"*");
   strcpy(test_cont->words[13],";");

   strcpy(test_cont->words[14],"RETURN");
   strcpy(test_cont->words[15],"A");
   strcpy(test_cont->words[16],"}");

   strcpy(test_cont->words[17],"IF");
   strcpy(test_cont->words[18],"abc");
   strcpy(test_cont->words[19],"{");
   strcpy(test_cont->words[20],"1");
   strcpy(test_cont->words[21],"}");
   strcpy(test_cont->words[22],">");
   strcpy(test_cont->words[23],"200");
   strcpy(test_cont->words[24],"{");
   strcpy(test_cont->words[25],"FD");
   strcpy(test_cont->words[26],"abc");
   strcpy(test_cont->words[27],"{");
   strcpy(test_cont->words[28],"7");
   strcpy(test_cont->words[29],"}");

   strcpy(test_cont->words[30],"}");


   strcpy(test_cont->words[31],"ELSE");
   strcpy(test_cont->words[32],"{");

   strcpy(test_cont->words[33],"RT");
   strcpy(test_cont->words[34],"abc");
   strcpy(test_cont->words[35],"{");
   strcpy(test_cont->words[36],"9");
   strcpy(test_cont->words[37],"}");
   strcpy(test_cont->words[38],"FD");
   strcpy(test_cont->words[39],"10");
   strcpy(test_cont->words[40],"}");

   strcpy(test_cont->words[41],"}");
   assert(run_main(test_cont,test_line_cont));

   assert(test_line_cont->size==1);
   assert(compare_doubles(test_line_cont->array[0]->end->x,10));
   free_word_cont(test_cont);
   free_line_cont(test_line_cont);



   return 0;
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
