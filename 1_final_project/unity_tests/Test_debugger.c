#include "../src/unity.h"
#include "../debugger_funcs.h"



#define MAXTESTCAP 150
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
   TEST_ASSERT_EQUAL_DOUBLE(test_d->output->pending_line->rotation,359.9);
   free_debugger(test_d);

   test_d=init_debugger();
   test_d->program = init_word_cont();
   test_d->output= init_line_cont();
   strcpy(test_d->program->words[0],"RT");
   strcpy(test_d->program->words[1],".1");
   TEST_ASSERT_TRUE(step_instruction(test_d));
   TEST_ASSERT_EQUAL_DOUBLE(test_d->output->pending_line->rotation,0.1);
   free_debugger(test_d);

   test_d=init_debugger();
   test_d->program = init_word_cont();
   test_d->output= init_line_cont();
   strcpy(test_d->program->words[0],"RT");
   strcpy(test_d->program->words[1],"-.1");
   TEST_ASSERT_TRUE(step_instruction(test_d));
   TEST_ASSERT_EQUAL_DOUBLE(test_d->output->pending_line->rotation,359.9);
   free_debugger(test_d);

   test_d=init_debugger();
   test_d->program = init_word_cont();
   test_d->output= init_line_cont();
   strcpy(test_d->program->words[0],"LT");
   strcpy(test_d->program->words[1],"-.1");
   TEST_ASSERT_TRUE(step_instruction(test_d));
   TEST_ASSERT_EQUAL_DOUBLE(test_d->output->pending_line->rotation,0.1);
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
   /*testing finding the end position*/
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
   i=find_end_pos(test_d->program,1);
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
   i=find_end_pos(test_d->program,1);

   TEST_ASSERT_TRUE(i==9);
   test_d->program->capacity=9;
   strcpy(test_d->program->words[0],"FD");
   strcpy(test_d->program->words[1],"1.1.");
   strcpy(test_d->program->words[2],"2");
   strcpy(test_d->program->words[3],"{");
   strcpy(test_d->program->words[4],"{");
   strcpy(test_d->program->words[5],"}");
   strcpy(test_d->program->words[6],"FD");
   strcpy(test_d->program->words[7],"1");
   strcpy(test_d->program->words[8],"}");
   i=find_end_pos(test_d->program,1);
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
   i=find_end_pos(test_d->program,1);
   TEST_ASSERT_TRUE(i==9);

   strcpy(test_d->program->words[0],"}");
   i=find_end_pos(test_d->program,1);
   TEST_ASSERT_TRUE(i==1);
   test_d->program->capacity=MAXTESTCAP;
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
   test_t=step_do_helper(test_d->program);
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
   TEST_ASSERT_EQUAL_DOUBLE(((loop_tracker*)test_d->loop_stack->start->data)->end_val,10);
   TEST_ASSERT_TRUE(((loop_tracker*)test_d->loop_stack->start->data)->start_pos==7);
   TEST_ASSERT_TRUE(((loop_tracker*)test_d->loop_stack->start->data)->end_pos==8);
   TEST_ASSERT_TRUE(((loop_tracker*)test_d->loop_stack->start->data)->var_pos==0);
   TEST_ASSERT_EQUAL_DOUBLE(*test_d->program->var_array[0],1);


   test_d->program->position=8;
   strcpy(test_d->program->words[8],"DO");
   strcpy(test_d->program->words[9],"A");
   strcpy(test_d->program->words[10],"FROM");
   strcpy(test_d->program->words[11],"5");
   strcpy(test_d->program->words[12],"TO");
   strcpy(test_d->program->words[13],"7");
   strcpy(test_d->program->words[14],"{");
   strcpy(test_d->program->words[15],"}");

   TEST_ASSERT_TRUE(step_do(test_d));

   TEST_ASSERT_EQUAL_DOUBLE(((loop_tracker*)test_d->loop_stack->start->next->data)->start_val,1);
   TEST_ASSERT_EQUAL_DOUBLE(((loop_tracker*)test_d->loop_stack->start->next->data)->end_val,10);
   TEST_ASSERT_TRUE(((loop_tracker*)test_d->loop_stack->start->next->data)->start_pos==7);
   TEST_ASSERT_TRUE(((loop_tracker*)test_d->loop_stack->start->next->data)->end_pos==8);
   TEST_ASSERT_TRUE(((loop_tracker*)test_d->loop_stack->start->next->data)->var_pos==0);

   TEST_ASSERT_EQUAL_DOUBLE(((loop_tracker*)test_d->loop_stack->start->data)->start_val,5);
   TEST_ASSERT_EQUAL_DOUBLE(((loop_tracker*)test_d->loop_stack->start->data)->end_val,7);
   TEST_ASSERT_TRUE(((loop_tracker*)test_d->loop_stack->start->data)->start_pos==15);
   TEST_ASSERT_TRUE(((loop_tracker*)test_d->loop_stack->start->data)->end_pos==16);
   TEST_ASSERT_TRUE(((loop_tracker*)test_d->loop_stack->start->data)->var_pos==0);
   TEST_ASSERT_EQUAL_DOUBLE(*test_d->program->var_array[0],5);
   /*loop tracker created even if loop doesnt get executed*/
   test_d->program->position=16;
   strcpy(test_d->program->words[16],"DO");
   strcpy(test_d->program->words[17],"B");
   strcpy(test_d->program->words[18],"FROM");
   strcpy(test_d->program->words[19],"5");
   strcpy(test_d->program->words[20],"TO");
   strcpy(test_d->program->words[21],"4");
   strcpy(test_d->program->words[22],"{");
   strcpy(test_d->program->words[23],"}");
   TEST_ASSERT_TRUE(step_do(test_d));


   TEST_ASSERT_EQUAL_DOUBLE(((loop_tracker*)test_d->loop_stack->start->data)->start_val,5);
   TEST_ASSERT_EQUAL_DOUBLE(((loop_tracker*)test_d->loop_stack->start->data)->end_val,4);
   TEST_ASSERT_TRUE(((loop_tracker*)test_d->loop_stack->start->data)->start_pos==23);
   TEST_ASSERT_TRUE(((loop_tracker*)test_d->loop_stack->start->data)->end_pos==24);
   TEST_ASSERT_TRUE(((loop_tracker*)test_d->loop_stack->start->data)->var_pos==1);
   TEST_ASSERT_EQUAL_DOUBLE(*test_d->program->var_array[1],5);
   TEST_ASSERT_EQUAL_DOUBLE(*test_d->program->var_array[0],5);
   test_d->program->position=23;
   TEST_ASSERT_TRUE(test_d->loop_stack->size==3);
   TEST_ASSERT_TRUE(check_loop_fin(test_d));
   TEST_ASSERT_TRUE(test_d->loop_stack->size==2);
   TEST_ASSERT_TRUE(test_d->program->position==24);
   TEST_ASSERT_TRUE(((loop_tracker*)test_d->loop_stack->start->data)->end_pos==16);
   free_debugger(test_d);

   test_d=init_debugger();
   test_d->program = init_word_cont();
   test_d->output= init_line_cont();
   strcpy(test_d->program->words[0],"DO");
   strcpy(test_d->program->words[1],"A");
   strcpy(test_d->program->words[2],"jfdnjddj");
   strcpy(test_d->program->words[3],"4");
   strcpy(test_d->program->words[4],"TO");
   strcpy(test_d->program->words[5],"3");
   strcpy(test_d->program->words[6],"{");
   strcpy(test_d->program->words[7],"}");
   test_t=step_do_helper(test_d->program);
   TEST_ASSERT_TRUE(!test_t);
   free_debugger(test_d);

   test_d=init_debugger();
   test_d->program = init_word_cont();
   test_d->output= init_line_cont();
   strcpy(test_d->program->words[0],"DO");
   strcpy(test_d->program->words[1],"A");
   strcpy(test_d->program->words[2],"FROM");
   strcpy(test_d->program->words[3],"4");
   strcpy(test_d->program->words[4],"TO");
   strcpy(test_d->program->words[5],"3");
   strcpy(test_d->program->words[6],"{");
   strcpy(test_d->program->words[7],"}");
   TEST_ASSERT_TRUE(step_do(test_d));
   test_d->program->position=10;
   TEST_ASSERT_TRUE(test_d->loop_stack->size==1);
   /*position 10 is not at end of loop so loop fin is false*/
   TEST_ASSERT_TRUE(!check_loop_fin(test_d));
   TEST_ASSERT_TRUE(test_d->loop_stack->size==1);
   test_d->program->position=7;
   /*position 7 is at end of loop and loop is finished
   so loop fin is true and loop tracker gets popped off stack*/
   TEST_ASSERT_TRUE(check_loop_fin(test_d));
   TEST_ASSERT_TRUE(test_d->loop_stack->size==0);
   /*no loops so returns false*/
   TEST_ASSERT_TRUE(!check_loop_fin(test_d));
   TEST_ASSERT_TRUE(test_d->loop_stack->size==0);
   free_debugger(test_d);

   test_d=init_debugger();
   test_d->program = init_word_cont();
   test_d->output= init_line_cont();
   strcpy(test_d->program->words[0],"DO");
   strcpy(test_d->program->words[1],"A");
   strcpy(test_d->program->words[2],"FROM");
   strcpy(test_d->program->words[3],"4");
   strcpy(test_d->program->words[4],"TO");
   strcpy(test_d->program->words[5],"5");
   strcpy(test_d->program->words[6],"{");
   strcpy(test_d->program->words[7],"}");
   TEST_ASSERT_TRUE(step_do(test_d));
   TEST_ASSERT_TRUE(test_d->loop_stack->size==1);
   TEST_ASSERT_TRUE(!check_loop_fin(test_d));
   TEST_ASSERT_TRUE(test_d->loop_stack->size==1);
   TEST_ASSERT_TRUE(test_d->program->position==7);
   free_debugger(test_d);


   test_d=init_debugger();
   test_d->program = init_word_cont();
   test_d->output= init_line_cont();
   strcpy(test_d->program->words[0],"DO");
   strcpy(test_d->program->words[1],"A");
   strcpy(test_d->program->words[2],"FROM");
   strcpy(test_d->program->words[3],"4");
   strcpy(test_d->program->words[4],"TO");
   strcpy(test_d->program->words[5],"5");
   strcpy(test_d->program->words[6],"{");
   strcpy(test_d->program->words[7],"}");
   TEST_ASSERT_TRUE(step_do(test_d));
   test_d->program->position=8;
   check_loop_end(test_d);
   TEST_ASSERT_TRUE(test_d->program->position==7);
   TEST_ASSERT_EQUAL_DOUBLE(*test_d->program->var_array[0],5);
   check_loop_end(test_d);
   TEST_ASSERT_TRUE(test_d->program->position==7);
   /*once it reaches TO value the value does not get any larger*/
   TEST_ASSERT_EQUAL_DOUBLE(*test_d->program->var_array[0],5);
   free_debugger(test_d);

   test_d=init_debugger();
   test_d->program = init_word_cont();
   test_d->output= init_line_cont();
   strcpy(test_d->program->words[0],"DO");
   strcpy(test_d->program->words[1],"A");
   strcpy(test_d->program->words[2],"FROM");
   strcpy(test_d->program->words[3],"4");
   strcpy(test_d->program->words[4],"TO");
   strcpy(test_d->program->words[5],"5");
   strcpy(test_d->program->words[6],"{");
   strcpy(test_d->program->words[7],"DO");
   strcpy(test_d->program->words[8],"B");
   strcpy(test_d->program->words[9],"FROM");
   strcpy(test_d->program->words[10],"5");
   strcpy(test_d->program->words[11],"TO");
   strcpy(test_d->program->words[12],"7");
   strcpy(test_d->program->words[13],"{");
   strcpy(test_d->program->words[14],"}");
   strcpy(test_d->program->words[15],"}");
   TEST_ASSERT_TRUE(step_do(test_d));
   TEST_ASSERT_TRUE(step_do(test_d));
   test_d->program->position=15;
   check_loop_end(test_d);
   TEST_ASSERT_TRUE(test_d->program->position==14);
   TEST_ASSERT_EQUAL_DOUBLE(*test_d->program->var_array[1],6);
   test_d->program->position=15;
   check_loop_end(test_d);
   TEST_ASSERT_TRUE(test_d->program->position==14);
   TEST_ASSERT_EQUAL_DOUBLE(*test_d->program->var_array[1],7);
   test_d->program->position=15;
   check_loop_end(test_d);
   TEST_ASSERT_EQUAL_DOUBLE(*test_d->program->var_array[1],7);
   TEST_ASSERT_TRUE(((loop_tracker*)test_d->loop_stack->start->data)->fin_flag);
   TEST_ASSERT_TRUE(check_loop_fin(test_d));
   TEST_ASSERT_TRUE(test_d->program->position==15);
   TEST_ASSERT_TRUE(test_d->loop_stack->size==1);
   test_d->program->position=15;
   check_loop_end(test_d);
   /*inner loop has been popped off so position doesnt
   change*/
   TEST_ASSERT_TRUE(test_d->program->position==15);


   test_d->program->position=16;
   check_loop_end(test_d);
   /*outer loop still on stack so position goes back to beginning
   of outer loop*/
   TEST_ASSERT_TRUE(test_d->program->position==7);
   TEST_ASSERT_EQUAL_DOUBLE(*test_d->program->var_array[0],5);
   free_debugger(test_d);
}

