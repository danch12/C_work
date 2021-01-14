#include "../src/unity.h"
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


void test_comparator(void)
{
   word_cont* test_cont;
   test_cont=init_word_cont();
   strcpy(test_cont->words[0],"==");
   TEST_ASSERT_TRUE(get_comparator(test_cont)==equal);
   strcpy(test_cont->words[0],"<");
   TEST_ASSERT_TRUE(get_comparator(test_cont)==less);
   strcpy(test_cont->words[0],">");
   TEST_ASSERT_TRUE(get_comparator(test_cont)==greater);
   strcpy(test_cont->words[0],"=");
   TEST_ASSERT_TRUE(get_comparator(test_cont)==inv_op);
   strcpy(test_cont->words[0],"<=");
   TEST_ASSERT_TRUE(get_comparator(test_cont)==less_equal);
   strcpy(test_cont->words[0],">=");
   TEST_ASSERT_TRUE(get_comparator(test_cont)==greater_equal);
   free_word_cont(test_cont);

}

void test_valid_expression(void)
{

   word_cont* test_cont;
   test_cont=init_word_cont();
   strcpy(test_cont->words[0],"10");
   strcpy(test_cont->words[1],"==");
   strcpy(test_cont->words[2],"A");
   TEST_ASSERT_TRUE(valid_expression(test_cont));

   test_cont->position=0;
   strcpy(test_cont->words[0],"10");
   strcpy(test_cont->words[1],"==");
   strcpy(test_cont->words[2],"10");
   TEST_ASSERT_TRUE(valid_expression(test_cont));


   test_cont->position=0;
   strcpy(test_cont->words[0],"10");
   strcpy(test_cont->words[1],">=");
   strcpy(test_cont->words[2],"10");
   TEST_ASSERT_TRUE(valid_expression(test_cont));

   test_cont->position=0;
   strcpy(test_cont->words[0],"10");
   strcpy(test_cont->words[1],"<=");
   strcpy(test_cont->words[2],"10");
   TEST_ASSERT_TRUE(valid_expression(test_cont));

   test_cont->position=0;
   strcpy(test_cont->words[0],"10");
   strcpy(test_cont->words[1],">");
   strcpy(test_cont->words[2],"10");
   TEST_ASSERT_TRUE(valid_expression(test_cont));

   test_cont->position=0;
   strcpy(test_cont->words[0],"10");
   strcpy(test_cont->words[1],"<");
   strcpy(test_cont->words[2],"10");
   TEST_ASSERT_TRUE(valid_expression(test_cont));

   test_cont->position=0;
   strcpy(test_cont->words[0],"B");
   strcpy(test_cont->words[1],"<");
   strcpy(test_cont->words[2],"A");
   TEST_ASSERT_TRUE(valid_expression(test_cont));

   test_cont->position=0;
   strcpy(test_cont->words[0],"<");
   strcpy(test_cont->words[1],"<");
   strcpy(test_cont->words[2],"A");
   TEST_ASSERT_TRUE(!valid_expression(test_cont));

   test_cont->position=0;
   strcpy(test_cont->words[0],"<");
   strcpy(test_cont->words[1],"10");
   strcpy(test_cont->words[2],"A");
   TEST_ASSERT_TRUE(!valid_expression(test_cont));

   test_cont->position=0;
   strcpy(test_cont->words[0],"10");
   strcpy(test_cont->words[1],"");
   strcpy(test_cont->words[2],"A");
   TEST_ASSERT_TRUE(!valid_expression(test_cont));

   test_cont->position=0;
   strcpy(test_cont->words[0],"10");
   strcpy(test_cont->words[1],"=");
   strcpy(test_cont->words[2],"A");
   TEST_ASSERT_TRUE(!valid_expression(test_cont));
   free_word_cont(test_cont);
}

