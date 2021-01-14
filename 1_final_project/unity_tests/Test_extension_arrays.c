#include "../src/unity.h"
#include "../extension_arrays.h"



#define MAXTESTCAP 100
#define MAXTESTLEN 70


word_cont* init_word_cont(void);


void setUp(void)
{

}

void tearDown(void)
{

}


void test_identifier(void)
{
   word_cont* test_cont;
   char test_name[MAXARRLEN];
   test_cont = init_word_cont();
   strcpy(test_cont->words[0],"a_myarr");
   TEST_ASSERT_TRUE(valid_arr_identifier(test_cont));
   TEST_ASSERT_TRUE(get_arr_identifier(test_cont,test_name));
   TEST_ASSERT_TRUE(strcmp(test_name,"a_myarr")==0);
   test_cont->position=0;

   strcpy(test_cont->words[0],"a_");
   TEST_ASSERT_TRUE(valid_arr_identifier(test_cont));
   TEST_ASSERT_TRUE(get_arr_identifier(test_cont,test_name));
   TEST_ASSERT_TRUE(strcmp(test_name,"a_")==0);
   test_cont->position=0;

   strcpy(test_cont->words[0],"a_bbbbbbbbbbbbbbbbbbbbbbbbbbbbbb");
   TEST_ASSERT_TRUE(valid_arr_identifier(test_cont));
   TEST_ASSERT_TRUE(get_arr_identifier(test_cont,test_name));
   TEST_ASSERT_TRUE(strcmp(test_name,"a_bbbbbbbbbbbbbbbbbbbbbbbbbbbbbb")==0);
   test_cont->position=0;


   strcpy(test_cont->words[0],"a_12334");
   TEST_ASSERT_TRUE(!valid_arr_identifier(test_cont));
   TEST_ASSERT_TRUE(!get_arr_identifier(test_cont,test_name));
   strcpy(test_cont->words[0],"abc");
   TEST_ASSERT_TRUE(!valid_arr_identifier(test_cont));
   TEST_ASSERT_TRUE(!get_arr_identifier(test_cont,test_name));
   strcpy(test_cont->words[0],"_abc");
   TEST_ASSERT_TRUE(!valid_arr_identifier(test_cont));
   TEST_ASSERT_TRUE(!get_arr_identifier(test_cont,test_name));
   strcpy(test_cont->words[0],"_");
   TEST_ASSERT_TRUE(!valid_arr_identifier(test_cont));
   TEST_ASSERT_TRUE(!get_arr_identifier(test_cont,test_name));
   strcpy(test_cont->words[0],"");
   TEST_ASSERT_TRUE(!valid_arr_identifier(test_cont));
   TEST_ASSERT_TRUE(!get_arr_identifier(test_cont,test_name));
   strcpy(test_cont->words[0],"a_myarrrrrrrrrrrrrrrrrrrrrrrrrrrrrramyarrrrrrrrrrrrrrrrrrrrrrrrrrrrrr");
   TEST_ASSERT_TRUE(!valid_arr_identifier(test_cont));
   TEST_ASSERT_TRUE(!get_arr_identifier(test_cont,test_name));
   free_word_cont(test_cont);
}

void test_init_arr(void)
{
   word_cont* test_cont;
   turt_arr* test_arr;
   line_cont* test_line_cont;
   test_cont = init_word_cont();

   strcpy(test_cont->words[0],"INITARR");
   strcpy(test_cont->words[1],"a_myarr");
   TEST_ASSERT_TRUE(valid_init_arr(test_cont));
   TEST_ASSERT_TRUE(test_cont->position==2);

   test_cont->position=0;
   strcpy(test_cont->words[0],"INITARR");
   strcpy(test_cont->words[1],"a_myarrrrrrrrrrrrrrrrrrrrrrrrrrrrrr");
   TEST_ASSERT_TRUE(valid_init_arr(test_cont));


   test_cont->position=0;
   strcpy(test_cont->words[0],"INITARR");
   strcpy(test_cont->words[1],"a_myarrrrrrrrrrrrrrrr_rrrrrrrrrrrrr");
   TEST_ASSERT_TRUE(!valid_init_arr(test_cont));

   test_cont->position=0;
   strcpy(test_cont->words[0],"INITARR");
   strcpy(test_cont->words[1],"a_myarrrrrrrr1rrrrrrrrrrrrr");
   TEST_ASSERT_TRUE(!valid_init_arr(test_cont));


   test_cont->position=0;
   strcpy(test_cont->words[0],"initarr");
   strcpy(test_cont->words[1],"a_myarrrrrrrrrrrrrrrrrrrrr");
   TEST_ASSERT_TRUE(!valid_init_arr(test_cont));

   test_cont->position=0;
   strcpy(test_cont->words[0],"");
   strcpy(test_cont->words[1],"a_myarrrrrrrrrrrrrrrrrrrrr");
   TEST_ASSERT_TRUE(!valid_init_arr(test_cont));


   free_word_cont(test_cont);

   test_cont = init_word_cont();

   strcpy(test_cont->words[0],"INITARR");
   strcpy(test_cont->words[1],"a_myarr");
   TEST_ASSERT_TRUE(run_init_arr(test_cont));
   test_arr=NULL;
   test_arr=assoc_lookup(test_cont->arr_map,"a_myarr");
   TEST_ASSERT_TRUE(test_arr);
   TEST_ASSERT_TRUE(test_cont->position==2);

   strcpy(test_cont->words[2],"INITARR");
   strcpy(test_cont->words[3],"a_myarragain");
   TEST_ASSERT_TRUE(run_init_arr(test_cont));
   /*checking we can overwrite without memory leaks*/
   strcpy(test_cont->words[4],"INITARR");
   strcpy(test_cont->words[5],"a_myarragain");
   TEST_ASSERT_TRUE(run_init_arr(test_cont));
   test_arr=assoc_lookup(test_cont->arr_map,"a_myarragain");
   TEST_ASSERT_TRUE(test_arr);
   free_word_cont(test_cont);

   test_cont = init_word_cont();

   strcpy(test_cont->words[0],"INITARR");
   strcpy(test_cont->words[1],"");
   strcpy(test_cont->words[2],"a_myarr");
   TEST_ASSERT_TRUE(!run_init_arr(test_cont));
   test_cont->position=0;

   strcpy(test_cont->words[0],"INITARR");
   strcpy(test_cont->words[1],"dhfn");
   TEST_ASSERT_TRUE(!run_init_arr(test_cont));
   test_cont->position=0;

   strcpy(test_cont->words[0],"a_arr");
   strcpy(test_cont->words[1],"INITARR");
   TEST_ASSERT_TRUE(!run_init_arr(test_cont));
   test_cont->position=0;

   free_word_cont(test_cont);


   test_cont = init_word_cont();
   test_line_cont= init_line_cont();
   strcpy(test_cont->words[0],"INITARR");
   strcpy(test_cont->words[1],"a_myarr");
   strcpy(test_cont->words[2],"APPEND");
   strcpy(test_cont->words[3],"a_myarr");
   strcpy(test_cont->words[4],"10");
   strcpy(test_cont->words[5],"16");
   strcpy(test_cont->words[6],"+");
   strcpy(test_cont->words[7],";");
   strcpy(test_cont->words[8],"INITARR");
   strcpy(test_cont->words[9],"a_myarr");

   TEST_ASSERT_TRUE(run_init_arr(test_cont));
   TEST_ASSERT_TRUE(run_append(test_cont,test_line_cont));
   TEST_ASSERT_TRUE(run_init_arr(test_cont));
   test_arr=assoc_lookup(test_cont->arr_map,"a_myarr");
   TEST_ASSERT_TRUE(test_arr->size==0);

   free_word_cont(test_cont);
   free_line_cont(test_line_cont);
}

