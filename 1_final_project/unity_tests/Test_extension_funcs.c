#include "../src/unity.h"
#include "../extension_funcs.h"

#define MAXTESTCAP 100
#define MAXTESTLEN 50



word_cont* init_word_cont(void);


void setUp(void)
{

}

void tearDown(void)
{

}

void test_valid_func_var(void)
{
   word_cont* test_cont;
   test_cont=init_word_cont();
   strcpy(test_cont->words[0],"aaabbbccc");
   TEST_ASSERT_TRUE(valid_funcvar(test_cont));
   strcpy(test_cont->words[0],"aaAaa");
   TEST_ASSERT_TRUE(!valid_funcvar(test_cont));
   strcpy(test_cont->words[0],"aa|aa");
   TEST_ASSERT_TRUE(!valid_funcvar(test_cont));
   strcpy(test_cont->words[0],"");
   TEST_ASSERT_TRUE(!valid_funcvar(test_cont));
   strcpy(test_cont->words[0],"123");
   TEST_ASSERT_TRUE(!valid_funcvar(test_cont));
   strcpy(test_cont->words[0],"FD");
   TEST_ASSERT_TRUE(!valid_funcvar(test_cont));
   strcpy(test_cont->words[0],"z");
   TEST_ASSERT_TRUE(valid_funcvar(test_cont));
   free_word_cont(test_cont);
}

void test_set_funcvar(void)
{
   word_cont* test_cont;
   char word[MAXTESTLEN];
   test_cont=init_word_cont();
   strcpy(test_cont->words[0],"aaabbbccc");
   TEST_ASSERT_TRUE(set_funcvar(test_cont,word));
   TEST_ASSERT_TRUE(strcmp(word,"aaabbbccc")==0);
   TEST_ASSERT_TRUE(test_cont->position==1);

   strcpy(test_cont->words[0],"aaaAaa");
   test_cont->position=0;
   TEST_ASSERT_TRUE(!set_funcvar(test_cont,word));
   strcpy(test_cont->words[0],"aa|aa");
   TEST_ASSERT_TRUE(!set_funcvar(test_cont,word));
   strcpy(test_cont->words[0],"");
   TEST_ASSERT_TRUE(!set_funcvar(test_cont,word));

   strcpy(test_cont->words[0],"z");
   TEST_ASSERT_TRUE(set_funcvar(test_cont,word));
   TEST_ASSERT_TRUE(strcmp(word,"z")==0);
   free_word_cont(test_cont);
}

void test_get_arg(void)
{
   word_cont* test_cont;
   word_cont* test_func;
   int i;
   test_cont=init_word_cont();
   test_func=init_func_cont();
   for(i=0;i<NUMVARS;i++)
   {
      TEST_ASSERT_TRUE(test_func->arg_placer[i]==UNUSED);
   }
   strcpy(test_cont->words[0],"A");
   test_cont->position=0;
   TEST_ASSERT_TRUE(get_arg(test_cont,test_func));
   TEST_ASSERT_TRUE(test_func->arg_placer[0]==0);
   TEST_ASSERT_TRUE(test_cont->position==1);
   test_cont->position=0;
   TEST_ASSERT_TRUE(!get_arg(test_cont,test_func));
   TEST_ASSERT_TRUE(strcmp(test_cont->err_message,"used same var twice when defining function")==0);
   free_word_cont(test_func);
   test_func=init_func_cont();
   strcpy(test_cont->words[0],"Z");
   TEST_ASSERT_TRUE(get_arg(test_cont,test_func));
   TEST_ASSERT_TRUE(test_func->arg_placer[0]==25);
   TEST_ASSERT_TRUE(test_cont->position==1);
   test_cont->position=0;
   strcpy(test_cont->words[0],"B");
   TEST_ASSERT_TRUE(get_arg(test_cont,test_func));
   TEST_ASSERT_TRUE(test_func->arg_placer[1]==1);

   test_cont->position=0;
   strcpy(test_cont->words[0],"a");
   TEST_ASSERT_TRUE(!get_arg(test_cont,test_func));

   test_cont->position=0;
   strcpy(test_cont->words[0],"@");
   TEST_ASSERT_TRUE(!get_arg(test_cont,test_func));

   test_cont->position=0;
   strcpy(test_cont->words[0],"");
   TEST_ASSERT_TRUE(!get_arg(test_cont,test_func));

   test_cont->position=0;
   strcpy(test_cont->words[0],"AA");
   TEST_ASSERT_TRUE(!get_arg(test_cont,test_func));

   free_word_cont(test_func);
   free_word_cont(test_cont);
}

void test_argset(void)
{
   word_cont* test_cont;
   word_cont* test_func;
   test_func=init_func_cont();
   test_cont=init_word_cont();

   strcpy(test_cont->words[0],"A");
   strcpy(test_cont->words[1],"B");
   strcpy(test_cont->words[2],"C");
   strcpy(test_cont->words[3],"}");
   TEST_ASSERT_TRUE(valid_argset(test_cont));
   test_cont->position=0;
   TEST_ASSERT_TRUE(get_argset(test_cont,test_func));
   TEST_ASSERT_TRUE(test_func->arg_placer[0]==0);
   TEST_ASSERT_TRUE(test_func->arg_placer[1]==1);
   TEST_ASSERT_TRUE(test_func->arg_placer[2]==2);
   TEST_ASSERT_TRUE(test_cont->position==4);

   free_word_cont(test_func);
   test_func=init_func_cont();

   test_cont->position=0;
   strcpy(test_cont->words[3],"D");
   TEST_ASSERT_TRUE(!valid_argset(test_cont));
   test_cont->position=0;
   TEST_ASSERT_TRUE(!get_argset(test_cont,test_func));


   free_word_cont(test_func);
   test_func=init_func_cont();

   test_cont->position=0;
   strcpy(test_cont->words[2],"B");
   strcpy(test_cont->words[3],"}");
   TEST_ASSERT_TRUE(valid_argset(test_cont));
   test_cont->position=0;
   TEST_ASSERT_TRUE(!get_argset(test_cont,test_func));
   TEST_ASSERT_TRUE(strcmp(test_cont->err_message,"used same var twice when defining function")==0);

   test_cont->position=0;
   free_word_cont(test_func);
   test_func=init_func_cont();
   strcpy(test_cont->words[0],"A");
   strcpy(test_cont->words[1],"DD");
   strcpy(test_cont->words[2],"C");
   strcpy(test_cont->words[3],"}");
   TEST_ASSERT_TRUE(!valid_argset(test_cont));
   test_cont->position=0;
   TEST_ASSERT_TRUE(!get_argset(test_cont,test_func));

   free_word_cont(test_func);
   free_word_cont(test_cont);

}

void test_resize(void)
{

   word_cont* test_func;
   test_func=init_func_cont();
   test_func->words[0]=safe_calloc(10,1);
   strcpy(test_func->words[0],"hello");

   test_func->words[2]=safe_calloc(10,1);
   strcpy(test_func->words[2],"goodbye");

   resize(test_func);
   TEST_ASSERT_TRUE(test_func->capacity==(INITFUNCCAP*FUNCSCALE));
   TEST_ASSERT_TRUE(strcmp(test_func->words[0],"hello")==0);
   TEST_ASSERT_TRUE(strcmp(test_func->words[2],"goodbye")==0);
   free_word_cont(test_func);
}


void test_get_funcinfo(void)
{
   word_cont* test_cont;
   word_cont* test_func;

   test_func=init_func_cont();
   test_cont=init_word_cont();

   strcpy(test_cont->words[0],"FD");
   strcpy(test_cont->words[1],"30");
   strcpy(test_cont->words[2],"LT");
   strcpy(test_cont->words[3],"LT");
   strcpy(test_cont->words[4],"}");

   TEST_ASSERT_TRUE(!get_func_info(test_cont,test_func));
   test_cont->position=0;
   strcpy(test_cont->words[3],"30");
   strcpy(test_cont->words[4],"30");
   TEST_ASSERT_TRUE(!get_func_info(test_cont,test_func));
   test_cont->position=0;
   strcpy(test_cont->words[3],"");
   strcpy(test_cont->words[4],"}");
   TEST_ASSERT_TRUE(!get_func_info(test_cont,test_func));
   test_cont->position=0;

   strcpy(test_cont->words[0],"FD");
   strcpy(test_cont->words[1],"-30");
   strcpy(test_cont->words[2],"LT");
   strcpy(test_cont->words[3],"30");
   strcpy(test_cont->words[4],"}");
   TEST_ASSERT_TRUE(get_func_info(test_cont,test_func));
   free_word_cont(test_func);
   free_word_cont(test_cont);

   test_func=init_func_cont();
   test_cont=init_word_cont();
   strcpy(test_cont->words[0],"FD");
   strcpy(test_cont->words[1],"-30.907");
   strcpy(test_cont->words[2],"LT");
   strcpy(test_cont->words[3],"30");
   strcpy(test_cont->words[4],"}");
   TEST_ASSERT_TRUE(get_func_info(test_cont,test_func));
   free_word_cont(test_func);
   free_word_cont(test_cont);
}