void test_valid_flowstate(void)
{
   word_cont* test_cont;
   test_cont=init_word_cont();
   strcpy(test_cont->words[0],"IF");
   strcpy(test_cont->words[1],"10");
   strcpy(test_cont->words[2],"<=");
   strcpy(test_cont->words[3],"A");
   strcpy(test_cont->words[4],"{");
   strcpy(test_cont->words[5],"FD");
   strcpy(test_cont->words[6],"10");
   strcpy(test_cont->words[7],"}");


   TEST_ASSERT_TRUE(valid_flowstate(test_cont));
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

   TEST_ASSERT_TRUE(valid_flowstate(test_cont));

   test_cont->position=0;
   strcpy(test_cont->words[0],"IF");
   strcpy(test_cont->words[1],"10");
   strcpy(test_cont->words[2],">=");
   strcpy(test_cont->words[3],"A");

   strcpy(test_cont->words[4],"{");

   strcpy(test_cont->words[5],"DO");
   strcpy(test_cont->words[6],"A");
   strcpy(test_cont->words[7],"FROM");
   strcpy(test_cont->words[8],"8");
   strcpy(test_cont->words[9],"TO");
   strcpy(test_cont->words[10],"9");
   strcpy(test_cont->words[11],"{");
   strcpy(test_cont->words[12],"FD");
   strcpy(test_cont->words[13],"A");
   strcpy(test_cont->words[14],"}");
   strcpy(test_cont->words[15],"}");

   TEST_ASSERT_TRUE(valid_flowstate(test_cont));

   test_cont->position=0;
   strcpy(test_cont->words[0],"IF");
   strcpy(test_cont->words[1],"10");
   strcpy(test_cont->words[2],"==");
   strcpy(test_cont->words[3],"A");

   strcpy(test_cont->words[4],"{");

   strcpy(test_cont->words[5],"DO");
   strcpy(test_cont->words[6],"A");
   strcpy(test_cont->words[7],"FROM");
   strcpy(test_cont->words[8],"8");
   strcpy(test_cont->words[9],"TO");
   strcpy(test_cont->words[10],"9");
   strcpy(test_cont->words[11],"{");
   strcpy(test_cont->words[12],"FD");
   strcpy(test_cont->words[13],"A");
   strcpy(test_cont->words[14],"}");
   strcpy(test_cont->words[15],"");


   TEST_ASSERT_TRUE(!valid_flowstate(test_cont));

   test_cont->position=0;
   strcpy(test_cont->words[0],"IF");
   strcpy(test_cont->words[1],"10");
   strcpy(test_cont->words[2],"=");
   strcpy(test_cont->words[3],"A");

   strcpy(test_cont->words[4],"{");

   strcpy(test_cont->words[5],"DO");
   strcpy(test_cont->words[6],"A");
   strcpy(test_cont->words[7],"FROM");
   strcpy(test_cont->words[8],"8");
   strcpy(test_cont->words[9],"TO");
   strcpy(test_cont->words[10],"9");
   strcpy(test_cont->words[11],"{");
   strcpy(test_cont->words[12],"FD");
   strcpy(test_cont->words[13],"A");
   strcpy(test_cont->words[14],"}");
   strcpy(test_cont->words[15],"}");


   TEST_ASSERT_TRUE(!valid_flowstate(test_cont));

   test_cont->position=0;
   strcpy(test_cont->words[0],"IF");
   strcpy(test_cont->words[1],"abc");
   strcpy(test_cont->words[2],">=");
   strcpy(test_cont->words[3],"A");

   strcpy(test_cont->words[4],"{");

   strcpy(test_cont->words[5],"DO");
   strcpy(test_cont->words[6],"A");
   strcpy(test_cont->words[7],"FROM");
   strcpy(test_cont->words[8],"8");
   strcpy(test_cont->words[9],"TO");
   strcpy(test_cont->words[10],"9");
   strcpy(test_cont->words[11],"{");
   strcpy(test_cont->words[12],"FD");
   strcpy(test_cont->words[13],"A");
   strcpy(test_cont->words[14],"}");
   strcpy(test_cont->words[15],"}");


   TEST_ASSERT_TRUE(!valid_flowstate(test_cont));

   free_word_cont(test_cont);


   test_cont=init_word_cont();

   strcpy(test_cont->words[0],"IF");
   strcpy(test_cont->words[1],"A");
   strcpy(test_cont->words[2],"==");
   strcpy(test_cont->words[3],"A");

   strcpy(test_cont->words[4],"{");

   strcpy(test_cont->words[5],"DO");
   strcpy(test_cont->words[6],"A");
   strcpy(test_cont->words[7],"FROM");
   strcpy(test_cont->words[8],"8");
   strcpy(test_cont->words[9],"TO");
   strcpy(test_cont->words[10],"9");
   strcpy(test_cont->words[11],"{");
   strcpy(test_cont->words[12],"FD");
   strcpy(test_cont->words[13],"A");
   strcpy(test_cont->words[14],"}");
   strcpy(test_cont->words[15],"}");

   strcpy(test_cont->words[16],"ELSE");
   strcpy(test_cont->words[17],"{");

   strcpy(test_cont->words[18],"DO");
   strcpy(test_cont->words[19],"A");
   strcpy(test_cont->words[20],"FROM");
   strcpy(test_cont->words[21],"8");
   strcpy(test_cont->words[22],"TO");
   strcpy(test_cont->words[23],"9");
   strcpy(test_cont->words[24],"{");
   strcpy(test_cont->words[25],"RT");
   strcpy(test_cont->words[26],"A");
   strcpy(test_cont->words[27],"}");
   strcpy(test_cont->words[28],"}");


   TEST_ASSERT_TRUE(valid_flowstate(test_cont));



   test_cont->position=0;
   strcpy(test_cont->words[0],"IF");
   strcpy(test_cont->words[1],"A");
   strcpy(test_cont->words[2],"==");
   strcpy(test_cont->words[3],"A");

   strcpy(test_cont->words[4],"{");

   strcpy(test_cont->words[5],"IF");
   strcpy(test_cont->words[6],"A");
   strcpy(test_cont->words[7],"==");
   strcpy(test_cont->words[8],"B");

   strcpy(test_cont->words[9],"{");
   strcpy(test_cont->words[10],"FD");
   strcpy(test_cont->words[11],"9");
   strcpy(test_cont->words[12],"}");

   strcpy(test_cont->words[13],"ELSE");
   strcpy(test_cont->words[14],"{");
   strcpy(test_cont->words[15],"FD");

   strcpy(test_cont->words[16],"12");
   strcpy(test_cont->words[17],"}");

   strcpy(test_cont->words[18],"}");

   strcpy(test_cont->words[19],"ELSE");
   strcpy(test_cont->words[20],"{");
   strcpy(test_cont->words[21],"FD");
   strcpy(test_cont->words[22],"13");
   strcpy(test_cont->words[23],"}");

   test_cont->position=0;
   TEST_ASSERT_TRUE(valid_flowstate(test_cont));


   test_cont->position=0;

   strcpy(test_cont->words[0],"IF");
   strcpy(test_cont->words[1],"A");
   strcpy(test_cont->words[2],"==");
   strcpy(test_cont->words[3],"A");

   strcpy(test_cont->words[4],"{");

   strcpy(test_cont->words[5],"IF");
   strcpy(test_cont->words[6],"A");
   strcpy(test_cont->words[7],"==");
   strcpy(test_cont->words[8],"B");

   strcpy(test_cont->words[9],"{");
   strcpy(test_cont->words[10],"FD");
   strcpy(test_cont->words[11],"FD");
   strcpy(test_cont->words[12],"}");

   strcpy(test_cont->words[13],"ELSE");
   strcpy(test_cont->words[14],"{");
   strcpy(test_cont->words[15],"FD");

   strcpy(test_cont->words[16],"12");
   strcpy(test_cont->words[17],"}");

   strcpy(test_cont->words[18],"}");

   strcpy(test_cont->words[19],"ELSE");
   strcpy(test_cont->words[20],"{");
   strcpy(test_cont->words[21],"FD");
   strcpy(test_cont->words[22],"13");
   strcpy(test_cont->words[23],"}");

   test_cont->position=0;
   TEST_ASSERT_TRUE(!valid_flowstate(test_cont));


   test_cont->position=0;

   strcpy(test_cont->words[0],"IF");
   strcpy(test_cont->words[1],"A");
   strcpy(test_cont->words[2],"==");
   strcpy(test_cont->words[3],"A");

   strcpy(test_cont->words[4],"{");

   strcpy(test_cont->words[5],"IF");
   strcpy(test_cont->words[6],"A");
   strcpy(test_cont->words[7],"==");
   strcpy(test_cont->words[8],"B");

   strcpy(test_cont->words[9],"{");
   strcpy(test_cont->words[10],"FD");
   strcpy(test_cont->words[11],"A");
   strcpy(test_cont->words[12],"}");

   strcpy(test_cont->words[13],"ELSE");
   strcpy(test_cont->words[14],"{");
   strcpy(test_cont->words[15],"FD");

   strcpy(test_cont->words[16],"12");
   strcpy(test_cont->words[17],"}");

   strcpy(test_cont->words[18],"}");

   strcpy(test_cont->words[19],"ELSE");
   strcpy(test_cont->words[20],"{");

   strcpy(test_cont->words[21],"}");


   test_cont->position=0;
   TEST_ASSERT_TRUE(valid_flowstate(test_cont));

   test_cont->position=0;
   strcpy(test_cont->words[0],"IF");
   strcpy(test_cont->words[1],"A");
   strcpy(test_cont->words[2],"==");
   strcpy(test_cont->words[3],"A");

   strcpy(test_cont->words[4],"{");

   strcpy(test_cont->words[5],"IF");
   strcpy(test_cont->words[6],"A");
   strcpy(test_cont->words[7],"==");
   strcpy(test_cont->words[8],"B");

   strcpy(test_cont->words[9],"{");
   strcpy(test_cont->words[10],"FD");
   strcpy(test_cont->words[11],"A");
   strcpy(test_cont->words[12],"}");

   strcpy(test_cont->words[13],"ELSE");
   strcpy(test_cont->words[14],"{");
   strcpy(test_cont->words[15],"FD");

   strcpy(test_cont->words[16],"12");
   strcpy(test_cont->words[17],"}");

   strcpy(test_cont->words[18],"}");

   strcpy(test_cont->words[19],"ELSE");
   strcpy(test_cont->words[20],"{");
   strcpy(test_cont->words[21],"{");
   strcpy(test_cont->words[22],"}");

   test_cont->position=0;
   TEST_ASSERT_TRUE(!valid_flowstate(test_cont));

   test_cont->position=0;
   strcpy(test_cont->words[21],"{");

   TEST_ASSERT_TRUE(!valid_flowstate(test_cont));

   test_cont->position=0;
   strcpy(test_cont->words[0],"IF");
   strcpy(test_cont->words[1],"A");
   strcpy(test_cont->words[2],"==");
   strcpy(test_cont->words[3],"A");

   strcpy(test_cont->words[4],"{");

   strcpy(test_cont->words[5],"IF");
   strcpy(test_cont->words[6],"A");
   strcpy(test_cont->words[7],"==");
   strcpy(test_cont->words[8],"B");

   strcpy(test_cont->words[9],"{");
   strcpy(test_cont->words[10],"FD");
   strcpy(test_cont->words[11],"A");
   strcpy(test_cont->words[12],"}");

   strcpy(test_cont->words[13],"ELSE");
   strcpy(test_cont->words[14],"{");
   strcpy(test_cont->words[15],"FD");

   strcpy(test_cont->words[16],"12");
   strcpy(test_cont->words[17],"}");

   strcpy(test_cont->words[18],"}");

   strcpy(test_cont->words[19],"ELE");
   strcpy(test_cont->words[20],"{");
   strcpy(test_cont->words[21],"FD");
   strcpy(test_cont->words[22],"20");
   strcpy(test_cont->words[23],"}");
   /*although there is error we let it pass
   to be checked by next function - can see
   that position has stayed on the ELE spot*/
   TEST_ASSERT_TRUE(valid_flowstate(test_cont));
   TEST_ASSERT_TRUE(test_cont->position==19);

   free_word_cont(test_cont);
}