void test_append(void)
{
   word_cont* test_cont;
   line_cont* test_line_cont;
   turt_arr* test_arr;
   test_cont = init_word_cont();
   strcpy(test_cont->words[0],"APPEND");
   strcpy(test_cont->words[1],"a_myarr");
   strcpy(test_cont->words[2],"23");
   strcpy(test_cont->words[3],";");
   TEST_ASSERT_TRUE(valid_append(test_cont));
   TEST_ASSERT_TRUE(test_cont->position==4);

   test_cont->position=0;
   strcpy(test_cont->words[0],"APPEND");
   strcpy(test_cont->words[1],"a_myarr");
   strcpy(test_cont->words[2],"A");
   strcpy(test_cont->words[3],";");
   TEST_ASSERT_TRUE(valid_append(test_cont));

   test_cont->position=0;
   strcpy(test_cont->words[0],"APPEND");
   strcpy(test_cont->words[1],"a_myarr");
   strcpy(test_cont->words[2],"A");
   strcpy(test_cont->words[3],"1");
   strcpy(test_cont->words[4],"/");
   strcpy(test_cont->words[5],";");
   TEST_ASSERT_TRUE(valid_append(test_cont));



   test_cont->position=0;
   strcpy(test_cont->words[0],"APPEND");
   strcpy(test_cont->words[1],"a_myarr");
   strcpy(test_cont->words[2],"A");
   strcpy(test_cont->words[3],"B");
   strcpy(test_cont->words[4],"/");
   strcpy(test_cont->words[5],";");
   TEST_ASSERT_TRUE(valid_append(test_cont));

   test_cont->position=0;
   strcpy(test_cont->words[0],"APPEND");
   strcpy(test_cont->words[1],"a_myarr");
   strcpy(test_cont->words[2],"A");
   strcpy(test_cont->words[3],"abc");
   strcpy(test_cont->words[4],"{");
   strcpy(test_cont->words[5],"}");
   strcpy(test_cont->words[6],"/");
   strcpy(test_cont->words[7],";");
   TEST_ASSERT_TRUE(valid_append(test_cont));


   test_cont->position=0;
   strcpy(test_cont->words[0],"APPEND");
   strcpy(test_cont->words[1],"a_myarr");
   strcpy(test_cont->words[2],"/");
   strcpy(test_cont->words[3],"abc");
   strcpy(test_cont->words[4],"{");
   strcpy(test_cont->words[5],"}");
   strcpy(test_cont->words[6],"/");
   strcpy(test_cont->words[7],";");
   TEST_ASSERT_TRUE(valid_append(test_cont));

   test_cont->position=0;
   strcpy(test_cont->words[0],"APPEND");
   strcpy(test_cont->words[1],"kancdj");
   strcpy(test_cont->words[2],"/");
   strcpy(test_cont->words[3],"abc");
   strcpy(test_cont->words[4],"{");
   strcpy(test_cont->words[5],"}");
   strcpy(test_cont->words[6],"/");
   strcpy(test_cont->words[7],";");
   TEST_ASSERT_TRUE(!valid_append(test_cont));

   test_cont->position=0;
   strcpy(test_cont->words[0],"append");
   strcpy(test_cont->words[1],"a_");
   strcpy(test_cont->words[2],"/");
   strcpy(test_cont->words[3],"abc");
   strcpy(test_cont->words[4],"{");
   strcpy(test_cont->words[5],"}");
   strcpy(test_cont->words[6],"/");
   strcpy(test_cont->words[7],";");
   TEST_ASSERT_TRUE(!valid_append(test_cont));


   test_cont->position=0;
   strcpy(test_cont->words[0],"APPEND");
   strcpy(test_cont->words[1],"a_");
   strcpy(test_cont->words[2],"");
   strcpy(test_cont->words[3],"abc");
   strcpy(test_cont->words[4],"{");
   strcpy(test_cont->words[5],"}");
   strcpy(test_cont->words[6],"/");
   strcpy(test_cont->words[7],";");
   TEST_ASSERT_TRUE(!valid_append(test_cont));

   test_cont->position=0;
   strcpy(test_cont->words[0],"append");
   strcpy(test_cont->words[1],"a_");
   strcpy(test_cont->words[2],"FD");
   strcpy(test_cont->words[3],"abc");
   strcpy(test_cont->words[4],"{");
   strcpy(test_cont->words[5],"}");
   strcpy(test_cont->words[6],"/");
   strcpy(test_cont->words[7],";");
   TEST_ASSERT_TRUE(!valid_append(test_cont));

   free_word_cont(test_cont);


   test_cont = init_word_cont();
   test_line_cont= init_line_cont();
   strcpy(test_cont->words[0],"INITARR");
   strcpy(test_cont->words[1],"a_myarr");
   strcpy(test_cont->words[2],"APPEND");
   strcpy(test_cont->words[3],"a_myarr");
   strcpy(test_cont->words[4],"10");
   strcpy(test_cont->words[5],"16");
   strcpy(test_cont->words[6],"+");
   strcpy(test_cont->words[7],";");
   TEST_ASSERT_TRUE(run_init_arr(test_cont));
   TEST_ASSERT_TRUE(run_append(test_cont,test_line_cont));
   test_arr=assoc_lookup(test_cont->arr_map,"a_myarr");
   TEST_ASSERT_EQUAL_DOUBLE(test_arr->head->data,26);

   free_word_cont(test_cont);

   test_cont = init_word_cont();

   strcpy(test_cont->words[0],"INITARR");
   strcpy(test_cont->words[1],"a_myarr");
   strcpy(test_cont->words[2],"APPEND");
   strcpy(test_cont->words[3],"a_myarr");
   strcpy(test_cont->words[4],"10");
   strcpy(test_cont->words[5],"16");
   strcpy(test_cont->words[6],"+");
   strcpy(test_cont->words[7],";");
   strcpy(test_cont->words[8],"APPEND");
   strcpy(test_cont->words[9],"a_myarr");
   strcpy(test_cont->words[10],"10");
   strcpy(test_cont->words[11],"1");
   strcpy(test_cont->words[12],"+");
   strcpy(test_cont->words[13],";");
   TEST_ASSERT_TRUE(run_init_arr(test_cont));
   TEST_ASSERT_TRUE(run_append(test_cont,test_line_cont));
   TEST_ASSERT_TRUE(run_append(test_cont,test_line_cont));
   test_arr=assoc_lookup(test_cont->arr_map,"a_myarr");
   TEST_ASSERT_EQUAL_DOUBLE(test_arr->head->next->data,11);

   free_word_cont(test_cont);


   test_cont = init_word_cont();
   strcpy(test_cont->words[0],"INITARR");
   strcpy(test_cont->words[1],"a_myarr");
   strcpy(test_cont->words[2],"APPEND");
   strcpy(test_cont->words[3],"a_myarr");
   strcpy(test_cont->words[4],"-10");
   strcpy(test_cont->words[5],"16");
   strcpy(test_cont->words[6],"+");
   strcpy(test_cont->words[7],";");
   TEST_ASSERT_TRUE(run_init_arr(test_cont));
   TEST_ASSERT_TRUE(run_append(test_cont,test_line_cont));
   test_arr=assoc_lookup(test_cont->arr_map,"a_myarr");
   TEST_ASSERT_EQUAL_DOUBLE(test_arr->head->data,6);

   free_word_cont(test_cont);


   test_cont = init_word_cont();
   strcpy(test_cont->words[0],"SET");
   strcpy(test_cont->words[1],"A");
   strcpy(test_cont->words[2],":=");
   strcpy(test_cont->words[3],"10");
   strcpy(test_cont->words[4],";");
   strcpy(test_cont->words[5],"INITARR");
   strcpy(test_cont->words[6],"a_myarr");
   strcpy(test_cont->words[7],"APPEND");
   strcpy(test_cont->words[8],"a_myarr");
   strcpy(test_cont->words[9],"A");
   strcpy(test_cont->words[10],"16");
   strcpy(test_cont->words[11],"+");
   strcpy(test_cont->words[12],";");
   TEST_ASSERT_TRUE(run_set(test_cont,test_line_cont));
   TEST_ASSERT_TRUE(run_init_arr(test_cont));
   TEST_ASSERT_TRUE(run_append(test_cont,test_line_cont));
   test_arr=assoc_lookup(test_cont->arr_map,"a_myarr");
   TEST_ASSERT_EQUAL_DOUBLE(test_arr->head->data,26);

   free_word_cont(test_cont);


   test_cont = init_word_cont();
   strcpy(test_cont->words[0],"SETFUNC");
   strcpy(test_cont->words[1],"a");
   strcpy(test_cont->words[2],"{");
   strcpy(test_cont->words[3],"}");
   strcpy(test_cont->words[4],"{");
   strcpy(test_cont->words[5],"RETURN");
   strcpy(test_cont->words[6],"10");
   strcpy(test_cont->words[7],"}");
   strcpy(test_cont->words[8],"INITARR");
   strcpy(test_cont->words[9],"a_myarr");
   strcpy(test_cont->words[10],"APPEND");
   strcpy(test_cont->words[11],"a_myarr");
   strcpy(test_cont->words[12],"a");
   strcpy(test_cont->words[13],"{");
   strcpy(test_cont->words[14],"}");
   strcpy(test_cont->words[15],"16");
   strcpy(test_cont->words[16],"+");
   strcpy(test_cont->words[17],";");
   TEST_ASSERT_TRUE(run_funcset(test_cont));
   TEST_ASSERT_TRUE(run_init_arr(test_cont));
   TEST_ASSERT_TRUE(run_append(test_cont,test_line_cont));
   test_arr=assoc_lookup(test_cont->arr_map,"a_myarr");
   TEST_ASSERT_EQUAL_DOUBLE(test_arr->head->data,26);
   free_word_cont(test_cont);


   test_cont = init_word_cont();

   strcpy(test_cont->words[0],"INITARR");
   strcpy(test_cont->words[1],"a_myarr");
   strcpy(test_cont->words[2],"APPEND");
   strcpy(test_cont->words[3],"a_");
   strcpy(test_cont->words[4],"10");
   strcpy(test_cont->words[5],"16");
   strcpy(test_cont->words[6],"+");
   strcpy(test_cont->words[7],";");
   TEST_ASSERT_TRUE(run_init_arr(test_cont));
   TEST_ASSERT_TRUE(!run_append(test_cont,test_line_cont));
   TEST_ASSERT_TRUE(strcmp(test_cont->err_message,"array not found - potentially not initalised yet")==0);
   free_word_cont(test_cont);

   test_cont = init_word_cont();
   strcpy(test_cont->words[0],"INITARR");
   strcpy(test_cont->words[1],"a_myarr");
   strcpy(test_cont->words[2],"APPEND");
   strcpy(test_cont->words[3],"a_myarr");
   strcpy(test_cont->words[4],"A");
   strcpy(test_cont->words[5],"16");
   strcpy(test_cont->words[6],"+");
   strcpy(test_cont->words[7],";");
   TEST_ASSERT_TRUE(run_init_arr(test_cont));
   TEST_ASSERT_TRUE(!run_append(test_cont,test_line_cont));
   free_word_cont(test_cont);


   test_cont = init_word_cont();
   strcpy(test_cont->words[0],"INITARR");
   strcpy(test_cont->words[1],"a_myarr");
   strcpy(test_cont->words[2],"APPEND");
   strcpy(test_cont->words[3],"a_myarr");
   strcpy(test_cont->words[4],"1");
   strcpy(test_cont->words[5],"16");
   strcpy(test_cont->words[6],"12");
   strcpy(test_cont->words[7],";");
   TEST_ASSERT_TRUE(run_init_arr(test_cont));
   TEST_ASSERT_TRUE(!run_append(test_cont,test_line_cont));
   free_word_cont(test_cont);
   free_line_cont(test_line_cont);
}


void test_valid_ind(void)
{
   word_cont* test_cont;
   line_cont* test_line_cont;
   int test_int;
   test_line_cont= init_line_cont();
   test_cont = init_word_cont();
   strcpy(test_cont->words[0],"1.1");
   TEST_ASSERT_TRUE(!get_valid_ind(test_cont,test_line_cont,&test_int));
   test_cont->position=0;


   strcpy(test_cont->words[0],"-1");
   TEST_ASSERT_TRUE(!get_valid_ind(test_cont,test_line_cont,&test_int));
   test_cont->position=0;


   strcpy(test_cont->words[0],"1");
   TEST_ASSERT_TRUE(get_valid_ind(test_cont,test_line_cont,&test_int));
   test_cont->position=0;
   TEST_ASSERT_TRUE(test_int==1);
   strcpy(test_cont->words[0],"0");
   TEST_ASSERT_TRUE(get_valid_ind(test_cont,test_line_cont,&test_int));
   test_cont->position=0;
   TEST_ASSERT_TRUE(test_int==0);
   strcpy(test_cont->words[0],"10000000");
   TEST_ASSERT_TRUE(get_valid_ind(test_cont,test_line_cont,&test_int));
   TEST_ASSERT_TRUE(test_int==10000000);

   free_word_cont(test_cont);
   free_line_cont(test_line_cont);
}


