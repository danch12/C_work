#include "../src/unity.h"
#include "../extension_funcs.h"
#include "../extension_flow.h"


#define MAXTESTCAP 100
#define MAXTESTLEN 50

word_cont* init_word_cont(void);

void setUp(void)
{

}

void tearDown(void)
{

}

void test_func_flow_interaction(void)
{
   word_cont* test_cont;


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
   TEST_ASSERT_TRUE(run_main(test_cont,test_line_cont));
   TEST_ASSERT_TRUE(test_line_cont->size==1);
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
   TEST_ASSERT_TRUE(run_main(test_cont,test_line_cont));
   TEST_ASSERT_TRUE(test_line_cont->size==1);
   TEST_ASSERT_EQUAL_DOUBLE(test_line_cont->array[0]->end->y,70);
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
   TEST_ASSERT_TRUE(run_main(test_cont,test_line_cont));

   TEST_ASSERT_TRUE(test_line_cont->size==1);
   TEST_ASSERT_EQUAL_DOUBLE(test_line_cont->array[0]->end->x,10);
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
   strcpy(test_cont->words[7],"IF");
   strcpy(test_cont->words[8],"A");
   strcpy(test_cont->words[9],">");
   strcpy(test_cont->words[10],"10");
   strcpy(test_cont->words[11],"{");
   strcpy(test_cont->words[12],"RETURN");
   strcpy(test_cont->words[13],"10");
   strcpy(test_cont->words[14],"}");

   strcpy(test_cont->words[15],"ELSE");
   strcpy(test_cont->words[16],"{");
   strcpy(test_cont->words[17],"SET");
   strcpy(test_cont->words[18],"A");
   strcpy(test_cont->words[19],":=");
   strcpy(test_cont->words[20],"10");
   strcpy(test_cont->words[21],"A");
   strcpy(test_cont->words[22],"*");
   strcpy(test_cont->words[23],";");

   strcpy(test_cont->words[24],"RETURN");
   strcpy(test_cont->words[25],"A");
   strcpy(test_cont->words[26],"}");
   strcpy(test_cont->words[27],"}");

   strcpy(test_cont->words[28],"FD");
   strcpy(test_cont->words[29],"abc");
   strcpy(test_cont->words[30],"{");
   strcpy(test_cont->words[31],"11");
   strcpy(test_cont->words[32],"}");
   strcpy(test_cont->words[33],"}");

   TEST_ASSERT_TRUE(run_main(test_cont,test_line_cont));
   TEST_ASSERT_TRUE(test_line_cont->size==1);
   TEST_ASSERT_EQUAL_DOUBLE(test_line_cont->array[0]->end->y,10);
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
   strcpy(test_cont->words[7],"IF");
   strcpy(test_cont->words[8],"A");
   strcpy(test_cont->words[9],">");
   strcpy(test_cont->words[10],"10");
   strcpy(test_cont->words[11],"{");
   strcpy(test_cont->words[12],"RETURN");
   strcpy(test_cont->words[13],"10");
   strcpy(test_cont->words[14],"}");

   strcpy(test_cont->words[15],"ELSE");
   strcpy(test_cont->words[16],"{");
   strcpy(test_cont->words[17],"SET");
   strcpy(test_cont->words[18],"A");
   strcpy(test_cont->words[19],":=");
   strcpy(test_cont->words[20],"10");
   strcpy(test_cont->words[21],"A");
   strcpy(test_cont->words[22],"*");
   strcpy(test_cont->words[23],";");

   strcpy(test_cont->words[24],"RETURN");
   strcpy(test_cont->words[25],"A");
   strcpy(test_cont->words[26],"}");
   strcpy(test_cont->words[27],"}");

   strcpy(test_cont->words[28],"FD");
   strcpy(test_cont->words[29],"abc");
   strcpy(test_cont->words[30],"{");
   strcpy(test_cont->words[31],"9");
   strcpy(test_cont->words[32],"}");
   strcpy(test_cont->words[33],"}");

   TEST_ASSERT_TRUE(run_main(test_cont,test_line_cont));
   TEST_ASSERT_TRUE(test_line_cont->size==1);
   TEST_ASSERT_EQUAL_DOUBLE(test_line_cont->array[0]->end->y,90);
   free_word_cont(test_cont);
   free_line_cont(test_line_cont);
}

int main(void)
{
   UNITY_BEGIN();
   RUN_TEST(test_func_flow_interaction);
   return UNITY_END();
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