void test_comparisons(void)
{
   word_cont* test_cont;
   bool test_bool;
   line_cont* test_line_cont;
   TEST_ASSERT_TRUE(do_comparison_helper(10.0,10.0,equal));
   TEST_ASSERT_TRUE(do_comparison_helper(0,0,equal));
   TEST_ASSERT_TRUE(!do_comparison_helper(10.0,9.9999,equal));
   TEST_ASSERT_TRUE(do_comparison_helper(10.0,11.0,less));
   TEST_ASSERT_TRUE(!do_comparison_helper(11.0,11.0,less));
   TEST_ASSERT_TRUE(!do_comparison_helper(12.0,11.0,less));

   TEST_ASSERT_TRUE(!do_comparison_helper(10.0,11.0,greater));
   TEST_ASSERT_TRUE(!do_comparison_helper(11.0,11.0,greater));
   TEST_ASSERT_TRUE(do_comparison_helper(12.0,11.0,greater));


   TEST_ASSERT_TRUE(do_comparison_helper(-1.0,-11.0,greater));
   TEST_ASSERT_TRUE(do_comparison_helper(-10.0,-1.0,less));

   test_cont=init_word_cont();
   test_line_cont=init_line_cont();
   test_cont->position=0;
   strcpy(test_cont->words[0],"IF");
   strcpy(test_cont->words[1],"A");
   strcpy(test_cont->words[2],"==");
   strcpy(test_cont->words[3],"A");

   TEST_ASSERT_TRUE(!do_comparison(test_cont,&test_bool,test_line_cont));
   test_cont->position=0;
   strcpy(test_cont->words[0],"IF");
   strcpy(test_cont->words[1],"10");
   strcpy(test_cont->words[2],"==");
   strcpy(test_cont->words[3],"10");
   TEST_ASSERT_TRUE(do_comparison(test_cont,&test_bool,test_line_cont));
   TEST_ASSERT_TRUE(test_bool);

   test_cont->position=0;
   strcpy(test_cont->words[0],"IF");
   strcpy(test_cont->words[1],"10");
   strcpy(test_cont->words[2],"==");
   strcpy(test_cont->words[3],"1");
   TEST_ASSERT_TRUE(do_comparison(test_cont,&test_bool,test_line_cont));
   TEST_ASSERT_TRUE(!test_bool);

   test_cont->position=0;
   strcpy(test_cont->words[0],"IF");
   strcpy(test_cont->words[1],"10");
   strcpy(test_cont->words[2],"==");
   strcpy(test_cont->words[3],"-10");
   TEST_ASSERT_TRUE(do_comparison(test_cont,&test_bool,test_line_cont));
   TEST_ASSERT_TRUE(!test_bool);

   test_cont->position=0;
   strcpy(test_cont->words[0],"IF");
   strcpy(test_cont->words[1],"10");
   strcpy(test_cont->words[2],"==");
   strcpy(test_cont->words[3],"10.0001");
   TEST_ASSERT_TRUE(do_comparison(test_cont,&test_bool,test_line_cont));
   TEST_ASSERT_TRUE(!test_bool);

   test_cont->position=0;
   strcpy(test_cont->words[0],"SET");
   strcpy(test_cont->words[1],"A");
   strcpy(test_cont->words[2],":=");
   strcpy(test_cont->words[3],"10");
   strcpy(test_cont->words[4],";");
   strcpy(test_cont->words[5],"IF");
   strcpy(test_cont->words[6],"A");
   strcpy(test_cont->words[7],"==");
   strcpy(test_cont->words[8],"10");
   TEST_ASSERT_TRUE(run_set(test_cont,test_line_cont));
   TEST_ASSERT_TRUE(do_comparison(test_cont,&test_bool,test_line_cont));
   TEST_ASSERT_TRUE(test_bool);
   free_word_cont(test_cont);

   test_cont=init_word_cont();
   test_cont->position=0;
   strcpy(test_cont->words[0],"IF");
   strcpy(test_cont->words[1],"10");
   strcpy(test_cont->words[2],"<");
   strcpy(test_cont->words[3],"10.0001");
   TEST_ASSERT_TRUE(do_comparison(test_cont,&test_bool,test_line_cont));
   TEST_ASSERT_TRUE(test_bool);

   test_cont->position=0;
   strcpy(test_cont->words[0],"IF");
   strcpy(test_cont->words[1],"10");
   strcpy(test_cont->words[2],"<");
   strcpy(test_cont->words[3],"10.000");
   TEST_ASSERT_TRUE(do_comparison(test_cont,&test_bool,test_line_cont));
   TEST_ASSERT_TRUE(!test_bool);

   test_cont->position=0;
   strcpy(test_cont->words[0],"IF");
   strcpy(test_cont->words[1],"10");
   strcpy(test_cont->words[2],"<");
   strcpy(test_cont->words[3],"9.000");
   TEST_ASSERT_TRUE(do_comparison(test_cont,&test_bool,test_line_cont));
   TEST_ASSERT_TRUE(!test_bool);

   test_cont->position=0;
   strcpy(test_cont->words[0],"IF");
   strcpy(test_cont->words[1],"-10");
   strcpy(test_cont->words[2],"<");
   strcpy(test_cont->words[3],"-9");
   TEST_ASSERT_TRUE(do_comparison(test_cont,&test_bool,test_line_cont));
   TEST_ASSERT_TRUE(test_bool);


   test_cont->position=0;
   strcpy(test_cont->words[0],"IF");
   strcpy(test_cont->words[1],"-10");
   strcpy(test_cont->words[2],"<=");
   strcpy(test_cont->words[3],"-10");
   TEST_ASSERT_TRUE(do_comparison(test_cont,&test_bool,test_line_cont));
   TEST_ASSERT_TRUE(test_bool);

   test_cont->position=0;
   strcpy(test_cont->words[0],"IF");
   strcpy(test_cont->words[1],"-10");
   strcpy(test_cont->words[2],"<=");
   strcpy(test_cont->words[3],"-9");
   TEST_ASSERT_TRUE(do_comparison(test_cont,&test_bool,test_line_cont));
   TEST_ASSERT_TRUE(test_bool);

   test_cont->position=0;
   strcpy(test_cont->words[0],"IF");
   strcpy(test_cont->words[1],"-10");
   strcpy(test_cont->words[2],"<=");
   strcpy(test_cont->words[3],"-11");
   TEST_ASSERT_TRUE(do_comparison(test_cont,&test_bool,test_line_cont));
   TEST_ASSERT_TRUE(!test_bool);

   test_cont->position=0;
   strcpy(test_cont->words[0],"IF");
   strcpy(test_cont->words[1],"-10");
   strcpy(test_cont->words[2],">=");
   strcpy(test_cont->words[3],"-10");
   TEST_ASSERT_TRUE(do_comparison(test_cont,&test_bool,test_line_cont));
   TEST_ASSERT_TRUE(test_bool);

   test_cont->position=0;
   strcpy(test_cont->words[0],"IF");
   strcpy(test_cont->words[1],"-10");
   strcpy(test_cont->words[2],">=");
   strcpy(test_cont->words[3],"-9");
   TEST_ASSERT_TRUE(do_comparison(test_cont,&test_bool,test_line_cont));
   TEST_ASSERT_TRUE(!test_bool);

   test_cont->position=0;
   strcpy(test_cont->words[0],"IF");
   strcpy(test_cont->words[1],"-10");
   strcpy(test_cont->words[2],">=");
   strcpy(test_cont->words[3],"-11");
   TEST_ASSERT_TRUE(do_comparison(test_cont,&test_bool,test_line_cont));
   TEST_ASSERT_TRUE(test_bool);


   test_cont->position=0;
   strcpy(test_cont->words[0],"SET");
   strcpy(test_cont->words[1],"A");
   strcpy(test_cont->words[2],":=");
   strcpy(test_cont->words[3],"9");
   strcpy(test_cont->words[4],";");
   strcpy(test_cont->words[5],"IF");
   strcpy(test_cont->words[6],"A");
   strcpy(test_cont->words[7],"<");
   strcpy(test_cont->words[8],"10");
   TEST_ASSERT_TRUE(run_set(test_cont,test_line_cont));
   TEST_ASSERT_TRUE(do_comparison(test_cont,&test_bool,test_line_cont));
   TEST_ASSERT_TRUE(test_bool);
   free_word_cont(test_cont);

   test_cont=init_word_cont();
   test_cont->position=0;
   strcpy(test_cont->words[0],"IF");
   strcpy(test_cont->words[1],"10");
   strcpy(test_cont->words[2],">");
   strcpy(test_cont->words[3],"10.0001");
   TEST_ASSERT_TRUE(do_comparison(test_cont,&test_bool,test_line_cont));
   TEST_ASSERT_TRUE(!test_bool);

   test_cont->position=0;
   strcpy(test_cont->words[0],"IF");
   strcpy(test_cont->words[1],"15");
   strcpy(test_cont->words[2],">");
   strcpy(test_cont->words[3],"10.0001");
   TEST_ASSERT_TRUE(do_comparison(test_cont,&test_bool,test_line_cont));
   TEST_ASSERT_TRUE(test_bool);

   test_cont->position=0;
   strcpy(test_cont->words[0],"IF");
   strcpy(test_cont->words[1],"-15");
   strcpy(test_cont->words[2],">");
   strcpy(test_cont->words[3],"-16");
   TEST_ASSERT_TRUE(do_comparison(test_cont,&test_bool,test_line_cont));
   TEST_ASSERT_TRUE(test_bool);

   test_cont->position=0;
   strcpy(test_cont->words[0],"SET");
   strcpy(test_cont->words[1],"A");
   strcpy(test_cont->words[2],":=");
   strcpy(test_cont->words[3],"11");
   strcpy(test_cont->words[4],";");
   strcpy(test_cont->words[5],"IF");
   strcpy(test_cont->words[6],"A");
   strcpy(test_cont->words[7],">");
   strcpy(test_cont->words[8],"10");
   TEST_ASSERT_TRUE(run_set(test_cont,test_line_cont));
   TEST_ASSERT_TRUE(do_comparison(test_cont,&test_bool,test_line_cont));
   TEST_ASSERT_TRUE(test_bool);
   free_word_cont(test_cont);

   test_cont=init_word_cont();
   test_cont->position=0;
   strcpy(test_cont->words[0],"IF");
   strcpy(test_cont->words[1],"a");
   strcpy(test_cont->words[2],">");
   strcpy(test_cont->words[3],"10.0001");
   TEST_ASSERT_TRUE(!do_comparison(test_cont,&test_bool,test_line_cont));

   test_cont->position=0;
   strcpy(test_cont->words[0],"IF");
   strcpy(test_cont->words[1],"A");
   strcpy(test_cont->words[2],"=");
   strcpy(test_cont->words[3],"10.0001");
   TEST_ASSERT_TRUE(!do_comparison(test_cont,&test_bool,test_line_cont));

   test_cont->position=0;
   strcpy(test_cont->words[0],"IF");
   strcpy(test_cont->words[1],"A");
   strcpy(test_cont->words[2],"");
   strcpy(test_cont->words[3],"10.0001");
   TEST_ASSERT_TRUE(!do_comparison(test_cont,&test_bool,test_line_cont));
   free_word_cont(test_cont);
   free_line_cont(test_line_cont);
}