void test_change(void)
{

   word_cont* test_cont;
   line_cont* test_line_cont;
   turt_arr* test_arr;
   test_cont = init_word_cont();
   strcpy(test_cont->words[0],"CHANGE");
   strcpy(test_cont->words[1],"a_bc");
   strcpy(test_cont->words[2],"[");
   strcpy(test_cont->words[3],"1");
   strcpy(test_cont->words[4],"]");
   strcpy(test_cont->words[5],":=");
   strcpy(test_cont->words[6],"10");
   strcpy(test_cont->words[7],";");
   TEST_ASSERT_TRUE(valid_change(test_cont));
   test_cont->position=0;
   strcpy(test_cont->words[6],"A");
   TEST_ASSERT_TRUE(valid_change(test_cont));
   test_cont->position=0;
   strcpy(test_cont->words[6],"1.1");
   TEST_ASSERT_TRUE(valid_change(test_cont));
   test_cont->position=0;
   strcpy(test_cont->words[6],"1");
   strcpy(test_cont->words[7],"1");
   strcpy(test_cont->words[8],"2");
   strcpy(test_cont->words[9],";");
   TEST_ASSERT_TRUE(valid_change(test_cont));

   free_word_cont(test_cont);

   test_line_cont= init_line_cont();
   test_cont = init_word_cont();
   strcpy(test_cont->words[0],"INITARR");
   strcpy(test_cont->words[1],"a_myarr");
   strcpy(test_cont->words[2],"APPEND");
   strcpy(test_cont->words[3],"a_myarr");
   strcpy(test_cont->words[4],"1");
   strcpy(test_cont->words[5],";");
   strcpy(test_cont->words[6],"CHANGE");
   strcpy(test_cont->words[7],"a_myarr");
   strcpy(test_cont->words[8],"[");
   strcpy(test_cont->words[9],"0");
   strcpy(test_cont->words[10],"]");
   strcpy(test_cont->words[11],":=");
   strcpy(test_cont->words[12],"10");
   strcpy(test_cont->words[13],";");
   TEST_ASSERT_TRUE(run_init_arr(test_cont));
   TEST_ASSERT_TRUE(run_append(test_cont,test_line_cont));
   TEST_ASSERT_TRUE(run_change(test_cont,test_line_cont));
   TEST_ASSERT_TRUE(test_cont->position==14);
   test_arr=assoc_lookup(test_cont->arr_map,"a_myarr");
   TEST_ASSERT_EQUAL_DOUBLE(test_arr->head->data,10);

   free_word_cont(test_cont);
   free_line_cont(test_line_cont);


   test_line_cont= init_line_cont();
   test_cont = init_word_cont();
   strcpy(test_cont->words[0],"INITARR");
   strcpy(test_cont->words[1],"a_myarr");
   strcpy(test_cont->words[2],"APPEND");
   strcpy(test_cont->words[3],"a_myarr");
   strcpy(test_cont->words[4],"1");
   strcpy(test_cont->words[5],";");
   strcpy(test_cont->words[6],"CHANGE");
   strcpy(test_cont->words[7],"a_myarr");
   strcpy(test_cont->words[8],"[");
   strcpy(test_cont->words[9],"0");
   strcpy(test_cont->words[10],"]");
   strcpy(test_cont->words[11],":=");
   strcpy(test_cont->words[12],"10");
   strcpy(test_cont->words[13],";");
   TEST_ASSERT_TRUE(run_init_arr(test_cont));
   TEST_ASSERT_TRUE(run_append(test_cont,test_line_cont));
   TEST_ASSERT_TRUE(run_change(test_cont,test_line_cont));
   TEST_ASSERT_TRUE(test_cont->position==14);
   test_arr=assoc_lookup(test_cont->arr_map,"a_myarr");
   TEST_ASSERT_EQUAL_DOUBLE(test_arr->head->data,10);

   free_word_cont(test_cont);
   free_line_cont(test_line_cont);



   test_line_cont= init_line_cont();
   test_cont = init_word_cont();
   strcpy(test_cont->words[0],"INITARR");
   strcpy(test_cont->words[1],"a_myarr");
   strcpy(test_cont->words[2],"APPEND");
   strcpy(test_cont->words[3],"a_myarr");
   strcpy(test_cont->words[4],"1");
   strcpy(test_cont->words[5],";");
   strcpy(test_cont->words[6],"SET");
   strcpy(test_cont->words[7],"A");
   strcpy(test_cont->words[8],":=");
   strcpy(test_cont->words[9],"12");
   strcpy(test_cont->words[10],";");
   strcpy(test_cont->words[11],"CHANGE");
   strcpy(test_cont->words[12],"a_myarr");
   strcpy(test_cont->words[13],"[");
   strcpy(test_cont->words[14],"0");
   strcpy(test_cont->words[15],"]");
   strcpy(test_cont->words[16],":=");
   strcpy(test_cont->words[17],"A");
   strcpy(test_cont->words[18],"5");
   strcpy(test_cont->words[19],"+");
   strcpy(test_cont->words[20],";");
   TEST_ASSERT_TRUE(run_init_arr(test_cont));
   TEST_ASSERT_TRUE(run_append(test_cont,test_line_cont));
   TEST_ASSERT_TRUE(run_set(test_cont,test_line_cont));
   TEST_ASSERT_TRUE(run_change(test_cont,test_line_cont));
   TEST_ASSERT_TRUE(test_cont->position==21);
   test_arr=assoc_lookup(test_cont->arr_map,"a_myarr");
   TEST_ASSERT_EQUAL_DOUBLE(test_arr->head->data,17);

   free_word_cont(test_cont);
   free_line_cont(test_line_cont);

   test_line_cont= init_line_cont();
   test_cont = init_word_cont();
   strcpy(test_cont->words[0],"INITARR");
   strcpy(test_cont->words[1],"a_myarr");
   strcpy(test_cont->words[2],"APPEND");
   strcpy(test_cont->words[3],"a_myarr");
   strcpy(test_cont->words[4],"1");
   strcpy(test_cont->words[5],";");
   strcpy(test_cont->words[6],"SET");
   strcpy(test_cont->words[7],"A");
   strcpy(test_cont->words[8],":=");
   strcpy(test_cont->words[9],"0");
   strcpy(test_cont->words[10],";");
   strcpy(test_cont->words[11],"CHANGE");
   strcpy(test_cont->words[12],"a_myarr");
   strcpy(test_cont->words[13],"[");
   strcpy(test_cont->words[14],"A");
   strcpy(test_cont->words[15],"]");
   strcpy(test_cont->words[16],":=");
   strcpy(test_cont->words[17],"A");
   strcpy(test_cont->words[18],"5");
   strcpy(test_cont->words[19],"+");
   strcpy(test_cont->words[20],";");
   TEST_ASSERT_TRUE(run_init_arr(test_cont));
   TEST_ASSERT_TRUE(run_append(test_cont,test_line_cont));
   TEST_ASSERT_TRUE(run_set(test_cont,test_line_cont));
   TEST_ASSERT_TRUE(run_change(test_cont,test_line_cont));
   TEST_ASSERT_TRUE(test_cont->position==21);
   test_arr=assoc_lookup(test_cont->arr_map,"a_myarr");
   TEST_ASSERT_EQUAL_DOUBLE(test_arr->head->data,5);

   free_word_cont(test_cont);
   free_line_cont(test_line_cont);


   test_line_cont= init_line_cont();
   test_cont = init_word_cont();
   strcpy(test_cont->words[0],"INITARR");
   strcpy(test_cont->words[1],"a_myarr");
   strcpy(test_cont->words[2],"APPEND");
   strcpy(test_cont->words[3],"a_myarr");
   strcpy(test_cont->words[4],"1");
   strcpy(test_cont->words[5],";");
   strcpy(test_cont->words[6],"SETFUNC");
   strcpy(test_cont->words[7],"abc");
   strcpy(test_cont->words[8],"{");
   strcpy(test_cont->words[9],"}");
   strcpy(test_cont->words[10],"{");
   strcpy(test_cont->words[11],"RETURN");
   strcpy(test_cont->words[12],"-1.1");
   strcpy(test_cont->words[13],"}");
   strcpy(test_cont->words[14],"CHANGE");
   strcpy(test_cont->words[15],"a_myarr");
   strcpy(test_cont->words[16],"[");
   strcpy(test_cont->words[17],"0");
   strcpy(test_cont->words[18],"]");
   strcpy(test_cont->words[19],":=");
   strcpy(test_cont->words[20],"abc");
   strcpy(test_cont->words[21],"{");
   strcpy(test_cont->words[22],"}");
   strcpy(test_cont->words[23],";");

   TEST_ASSERT_TRUE(run_init_arr(test_cont));
   TEST_ASSERT_TRUE(run_append(test_cont,test_line_cont));
   TEST_ASSERT_TRUE(run_funcset(test_cont));
   TEST_ASSERT_TRUE(run_change(test_cont,test_line_cont));
   TEST_ASSERT_TRUE(test_cont->position==24);
   test_arr=assoc_lookup(test_cont->arr_map,"a_myarr");
   TEST_ASSERT_EQUAL_DOUBLE(test_arr->head->data,-1.1);

   free_word_cont(test_cont);
   free_line_cont(test_line_cont);

   test_line_cont= init_line_cont();
   test_cont = init_word_cont();
   strcpy(test_cont->words[0],"INITARR");
   strcpy(test_cont->words[1],"a_myarr");
   strcpy(test_cont->words[2],"APPEND");
   strcpy(test_cont->words[3],"a_myarr");
   strcpy(test_cont->words[4],"1");
   strcpy(test_cont->words[5],";");
   strcpy(test_cont->words[6],"SETFUNC");
   strcpy(test_cont->words[7],"abc");
   strcpy(test_cont->words[8],"{");
   strcpy(test_cont->words[9],"}");
   strcpy(test_cont->words[10],"{");
   strcpy(test_cont->words[11],"RETURN");
   strcpy(test_cont->words[12],"0");
   strcpy(test_cont->words[13],"}");
   strcpy(test_cont->words[14],"CHANGE");
   strcpy(test_cont->words[15],"a_myarr");
   strcpy(test_cont->words[16],"[");
   strcpy(test_cont->words[17],"abc");
   strcpy(test_cont->words[18],"{");
   strcpy(test_cont->words[19],"}");
   strcpy(test_cont->words[20],"]");
   strcpy(test_cont->words[21],":=");
   strcpy(test_cont->words[22],"abc");
   strcpy(test_cont->words[23],"{");
   strcpy(test_cont->words[24],"}");
   strcpy(test_cont->words[25],"10");
   strcpy(test_cont->words[26],"-");
   strcpy(test_cont->words[27],";");

   TEST_ASSERT_TRUE(run_init_arr(test_cont));
   TEST_ASSERT_TRUE(run_append(test_cont,test_line_cont));
   TEST_ASSERT_TRUE(run_funcset(test_cont));
   TEST_ASSERT_TRUE(run_change(test_cont,test_line_cont));
   TEST_ASSERT_TRUE(test_cont->position==28);
   test_arr=assoc_lookup(test_cont->arr_map,"a_myarr");
   TEST_ASSERT_EQUAL_DOUBLE(test_arr->head->data,-10);

   free_word_cont(test_cont);
   free_line_cont(test_line_cont);



   test_line_cont= init_line_cont();
   test_cont = init_word_cont();
   strcpy(test_cont->words[0],"INITARR");
   strcpy(test_cont->words[1],"a_myarr");
   strcpy(test_cont->words[2],"APPEND");
   strcpy(test_cont->words[3],"a_myarr");
   strcpy(test_cont->words[4],"1");
   strcpy(test_cont->words[5],";");
   strcpy(test_cont->words[6],"CHANGE");
   strcpy(test_cont->words[7],"a_wrong");
   strcpy(test_cont->words[8],"[");
   strcpy(test_cont->words[9],"0");
   strcpy(test_cont->words[10],"]");
   strcpy(test_cont->words[11],":=");
   strcpy(test_cont->words[12],"10");
   strcpy(test_cont->words[13],";");
   TEST_ASSERT_TRUE(run_init_arr(test_cont));
   TEST_ASSERT_TRUE(run_append(test_cont,test_line_cont));
   TEST_ASSERT_TRUE(!run_change(test_cont,test_line_cont));
   TEST_ASSERT_TRUE(strcmp(test_cont->err_message,"array not found - potentially not initalised yet")==0);


   free_word_cont(test_cont);
   free_line_cont(test_line_cont);


   test_line_cont= init_line_cont();
   test_cont = init_word_cont();
   strcpy(test_cont->words[0],"INITARR");
   strcpy(test_cont->words[1],"a_myarr");
   strcpy(test_cont->words[2],"APPEND");
   strcpy(test_cont->words[3],"a_myarr");
   strcpy(test_cont->words[4],"1");
   strcpy(test_cont->words[5],";");
   strcpy(test_cont->words[6],"CHANGE");
   strcpy(test_cont->words[7],"a_myarr");
   strcpy(test_cont->words[8],"[");
   strcpy(test_cont->words[9],"1000");
   strcpy(test_cont->words[10],"]");
   strcpy(test_cont->words[11],":=");
   strcpy(test_cont->words[12],"10");
   strcpy(test_cont->words[13],";");
   TEST_ASSERT_TRUE(run_init_arr(test_cont));
   TEST_ASSERT_TRUE(run_append(test_cont,test_line_cont));
   TEST_ASSERT_TRUE(!run_change(test_cont,test_line_cont));
   TEST_ASSERT_TRUE(strcmp(test_cont->err_message,"potentially trying to change a position greater than the size of array")==0);


   free_word_cont(test_cont);
   free_line_cont(test_line_cont);


   test_line_cont= init_line_cont();
   test_cont = init_word_cont();
   strcpy(test_cont->words[0],"INITARR");
   strcpy(test_cont->words[1],"a_myarr");
   strcpy(test_cont->words[2],"APPEND");
   strcpy(test_cont->words[3],"a_myarr");
   strcpy(test_cont->words[4],"1");
   strcpy(test_cont->words[5],";");
   strcpy(test_cont->words[6],"CHANGE");
   strcpy(test_cont->words[7],"a_myarr");
   strcpy(test_cont->words[8],"[");
   strcpy(test_cont->words[9],"-1");
   strcpy(test_cont->words[10],"]");
   strcpy(test_cont->words[11],":=");
   strcpy(test_cont->words[12],"10");
   strcpy(test_cont->words[13],";");
   TEST_ASSERT_TRUE(run_init_arr(test_cont));
   TEST_ASSERT_TRUE(run_append(test_cont,test_line_cont));
   TEST_ASSERT_TRUE(!run_change(test_cont,test_line_cont));
   TEST_ASSERT_TRUE(strcmp(test_cont->err_message,"negative indexes not allowed")==0);


   free_word_cont(test_cont);
   free_line_cont(test_line_cont);


   test_line_cont= init_line_cont();
   test_cont = init_word_cont();
   strcpy(test_cont->words[0],"INITARR");
   strcpy(test_cont->words[1],"a_myarr");
   strcpy(test_cont->words[2],"APPEND");
   strcpy(test_cont->words[3],"a_myarr");
   strcpy(test_cont->words[4],"1");
   strcpy(test_cont->words[5],";");
   strcpy(test_cont->words[6],"CHANGE");
   strcpy(test_cont->words[7],"a_myarr");
   strcpy(test_cont->words[8],"[");
   strcpy(test_cont->words[9],"1.1");
   strcpy(test_cont->words[10],"]");
   strcpy(test_cont->words[11],":=");
   strcpy(test_cont->words[12],"10");
   strcpy(test_cont->words[13],";");
   TEST_ASSERT_TRUE(run_init_arr(test_cont));
   TEST_ASSERT_TRUE(run_append(test_cont,test_line_cont));
   TEST_ASSERT_TRUE(!run_change(test_cont,test_line_cont));
   TEST_ASSERT_TRUE(strcmp(test_cont->err_message,"decimal indexes not allowed")==0);


   free_word_cont(test_cont);
   free_line_cont(test_line_cont);

}