void test_step_instruct_loops(void)
{
   debugger* test_d;

   test_d=init_debugger();
   test_d->program = init_word_cont();
   test_d->output= init_line_cont();
   test_d->program->capacity=11;
   strcpy(test_d->program->words[0],"DO");
   strcpy(test_d->program->words[1],"A");
   strcpy(test_d->program->words[2],"FROM");
   strcpy(test_d->program->words[3],"1");
   strcpy(test_d->program->words[4],"TO");
   strcpy(test_d->program->words[5],"2");
   strcpy(test_d->program->words[6],"{");
   strcpy(test_d->program->words[7],"FD");
   strcpy(test_d->program->words[8],"1");
   strcpy(test_d->program->words[9],"}");
   strcpy(test_d->program->words[10],"}");
   TEST_ASSERT_TRUE(step_instruction(test_d));
   TEST_ASSERT_TRUE(test_d->loop_stack->size==1);
   TEST_ASSERT_EQUAL_DOUBLE(((loop_tracker*)test_d->loop_stack->start->data)->start_val,1);
   TEST_ASSERT_EQUAL_DOUBLE(((loop_tracker*)test_d->loop_stack->start->data)->end_val,2);
   TEST_ASSERT_EQUAL_DOUBLE(((loop_tracker*)test_d->loop_stack->start->data)->start_pos,7);
   TEST_ASSERT_EQUAL_DOUBLE(((loop_tracker*)test_d->loop_stack->start->data)->end_pos,10);
   TEST_ASSERT_TRUE(test_d->program->position==7);

   TEST_ASSERT_TRUE(step_instruction(test_d));
   TEST_ASSERT_TRUE(test_d->program->position==9);
   TEST_ASSERT_TRUE(step_instruction(test_d));
   TEST_ASSERT_TRUE(test_d->program->position==10);
   TEST_ASSERT_TRUE(step_instruction(test_d));
   TEST_ASSERT_TRUE(test_d->program->position==9);
   TEST_ASSERT_TRUE(step_instruction(test_d));
   TEST_ASSERT_TRUE(test_d->program->position==10);
   TEST_ASSERT_TRUE(!step_instruction(test_d));
   TEST_ASSERT_TRUE(test_d->program->position==10);
   TEST_ASSERT_TRUE(test_d->output->size==2);
   test_d->program->capacity=MAXTESTCAP;
   free_debugger(test_d);


   test_d=init_debugger();
   test_d->program = init_word_cont();
   test_d->output= init_line_cont();
   test_d->program->capacity=20;
   strcpy(test_d->program->words[0],"DO");
   strcpy(test_d->program->words[1],"A");
   strcpy(test_d->program->words[2],"FROM");
   strcpy(test_d->program->words[3],"1");
   strcpy(test_d->program->words[4],"TO");
   strcpy(test_d->program->words[5],"2");
   strcpy(test_d->program->words[6],"{");

   strcpy(test_d->program->words[7],"DO");
   strcpy(test_d->program->words[8],"B");
   strcpy(test_d->program->words[9],"FROM");
   strcpy(test_d->program->words[10],"2");
   strcpy(test_d->program->words[11],"TO");
   strcpy(test_d->program->words[12],"2");
   strcpy(test_d->program->words[13],"{");
   strcpy(test_d->program->words[14],"FD");
   strcpy(test_d->program->words[15],"B");
   strcpy(test_d->program->words[16],"}");

   strcpy(test_d->program->words[17],"}");
   strcpy(test_d->program->words[18],"}");

   TEST_ASSERT_TRUE(step_instruction(test_d));

   TEST_ASSERT_EQUAL_DOUBLE(*test_d->program->var_array[0],1);
   TEST_ASSERT_TRUE(test_d->program->position==7);
   TEST_ASSERT_TRUE(step_instruction(test_d));

   TEST_ASSERT_TRUE(test_d->program->position==14);
   TEST_ASSERT_TRUE(step_instruction(test_d));

   TEST_ASSERT_TRUE(test_d->program->position==16);
   TEST_ASSERT_TRUE(step_instruction(test_d));
   TEST_ASSERT_TRUE(step_instruction(test_d));
   TEST_ASSERT_TRUE(step_instruction(test_d));
   TEST_ASSERT_EQUAL_DOUBLE(*test_d->program->var_array[0],2);
   TEST_ASSERT_TRUE(test_d->program->position==14);
   TEST_ASSERT_TRUE(step_instruction(test_d));
   TEST_ASSERT_TRUE(step_instruction(test_d));
   TEST_ASSERT_TRUE(step_instruction(test_d));
   TEST_ASSERT_TRUE(step_instruction(test_d));

   TEST_ASSERT_TRUE(!step_instruction(test_d));
   TEST_ASSERT_TRUE(test_d->output->size==2);
   TEST_ASSERT_TRUE(test_d->program->position==19);
   test_d->program->capacity=MAXTESTCAP;
   free_debugger(test_d);

   test_d=init_debugger();
   test_d->program = init_word_cont();
   test_d->output= init_line_cont();
   test_d->program->capacity=12;
   strcpy(test_d->program->words[0],"DO");
   strcpy(test_d->program->words[1],"A");
   strcpy(test_d->program->words[2],"FROM");
   strcpy(test_d->program->words[3],"3");
   strcpy(test_d->program->words[4],"TO");
   strcpy(test_d->program->words[5],"2");
   strcpy(test_d->program->words[6],"{");
   strcpy(test_d->program->words[7],"FD");
   strcpy(test_d->program->words[8],"NO");
   strcpy(test_d->program->words[9],"}");
   strcpy(test_d->program->words[10],"}");
   TEST_ASSERT_TRUE(!step_instruction(test_d));
   test_d->program->capacity=MAXTESTCAP;
   free_debugger(test_d);


   test_d=init_debugger();
   test_d->program = init_word_cont();
   test_d->output= init_line_cont();
   test_d->program->capacity=13;
   strcpy(test_d->program->words[0],"DO");
   strcpy(test_d->program->words[1],"A");
   strcpy(test_d->program->words[2],"FROM");
   strcpy(test_d->program->words[3],"1");
   strcpy(test_d->program->words[4],"TO");
   strcpy(test_d->program->words[5],"2");
   strcpy(test_d->program->words[6],"{");
   strcpy(test_d->program->words[7],"FD");
   strcpy(test_d->program->words[8],"1");
   strcpy(test_d->program->words[9],"}");
   strcpy(test_d->program->words[10],"FD");
   strcpy(test_d->program->words[11],"1");
   TEST_ASSERT_TRUE(step_instruction(test_d));
   TEST_ASSERT_TRUE(step_instruction(test_d));
   TEST_ASSERT_TRUE(step_instruction(test_d));
   TEST_ASSERT_TRUE(step_instruction(test_d));
   TEST_ASSERT_TRUE(step_instruction(test_d));
   TEST_ASSERT_TRUE(step_instruction(test_d));
   TEST_ASSERT_EQUAL_INT(test_d->output->size,3);
   test_d->program->capacity=MAXTESTCAP;
   free_debugger(test_d);


   test_d=init_debugger();
   test_d->program = init_word_cont();
   test_d->output= init_line_cont();
   test_d->program->capacity=13;
   strcpy(test_d->program->words[0],"DO");
   strcpy(test_d->program->words[1],"A");
   strcpy(test_d->program->words[2],"FROM");
   strcpy(test_d->program->words[3],"1");
   strcpy(test_d->program->words[4],"TO");
   strcpy(test_d->program->words[5],"2");
   strcpy(test_d->program->words[6],"{");
   strcpy(test_d->program->words[7],"FD");
   strcpy(test_d->program->words[8],"jndnf");
   strcpy(test_d->program->words[9],"}");
   TEST_ASSERT_TRUE(step_instruction(test_d));
   TEST_ASSERT_TRUE(!step_instruction(test_d));
   test_d->program->capacity=MAXTESTCAP;
   free_debugger(test_d);
}