void test_else(void)
{
   word_cont* test_cont;
   line_cont* test_line_cont;
   test_line_cont=init_line_cont();
   test_cont=init_word_cont();

   strcpy(test_cont->words[0],"ELSE");
   strcpy(test_cont->words[1],"{");
   strcpy(test_cont->words[2],"FD");
   strcpy(test_cont->words[3],"10.0001");
   strcpy(test_cont->words[4],"}");

   TEST_ASSERT_TRUE(run_else(test_cont,test_line_cont));
   TEST_ASSERT_TRUE(test_line_cont->size==1);

   free_line_cont(test_line_cont);

   test_cont->position=0;
   test_line_cont=init_line_cont();
   strcpy(test_cont->words[0],"ELSE");
   strcpy(test_cont->words[1],"{");
   strcpy(test_cont->words[2],"DO");
   strcpy(test_cont->words[3],"A");
   strcpy(test_cont->words[4],"FROM");
   strcpy(test_cont->words[5],"1");
   strcpy(test_cont->words[6],"TO");
   strcpy(test_cont->words[7],"10");
   strcpy(test_cont->words[8],"{");
   strcpy(test_cont->words[9],"FD");
   strcpy(test_cont->words[10],"1");
   strcpy(test_cont->words[11],"}");
   strcpy(test_cont->words[12],"}");


   TEST_ASSERT_TRUE(run_else(test_cont,test_line_cont));
   TEST_ASSERT_TRUE(test_line_cont->size==10);

   free_line_cont(test_line_cont);

   test_cont->position=0;
   test_line_cont=init_line_cont();
   strcpy(test_cont->words[0],"ELSE");
   strcpy(test_cont->words[1],"{");
   strcpy(test_cont->words[2],"DO");
   strcpy(test_cont->words[3],"A");
   strcpy(test_cont->words[4],"FROM");
   strcpy(test_cont->words[5],"1");
   strcpy(test_cont->words[6],"TO");
   strcpy(test_cont->words[7],"");
   strcpy(test_cont->words[8],"{");
   strcpy(test_cont->words[9],"FD");
   strcpy(test_cont->words[10],"1");
   strcpy(test_cont->words[11],"}");
   strcpy(test_cont->words[12],"}");


   TEST_ASSERT_TRUE(!run_else(test_cont,test_line_cont));

   free_line_cont(test_line_cont);

   test_cont->position=0;
   test_line_cont=init_line_cont();
   strcpy(test_cont->words[0],"ELS");
   strcpy(test_cont->words[1],"{");
   strcpy(test_cont->words[2],"FD");
   strcpy(test_cont->words[3],"1");
   strcpy(test_cont->words[4],"}");


   TEST_ASSERT_TRUE(!run_else(test_cont,test_line_cont));
   free_line_cont(test_line_cont);
   free_word_cont(test_cont);

}