void test_delete(void)
{
   word_cont* test_cont;
   line_cont* test_line_cont;
   turt_arr* test_arr;
   test_cont = init_word_cont();
   strcpy(test_cont->words[0],"DEL");
   strcpy(test_cont->words[1],"a_myarr");
   strcpy(test_cont->words[2],"[");
   strcpy(test_cont->words[3],"10");
   strcpy(test_cont->words[4],"]");
   TEST_ASSERT_TRUE(valid_delete_arr_val(test_cont));
   TEST_ASSERT_TRUE(test_cont->position==5);
   test_cont->position=0;

   strcpy(test_cont->words[0],"DEL");
   strcpy(test_cont->words[1],"a_myarr");
   strcpy(test_cont->words[2],"[");
   strcpy(test_cont->words[3],"abc");
   strcpy(test_cont->words[4],"{");
   strcpy(test_cont->words[5],"}");
   strcpy(test_cont->words[6],"]");
   TEST_ASSERT_TRUE(valid_delete_arr_val(test_cont));
   TEST_ASSERT_TRUE(test_cont->position==7);
   test_cont->position=0;


   strcpy(test_cont->words[0],"DEL");
   strcpy(test_cont->words[1],"a_myarr");
   strcpy(test_cont->words[2],"[");
   strcpy(test_cont->words[3],"A");
   strcpy(test_cont->words[4],"]");

   TEST_ASSERT_TRUE(valid_delete_arr_val(test_cont));
   TEST_ASSERT_TRUE(test_cont->position==5);
   test_cont->position=0;

   strcpy(test_cont->words[0],"DEL");
   strcpy(test_cont->words[1],"abc");
   strcpy(test_cont->words[2],"[");
   strcpy(test_cont->words[3],"A");
   strcpy(test_cont->words[4],"]");

   TEST_ASSERT_TRUE(!valid_delete_arr_val(test_cont));
   test_cont->position=0;

   strcpy(test_cont->words[0],"DEL");
   strcpy(test_cont->words[1],"a_myarr");
   strcpy(test_cont->words[2],"[");
   strcpy(test_cont->words[3],"a_myarr");
   strcpy(test_cont->words[4],"]");
   TEST_ASSERT_TRUE(!valid_delete_arr_val(test_cont));
   test_cont->position=0;
   free_word_cont(test_cont);

   test_cont = init_word_cont();
   test_line_cont= init_line_cont();
   strcpy(test_cont->words[0],"INITARR");
   strcpy(test_cont->words[1],"a_myarr");
   strcpy(test_cont->words[2],"APPEND");
   strcpy(test_cont->words[3],"a_myarr");
   strcpy(test_cont->words[4],"-10");
   strcpy(test_cont->words[5],"16");
   strcpy(test_cont->words[6],"+");
   strcpy(test_cont->words[7],";");
   strcpy(test_cont->words[8],"DEL");
   strcpy(test_cont->words[9],"a_myarr");
   strcpy(test_cont->words[10],"[");
   strcpy(test_cont->words[11],"0");
   strcpy(test_cont->words[12],"]");
   TEST_ASSERT_TRUE(run_init_arr(test_cont));
   TEST_ASSERT_TRUE(run_append(test_cont,test_line_cont));
   test_arr=assoc_lookup(test_cont->arr_map,"a_myarr");
   TEST_ASSERT_EQUAL_DOUBLE(test_arr->head->data,6);
   TEST_ASSERT_TRUE(run_delete_arr_val(test_cont,test_line_cont));
   test_arr=assoc_lookup(test_cont->arr_map,"a_myarr");
   TEST_ASSERT_TRUE(test_arr->size==0);
   free_word_cont(test_cont);
   free_line_cont(test_line_cont);

   test_cont = init_word_cont();
   test_line_cont= init_line_cont();
   strcpy(test_cont->words[0],"INITARR");
   strcpy(test_cont->words[1],"a_myarr");
   strcpy(test_cont->words[2],"APPEND");
   strcpy(test_cont->words[3],"a_myarr");
   strcpy(test_cont->words[4],"-10");
   strcpy(test_cont->words[5],"16");
   strcpy(test_cont->words[6],"+");
   strcpy(test_cont->words[7],";");
   strcpy(test_cont->words[8],"APPEND");
   strcpy(test_cont->words[9],"a_myarr");
   strcpy(test_cont->words[10],"-10");
   strcpy(test_cont->words[11],"16");
   strcpy(test_cont->words[12],"*");
   strcpy(test_cont->words[13],";");
   strcpy(test_cont->words[14],"DEL");
   strcpy(test_cont->words[15],"a_myarr");
   strcpy(test_cont->words[16],"[");
   strcpy(test_cont->words[17],"0");
   strcpy(test_cont->words[18],"]");
   TEST_ASSERT_TRUE(run_init_arr(test_cont));
   TEST_ASSERT_TRUE(run_append(test_cont,test_line_cont));
   TEST_ASSERT_TRUE(run_append(test_cont,test_line_cont));
   test_arr=assoc_lookup(test_cont->arr_map,"a_myarr");
   TEST_ASSERT_EQUAL_DOUBLE(test_arr->head->data,6);
   TEST_ASSERT_TRUE(run_delete_arr_val(test_cont,test_line_cont));
   test_arr=assoc_lookup(test_cont->arr_map,"a_myarr");
   TEST_ASSERT_TRUE(test_arr->size==1);
   TEST_ASSERT_EQUAL_DOUBLE(test_arr->head->data,-160);
   free_word_cont(test_cont);
   free_line_cont(test_line_cont);


   test_cont = init_word_cont();
   test_line_cont= init_line_cont();
   strcpy(test_cont->words[0],"INITARR");
   strcpy(test_cont->words[1],"a_myarr");
   strcpy(test_cont->words[2],"APPEND");
   strcpy(test_cont->words[3],"a_myarr");
   strcpy(test_cont->words[4],"-10");
   strcpy(test_cont->words[5],"16");
   strcpy(test_cont->words[6],"+");
   strcpy(test_cont->words[7],";");
   strcpy(test_cont->words[8],"APPEND");
   strcpy(test_cont->words[9],"a_myarr");
   strcpy(test_cont->words[10],"-10");
   strcpy(test_cont->words[11],"16");
   strcpy(test_cont->words[12],"*");
   strcpy(test_cont->words[13],";");
   strcpy(test_cont->words[14],"DEL");
   strcpy(test_cont->words[15],"a_myarr");
   strcpy(test_cont->words[16],"[");
   strcpy(test_cont->words[17],"1");
   strcpy(test_cont->words[18],"]");
   TEST_ASSERT_TRUE(run_init_arr(test_cont));
   TEST_ASSERT_TRUE(run_append(test_cont,test_line_cont));
   TEST_ASSERT_TRUE(run_append(test_cont,test_line_cont));
   test_arr=assoc_lookup(test_cont->arr_map,"a_myarr");
   TEST_ASSERT_EQUAL_DOUBLE(test_arr->head->data,6);
   TEST_ASSERT_TRUE(run_delete_arr_val(test_cont,test_line_cont));
   test_arr=assoc_lookup(test_cont->arr_map,"a_myarr");
   TEST_ASSERT_TRUE(test_arr->size==1);
   TEST_ASSERT_EQUAL_DOUBLE(test_arr->head->data,6);
   free_word_cont(test_cont);
   free_line_cont(test_line_cont);


   test_cont = init_word_cont();
   test_line_cont= init_line_cont();
   strcpy(test_cont->words[0],"INITARR");
   strcpy(test_cont->words[1],"a_myarr");
   strcpy(test_cont->words[2],"APPEND");
   strcpy(test_cont->words[3],"a_myarr");
   strcpy(test_cont->words[4],"-10");
   strcpy(test_cont->words[5],"16");
   strcpy(test_cont->words[6],"+");
   strcpy(test_cont->words[7],";");
   strcpy(test_cont->words[8],"APPEND");
   strcpy(test_cont->words[9],"a_myarr");
   strcpy(test_cont->words[10],"-10");
   strcpy(test_cont->words[11],"16");
   strcpy(test_cont->words[12],"*");
   strcpy(test_cont->words[13],";");
   strcpy(test_cont->words[14],"APPEND");
   strcpy(test_cont->words[15],"a_myarr");
   strcpy(test_cont->words[16],"-10");
   strcpy(test_cont->words[17],"0");
   strcpy(test_cont->words[18],"+");
   strcpy(test_cont->words[19],";");
   strcpy(test_cont->words[20],"DEL");
   strcpy(test_cont->words[21],"a_myarr");
   strcpy(test_cont->words[22],"[");
   strcpy(test_cont->words[23],"1");
   strcpy(test_cont->words[24],"]");
   TEST_ASSERT_TRUE(run_init_arr(test_cont));
   TEST_ASSERT_TRUE(run_append(test_cont,test_line_cont));
   TEST_ASSERT_TRUE(run_append(test_cont,test_line_cont));
   TEST_ASSERT_TRUE(run_append(test_cont,test_line_cont));
   test_arr=assoc_lookup(test_cont->arr_map,"a_myarr");
   TEST_ASSERT_EQUAL_DOUBLE(test_arr->head->data,6);
   TEST_ASSERT_TRUE(run_delete_arr_val(test_cont,test_line_cont));
   test_arr=assoc_lookup(test_cont->arr_map,"a_myarr");
   TEST_ASSERT_TRUE(test_arr->size==2);
   TEST_ASSERT_EQUAL_DOUBLE(test_arr->head->data,6);
   TEST_ASSERT_EQUAL_DOUBLE(test_arr->head->next->data,-10);
   free_word_cont(test_cont);
   free_line_cont(test_line_cont);


   test_cont = init_word_cont();
   test_line_cont= init_line_cont();
   strcpy(test_cont->words[0],"INITARR");
   strcpy(test_cont->words[1],"a_myarr");
   strcpy(test_cont->words[2],"APPEND");
   strcpy(test_cont->words[3],"a_myarr");
   strcpy(test_cont->words[4],"-10");
   strcpy(test_cont->words[5],"16");
   strcpy(test_cont->words[6],"+");
   strcpy(test_cont->words[7],";");
   strcpy(test_cont->words[8],"DEL");
   strcpy(test_cont->words[9],"a_myarr");
   strcpy(test_cont->words[10],"[");
   strcpy(test_cont->words[11],"20");
   strcpy(test_cont->words[12],"]");
   TEST_ASSERT_TRUE(run_init_arr(test_cont));
   TEST_ASSERT_TRUE(run_append(test_cont,test_line_cont));
   test_arr=assoc_lookup(test_cont->arr_map,"a_myarr");
   TEST_ASSERT_EQUAL_DOUBLE(test_arr->head->data,6);
   TEST_ASSERT_TRUE(run_delete_arr_val(test_cont,test_line_cont));
   free_word_cont(test_cont);
   free_line_cont(test_line_cont);


   test_cont = init_word_cont();
   test_line_cont= init_line_cont();
   strcpy(test_cont->words[0],"INITARR");
   strcpy(test_cont->words[1],"a_myarr");
   strcpy(test_cont->words[2],"APPEND");
   strcpy(test_cont->words[3],"a_myarr");
   strcpy(test_cont->words[4],"-10");
   strcpy(test_cont->words[5],"16");
   strcpy(test_cont->words[6],"+");
   strcpy(test_cont->words[7],";");
   strcpy(test_cont->words[8],"SET");
   strcpy(test_cont->words[9],"A");
   strcpy(test_cont->words[10],":=");
   strcpy(test_cont->words[11],"0");
   strcpy(test_cont->words[12],";");
   strcpy(test_cont->words[13],"DEL");
   strcpy(test_cont->words[14],"a_myarr");
   strcpy(test_cont->words[15],"[");
   strcpy(test_cont->words[16],"A");
   strcpy(test_cont->words[17],"]");
   TEST_ASSERT_TRUE(run_init_arr(test_cont));
   TEST_ASSERT_TRUE(run_append(test_cont,test_line_cont));
   TEST_ASSERT_TRUE(run_set(test_cont,test_line_cont));
   test_arr=assoc_lookup(test_cont->arr_map,"a_myarr");
   TEST_ASSERT_EQUAL_DOUBLE(test_arr->head->data,6);
   TEST_ASSERT_TRUE(run_delete_arr_val(test_cont,test_line_cont));
   test_arr=assoc_lookup(test_cont->arr_map,"a_myarr");
   TEST_ASSERT_TRUE(test_arr->size==0);
   free_word_cont(test_cont);
   free_line_cont(test_line_cont);



   test_cont = init_word_cont();
   test_line_cont= init_line_cont();
   strcpy(test_cont->words[0],"INITARR");
   strcpy(test_cont->words[1],"a_myarr");
   strcpy(test_cont->words[2],"APPEND");
   strcpy(test_cont->words[3],"a_myarr");
   strcpy(test_cont->words[4],"-10");
   strcpy(test_cont->words[5],"16");
   strcpy(test_cont->words[6],"+");
   strcpy(test_cont->words[7],";");
   strcpy(test_cont->words[8],"SETFUNC");
   strcpy(test_cont->words[9],"a");
   strcpy(test_cont->words[10],"{");
   strcpy(test_cont->words[11],"}");
   strcpy(test_cont->words[12],"{");
   strcpy(test_cont->words[13],"RETURN");
   strcpy(test_cont->words[14],"0");
   strcpy(test_cont->words[15],"}");
   strcpy(test_cont->words[16],"DEL");
   strcpy(test_cont->words[17],"a_myarr");
   strcpy(test_cont->words[18],"[");
   strcpy(test_cont->words[19],"a");
   strcpy(test_cont->words[20],"{");
   strcpy(test_cont->words[21],"}");
   strcpy(test_cont->words[22],"]");
   TEST_ASSERT_TRUE(run_init_arr(test_cont));
   TEST_ASSERT_TRUE(run_append(test_cont,test_line_cont));
   TEST_ASSERT_TRUE(run_funcset(test_cont));
   test_arr=assoc_lookup(test_cont->arr_map,"a_myarr");
   TEST_ASSERT_EQUAL_DOUBLE(test_arr->head->data,6);
   TEST_ASSERT_TRUE(run_delete_arr_val(test_cont,test_line_cont));
   test_arr=assoc_lookup(test_cont->arr_map,"a_myarr");
   TEST_ASSERT_TRUE(test_arr->size==0);
   free_word_cont(test_cont);
   free_line_cont(test_line_cont);

   test_cont = init_word_cont();
   test_line_cont= init_line_cont();
   strcpy(test_cont->words[0],"INITARR");
   strcpy(test_cont->words[1],"a_myarr");
   strcpy(test_cont->words[2],"APPEND");
   strcpy(test_cont->words[3],"a_myarr");
   strcpy(test_cont->words[4],"-10");
   strcpy(test_cont->words[5],"16");
   strcpy(test_cont->words[6],"+");
   strcpy(test_cont->words[7],";");
   strcpy(test_cont->words[8],"DEL");
   strcpy(test_cont->words[9],"a_my");
   strcpy(test_cont->words[10],"[");
   strcpy(test_cont->words[11],"0");
   strcpy(test_cont->words[12],"]");
   TEST_ASSERT_TRUE(run_init_arr(test_cont));
   TEST_ASSERT_TRUE(run_append(test_cont,test_line_cont));

   TEST_ASSERT_TRUE(!run_delete_arr_val(test_cont,test_line_cont));
   TEST_ASSERT_TRUE(strcmp(test_cont->err_message,"array not found - potentially not initalised yet")==0);
   free_word_cont(test_cont);
   free_line_cont(test_line_cont);

   test_cont = init_word_cont();
   test_line_cont= init_line_cont();
   strcpy(test_cont->words[0],"INITARR");
   strcpy(test_cont->words[1],"a_myarr");
   strcpy(test_cont->words[2],"APPEND");
   strcpy(test_cont->words[3],"a_myarr");
   strcpy(test_cont->words[4],"-10");
   strcpy(test_cont->words[5],"16");
   strcpy(test_cont->words[6],"+");
   strcpy(test_cont->words[7],";");
   strcpy(test_cont->words[8],"DEL");
   strcpy(test_cont->words[9],"a_my");
   strcpy(test_cont->words[10],"[");
   strcpy(test_cont->words[11],"0");
   strcpy(test_cont->words[12],"]");
   TEST_ASSERT_TRUE(run_init_arr(test_cont));
   TEST_ASSERT_TRUE(run_append(test_cont,test_line_cont));

   TEST_ASSERT_FALSE(run_delete_arr_val(test_cont,test_line_cont));
   TEST_ASSERT_TRUE(strcmp(test_cont->err_message,"array not found - potentially not initalised yet")==0);
   free_word_cont(test_cont);
   free_line_cont(test_line_cont);


   test_cont = init_word_cont();
   test_line_cont= init_line_cont();
   strcpy(test_cont->words[0],"INITARR");
   strcpy(test_cont->words[1],"a_myarr");
   strcpy(test_cont->words[2],"APPEND");
   strcpy(test_cont->words[3],"a_myarr");
   strcpy(test_cont->words[4],"-10");
   strcpy(test_cont->words[5],"16");
   strcpy(test_cont->words[6],"+");
   strcpy(test_cont->words[7],";");
   strcpy(test_cont->words[8],"DEL");
   strcpy(test_cont->words[9],"a_my");
   strcpy(test_cont->words[10],"[");
   strcpy(test_cont->words[11],"0");
   strcpy(test_cont->words[12],"]");
   TEST_ASSERT_TRUE(run_init_arr(test_cont));
   TEST_ASSERT_TRUE(run_append(test_cont,test_line_cont));

   TEST_ASSERT_FALSE(run_delete_arr_val(test_cont,test_line_cont));
   TEST_ASSERT_TRUE(strcmp(test_cont->err_message,"array not found - potentially not initalised yet")==0);
   free_word_cont(test_cont);
   free_line_cont(test_line_cont);


   test_cont = init_word_cont();
   test_line_cont= init_line_cont();
   strcpy(test_cont->words[0],"INITARR");
   strcpy(test_cont->words[1],"a_myarr");
   strcpy(test_cont->words[2],"APPEND");
   strcpy(test_cont->words[3],"a_myarr");
   strcpy(test_cont->words[4],"-10");
   strcpy(test_cont->words[5],"16");
   strcpy(test_cont->words[6],"+");
   strcpy(test_cont->words[7],";");
   strcpy(test_cont->words[8],"DEL");
   strcpy(test_cont->words[9],"a_myarr");
   strcpy(test_cont->words[10],"[");
   strcpy(test_cont->words[11],"-1");
   strcpy(test_cont->words[12],"]");
   TEST_ASSERT_TRUE(run_init_arr(test_cont));
   TEST_ASSERT_TRUE(run_append(test_cont,test_line_cont));

   TEST_ASSERT_FALSE(run_delete_arr_val(test_cont,test_line_cont));
   TEST_ASSERT_TRUE(strcmp(test_cont->err_message,"negative indexes not allowed")==0);
   free_word_cont(test_cont);
   free_line_cont(test_line_cont);


   test_cont = init_word_cont();
   test_line_cont= init_line_cont();
   strcpy(test_cont->words[0],"INITARR");
   strcpy(test_cont->words[1],"a_myarr");
   strcpy(test_cont->words[2],"APPEND");
   strcpy(test_cont->words[3],"a_myarr");
   strcpy(test_cont->words[4],"-10");
   strcpy(test_cont->words[5],"16");
   strcpy(test_cont->words[6],"+");
   strcpy(test_cont->words[7],";");
   strcpy(test_cont->words[8],"DEL");
   strcpy(test_cont->words[9],"a_myarr");
   strcpy(test_cont->words[10],"[");
   strcpy(test_cont->words[11],"1.1");
   strcpy(test_cont->words[12],"]");
   TEST_ASSERT_TRUE(run_init_arr(test_cont));
   TEST_ASSERT_TRUE(run_append(test_cont,test_line_cont));

   TEST_ASSERT_FALSE(run_delete_arr_val(test_cont,test_line_cont));
   TEST_ASSERT_TRUE(strcmp(test_cont->err_message,"decimal indexes not allowed")==0);
   free_word_cont(test_cont);
   free_line_cont(test_line_cont);
}