void test_step_instruct_set(void)
{
   debugger* test_d;
   test_d=init_debugger();
   test_d->program = init_word_cont();
   test_d->output= init_line_cont();
   test_d->program->capacity=8;
   strcpy(test_d->program->words[0],"SET");
   strcpy(test_d->program->words[1],"A");
   strcpy(test_d->program->words[2],":=");
   strcpy(test_d->program->words[3],"1");
   strcpy(test_d->program->words[4],"1");
   strcpy(test_d->program->words[5],"+");
   strcpy(test_d->program->words[6],";");
   TEST_ASSERT_TRUE(step_instruction(test_d));
   TEST_ASSERT_TRUE(test_d->program->position==7);
   TEST_ASSERT_EQUAL_DOUBLE(*test_d->program->var_array[0],2);
   test_d->program->capacity=MAXTESTCAP;
   free_debugger(test_d);


   test_d=init_debugger();
   test_d->program = init_word_cont();
   test_d->output= init_line_cont();
   test_d->program->capacity=16;
   strcpy(test_d->program->words[0],"DO");
   strcpy(test_d->program->words[1],"A");
   strcpy(test_d->program->words[2],"FROM");
   strcpy(test_d->program->words[3],"1");
   strcpy(test_d->program->words[4],"TO");
   strcpy(test_d->program->words[5],"4");
   strcpy(test_d->program->words[6],"{");

   strcpy(test_d->program->words[7],"SET");
   strcpy(test_d->program->words[8],"A");
   strcpy(test_d->program->words[9],":=");
   strcpy(test_d->program->words[10],"8");
   strcpy(test_d->program->words[11],";");
   strcpy(test_d->program->words[12],"FD");
   strcpy(test_d->program->words[13],"A");
   strcpy(test_d->program->words[14],"}");

   strcpy(test_d->program->words[15],"}");
   /*first step through do*/
   TEST_ASSERT_TRUE(step_instruction(test_d));
   TEST_ASSERT_TRUE(test_d->loop_stack->size==1);
   /*then step through set*/
   TEST_ASSERT_TRUE(step_instruction(test_d));
   /*then step through forward*/
   TEST_ASSERT_TRUE(step_instruction(test_d));
   /*then step through instructlist bracket*/
   TEST_ASSERT_TRUE(step_instruction(test_d));
   /*then loop finishes because A has been changed in
   the loop and program finishes*/
   TEST_ASSERT_TRUE(!step_instruction(test_d));
   TEST_ASSERT_TRUE(test_d->output->size==1);
   TEST_ASSERT_TRUE(test_d->loop_stack->size==0);
   test_d->program->capacity=MAXTESTCAP;
   free_debugger(test_d);
}

void test_advance(void)
{
   debugger* test_d;

   test_d=init_debugger();
   test_d->program = init_word_cont();
   test_d->output= init_line_cont();
   test_d->program->capacity=16;
   strcpy(test_d->program->words[0],"DO");
   strcpy(test_d->program->words[1],"A");
   strcpy(test_d->program->words[2],"FROM");
   strcpy(test_d->program->words[3],"1");
   strcpy(test_d->program->words[4],"TO");
   strcpy(test_d->program->words[5],"4");
   strcpy(test_d->program->words[6],"{");

   strcpy(test_d->program->words[7],"SET");
   strcpy(test_d->program->words[8],"A");
   strcpy(test_d->program->words[9],":=");
   strcpy(test_d->program->words[10],"8");
   strcpy(test_d->program->words[11],";");
   strcpy(test_d->program->words[12],"FD");
   strcpy(test_d->program->words[13],"A");
   strcpy(test_d->program->words[14],"}");

   strcpy(test_d->program->words[15],"}");

   TEST_ASSERT_TRUE(advance_to_mistake(test_d));
   test_d->program->capacity=MAXTESTCAP;
   free_debugger(test_d);

   test_d=init_debugger();
   test_d->program = init_word_cont();
   test_d->output= init_line_cont();
   test_d->program->capacity=11;
   strcpy(test_d->program->words[0],"DO");
   strcpy(test_d->program->words[1],"A");
   strcpy(test_d->program->words[2],"FROM");
   strcpy(test_d->program->words[3],"3");
   strcpy(test_d->program->words[4],"TO");
   strcpy(test_d->program->words[5],"2");
   strcpy(test_d->program->words[6],"{");
   strcpy(test_d->program->words[7],"FD");
   strcpy(test_d->program->words[8],"NO");
   strcpy(test_d->program->words[9],"}");
   strcpy(test_d->program->words[10],"}");
   TEST_ASSERT_TRUE(!advance_to_mistake(test_d));
   TEST_ASSERT_EQUAL_INT(test_d->program->position,8);
   test_d->program->capacity=MAXTESTCAP;
   free_debugger(test_d);





   test_d=init_debugger();
   test_d->program = init_word_cont();
   test_d->output= init_line_cont();
   test_d->program->capacity=3;
   strcpy(test_d->program->words[0],"FD");
   strcpy(test_d->program->words[1],"A");
   strcpy(test_d->program->words[2],"}");

   TEST_ASSERT_TRUE(!advance_to_mistake(test_d));
   TEST_ASSERT_EQUAL_INT(test_d->program->position,0);
   test_d->program->capacity=MAXTESTCAP;
   free_debugger(test_d);

   test_d=init_debugger();
   test_d->program = init_word_cont();
   test_d->output= init_line_cont();
   test_d->program->capacity=10;
   strcpy(test_d->program->words[0],"SET");
   strcpy(test_d->program->words[1],"A");
   strcpy(test_d->program->words[2],":=");
   strcpy(test_d->program->words[3],"10");
   strcpy(test_d->program->words[4],";");
   strcpy(test_d->program->words[5],"FD");
   strcpy(test_d->program->words[6],"10");
   strcpy(test_d->program->words[7],"LT");
   strcpy(test_d->program->words[8],"B");
   strcpy(test_d->program->words[9],"}");
   TEST_ASSERT_TRUE(!advance_to_mistake(test_d));
   TEST_ASSERT_EQUAL_INT(test_d->program->position,7);
   test_d->program->capacity=MAXTESTCAP;
   free_debugger(test_d);

   test_d=init_debugger();
   test_d->program = init_word_cont();
   test_d->output= init_line_cont();
   test_d->program->capacity=10;
   strcpy(test_d->program->words[0],"SET");
   strcpy(test_d->program->words[1],"A");
   strcpy(test_d->program->words[2],":=");
   strcpy(test_d->program->words[3],"10");
   strcpy(test_d->program->words[4],";");
   strcpy(test_d->program->words[5],"FD");
   strcpy(test_d->program->words[6],"10");
   strcpy(test_d->program->words[7],"LT");
   strcpy(test_d->program->words[8],"A");
   strcpy(test_d->program->words[9],"}");
   TEST_ASSERT_TRUE(advance_to_mistake(test_d));
   TEST_ASSERT_EQUAL_INT(test_d->program->position,9);
   test_d->program->capacity=MAXTESTCAP;
   free_debugger(test_d);


   test_d=init_debugger();
   test_d->program = init_word_cont();
   test_d->output= init_line_cont();
   test_d->program->capacity=10;
   strcpy(test_d->program->words[0],"DO");
   strcpy(test_d->program->words[1],"A");
   strcpy(test_d->program->words[2],"FROM");
   strcpy(test_d->program->words[3],"1000");
   strcpy(test_d->program->words[4],"TO");
   strcpy(test_d->program->words[5],"1001");
   strcpy(test_d->program->words[6],"{");
   strcpy(test_d->program->words[7],"FD");
   strcpy(test_d->program->words[8],"2");
   strcpy(test_d->program->words[9],"}");
   TEST_ASSERT_TRUE(!advance_to_mistake(test_d));

   TEST_ASSERT_EQUAL_INT(test_d->program->position,9);
   TEST_ASSERT_TRUE(strcmp(test_d->info,"missing bracket in code")==0);
   test_d->program->capacity=MAXTESTCAP;
   free_debugger(test_d);


}


