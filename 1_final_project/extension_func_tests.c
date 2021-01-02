#include "extension_funcs.h"
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
   char word[MAXTESTLEN];
   line_cont* test_line_cont;

   test_cont=init_word_cont();
   strcpy(test_cont->words[0],"aaabbbccc");
   assert(valid_funcvar(test_cont));
   strcpy(test_cont->words[0],"aaAaa");
   assert(!valid_funcvar(test_cont));
   strcpy(test_cont->words[0],"aa|aa");
   assert(!valid_funcvar(test_cont));
   strcpy(test_cont->words[0],"");
   assert(!valid_funcvar(test_cont));
   strcpy(test_cont->words[0],"z");
   assert(valid_funcvar(test_cont));

   strcpy(test_cont->words[0],"aaabbbccc");
   assert(set_funcvar(test_cont,word));
   assert(strcmp(word,"aaabbbccc")==0);
   assert(test_cont->position==1);

   strcpy(test_cont->words[0],"aaaAaa");
   test_cont->position=0;
   assert(!set_funcvar(test_cont,word));
   strcpy(test_cont->words[0],"aa|aa");
   assert(!set_funcvar(test_cont,word));
   strcpy(test_cont->words[0],"");
   assert(!set_funcvar(test_cont,word));

   strcpy(test_cont->words[0],"z");
   assert(set_funcvar(test_cont,word));
   assert(strcmp(word,"z")==0);

   test_func=init_func_cont();
   for(i=0;i<NUMVARS;i++)
   {
      assert(test_func->arg_placer[i]==UNUSED);
   }
   strcpy(test_cont->words[0],"A");
   test_cont->position=0;
   assert(get_arg(test_cont,test_func));
   assert(test_func->arg_placer[0]==0);
   assert(test_cont->position==1);
   test_cont->position=0;
   assert(!get_arg(test_cont,test_func));
   assert(strcmp(test_cont->err_message,"used same var twice when defining function")==0);
   free_word_cont(test_func);

   test_func=init_func_cont();
   strcpy(test_cont->words[0],"Z");
   assert(get_arg(test_cont,test_func));
   assert(test_func->arg_placer[0]==25);
   assert(test_cont->position==1);
   test_cont->position=0;
   strcpy(test_cont->words[0],"B");
   assert(get_arg(test_cont,test_func));
   assert(test_func->arg_placer[1]==1);

   test_cont->position=0;
   strcpy(test_cont->words[0],"a");
   assert(!get_arg(test_cont,test_func));

   test_cont->position=0;
   strcpy(test_cont->words[0],"@");
   assert(!get_arg(test_cont,test_func));

   test_cont->position=0;
   strcpy(test_cont->words[0],"");
   assert(!get_arg(test_cont,test_func));

   test_cont->position=0;
   strcpy(test_cont->words[0],"AA");
   assert(!get_arg(test_cont,test_func));

   free_word_cont(test_func);
   free_word_cont(test_cont);

   test_func=init_func_cont();
   test_cont=init_word_cont();

   strcpy(test_cont->words[0],"A");
   strcpy(test_cont->words[1],"B");
   strcpy(test_cont->words[2],"C");
   strcpy(test_cont->words[3],"}");
   assert(valid_argset(test_cont));
   test_cont->position=0;
   assert(get_argset(test_cont,test_func));
   assert(test_func->arg_placer[0]==0);
   assert(test_func->arg_placer[1]==1);
   assert(test_func->arg_placer[2]==2);
   assert(test_cont->position==4);

   free_word_cont(test_func);
   test_func=init_func_cont();

   test_cont->position=0;
   strcpy(test_cont->words[3],"D");
   assert(!valid_argset(test_cont));
   test_cont->position=0;
   assert(!get_argset(test_cont,test_func));


   free_word_cont(test_func);
   test_func=init_func_cont();

   test_cont->position=0;
   strcpy(test_cont->words[2],"B");
   strcpy(test_cont->words[3],"}");
   assert(valid_argset(test_cont));
   test_cont->position=0;
   assert(!get_argset(test_cont,test_func));
   assert(strcmp(test_cont->err_message,"used same var twice when defining function")==0);

   test_cont->position=0;
   free_word_cont(test_func);
   test_func=init_func_cont();
   strcpy(test_cont->words[0],"A");
   strcpy(test_cont->words[1],"DD");
   strcpy(test_cont->words[2],"C");
   strcpy(test_cont->words[3],"}");
   assert(!valid_argset(test_cont));
   test_cont->position=0;
   assert(!get_argset(test_cont,test_func));

   free_word_cont(test_func);

   test_func=init_func_cont();
   test_func->words[0]=safe_calloc(10,1);
   strcpy(test_func->words[0],"hello");

   test_func->words[2]=safe_calloc(10,1);
   strcpy(test_func->words[2],"goodbye");

   resize(test_func);
   assert(strcmp(test_func->words[0],"hello")==0);
   assert(strcmp(test_func->words[2],"goodbye")==0);
   free_word_cont(test_func);
   free_word_cont(test_cont);

   test_func=init_func_cont();
   test_cont=init_word_cont();

   strcpy(test_cont->words[0],"FD");
   strcpy(test_cont->words[1],"30");
   strcpy(test_cont->words[2],"LT");
   strcpy(test_cont->words[3],"20");
   strcpy(test_cont->words[4],"}");

   assert(get_func_info(test_cont,test_func));
   assert(strcmp(test_func->words[0],"FD")==0);
   assert(strcmp(test_func->words[4],"}")==0);
   assert(test_func->position==0);
   assert(test_cont->position==5);

   free_word_cont(test_func);
   free_word_cont(test_cont);


   test_func=init_func_cont();
   test_cont=init_word_cont();

   strcpy(test_cont->words[0],"FD");
   strcpy(test_cont->words[1],"30");
   strcpy(test_cont->words[2],"LT");
   strcpy(test_cont->words[3],"LT");
   strcpy(test_cont->words[4],"}");

   assert(!get_func_info(test_cont,test_func));
   test_cont->position=0;
   strcpy(test_cont->words[3],"30");
   strcpy(test_cont->words[4],"30");
   assert(!get_func_info(test_cont,test_func));
   test_cont->position=0;
   strcpy(test_cont->words[3],"");
   strcpy(test_cont->words[4],"}");
   assert(!get_func_info(test_cont,test_func));
   test_cont->position=0;

   strcpy(test_cont->words[0],"FD");
   strcpy(test_cont->words[1],"-30");
   strcpy(test_cont->words[2],"LT");
   strcpy(test_cont->words[3],"30");
   strcpy(test_cont->words[4],"}");
   assert(get_func_info(test_cont,test_func));
   free_word_cont(test_func);
   free_word_cont(test_cont);

   test_func=init_func_cont();
   test_cont=init_word_cont();
   strcpy(test_cont->words[0],"FD");
   strcpy(test_cont->words[1],"-30.907");
   strcpy(test_cont->words[2],"LT");
   strcpy(test_cont->words[3],"30");
   strcpy(test_cont->words[4],"}");
   assert(get_func_info(test_cont,test_func));
   free_word_cont(test_func);
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
   assert(run_funcset(test_cont));
   test_func=assoc_lookup(test_cont->func_map,"abc");
   assert(test_func);
   assert(strcmp(test_func->words[0],"FD")==0);
   assert(strcmp(test_func->words[1],"A")==0);
   assert(test_func->arg_placer[0]==0);
   assert(test_cont->position==9);
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
      assert(assoc_count(test_cont->func_map)==(unsigned int)i);
      assert(run_funcset(test_cont));


   }
   word[0]='\0';
   for(i=0;i<40;i++)
   {
      strcat(word,"a");
      assert(assoc_lookup(test_cont->func_map,word));
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
   assert(!valid_funcset(test_cont));
   test_cont->position=0;
   assert(!run_funcset(test_cont));

   strcpy(test_cont->words[1],"");
   test_cont->position=0;
   assert(!valid_funcset(test_cont));
   test_cont->position=0;
   assert(!run_funcset(test_cont));

   test_cont->position=0;
   strcpy(test_cont->words[1],"abc");
   strcpy(test_cont->words[3],"a");
   assert(!valid_funcset(test_cont));
   test_cont->position=0;
   assert(!run_funcset(test_cont));

   test_cont->position=0;
   strcpy(test_cont->words[3],"A");
   strcpy(test_cont->words[7],"dnjdbjfnekdddd");
   assert(!valid_funcset(test_cont));
   test_cont->position=0;
   assert(!run_funcset(test_cont));

   free_word_cont(test_cont);

   /*bit long to test the get funcvar function could just
   insert the functions but using runfuncset we can see
   how the functions will interact*/

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
   assert(run_funcset(test_cont));
   test_func=NULL;
   assert(get_funcvar(test_cont,&test_func));
   assert(test_func->position==0);
   assert(strcmp(test_func->words[0],"FD")==0);
   assert(strcmp(test_func->words[1],"A")==0);
   assert(strcmp(test_func->words[2],"}")==0);

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
   assert(run_funcset(test_cont));
   assert(get_funcvar(test_cont,&test_func));
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
   assert(run_funcset(test_cont));
   assert(get_funcvar(test_cont,&test_func));
   assert(test_func);
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
   assert(run_funcset(test_cont));
   assert(!get_funcvar(test_cont,&test_func));
   assert(strcmp(test_cont->err_message,"function name not found, has it been set?")==0);
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
   assert(run_funcset(test_cont));
   assert(!get_funcvar(test_cont,&test_func));
   free_word_cont(test_cont);

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
   assert(run_funcset(test_cont));
   assert(get_funcvar(test_cont,&test_func));
   assert(place_arg(test_cont,test_func,0,test_line_cont));
   /*as A was the first argument when we set the function
   we can expect to find 30 in position 0 in the var array */
   assert(compare_doubles(*test_func->var_array[0],30));
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
   assert(run_funcset(test_cont));
   assert(get_funcvar(test_cont,&test_func));
   assert(place_arg(test_cont,test_func,0,test_line_cont));
   assert(place_arg(test_cont,test_func,1,test_line_cont));
   assert(compare_doubles(*test_func->var_array[0],-30));
   assert(compare_doubles(*test_func->var_array[25],3.78));
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

   /*strcpy(test_cont->words[33],"-30");*/

   assert(run_funcset(test_cont));
   assert(get_funcvar(test_cont,&test_func));
   for(i=0;i<26;i++)
   {
      test_cont->position=33;
      sprintf(word, "%f", (double)i);
      strcpy(test_cont->words[33],word);
      assert(place_arg(test_cont,test_func,i,test_line_cont));
      assert(compare_doubles(*test_func->var_array[i],i));
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
   assert(run_funcset(test_cont));
   assert(get_funcvar(test_cont,&test_func));
   assert(!place_arg(test_cont,test_func,1,test_line_cont));
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
   assert(run_funcset(test_cont));
   assert(get_funcvar(test_cont,&test_func));
   assert(!place_arg(test_cont,test_func,0,test_line_cont));
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
   assert(run_funcset(test_cont));
   assert(get_funcvar(test_cont,&test_func));
   assert(!place_arg(test_cont,test_func,0,test_line_cont));
   assert(strcmp(test_cont->err_message,"potentially haven't set variable before calling it")==0);
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
   assert(valid_funcset(test_cont));
   test_cont->position=0;
   assert(run_funcset(test_cont));
   assert(get_funcvar(test_cont,&test_func));
   assert(!place_arg(test_cont,test_func,0,test_line_cont));
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
   assert(run_set(test_cont,test_line_cont));
   assert(run_funcset(test_cont));
   assert(get_funcvar(test_cont,&test_func));
   assert(place_arg(test_cont,test_func,0,test_line_cont));
   assert(compare_doubles(*test_func->var_array[0],90.5));
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
   strcpy(test_cont->words[14],"}");
   assert(run_set(test_cont,test_line_cont));
   assert(valid_funcset(test_cont));
   test_cont->position=5;
   assert(run_funcset(test_cont));
   assert(get_funcvar(test_cont,&test_func));
   assert(valid_argsrun(test_cont));
   test_cont->position=13;
   assert(place_all_args(test_cont,test_func,0,test_line_cont));
   assert(compare_doubles(*test_func->var_array[0],90.5));
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
   strcpy(test_cont->words[11],"}");
   assert(valid_funcset(test_cont));
   test_cont->position=0;
   assert(run_funcset(test_cont));
   assert(get_funcvar(test_cont,&test_func));
   assert(valid_argsrun(test_cont));
   test_cont->position=9;
   assert(place_all_args(test_cont,test_func,0,test_line_cont));
   assert(compare_doubles(*test_func->var_array[0],-30));
   assert(compare_doubles(*test_func->var_array[25],3.78));
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
   strcpy(test_cont->words[8],"-30");
   strcpy(test_cont->words[9],"}");
   assert(valid_funcset(test_cont));
   test_cont->position=0;
   assert(run_funcset(test_cont));
   assert(get_funcvar(test_cont,&test_func));
   assert(valid_argsrun(test_cont));
   test_cont->position=8;
   assert(place_all_args(test_cont,test_func,0,test_line_cont));
   assert(compare_doubles(*test_func->var_array[0],-30));
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
   strcpy(test_cont->words[8],"-30");
   strcpy(test_cont->words[9],"3.78");
   strcpy(test_cont->words[10],"}");
   assert(valid_funcset(test_cont));
   test_cont->position=0;
   assert(run_funcset(test_cont));
   assert(get_funcvar(test_cont,&test_func));
   assert(valid_argsrun(test_cont));
   test_cont->position=8;
   assert(!place_all_args(test_cont,test_func,0,test_line_cont));
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
   assert(valid_funcset(test_cont));
   test_cont->position=0;
   assert(run_funcset(test_cont));
   assert(get_funcvar(test_cont,&test_func));
   assert(valid_argsrun(test_cont));
   test_cont->position=9;
   assert(!place_all_args(test_cont,test_func,0,test_line_cont));
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
   assert(valid_funcset(test_cont));
   test_cont->position=0;
   assert(run_funcset(test_cont));
   assert(get_funcvar(test_cont,&test_func));
   assert(valid_argsrun(test_cont));
   test_cont->position=8;
   assert(!place_all_args(test_cont,test_func,0,test_line_cont));
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
   assert(run_funcset(test_cont));
   assert(get_funcvar(test_cont,&test_func));
   assert(!valid_argsrun(test_cont));
   test_cont->position=8;
   assert(!place_all_args(test_cont,test_func,0,test_line_cont));
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
   assert(run_funcset(test_cont));
   assert(get_funcvar(test_cont,&test_func));
   assert(!valid_argsrun(test_cont));
   test_cont->position=8;
   assert(!place_all_args(test_cont,test_func,0,test_line_cont));
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
   strcpy(test_cont->words[9],"10");
   strcpy(test_cont->words[10],"}");
   assert(run_funcset(test_cont));

   assert(valid_funcrun(test_cont));
   test_cont->position=7;
   assert(run_funcrun(test_cont,test_line_cont));
   assert(test_cont->position==11);
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
   assert(run_funcset(test_cont));
   assert(!valid_funcrun(test_cont));
   test_cont->position=7;
   assert(!run_funcrun(test_cont,test_line_cont));

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
   assert(run_funcset(test_cont));
   assert(run_funcrun(test_cont,test_line_cont));
   assert(test_cont->position==15);
   assert(run_funcrun(test_cont,test_line_cont));
   assert(test_line_cont->size==2);
   assert(compare_doubles(test_line_cont->array[0]->end->x,-90));
   assert(compare_doubles(test_line_cont->array[0]->end->y,0));
   assert(compare_doubles(test_line_cont->array[1]->end->x,-90));
   assert(compare_doubles(test_line_cont->array[1]->end->y,-90));
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
   assert(run_funcset(test_cont));
   assert(!run_funcrun(test_cont,test_line_cont));
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
   assert(run_funcset(test_cont));
   assert(!run_funcrun(test_cont,test_line_cont));
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

   assert(run_funcset(test_cont));
   assert(valid_funcrun(test_cont));
   test_cont->position=7;
   assert(!run_funcrun(test_cont,test_line_cont));
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

   assert(run_funcset(test_cont));
   assert(valid_funcrun(test_cont));
   test_cont->position=7;
   assert(!run_funcrun(test_cont,test_line_cont));
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
   strcpy(test_cont->words[7],"}");

   strcpy(test_cont->words[8],"abc");
   strcpy(test_cont->words[9],"{");
   strcpy(test_cont->words[10],"90");
   strcpy(test_cont->words[11],"}");

   strcpy(test_cont->words[12],"}");
   assert(run_main(test_cont,test_line_cont));
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


   assert(run_main(test_cont,test_line_cont));

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
   assert(run_main(test_cont,test_line_cont));
   assert(test_line_cont->size==1);
   assert(compare_doubles(test_line_cont->array[0]->end->y,90));
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
   assert(run_main(test_cont,test_line_cont));
   assert(test_line_cont->size==9);
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

   assert(run_main(test_cont,test_line_cont));
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

   assert(run_main(test_cont,test_line_cont));
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

   assert(run_main(test_cont,test_line_cont));
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

   assert(!run_main(test_cont,test_line_cont));
   test_cont->position=0;
   strcpy(test_cont->words[6],"SETFUNC");
   strcpy(test_cont->words[7],"ABC");
   strcpy(test_cont->words[17],"ABC");
   assert(!run_main(test_cont,test_line_cont));
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

   assert(!run_main(test_cont,test_line_cont));
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


   assert(!run_main(test_cont,test_line_cont));
   free_word_cont(test_cont);
   free_line_cont(test_line_cont);


   /*building in returns*/
   
   test_cont=init_word_cont();
   strcpy(test_cont->words[0],"RETURN");
   strcpy(test_cont->words[1],"A");
   assert(valid_return(test_cont));
   assert(test_cont->position==2);
   test_cont->position=0;
   strcpy(test_cont->words[1],"1");
   assert(valid_return(test_cont));
   test_cont->position=0;
   strcpy(test_cont->words[1],"-1");
   assert(valid_return(test_cont));
   test_cont->position=0;
   strcpy(test_cont->words[1],"0.1");
   assert(valid_return(test_cont));

   test_cont->position=0;
   strcpy(test_cont->words[1],"0.1A");
   assert(!valid_return(test_cont));
   test_cont->position=0;
   strcpy(test_cont->words[0],"return");
   assert(!valid_return(test_cont));
   test_cont->position=0;
   strcpy(test_cont->words[0],"");
   assert(!valid_return(test_cont));

   free_word_cont(test_cont);

   test_line_cont=init_line_cont();
   test_cont=init_word_cont();
   strcpy(test_cont->words[0],"RETURN");
   strcpy(test_cont->words[1],"12");
   assert(run_return(test_cont,test_line_cont));
   assert(compare_doubles(*test_cont->return_val,12));
   free_word_cont(test_cont);

   test_cont=init_word_cont();
   strcpy(test_cont->words[0],"RETURN");
   strcpy(test_cont->words[1],"-12");
   assert(run_return(test_cont,test_line_cont));
   assert(compare_doubles(*test_cont->return_val,-12));
   free_word_cont(test_cont);

   test_cont=init_word_cont();
   strcpy(test_cont->words[0],"RETURN");
   strcpy(test_cont->words[1],"1.2");
   assert(run_return(test_cont,test_line_cont));
   assert(compare_doubles(*test_cont->return_val,1.2));
   free_word_cont(test_cont);

   test_cont=init_word_cont();
   strcpy(test_cont->words[0],"SET");
   strcpy(test_cont->words[1],"A");
   strcpy(test_cont->words[2],":=");
   strcpy(test_cont->words[3],"12");
   strcpy(test_cont->words[4],";");
   strcpy(test_cont->words[5],"RETURN");
   strcpy(test_cont->words[6],"A");
   assert(run_set(test_cont,test_line_cont));
   assert(run_return(test_cont,test_line_cont));
   assert(test_cont->position==7);
   assert(compare_doubles(*test_cont->return_val,12));
   free_word_cont(test_cont);


   test_cont=init_word_cont();
   strcpy(test_cont->words[0],"RETURN");
   strcpy(test_cont->words[1],"A");
   assert(!run_return(test_cont,test_line_cont));
   test_cont->position=0;
   strcpy(test_cont->words[1],"ab");
   assert(!run_return(test_cont,test_line_cont));
   test_cont->position=0;
   strcpy(test_cont->words[0],"return");
   strcpy(test_cont->words[1],"1");
   assert(!run_return(test_cont,test_line_cont));
   test_cont->position=0;
   strcpy(test_cont->words[0],"RETURN");
   strcpy(test_cont->words[1],"");
   assert(!run_return(test_cont,test_line_cont));
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
   assert(run_funcset(test_cont));
   assert(move_forward(test_cont,test_line_cont));
   assert(compare_doubles(test_line_cont->array[0]->end->y,10));
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
   assert(run_funcset(test_cont));
   assert(run_instruction(test_cont,test_line_cont));
   assert(run_instruction(test_cont,test_line_cont));
   assert(test_cont->position==16);
   assert(compare_doubles(test_line_cont->array[0]->end->x,-90));
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

   assert(run_funcset(test_cont));
   assert(run_do(test_cont,test_line_cont));
   assert(test_cont->position==20);
   assert(test_line_cont->size==5);
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

   assert(run_funcset(test_cont));
   assert(run_set(test_cont,test_line_cont));
   assert(run_instruction(test_cont,test_line_cont));
   assert(test_cont->position==17);
   assert(test_line_cont->size==1);
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

   assert(run_funcset(test_cont));
   assert(run_instruction(test_cont,test_line_cont));
   assert(test_cont->position==14);
   assert(compare_doubles(test_line_cont->array[0]->end->x,-90));
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

   assert(run_funcset(test_cont));
   assert(run_instruction(test_cont,test_line_cont));
   assert(test_cont->position==14);
   assert(compare_doubles(test_line_cont->array[0]->end->y,90));
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