void test_funcset(void)
{
   word_cont* test_cont;
   word_cont* test_func;
   char word[MAXTESTLEN];
   int i;
   test_cont=init_word_cont();
   strcpy(test_cont->words[0],"SETFUNC");
   strcpy(test_cont->words[1],"abc");
   strcpy(test_cont->words[2],"{");
   strcpy(test_cont->words[3],"A");
   strcpy(test_cont->words[4],"}");
   strcpy(test_cont->words[5],"{");
   strcpy(test_cont->words[6],"FD");
   strcpy(test_cont->words[7],"A");
   strcpy(test_cont->words[8],"}");
   TEST_ASSERT_TRUE(run_funcset(test_cont));
   test_func=assoc_lookup(test_cont->func_map,"abc");
   TEST_ASSERT_TRUE(test_func);
   TEST_ASSERT_TRUE(strcmp(test_func->words[0],"FD")==0);
   TEST_ASSERT_TRUE(strcmp(test_func->words[1],"A")==0);
   TEST_ASSERT_TRUE(test_func->arg_placer[0]==0);
   TEST_ASSERT_TRUE(test_cont->position==9);
   free_word_cont(test_cont);


   test_cont=init_word_cont();
   strcpy(test_cont->words[0],"SETFUNC");
   strcpy(test_cont->words[1],"abc");
   strcpy(test_cont->words[2],"{");
   strcpy(test_cont->words[3],"A");
   strcpy(test_cont->words[4],"}");
   strcpy(test_cont->words[5],"{");
   strcpy(test_cont->words[6],"FD");
   strcpy(test_cont->words[7],"A");
   strcpy(test_cont->words[8],"}");

   word[0]='\0';
   /*just need to do enough to see that
   resize function is working ok*/
   for(i=0;i<40;i++)
   {
      strcat(word,"a");
      test_cont->position=0;
      strcpy(test_cont->words[1],word);
      TEST_ASSERT_TRUE(assoc_count(test_cont->func_map)==(unsigned int)i);
      TEST_ASSERT_TRUE(run_funcset(test_cont));


   }
   word[0]='\0';
   for(i=0;i<40;i++)
   {
      strcat(word,"a");
      TEST_ASSERT_TRUE(assoc_lookup(test_cont->func_map,word));
   }
   free_word_cont(test_cont);

   test_cont=init_word_cont();
   strcpy(test_cont->words[0],"SETFUNC");
   strcpy(test_cont->words[1],"abcA");
   strcpy(test_cont->words[2],"{");
   strcpy(test_cont->words[3],"A");
   strcpy(test_cont->words[4],"}");
   strcpy(test_cont->words[5],"{");
   strcpy(test_cont->words[6],"FD");
   strcpy(test_cont->words[7],"A");
   strcpy(test_cont->words[8],"}");
   TEST_ASSERT_TRUE(!valid_funcset(test_cont));
   test_cont->position=0;
   TEST_ASSERT_TRUE(!run_funcset(test_cont));

   strcpy(test_cont->words[1],"");
   test_cont->position=0;
   TEST_ASSERT_TRUE(!valid_funcset(test_cont));
   test_cont->position=0;
   TEST_ASSERT_TRUE(!run_funcset(test_cont));

   test_cont->position=0;
   strcpy(test_cont->words[1],"abc");
   strcpy(test_cont->words[3],"a");
   TEST_ASSERT_TRUE(!valid_funcset(test_cont));
   test_cont->position=0;
   TEST_ASSERT_TRUE(!run_funcset(test_cont));

   test_cont->position=0;
   strcpy(test_cont->words[3],"A");
   strcpy(test_cont->words[7],"dnjdbjfnekdddd");
   TEST_ASSERT_TRUE(!valid_funcset(test_cont));
   test_cont->position=0;
   TEST_ASSERT_TRUE(!run_funcset(test_cont));

   free_word_cont(test_cont);
}

void test_get_funcvar(void)
{
   word_cont* test_cont;
   word_cont* test_func;
   test_cont=init_word_cont();
   strcpy(test_cont->words[0],"SETFUNC");
   strcpy(test_cont->words[1],"abc");
   strcpy(test_cont->words[2],"{");
   strcpy(test_cont->words[3],"A");
   strcpy(test_cont->words[4],"}");
   strcpy(test_cont->words[5],"{");
   strcpy(test_cont->words[6],"FD");
   strcpy(test_cont->words[7],"A");
   strcpy(test_cont->words[8],"}");
   strcpy(test_cont->words[9],"abc");
   TEST_ASSERT_TRUE(run_funcset(test_cont));
   test_func=NULL;
   TEST_ASSERT_TRUE(get_funcvar(test_cont,&test_func));
   TEST_ASSERT_TRUE(test_func->position==0);
   TEST_ASSERT_TRUE(strcmp(test_func->words[0],"FD")==0);
   TEST_ASSERT_TRUE(strcmp(test_func->words[1],"A")==0);
   TEST_ASSERT_TRUE(strcmp(test_func->words[2],"}")==0);

   free_word_cont(test_cont);
   test_func=NULL;

   test_cont=init_word_cont();
   strcpy(test_cont->words[0],"SETFUNC");
   strcpy(test_cont->words[1],"abc");
   strcpy(test_cont->words[2],"{");
   strcpy(test_cont->words[3],"A");
   strcpy(test_cont->words[4],"}");
   strcpy(test_cont->words[5],"{");
   strcpy(test_cont->words[6],"LT");
   strcpy(test_cont->words[7],"A");
   strcpy(test_cont->words[8],"RT");
   strcpy(test_cont->words[9],"50");
   strcpy(test_cont->words[10],"FD");
   strcpy(test_cont->words[11],"10");
   strcpy(test_cont->words[12],"}");
   strcpy(test_cont->words[13],"abc");
   TEST_ASSERT_TRUE(run_funcset(test_cont));
   TEST_ASSERT_TRUE(get_funcvar(test_cont,&test_func));
   free_word_cont(test_cont);
   test_func=NULL;

   test_cont=init_word_cont();
   strcpy(test_cont->words[0],"SETFUNC");
   strcpy(test_cont->words[1],"abc");
   strcpy(test_cont->words[2],"{");
   strcpy(test_cont->words[3],"A");
   strcpy(test_cont->words[4],"}");
   strcpy(test_cont->words[5],"{");
   strcpy(test_cont->words[6],"}");
   strcpy(test_cont->words[7],"abc");
   TEST_ASSERT_TRUE(run_funcset(test_cont));
   TEST_ASSERT_TRUE(get_funcvar(test_cont,&test_func));
   TEST_ASSERT_TRUE(test_func);
   free_word_cont(test_cont);
   test_func=NULL;



   test_cont=init_word_cont();
   strcpy(test_cont->words[0],"SETFUNC");
   strcpy(test_cont->words[1],"abc");
   strcpy(test_cont->words[2],"{");
   strcpy(test_cont->words[3],"A");
   strcpy(test_cont->words[4],"}");
   strcpy(test_cont->words[5],"{");
   strcpy(test_cont->words[6],"LT");
   strcpy(test_cont->words[7],"A");
   strcpy(test_cont->words[8],"RT");
   strcpy(test_cont->words[9],"50");
   strcpy(test_cont->words[10],"FD");
   strcpy(test_cont->words[11],"10");
   strcpy(test_cont->words[12],"}");
   strcpy(test_cont->words[13],"ab");
   TEST_ASSERT_TRUE(run_funcset(test_cont));
   TEST_ASSERT_TRUE(!get_funcvar(test_cont,&test_func));
   TEST_ASSERT_TRUE(strcmp(test_cont->err_message,"function name not found, has it been set?")==0);
   free_word_cont(test_cont);

   test_cont=init_word_cont();
   strcpy(test_cont->words[0],"SETFUNC");
   strcpy(test_cont->words[1],"abc");
   strcpy(test_cont->words[2],"{");
   strcpy(test_cont->words[3],"A");
   strcpy(test_cont->words[4],"}");
   strcpy(test_cont->words[5],"{");
   strcpy(test_cont->words[6],"LT");
   strcpy(test_cont->words[7],"A");
   strcpy(test_cont->words[8],"RT");
   strcpy(test_cont->words[9],"50");
   strcpy(test_cont->words[10],"FD");
   strcpy(test_cont->words[11],"10");
   strcpy(test_cont->words[12],"}");
   strcpy(test_cont->words[13],"");
   TEST_ASSERT_TRUE(run_funcset(test_cont));
   TEST_ASSERT_TRUE(!get_funcvar(test_cont,&test_func));
   free_word_cont(test_cont);
   free_word_cont(test_func);
}