void test_code_after_main(void)
{
   debugger* test_d;
   int i;

   test_d=init_debugger();
   test_d->program = init_word_cont();
   test_d->output= init_line_cont();
   strcpy(test_d->program->words[0],"{");
   strcpy(test_d->program->words[1],"FD");
   strcpy(test_d->program->words[2],"1.1.");
   strcpy(test_d->program->words[3],"FD");
   strcpy(test_d->program->words[4],"1.1.");
   strcpy(test_d->program->words[5],"FD");
   strcpy(test_d->program->words[6],"1.1.");
   strcpy(test_d->program->words[7],"FD");
   strcpy(test_d->program->words[8],"1.1.");
   strcpy(test_d->program->words[9],"}");
   i=find_end_pos(test_d->program,0);
   TEST_ASSERT_EQUAL_INT(i,10);
   free_debugger(test_d);

   test_d=init_debugger();
   test_d->program = init_word_cont();
   test_d->output= init_line_cont();
   strcpy(test_d->program->words[0],"{");
   strcpy(test_d->program->words[1],"FD");
   strcpy(test_d->program->words[2],"1.1.");
   strcpy(test_d->program->words[3],"FD");
   strcpy(test_d->program->words[4],"1.1.");
   strcpy(test_d->program->words[5],"{");
   strcpy(test_d->program->words[6],"}");
   strcpy(test_d->program->words[7],"FD");
   strcpy(test_d->program->words[8],"1.1.");
   strcpy(test_d->program->words[9],"}");
   i=find_end_pos(test_d->program,0);
   TEST_ASSERT_EQUAL_INT(i,10);
   free_debugger(test_d);


   test_d=init_debugger();
   test_d->program = init_word_cont();
   test_d->output= init_line_cont();
   strcpy(test_d->program->words[0],"{");
   strcpy(test_d->program->words[1],"FD");
   strcpy(test_d->program->words[2],"1.1.");
   strcpy(test_d->program->words[3],"FD");
   strcpy(test_d->program->words[4],"}");
   strcpy(test_d->program->words[5],"{");
   strcpy(test_d->program->words[6],"}");
   strcpy(test_d->program->words[7],"FD");
   strcpy(test_d->program->words[8],"1.1.");
   strcpy(test_d->program->words[9],"}");
   i=find_end_pos(test_d->program,0);
   TEST_ASSERT_EQUAL_INT(i,5);
   free_debugger(test_d);

   test_d=init_debugger();
   test_d->program = init_word_cont();
   test_d->output= init_line_cont();
   strcpy(test_d->program->words[0],"{");
   strcpy(test_d->program->words[1],"FD");
   strcpy(test_d->program->words[2],"1.1.");
   strcpy(test_d->program->words[3],"FD");
   strcpy(test_d->program->words[4],"}");
   strcpy(test_d->program->words[5],"{");
   strcpy(test_d->program->words[6],"}");
   strcpy(test_d->program->words[7],"FD");
   strcpy(test_d->program->words[8],"1.1.");
   strcpy(test_d->program->words[9],"}");
   i=find_end_pos(test_d->program,0);
   TEST_ASSERT_EQUAL_INT(i,5);
   free_debugger(test_d);

   test_d=init_debugger();
   test_d->program = init_word_cont();
   test_d->output= init_line_cont();
   strcpy(test_d->program->words[0],"{");
   strcpy(test_d->program->words[1],"{");
   strcpy(test_d->program->words[2],"1.1.");
   strcpy(test_d->program->words[3],"FD");
   strcpy(test_d->program->words[4],"}");
   strcpy(test_d->program->words[5],"{");
   strcpy(test_d->program->words[6],"}");
   strcpy(test_d->program->words[7],"FD");
   strcpy(test_d->program->words[8],"1.1.");
   strcpy(test_d->program->words[9],"}");
   i=find_end_pos(test_d->program,0);
   TEST_ASSERT_EQUAL_INT(i,10);
   free_debugger(test_d);

   test_d=init_debugger();
   test_d->program = init_word_cont();
   test_d->output= init_line_cont();

   strcpy(test_d->program->words[0],"{");
   strcpy(test_d->program->words[1],"{");
   strcpy(test_d->program->words[2],"1.1.");
   strcpy(test_d->program->words[3],"FD");
   strcpy(test_d->program->words[4],"}");
   strcpy(test_d->program->words[5],"{");
   strcpy(test_d->program->words[6],"}");
   strcpy(test_d->program->words[7],"FD");
   strcpy(test_d->program->words[8],"1.1.");
   strcpy(test_d->program->words[9],"");
   i=find_end_pos(test_d->program,0);
   TEST_ASSERT_EQUAL_INT(i,-1);
   free_debugger(test_d);


   test_d=init_debugger();
   test_d->program = init_word_cont();
   test_d->output= init_line_cont();
   test_d->program->capacity=9;
   strcpy(test_d->program->words[0],"{");
   strcpy(test_d->program->words[1],"{");
   strcpy(test_d->program->words[2],"1.1.");
   strcpy(test_d->program->words[3],"FD");
   strcpy(test_d->program->words[4],"}");
   strcpy(test_d->program->words[5],"}");
   strcpy(test_d->program->words[6],"}");
   strcpy(test_d->program->words[7],"FD");
   strcpy(test_d->program->words[8],"1.1.");
   i=find_end_pos(test_d->program,0);
   TEST_ASSERT_EQUAL_INT(i,6);
   TEST_ASSERT_TRUE(!check_past_main(test_d));
   test_d->program->capacity=MAXTESTCAP;
   free_debugger(test_d);

   test_d=init_debugger();
   test_d->program = init_word_cont();
   test_d->output= init_line_cont();
   test_d->program->capacity=10;
   strcpy(test_d->program->words[0],"{");
   strcpy(test_d->program->words[1],"{");
   strcpy(test_d->program->words[2],"1.1.");
   strcpy(test_d->program->words[3],"FD");
   strcpy(test_d->program->words[4],"}");
   strcpy(test_d->program->words[5],"FD");
   strcpy(test_d->program->words[6],"56");
   strcpy(test_d->program->words[7],"FD");
   strcpy(test_d->program->words[8],"}");
   strcpy(test_d->program->words[9],"FD");
   i=find_end_pos(test_d->program,0);
   TEST_ASSERT_EQUAL_INT(i,9);
   TEST_ASSERT_TRUE(!check_past_main(test_d));
   test_d->program->capacity=MAXTESTCAP;
   free_debugger(test_d);

   test_d=init_debugger();
   test_d->program = init_word_cont();
   test_d->output= init_line_cont();
   test_d->program->capacity=10;
   strcpy(test_d->program->words[0],"{");
   strcpy(test_d->program->words[1],"{");
   strcpy(test_d->program->words[2],"1.1.");
   strcpy(test_d->program->words[3],"FD");
   strcpy(test_d->program->words[4],"}");
   strcpy(test_d->program->words[5],"FD");
   strcpy(test_d->program->words[6],"56");
   strcpy(test_d->program->words[7],"FD");
   strcpy(test_d->program->words[8],"90");
   strcpy(test_d->program->words[9],"}");
   i=find_end_pos(test_d->program,0);
   TEST_ASSERT_EQUAL_INT(i,10);
   TEST_ASSERT_TRUE(check_past_main(test_d));
   test_d->program->capacity=MAXTESTCAP;
   free_debugger(test_d);

}