void test_false(void)
{
   word_cont* test_cont;
   line_cont* test_line_cont;
   test_line_cont=init_line_cont();
   test_cont=init_word_cont();
   strcpy(test_cont->words[0],"{");

   strcpy(test_cont->words[1],"DO");
   strcpy(test_cont->words[2],"A");
   strcpy(test_cont->words[3],"FROM");
   strcpy(test_cont->words[4],"1");
   strcpy(test_cont->words[5],"TO");
   strcpy(test_cont->words[6],"10");
   strcpy(test_cont->words[7],"{");
   strcpy(test_cont->words[8],"FD");
   strcpy(test_cont->words[9],"1");
   strcpy(test_cont->words[10],"}");

   strcpy(test_cont->words[11],"}");
   strcpy(test_cont->words[12],"ELSE");

   strcpy(test_cont->words[13],"{");
   strcpy(test_cont->words[14],"FD");
   strcpy(test_cont->words[15],"1");
   strcpy(test_cont->words[16],"}");

   TEST_ASSERT_TRUE(run_false(test_cont,test_line_cont));
   TEST_ASSERT_TRUE(test_line_cont->size==1);
   TEST_ASSERT_TRUE(test_cont->position==17);

   strcpy(test_cont->words[8],"");
   test_cont->position=0;
   TEST_ASSERT_TRUE(!run_false(test_cont,test_line_cont));

   strcpy(test_cont->words[8],"FD");
   strcpy(test_cont->words[14],"1");
   test_cont->position=0;
   TEST_ASSERT_TRUE(!run_false(test_cont,test_line_cont));
   free_line_cont(test_line_cont);


   test_line_cont=init_line_cont();
   test_cont->position=0;
   strcpy(test_cont->words[0],"{");

   strcpy(test_cont->words[1],"DO");
   strcpy(test_cont->words[2],"A");
   strcpy(test_cont->words[3],"FROM");
   strcpy(test_cont->words[4],"1");
   strcpy(test_cont->words[5],"TO");
   strcpy(test_cont->words[6],"10");
   strcpy(test_cont->words[7],"{");
   strcpy(test_cont->words[8],"FD");
   strcpy(test_cont->words[9],"1");
   strcpy(test_cont->words[10],"}");

   strcpy(test_cont->words[11],"}");
   strcpy(test_cont->words[12],"ESE");

   strcpy(test_cont->words[13],"{");
   strcpy(test_cont->words[14],"FD");
   strcpy(test_cont->words[15],"1");
   strcpy(test_cont->words[16],"}");

   TEST_ASSERT_TRUE(run_false(test_cont,test_line_cont));
   TEST_ASSERT_TRUE(test_cont->position==12);

   free_line_cont(test_line_cont);
   free_word_cont(test_cont);
}