void test_place_arg(void)
{
   word_cont* test_cont;
   word_cont* test_func;
   int i;
   char word[MAXTESTLEN];
   line_cont* test_line_cont;

   test_cont=init_word_cont();
   test_line_cont=init_line_cont();
   strcpy(test_cont->words[0],"SETFUNC");
   strcpy(test_cont->words[1],"abc");
   strcpy(test_cont->words[2],"{");
   strcpy(test_cont->words[3],"A");
   strcpy(test_cont->words[4],"}");
   strcpy(test_cont->words[5],"{");
   strcpy(test_cont->words[6],"}");
   strcpy(test_cont->words[7],"abc");
   strcpy(test_cont->words[8],"30");
   TEST_ASSERT_TRUE(run_funcset(test_cont));
   TEST_ASSERT_TRUE(get_funcvar(test_cont,&test_func));
   TEST_ASSERT_TRUE(place_arg(test_cont,test_func,0,test_line_cont));
   /*as A was the first argument when we set the function
   we can expect to find 30 in position 0 in the var array */
   TEST_ASSERT_EQUAL_DOUBLE(*test_func->var_array[0],30);
   free_word_cont(test_cont);

   test_cont=init_word_cont();
   strcpy(test_cont->words[0],"SETFUNC");
   strcpy(test_cont->words[1],"abc");
   strcpy(test_cont->words[2],"{");
   strcpy(test_cont->words[3],"A");
   strcpy(test_cont->words[4],"Z");
   strcpy(test_cont->words[5],"}");
   strcpy(test_cont->words[6],"{");
   strcpy(test_cont->words[7],"}");
   strcpy(test_cont->words[8],"abc");
   strcpy(test_cont->words[9],"-30");
   strcpy(test_cont->words[10],"3.78");
   TEST_ASSERT_TRUE(run_funcset(test_cont));
   TEST_ASSERT_TRUE(get_funcvar(test_cont,&test_func));
   TEST_ASSERT_TRUE(place_arg(test_cont,test_func,0,test_line_cont));
   TEST_ASSERT_TRUE(place_arg(test_cont,test_func,1,test_line_cont));
   TEST_ASSERT_EQUAL_DOUBLE(*test_func->var_array[0],-30);
   TEST_ASSERT_EQUAL_DOUBLE(*test_func->var_array[25],3.78);
   free_word_cont(test_cont);

   test_cont=init_word_cont();


   strcpy(test_cont->words[0],"SETFUNC");
   strcpy(test_cont->words[1],"abc");
   strcpy(test_cont->words[2],"{");
   strcpy(word,"A");
   for(i=0;i<26;i++)
   {
      word[0]=word[0]+i;
      strcpy(test_cont->words[3+i],word);
      word[0]='A';
   }

   strcpy(test_cont->words[29],"}");
   strcpy(test_cont->words[30],"{");
   strcpy(test_cont->words[31],"}");
   strcpy(test_cont->words[32],"abc");



   TEST_ASSERT_TRUE(run_funcset(test_cont));
   TEST_ASSERT_TRUE(get_funcvar(test_cont,&test_func));
   for(i=0;i<26;i++)
   {
      test_cont->position=33;
      sprintf(word, "%f", (double)i);
      strcpy(test_cont->words[33],word);
      TEST_ASSERT_TRUE(place_arg(test_cont,test_func,i,test_line_cont));
      TEST_ASSERT_EQUAL_DOUBLE(*test_func->var_array[i],i);
   }

   free_word_cont(test_cont);

   test_cont=init_word_cont();
   strcpy(test_cont->words[0],"SETFUNC");
   strcpy(test_cont->words[1],"abc");
   strcpy(test_cont->words[2],"{");
   strcpy(test_cont->words[3],"A");
   strcpy(test_cont->words[4],"}");
   strcpy(test_cont->words[5],"{");
   strcpy(test_cont->words[6],"}");
   strcpy(test_cont->words[7],"abc");
   strcpy(test_cont->words[8],"30");
   TEST_ASSERT_TRUE(run_funcset(test_cont));
   TEST_ASSERT_TRUE(get_funcvar(test_cont,&test_func));
   TEST_ASSERT_TRUE(!place_arg(test_cont,test_func,1,test_line_cont));
   free_word_cont(test_cont);

   test_cont=init_word_cont();
   strcpy(test_cont->words[0],"SETFUNC");
   strcpy(test_cont->words[1],"abc");
   strcpy(test_cont->words[2],"{");
   strcpy(test_cont->words[3],"A");
   strcpy(test_cont->words[4],"}");
   strcpy(test_cont->words[5],"{");
   strcpy(test_cont->words[6],"}");
   strcpy(test_cont->words[7],"abc");
   strcpy(test_cont->words[8],"");
   TEST_ASSERT_TRUE(run_funcset(test_cont));
   TEST_ASSERT_TRUE(get_funcvar(test_cont,&test_func));
   TEST_ASSERT_TRUE(!place_arg(test_cont,test_func,0,test_line_cont));
   free_word_cont(test_cont);

   test_cont=init_word_cont();

   strcpy(test_cont->words[0],"SETFUNC");
   strcpy(test_cont->words[1],"abc");
   strcpy(test_cont->words[2],"{");
   strcpy(test_cont->words[3],"A");
   strcpy(test_cont->words[4],"}");
   strcpy(test_cont->words[5],"{");
   strcpy(test_cont->words[6],"}");
   strcpy(test_cont->words[7],"abc");
   strcpy(test_cont->words[8],"B");
   TEST_ASSERT_TRUE(run_funcset(test_cont));
   TEST_ASSERT_TRUE(get_funcvar(test_cont,&test_func));
   TEST_ASSERT_TRUE(!place_arg(test_cont,test_func,0,test_line_cont));
   TEST_ASSERT_TRUE(strcmp(test_cont->err_message,"potentially haven't set variable before calling it")==0);
   free_word_cont(test_cont);



   test_cont=init_word_cont();
   strcpy(test_cont->words[0],"SETFUNC");
   strcpy(test_cont->words[1],"abc");
   strcpy(test_cont->words[2],"{");
   strcpy(test_cont->words[3],"}");
   strcpy(test_cont->words[4],"{");
   strcpy(test_cont->words[5],"}");
   strcpy(test_cont->words[6],"abc");
   strcpy(test_cont->words[7],"21");
   TEST_ASSERT_TRUE(valid_funcset(test_cont));
   test_cont->position=0;
   TEST_ASSERT_TRUE(run_funcset(test_cont));
   TEST_ASSERT_TRUE(get_funcvar(test_cont,&test_func));
   TEST_ASSERT_TRUE(!place_arg(test_cont,test_func,0,test_line_cont));
   free_word_cont(test_cont);

   test_cont=init_word_cont();
   strcpy(test_cont->words[0],"SET");
   strcpy(test_cont->words[1],"Z");
   strcpy(test_cont->words[2],":=");
   strcpy(test_cont->words[3],"90.5");
   strcpy(test_cont->words[4],";");
   strcpy(test_cont->words[5],"SETFUNC");
   strcpy(test_cont->words[6],"abc");
   strcpy(test_cont->words[7],"{");
   strcpy(test_cont->words[8],"A");
   strcpy(test_cont->words[9],"}");
   strcpy(test_cont->words[10],"{");
   strcpy(test_cont->words[11],"}");
   strcpy(test_cont->words[12],"abc");
   strcpy(test_cont->words[13],"Z");
   TEST_ASSERT_TRUE(run_set(test_cont,test_line_cont));
   TEST_ASSERT_TRUE(run_funcset(test_cont));
   TEST_ASSERT_TRUE(get_funcvar(test_cont,&test_func));
   TEST_ASSERT_TRUE(place_arg(test_cont,test_func,0,test_line_cont));
   TEST_ASSERT_EQUAL_DOUBLE(*test_func->var_array[0],90.5);
   free_word_cont(test_cont);

   free_line_cont(test_line_cont);

}