void test_show_args(void)
{
   debugger* test_d;
   char test_str[ONEARGLEN];
   char test_str_2[FULLARGSTRLEN];
   int i;
   str_num(999999.999,test_str);

   TEST_ASSERT_TRUE(strcmp(test_str,"= 999... ")==0);
   str_num(99.999,test_str);
   TEST_ASSERT_TRUE(strcmp(test_str,"= 99.99 ")==0);
   str_num(199.099,test_str);
   TEST_ASSERT_TRUE(strcmp(test_str,"= 199.09 ")==0);

   str_num(1.099,test_str);
   TEST_ASSERT_TRUE(strcmp(test_str,"= 1.09 ")==0);
   /*doesnt overflow even with massive numbers*/
   str_num(-1487475757575757575656565656565667755555555555555555555555555555555555555555555555555555.099,test_str);
   TEST_ASSERT_TRUE(strcmp(test_str,"= -148... ")==0);

   test_d=init_debugger();
   test_d->program = init_word_cont();
   test_d->output= init_line_cont();
   test_d->program->capacity=11;
   strcpy(test_d->program->words[0],"DO");
   strcpy(test_d->program->words[1],"A");
   strcpy(test_d->program->words[2],"FROM");
   strcpy(test_d->program->words[3],"1000");
   strcpy(test_d->program->words[4],"TO");
   strcpy(test_d->program->words[5],"1001");
   strcpy(test_d->program->words[6],"{");
   strcpy(test_d->program->words[7],"FD");
   strcpy(test_d->program->words[8],"2");
   strcpy(test_d->program->words[9],"}");
   strcpy(test_d->program->words[10],"}");
   TEST_ASSERT_TRUE(advance_to_mistake(test_d));
   show_current_vars(test_d,test_str_2);
   TEST_ASSERT_TRUE(strcmp(test_str_2,"A= 100... B= NA C= NA D= NA E= NA \nF= NA G= NA H= NA I= NA J= NA \nK= NA L= NA M= NA N= NA O= NA \nP= NA Q= NA R= NA S= NA T= NA \nU= NA V= NA W= NA X= NA Y= NA \nZ= NA ")==0);
   test_d->program->capacity=MAXTESTCAP;
   free_debugger(test_d);


   test_d=init_debugger();
   test_d->program = init_word_cont();
   test_d->output= init_line_cont();
   test_d->program->capacity=11;
   strcpy(test_d->program->words[0],"SET");
   strcpy(test_d->program->words[1],"A");
   strcpy(test_d->program->words[2],":=");
   strcpy(test_d->program->words[3],"1");
   strcpy(test_d->program->words[4],";");
   strcpy(test_d->program->words[5],"SET");
   strcpy(test_d->program->words[6],"D");
   strcpy(test_d->program->words[7],":=");
   strcpy(test_d->program->words[8],"50.4");
   strcpy(test_d->program->words[9],";");
   strcpy(test_d->program->words[10],"}");
   TEST_ASSERT_TRUE(advance_to_mistake(test_d));
   show_current_vars(test_d,test_str_2);
   TEST_ASSERT_TRUE(strcmp(test_str_2,"A= 1.00 B= NA C= NA D= 50.40 E= NA \nF= NA G= NA H= NA I= NA J= NA \nK= NA L= NA M= NA N= NA O= NA \nP= NA Q= NA R= NA S= NA T= NA \nU= NA V= NA W= NA X= NA Y= NA \nZ= NA ")==0);
   test_d->program->capacity=MAXTESTCAP;
   free_debugger(test_d);

   test_d=init_debugger();
   test_d->program = init_word_cont();
   test_d->output= init_line_cont();
   test_d->program->capacity=11;
   strcpy(test_d->program->words[0],"SET");
   strcpy(test_d->program->words[1],"A");
   strcpy(test_d->program->words[2],":=");
   strcpy(test_d->program->words[3],"999.99");
   strcpy(test_d->program->words[4],";");
   strcpy(test_d->program->words[5],"SET");
   strcpy(test_d->program->words[6],"D");
   strcpy(test_d->program->words[7],":=");
   strcpy(test_d->program->words[8],"999.99");
   strcpy(test_d->program->words[9],";");
   strcpy(test_d->program->words[10],"}");
   TEST_ASSERT_TRUE(advance_to_mistake(test_d));
   show_current_vars(test_d,test_str_2);
   TEST_ASSERT_TRUE(strcmp(test_str_2,"A= 999.99 B= NA C= NA D= 999.99 E= NA \nF= NA G= NA H= NA I= NA J= NA \nK= NA L= NA M= NA N= NA O= NA \nP= NA Q= NA R= NA S= NA T= NA \nU= NA V= NA W= NA X= NA Y= NA \nZ= NA ")==0);
   test_d->program->capacity=MAXTESTCAP;
   free_debugger(test_d);
   test_d=init_debugger();
   test_d->program = init_word_cont();
   test_d->output= init_line_cont();
   test_d->program->capacity=131;

   for(i=0;i<130;i+=5)
   {

      strcpy(test_d->program->words[i],"SET");
      test_d->program->words[i+1][0]='A'+i/5;
      test_d->program->words[i+1][1]='\0';
      strcpy(test_d->program->words[i+2],":=");
      strcpy(test_d->program->words[i+3],"999.99");
      strcpy(test_d->program->words[i+4],";");
   }
   strcpy(test_d->program->words[130],"}");
   TEST_ASSERT_TRUE(advance_to_mistake(test_d));
   show_current_vars(test_d,test_str_2);
   TEST_ASSERT_TRUE(strcmp(test_str_2,"A= 999.99 B= 999.99 C= 999.99 D= 999.99 E= 999.99 \nF= 999.99 G= 999.99 H= 999.99 I= 999.99 J= 999.99 \nK= 999.99 L= 999.99 M= 999.99 N= 999.99 O= 999.99 \nP= 999.99 Q= 999.99 R= 999.99 S= 999.99 T= 999.99 \nU= 999.99 V= 999.99 W= 999.99 X= 999.99 Y= 999.99 \nZ= 999.99 ")==0);
   /*999.99 is the longest number possible*/
   test_d->program->capacity=MAXTESTCAP;
   free_debugger(test_d);
}

void test_show_code_pos(void)
{
   debugger* test_d;
   char test_str_2[FULLARGSTRLEN];
   test_d=init_debugger();
   test_d->program = init_word_cont();
   test_d->output= init_line_cont();
   test_d->program->capacity=5;
   strcpy(test_d->program->words[0],"SET");
   strcpy(test_d->program->words[1],"A");
   strcpy(test_d->program->words[2],":=");
   strcpy(test_d->program->words[3],"999.99");
   strcpy(test_d->program->words[4],";");
   show_code_pos(test_d,test_str_2);

   TEST_ASSERT_TRUE(strcmp(test_str_2,"SET<- current pos\nA\n:=\n999.99\n;\n")==0);
   TEST_ASSERT_TRUE(step_instruction(test_d));
   show_code_pos(test_d,test_str_2);
   TEST_ASSERT_TRUE(strcmp(test_str_2,"SET\nA\n:=\n999.99\n;\n")==0);
   test_d->program->capacity=MAXTESTCAP;
   free_debugger(test_d);

   test_d=init_debugger();
   test_d->program = init_word_cont();
   test_d->output= init_line_cont();
   test_d->program->capacity=11;
   strcpy(test_d->program->words[0],"SET");
   strcpy(test_d->program->words[1],"A");
   strcpy(test_d->program->words[2],":=");
   strcpy(test_d->program->words[3],"999.99");
   strcpy(test_d->program->words[4],";");
   strcpy(test_d->program->words[5],"SET");
   strcpy(test_d->program->words[6],"D");
   strcpy(test_d->program->words[7],":=");
   strcpy(test_d->program->words[8],"999.99");
   strcpy(test_d->program->words[9],";");
   strcpy(test_d->program->words[10],"}");
   TEST_ASSERT_TRUE(step_instruction(test_d));
   show_code_pos(test_d,test_str_2);
   TEST_ASSERT_TRUE(strcmp(test_str_2,"SET\nA\n:=\n999.99\n;\nSET<- current pos\nD\n:=\n999.99\n;\n}\n")==0);
   test_d->program->capacity=MAXTESTCAP;
   free_debugger(test_d);
}

void test_show_recent_coords(void)
{
   debugger* test_d;
   char test_str_2[FULLARGSTRLEN];
   test_d=init_debugger();
   test_d->program = init_word_cont();
   test_d->output= init_line_cont();
   test_d->program->capacity=11;
   strcpy(test_d->program->words[0],"DO");
   strcpy(test_d->program->words[1],"A");
   strcpy(test_d->program->words[2],"FROM");
   strcpy(test_d->program->words[3],"900.9");
   strcpy(test_d->program->words[4],"TO");
   strcpy(test_d->program->words[5],"905.9");
   strcpy(test_d->program->words[6],"{");
   strcpy(test_d->program->words[7],"FD");
   strcpy(test_d->program->words[8],"A");
   strcpy(test_d->program->words[9],"}");
   strcpy(test_d->program->words[10],"}");
   TEST_ASSERT_TRUE(advance_to_mistake(test_d));
   show_recent_coords(test_d,test_str_2);
   TEST_ASSERT_TRUE(strcmp(test_str_2,"  0 y = 900.90 | x = 0.00 \n  1 y = 180... | x = 0.00 \n  2 y = 270... | x = 0.00 \n  3 y = 360... | x = 0.00 \n  4 y = 451... | x = 0.00 \n  5 y = 542... | x = 0.00 \n")==0);
   test_d->program->capacity=MAXTESTCAP;
   free_debugger(test_d);


   test_d=init_debugger();
   test_d->program = init_word_cont();
   test_d->output= init_line_cont();
   test_d->program->capacity=11;
   strcpy(test_d->program->words[0],"DO");
   strcpy(test_d->program->words[1],"A");
   strcpy(test_d->program->words[2],"FROM");
   strcpy(test_d->program->words[3],"900.9");
   strcpy(test_d->program->words[4],"TO");
   strcpy(test_d->program->words[5],"910.9");
   strcpy(test_d->program->words[6],"{");
   strcpy(test_d->program->words[7],"FD");
   strcpy(test_d->program->words[8],"A");
   strcpy(test_d->program->words[9],"}");
   strcpy(test_d->program->words[10],"}");
   TEST_ASSERT_TRUE(advance_to_mistake(test_d));
   show_recent_coords(test_d,test_str_2);

   TEST_ASSERT_TRUE(strcmp(test_str_2,"  5 y = 542... | x = 0.00 \n  6 y = 632... | x = 0.00 \n  7 y = 723... | x = 0.00 \n  8 y = 814... | x = 0.00 \n  9 y = 905... | x = 0.00 \n 10 y = 996... | x = 0.00 \n")==0);
   test_d->program->capacity=MAXTESTCAP;
   free_debugger(test_d);


   test_d=init_debugger();
   test_d->program = init_word_cont();
   test_d->output= init_line_cont();
   test_d->program->capacity=13;
   strcpy(test_d->program->words[0],"DO");
   strcpy(test_d->program->words[1],"A");
   strcpy(test_d->program->words[2],"FROM");
   strcpy(test_d->program->words[3],"1");
   strcpy(test_d->program->words[4],"TO");
   strcpy(test_d->program->words[5],"4");
   strcpy(test_d->program->words[6],"{");
   strcpy(test_d->program->words[7],"FD");
   strcpy(test_d->program->words[8],"A");
   strcpy(test_d->program->words[9],"LT");
   strcpy(test_d->program->words[10],"A");
   strcpy(test_d->program->words[11],"}");
   strcpy(test_d->program->words[12],"}");
   TEST_ASSERT_TRUE(advance_to_mistake(test_d));
   show_recent_coords(test_d,test_str_2);
   TEST_ASSERT_TRUE(strcmp(test_str_2,"  0 y = 1.00 | x = 0.00 \n  1 y = 2.99 | x = 0.03 \n  2 y = 5.99 | x = 0.19 \n  3 y = 9.97 | x = 0.61 \n")==0);
   test_d->program->capacity=MAXTESTCAP;
   free_debugger(test_d);

   test_d=init_debugger();
   test_d->program = init_word_cont();
   test_d->output= init_line_cont();
   show_recent_coords(test_d,test_str_2);
   TEST_ASSERT_TRUE(strcmp(test_str_2,"\n"));
   free_debugger(test_d);
}