void test_access(void)
{
   word_cont* test_cont;
   line_cont* test_line_cont;
   double test_double;
   test_cont = init_word_cont();
   strcpy(test_cont->words[0],"a_myarr");
   strcpy(test_cont->words[1],"[");
   strcpy(test_cont->words[2],"1");
   strcpy(test_cont->words[3],"]");
   TEST_ASSERT_TRUE(valid_access_val(test_cont));
   TEST_ASSERT_TRUE(test_cont->position==4);
   test_cont->position=0;

   strcpy(test_cont->words[0],"a_myarr");
   strcpy(test_cont->words[1],"[");
   strcpy(test_cont->words[2],"100000");
   strcpy(test_cont->words[3],"]");
   TEST_ASSERT_TRUE(valid_access_val(test_cont));
   TEST_ASSERT_TRUE(test_cont->position==4);
   test_cont->position=0;

   strcpy(test_cont->words[0],"a_myarr");
   strcpy(test_cont->words[1],"[");
   strcpy(test_cont->words[2],"abc");
   strcpy(test_cont->words[3],"{");
   strcpy(test_cont->words[4],"}");
   strcpy(test_cont->words[5],"]");
   TEST_ASSERT_TRUE(valid_access_val(test_cont));
   TEST_ASSERT_TRUE(test_cont->position==6);
   test_cont->position=0;

   strcpy(test_cont->words[0],"a_myarr");
   strcpy(test_cont->words[1],"[");
   strcpy(test_cont->words[2],"A");
   strcpy(test_cont->words[3],"]");
   TEST_ASSERT_TRUE(valid_access_val(test_cont));
   TEST_ASSERT_TRUE(test_cont->position==4);
   test_cont->position=0;

   strcpy(test_cont->words[0],"a_myarr");
   strcpy(test_cont->words[1],"[");
   strcpy(test_cont->words[2],"FD");
   strcpy(test_cont->words[3],"]");
   TEST_ASSERT_FALSE(valid_access_val(test_cont));
   test_cont->position=0;

   strcpy(test_cont->words[0],"a_myarr");
   strcpy(test_cont->words[1],"[");
   strcpy(test_cont->words[2],"]");
   TEST_ASSERT_FALSE(valid_access_val(test_cont));
   test_cont->position=0;
   free_word_cont(test_cont);

   test_cont = init_word_cont();
   test_line_cont= init_line_cont();
   strcpy(test_cont->words[0],"INITARR");
   strcpy(test_cont->words[1],"a_myarr");
   strcpy(test_cont->words[2],"APPEND");
   strcpy(test_cont->words[3],"a_myarr");
   strcpy(test_cont->words[4],"-10");
   strcpy(test_cont->words[5],"16");
   strcpy(test_cont->words[6],"+");
   strcpy(test_cont->words[7],";");
   strcpy(test_cont->words[8],"a_myarr");
   strcpy(test_cont->words[9],"[");
   strcpy(test_cont->words[10],"0");
   strcpy(test_cont->words[11],"]");
   TEST_ASSERT_TRUE(run_init_arr(test_cont));
   TEST_ASSERT_TRUE(run_append(test_cont,test_line_cont));
   TEST_ASSERT_TRUE(run_access_val(test_cont,test_line_cont,&test_double));
   TEST_ASSERT_TRUE(test_cont->position==12);
   TEST_ASSERT_EQUAL_DOUBLE(test_double,6);

   free_word_cont(test_cont);
   free_line_cont(test_line_cont);


   test_cont = init_word_cont();
   test_line_cont= init_line_cont();
   strcpy(test_cont->words[0],"INITARR");
   strcpy(test_cont->words[1],"a_myarr");
   strcpy(test_cont->words[2],"APPEND");
   strcpy(test_cont->words[3],"a_myarr");
   strcpy(test_cont->words[4],"-10");
   strcpy(test_cont->words[5],"16");
   strcpy(test_cont->words[6],"+");
   strcpy(test_cont->words[7],";");
   strcpy(test_cont->words[8],"APPEND");
   strcpy(test_cont->words[9],"a_myarr");
   strcpy(test_cont->words[10],"-10");
   strcpy(test_cont->words[11],"1");
   strcpy(test_cont->words[12],"+");
   strcpy(test_cont->words[13],";");
   strcpy(test_cont->words[14],"a_myarr");
   strcpy(test_cont->words[15],"[");
   strcpy(test_cont->words[16],"1");
   strcpy(test_cont->words[17],"]");
   TEST_ASSERT_TRUE(run_init_arr(test_cont));
   TEST_ASSERT_TRUE(run_append(test_cont,test_line_cont));
   TEST_ASSERT_TRUE(run_append(test_cont,test_line_cont));
   TEST_ASSERT_TRUE(run_access_val(test_cont,test_line_cont,&test_double));
   TEST_ASSERT_EQUAL_DOUBLE(test_double,-9);

   free_word_cont(test_cont);
   free_line_cont(test_line_cont);

   test_cont = init_word_cont();
   test_line_cont= init_line_cont();
   strcpy(test_cont->words[0],"INITARR");
   strcpy(test_cont->words[1],"a_myarr");
   strcpy(test_cont->words[2],"APPEND");
   strcpy(test_cont->words[3],"a_myarr");
   strcpy(test_cont->words[4],"-10");
   strcpy(test_cont->words[5],"16");
   strcpy(test_cont->words[6],"+");
   strcpy(test_cont->words[7],";");

   strcpy(test_cont->words[8],"SET");
   strcpy(test_cont->words[9],"A");
   strcpy(test_cont->words[10],":=");
   strcpy(test_cont->words[11],"0");
   strcpy(test_cont->words[12],";");

   strcpy(test_cont->words[13],"a_myarr");
   strcpy(test_cont->words[14],"[");
   strcpy(test_cont->words[15],"A");
   strcpy(test_cont->words[16],"]");
   TEST_ASSERT_TRUE(run_init_arr(test_cont));
   TEST_ASSERT_TRUE(run_append(test_cont,test_line_cont));
   TEST_ASSERT_TRUE(run_set(test_cont,test_line_cont));
   TEST_ASSERT_TRUE(run_access_val(test_cont,test_line_cont,&test_double));
   TEST_ASSERT_TRUE(test_cont->position==17);
   TEST_ASSERT_EQUAL_DOUBLE(test_double,6);

   free_word_cont(test_cont);
   free_line_cont(test_line_cont);


   test_cont = init_word_cont();
   test_line_cont= init_line_cont();
   strcpy(test_cont->words[0],"INITARR");
   strcpy(test_cont->words[1],"a_myarr");
   strcpy(test_cont->words[2],"APPEND");
   strcpy(test_cont->words[3],"a_myarr");
   strcpy(test_cont->words[4],"-10");
   strcpy(test_cont->words[5],"16");
   strcpy(test_cont->words[6],"+");
   strcpy(test_cont->words[7],";");

   strcpy(test_cont->words[8],"SETFUNC");
   strcpy(test_cont->words[9],"abc");
   strcpy(test_cont->words[10],"{");
   strcpy(test_cont->words[11],"}");
   strcpy(test_cont->words[12],"{");
   strcpy(test_cont->words[13],"RETURN");
   strcpy(test_cont->words[14],"0");
   strcpy(test_cont->words[15],"}");

   strcpy(test_cont->words[16],"a_myarr");
   strcpy(test_cont->words[17],"[");
   strcpy(test_cont->words[18],"abc");
   strcpy(test_cont->words[19],"{");
   strcpy(test_cont->words[20],"}");
   strcpy(test_cont->words[21],"]");
   TEST_ASSERT_TRUE(run_init_arr(test_cont));
   TEST_ASSERT_TRUE(run_append(test_cont,test_line_cont));
   TEST_ASSERT_TRUE(run_funcset(test_cont));
   TEST_ASSERT_TRUE(run_access_val(test_cont,test_line_cont,&test_double));
   TEST_ASSERT_TRUE(test_cont->position==22);
   TEST_ASSERT_EQUAL_DOUBLE(test_double,6);

   free_word_cont(test_cont);
   free_line_cont(test_line_cont);


   test_cont = init_word_cont();
   test_line_cont= init_line_cont();
   strcpy(test_cont->words[0],"INITARR");
   strcpy(test_cont->words[1],"a_myarr");
   strcpy(test_cont->words[2],"APPEND");
   strcpy(test_cont->words[3],"a_myarr");
   strcpy(test_cont->words[4],"-10");
   strcpy(test_cont->words[5],"16");
   strcpy(test_cont->words[6],"+");
   strcpy(test_cont->words[7],";");
   strcpy(test_cont->words[8],"a_myarr");
   strcpy(test_cont->words[9],"[");
   strcpy(test_cont->words[10],"A");
   strcpy(test_cont->words[11],"]");
   TEST_ASSERT_TRUE(run_init_arr(test_cont));
   TEST_ASSERT_TRUE(run_append(test_cont,test_line_cont));
   TEST_ASSERT_FALSE(run_access_val(test_cont,test_line_cont,&test_double));


   free_word_cont(test_cont);
   free_line_cont(test_line_cont);


   test_cont = init_word_cont();
   test_line_cont= init_line_cont();
   strcpy(test_cont->words[0],"INITARR");
   strcpy(test_cont->words[1],"a_myarr");
   strcpy(test_cont->words[2],"APPEND");
   strcpy(test_cont->words[3],"a_myarr");
   strcpy(test_cont->words[4],"-10");
   strcpy(test_cont->words[5],"16");
   strcpy(test_cont->words[6],"+");
   strcpy(test_cont->words[7],";");
   strcpy(test_cont->words[8],"a_myarr");
   strcpy(test_cont->words[9],"[");
   strcpy(test_cont->words[10],"-1");
   strcpy(test_cont->words[11],"]");
   TEST_ASSERT_TRUE(run_init_arr(test_cont));
   TEST_ASSERT_TRUE(run_append(test_cont,test_line_cont));
   TEST_ASSERT_FALSE(run_access_val(test_cont,test_line_cont,&test_double));


   free_word_cont(test_cont);
   free_line_cont(test_line_cont);


   test_cont = init_word_cont();
   test_line_cont= init_line_cont();
   strcpy(test_cont->words[0],"INITARR");
   strcpy(test_cont->words[1],"a_myarr");
   strcpy(test_cont->words[2],"APPEND");
   strcpy(test_cont->words[3],"a_myarr");
   strcpy(test_cont->words[4],"-10");
   strcpy(test_cont->words[5],"16");
   strcpy(test_cont->words[6],"+");
   strcpy(test_cont->words[7],";");
   strcpy(test_cont->words[8],"a_myarr");
   strcpy(test_cont->words[9],"[");
   strcpy(test_cont->words[10],"abc");
   strcpy(test_cont->words[11],"{");
   strcpy(test_cont->words[12],"}");
   strcpy(test_cont->words[13],"]");
   TEST_ASSERT_TRUE(run_init_arr(test_cont));
   TEST_ASSERT_TRUE(run_append(test_cont,test_line_cont));
   TEST_ASSERT_FALSE(run_access_val(test_cont,test_line_cont,&test_double));


   free_word_cont(test_cont);
   free_line_cont(test_line_cont);


   test_cont = init_word_cont();
   test_line_cont= init_line_cont();
   strcpy(test_cont->words[0],"INITARR");
   strcpy(test_cont->words[1],"a_myarr");
   strcpy(test_cont->words[2],"APPEND");
   strcpy(test_cont->words[3],"a_myarr");
   strcpy(test_cont->words[4],"-10");
   strcpy(test_cont->words[5],"16");
   strcpy(test_cont->words[6],"+");
   strcpy(test_cont->words[7],";");
   strcpy(test_cont->words[8],"a_myarr");
   strcpy(test_cont->words[9],"[");
   strcpy(test_cont->words[10],"100000");
   strcpy(test_cont->words[11],"]");
   TEST_ASSERT_TRUE(run_init_arr(test_cont));
   TEST_ASSERT_TRUE(run_append(test_cont,test_line_cont));
   TEST_ASSERT_FALSE(run_access_val(test_cont,test_line_cont,&test_double));


   free_word_cont(test_cont);
   free_line_cont(test_line_cont);


   test_cont = init_word_cont();
   test_line_cont= init_line_cont();
   strcpy(test_cont->words[0],"INITARR");
   strcpy(test_cont->words[1],"a_myarr");
   strcpy(test_cont->words[2],"APPEND");
   strcpy(test_cont->words[3],"a_myarr");
   strcpy(test_cont->words[4],"-10");
   strcpy(test_cont->words[5],"16");
   strcpy(test_cont->words[6],"+");
   strcpy(test_cont->words[7],";");
   strcpy(test_cont->words[8],"a_myarr");
   strcpy(test_cont->words[9],"[");
   strcpy(test_cont->words[10],"1.1");
   strcpy(test_cont->words[11],"]");
   TEST_ASSERT_TRUE(run_init_arr(test_cont));
   TEST_ASSERT_TRUE(run_append(test_cont,test_line_cont));
   TEST_ASSERT_FALSE(run_access_val(test_cont,test_line_cont,&test_double));


   free_word_cont(test_cont);
   free_line_cont(test_line_cont);
}


