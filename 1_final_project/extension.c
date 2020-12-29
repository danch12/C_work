#include "extension.h"

#define MAXTESTCAP 100
#define MAXTESTLEN 50
#define EPSILON 0.00001

bool set_funcvar(word_cont* to_check,char func_name[MAXFUNCLEN]);
bool valid_funcvar(word_cont* to_check);

bool valid_argset(word_cont* to_check);
bool get_argset(word_cont* to_check,word_cont* n_func);
bool get_arg(word_cont* to_check,word_cont* n_func);
bool get_func_info(word_cont* to_check,word_cont* n_func);
bool valid_funcset(word_cont* to_check);
bool run_funcset(word_cont* to_check);
bool copy_over(word_cont* to_check,word_cont* n_func);

bool get_funcvar(word_cont* to_check,word_cont** to_get);
bool place_arg(word_cont* to_check,word_cont* n_func,\
               int position);
bool place_all_args(word_cont* to_check,word_cont* n_func,int pos);
void reset_func(word_cont* func);
bool valid_argsrun(word_cont* to_check);
bool valid_funcrun(word_cont* to_check);
bool run_funcrun(word_cont* to_check,line_cont* line_arr);

word_cont* init_func_cont(void);
bool free_word_cont(word_cont* to_free);
word_cont* init_word_cont(void);
void resize(word_cont* n_func);


bool compare_doubles(double d_1, double d_2);
void test(void);

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
   assert(place_arg(test_cont,test_func,0));
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
   assert(place_arg(test_cont,test_func,0));
   assert(place_arg(test_cont,test_func,1));
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
      assert(place_arg(test_cont,test_func,i));
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
   assert(!place_arg(test_cont,test_func,1));
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
   assert(!place_arg(test_cont,test_func,0));
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
   assert(!place_arg(test_cont,test_func,0));
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
   assert(!place_arg(test_cont,test_func,0));
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
   assert(run_set(test_cont));
   assert(run_funcset(test_cont));
   assert(get_funcvar(test_cont,&test_func));
   assert(place_arg(test_cont,test_func,0));
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
   assert(run_set(test_cont));
   assert(valid_funcset(test_cont));
   test_cont->position=5;
   assert(run_funcset(test_cont));
   assert(get_funcvar(test_cont,&test_func));
   assert(valid_argsrun(test_cont));
   test_cont->position=13;
   assert(place_all_args(test_cont,test_func,0));
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
   assert(place_all_args(test_cont,test_func,0));
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
   assert(place_all_args(test_cont,test_func,0));
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
   assert(!place_all_args(test_cont,test_func,0));
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
   assert(!place_all_args(test_cont,test_func,0));
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
   assert(!place_all_args(test_cont,test_func,0));
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
   assert(!place_all_args(test_cont,test_func,0));
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
   assert(!place_all_args(test_cont,test_func,0));
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


word_cont* init_func_cont(void)
{
   word_cont* n_cont;
   int i;
   n_cont=(word_cont*)safe_calloc(1,sizeof(word_cont));
   n_cont->capacity=INITFUNCCAP;
   n_cont->words=(char**)safe_calloc(INITFUNCCAP,sizeof(char*));
   n_cont->position=0;
   for(i=0;i<NUMVARS;i++)
   {
      n_cont->var_array[i]=NULL;
      n_cont->arg_placer[i]=UNUSED;
   }
   n_cont->func_map=assoc_init();
   n_cont->stackptr=stack_init();
   n_cont->n_args=0;
   return n_cont;
}



bool valid_funcvar(word_cont* to_check)
{
   int i;
   if(to_check->position>=to_check->capacity)
   {
      return false;
   }
   i=0;
   if(strlen(to_check->words[to_check->position])==0)
   {
      return false;
   }
   while(to_check->words[to_check->position][i])
   {
      if(!(to_check->words[to_check->position][i]>='a')||\
         !(to_check->words[to_check->position][i]<='z'))
      {
         return false;
      }
      i++;
   }
   return true;
}