void test_error_collate(void)
{
   debugger* test_d;
   char test_str_2[FULLARGSTRLEN];
   test_d=init_debugger();
   test_d->program = init_word_cont();
   test_d->output= init_line_cont();
   test_d->program->capacity=10;
   strcpy(test_d->program->words[0],"SET");
   strcpy(test_d->program->words[1],"A");
   strcpy(test_d->program->words[2],":=");
   strcpy(test_d->program->words[3],"999.99");
   strcpy(test_d->program->words[4],";");
   strcpy(test_d->program->words[5],"SET");
   strcpy(test_d->program->words[6],"D");
   strcpy(test_d->program->words[7],":=");
   strcpy(test_d->program->words[8],"999.99");
   strcpy(test_d->program->words[9],";");

   TEST_ASSERT_TRUE(!advance_to_mistake(test_d));
   collate_instruct_messages(test_d,test_str_2);
   TEST_ASSERT_TRUE(strcmp(test_str_2,"error around word 10 \nmissing bracket in code\n")==0);
   test_d->program->capacity=MAXTESTCAP;
   free_debugger(test_d);


   test_d=init_debugger();
   test_d->program = init_word_cont();
   test_d->output= init_line_cont();
   test_d->program->capacity=9;
   strcpy(test_d->program->words[0],"SET");
   strcpy(test_d->program->words[1],"A");
   strcpy(test_d->program->words[2],":=");
   strcpy(test_d->program->words[3],"999.99");
   strcpy(test_d->program->words[4],"192");
   strcpy(test_d->program->words[5],"453");
   strcpy(test_d->program->words[6],"/");
   strcpy(test_d->program->words[7],";");
   strcpy(test_d->program->words[8],"}");
   TEST_ASSERT_TRUE(!advance_to_mistake(test_d));
   collate_instruct_messages(test_d,test_str_2);
   TEST_ASSERT_TRUE(strcmp(test_str_2,"error around word 0 SET\nmore than one number left on stack at end of expr\n")==0);
   test_d->program->capacity=MAXTESTCAP;
   free_debugger(test_d);

   test_d=init_debugger();
   test_d->program = init_word_cont();
   test_d->output= init_line_cont();
   test_d->program->capacity=10;
   strcpy(test_d->program->words[0],"DO");
   strcpy(test_d->program->words[1],"A");
   strcpy(test_d->program->words[2],"FROM");
   strcpy(test_d->program->words[3],"1");
   strcpy(test_d->program->words[4],"TO");
   strcpy(test_d->program->words[5],"2");
   strcpy(test_d->program->words[6],"{");
   strcpy(test_d->program->words[7],"FD");
   strcpy(test_d->program->words[8],"A");
   strcpy(test_d->program->words[9],"}");

   TEST_ASSERT_TRUE(!advance_to_mistake(test_d));
   collate_instruct_messages(test_d,test_str_2);

   TEST_ASSERT_TRUE(strcmp(test_str_2,"error around word 9 }\nmissing bracket in code\n")==0);
   test_d->program->capacity=MAXTESTCAP;
   free_debugger(test_d);


   test_d=init_debugger();
   test_d->program = init_word_cont();
   test_d->output= init_line_cont();
   test_d->program->capacity=10;
   strcpy(test_d->program->words[0],"DO");
   strcpy(test_d->program->words[1],"A");
   strcpy(test_d->program->words[2],"FROM");
   strcpy(test_d->program->words[3],"3");
   strcpy(test_d->program->words[4],"TO");
   strcpy(test_d->program->words[5],"2");
   strcpy(test_d->program->words[6],"{");
   strcpy(test_d->program->words[7],"FD");
   strcpy(test_d->program->words[8],"fhhjg");
   strcpy(test_d->program->words[9],"}");

   TEST_ASSERT_TRUE(!advance_to_mistake(test_d));
   collate_instruct_messages(test_d,test_str_2);
   TEST_ASSERT_TRUE(strcmp(test_str_2,"error around word 8 fhhjg\nsyntax error in unused loop\n")==0);
   test_d->program->capacity=MAXTESTCAP;
   free_debugger(test_d);

   test_d=init_debugger();
   test_d->program = init_word_cont();
   test_d->output= init_line_cont();
   test_d->program->capacity=10;
   strcpy(test_d->program->words[0],"DO");
   strcpy(test_d->program->words[1],"A");
   strcpy(test_d->program->words[2],"FROM");
   strcpy(test_d->program->words[3],"1");
   strcpy(test_d->program->words[4],"TO");
   strcpy(test_d->program->words[5],"2");
   strcpy(test_d->program->words[6],"{");
   strcpy(test_d->program->words[7],"FD");
   strcpy(test_d->program->words[8],"B");
   strcpy(test_d->program->words[9],"}");

   TEST_ASSERT_TRUE(!advance_to_mistake(test_d));
   collate_instruct_messages(test_d,test_str_2);
   TEST_ASSERT_TRUE(strcmp(test_str_2,"error around word 7 FD\npotentially haven't set variable before calling it\n")==0);
   test_d->program->capacity=MAXTESTCAP;
   free_debugger(test_d);

   test_d=init_debugger();
   test_d->program = init_word_cont();
   test_d->output= init_line_cont();
   test_d->program->capacity=11;
   strcpy(test_d->program->words[0],"DO");
   strcpy(test_d->program->words[1],"A");
   strcpy(test_d->program->words[2],"FROM");
   strcpy(test_d->program->words[3],"1");
   strcpy(test_d->program->words[4],"TO");
   strcpy(test_d->program->words[5],"2");
   strcpy(test_d->program->words[6],"{");
   strcpy(test_d->program->words[7],"FD");
   strcpy(test_d->program->words[8],"A");
   strcpy(test_d->program->words[9],"}");
   strcpy(test_d->program->words[10],"}");

   TEST_ASSERT_TRUE(advance_to_mistake(test_d));
   collate_instruct_messages(test_d,test_str_2);
   TEST_ASSERT_TRUE(strcmp(test_str_2,"no mistakes found. At end of program")==0);
   test_d->program->capacity=MAXTESTCAP;
   free_debugger(test_d);
}