void test_place_all_args(void)
{

   word_cont* test_cont;
   word_cont* test_func;
   line_cont* test_line_cont;
   test_cont=init_word_cont();
   test_line_cont=init_line_cont();
   strcpy(test_cont->words[0],"SET");
   strcpy(test_cont->words[1],"Z");
   strcpy(test_cont->words[2],":=");
   strcpy(test_cont->words[3],"90.5");
   strcpy(test_cont->words[4],";");
   strcpy(test_cont->words[5],"SETFUNC");
   strcpy(test_cont->words[6],"abc");
   strcpy(test_cont->words[7],"{");
   strcpy(test_cont->words[8],"A");
   strcpy(test_cont->words[9],"}");
   strcpy(test_cont->words[10],"{");
   strcpy(test_cont->words[11],"}");
   strcpy(test_cont->words[12],"abc");
   strcpy(test_cont->words[13],"Z");
   strcpy(test_cont->words[14],"}");
   TEST_ASSERT_TRUE(run_set(test_cont,test_line_cont));
   TEST_ASSERT_TRUE(valid_funcset(test_cont));
   test_cont->position=5;
   TEST_ASSERT_TRUE(run_funcset(test_cont));
   TEST_ASSERT_TRUE(get_funcvar(test_cont,&test_func));
   TEST_ASSERT_TRUE(valid_argsrun(test_cont));
   test_cont->position=13;
   TEST_ASSERT_TRUE(place_all_args(test_cont,test_func,0,test_line_cont));
   TEST_ASSERT_EQUAL_DOUBLE(*test_func->var_array[0],90.5);
   free_word_cont(test_cont);
   free_line_cont(test_line_cont);

   test_line_cont=init_line_cont();
   test_cont=init_word_cont();
   strcpy(test_cont->words[0],"SETFUNC");
   strcpy(test_cont->words[1],"abc");
   strcpy(test_cont->words[2],"{");
   strcpy(test_cont->words[3],"A");
   strcpy(test_cont->words[4],"Z");
   strcpy(test_cont->words[5],"}");
   strcpy(test_cont->words[6],"{");
   strcpy(test_cont->words[7],"}");
   strcpy(test_cont->words[8],"abc");
   strcpy(test_cont->words[9],"-30");
   strcpy(test_cont->words[10],"3.78");
   strcpy(test_cont->words[11],"}");
   TEST_ASSERT_TRUE(valid_funcset(test_cont));
   test_cont->position=0;
   TEST_ASSERT_TRUE(run_funcset(test_cont));
   TEST_ASSERT_TRUE(get_funcvar(test_cont,&test_func));
   TEST_ASSERT_TRUE(valid_argsrun(test_cont));
   test_cont->position=9;
   TEST_ASSERT_TRUE(place_all_args(test_cont,test_func,0,test_line_cont));
   TEST_ASSERT_EQUAL_DOUBLE(*test_func->var_array[0],-30);
   TEST_ASSERT_EQUAL_DOUBLE(*test_func->var_array[25],3.78);
   free_word_cont(test_cont);
   free_line_cont(test_line_cont);

   test_line_cont=init_line_cont();
   test_cont=init_word_cont();
   strcpy(test_cont->words[0],"SETFUNC");
   strcpy(test_cont->words[1],"abc");
   strcpy(test_cont->words[2],"{");
   strcpy(test_cont->words[3],"A");
   strcpy(test_cont->words[4],"}");
   strcpy(test_cont->words[5],"{");
   strcpy(test_cont->words[6],"}");
   strcpy(test_cont->words[7],"abc");
   strcpy(test_cont->words[8],"-30");
   strcpy(test_cont->words[9],"}");
   TEST_ASSERT_TRUE(valid_funcset(test_cont));
   test_cont->position=0;
   TEST_ASSERT_TRUE(run_funcset(test_cont));
   TEST_ASSERT_TRUE(get_funcvar(test_cont,&test_func));
   TEST_ASSERT_TRUE(valid_argsrun(test_cont));
   test_cont->position=8;
   TEST_ASSERT_TRUE(place_all_args(test_cont,test_func,0,test_line_cont));
   TEST_ASSERT_EQUAL_DOUBLE(*test_func->var_array[0],-30);
   free_word_cont(test_cont);
   free_line_cont(test_line_cont);


   test_line_cont=init_line_cont();
   test_cont=init_word_cont();
   strcpy(test_cont->words[0],"SETFUNC");
   strcpy(test_cont->words[1],"abc");
   strcpy(test_cont->words[2],"{");
   strcpy(test_cont->words[3],"A");
   strcpy(test_cont->words[4],"}");
   strcpy(test_cont->words[5],"{");
   strcpy(test_cont->words[6],"}");
   strcpy(test_cont->words[7],"abc");
   strcpy(test_cont->words[8],"-30");
   strcpy(test_cont->words[9],"3.78");
   strcpy(test_cont->words[10],"}");
   TEST_ASSERT_TRUE(valid_funcset(test_cont));
   test_cont->position=0;
   TEST_ASSERT_TRUE(run_funcset(test_cont));
   TEST_ASSERT_TRUE(get_funcvar(test_cont,&test_func));
   TEST_ASSERT_TRUE(valid_argsrun(test_cont));
   test_cont->position=8;
   TEST_ASSERT_TRUE(!place_all_args(test_cont,test_func,0,test_line_cont));
   free_word_cont(test_cont);


   test_cont=init_word_cont();
   strcpy(test_cont->words[0],"SETFUNC");
   strcpy(test_cont->words[1],"abc");
   strcpy(test_cont->words[2],"{");
   strcpy(test_cont->words[3],"A");
   strcpy(test_cont->words[4],"Z");
   strcpy(test_cont->words[5],"}");
   strcpy(test_cont->words[6],"{");
   strcpy(test_cont->words[7],"}");
   strcpy(test_cont->words[8],"abc");
   strcpy(test_cont->words[9],"-30");
   strcpy(test_cont->words[10],"}");
   TEST_ASSERT_TRUE(valid_funcset(test_cont));
   test_cont->position=0;
   TEST_ASSERT_TRUE(run_funcset(test_cont));
   TEST_ASSERT_TRUE(get_funcvar(test_cont,&test_func));
   TEST_ASSERT_TRUE(valid_argsrun(test_cont));
   test_cont->position=9;
   TEST_ASSERT_TRUE(!place_all_args(test_cont,test_func,0,test_line_cont));
   free_word_cont(test_cont);

   test_cont=init_word_cont();
   strcpy(test_cont->words[0],"SETFUNC");
   strcpy(test_cont->words[1],"abc");
   strcpy(test_cont->words[2],"{");
   strcpy(test_cont->words[3],"A");
   strcpy(test_cont->words[4],"}");
   strcpy(test_cont->words[5],"{");
   strcpy(test_cont->words[6],"}");
   strcpy(test_cont->words[7],"abc");
   strcpy(test_cont->words[8],"A");
   strcpy(test_cont->words[9],"}");
   TEST_ASSERT_TRUE(valid_funcset(test_cont));
   test_cont->position=0;
   TEST_ASSERT_TRUE(run_funcset(test_cont));
   TEST_ASSERT_TRUE(get_funcvar(test_cont,&test_func));
   TEST_ASSERT_TRUE(valid_argsrun(test_cont));
   test_cont->position=8;
   TEST_ASSERT_TRUE(!place_all_args(test_cont,test_func,0,test_line_cont));
   free_word_cont(test_cont);



   test_cont=init_word_cont();
   strcpy(test_cont->words[0],"SETFUNC");
   strcpy(test_cont->words[1],"abc");
   strcpy(test_cont->words[2],"{");
   strcpy(test_cont->words[3],"A");
   strcpy(test_cont->words[4],"}");
   strcpy(test_cont->words[5],"{");
   strcpy(test_cont->words[6],"}");
   strcpy(test_cont->words[7],"abc");
   strcpy(test_cont->words[8],"");
   strcpy(test_cont->words[9],"}");
   TEST_ASSERT_TRUE(run_funcset(test_cont));
   TEST_ASSERT_TRUE(get_funcvar(test_cont,&test_func));
   TEST_ASSERT_TRUE(!valid_argsrun(test_cont));
   test_cont->position=8;
   TEST_ASSERT_TRUE(!place_all_args(test_cont,test_func,0,test_line_cont));
   free_word_cont(test_cont);

   test_cont=init_word_cont();
   strcpy(test_cont->words[0],"SETFUNC");
   strcpy(test_cont->words[1],"abc");
   strcpy(test_cont->words[2],"{");
   strcpy(test_cont->words[3],"A");
   strcpy(test_cont->words[4],"}");
   strcpy(test_cont->words[5],"{");
   strcpy(test_cont->words[6],"}");
   strcpy(test_cont->words[7],"abc");
   strcpy(test_cont->words[8],"1");
   strcpy(test_cont->words[9],"");
   TEST_ASSERT_TRUE(run_funcset(test_cont));
   TEST_ASSERT_TRUE(get_funcvar(test_cont,&test_func));
   TEST_ASSERT_TRUE(!valid_argsrun(test_cont));
   test_cont->position=8;
   TEST_ASSERT_TRUE(!place_all_args(test_cont,test_func,0,test_line_cont));
   free_word_cont(test_cont);
   free_line_cont(test_line_cont);

}

void test_funcrun(void)
{
   word_cont* test_cont;
   line_cont* test_line_cont;
   double** test_d;
   test_d=NULL;
   test_cont=init_word_cont();
   test_line_cont=init_line_cont();
   strcpy(test_cont->words[0],"SETFUNC");
   strcpy(test_cont->words[1],"abc");
   strcpy(test_cont->words[2],"{");
   strcpy(test_cont->words[3],"A");
   strcpy(test_cont->words[4],"}");
   strcpy(test_cont->words[5],"{");
   strcpy(test_cont->words[6],"}");
   strcpy(test_cont->words[7],"abc");
   strcpy(test_cont->words[8],"{");
   strcpy(test_cont->words[9],"10");
   strcpy(test_cont->words[10],"}");
   TEST_ASSERT_TRUE(run_funcset(test_cont));

   TEST_ASSERT_TRUE(valid_funcrun(test_cont));
   test_cont->position=7;
   TEST_ASSERT_TRUE(run_funcrun(test_cont,test_line_cont,test_d));
   TEST_ASSERT_TRUE(test_cont->position==11);
   free_word_cont(test_cont);
   free_line_cont(test_line_cont);

   test_cont=init_word_cont();
   test_line_cont=init_line_cont();
   strcpy(test_cont->words[0],"SETFUNC");
   strcpy(test_cont->words[1],"abc");
   strcpy(test_cont->words[2],"{");
   strcpy(test_cont->words[3],"A");
   strcpy(test_cont->words[4],"}");
   strcpy(test_cont->words[5],"{");
   strcpy(test_cont->words[6],"}");
   strcpy(test_cont->words[7],"abc");
   strcpy(test_cont->words[8],"{");
   strcpy(test_cont->words[9],"ddddddd");
   strcpy(test_cont->words[10],"}");
   TEST_ASSERT_TRUE(run_funcset(test_cont));
   TEST_ASSERT_TRUE(!valid_funcrun(test_cont));
   test_cont->position=7;
   TEST_ASSERT_TRUE(!run_funcrun(test_cont,test_line_cont,test_d));

   free_word_cont(test_cont);
   free_line_cont(test_line_cont);

   test_cont=init_word_cont();
   test_line_cont=init_line_cont();
   strcpy(test_cont->words[0],"SETFUNC");
   strcpy(test_cont->words[1],"abc");
   strcpy(test_cont->words[2],"{");
   strcpy(test_cont->words[3],"A");
   strcpy(test_cont->words[4],"}");

   strcpy(test_cont->words[5],"{");
   strcpy(test_cont->words[6],"LT");
   strcpy(test_cont->words[7],"A");
   strcpy(test_cont->words[8],"FD");
   strcpy(test_cont->words[9],"A");
   strcpy(test_cont->words[10],"}");

   strcpy(test_cont->words[11],"abc");
   strcpy(test_cont->words[12],"{");
   strcpy(test_cont->words[13],"90");
   strcpy(test_cont->words[14],"}");
   strcpy(test_cont->words[15],"abc");
   strcpy(test_cont->words[16],"{");
   strcpy(test_cont->words[17],"90");
   strcpy(test_cont->words[18],"}");
   TEST_ASSERT_TRUE(run_funcset(test_cont));
   TEST_ASSERT_TRUE(run_funcrun(test_cont,test_line_cont,test_d));
   TEST_ASSERT_TRUE(test_cont->position==15);
   TEST_ASSERT_TRUE(run_funcrun(test_cont,test_line_cont,test_d));
   TEST_ASSERT_TRUE(test_line_cont->size==2);
   TEST_ASSERT_EQUAL_DOUBLE(test_line_cont->array[0]->end->x,-90);
   TEST_ASSERT_DOUBLE_WITHIN(EPSILON,test_line_cont->array[0]->end->y,0);
   TEST_ASSERT_EQUAL_DOUBLE(test_line_cont->array[1]->end->x,-90);
   TEST_ASSERT_EQUAL_DOUBLE(test_line_cont->array[1]->end->y,-90);
   free_word_cont(test_cont);
   free_line_cont(test_line_cont);


   test_cont=init_word_cont();
   test_line_cont=init_line_cont();
   strcpy(test_cont->words[0],"SETFUNC");
   strcpy(test_cont->words[1],"abc");
   strcpy(test_cont->words[2],"{");
   strcpy(test_cont->words[3],"A");
   strcpy(test_cont->words[4],"}");
   strcpy(test_cont->words[5],"{");
   strcpy(test_cont->words[6],"}");
   strcpy(test_cont->words[7],"abc");
   strcpy(test_cont->words[8],"{");
   strcpy(test_cont->words[9],"");
   strcpy(test_cont->words[10],"}");
   TEST_ASSERT_TRUE(run_funcset(test_cont));
   TEST_ASSERT_TRUE(!run_funcrun(test_cont,test_line_cont,test_d));
   free_word_cont(test_cont);
   free_line_cont(test_line_cont);


   test_cont=init_word_cont();
   test_line_cont=init_line_cont();
   strcpy(test_cont->words[0],"SETFUNC");
   strcpy(test_cont->words[1],"abc");
   strcpy(test_cont->words[2],"{");
   strcpy(test_cont->words[3],"A");
   strcpy(test_cont->words[4],"}");
   strcpy(test_cont->words[5],"{");
   strcpy(test_cont->words[6],"}");
   strcpy(test_cont->words[7],"abc");
   strcpy(test_cont->words[8],"");
   strcpy(test_cont->words[9],"{");
   strcpy(test_cont->words[10],"90");
   strcpy(test_cont->words[11],"}");
   TEST_ASSERT_TRUE(run_funcset(test_cont));
   TEST_ASSERT_TRUE(!run_funcrun(test_cont,test_line_cont,test_d));
   free_word_cont(test_cont);
   free_line_cont(test_line_cont);


   test_cont=init_word_cont();
   test_line_cont=init_line_cont();
   strcpy(test_cont->words[0],"SETFUNC");
   strcpy(test_cont->words[1],"abc");
   strcpy(test_cont->words[2],"{");
   strcpy(test_cont->words[3],"A");
   strcpy(test_cont->words[4],"}");
   strcpy(test_cont->words[5],"{");
   strcpy(test_cont->words[6],"}");
   strcpy(test_cont->words[7],"bca");
   strcpy(test_cont->words[8],"{");
   strcpy(test_cont->words[9],"90");
   strcpy(test_cont->words[10],"}");

   TEST_ASSERT_TRUE(run_funcset(test_cont));
   TEST_ASSERT_TRUE(valid_funcrun(test_cont));
   test_cont->position=7;
   TEST_ASSERT_TRUE(!run_funcrun(test_cont,test_line_cont,test_d));
   free_word_cont(test_cont);
   free_line_cont(test_line_cont);

   test_cont=init_word_cont();
   test_line_cont=init_line_cont();
   strcpy(test_cont->words[0],"SETFUNC");
   strcpy(test_cont->words[1],"abc");
   strcpy(test_cont->words[2],"{");
   strcpy(test_cont->words[3],"A");
   strcpy(test_cont->words[4],"}");
   strcpy(test_cont->words[5],"{");
   strcpy(test_cont->words[6],"}");
   strcpy(test_cont->words[7],"abc");
   strcpy(test_cont->words[8],"{");
   strcpy(test_cont->words[9],"}");

   TEST_ASSERT_TRUE(run_funcset(test_cont));
   TEST_ASSERT_TRUE(valid_funcrun(test_cont));
   test_cont->position=7;
   TEST_ASSERT_TRUE(!run_funcrun(test_cont,test_line_cont,test_d));
   free_word_cont(test_cont);
   free_line_cont(test_line_cont);
}