void test_do_arr(void)
{
   word_cont* test_cont;
   line_cont* test_line_cont;
   turt_arr* test_arr;
   test_cont = init_word_cont();
   test_line_cont= init_line_cont();
   strcpy(test_cont->words[0],"INITARR");
   strcpy(test_cont->words[1],"a_myarr");
   strcpy(test_cont->words[2],"APPEND");
   strcpy(test_cont->words[3],"a_myarr");
   strcpy(test_cont->words[4],"5");
   strcpy(test_cont->words[5],";");

   strcpy(test_cont->words[6],"DO");
   strcpy(test_cont->words[7],"A");
   strcpy(test_cont->words[8],"FROM");
   strcpy(test_cont->words[9],"a_myarr");
   strcpy(test_cont->words[10],"[");
   strcpy(test_cont->words[11],"0");
   strcpy(test_cont->words[12],"]");
   strcpy(test_cont->words[13],"TO");
   strcpy(test_cont->words[14],"10");
   strcpy(test_cont->words[15],"{");
   strcpy(test_cont->words[16],"FD");

   strcpy(test_cont->words[17],"10");
   strcpy(test_cont->words[18],"}");
   TEST_ASSERT_TRUE(run_init_arr(test_cont));
   TEST_ASSERT_TRUE(run_append(test_cont,test_line_cont));
   TEST_ASSERT_TRUE(run_do(test_cont,test_line_cont));
   TEST_ASSERT_TRUE(test_line_cont->size==6);
   free_word_cont(test_cont);
   free_line_cont(test_line_cont);


   test_cont = init_word_cont();
   test_line_cont= init_line_cont();
   strcpy(test_cont->words[0],"INITARR");
   strcpy(test_cont->words[1],"a_myarr");
   strcpy(test_cont->words[2],"APPEND");
   strcpy(test_cont->words[3],"a_myarr");
   strcpy(test_cont->words[4],"5");
   strcpy(test_cont->words[5],";");
   strcpy(test_cont->words[6],"APPEND");
   strcpy(test_cont->words[7],"a_myarr");
   strcpy(test_cont->words[8],"10");
   strcpy(test_cont->words[9],";");

   strcpy(test_cont->words[10],"DO");
   strcpy(test_cont->words[11],"A");
   strcpy(test_cont->words[12],"FROM");
   strcpy(test_cont->words[13],"a_myarr");
   strcpy(test_cont->words[14],"[");
   strcpy(test_cont->words[15],"0");
   strcpy(test_cont->words[16],"]");
   strcpy(test_cont->words[17],"TO");
   strcpy(test_cont->words[18],"a_myarr");
   strcpy(test_cont->words[19],"[");
   strcpy(test_cont->words[20],"1");
   strcpy(test_cont->words[21],"]");
   strcpy(test_cont->words[22],"{");
   strcpy(test_cont->words[23],"FD");
   strcpy(test_cont->words[24],"10");
   strcpy(test_cont->words[25],"}");
   TEST_ASSERT_TRUE(run_init_arr(test_cont));
   TEST_ASSERT_TRUE(run_append(test_cont,test_line_cont));
   TEST_ASSERT_TRUE(run_append(test_cont,test_line_cont));
   TEST_ASSERT_TRUE(run_do(test_cont,test_line_cont));
   TEST_ASSERT_TRUE(test_line_cont->size==6);
   free_word_cont(test_cont);
   free_line_cont(test_line_cont);


   test_cont = init_word_cont();
   test_line_cont= init_line_cont();
   strcpy(test_cont->words[0],"INITARR");
   strcpy(test_cont->words[1],"a_myarr");
   strcpy(test_cont->words[2],"DO");
   strcpy(test_cont->words[3],"A");
   strcpy(test_cont->words[4],"FROM");
   strcpy(test_cont->words[5],"0");
   strcpy(test_cont->words[6],"TO");
   strcpy(test_cont->words[7],"100");
   strcpy(test_cont->words[8],"{");
   strcpy(test_cont->words[9],"APPEND");

   strcpy(test_cont->words[10],"a_myarr");
   strcpy(test_cont->words[11],"A");
   strcpy(test_cont->words[12],";");
   strcpy(test_cont->words[13],"}");
   TEST_ASSERT_TRUE(run_init_arr(test_cont));
   TEST_ASSERT_TRUE(run_do(test_cont,test_line_cont));
   test_arr=assoc_lookup(test_cont->arr_map,"a_myarr");
   TEST_ASSERT_TRUE(test_arr->size==101);
   free_word_cont(test_cont);
   free_line_cont(test_line_cont);

   test_cont = init_word_cont();
   test_line_cont= init_line_cont();
   strcpy(test_cont->words[0],"DO");
   strcpy(test_cont->words[1],"A");
   strcpy(test_cont->words[2],"FROM");
   strcpy(test_cont->words[3],"0");
   strcpy(test_cont->words[4],"TO");
   strcpy(test_cont->words[5],"100");
   strcpy(test_cont->words[6],"{");
   strcpy(test_cont->words[7],"INITARR");

   strcpy(test_cont->words[8],"a_myarr");
   strcpy(test_cont->words[9],"APPEND");
   strcpy(test_cont->words[10],"a_myarr");
   strcpy(test_cont->words[11],"A");
   strcpy(test_cont->words[12],";");
   strcpy(test_cont->words[13],"}");

   TEST_ASSERT_TRUE(run_do(test_cont,test_line_cont));
   test_arr=assoc_lookup(test_cont->arr_map,"a_myarr");
   free_word_cont(test_cont);
   free_line_cont(test_line_cont);


   /*once you set the bounds for a loop cant change
   so we prevent infinite loops and weird behaviour
   when values in a list are deleted*/
   test_cont = init_word_cont();
   test_line_cont= init_line_cont();
   strcpy(test_cont->words[0],"INITARR");
   strcpy(test_cont->words[1],"a_myarr");
   strcpy(test_cont->words[2],"APPEND");
   strcpy(test_cont->words[3],"a_myarr");
   strcpy(test_cont->words[4],"10");
   strcpy(test_cont->words[5],";");
   strcpy(test_cont->words[6],"DO");
   strcpy(test_cont->words[7],"A");
   strcpy(test_cont->words[8],"FROM");
   strcpy(test_cont->words[9],"5");
   strcpy(test_cont->words[10],"TO");
   strcpy(test_cont->words[11],"a_myarr");
   strcpy(test_cont->words[12],"[");
   strcpy(test_cont->words[13],"0");
   strcpy(test_cont->words[14],"]");
   strcpy(test_cont->words[15],"{");
   strcpy(test_cont->words[16],"FD");
   strcpy(test_cont->words[17],"10");
   strcpy(test_cont->words[18],"CHANGE");
   strcpy(test_cont->words[19],"a_myarr");
   strcpy(test_cont->words[20],"[");
   strcpy(test_cont->words[21],"0");
   strcpy(test_cont->words[22],"]");
   strcpy(test_cont->words[23],":=");
   strcpy(test_cont->words[24],"1");
   strcpy(test_cont->words[25],";");
   strcpy(test_cont->words[26],"}");
   TEST_ASSERT_TRUE(run_init_arr(test_cont));
   TEST_ASSERT_TRUE(run_append(test_cont,test_line_cont));
   TEST_ASSERT_TRUE(run_do(test_cont,test_line_cont));
   TEST_ASSERT_TRUE(test_line_cont->size==6);
   free_word_cont(test_cont);
   free_line_cont(test_line_cont);


   test_cont = init_word_cont();
   test_line_cont= init_line_cont();
   strcpy(test_cont->words[0],"INITARR");
   strcpy(test_cont->words[1],"a_myarr");
   strcpy(test_cont->words[2],"APPEND");
   strcpy(test_cont->words[3],"a_myarr");
   strcpy(test_cont->words[4],"10");
   strcpy(test_cont->words[5],";");
   strcpy(test_cont->words[6],"DO");
   strcpy(test_cont->words[7],"A");
   strcpy(test_cont->words[8],"FROM");
   strcpy(test_cont->words[9],"5");
   strcpy(test_cont->words[10],"TO");
   strcpy(test_cont->words[11],"a_myarr");
   strcpy(test_cont->words[12],"[");
   strcpy(test_cont->words[13],"0");
   strcpy(test_cont->words[14],"]");
   strcpy(test_cont->words[15],"{");
   strcpy(test_cont->words[16],"FD");
   strcpy(test_cont->words[17],"10");
   strcpy(test_cont->words[18],"DEL");
   strcpy(test_cont->words[19],"a_myarr");
   strcpy(test_cont->words[20],"[");
   strcpy(test_cont->words[21],"0");
   strcpy(test_cont->words[22],"]");
   strcpy(test_cont->words[23],"}");

   TEST_ASSERT_TRUE(run_init_arr(test_cont));
   TEST_ASSERT_TRUE(run_append(test_cont,test_line_cont));
   TEST_ASSERT_TRUE(run_do(test_cont,test_line_cont));
   free_word_cont(test_cont);
   free_line_cont(test_line_cont);

   test_cont = init_word_cont();
   test_line_cont= init_line_cont();
   strcpy(test_cont->words[0],"DO");
   strcpy(test_cont->words[1],"A");
   strcpy(test_cont->words[2],"FROM");
   strcpy(test_cont->words[3],"5");
   strcpy(test_cont->words[4],"TO");
   strcpy(test_cont->words[5],"4");

   strcpy(test_cont->words[6],"{");
   strcpy(test_cont->words[7],"FD");
   strcpy(test_cont->words[8],"10");
   strcpy(test_cont->words[9],"DEL");
   strcpy(test_cont->words[10],"a_myarr");
   strcpy(test_cont->words[11],"[");
   strcpy(test_cont->words[12],"0");
   strcpy(test_cont->words[13],"]");
   strcpy(test_cont->words[14],"}");

   TEST_ASSERT_TRUE(run_do(test_cont,test_line_cont));
   free_word_cont(test_cont);
   free_line_cont(test_line_cont);

   test_cont = init_word_cont();
   test_line_cont= init_line_cont();
   strcpy(test_cont->words[0],"DO");
   strcpy(test_cont->words[1],"A");
   strcpy(test_cont->words[2],"FROM");
   strcpy(test_cont->words[3],"5");
   strcpy(test_cont->words[4],"TO");
   strcpy(test_cont->words[5],"a_myarr");
   strcpy(test_cont->words[6],"[");
   strcpy(test_cont->words[7],"0");
   strcpy(test_cont->words[8],"]");
   strcpy(test_cont->words[9],"{");
   strcpy(test_cont->words[10],"FD");
   strcpy(test_cont->words[11],"10");
   strcpy(test_cont->words[12],"}");
   TEST_ASSERT_FALSE(run_do(test_cont,test_line_cont));
   free_word_cont(test_cont);
   free_line_cont(test_line_cont);

   test_cont = init_word_cont();
   test_line_cont= init_line_cont();
   strcpy(test_cont->words[0],"DO");
   strcpy(test_cont->words[1],"A");
   strcpy(test_cont->words[2],"FROM");
   strcpy(test_cont->words[3],"5");
   strcpy(test_cont->words[4],"TO");
   strcpy(test_cont->words[5],"6");
   strcpy(test_cont->words[6],"{");
   strcpy(test_cont->words[7],"FD");
   strcpy(test_cont->words[8],"a_myarr");
   strcpy(test_cont->words[9],"[");
   strcpy(test_cont->words[10],"0");
   strcpy(test_cont->words[11],"]");
   strcpy(test_cont->words[12],"}");
   TEST_ASSERT_FALSE(run_do(test_cont,test_line_cont));
   free_word_cont(test_cont);
   free_line_cont(test_line_cont);
}