void test_run_action(void)
{
   debugger* test_d;
   char word1[MAXACTIONLEN];
   char test_str_2[FULLARGSTRLEN];
   test_d=init_debugger();
   test_d->program = init_word_cont();
   test_d->output= init_line_cont();
   test_d->program->capacity=11;
   strcpy(test_d->program->words[0],"DO");
   strcpy(test_d->program->words[1],"A");
   strcpy(test_d->program->words[2],"FROM");
   strcpy(test_d->program->words[3],"1");
   strcpy(test_d->program->words[4],"TO");
   strcpy(test_d->program->words[5],"2");
   strcpy(test_d->program->words[6],"{");
   strcpy(test_d->program->words[7],"FD");
   strcpy(test_d->program->words[8],"A");
   strcpy(test_d->program->words[9],"}");
   strcpy(test_d->program->words[10],"}");
   TEST_ASSERT_TRUE(run_action(test_d,"show vars\n",word1,test_str_2));
   TEST_ASSERT_TRUE(strcmp(test_str_2,"A= NA B= NA C= NA D= NA E= NA \nF= NA G= NA H= NA I= NA J= NA \nK= NA L= NA M= NA N= NA O= NA \nP= NA Q= NA R= NA S= NA T= NA \nU= NA V= NA W= NA X= NA Y= NA \nZ= NA ")==0);
   TEST_ASSERT_TRUE(run_action(test_d,"single step\n",word1,test_str_2));
   TEST_ASSERT_TRUE(run_action(test_d,"show vars\n",word1,test_str_2));
   TEST_ASSERT_TRUE(strcmp(test_str_2,"A= 1.00 B= NA C= NA D= NA E= NA \nF= NA G= NA H= NA I= NA J= NA \nK= NA L= NA M= NA N= NA O= NA \nP= NA Q= NA R= NA S= NA T= NA \nU= NA V= NA W= NA X= NA Y= NA \nZ= NA ")==0);
   TEST_ASSERT_TRUE(run_action(test_d,"advance to error\n",word1,test_str_2));
   TEST_ASSERT_TRUE(test_d->program->position==10);

   test_d->program->capacity=MAXTESTCAP;
   free_debugger(test_d);

   test_d=init_debugger();
   test_d->program = init_word_cont();
   test_d->output= init_line_cont();
   test_d->program->capacity=2;
   test_d->program->position=1;
   strcpy(test_d->program->words[0],"{");
   strcpy(test_d->program->words[1],"}");
   TEST_ASSERT_TRUE(run_action(test_d,"advance to error\n",word1,test_str_2));
   TEST_ASSERT_EQUAL_INT(test_d->program->position,1);
   collate_instruct_messages(test_d,test_str_2);
   TEST_ASSERT_TRUE(strcmp(test_str_2,"no mistakes found. At end of program")==0);
   test_d->program->capacity=MAXTESTCAP;
   free_debugger(test_d);


   test_d=init_debugger();
   test_d->program = init_word_cont();
   test_d->output= init_line_cont();
   test_d->program->capacity=1;
   strcpy(test_d->program->words[0],"{{");
   TEST_ASSERT_TRUE(run_action(test_d,"advance to error\n",word1,test_str_2));
   TEST_ASSERT_EQUAL_INT(test_d->program->position,0);
   collate_instruct_messages(test_d,test_str_2);
   TEST_ASSERT_TRUE(strcmp("error around word 0 {{\nmissing bracket in code\n",test_str_2)==0);
   test_d->program->capacity=MAXTESTCAP;
   free_debugger(test_d);


   test_d=init_debugger();
   test_d->program = init_word_cont();
   test_d->output= init_line_cont();
   strcpy(test_d->program->words[0],"{");
   TEST_ASSERT_TRUE(check_start(test_d));
   free_debugger(test_d);

   test_d=init_debugger();
   test_d->program = init_word_cont();
   test_d->output= init_line_cont();
   strcpy(test_d->program->words[0],"{{");
   TEST_ASSERT_TRUE(!check_start(test_d));
   TEST_ASSERT_TRUE(strcmp(test_d->info,"error in starting bracket")==0);
   free_debugger(test_d);

   test_d=init_debugger();
   test_d->program = read_in_file("test_files/test_turtles/parser/invalid/missing_bracket.ttl");

   test_d->output= init_line_cont();
   TEST_ASSERT_TRUE(!check_start(test_d));
   TEST_ASSERT_EQUAL_INT(test_d->program->position,0);
   free_debugger(test_d);

   test_d=init_debugger();
   test_d->program = read_in_file("test_files/test_turtles/interpreter/valid/circle_with_more_circles.ttl");

   test_d->output= init_line_cont();
   TEST_ASSERT_TRUE(check_start(test_d));
   test_d->program->position++;
   TEST_ASSERT_TRUE(run_action(test_d,"advance to error\n",word1,test_str_2));
   TEST_ASSERT_EQUAL_INT(test_d->program->position,32);
   free_debugger(test_d);


}

void test_fuzzy(void)
{
   char word1[MAXLEN],word2[MAXLEN],test_s[FULLARGSTRLEN];
   debugger* test_d;
   TEST_ASSERT_EQUAL_INT(min_three(1,2,3),1);
   TEST_ASSERT_EQUAL_INT(min_three(1,1,3),1);
   TEST_ASSERT_EQUAL_INT(min_three(3,200,3),3);
   TEST_ASSERT_EQUAL_INT(levenshtein("sitting","kitten"),3);
   TEST_ASSERT_EQUAL_INT(levenshtein("TO","DO"),1);
   /*caps sensitive*/
   TEST_ASSERT_EQUAL_INT(levenshtein("TO","to"),2);
   TEST_ASSERT_EQUAL_INT(levenshtein("TO",""),2);
   TEST_ASSERT_EQUAL_INT(levenshtein("TO","123"),3);
   strcpy(word1,"hello");
   str_to_upper(word1,word2);
   TEST_ASSERT_TRUE(strcmp(word2,"HELLO")==0);
   strcpy(word1,"he11o");
   str_to_upper(word1,word2);
   TEST_ASSERT_TRUE(strcmp(word2,"HE11O")==0);

   strcpy(word1,"TD");
   suggest_keyword(word1,test_s);
   TEST_ASSERT_TRUE(strcmp(test_s,"TO")==0);
   strcpy(word1,"td");
   suggest_keyword(word1,test_s);
   TEST_ASSERT_TRUE(strcmp(test_s,"TO")==0);

   strcpy(word1,"FRAM");
   suggest_keyword(word1,test_s);
   TEST_ASSERT_TRUE(strcmp(test_s,"FROM")==0);

   strcpy(word1,"SE");
   suggest_keyword(word1,test_s);
   TEST_ASSERT_TRUE(strcmp(test_s,"SET")==0);

   strcpy(word1,"set");
   suggest_keyword(word1,test_s);
   TEST_ASSERT_TRUE(strcmp(test_s,"SET")==0);


   strcpy(word1,"abhehdhdjdjdnn");
   suggest_keyword(word1,test_s);
   TEST_ASSERT_TRUE(strcmp(test_s,"no suggestion possible")==0);

   strcpy(word1,"");
   suggest_keyword(word1,test_s);
   TEST_ASSERT_TRUE(strcmp(test_s,"no suggestion possible:blank string?")==0);


   test_d=init_debugger();
   test_d->program = init_word_cont();
   test_d->output= init_line_cont();
   strcpy(test_d->program->words[0],"td");
   make_suggestion(test_d,test_s);
   TEST_ASSERT_TRUE(strcmp(test_s,"TO")==0);
   strcpy(test_d->program->words[0],"");
   make_suggestion(test_d,test_s);
   TEST_ASSERT_TRUE(strcmp(test_s,"no suggestion possible:blank string?")==0);
   strcpy(test_d->program->words[0],"<<");
   make_suggestion(test_d,test_s);
   /*its not perfect*/
   TEST_ASSERT_TRUE(strcmp(test_s,"TO")==0);

   strcpy(test_d->program->words[0],"fram");
   make_suggestion(test_d,test_s);
   TEST_ASSERT_TRUE(strcmp(test_s,"FROM")==0);
   free_debugger(test_d);
}

void test_find_end(void)
{
   word_cont* test_cont;
   test_cont=init_word_cont();
   strcpy(test_cont->words[0],"{");
   strcpy(test_cont->words[1],"FD");
   strcpy(test_cont->words[2],"40");


   strcpy(test_cont->words[3],"DO");
   strcpy(test_cont->words[4],"B");
   strcpy(test_cont->words[5],"FROM");
   strcpy(test_cont->words[6],"A");
   strcpy(test_cont->words[7],"TO");
   strcpy(test_cont->words[8],"30");
   strcpy(test_cont->words[9],"{");

   strcpy(test_cont->words[10],"}");
   strcpy(test_cont->words[11],"}");
   TEST_ASSERT_EQUAL_INT(12,find_end_pos(test_cont,0));
   test_cont->position=0;
   strcpy(test_cont->words[0],"{");
   strcpy(test_cont->words[1],"{");
   strcpy(test_cont->words[2],"}");


   strcpy(test_cont->words[3],"{");
   strcpy(test_cont->words[4],"{");
   strcpy(test_cont->words[5],"}");
   strcpy(test_cont->words[6],"}");
   strcpy(test_cont->words[7],"TO");
   strcpy(test_cont->words[8],"30");
   strcpy(test_cont->words[9],"{");

   strcpy(test_cont->words[10],"}");
   strcpy(test_cont->words[11],"}");
   TEST_ASSERT_EQUAL_INT(12,find_end_pos(test_cont,0));
   test_cont->position=0;

   strcpy(test_cont->words[0],"{");
   strcpy(test_cont->words[1],"{");
   strcpy(test_cont->words[2],"}");


   strcpy(test_cont->words[3],"{");
   strcpy(test_cont->words[4],"{");
   strcpy(test_cont->words[5],"AD");
   strcpy(test_cont->words[6],"}");
   strcpy(test_cont->words[7],"}");
   strcpy(test_cont->words[8],"30");
   strcpy(test_cont->words[9],"{");

   strcpy(test_cont->words[10],"}");
   strcpy(test_cont->words[11],"}");
   TEST_ASSERT_EQUAL_INT(12,find_end_pos(test_cont,0));
   test_cont->position=0;


   strcpy(test_cont->words[0],"{");
   strcpy(test_cont->words[1],"}");
   strcpy(test_cont->words[2],"}");


   strcpy(test_cont->words[3],"{");
   strcpy(test_cont->words[4],"{");
   strcpy(test_cont->words[5],"AD");
   strcpy(test_cont->words[6],"}");
   strcpy(test_cont->words[7],"}");
   strcpy(test_cont->words[8],"30");
   strcpy(test_cont->words[9],"{");

   strcpy(test_cont->words[10],"}");
   strcpy(test_cont->words[11],"}");
   TEST_ASSERT_EQUAL_INT(2,find_end_pos(test_cont,0));
   test_cont->position=0;

   strcpy(test_cont->words[0],"{");
   strcpy(test_cont->words[1],"{");
   strcpy(test_cont->words[2],"{");


   strcpy(test_cont->words[3],"{");
   strcpy(test_cont->words[4],"{");
   strcpy(test_cont->words[5],"AD");
   strcpy(test_cont->words[6],"}");
   strcpy(test_cont->words[7],"}");
   strcpy(test_cont->words[8],"30");
   strcpy(test_cont->words[9],"{");

   strcpy(test_cont->words[10],"}");
   strcpy(test_cont->words[11],"}");
   TEST_ASSERT_EQUAL_INT(-1,find_end_pos(test_cont,0));

}