bool set_funcvar(word_cont* to_check,char func_name[MAXFUNCLEN])
{
   if(valid_funcvar(to_check))
   {
      strcpy(func_name,to_check->words[to_check->position]);
      to_check->position++;
      return true;
   }
   return false;
}

bool valid_funcset(word_cont* to_check)
{
   if(strcmp(to_check->words[to_check->position],"SETFUNC")==0)
   {
      to_check->position++;
      if(valid_funcvar(to_check))
      {
         to_check->position++;
         if(strcmp(to_check->words[to_check->position],"{")==0)
         {
            to_check->position++;
            if(valid_argset(to_check))
            {
               if(strcmp(to_check->words[to_check->position],"{")==0)
               {
                  to_check->position++;
                  {
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

bool get_funcvar(word_cont* to_check,word_cont** to_get)
{
   if(valid_funcvar(to_check))
   {
      *to_get=assoc_lookup(to_check->func_map,\
                           to_check->words[to_check->position]);
      if(*to_get==NULL)
      {
         strcpy(to_check->err_message,"function name not found, has it been set?");
         return false;
      }
      to_check->position++;
      return true;
   }
   return false;
}

/*downside is that we will only see errors like using undeclared vars
when the function is called not when it is set*/
bool get_func_info(word_cont* to_check,word_cont* n_func)
{
   int initial_pos;
   initial_pos=to_check->position;
   if(valid_instructlist(to_check))
   {

      to_check->position=initial_pos;
      if(copy_over(to_check,n_func))
      {
         n_func->position=0;
         return true;
      }
      n_func->position=0;
      return true;
   }
   return false;
}

/*there are better ways to balance brackets out there
but this will be adequate*/
bool copy_over(word_cont* to_check,word_cont* n_func)
{

   int left_brackets,right_brackets;
   left_brackets=START_BRACKET;
   right_brackets=0;
   while(left_brackets!=right_brackets)
   {
      if(to_check->position>=to_check->capacity)
      {
         return false;
      }
      if(strcmp(to_check->words[to_check->position],"}")==0)
      {
         right_brackets++;
      }
      if(strcmp(to_check->words[to_check->position],"{")==0)
      {
         left_brackets++;
      }
      n_func->words[n_func->position]=(char*)safe_calloc(\
            strlen(to_check->words[to_check->position])+1,sizeof(char));

      strcpy(n_func->words[n_func->position],\
            to_check->words[to_check->position]);

      n_func->position++;
      to_check->position++;
      if(n_func->position>=n_func->capacity)
      {
         resize(n_func);
      }
   }

   return true;
}

void resize(word_cont* n_func)
{
   char** n_words;
   char** temp;
   int i;
   n_words=(char**)safe_calloc(n_func->capacity*FUNCSCALE,\
                              sizeof(char*));
   for(i=0;i<n_func->capacity;i++)
   {
      if(n_func->words[i])
      {
         n_words[i]=(char*)safe_calloc(strlen(n_func->words[i])+1,\
                                       sizeof(char));
         strcpy(n_words[i],n_func->words[i]);
      }
   }
   temp=n_func->words;
   n_func->words=n_words;

   for(i=0;i<n_func->capacity;i++)
   {
      if(temp[i])
      {
         free(temp[i]);
      }

   }
   n_func->capacity=n_func->capacity*FUNCSCALE;
   free(temp);
}





/*need to test setting loads of functions see if resizing
breaks it*/


bool run_funcset(word_cont* to_check)
{
   word_cont* n_func;
   char* func_name;
   func_name=(char*)safe_calloc(MAXFUNCLEN,sizeof(char));
   n_func=init_func_cont();
   if(strcmp(to_check->words[to_check->position],"SETFUNC")==0)
   {

      to_check->position++;
      if(set_funcvar(to_check,func_name))
      {

         if(strcmp(to_check->words[to_check->position],"{")==0)
         {

            to_check->position++;
            if(get_argset(to_check,n_func))
            {
               if(strcmp(to_check->words[to_check->position],"{")==0)
               {
                  to_check->position++;
                  if(get_func_info(to_check,n_func))
                  {
                     assoc_insert(&to_check->func_map,func_name,\
                                 n_func);
                     return true;
                  }
               }
            }
         }
      }
   }
   free_word_cont(n_func);
   free(func_name);
   return false;
}


bool valid_argset(word_cont* to_check)
{
   if(strcmp(to_check->words[to_check->position],"}")==0)
   {
      to_check->position++;
      return true;
   }
   if(valid_variable(to_check))
   {
      to_check->position++;
      if(valid_argset(to_check))
      {
         return true;
      }
   }
   return false;
}


bool get_argset(word_cont* to_check,word_cont* n_func)
{
   if(strcmp(to_check->words[to_check->position],"}")==0)
   {
      to_check->position++;
      return true;
   }
   if(get_arg(to_check,n_func))
   {
      if(get_argset(to_check,n_func))
      {
         return true;
      }
   }
   return false;
}

bool get_arg(word_cont* to_check,word_cont* n_func)
{

   int i;
   int var_pos;

   if(valid_variable(to_check))
   {
      i=0;
      var_pos=to_check->words[to_check->position][0]-LETTER_TO_NUM;
      /*find first empty position in arg array*/
      while(n_func->arg_placer[i]!=UNUSED)
      {
         /*if var already used in arguments*/
         if(n_func->arg_placer[i]==var_pos)
         {
            strcpy(to_check->err_message,"used same var twice when defining function");
            return false;
         }
         i++;
      }
      n_func->arg_placer[i]= var_pos;
      n_func->n_args++;
      to_check->position++;
      return true;
   }
   return false;
}


bool place_arg(word_cont* to_check,word_cont* n_func,\
               int position)
{
   double to_place;
   int func_var;
   if(get_varnum(to_check,&to_place))
   {
      func_var= n_func->arg_placer[position];
      if(func_var==UNUSED)
      {
         strcpy(to_check->err_message,"too many arguments for function");
         return false;
      }
      n_func->var_array[func_var]=(double*)safe_calloc(1,\
                                 sizeof(double));
      *n_func->var_array[func_var]=to_place;
      return true;
   }
   return false;
}


bool place_all_args(word_cont* to_check,word_cont* n_func,int pos)
{

   if(strcmp(to_check->words[to_check->position],"}")==0)
   {

      if(pos!=n_func->n_args)
      {
         strcpy(to_check->err_message,"incorrect amount of args passed to function");
         return false;
      }
      to_check->position++;
      return true;
   }
   if(place_arg(to_check,n_func,pos))
   {
      if(place_all_args(to_check,n_func,pos+1))
      {
         return true;
      }
   }
   return false;
}

/*the function name sounds silly but
keeps the theme*/

bool run_funcrun(word_cont* to_check,line_cont* line_arr)
{
   word_cont* to_run;
   to_run=NULL;
   if(get_funcvar(to_check,&to_run))
   {
      if(strcmp(to_check->words[to_check->position],"{")==0)
      {
         to_check->position++;
         if(place_all_args(to_check,to_run,START))
         {
            if(run_instruction_list(to_run,line_arr))
            {
               /*set back to beginning*/
               reset_func(to_run);
               return true;
            }
         }
      }
   }
   return false;
}


bool valid_argsrun(word_cont* to_check)
{
   if(strcmp(to_check->words[to_check->position],"}")==0)
   {
      to_check->position++;
      return true;
   }
   if(valid_varnum(to_check))
   {
      if(valid_argsrun(to_check))
      {
         return true;
      }
   }
   return false;
}

bool valid_funcrun(word_cont* to_check)
{
   if(valid_funcvar(to_check))
   {
      to_check->position++;
      if(strcmp(to_check->words[to_check->position],"{")==0)
      {
         to_check->position++;
         if(valid_argsrun(to_check))
         {
            return true;
         }
      }
   }
   return false;

}

void reset_func(word_cont* func)
{
   int i;
   func->position=0;
   for(i=0;i<NUMVARS;i++)
   {
      if(func->var_array[i])
      {
         free(func->var_array[i]);
         func->var_array[i]=NULL;
      }
   }
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