void test_main_no_returns(void)
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
   strcpy(test_cont->words[7],"}");

   strcpy(test_cont->words[8],"abc");
   strcpy(test_cont->words[9],"{");
   strcpy(test_cont->words[10],"90");
   strcpy(test_cont->words[11],"}");

   strcpy(test_cont->words[12],"}");
   TEST_ASSERT_TRUE(run_main(test_cont,test_line_cont));
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


   strcpy(test_cont->words[7],"FD");
   strcpy(test_cont->words[8],"A");
   strcpy(test_cont->words[9],"LT");
   strcpy(test_cont->words[10],"40");
   strcpy(test_cont->words[11],"}");

   strcpy(test_cont->words[12],"abc");
   strcpy(test_cont->words[13],"{");
   strcpy(test_cont->words[14],"90");
   strcpy(test_cont->words[15],"}");

   strcpy(test_cont->words[16],"}");


   TEST_ASSERT_TRUE(run_main(test_cont,test_line_cont));

   free_word_cont(test_cont);
   free_line_cont(test_line_cont);

   /*testing redefining functions that theres
   no memory leaks or whatever*/
   test_cont=init_word_cont();
   test_line_cont=init_line_cont();
   strcpy(test_cont->words[0],"{");

   strcpy(test_cont->words[1],"SETFUNC");
   strcpy(test_cont->words[2],"abc");
   strcpy(test_cont->words[3],"{");
   strcpy(test_cont->words[4],"A");
   strcpy(test_cont->words[5],"}");

   strcpy(test_cont->words[6],"{");

   strcpy(test_cont->words[7],"SETFUNC");
   strcpy(test_cont->words[8],"abc");
   strcpy(test_cont->words[9],"{");
   strcpy(test_cont->words[10],"A");
   strcpy(test_cont->words[11],"}");

   strcpy(test_cont->words[12],"{");
   strcpy(test_cont->words[13],"FD");
   strcpy(test_cont->words[14],"A");
   strcpy(test_cont->words[15],"}");


   strcpy(test_cont->words[16],"abc");
   strcpy(test_cont->words[17],"{");
   strcpy(test_cont->words[18],"A");
   strcpy(test_cont->words[19],"}");

   strcpy(test_cont->words[20],"}");

   strcpy(test_cont->words[21],"abc");
   strcpy(test_cont->words[22],"{");
   strcpy(test_cont->words[23],"90");
   strcpy(test_cont->words[24],"}");

   strcpy(test_cont->words[25],"}");
   TEST_ASSERT_TRUE(run_main(test_cont,test_line_cont));
   TEST_ASSERT_TRUE(test_line_cont->size==1);
   TEST_ASSERT_EQUAL_DOUBLE(test_line_cont->array[0]->end->y,90);
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

   strcpy(test_cont->words[7],"DO");
   strcpy(test_cont->words[8],"B");
   strcpy(test_cont->words[9],"FROM");
   strcpy(test_cont->words[10],"A");
   strcpy(test_cont->words[11],"TO");
   strcpy(test_cont->words[12],"9");
   strcpy(test_cont->words[13],"{");
   strcpy(test_cont->words[14],"FD");
   strcpy(test_cont->words[15],"A");
   strcpy(test_cont->words[16],"RT");
   strcpy(test_cont->words[17],"A");
   strcpy(test_cont->words[18],"}");

   strcpy(test_cont->words[19],"}");

   strcpy(test_cont->words[20],"abc");
   strcpy(test_cont->words[21],"{");
   strcpy(test_cont->words[22],"1");
   strcpy(test_cont->words[23],"}");

   strcpy(test_cont->words[24],"}");
   TEST_ASSERT_TRUE(run_main(test_cont,test_line_cont));
   TEST_ASSERT_TRUE(test_line_cont->size==9);
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

   strcpy(test_cont->words[7],"DO");
   strcpy(test_cont->words[8],"B");
   strcpy(test_cont->words[9],"FROM");
   strcpy(test_cont->words[10],"1");
   strcpy(test_cont->words[11],"TO");
   strcpy(test_cont->words[12],"9");
   strcpy(test_cont->words[13],"{");
   strcpy(test_cont->words[14],"FD");
   strcpy(test_cont->words[15],"A");
   strcpy(test_cont->words[16],"RT");
   strcpy(test_cont->words[17],"A");
   strcpy(test_cont->words[18],"}");

   strcpy(test_cont->words[19],"}");

   strcpy(test_cont->words[20],"DO");
   strcpy(test_cont->words[21],"B");
   strcpy(test_cont->words[22],"FROM");
   strcpy(test_cont->words[23],"1");

   strcpy(test_cont->words[24],"TO");
   strcpy(test_cont->words[25],"9");
   strcpy(test_cont->words[26],"{");
   strcpy(test_cont->words[27],"abc");
   strcpy(test_cont->words[28],"{");
   strcpy(test_cont->words[29],"1");
   strcpy(test_cont->words[30],"}");

   strcpy(test_cont->words[31],"}");
   strcpy(test_cont->words[32],"}");

   TEST_ASSERT_TRUE(run_main(test_cont,test_line_cont));
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

   strcpy(test_cont->words[7],"DO");
   strcpy(test_cont->words[8],"B");
   strcpy(test_cont->words[9],"FROM");
   strcpy(test_cont->words[10],"1");
   strcpy(test_cont->words[11],"TO");
   strcpy(test_cont->words[12],"9");
   strcpy(test_cont->words[13],"{");
   strcpy(test_cont->words[14],"FD");
   strcpy(test_cont->words[15],"A");
   strcpy(test_cont->words[16],"RT");
   strcpy(test_cont->words[17],"A");
   strcpy(test_cont->words[18],"}");

   strcpy(test_cont->words[19],"}");

   strcpy(test_cont->words[20],"SETFUNC");
   strcpy(test_cont->words[21],"xyz");
   strcpy(test_cont->words[22],"{");
   strcpy(test_cont->words[23],"}");

   strcpy(test_cont->words[24],"{");

   strcpy(test_cont->words[25],"abc");
   strcpy(test_cont->words[26],"{");
   strcpy(test_cont->words[27],"20");
   strcpy(test_cont->words[28],"}");

   strcpy(test_cont->words[29],"}");
   strcpy(test_cont->words[30],"xyz");

   strcpy(test_cont->words[31],"{");
   strcpy(test_cont->words[32],"}");
   strcpy(test_cont->words[33],"}");

   TEST_ASSERT_TRUE(run_main(test_cont,test_line_cont));
   free_word_cont(test_cont);
   free_line_cont(test_line_cont);


   test_cont=init_word_cont();
   test_line_cont=init_line_cont();
   strcpy(test_cont->words[0],"{");

   strcpy(test_cont->words[1],"SET");
   strcpy(test_cont->words[2],"A");
   strcpy(test_cont->words[3],":=");
   strcpy(test_cont->words[4],"9");
   strcpy(test_cont->words[5],";");

   strcpy(test_cont->words[6],"SETFUNC");

   strcpy(test_cont->words[7],"abc");
   strcpy(test_cont->words[8],"{");
   strcpy(test_cont->words[9],"A");
   strcpy(test_cont->words[10],"}");

   strcpy(test_cont->words[11],"{");
   strcpy(test_cont->words[12],"FD");
   strcpy(test_cont->words[13],"A");
   strcpy(test_cont->words[14],"}");
   strcpy(test_cont->words[15],"RT");
   strcpy(test_cont->words[16],"A");
   strcpy(test_cont->words[17],"abc");
   strcpy(test_cont->words[18],"{");
   strcpy(test_cont->words[19],"A");
   strcpy(test_cont->words[20],"}");
   strcpy(test_cont->words[21],"}");

   TEST_ASSERT_TRUE(run_main(test_cont,test_line_cont));
   free_word_cont(test_cont);
   free_line_cont(test_line_cont);

   test_cont=init_word_cont();
   test_line_cont=init_line_cont();
   strcpy(test_cont->words[0],"{");

   strcpy(test_cont->words[1],"SET");
   strcpy(test_cont->words[2],"A");
   strcpy(test_cont->words[3],":=");
   strcpy(test_cont->words[4],"9");
   strcpy(test_cont->words[5],";");

   strcpy(test_cont->words[6],"setfunc");

   strcpy(test_cont->words[7],"abc");
   strcpy(test_cont->words[8],"{");
   strcpy(test_cont->words[9],"A");
   strcpy(test_cont->words[10],"}");

   strcpy(test_cont->words[11],"{");
   strcpy(test_cont->words[12],"FD");
   strcpy(test_cont->words[13],"A");
   strcpy(test_cont->words[14],"}");
   strcpy(test_cont->words[15],"RT");
   strcpy(test_cont->words[16],"A");
   strcpy(test_cont->words[17],"abc");
   strcpy(test_cont->words[18],"{");
   strcpy(test_cont->words[19],"A");
   strcpy(test_cont->words[20],"}");
   strcpy(test_cont->words[21],"}");

   TEST_ASSERT_TRUE(!run_main(test_cont,test_line_cont));
   test_cont->position=0;
   strcpy(test_cont->words[6],"SETFUNC");
   strcpy(test_cont->words[7],"ABC");
   strcpy(test_cont->words[17],"ABC");
   TEST_ASSERT_TRUE(!run_main(test_cont,test_line_cont));
   free_word_cont(test_cont);
   free_line_cont(test_line_cont);


   test_cont=init_word_cont();
   test_line_cont=init_line_cont();
   strcpy(test_cont->words[0],"{");

   strcpy(test_cont->words[1],"SET");
   strcpy(test_cont->words[2],"A");
   strcpy(test_cont->words[3],":=");
   strcpy(test_cont->words[4],"9");
   strcpy(test_cont->words[5],";");

   strcpy(test_cont->words[6],"SETFUNC");

   strcpy(test_cont->words[7],"bac");
   strcpy(test_cont->words[8],"{");
   strcpy(test_cont->words[9],"A");
   strcpy(test_cont->words[10],"}");

   strcpy(test_cont->words[11],"{");
   strcpy(test_cont->words[12],"FD");
   strcpy(test_cont->words[13],"A");
   strcpy(test_cont->words[14],"}");
   strcpy(test_cont->words[15],"RT");
   strcpy(test_cont->words[16],"A");
   strcpy(test_cont->words[17],"abc");
   strcpy(test_cont->words[18],"{");
   strcpy(test_cont->words[19],"A");
   strcpy(test_cont->words[20],"}");
   strcpy(test_cont->words[21],"}");

   TEST_ASSERT_TRUE(!run_main(test_cont,test_line_cont));
   free_word_cont(test_cont);
   free_line_cont(test_line_cont);

   test_cont=init_word_cont();
   test_line_cont=init_line_cont();
   strcpy(test_cont->words[0],"{");

   strcpy(test_cont->words[1],"SET");
   strcpy(test_cont->words[2],"A");
   strcpy(test_cont->words[3],":=");
   strcpy(test_cont->words[4],"9");
   strcpy(test_cont->words[5],";");

   strcpy(test_cont->words[6],"SETFUNC");

   strcpy(test_cont->words[7],"abc");
   strcpy(test_cont->words[8],"{");
   strcpy(test_cont->words[9],"A");
   strcpy(test_cont->words[10],"}");

   strcpy(test_cont->words[11],"{");
   strcpy(test_cont->words[12],"FD");
   strcpy(test_cont->words[13],"A");
   strcpy(test_cont->words[14],"}");
   strcpy(test_cont->words[15],"RT");
   strcpy(test_cont->words[16],"A");
   strcpy(test_cont->words[17],"abc");
   strcpy(test_cont->words[18],"{");
   strcpy(test_cont->words[19],"A");
   strcpy(test_cont->words[20],"}");


   TEST_ASSERT_TRUE(!run_main(test_cont,test_line_cont));
   free_word_cont(test_cont);
   free_line_cont(test_line_cont);


   test_cont=init_word_cont();
   test_line_cont=init_line_cont();
   strcpy(test_cont->words[0],"SETFUNC");
   strcpy(test_cont->words[1],"abc");
   strcpy(test_cont->words[2],"{");
   strcpy(test_cont->words[3],"A");
   strcpy(test_cont->words[4],"}");

   strcpy(test_cont->words[5],"{");

   strcpy(test_cont->words[6],"SETFUNC");
   strcpy(test_cont->words[7],"xyz");
   strcpy(test_cont->words[8],"{");
   strcpy(test_cont->words[9],"}");
   strcpy(test_cont->words[10],"{");

   strcpy(test_cont->words[11],"FD");
   strcpy(test_cont->words[12],"10");

   strcpy(test_cont->words[13],"}");

   strcpy(test_cont->words[14],"}");

   strcpy(test_cont->words[15],"xyz");

   strcpy(test_cont->words[16],"{");
   strcpy(test_cont->words[17],"}");

   strcpy(test_cont->words[18],"}");
   TEST_ASSERT_TRUE(!run_instruction_list(test_cont,test_line_cont));
   free_word_cont(test_cont);
   free_line_cont(test_line_cont);
}