void test_set_arr(void)
{
   word_cont* test_cont;
   line_cont* test_line_cont;

   test_cont = init_word_cont();
   test_line_cont= init_line_cont();
   strcpy(test_cont->words[0],"INITARR");
   strcpy(test_cont->words[1],"a_myarr");
   strcpy(test_cont->words[2],"APPEND");
   strcpy(test_cont->words[3],"a_myarr");
   strcpy(test_cont->words[4],"10");
   strcpy(test_cont->words[5],";");
   strcpy(test_cont->words[6],"SET");
   strcpy(test_cont->words[7],"A");
   strcpy(test_cont->words[8],":=");
   strcpy(test_cont->words[9],"a_myarr");
   strcpy(test_cont->words[10],"[");
   strcpy(test_cont->words[11],"0");
   strcpy(test_cont->words[12],"]");
   strcpy(test_cont->words[13],";");
   strcpy(test_cont->words[14],"FD");
   strcpy(test_cont->words[15],"A");
   TEST_ASSERT_TRUE(run_init_arr(test_cont));
   TEST_ASSERT_TRUE(run_append(test_cont,test_line_cont));
   TEST_ASSERT_TRUE(run_set(test_cont,test_line_cont));
   TEST_ASSERT_TRUE(run_instruction(test_cont,test_line_cont));
   TEST_ASSERT_EQUAL_DOUBLE(test_line_cont->array[0]->end->y,10);
   free_word_cont(test_cont);
   free_line_cont(test_line_cont);

   test_cont = init_word_cont();
   test_line_cont= init_line_cont();
   strcpy(test_cont->words[0],"INITARR");
   strcpy(test_cont->words[1],"a_myarr");
   strcpy(test_cont->words[2],"APPEND");
   strcpy(test_cont->words[3],"a_myarr");
   strcpy(test_cont->words[4],"10");
   strcpy(test_cont->words[5],";");
   strcpy(test_cont->words[6],"SET");
   strcpy(test_cont->words[7],"A");
   strcpy(test_cont->words[8],":=");
   strcpy(test_cont->words[9],"a_myarr");
   strcpy(test_cont->words[10],"[");
   strcpy(test_cont->words[11],"0");
   strcpy(test_cont->words[12],"]");
   strcpy(test_cont->words[13],"10");
   strcpy(test_cont->words[14],"*");
   strcpy(test_cont->words[15],";");
   strcpy(test_cont->words[16],"FD");
   strcpy(test_cont->words[17],"A");
   TEST_ASSERT_TRUE(run_init_arr(test_cont));
   TEST_ASSERT_TRUE(run_append(test_cont,test_line_cont));
   TEST_ASSERT_TRUE(run_set(test_cont,test_line_cont));
   TEST_ASSERT_TRUE(run_instruction(test_cont,test_line_cont));
   TEST_ASSERT_EQUAL_DOUBLE(test_line_cont->array[0]->end->y,100);
   free_word_cont(test_cont);
   free_line_cont(test_line_cont);

   test_cont = init_word_cont();
   test_line_cont= init_line_cont();
   strcpy(test_cont->words[0],"SET");
   strcpy(test_cont->words[1],"A");
   strcpy(test_cont->words[2],":=");
   strcpy(test_cont->words[3],"a_myarr");
   strcpy(test_cont->words[4],"[");
   strcpy(test_cont->words[5],"0");
   strcpy(test_cont->words[6],"]");
   strcpy(test_cont->words[7],"10");
   strcpy(test_cont->words[8],"*");
   strcpy(test_cont->words[9],";");
   strcpy(test_cont->words[10],"FD");
   strcpy(test_cont->words[11],"A");
   TEST_ASSERT_FALSE(run_set(test_cont,test_line_cont));
   TEST_ASSERT_TRUE(strcmp(test_cont->err_message,"array not found - potentially not initalised yet")==0);
   free_word_cont(test_cont);
   free_line_cont(test_line_cont);

}

void test_paths(void)
{
   word_cont* test_cont;
   turt_arr* test_arr;




   test_cont = init_word_cont();
   strcpy(test_cont->words[0],"hfihffdj.txt");
   TEST_ASSERT_TRUE(valid_filepath(test_cont));
   free_word_cont(test_cont);

   test_cont = init_word_cont();
   strcpy(test_cont->words[0],"hfihffdj.t.xt");
   TEST_ASSERT_FALSE(valid_filepath(test_cont));
   free_word_cont(test_cont);

   test_cont = init_word_cont();
   strcpy(test_cont->words[0],"hfihffdjtxt");
   TEST_ASSERT_FALSE(valid_filepath(test_cont));
   TEST_ASSERT_TRUE(strcmp(test_cont->err_message,"not valid file path, make sure it's a txt file")==0);
   free_word_cont(test_cont);

   test_cont = init_word_cont();
   strcpy(test_cont->words[0],"hfih___ffdj.txt");
   TEST_ASSERT_TRUE(valid_filepath(test_cont));
   free_word_cont(test_cont);

   test_cont = init_word_cont();
   strcpy(test_cont->words[0],"/hfih/___ffdj.txt");
   TEST_ASSERT_TRUE(valid_filepath(test_cont));
   free_word_cont(test_cont);


   test_cont = init_word_cont();
   strcpy(test_cont->words[0],"LOAD");
   strcpy(test_cont->words[1],"a_myarr");
   strcpy(test_cont->words[2],"/hfih/___ffdj.txt");
   TEST_ASSERT_TRUE(valid_file_to_array(test_cont));
   TEST_ASSERT_TRUE(test_cont->position==3);
   free_word_cont(test_cont);

   test_cont = init_word_cont();
   strcpy(test_cont->words[0],"LOAD");
   strcpy(test_cont->words[1],"a_myarr");
   strcpy(test_cont->words[2],"/hfih/ffdj.txt");
   TEST_ASSERT_TRUE(valid_file_to_array(test_cont));
   TEST_ASSERT_TRUE(test_cont->position==3);
   free_word_cont(test_cont);

   test_cont = init_word_cont();
   strcpy(test_cont->words[0],"LOAD");
   strcpy(test_cont->words[1],"a_");
   strcpy(test_cont->words[2],"/hfih/ffdj.txt");
   TEST_ASSERT_TRUE(valid_file_to_array(test_cont));
   TEST_ASSERT_TRUE(test_cont->position==3);
   free_word_cont(test_cont);


   test_cont = init_word_cont();
   strcpy(test_cont->words[0],"LOAD");
   strcpy(test_cont->words[1],"a_");
   strcpy(test_cont->words[2],"/hfih/ffdj.xt");
   TEST_ASSERT_FALSE(valid_file_to_array(test_cont));
   free_word_cont(test_cont);

   test_cont = init_word_cont();
   strcpy(test_cont->words[0],"LOAD");
   strcpy(test_cont->words[1],"a_");
   strcpy(test_cont->words[2],"/hfih/ffdjtxt");
   TEST_ASSERT_FALSE(valid_file_to_array(test_cont));
   free_word_cont(test_cont);

   test_cont = init_word_cont();
   strcpy(test_cont->words[0],"LOAD");
   strcpy(test_cont->words[1],"a");
   strcpy(test_cont->words[2],"/hfih/ffdj.txt");
   TEST_ASSERT_FALSE(valid_file_to_array(test_cont));
   free_word_cont(test_cont);





   test_cont = init_word_cont();
   test_arr=arr_init();
   TEST_ASSERT_TRUE(load_in(test_cont,test_arr,"test_files/test_sizes/7words.txt"));
   TEST_ASSERT_TRUE(test_arr->size==7);
   TEST_ASSERT_EQUAL_DOUBLE(test_arr->head->data,1);
   TEST_ASSERT_EQUAL_DOUBLE(test_arr->head->next->data,2);
   free_arr(test_arr);
   free_word_cont(test_cont);

   test_cont = init_word_cont();
   test_arr=arr_init();
   TEST_ASSERT_TRUE(load_in(test_cont,test_arr,"test_files/test_sizes/blank.txt"));
   TEST_ASSERT_TRUE(test_arr->size==0);

   free_arr(test_arr);
   free_word_cont(test_cont);

   test_cont = init_word_cont();
   test_arr=arr_init();
   TEST_ASSERT_TRUE(load_in(test_cont,test_arr,"test_files/test_sizes/random_nums.txt"));
   TEST_ASSERT_TRUE(test_arr->size==4);
   TEST_ASSERT_EQUAL_DOUBLE(test_arr->head->data,40.3454);
   TEST_ASSERT_EQUAL_DOUBLE(test_arr->head->next->data,306.111);
   free_arr(test_arr);
   free_word_cont(test_cont);


   test_cont = init_word_cont();
   test_arr=arr_init();
   TEST_ASSERT_FALSE(load_in(test_cont,test_arr,"test_files/test_sizes/not_a_file.txt"));
   TEST_ASSERT_TRUE(strcmp(test_cont->err_message,"error while opening file- does it exist?")==0);
   free_arr(test_arr);
   free_word_cont(test_cont);


   test_cont = init_word_cont();
   test_arr=arr_init();
   TEST_ASSERT_FALSE(load_in(test_cont,test_arr,"test_files/test_sizes/not_a_file.txt"));
   TEST_ASSERT_TRUE(strcmp(test_cont->err_message,"error while opening file- does it exist?")==0);
   free_arr(test_arr);
   free_word_cont(test_cont);

   test_cont = init_word_cont();
   test_arr=arr_init();
   TEST_ASSERT_FALSE(load_in(test_cont,test_arr,"test_files/test_sizes/random_letters.txt"));
   TEST_ASSERT_TRUE(strcmp(test_cont->err_message,"error reading data in file")==0);
   free_arr(test_arr);
   free_word_cont(test_cont);



   test_cont = init_word_cont();

   strcpy(test_cont->words[0],"INITARR");
   strcpy(test_cont->words[1],"a_myarr");
   strcpy(test_cont->words[2],"LOAD");
   strcpy(test_cont->words[3],"a_myarr");
   strcpy(test_cont->words[4],"test_files/test_sizes/random_nums.txt");
   TEST_ASSERT_TRUE(run_init_arr(test_cont));
   TEST_ASSERT_TRUE(run_file_to_array(test_cont));
   TEST_ASSERT_TRUE(test_cont->position==5);
   test_arr=assoc_lookup(test_cont->arr_map,"a_myarr");
   TEST_ASSERT_TRUE(test_arr->size==4);
   TEST_ASSERT_EQUAL_DOUBLE(test_arr->head->data,40.3454);
   TEST_ASSERT_EQUAL_DOUBLE(test_arr->head->next->data,306.111);
   free_word_cont(test_cont);


   test_cont = init_word_cont();

   strcpy(test_cont->words[0],"INITARR");
   strcpy(test_cont->words[1],"a_myarr");
   strcpy(test_cont->words[2],"LOAD");
   strcpy(test_cont->words[3],"a_myarr");
   strcpy(test_cont->words[4],"test_files/test_sizes/7words.txt");
   TEST_ASSERT_TRUE(run_init_arr(test_cont));
   TEST_ASSERT_TRUE(run_file_to_array(test_cont));
   TEST_ASSERT_TRUE(test_cont->position==5);
   test_arr=assoc_lookup(test_cont->arr_map,"a_myarr");
   TEST_ASSERT_TRUE(test_arr->size==7);
   TEST_ASSERT_EQUAL_DOUBLE(test_arr->head->data,1);
   TEST_ASSERT_EQUAL_DOUBLE(test_arr->head->next->data,2);
   free_word_cont(test_cont);

   test_cont = init_word_cont();

   strcpy(test_cont->words[0],"INITARR");
   strcpy(test_cont->words[1],"a_myarr");
   strcpy(test_cont->words[2],"LOAD");
   strcpy(test_cont->words[3],"a_myarr");
   strcpy(test_cont->words[4],"test_files/test_sizes/blank.txt");
   TEST_ASSERT_TRUE(run_init_arr(test_cont));
   TEST_ASSERT_TRUE(run_file_to_array(test_cont));
   TEST_ASSERT_TRUE(test_cont->position==5);
   test_arr=assoc_lookup(test_cont->arr_map,"a_myarr");
   TEST_ASSERT_TRUE(test_arr->size==0);
   free_word_cont(test_cont);



   test_cont = init_word_cont();

   strcpy(test_cont->words[0],"INITARR");
   strcpy(test_cont->words[1],"a_myarr");
   strcpy(test_cont->words[2],"LOAD");
   strcpy(test_cont->words[3],"a_myarr");
   strcpy(test_cont->words[4],"test_files/test_sizes/notafile.txt");
   TEST_ASSERT_TRUE(run_init_arr(test_cont));
   TEST_ASSERT_FALSE(run_file_to_array(test_cont));
   TEST_ASSERT_TRUE(strcmp(test_cont->err_message,"error while opening file- does it exist?")==0);
   free_word_cont(test_cont);


   test_cont = init_word_cont();

   strcpy(test_cont->words[0],"INITARR");
   strcpy(test_cont->words[1],"a_myarr");
   strcpy(test_cont->words[2],"LOAD");
   strcpy(test_cont->words[3],"a_myarr");
   strcpy(test_cont->words[4],"test_files/test_sizes/random_letters.txt");
   TEST_ASSERT_TRUE(run_init_arr(test_cont));
   TEST_ASSERT_FALSE(run_file_to_array(test_cont));
   TEST_ASSERT_TRUE(strcmp(test_cont->err_message,"error reading data in file")==0);
   free_word_cont(test_cont);


   test_cont = init_word_cont();
   strcpy(test_cont->words[0],"LOAD");
   strcpy(test_cont->words[1],"a_myarr");
   strcpy(test_cont->words[2],"test_files/test_sizes/random_letters.txt");

   TEST_ASSERT_FALSE(run_file_to_array(test_cont));
   TEST_ASSERT_TRUE(strcmp(test_cont->err_message,"array not found - potentially not initalised yet")==0);
   free_word_cont(test_cont);


   test_cont = init_word_cont();

   strcpy(test_cont->words[0],"INITARR");
   strcpy(test_cont->words[1],"a_myarr");
   strcpy(test_cont->words[2],"LOAD");
   strcpy(test_cont->words[3],"a_myarr");
   strcpy(test_cont->words[4],"test_files/test_sizes/random_letters.dan");
   TEST_ASSERT_TRUE(run_init_arr(test_cont));
   TEST_ASSERT_FALSE(run_file_to_array(test_cont));
   TEST_ASSERT_TRUE(strcmp(test_cont->err_message,"not valid file path, make sure it's a txt file")==0);
   free_word_cont(test_cont);

}