void test_true(void)
{
   word_cont* test_cont;
   line_cont* test_line_cont;
   test_cont=init_word_cont();
   test_line_cont=init_line_cont();

   strcpy(test_cont->words[0],"{");

   strcpy(test_cont->words[1],"DO");
   strcpy(test_cont->words[2],"A");
   strcpy(test_cont->words[3],"FROM");
   strcpy(test_cont->words[4],"1");
   strcpy(test_cont->words[5],"TO");
   strcpy(test_cont->words[6],"10");
   strcpy(test_cont->words[7],"{");
   strcpy(test_cont->words[8],"FD");
   strcpy(test_cont->words[9],"1");
   strcpy(test_cont->words[10],"}");

   strcpy(test_cont->words[11],"}");
   strcpy(test_cont->words[12],"ELSE");

   strcpy(test_cont->words[13],"{");
   strcpy(test_cont->words[14],"FD");
   strcpy(test_cont->words[15],"1");
   strcpy(test_cont->words[16],"}");

   TEST_ASSERT_TRUE(run_true(test_cont,test_line_cont));
   TEST_ASSERT_TRUE(test_line_cont->size==10);
   TEST_ASSERT_TRUE(test_cont->position==17);


   test_cont=init_word_cont();
   test_line_cont=init_line_cont();

   strcpy(test_cont->words[0],"{");

   strcpy(test_cont->words[1],"SET");
   strcpy(test_cont->words[2],"A");
   strcpy(test_cont->words[3],":=");
   strcpy(test_cont->words[4],"1");
   strcpy(test_cont->words[5],";");

   strcpy(test_cont->words[6],"}");

   strcpy(test_cont->words[7],"ELSE");
   strcpy(test_cont->words[8],"{");
   strcpy(test_cont->words[9],"FD");
   strcpy(test_cont->words[10],"A");
   strcpy(test_cont->words[11],"}");
   TEST_ASSERT_TRUE(run_true(test_cont,test_line_cont));
   TEST_ASSERT_TRUE(test_line_cont->size==0);

   TEST_ASSERT_TRUE(test_cont->position==12);

   free_word_cont(test_cont);


   test_cont=init_word_cont();
   strcpy(test_cont->words[0],"{");

   strcpy(test_cont->words[1],"SET");
   strcpy(test_cont->words[2],"A");
   strcpy(test_cont->words[3],":=");
   strcpy(test_cont->words[4],"1");
   strcpy(test_cont->words[5],";");

   strcpy(test_cont->words[6],"}");

   strcpy(test_cont->words[7],"ELSE");
   strcpy(test_cont->words[8],"{");
   strcpy(test_cont->words[9],"FD");
   strcpy(test_cont->words[10],"A");
   strcpy(test_cont->words[11],"}");
   TEST_ASSERT_TRUE(!run_false(test_cont,test_line_cont));

   free_line_cont(test_line_cont);
   free_word_cont(test_cont);

   test_line_cont=init_line_cont();
   test_cont=init_word_cont();
   strcpy(test_cont->words[0],"{");
   strcpy(test_cont->words[1],"FD");
   strcpy(test_cont->words[2],"A");
   strcpy(test_cont->words[3],"}");
   strcpy(test_cont->words[4],"ELSE");
   strcpy(test_cont->words[5],"{");
   strcpy(test_cont->words[6],"SET");
   strcpy(test_cont->words[7],"A");
   strcpy(test_cont->words[8],":=");
   strcpy(test_cont->words[9],"1");
   strcpy(test_cont->words[10],";");
   strcpy(test_cont->words[11],"}");
   TEST_ASSERT_TRUE(!run_true(test_cont,test_line_cont));
   test_cont->position=0;
   TEST_ASSERT_TRUE(run_false(test_cont,test_line_cont));

   free_line_cont(test_line_cont);
   free_word_cont(test_cont);


   test_line_cont=init_line_cont();
   test_cont=init_word_cont();
   strcpy(test_cont->words[0],"{");
   strcpy(test_cont->words[1],"FD");
   strcpy(test_cont->words[2],"A");
   strcpy(test_cont->words[3],"}");
   strcpy(test_cont->words[4],"ELSE");
   strcpy(test_cont->words[5],"{");
   strcpy(test_cont->words[6],"SET");
   strcpy(test_cont->words[7],"A");
   strcpy(test_cont->words[8],":");
   strcpy(test_cont->words[9],"1");
   strcpy(test_cont->words[10],";");
   strcpy(test_cont->words[11],"}");
   TEST_ASSERT_TRUE(!run_true(test_cont,test_line_cont));
   free_line_cont(test_line_cont);
   free_word_cont(test_cont);
}