void test_valid_return(void)
{
   word_cont* test_cont;
   test_cont=init_word_cont();
   strcpy(test_cont->words[0],"RETURN");
   strcpy(test_cont->words[1],"A");
   TEST_ASSERT_TRUE(valid_return(test_cont));
   TEST_ASSERT_TRUE(test_cont->position==2);
   test_cont->position=0;
   strcpy(test_cont->words[1],"1");
   TEST_ASSERT_TRUE(valid_return(test_cont));
   test_cont->position=0;
   strcpy(test_cont->words[1],"-1");
   TEST_ASSERT_TRUE(valid_return(test_cont));
   test_cont->position=0;
   strcpy(test_cont->words[1],"0.1");
   TEST_ASSERT_TRUE(valid_return(test_cont));

   test_cont->position=0;
   strcpy(test_cont->words[1],"0.1A");
   TEST_ASSERT_TRUE(!valid_return(test_cont));
   test_cont->position=0;
   strcpy(test_cont->words[0],"return");
   TEST_ASSERT_TRUE(!valid_return(test_cont));
   test_cont->position=0;
   strcpy(test_cont->words[0],"");
   TEST_ASSERT_TRUE(!valid_return(test_cont));

   free_word_cont(test_cont);

}

void test_run_return(void)
{
   word_cont* test_cont;
   line_cont* test_line_cont;

   test_line_cont=init_line_cont();
   test_cont=init_word_cont();
   strcpy(test_cont->words[0],"RETURN");
   strcpy(test_cont->words[1],"12");
   TEST_ASSERT_TRUE(run_return(test_cont,test_line_cont));
   TEST_ASSERT_EQUAL_DOUBLE(*test_cont->return_val,12);
   free_word_cont(test_cont);

   test_cont=init_word_cont();
   strcpy(test_cont->words[0],"RETURN");
   strcpy(test_cont->words[1],"-12");
   TEST_ASSERT_TRUE(run_return(test_cont,test_line_cont));
   TEST_ASSERT_EQUAL_DOUBLE(*test_cont->return_val,-12);
   free_word_cont(test_cont);

   test_cont=init_word_cont();
   strcpy(test_cont->words[0],"RETURN");
   strcpy(test_cont->words[1],"1.2");
   TEST_ASSERT_TRUE(run_return(test_cont,test_line_cont));
   TEST_ASSERT_EQUAL_DOUBLE(*test_cont->return_val,1.2);
   free_word_cont(test_cont);

   test_cont=init_word_cont();
   strcpy(test_cont->words[0],"SET");
   strcpy(test_cont->words[1],"A");
   strcpy(test_cont->words[2],":=");
   strcpy(test_cont->words[3],"12");
   strcpy(test_cont->words[4],";");
   strcpy(test_cont->words[5],"RETURN");
   strcpy(test_cont->words[6],"A");
   TEST_ASSERT_TRUE(run_set(test_cont,test_line_cont));
   TEST_ASSERT_TRUE(run_return(test_cont,test_line_cont));
   TEST_ASSERT_TRUE(test_cont->position==7);
   TEST_ASSERT_EQUAL_DOUBLE(*test_cont->return_val,12);
   free_word_cont(test_cont);


   test_cont=init_word_cont();
   strcpy(test_cont->words[0],"RETURN");
   strcpy(test_cont->words[1],"A");
   TEST_ASSERT_TRUE(!run_return(test_cont,test_line_cont));
   test_cont->position=0;
   strcpy(test_cont->words[1],"ab");
   TEST_ASSERT_TRUE(!run_return(test_cont,test_line_cont));
   test_cont->position=0;
   strcpy(test_cont->words[0],"return");
   strcpy(test_cont->words[1],"1");
   TEST_ASSERT_TRUE(!run_return(test_cont,test_line_cont));
   test_cont->position=0;
   strcpy(test_cont->words[0],"RETURN");
   strcpy(test_cont->words[1],"");
   TEST_ASSERT_TRUE(!run_return(test_cont,test_line_cont));
   free_word_cont(test_cont);
   free_line_cont(test_line_cont);

   test_cont=init_word_cont();
   test_line_cont=init_line_cont();
   strcpy(test_cont->words[0],"SETFUNC");
   strcpy(test_cont->words[1],"abc");
   strcpy(test_cont->words[2],"{");
   strcpy(test_cont->words[3],"}");

   strcpy(test_cont->words[4],"{");
   strcpy(test_cont->words[5],"RETURN");
   strcpy(test_cont->words[6],"10");
   strcpy(test_cont->words[7],"}");

   strcpy(test_cont->words[8],"FD");
   strcpy(test_cont->words[9],"abc");
   strcpy(test_cont->words[10],"{");
   strcpy(test_cont->words[11],"}");
   TEST_ASSERT_TRUE(run_funcset(test_cont));
   TEST_ASSERT_TRUE(move_forward(test_cont,test_line_cont));
   TEST_ASSERT_EQUAL_DOUBLE(test_line_cont->array[0]->end->y,10);
   free_word_cont(test_cont);
   free_line_cont(test_line_cont);



   test_cont=init_word_cont();
   test_line_cont=init_line_cont();
   strcpy(test_cont->words[0],"SETFUNC");
   strcpy(test_cont->words[1],"abc");
   strcpy(test_cont->words[2],"{");
   strcpy(test_cont->words[3],"}");

   strcpy(test_cont->words[4],"{");
   strcpy(test_cont->words[5],"RETURN");
   strcpy(test_cont->words[6],"10");
   strcpy(test_cont->words[7],"}");

   strcpy(test_cont->words[8],"SETFUNC");
   strcpy(test_cont->words[9],"abc");
   strcpy(test_cont->words[10],"{");
   strcpy(test_cont->words[11],"}");

   strcpy(test_cont->words[12],"{");
   strcpy(test_cont->words[13],"RETURN");
   strcpy(test_cont->words[14],"20");
   strcpy(test_cont->words[15],"}");

   strcpy(test_cont->words[16],"FD");
   strcpy(test_cont->words[17],"abc");
   strcpy(test_cont->words[18],"{");
   strcpy(test_cont->words[19],"}");
   TEST_ASSERT_TRUE(run_funcset(test_cont));
   TEST_ASSERT_TRUE(run_funcset(test_cont));
   TEST_ASSERT_TRUE(move_forward(test_cont,test_line_cont));
   TEST_ASSERT_EQUAL_DOUBLE(test_line_cont->array[0]->end->y,20);
   free_word_cont(test_cont);
   free_line_cont(test_line_cont);

   test_cont=init_word_cont();
   test_line_cont=init_line_cont();
   strcpy(test_cont->words[0],"SETFUNC");
   strcpy(test_cont->words[1],"abc");
   strcpy(test_cont->words[2],"{");
   strcpy(test_cont->words[3],"}");

   strcpy(test_cont->words[4],"{");
   strcpy(test_cont->words[5],"RETURN");
   strcpy(test_cont->words[6],"90");
   strcpy(test_cont->words[7],"}");

   strcpy(test_cont->words[8],"LT");
   strcpy(test_cont->words[9],"abc");
   strcpy(test_cont->words[10],"{");
   strcpy(test_cont->words[11],"}");
   strcpy(test_cont->words[12],"FD");
   strcpy(test_cont->words[13],"abc");
   strcpy(test_cont->words[14],"{");
   strcpy(test_cont->words[15],"}");
   TEST_ASSERT_TRUE(run_funcset(test_cont));
   TEST_ASSERT_TRUE(run_instruction(test_cont,test_line_cont));
   TEST_ASSERT_TRUE(run_instruction(test_cont,test_line_cont));
   TEST_ASSERT_TRUE(test_cont->position==16);
   TEST_ASSERT_EQUAL_DOUBLE(test_line_cont->array[0]->end->x,-90);
   free_word_cont(test_cont);
   free_line_cont(test_line_cont);


   test_cont=init_word_cont();
   test_line_cont=init_line_cont();
   strcpy(test_cont->words[0],"SETFUNC");
   strcpy(test_cont->words[1],"abc");
   strcpy(test_cont->words[2],"{");
   strcpy(test_cont->words[3],"}");

   strcpy(test_cont->words[4],"{");
   strcpy(test_cont->words[5],"RETURN");
   strcpy(test_cont->words[6],"1");
   strcpy(test_cont->words[7],"}");

   strcpy(test_cont->words[8],"DO");
   strcpy(test_cont->words[9],"A");
   strcpy(test_cont->words[10],"FROM");
   strcpy(test_cont->words[11],"abc");
   strcpy(test_cont->words[12],"{");
   strcpy(test_cont->words[13],"}");
   strcpy(test_cont->words[14],"TO");
   strcpy(test_cont->words[15],"5");

   strcpy(test_cont->words[16],"{");
   strcpy(test_cont->words[17],"FD");
   strcpy(test_cont->words[18],"1");
   strcpy(test_cont->words[19],"}");

   TEST_ASSERT_TRUE(run_funcset(test_cont));
   TEST_ASSERT_TRUE(run_do(test_cont,test_line_cont));
   TEST_ASSERT_TRUE(test_cont->position==20);
   TEST_ASSERT_TRUE(test_line_cont->size==5);
   free_word_cont(test_cont);
   free_line_cont(test_line_cont);

   test_cont=init_word_cont();
   test_line_cont=init_line_cont();
   strcpy(test_cont->words[0],"SETFUNC");
   strcpy(test_cont->words[1],"abc");
   strcpy(test_cont->words[2],"{");
   strcpy(test_cont->words[3],"}");

   strcpy(test_cont->words[4],"{");
   strcpy(test_cont->words[5],"RETURN");
   strcpy(test_cont->words[6],"1");
   strcpy(test_cont->words[7],"}");

   strcpy(test_cont->words[8],"SET");
   strcpy(test_cont->words[9],"A");
   strcpy(test_cont->words[10],":=");
   strcpy(test_cont->words[11],"abc");
   strcpy(test_cont->words[12],"{");
   strcpy(test_cont->words[13],"}");
   strcpy(test_cont->words[14],";");
   strcpy(test_cont->words[15],"FD");
   strcpy(test_cont->words[16],"A");

   TEST_ASSERT_TRUE(run_funcset(test_cont));
   TEST_ASSERT_TRUE(run_set(test_cont,test_line_cont));
   TEST_ASSERT_TRUE(run_instruction(test_cont,test_line_cont));
   TEST_ASSERT_TRUE(test_cont->position==17);
   TEST_ASSERT_TRUE(test_line_cont->size==1);
   free_word_cont(test_cont);
   free_line_cont(test_line_cont);



   test_cont=init_word_cont();
   test_line_cont=init_line_cont();
   strcpy(test_cont->words[0],"SETFUNC");
   strcpy(test_cont->words[1],"abc");
   strcpy(test_cont->words[2],"{");
   strcpy(test_cont->words[3],"}");

   strcpy(test_cont->words[4],"{");
   strcpy(test_cont->words[5],"LT");
   strcpy(test_cont->words[6],"90");
   strcpy(test_cont->words[7],"RETURN");
   strcpy(test_cont->words[8],"90");
   strcpy(test_cont->words[9],"}");

   strcpy(test_cont->words[10],"FD");
   strcpy(test_cont->words[11],"abc");
   strcpy(test_cont->words[12],"{");
   strcpy(test_cont->words[13],"}");

   TEST_ASSERT_TRUE(run_funcset(test_cont));
   TEST_ASSERT_TRUE(run_instruction(test_cont,test_line_cont));
   TEST_ASSERT_TRUE(test_cont->position==14);
   TEST_ASSERT_EQUAL_DOUBLE(test_line_cont->array[0]->end->x,-90);
   free_word_cont(test_cont);
   free_line_cont(test_line_cont);



   test_cont=init_word_cont();
   test_line_cont=init_line_cont();
   strcpy(test_cont->words[0],"SETFUNC");
   strcpy(test_cont->words[1],"abc");
   strcpy(test_cont->words[2],"{");
   strcpy(test_cont->words[3],"}");

   strcpy(test_cont->words[4],"{");
   strcpy(test_cont->words[5],"RETURN");
   strcpy(test_cont->words[6],"90");
   strcpy(test_cont->words[7],"LT");
   strcpy(test_cont->words[8],"90");
   strcpy(test_cont->words[9],"}");

   strcpy(test_cont->words[10],"FD");
   strcpy(test_cont->words[11],"abc");
   strcpy(test_cont->words[12],"{");
   strcpy(test_cont->words[13],"}");

   TEST_ASSERT_TRUE(run_funcset(test_cont));
   TEST_ASSERT_TRUE(run_instruction(test_cont,test_line_cont));
   TEST_ASSERT_TRUE(test_cont->position==14);
   TEST_ASSERT_EQUAL_DOUBLE(test_line_cont->array[0]->end->y,90);
   free_word_cont(test_cont);
   free_line_cont(test_line_cont);

}

