#include "../src/unity.h"
#include "../debugger_funcs.h"



#define MAXTESTCAP 100
#define MAXTESTLEN 70


word_cont* init_word_cont(void);


void setUp(void)
{

}

void tearDown(void)
{

}

void test_step_instruction_basic(void)
{
   /*basic step instruction with just moves
   is very similar to run instruction- pretty much the same*/
   debugger* test_d;
   test_d=init_debugger();

   test_d->program = init_word_cont();
   test_d->output= init_line_cont();
   strcpy(test_d->program->words[0],"FD");
   strcpy(test_d->program->words[1],"10");
   TEST_ASSERT_TRUE(step_instruction(test_d));
   TEST_ASSERT_TRUE(test_d->program->position==2);
   TEST_ASSERT_EQUAL_DOUBLE(test_d->output->array[0]->end->y,10);
   free_debugger(test_d);

   test_d=init_debugger();
   test_d->program = init_word_cont();
   test_d->output= init_line_cont();
   strcpy(test_d->program->words[0],"FD");
   strcpy(test_d->program->words[1],"-10");
   TEST_ASSERT_TRUE(step_instruction(test_d));
   TEST_ASSERT_EQUAL_DOUBLE(test_d->output->array[0]->end->y,-10);
   free_debugger(test_d);


   test_d=init_debugger();
   test_d->program = init_word_cont();
   test_d->output= init_line_cont();
   strcpy(test_d->program->words[0],"FD");
   strcpy(test_d->program->words[1],"1.1");
   TEST_ASSERT_TRUE(step_instruction(test_d));
   TEST_ASSERT_EQUAL_DOUBLE(test_d->output->array[0]->end->y,1.1);
   free_debugger(test_d);

   test_d=init_debugger();
   test_d->program = init_word_cont();
   test_d->output= init_line_cont();
   strcpy(test_d->program->words[0],"FD");
   strcpy(test_d->program->words[1],"-1.1");
   TEST_ASSERT_TRUE(step_instruction(test_d));
   TEST_ASSERT_EQUAL_DOUBLE(test_d->output->array[0]->end->y,-1.1);
   free_debugger(test_d);


   test_d=init_debugger();
   test_d->program = init_word_cont();
   test_d->output= init_line_cont();
   strcpy(test_d->program->words[0],"FD");
   strcpy(test_d->program->words[1],".1");
   TEST_ASSERT_TRUE(step_instruction(test_d));
   TEST_ASSERT_EQUAL_DOUBLE(test_d->output->array[0]->end->y,0.1);
   free_debugger(test_d);


   test_d=init_debugger();
   test_d->program = init_word_cont();
   test_d->output= init_line_cont();
   strcpy(test_d->program->words[0],"LT");
   strcpy(test_d->program->words[1],".1");
   TEST_ASSERT_TRUE(step_instruction(test_d));
   TEST_ASSERT_EQUAL_DOUBLE(test_d->output->pending_line->rotation,0.1);
   free_debugger(test_d);

   test_d=init_debugger();
   test_d->program = init_word_cont();
   test_d->output= init_line_cont();
   strcpy(test_d->program->words[0],"RT");
   strcpy(test_d->program->words[1],".1");
   TEST_ASSERT_TRUE(step_instruction(test_d));
   TEST_ASSERT_EQUAL_DOUBLE(test_d->output->pending_line->rotation,359.9);
   free_debugger(test_d);

   test_d=init_debugger();
   test_d->program = init_word_cont();
   test_d->output= init_line_cont();
   strcpy(test_d->program->words[0],"RT");
   strcpy(test_d->program->words[1],"-.1");
   TEST_ASSERT_TRUE(step_instruction(test_d));
   TEST_ASSERT_EQUAL_DOUBLE(test_d->output->pending_line->rotation,0.1);
   free_debugger(test_d);

   test_d=init_debugger();
   test_d->program = init_word_cont();
   test_d->output= init_line_cont();
   strcpy(test_d->program->words[0],"LT");
   strcpy(test_d->program->words[1],"-.1");
   TEST_ASSERT_TRUE(step_instruction(test_d));
   TEST_ASSERT_EQUAL_DOUBLE(test_d->output->pending_line->rotation,359.9);
   free_debugger(test_d);

   test_d=init_debugger();
   test_d->program = init_word_cont();
   test_d->output= init_line_cont();
   strcpy(test_d->program->words[0],"LT");
   strcpy(test_d->program->words[1],"1-.1");
   TEST_ASSERT_FALSE(step_instruction(test_d));
   free_debugger(test_d);

   test_d=init_debugger();
   test_d->program = init_word_cont();
   test_d->output= init_line_cont();
   strcpy(test_d->program->words[0],"FD");
   strcpy(test_d->program->words[1],"1-");
   TEST_ASSERT_FALSE(step_instruction(test_d));
   free_debugger(test_d);

   test_d=init_debugger();
   test_d->program = init_word_cont();
   test_d->output= init_line_cont();
   strcpy(test_d->program->words[0],"FD");
   strcpy(test_d->program->words[1],"");
   TEST_ASSERT_FALSE(step_instruction(test_d));
   free_debugger(test_d);

   test_d=init_debugger();
   test_d->program = init_word_cont();
   test_d->output= init_line_cont();
   strcpy(test_d->program->words[0],"FD");
   strcpy(test_d->program->words[1],"1.1.");
   TEST_ASSERT_FALSE(step_instruction(test_d));
   free_debugger(test_d);
}