void test_run_flowstate(void)
{
   word_cont* test_cont;
   line_cont* test_line_cont;
   test_line_cont=init_line_cont();
   test_cont=init_word_cont();
   strcpy(test_cont->words[0],"IF");
   strcpy(test_cont->words[1],"10");
   strcpy(test_cont->words[2],"==");
   strcpy(test_cont->words[3],"11");
   strcpy(test_cont->words[4],"{");
   strcpy(test_cont->words[5],"FD");
   strcpy(test_cont->words[6],"10");
   strcpy(test_cont->words[7],"}");

   TEST_ASSERT_TRUE(run_flowstate(test_cont,test_line_cont));
   TEST_ASSERT_TRUE(test_line_cont->size==0);
   free_line_cont(test_line_cont);
   free_word_cont(test_cont);

   test_line_cont=init_line_cont();
   test_cont=init_word_cont();
   strcpy(test_cont->words[0],"IF");
   strcpy(test_cont->words[1],"11");
   strcpy(test_cont->words[2],"==");
   strcpy(test_cont->words[3],"11");
   strcpy(test_cont->words[4],"{");
   strcpy(test_cont->words[5],"FD");
   strcpy(test_cont->words[6],"10");
   strcpy(test_cont->words[7],"}");

   TEST_ASSERT_TRUE(run_flowstate(test_cont,test_line_cont));
   TEST_ASSERT_TRUE(test_line_cont->size==1);
   free_line_cont(test_line_cont);
   free_word_cont(test_cont);

   test_line_cont=init_line_cont();
   test_cont=init_word_cont();
   strcpy(test_cont->words[0],"IF");
   strcpy(test_cont->words[1],"11");
   strcpy(test_cont->words[2],"==");
   strcpy(test_cont->words[3],"11");
   strcpy(test_cont->words[4],"{");
   strcpy(test_cont->words[5],"FD");
   strcpy(test_cont->words[6],"10");
   strcpy(test_cont->words[7],"}");
   strcpy(test_cont->words[8],"ELSE");
   strcpy(test_cont->words[9],"{");
   strcpy(test_cont->words[10],"FD");
   strcpy(test_cont->words[11],"10");
   strcpy(test_cont->words[12],"FD");
   strcpy(test_cont->words[13],"10");
   strcpy(test_cont->words[14],"}");


   TEST_ASSERT_TRUE(run_flowstate(test_cont,test_line_cont));
   TEST_ASSERT_TRUE(test_line_cont->size==1);
   free_line_cont(test_line_cont);
   free_word_cont(test_cont);

   test_line_cont=init_line_cont();
   test_cont=init_word_cont();
   strcpy(test_cont->words[0],"IF");
   strcpy(test_cont->words[1],"-11");
   strcpy(test_cont->words[2],"==");
   strcpy(test_cont->words[3],"11");
   strcpy(test_cont->words[4],"{");
   strcpy(test_cont->words[5],"FD");
   strcpy(test_cont->words[6],"10");
   strcpy(test_cont->words[7],"}");
   strcpy(test_cont->words[8],"ELSE");
   strcpy(test_cont->words[9],"{");
   strcpy(test_cont->words[10],"FD");
   strcpy(test_cont->words[11],"10");
   strcpy(test_cont->words[12],"FD");
   strcpy(test_cont->words[13],"10");
   strcpy(test_cont->words[14],"}");


   TEST_ASSERT_TRUE(run_flowstate(test_cont,test_line_cont));
   TEST_ASSERT_TRUE(test_line_cont->size==2);
   free_line_cont(test_line_cont);
   free_word_cont(test_cont);


   test_line_cont=init_line_cont();
   test_cont=init_word_cont();
   strcpy(test_cont->words[0],"IF");
   strcpy(test_cont->words[1],"-11");
   strcpy(test_cont->words[2],"<");
   strcpy(test_cont->words[3],"11");
   strcpy(test_cont->words[4],"{");
   strcpy(test_cont->words[5],"FD");
   strcpy(test_cont->words[6],"10");
   strcpy(test_cont->words[7],"}");
   strcpy(test_cont->words[8],"ELSE");
   strcpy(test_cont->words[9],"{");
   strcpy(test_cont->words[10],"FD");
   strcpy(test_cont->words[11],"10");
   strcpy(test_cont->words[12],"FD");
   strcpy(test_cont->words[13],"10");
   strcpy(test_cont->words[14],"}");


   TEST_ASSERT_TRUE(run_flowstate(test_cont,test_line_cont));
   TEST_ASSERT_TRUE(test_line_cont->size==1);
   free_line_cont(test_line_cont);
   free_word_cont(test_cont);

   test_line_cont=init_line_cont();
   test_cont=init_word_cont();
   strcpy(test_cont->words[0],"IF");
   strcpy(test_cont->words[1],"-11");
   strcpy(test_cont->words[2],">");
   strcpy(test_cont->words[3],"11");
   strcpy(test_cont->words[4],"{");
   strcpy(test_cont->words[5],"FD");
   strcpy(test_cont->words[6],"10");
   strcpy(test_cont->words[7],"}");
   strcpy(test_cont->words[8],"ELSE");
   strcpy(test_cont->words[9],"{");
   strcpy(test_cont->words[10],"FD");
   strcpy(test_cont->words[11],"10");
   strcpy(test_cont->words[12],"FD");
   strcpy(test_cont->words[13],"10");
   strcpy(test_cont->words[14],"}");


   TEST_ASSERT_TRUE(run_flowstate(test_cont,test_line_cont));
   TEST_ASSERT_TRUE(test_line_cont->size==2);
   free_line_cont(test_line_cont);
   free_word_cont(test_cont);


   test_line_cont=init_line_cont();
   test_cont=init_word_cont();
   strcpy(test_cont->words[0],"IF");
   strcpy(test_cont->words[1],"-11");
   strcpy(test_cont->words[2],">");
   strcpy(test_cont->words[3],"11");
   strcpy(test_cont->words[4],"{");
   strcpy(test_cont->words[5],"SET");
   strcpy(test_cont->words[6],"A");
   strcpy(test_cont->words[7],":=");
   strcpy(test_cont->words[8],"10");
   strcpy(test_cont->words[9],";");
   strcpy(test_cont->words[10],"}");
   strcpy(test_cont->words[11],"ELSE");
   strcpy(test_cont->words[12],"{");
   strcpy(test_cont->words[13],"FD");
   strcpy(test_cont->words[14],"10");
   strcpy(test_cont->words[15],"}");

   TEST_ASSERT_TRUE(run_flowstate(test_cont,test_line_cont));
   TEST_ASSERT_TRUE(test_line_cont->size==1);
   TEST_ASSERT_TRUE(!test_cont->var_array[0]);
   free_line_cont(test_line_cont);
   free_word_cont(test_cont);

   test_line_cont=init_line_cont();
   test_cont=init_word_cont();
   strcpy(test_cont->words[0],"IF");
   strcpy(test_cont->words[1],"-11");
   strcpy(test_cont->words[2],">");
   strcpy(test_cont->words[3],"11");
   strcpy(test_cont->words[4],"{");
   strcpy(test_cont->words[5],"SET");
   strcpy(test_cont->words[6],"");
   strcpy(test_cont->words[7],":=");
   strcpy(test_cont->words[8],"10");
   strcpy(test_cont->words[9],";");
   strcpy(test_cont->words[10],"}");
   strcpy(test_cont->words[11],"ELSE");
   strcpy(test_cont->words[12],"{");
   strcpy(test_cont->words[13],"FD");
   strcpy(test_cont->words[14],"10");
   strcpy(test_cont->words[15],"}");

   TEST_ASSERT_TRUE(!run_flowstate(test_cont,test_line_cont));

   free_line_cont(test_line_cont);
   free_word_cont(test_cont);

   test_line_cont=init_line_cont();
   test_cont=init_word_cont();
   strcpy(test_cont->words[0],"F");
   strcpy(test_cont->words[1],"-11");
   strcpy(test_cont->words[2],">");
   strcpy(test_cont->words[3],"11");
   strcpy(test_cont->words[4],"{");
   strcpy(test_cont->words[5],"SET");
   strcpy(test_cont->words[6],"A");
   strcpy(test_cont->words[7],":=");
   strcpy(test_cont->words[8],"10");
   strcpy(test_cont->words[9],";");
   strcpy(test_cont->words[10],"}");
   strcpy(test_cont->words[11],"ELSE");
   strcpy(test_cont->words[12],"{");
   strcpy(test_cont->words[13],"FD");
   strcpy(test_cont->words[14],"10");
   strcpy(test_cont->words[15],"}");

   TEST_ASSERT_TRUE(!run_flowstate(test_cont,test_line_cont));

   free_line_cont(test_line_cont);
   free_word_cont(test_cont);

   test_line_cont=init_line_cont();
   test_cont=init_word_cont();
   strcpy(test_cont->words[0],"IF");
   strcpy(test_cont->words[1],"abc");
   strcpy(test_cont->words[2],">");
   strcpy(test_cont->words[3],"11");
   strcpy(test_cont->words[4],"{");
   strcpy(test_cont->words[5],"SET");
   strcpy(test_cont->words[6],"A");
   strcpy(test_cont->words[7],":=");
   strcpy(test_cont->words[8],"10");
   strcpy(test_cont->words[9],";");
   strcpy(test_cont->words[10],"}");
   strcpy(test_cont->words[11],"ELSE");
   strcpy(test_cont->words[12],"{");
   strcpy(test_cont->words[13],"FD");
   strcpy(test_cont->words[14],"10");
   strcpy(test_cont->words[15],"}");

   TEST_ASSERT_TRUE(!run_flowstate(test_cont,test_line_cont));

   free_line_cont(test_line_cont);
   free_word_cont(test_cont);

   test_line_cont=init_line_cont();
   test_cont=init_word_cont();
   strcpy(test_cont->words[0],"IF");
   strcpy(test_cont->words[1],"abc");
   strcpy(test_cont->words[2],">");
   strcpy(test_cont->words[3],"11");
   strcpy(test_cont->words[4],"{");
   strcpy(test_cont->words[5],"SET");
   strcpy(test_cont->words[6],"A");
   strcpy(test_cont->words[7],":=");
   strcpy(test_cont->words[8],"10");
   strcpy(test_cont->words[9],";");
   strcpy(test_cont->words[10],"}");
   strcpy(test_cont->words[11],"ELSE");
   strcpy(test_cont->words[12],"{");
   strcpy(test_cont->words[13],"FD");
   strcpy(test_cont->words[14],"10");
   strcpy(test_cont->words[15],"}");

   TEST_ASSERT_TRUE(!run_flowstate(test_cont,test_line_cont));

   free_line_cont(test_line_cont);
   free_word_cont(test_cont);


   test_line_cont=init_line_cont();
   test_cont=init_word_cont();
   strcpy(test_cont->words[0],"IF");
   strcpy(test_cont->words[1],"10");
   strcpy(test_cont->words[2],">");
   strcpy(test_cont->words[3],"11");

   strcpy(test_cont->words[4],"{");
   strcpy(test_cont->words[5],"IF");
   strcpy(test_cont->words[6],"7");
   strcpy(test_cont->words[7],"==");
   strcpy(test_cont->words[8],"1");

   strcpy(test_cont->words[9],"{");
   strcpy(test_cont->words[10],"FD");
   strcpy(test_cont->words[11],"30");
   strcpy(test_cont->words[12],"}");

   strcpy(test_cont->words[13],"ELSE");
   strcpy(test_cont->words[14],"{");
   strcpy(test_cont->words[15],"LT");
   strcpy(test_cont->words[16],"10");
   strcpy(test_cont->words[17],"}");
   strcpy(test_cont->words[18],"}");
   strcpy(test_cont->words[19],"ELSE");
   strcpy(test_cont->words[20],"{");
   strcpy(test_cont->words[21],"FD");
   strcpy(test_cont->words[22],"10");
   strcpy(test_cont->words[23],"}");

   TEST_ASSERT_TRUE(run_flowstate(test_cont,test_line_cont));
   TEST_ASSERT_TRUE(test_line_cont->size==1);
   free_line_cont(test_line_cont);
   free_word_cont(test_cont);
}

int main(void)
{
   UNITY_BEGIN();
   RUN_TEST(test_comparator);
   RUN_TEST(test_valid_expression);
   RUN_TEST(test_valid_flowstate);
   RUN_TEST(test_comparisons);
   RUN_TEST(test_else);
   RUN_TEST(test_false);
   RUN_TEST(test_true);
   RUN_TEST(test_run_flowstate);
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

   n_cont->stackptr=stack_init(sizeof(double));
   for(i=0;i<NUMVARS;i++)
   {
      n_cont->var_array[i]=NULL;
   }
   n_cont->err_message[0]='\0';
   n_cont->func_map=assoc_init();


   return n_cont;
}