void test_copy_funcs(void)
{
   word_cont* test_cont;
   word_cont* test_func;
   word_cont* test_func2;
   line_cont* test_line_cont;
   int i;

   test_cont=init_word_cont();
   strcpy(test_cont->words[0],"SETFUNC");
   strcpy(test_cont->words[1],"abc");
   strcpy(test_cont->words[2],"{");
   strcpy(test_cont->words[3],"}");
   strcpy(test_cont->words[4],"SETFUNC");
   strcpy(test_cont->words[5],"abc");
   strcpy(test_cont->words[6],"{");
   strcpy(test_cont->words[7],"}");
   strcpy(test_cont->words[8],"}");
   test_func=init_func_cont();
   TEST_ASSERT_TRUE(copy_over_orig(test_cont,test_func));
   for(i=0;i<9;i++)
   {
      TEST_ASSERT_TRUE(strcmp(test_cont->words[i],test_func->words[i])==0);
   }

   free_word_cont(test_cont);
   free_word_cont(test_func);

   test_cont=init_word_cont();
   strcpy(test_cont->words[0],"SETFUNC");
   strcpy(test_cont->words[1],"abc");
   strcpy(test_cont->words[2],"{");
   strcpy(test_cont->words[3],"}");
   strcpy(test_cont->words[4],"SETFUNC");
   strcpy(test_cont->words[5],"abc");
   strcpy(test_cont->words[6],"{");
   strcpy(test_cont->words[7],"}");
   test_func=init_func_cont();
   TEST_ASSERT_TRUE(!copy_over_orig(test_cont,test_func));

   free_word_cont(test_cont);
   free_word_cont(test_func);

   test_cont=init_word_cont();
   strcpy(test_cont->words[0],"SETFUNC");
   strcpy(test_cont->words[1],"abc");

   strcpy(test_cont->words[2],"}");
   strcpy(test_cont->words[3],"SETFUNC");
   strcpy(test_cont->words[4],"abc");
   strcpy(test_cont->words[5],"{");
   strcpy(test_cont->words[6],"}");
   test_func=init_func_cont();
   TEST_ASSERT_TRUE(copy_over_orig(test_cont,test_func));

   free_word_cont(test_cont);
   free_word_cont(test_func);

   test_cont=init_word_cont();
   strcpy(test_cont->words[0],"SETFUNC");
   strcpy(test_cont->words[1],"abc");
   strcpy(test_cont->words[2],"{");
   strcpy(test_cont->words[3],"}");
   test_cont->n_args=3;
   test_func=deep_copy_func(test_cont);
   TEST_ASSERT_TRUE(strcmp(test_func->words[2],"{")==0);
   TEST_ASSERT_TRUE(test_func->capacity==test_cont->capacity);
   TEST_ASSERT_TRUE(test_func->n_args==3);
   free_copy(test_func);
   free_word_cont(test_cont);


   test_cont=init_word_cont();
   test_line_cont=init_line_cont();
   strcpy(test_cont->words[0],"SETFUNC");
   strcpy(test_cont->words[1],"abc");
   strcpy(test_cont->words[2],"{");
   strcpy(test_cont->words[3],"}");

   strcpy(test_cont->words[4],"{");
   strcpy(test_cont->words[5],"RETURN");
   strcpy(test_cont->words[6],"1");
   strcpy(test_cont->words[7],"}");

   strcpy(test_cont->words[8],"SET");
   strcpy(test_cont->words[9],"A");
   strcpy(test_cont->words[10],":=");
   strcpy(test_cont->words[11],"abc");
   strcpy(test_cont->words[12],"{");
   strcpy(test_cont->words[13],"}");
   strcpy(test_cont->words[14],";");
   strcpy(test_cont->words[15],"FD");
   strcpy(test_cont->words[16],"A");
   TEST_ASSERT_TRUE(run_funcset(test_cont));
   TEST_ASSERT_TRUE(run_set(test_cont,test_line_cont));
   TEST_ASSERT_TRUE(run_instruction(test_cont,test_line_cont));
   test_func=deep_copy_func(test_cont);
   test_func2=assoc_lookup(test_func->func_map,"abc");
   TEST_ASSERT_TRUE(strcmp(test_func->words[10],":=")==0);
   TEST_ASSERT_TRUE(strcmp(test_func->words[16],"A")==0);
   TEST_ASSERT_TRUE(test_func2);
   TEST_ASSERT_EQUAL_DOUBLE(*test_func->var_array[0],1);
   TEST_ASSERT_TRUE(test_func->stackptr!=test_cont->stackptr);
   TEST_ASSERT_TRUE(test_func->words!=test_cont->words);

   free_copy(test_func);
   free_word_cont(test_cont);
   free_line_cont(test_line_cont);

   free_copy(NULL);
   TEST_ASSERT_TRUE(!deep_copy_func(NULL));

}