void test_loops(void)
{
   debugger* test_d;
   int i;
   loop_tracker* test_t;
   test_d=init_debugger();
   test_d->program = init_word_cont();
   test_d->output= init_line_cont();
   strcpy(test_d->program->words[0],"FD");
   strcpy(test_d->program->words[1],"1.1.");
   strcpy(test_d->program->words[2],"FD");
   strcpy(test_d->program->words[3],"1.1.");
   strcpy(test_d->program->words[4],"FD");
   strcpy(test_d->program->words[5],"1.1.");
   strcpy(test_d->program->words[6],"FD");
   strcpy(test_d->program->words[7],"1.1.");
   strcpy(test_d->program->words[8],"}");
   i=find_end_pos(test_d->program);
   TEST_ASSERT_TRUE(i==9);

   strcpy(test_d->program->words[0],"FD");
   strcpy(test_d->program->words[1],"1.1.");
   strcpy(test_d->program->words[2],"{");
   strcpy(test_d->program->words[3],"{");
   strcpy(test_d->program->words[4],"}");
   strcpy(test_d->program->words[5],"}");
   strcpy(test_d->program->words[6],"FD");
   strcpy(test_d->program->words[7],"1");
   strcpy(test_d->program->words[8],"}");
   i=find_end_pos(test_d->program);

   TEST_ASSERT_TRUE(i==9);

   strcpy(test_d->program->words[0],"FD");
   strcpy(test_d->program->words[1],"1.1.");
   strcpy(test_d->program->words[2],"2");
   strcpy(test_d->program->words[3],"{");
   strcpy(test_d->program->words[4],"{");
   strcpy(test_d->program->words[5],"}");
   strcpy(test_d->program->words[6],"FD");
   strcpy(test_d->program->words[7],"1");
   strcpy(test_d->program->words[8],"}");
   i=find_end_pos(test_d->program);
   TEST_ASSERT_TRUE(i==-1);

   strcpy(test_d->program->words[0],"FD");
   strcpy(test_d->program->words[1],"{");
   strcpy(test_d->program->words[2],"}");
   strcpy(test_d->program->words[3],"{");
   strcpy(test_d->program->words[4],"}");
   strcpy(test_d->program->words[5],"1");
   strcpy(test_d->program->words[6],"FD");
   strcpy(test_d->program->words[7],"1");
   strcpy(test_d->program->words[8],"}");
   i=find_end_pos(test_d->program);
   TEST_ASSERT_TRUE(i==9);

   strcpy(test_d->program->words[0],"}");
   i=find_end_pos(test_d->program);
   TEST_ASSERT_TRUE(i==1);

   free_debugger(test_d);

   test_d=init_debugger();
   test_d->program = init_word_cont();
   test_d->output= init_line_cont();
   strcpy(test_d->program->words[0],"DO");
   strcpy(test_d->program->words[1],"A");
   strcpy(test_d->program->words[2],"FROM");
   strcpy(test_d->program->words[3],"1");
   strcpy(test_d->program->words[4],"TO");
   strcpy(test_d->program->words[5],"10");
   strcpy(test_d->program->words[6],"{");
   strcpy(test_d->program->words[7],"}");
   test_t=step_do_helper(test_d->program,test_d->output);
   TEST_ASSERT_TRUE(test_t);
   TEST_ASSERT_EQUAL_DOUBLE(test_t->start_val,1);
   TEST_ASSERT_EQUAL_DOUBLE(test_t->end_val,10);
   TEST_ASSERT_TRUE(test_t->start_pos==7);
   TEST_ASSERT_TRUE(test_t->end_pos==8);
   TEST_ASSERT_TRUE(test_t->var_pos==0);
   free(test_t);
   test_d->program->position=0;
   
   TEST_ASSERT_TRUE(step_do(test_d));
   TEST_ASSERT_TRUE(test_d->loop_stack->size==1);

   TEST_ASSERT_EQUAL_DOUBLE(((loop_tracker*)test_d->loop_stack->start->data)->start_val,1);

   /*TEST_ASSERT_TRUE(step_do(test_d));
   TEST_ASSERT_TRUE(test_d->loop_stack->size==1);
   TEST_ASSERT_EQUAL_DOUBLE(((loop_tracker*)test_d->loop_stack->start)->start_val,1);
   TEST_ASSERT_EQUAL_DOUBLE(((loop_tracker*)test_d->loop_stack->start)->end_val,10);
   TEST_ASSERT_TRUE(((loop_tracker*)test_d->loop_stack->start)->start_pos==7);
   TEST_ASSERT_TRUE(((loop_tracker*)test_d->loop_stack->start)->end_pos==8);
   TEST_ASSERT_TRUE(((loop_tracker*)test_d->loop_stack->start)->var_pos==0);
   free_debugger(test_d);*/
}


int main(void)
{
   UNITY_BEGIN();
   RUN_TEST(test_step_instruction_basic);
   RUN_TEST(test_loops);
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
   n_cont->arr_map=assoc_init();
   return n_cont;
}