void test_break_points(void)
{
   debugger* test_d;
   char word1[MAXACTIONLEN],test_s[FULLARGSTRLEN];
   test_d=init_debugger();
   test_d->program = init_word_cont();
   test_d->output= init_line_cont();
   test_d->program->capacity=7;
   strcpy(test_d->program->words[0],"FD");
   strcpy(test_d->program->words[1],"10");
   strcpy(test_d->program->words[2],"FD");
   strcpy(test_d->program->words[3],"10");
   strcpy(test_d->program->words[4],"FD");
   strcpy(test_d->program->words[5],"10");
   strcpy(test_d->program->words[6],"}");
   run_set_break(test_d,"4",test_s);
   TEST_ASSERT_TRUE(run_action(test_d,"advance to error\n",word1,test_s));
   TEST_ASSERT_EQUAL_INT(test_d->program->position,4);
   TEST_ASSERT_TRUE(strcmp(test_d->info,"at break point")==0);
   run_rm_break(test_d);
   TEST_ASSERT_TRUE(run_action(test_d,"advance to error\n",word1,test_s));
   TEST_ASSERT_EQUAL_INT(test_d->program->position,6);
   TEST_ASSERT_TRUE(strcmp(test_d->info,"no mistakes found. At end of program")==0);
   free_debugger(test_d);

   test_d=init_debugger();
   test_d->program = init_word_cont();
   test_d->output= init_line_cont();
   test_d->program->capacity=7;
   strcpy(test_d->program->words[0],"FD");
   strcpy(test_d->program->words[1],"10");
   strcpy(test_d->program->words[2],"FD");
   strcpy(test_d->program->words[3],"10");
   strcpy(test_d->program->words[4],"FD");
   strcpy(test_d->program->words[5],"10");
   strcpy(test_d->program->words[6],"}");
   run_set_break(test_d,"3",test_s);
   TEST_ASSERT_TRUE(run_action(test_d,"advance to error\n",word1,test_s));
   TEST_ASSERT_EQUAL_INT(test_d->program->position,4);
   TEST_ASSERT_TRUE(run_action(test_d,"advance to error\n",word1,test_s));
   TEST_ASSERT_EQUAL_INT(test_d->program->position,4);
   TEST_ASSERT_TRUE(strcmp(test_d->info,"at break point")==0);
   run_rm_break(test_d);
   TEST_ASSERT_TRUE(run_action(test_d,"advance to error\n",word1,test_s));
   TEST_ASSERT_EQUAL_INT(test_d->program->position,6);
   free_debugger(test_d);

   test_d=init_debugger();
   test_d->program = init_word_cont();
   test_d->output= init_line_cont();
   test_d->program->capacity=7;
   strcpy(test_d->program->words[0],"FD");
   strcpy(test_d->program->words[1],"10");
   strcpy(test_d->program->words[2],"FD");
   strcpy(test_d->program->words[3],"10");
   strcpy(test_d->program->words[4],"FD");
   strcpy(test_d->program->words[5],"10");
   strcpy(test_d->program->words[6],"}");
   TEST_ASSERT_TRUE(run_action(test_d,"single step\n",word1,test_s));
   run_set_break(test_d,"0",test_s);
   TEST_ASSERT_TRUE(run_action(test_d,"single step\n",word1,test_s));
   TEST_ASSERT_EQUAL_INT(test_d->program->position,2);
   TEST_ASSERT_TRUE(run_action(test_d,"single step\n",word1,test_s));
   TEST_ASSERT_EQUAL_INT(test_d->program->position,2);
   run_rm_break(test_d);
   run_set_break(test_d,"-10",test_s);
   TEST_ASSERT_TRUE(strcmp(test_s,"cant set negative break points")==0);
   TEST_ASSERT_EQUAL_INT(test_d->break_p,-1);
   run_set_break(test_d,"1aaaaa",test_s);
   TEST_ASSERT_EQUAL_INT(test_d->break_p,1);
   free_debugger(test_d);

   /*if loop not executed and breakpoint set in loop
   program will go to end of loop*/
   test_d=init_debugger();
   test_d->program = init_word_cont();
   test_d->output= init_line_cont();
   test_d->program->capacity=13;
   strcpy(test_d->program->words[0],"FD");
   strcpy(test_d->program->words[1],"10");
   strcpy(test_d->program->words[2],"DO");
   strcpy(test_d->program->words[3],"A");
   strcpy(test_d->program->words[4],"FROM");
   strcpy(test_d->program->words[5],"10");
   strcpy(test_d->program->words[6],"TO");
   strcpy(test_d->program->words[7],"9");
   strcpy(test_d->program->words[8],"{");
   strcpy(test_d->program->words[9],"FD");
   strcpy(test_d->program->words[10],"10");
   strcpy(test_d->program->words[11],"}");
   strcpy(test_d->program->words[12],"FD");
   strcpy(test_d->program->words[13],"10");
   strcpy(test_d->program->words[14],"}");
   run_set_break(test_d,"9",test_s);
   TEST_ASSERT_TRUE(run_action(test_d,"advance to error\n",word1,test_s));
   TEST_ASSERT_EQUAL_INT(test_d->program->position,12);
   TEST_ASSERT_TRUE(run_action(test_d,"single step\n",word1,test_s));
   TEST_ASSERT_EQUAL_INT(test_d->program->position,12);

   free_debugger(test_d);

   test_d=init_debugger();
   test_d->program = init_word_cont();
   test_d->output= init_line_cont();
   test_d->program->capacity=13;
   strcpy(test_d->program->words[0],"FD");
   strcpy(test_d->program->words[1],"10");
   strcpy(test_d->program->words[2],"DO");
   strcpy(test_d->program->words[3],"A");
   strcpy(test_d->program->words[4],"FROM");
   strcpy(test_d->program->words[5],"8");
   strcpy(test_d->program->words[6],"TO");
   strcpy(test_d->program->words[7],"9");
   strcpy(test_d->program->words[8],"{");
   strcpy(test_d->program->words[9],"FD");
   strcpy(test_d->program->words[10],"10");
   strcpy(test_d->program->words[11],"}");
   strcpy(test_d->program->words[12],"FD");
   strcpy(test_d->program->words[13],"10");
   strcpy(test_d->program->words[14],"}");
   run_set_break(test_d,"9",test_s);
   TEST_ASSERT_TRUE(run_action(test_d,"advance to error\n",word1,test_s));
   TEST_ASSERT_EQUAL_INT(test_d->program->position,9);
   TEST_ASSERT_TRUE(run_action(test_d,"single step\n",word1,test_s));
   TEST_ASSERT_EQUAL_INT(test_d->program->position,9);

   free_debugger(test_d);


   test_d=init_debugger();
   test_d->program = init_word_cont();
   test_d->output= init_line_cont();
   test_d->program->capacity=13;
   strcpy(test_d->program->words[0],"FD");
   strcpy(test_d->program->words[1],"10");
   strcpy(test_d->program->words[2],"DO");
   strcpy(test_d->program->words[3],"A");
   strcpy(test_d->program->words[4],"FROM");
   strcpy(test_d->program->words[5],"8");
   strcpy(test_d->program->words[6],"TO");
   strcpy(test_d->program->words[7],"9");
   strcpy(test_d->program->words[8],"{");
   strcpy(test_d->program->words[9],"FD");
   strcpy(test_d->program->words[10],"10");
   strcpy(test_d->program->words[11],"}");
   strcpy(test_d->program->words[12],"FD");
   strcpy(test_d->program->words[13],"10");
   strcpy(test_d->program->words[14],"}");
   TEST_ASSERT_TRUE(run_action(test_d,"set break\n","9",test_s));
   TEST_ASSERT_TRUE(run_action(test_d,"advance to error\n",word1,test_s));
   TEST_ASSERT_EQUAL_INT(test_d->program->position,9);
   TEST_ASSERT_TRUE(run_action(test_d,"single step\n",word1,test_s));
   TEST_ASSERT_EQUAL_INT(test_d->program->position,9);
   TEST_ASSERT_TRUE(run_action(test_d,"rm break\n",word1,test_s));
   TEST_ASSERT_TRUE(run_action(test_d,"single step\n",word1,test_s));
   TEST_ASSERT_EQUAL_INT(test_d->program->position,11);
   free_debugger(test_d);

}

int main(void)
{
   UNITY_BEGIN();
   RUN_TEST(test_step_instruction_basic);
   RUN_TEST(test_loops);
   RUN_TEST(test_step_instruct_loops);
   RUN_TEST(test_step_instruct_set);
   RUN_TEST(test_advance);
   RUN_TEST(test_code_after_main);
   RUN_TEST(test_show_args);
   RUN_TEST(test_show_code_pos);
   RUN_TEST(test_show_recent_coords);
   RUN_TEST(test_error_collate);
   RUN_TEST(test_run_action);
   RUN_TEST(test_fuzzy);
   RUN_TEST(test_find_end);
   RUN_TEST(test_break_points);
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
   return n_cont;
}