void test_len(void)
{
   word_cont* test_cont;
   double test_double;
   double** func_pointer;
   line_cont* test_line_cont;
   test_cont = init_word_cont();
   strcpy(test_cont->words[0],"LEN");
   strcpy(test_cont->words[1],"a_myarr");
   TEST_ASSERT_TRUE(valid_len(test_cont));
   TEST_ASSERT_TRUE(test_cont->position==2);
   free_word_cont(test_cont);

   test_cont = init_word_cont();
   strcpy(test_cont->words[0],"LEN");
   strcpy(test_cont->words[1],"");
   TEST_ASSERT_FALSE(valid_len(test_cont));
   free_word_cont(test_cont);

   test_cont = init_word_cont();
   strcpy(test_cont->words[0],"LEN");
   strcpy(test_cont->words[1],"///a_arr");
   TEST_ASSERT_FALSE(valid_len(test_cont));
   free_word_cont(test_cont);


   test_cont = init_word_cont();
   strcpy(test_cont->words[0],"LENYES");
   strcpy(test_cont->words[1],"///a_arr");
   TEST_ASSERT_FALSE(valid_len(test_cont));
   free_word_cont(test_cont);


   test_cont = init_word_cont();
   strcpy(test_cont->words[0],"INITARR");
   strcpy(test_cont->words[1],"a_myarr");
   strcpy(test_cont->words[2],"LEN");
   strcpy(test_cont->words[3],"a_myarr");
   TEST_ASSERT_TRUE(run_init_arr(test_cont));
   TEST_ASSERT_TRUE(run_len(test_cont,&test_double));
   TEST_ASSERT_EQUAL_DOUBLE(test_double,0);
   free_word_cont(test_cont);

   test_cont = init_word_cont();
   test_line_cont=init_line_cont();
   strcpy(test_cont->words[0],"INITARR");
   strcpy(test_cont->words[1],"a_myarr");
   strcpy(test_cont->words[2],"APPEND");
   strcpy(test_cont->words[3],"a_myarr");
   strcpy(test_cont->words[4],"10");
   strcpy(test_cont->words[5],";");
   strcpy(test_cont->words[6],"LEN");

   strcpy(test_cont->words[7],"a_myarr");
   strcpy(test_cont->words[8],"APPEND");
   strcpy(test_cont->words[9],"a_myarr");
   strcpy(test_cont->words[10],"12");
   strcpy(test_cont->words[11],";");
   strcpy(test_cont->words[12],"LEN");
   strcpy(test_cont->words[13],"a_myarr");
   strcpy(test_cont->words[14],"DEL");
   strcpy(test_cont->words[15],"a_myarr");
   strcpy(test_cont->words[16],"[");
   strcpy(test_cont->words[17],"0");
   strcpy(test_cont->words[18],"]");
   strcpy(test_cont->words[19],"LEN");
   strcpy(test_cont->words[20],"a_myarr");
   TEST_ASSERT_TRUE(run_init_arr(test_cont));
   TEST_ASSERT_TRUE(run_append(test_cont,test_line_cont));
   TEST_ASSERT_TRUE(run_len(test_cont,&test_double));
   TEST_ASSERT_EQUAL_DOUBLE(test_double,1);
   TEST_ASSERT_TRUE(test_cont->position==8);

   TEST_ASSERT_TRUE(run_append(test_cont,test_line_cont));
   TEST_ASSERT_TRUE(run_len(test_cont,&test_double));
   TEST_ASSERT_EQUAL_DOUBLE(test_double,2);

   TEST_ASSERT_TRUE(run_delete_arr_val(test_cont,test_line_cont));
   TEST_ASSERT_TRUE(run_len(test_cont,&test_double));
   TEST_ASSERT_EQUAL_DOUBLE(test_double,1);
   TEST_ASSERT_TRUE(test_cont->position==21);
   free_word_cont(test_cont);
   free_line_cont(test_line_cont);

   test_cont = init_word_cont();
   strcpy(test_cont->words[0],"LEN");
   strcpy(test_cont->words[1],"a_myarr");
   TEST_ASSERT_FALSE(run_len(test_cont,&test_double));
   free_word_cont(test_cont);


   test_cont = init_word_cont();
   strcpy(test_cont->words[0],"INITARR");
   strcpy(test_cont->words[1],"a_myarr");
   strcpy(test_cont->words[2],"LEN");
   strcpy(test_cont->words[3],"a_rm");
   TEST_ASSERT_TRUE(run_init_arr(test_cont));
   TEST_ASSERT_FALSE(run_len(test_cont,&test_double));
   free_word_cont(test_cont);

   test_cont = init_word_cont();
   strcpy(test_cont->words[0],"INITARR");
   strcpy(test_cont->words[1],"a_myarr");
   strcpy(test_cont->words[2],"L EN");
   strcpy(test_cont->words[3],"a_rm");
   TEST_ASSERT_TRUE(run_init_arr(test_cont));
   TEST_ASSERT_FALSE(run_len(test_cont,&test_double));
   free_word_cont(test_cont);

   test_cont = init_word_cont();
   test_line_cont=init_line_cont();
   strcpy(test_cont->words[0],"INITARR");
   strcpy(test_cont->words[1],"a_myarr");
   strcpy(test_cont->words[2],"SET");
   strcpy(test_cont->words[3],"A");
   strcpy(test_cont->words[4],":=");
   strcpy(test_cont->words[5],"LEN");
   strcpy(test_cont->words[6],"a_myarr");
   strcpy(test_cont->words[7],"1");
   strcpy(test_cont->words[8],"+");
   strcpy(test_cont->words[9],";");
   TEST_ASSERT_TRUE(run_init_arr(test_cont));
   TEST_ASSERT_TRUE(run_set(test_cont,test_line_cont));
   TEST_ASSERT_EQUAL_DOUBLE(*test_cont->var_array[0],1);

   free_word_cont(test_cont);
   free_line_cont(test_line_cont);

   test_cont = init_word_cont();
   test_line_cont=init_line_cont();
   strcpy(test_cont->words[0],"INITARR");
   strcpy(test_cont->words[1],"a_myarr");
   strcpy(test_cont->words[2],"DO");
   strcpy(test_cont->words[3],"A");
   strcpy(test_cont->words[4],"FROM");
   strcpy(test_cont->words[5],"LEN");
   strcpy(test_cont->words[6],"a_myarr");
   strcpy(test_cont->words[7],"TO");
   strcpy(test_cont->words[8],"10");
   strcpy(test_cont->words[9],"{");
   strcpy(test_cont->words[10],"FD");
   strcpy(test_cont->words[11],"LEN");
   strcpy(test_cont->words[12],"a_myarr");
   strcpy(test_cont->words[13],"}");
   TEST_ASSERT_TRUE(run_init_arr(test_cont));
   TEST_ASSERT_TRUE(run_do(test_cont,test_line_cont));
   TEST_ASSERT_TRUE(test_line_cont->size==11);

   free_word_cont(test_cont);
   free_line_cont(test_line_cont);

   test_cont = init_word_cont();
   test_line_cont=init_line_cont();
   strcpy(test_cont->words[0],"INITARR");
   strcpy(test_cont->words[1],"a_myarr");
   strcpy(test_cont->words[2],"DO");
   strcpy(test_cont->words[3],"A");
   strcpy(test_cont->words[4],"FROM");
   strcpy(test_cont->words[5],"LEN");
   strcpy(test_cont->words[6],"a_myarr");
   strcpy(test_cont->words[7],"TO");
   strcpy(test_cont->words[8],"10");
   strcpy(test_cont->words[9],"{");
   strcpy(test_cont->words[10],"FD");
   strcpy(test_cont->words[11],"LEN");
   strcpy(test_cont->words[12],"a_myarr");
   strcpy(test_cont->words[13],"}");
   TEST_ASSERT_TRUE(run_init_arr(test_cont));
   TEST_ASSERT_TRUE(run_do(test_cont,test_line_cont));
   TEST_ASSERT_TRUE(test_line_cont->size==11);

   free_word_cont(test_cont);
   free_line_cont(test_line_cont);



   test_cont = init_word_cont();
   test_line_cont=init_line_cont();
   func_pointer=NULL;
   strcpy(test_cont->words[0],"INITARR");
   strcpy(test_cont->words[1],"a_myarr");
   strcpy(test_cont->words[2],"SETFUNC");
   strcpy(test_cont->words[3],"abc");
   strcpy(test_cont->words[4],"{");
   strcpy(test_cont->words[5],"A");
   strcpy(test_cont->words[6],"}");
   strcpy(test_cont->words[7],"{");
   strcpy(test_cont->words[8],"FD");
   strcpy(test_cont->words[9],"A");
   strcpy(test_cont->words[10],"}");
   strcpy(test_cont->words[11],"APPEND");
   strcpy(test_cont->words[12],"a_myarr");
   strcpy(test_cont->words[13],"10");
   strcpy(test_cont->words[14],";");
   strcpy(test_cont->words[15],"abc");
   strcpy(test_cont->words[16],"{");
   strcpy(test_cont->words[17],"LEN");
   strcpy(test_cont->words[18],"a_myarr");
   strcpy(test_cont->words[19],"}");
   TEST_ASSERT_TRUE(run_init_arr(test_cont));
   TEST_ASSERT_TRUE(run_funcset(test_cont));
   TEST_ASSERT_TRUE(run_append(test_cont,test_line_cont));
   TEST_ASSERT_TRUE(run_funcrun(test_cont,test_line_cont,func_pointer));
   TEST_ASSERT_EQUAL_DOUBLE(test_line_cont->array[0]->end->y,1);

   free_word_cont(test_cont);
   free_line_cont(test_line_cont);

   test_cont = init_word_cont();
   test_line_cont=init_line_cont();
   func_pointer=NULL;
   strcpy(test_cont->words[0],"INITARR");
   strcpy(test_cont->words[1],"a_myarr");
   strcpy(test_cont->words[2],"IF");
   strcpy(test_cont->words[3],"LEN");
   strcpy(test_cont->words[4],"a_myarr");
   strcpy(test_cont->words[5],">");
   strcpy(test_cont->words[6],"0");
   strcpy(test_cont->words[7],"{");
   strcpy(test_cont->words[8],"FD");
   strcpy(test_cont->words[9],"1");
   strcpy(test_cont->words[10],"}");
   strcpy(test_cont->words[11],"ELSE");
   strcpy(test_cont->words[12],"{");
   strcpy(test_cont->words[13],"APPEND");
   strcpy(test_cont->words[14],"a_myarr");
   strcpy(test_cont->words[15],"10");
   strcpy(test_cont->words[16],";");
   strcpy(test_cont->words[17],"}");
   TEST_ASSERT_TRUE(run_init_arr(test_cont));
   TEST_ASSERT_TRUE(run_flowstate(test_cont,test_line_cont));
   TEST_ASSERT_TRUE(test_line_cont->size==0);

   free_word_cont(test_cont);
   free_line_cont(test_line_cont);

}

int main(void)
{
   UNITY_BEGIN();
   RUN_TEST(test_identifier);
   RUN_TEST(test_init_arr);
   RUN_TEST(test_append);
   RUN_TEST(test_valid_ind);
   RUN_TEST(test_change);
   RUN_TEST(test_delete);
   RUN_TEST(test_access);
   RUN_TEST(test_do_arr);
   RUN_TEST(test_set_arr);
   RUN_TEST(test_paths);
   RUN_TEST(test_len);
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
   n_cont->arr_map=assoc_init();
   return n_cont;
}