void test_instructlist_returns(void)
{
   word_cont* test_cont;
   word_cont* test_func;
   word_cont* test_func2;
   line_cont* test_line_cont;
   test_cont=init_word_cont();
   test_line_cont=init_line_cont();
   strcpy(test_cont->words[0],"SETFUNC");
   strcpy(test_cont->words[1],"abc");
   strcpy(test_cont->words[2],"{");
   strcpy(test_cont->words[3],"A");
   strcpy(test_cont->words[4],"}");

   strcpy(test_cont->words[5],"{");

   strcpy(test_cont->words[6],"IF");
   strcpy(test_cont->words[7],"A");
   strcpy(test_cont->words[8],">");
   strcpy(test_cont->words[9],"5");
   strcpy(test_cont->words[10],"{");

   strcpy(test_cont->words[11],"SET");
   strcpy(test_cont->words[12],"B");
   strcpy(test_cont->words[13],":=");
   strcpy(test_cont->words[14],"A");
   strcpy(test_cont->words[15],"1");
   strcpy(test_cont->words[16],"-");
   strcpy(test_cont->words[17],";");

   strcpy(test_cont->words[18],"SET");
   strcpy(test_cont->words[19],"C");
   strcpy(test_cont->words[20],":=");
   strcpy(test_cont->words[21],"A");
   strcpy(test_cont->words[22],"1");
   strcpy(test_cont->words[23],"-");
   strcpy(test_cont->words[24],";");

   strcpy(test_cont->words[25],"FD");
   strcpy(test_cont->words[26],"10");

   strcpy(test_cont->words[27],"abc");
   strcpy(test_cont->words[28],"{");
   strcpy(test_cont->words[29],"B");
   strcpy(test_cont->words[30],"}");

   strcpy(test_cont->words[31],"}");

   strcpy(test_cont->words[32],"ELSE");
   strcpy(test_cont->words[33],"{");
   strcpy(test_cont->words[34],"RETURN");
   strcpy(test_cont->words[35],"10");
   strcpy(test_cont->words[36],"}");


   strcpy(test_cont->words[37],"}");

   strcpy(test_cont->words[38],"abc");
   strcpy(test_cont->words[39],"{");
   strcpy(test_cont->words[40],"8");
   strcpy(test_cont->words[41],"}");
   strcpy(test_cont->words[42],"}");

   TEST_ASSERT_TRUE(run_instruction_list(test_cont,test_line_cont));
   test_func=assoc_lookup(test_cont->func_map,"abc");
   test_func2=assoc_lookup(test_func->func_map,"abc");
   TEST_ASSERT_TRUE(!test_func2);
   TEST_ASSERT_TRUE(test_line_cont->size==3);
   free_word_cont(test_cont);
   free_line_cont(test_line_cont);


   test_cont=init_word_cont();
   test_line_cont=init_line_cont();
   strcpy(test_cont->words[0],"SETFUNC");
   strcpy(test_cont->words[1],"abc");
   strcpy(test_cont->words[2],"{");
   strcpy(test_cont->words[3],"A");
   strcpy(test_cont->words[4],"}");

   strcpy(test_cont->words[5],"{");

   strcpy(test_cont->words[6],"SET");
   strcpy(test_cont->words[7],"A");
   strcpy(test_cont->words[8],":=");
   strcpy(test_cont->words[9],"A");
   strcpy(test_cont->words[10],"10");

   strcpy(test_cont->words[11],"*");
   strcpy(test_cont->words[12],";");

   strcpy(test_cont->words[13],"RETURN");

   strcpy(test_cont->words[14],"A");
   strcpy(test_cont->words[15],"}");

   strcpy(test_cont->words[16],"IF");
   strcpy(test_cont->words[17],"100");

   strcpy(test_cont->words[18],"==");
   strcpy(test_cont->words[19],"abc");
   strcpy(test_cont->words[20],"{");
   strcpy(test_cont->words[21],"10");
   strcpy(test_cont->words[22],"}");
   strcpy(test_cont->words[23],"{");
   strcpy(test_cont->words[24],"FD");

   strcpy(test_cont->words[25],"-20.2");
   strcpy(test_cont->words[26],"}");

   strcpy(test_cont->words[27],"ELSE");
   strcpy(test_cont->words[28],"{");
   strcpy(test_cont->words[29],"FD");
   strcpy(test_cont->words[30],"20");

   strcpy(test_cont->words[31],"}");

   strcpy(test_cont->words[32],"}");

   TEST_ASSERT_TRUE(run_instruction_list(test_cont,test_line_cont));
   TEST_ASSERT_TRUE(test_line_cont->size==1);
   TEST_ASSERT_EQUAL_DOUBLE(test_line_cont->array[0]->end->y,-20.2);
   free_word_cont(test_cont);
   free_line_cont(test_line_cont);



}


int main(void)
{
   UNITY_BEGIN();
   RUN_TEST(test_valid_func_var);
   RUN_TEST(test_set_funcvar);
   RUN_TEST(test_get_arg);
   RUN_TEST(test_argset);
   RUN_TEST(test_resize);
   RUN_TEST(test_get_funcinfo);
   RUN_TEST(test_funcset);
   RUN_TEST(test_get_funcvar);
   RUN_TEST(test_place_arg);
   RUN_TEST(test_place_all_args);
   RUN_TEST(test_funcrun);
   RUN_TEST(test_main_no_returns);
   RUN_TEST(test_valid_return);
   RUN_TEST(test_run_return);
   RUN_TEST(test_copy_funcs);
   RUN_TEST(test_instructlist_returns);
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
