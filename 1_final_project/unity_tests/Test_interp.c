#include "../src/unity.h"
#include "../interpreter_funcs.h"

#define MAXTESTCAP 100
#define MAXTESTLEN 50
#define EPSILON 0.000001

word_cont* init_word_cont(void);

void setUp(void)
{

}

void tearDown(void)
{

}


void test_coord_funcs(void)
{
   coord* test_coord,*test_coord2, *origin;
   int i;


   origin=init_coords(0,0);
   test_coord=init_coords(4,3);
   TEST_ASSERT_EQUAL_DOUBLE(4,test_coord->x);
   TEST_ASSERT_EQUAL_DOUBLE(3,test_coord->y);

   TEST_ASSERT_TRUE(rotate(180,test_coord,origin));
   TEST_ASSERT_EQUAL_DOUBLE(-4,test_coord->x);
   TEST_ASSERT_EQUAL_DOUBLE(-3,test_coord->y);
   TEST_ASSERT_TRUE(rotate(180,test_coord,origin));
   TEST_ASSERT_EQUAL_DOUBLE(4,test_coord->x);
   TEST_ASSERT_EQUAL_DOUBLE(3,test_coord->y);
   TEST_ASSERT_TRUE(rotate(90,test_coord,origin));
   TEST_ASSERT_EQUAL_DOUBLE(-3,test_coord->x);
   TEST_ASSERT_EQUAL_DOUBLE(4,test_coord->y);
   TEST_ASSERT_TRUE(rotate(270,test_coord,origin));
   TEST_ASSERT_EQUAL_DOUBLE(4,test_coord->x);
   TEST_ASSERT_EQUAL_DOUBLE(3,test_coord->y);
   free(test_coord);

   test_coord=init_coords(3,-4);
   TEST_ASSERT_TRUE(rotate(29.5, test_coord,origin));

   TEST_ASSERT_EQUAL_DOUBLE(4.580761,test_coord->x);
   TEST_ASSERT_EQUAL_DOUBLE(-2.004152,test_coord->y);
   free(test_coord);
   TEST_ASSERT_FALSE(rotate(29.5, NULL,origin));

   test_coord=init_coords(5,6);
   for(i=0;i<8;i++)
   {
      TEST_ASSERT_TRUE(rotate(45,test_coord,origin));
   }
   TEST_ASSERT_EQUAL_DOUBLE(5,test_coord->x);
   TEST_ASSERT_EQUAL_DOUBLE(6,test_coord->y);
   free(test_coord);

   test_coord2=init_coords(11,1.2);
   test_coord=init_coords(11,1.2);
   TEST_ASSERT_TRUE(rotate(10, test_coord,origin));
   TEST_ASSERT_TRUE(rotate(10, test_coord,origin));

   TEST_ASSERT_TRUE(rotate(20, test_coord2,origin));

   TEST_ASSERT_EQUAL_DOUBLE(test_coord2->x,test_coord->x);
   TEST_ASSERT_EQUAL_DOUBLE(test_coord2->y,test_coord->y);

   TEST_ASSERT_TRUE(rotate(0.5, test_coord,origin));
   TEST_ASSERT_TRUE(rotate(0.5, test_coord,origin));
   TEST_ASSERT_TRUE(rotate(1, test_coord2,origin));

   TEST_ASSERT_EQUAL_DOUBLE(test_coord2->x,test_coord->x);
   TEST_ASSERT_EQUAL_DOUBLE(test_coord2->y,test_coord->y);

   free(test_coord);
   free(test_coord2);
   free(origin);

   origin=init_coords(10,10);
   test_coord=init_coords(4,3);
   TEST_ASSERT_TRUE(rotate(180,test_coord,origin));
   TEST_ASSERT_EQUAL_DOUBLE(16,test_coord->x);
   TEST_ASSERT_EQUAL_DOUBLE(17,test_coord->y);

   TEST_ASSERT_TRUE(rotate(180,test_coord,origin));
   TEST_ASSERT_TRUE(rotate(270,test_coord,origin));
   TEST_ASSERT_EQUAL_DOUBLE(3,test_coord->x);
   TEST_ASSERT_EQUAL_DOUBLE(16,test_coord->y);
   free(test_coord);
   free(origin);
}

void test_get_num(void)
{
   word_cont* test_cont;
   double test_double;

   test_cont=init_word_cont();
   strcpy(test_cont->words[0],"123.4");
   TEST_ASSERT_TRUE(get_num(test_cont,&test_double));
   TEST_ASSERT_TRUE(test_cont->position==1);
   TEST_ASSERT_EQUAL_DOUBLE(test_double,123.4);


   strcpy(test_cont->words[0],"123.b");
   test_cont->position=0;

   TEST_ASSERT_FALSE(get_num(test_cont,&test_double));
   TEST_ASSERT_FALSE(get_num(NULL,&test_double));
   free_word_cont(test_cont);
}


void test_directions(void)
{
   word_cont* test_cont;
   line_cont* test_line_cont;

   test_cont=init_word_cont();
   strcpy(test_cont->words[0],"LT");
   strcpy(test_cont->words[1],"123.4");

   TEST_ASSERT_TRUE(direction_helper(test_cont)==left);

   strcpy(test_cont->words[0],"RT");
   test_cont->position=0;
   TEST_ASSERT_TRUE(direction_helper(test_cont)==right);

   strcpy(test_cont->words[0],"inv");
   test_cont->position=0;
   TEST_ASSERT_TRUE(direction_helper(test_cont)==invalid);
   /*position doesnt change when invalid*/
   TEST_ASSERT_TRUE(test_cont->position==0);

   test_cont->position=0;
   strcpy(test_cont->words[0],"LT");
   test_line_cont=init_line_cont();
   TEST_ASSERT_EQUAL_DOUBLE(test_line_cont->pending_line->rotation,0);
   TEST_ASSERT_EQUAL_DOUBLE(test_line_cont->pending_line->start->x,0);
   TEST_ASSERT_EQUAL_DOUBLE(test_line_cont->pending_line->start->y,0);
   TEST_ASSERT_TRUE(get_rotation(test_cont,test_line_cont));
   TEST_ASSERT_EQUAL_DOUBLE(test_line_cont->pending_line->rotation,123.4);
   TEST_ASSERT_TRUE(test_cont->position==2);

   strcpy(test_cont->words[2],"LT");
   strcpy(test_cont->words[3],"500");
   get_rotation(test_cont,test_line_cont);

   TEST_ASSERT_EQUAL_DOUBLE(test_line_cont->pending_line->rotation,263.4);
   strcpy(test_cont->words[4],"LT");
   strcpy(test_cont->words[5],"50");
   get_rotation(test_cont,test_line_cont);
   TEST_ASSERT_EQUAL_DOUBLE(test_line_cont->pending_line->rotation,313.4);

   strcpy(test_cont->words[6],"RT");
   strcpy(test_cont->words[7],"50");
   get_rotation(test_cont,test_line_cont);
   TEST_ASSERT_EQUAL_DOUBLE(test_line_cont->pending_line->rotation,263.4);

   strcpy(test_cont->words[8],"RT");
   strcpy(test_cont->words[9],"500");
   get_rotation(test_cont,test_line_cont);

   TEST_ASSERT_EQUAL_DOUBLE(test_line_cont->pending_line->rotation,123.4);

   strcpy(test_cont->words[10],"RT");
   strcpy(test_cont->words[11],"123.4");
   get_rotation(test_cont,test_line_cont);
   TEST_ASSERT_EQUAL_DOUBLE(test_line_cont->pending_line->rotation,0);

   free_word_cont(test_cont);
   free_line_cont(test_line_cont);
}

void test_line_cont(void)
{
   line_cont* test_line_cont;
   int i;
   line* test_line;
   test_line_cont=init_line_cont();
   TEST_ASSERT_TRUE(test_line_cont->size==0);
   TEST_ASSERT_TRUE(test_line_cont->capacity==INITSIZE);
   store_line(test_line_cont,NULL);
   TEST_ASSERT_TRUE(test_line_cont->size==0);

   for(i=0;i<10000;i++)
   {
      test_line=init_origin();
      TEST_ASSERT_TRUE(test_line_cont->size==i);
      store_line(test_line_cont,test_line);

   }

   TEST_ASSERT_TRUE(free_line(NULL));
   TEST_ASSERT_TRUE(free_line_cont(test_line_cont));
   TEST_ASSERT_TRUE(free_line_cont(NULL));

}

void test_move_forward(void)
{
   word_cont* test_cont;
   line_cont* test_line_cont;
   int i;
   test_line_cont=init_line_cont();
   test_cont=init_word_cont();

   strcpy(test_cont->words[0],"FD");
   strcpy(test_cont->words[1],"30");
   TEST_ASSERT_TRUE(move_forward(test_cont,test_line_cont));
   TEST_ASSERT_EQUAL_DOUBLE(test_line_cont->pending_line->start->y,30);
   TEST_ASSERT_EQUAL_DOUBLE(test_line_cont->pending_line->start->x,0);

   TEST_ASSERT_EQUAL_DOUBLE(test_line_cont->array[0]->start->y,0);
   TEST_ASSERT_EQUAL_DOUBLE(test_line_cont->array[0]->start->x,0);
   TEST_ASSERT_EQUAL_DOUBLE(test_line_cont->array[0]->end->y,30);
   TEST_ASSERT_EQUAL_DOUBLE(test_line_cont->array[0]->end->x,0);

   test_cont->position=0;
   strcpy(test_cont->words[0],"LT");
   strcpy(test_cont->words[1],"90");
   strcpy(test_cont->words[2],"FD");
   strcpy(test_cont->words[3],"30");
   TEST_ASSERT_TRUE(get_rotation(test_cont,test_line_cont));
   TEST_ASSERT_TRUE(move_forward(test_cont,test_line_cont));

   TEST_ASSERT_EQUAL_DOUBLE(test_line_cont->array[1]->end->y,30);
   TEST_ASSERT_EQUAL_DOUBLE(test_line_cont->array[1]->end->x,-30);
   TEST_ASSERT_EQUAL_DOUBLE(test_line_cont->pending_line->start->y,30);
   TEST_ASSERT_EQUAL_DOUBLE(test_line_cont->pending_line->start->x,-30);
   free_line_cont(test_line_cont);



   test_line_cont=init_line_cont();
   test_cont->position=0;
   strcpy(test_cont->words[0],"RT");
   strcpy(test_cont->words[1],"90");
   strcpy(test_cont->words[2],"FD");
   strcpy(test_cont->words[3],"30");
   TEST_ASSERT_TRUE(get_rotation(test_cont,test_line_cont));
   TEST_ASSERT_TRUE(move_forward(test_cont,test_line_cont));
   TEST_ASSERT_DOUBLE_WITHIN(EPSILON,test_line_cont->array[0]->end->y,0);
   TEST_ASSERT_EQUAL_DOUBLE(test_line_cont->array[0]->end->x,30);
   TEST_ASSERT_DOUBLE_WITHIN(EPSILON,test_line_cont->pending_line->start->y,0);
   TEST_ASSERT_EQUAL_DOUBLE(test_line_cont->pending_line->start->x,30);
   TEST_ASSERT_EQUAL_DOUBLE(test_line_cont->pending_line->rotation,270);

   free_line_cont(test_line_cont);


   test_cont->position=0;

   test_line_cont=init_line_cont();
   for(i=0;i<8;i++)
   {
      strcpy(test_cont->words[i*4],"FD");
      strcpy(test_cont->words[(i*4)+1],"30");
      strcpy(test_cont->words[(i*4)+2],"LT");
      strcpy(test_cont->words[(i*4)+3],"45");
   }

   for(i=0;i<8;i++)
   {

      TEST_ASSERT_TRUE(move_forward(test_cont,test_line_cont));
      TEST_ASSERT_TRUE(get_rotation(test_cont,test_line_cont));
   }

   TEST_ASSERT_DOUBLE_WITHIN(EPSILON,test_line_cont->pending_line->start->y,0);
   TEST_ASSERT_DOUBLE_WITHIN(EPSILON,test_line_cont->pending_line->start->x,0);
   TEST_ASSERT_DOUBLE_WITHIN(EPSILON,test_line_cont->pending_line->rotation,0);
   TEST_ASSERT_EQUAL_DOUBLE(test_line_cont->array[4]->rotation,180);
   TEST_ASSERT_TRUE(test_cont->position==(8*4));
   TEST_ASSERT_TRUE(free_line_cont(test_line_cont));

   test_cont->position=0;
   strcpy(test_cont->words[0],"NO");

   TEST_ASSERT_FALSE(move_forward(test_cont,test_line_cont));

   free_word_cont(test_cont);

   test_line_cont=init_line_cont();
   test_cont=init_word_cont();

   strcpy(test_cont->words[0],"FD");
   strcpy(test_cont->words[1],"-30");
   TEST_ASSERT_TRUE(move_forward(test_cont,test_line_cont));
   TEST_ASSERT_EQUAL_DOUBLE(test_line_cont->pending_line->start->y,-30);
   TEST_ASSERT_DOUBLE_WITHIN(EPSILON,test_line_cont->pending_line->start->x,0);
   free_word_cont(test_cont);
   free_line_cont(test_line_cont);


   test_line_cont=init_line_cont();
   test_cont=init_word_cont();

   strcpy(test_cont->words[0],"FD");
   strcpy(test_cont->words[1],"100000000");
   TEST_ASSERT_TRUE(move_forward(test_cont,test_line_cont));
   TEST_ASSERT_EQUAL_DOUBLE(test_line_cont->pending_line->start->y,100000000);
   TEST_ASSERT_DOUBLE_WITHIN(EPSILON,test_line_cont->pending_line->start->x,0);
   free_word_cont(test_cont);
   free_line_cont(test_line_cont);


}


void test_run_instruct_basic(void)
{
   word_cont* test_cont;
   line_cont* test_line_cont;

   test_line_cont=init_line_cont();
   test_cont=init_word_cont();

   strcpy(test_cont->words[0],"LT");
   strcpy(test_cont->words[1],"90");
   strcpy(test_cont->words[2],"FD");
   strcpy(test_cont->words[3],"30");



   TEST_ASSERT_TRUE(run_instruction(test_cont,test_line_cont));
   TEST_ASSERT_EQUAL_DOUBLE(test_line_cont->pending_line->rotation,90);
   TEST_ASSERT_TRUE(test_cont->position==2);
   TEST_ASSERT_TRUE(test_line_cont->size==0);

   TEST_ASSERT_TRUE(run_instruction(test_cont,test_line_cont));
   TEST_ASSERT_TRUE(test_cont->position==4);
   TEST_ASSERT_TRUE(test_line_cont->size==1);
   TEST_ASSERT_DOUBLE_WITHIN(EPSILON,test_line_cont->array[0]->end->y,0);
   TEST_ASSERT_EQUAL_DOUBLE(test_line_cont->array[0]->end->x,-30);
   TEST_ASSERT_DOUBLE_WITHIN(EPSILON,test_line_cont->pending_line->start->y,0);
   TEST_ASSERT_EQUAL_DOUBLE(test_line_cont->pending_line->start->x,-30);

   strcpy(test_cont->words[0],"L");
   test_cont->position=0;
   TEST_ASSERT_TRUE(!run_instruction(test_cont,test_line_cont));
   strcpy(test_cont->words[0],"LT");

   test_cont->position=0;
   strcpy(test_cont->words[1],"LT");
   TEST_ASSERT_TRUE(!run_instruction(test_cont,test_line_cont));
   strcpy(test_cont->words[1],"90");

   test_cont->position=0;
   strcpy(test_cont->words[0],"");
   TEST_ASSERT_TRUE(!run_instruction(test_cont,test_line_cont));
   strcpy(test_cont->words[0],"LT");


   free_line_cont(test_line_cont);
   free_word_cont(test_cont);

}

void test_basic_instruct_list(void)
{
   word_cont* test_cont;
   line_cont* test_line_cont;


   test_line_cont=init_line_cont();

   test_cont=init_word_cont();

   strcpy(test_cont->words[0],"LT");
   strcpy(test_cont->words[1],"90");
   strcpy(test_cont->words[2],"FD");
   strcpy(test_cont->words[3],"30");
   strcpy(test_cont->words[4],"}");
   TEST_ASSERT_TRUE(run_instruction_list(test_cont,test_line_cont));
   TEST_ASSERT_TRUE(test_cont->position==5);
   TEST_ASSERT_TRUE(test_line_cont->size==1);
   TEST_ASSERT_DOUBLE_WITHIN(EPSILON,test_line_cont->array[0]->end->y,0);
   TEST_ASSERT_EQUAL_DOUBLE(test_line_cont->array[0]->end->x,-30);
   TEST_ASSERT_DOUBLE_WITHIN(EPSILON,test_line_cont->pending_line->start->y,0);
   TEST_ASSERT_EQUAL_DOUBLE(test_line_cont->pending_line->start->x,-30);

   free_line_cont(test_line_cont);
   free_word_cont(test_cont);


   test_line_cont=init_line_cont();

   test_cont=init_word_cont();

   strcpy(test_cont->words[0],"LT");
   strcpy(test_cont->words[1],"90");
   strcpy(test_cont->words[2],"FD");
   strcpy(test_cont->words[3],"30");

   strcpy(test_cont->words[4],"LT");
   strcpy(test_cont->words[5],"90");
   strcpy(test_cont->words[6],"FD");
   strcpy(test_cont->words[7],"30");

   strcpy(test_cont->words[8],"LT");
   strcpy(test_cont->words[9],"90");
   strcpy(test_cont->words[10],"FD");
   strcpy(test_cont->words[11],"30");

   strcpy(test_cont->words[12],"RT");
   strcpy(test_cont->words[13],"90");
   strcpy(test_cont->words[14],"FD");
   strcpy(test_cont->words[15],"30");

   strcpy(test_cont->words[16],"}");
   TEST_ASSERT_FALSE(run_main(test_cont,test_line_cont));
   TEST_ASSERT_TRUE(run_instruction_list(test_cont,test_line_cont));
   TEST_ASSERT_TRUE(test_cont->position==17);
   TEST_ASSERT_TRUE(test_line_cont->size==4);
   TEST_ASSERT_EQUAL_DOUBLE(test_line_cont->pending_line->rotation,180);

   TEST_ASSERT_EQUAL_DOUBLE(test_line_cont->array[0]->rotation,90);
   TEST_ASSERT_EQUAL_DOUBLE(test_line_cont->array[1]->rotation,180);
   TEST_ASSERT_EQUAL_DOUBLE(test_line_cont->array[2]->rotation,270);
   TEST_ASSERT_EQUAL_DOUBLE(test_line_cont->array[3]->rotation,180);


   TEST_ASSERT_DOUBLE_WITHIN(EPSILON,test_line_cont->array[0]->start->x,0);
   TEST_ASSERT_DOUBLE_WITHIN(EPSILON,test_line_cont->array[0]->start->y,0);
   TEST_ASSERT_EQUAL_DOUBLE(test_line_cont->array[0]->end->x,-30);
   TEST_ASSERT_DOUBLE_WITHIN(EPSILON,test_line_cont->array[0]->end->y,0);

   TEST_ASSERT_EQUAL_DOUBLE(test_line_cont->array[1]->start->x,-30);
   TEST_ASSERT_DOUBLE_WITHIN(EPSILON,test_line_cont->array[1]->start->y,0);
   TEST_ASSERT_EQUAL_DOUBLE(test_line_cont->array[1]->end->x,-30);
   TEST_ASSERT_EQUAL_DOUBLE(test_line_cont->array[1]->end->y,-30);

   TEST_ASSERT_EQUAL_DOUBLE(test_line_cont->array[2]->start->x,-30);
   TEST_ASSERT_EQUAL_DOUBLE(test_line_cont->array[2]->start->y,-30);
   TEST_ASSERT_DOUBLE_WITHIN(EPSILON,test_line_cont->array[2]->end->x,0);
   TEST_ASSERT_EQUAL_DOUBLE(test_line_cont->array[2]->end->y,-30);


   TEST_ASSERT_DOUBLE_WITHIN(EPSILON,test_line_cont->array[3]->start->x,0);
   TEST_ASSERT_EQUAL_DOUBLE(test_line_cont->array[3]->start->y,-30);
   TEST_ASSERT_DOUBLE_WITHIN(EPSILON,test_line_cont->array[3]->end->x,0);
   TEST_ASSERT_EQUAL_DOUBLE(test_line_cont->array[3]->end->y,-60);


   test_cont->position=0;
   strcpy(test_cont->words[0],"NO");
   TEST_ASSERT_FALSE(run_instruction_list(test_cont,test_line_cont));
   strcpy(test_cont->words[0],"LT");

   test_cont->position=0;
   strcpy(test_cont->words[1],"LT");
   TEST_ASSERT_FALSE(run_instruction_list(test_cont,test_line_cont));
   strcpy(test_cont->words[1],"90");

   test_cont->position=0;
   strcpy(test_cont->words[16],"FD");
   TEST_ASSERT_FALSE(run_instruction_list(test_cont,test_line_cont));
   strcpy(test_cont->words[16],"}");

   test_cont->position=0;
   strcpy(test_cont->words[11],"}");
   TEST_ASSERT_FALSE(run_instruction_list(test_cont,test_line_cont));
   strcpy(test_cont->words[11],"30");

   test_cont->position=0;
   strcpy(test_cont->words[1],"");
   TEST_ASSERT_FALSE(run_instruction_list(test_cont,test_line_cont));
   strcpy(test_cont->words[1],"90");

   test_cont->position=0;
   strcpy(test_cont->words[1],"aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa90");
   TEST_ASSERT_FALSE(run_instruction_list(test_cont,test_line_cont));
   strcpy(test_cont->words[1],"90");
   free_word_cont(test_cont);
   free_line_cont(test_line_cont);
}


void test_basic_main(void)
{

   word_cont* test_cont;
   line_cont* test_line_cont;
   test_line_cont=init_line_cont();

   test_cont=init_word_cont();

   strcpy(test_cont->words[0],"{");
   strcpy(test_cont->words[1],"LT");
   strcpy(test_cont->words[2],"90");
   strcpy(test_cont->words[3],"FD");
   strcpy(test_cont->words[4],"30");

   strcpy(test_cont->words[5],"LT");
   strcpy(test_cont->words[6],"90");
   strcpy(test_cont->words[7],"FD");
   strcpy(test_cont->words[8],"30");

   strcpy(test_cont->words[9],"LT");
   strcpy(test_cont->words[10],"90");
   strcpy(test_cont->words[11],"FD");
   strcpy(test_cont->words[12],"30");

   strcpy(test_cont->words[13],"RT");
   strcpy(test_cont->words[14],"90");
   strcpy(test_cont->words[15],"FD");
   strcpy(test_cont->words[16],"30");

   strcpy(test_cont->words[17],"}");
   /*running main should be exactly the same so going to use same tests*/
   TEST_ASSERT_TRUE(!run_instruction_list(test_cont,test_line_cont));
   TEST_ASSERT_TRUE(run_main(test_cont,test_line_cont));

   free_line_cont(test_line_cont);


   test_cont->position=0;
   test_line_cont=init_line_cont();


   strcpy(test_cont->words[0],"{");
   strcpy(test_cont->words[1],"LT");
   strcpy(test_cont->words[2],"89.25");
   strcpy(test_cont->words[3],"FD");
   strcpy(test_cont->words[4],"30");

   strcpy(test_cont->words[5],"LT");
   strcpy(test_cont->words[6],"89.25");
   strcpy(test_cont->words[7],"FD");
   strcpy(test_cont->words[8],"30");

   strcpy(test_cont->words[9],"LT");
   strcpy(test_cont->words[10],"89.25");
   strcpy(test_cont->words[11],"FD");
   strcpy(test_cont->words[12],"30");

   strcpy(test_cont->words[13],"LT");
   strcpy(test_cont->words[14],"89.25");
   strcpy(test_cont->words[15],"FD");
   strcpy(test_cont->words[16],"30");

   strcpy(test_cont->words[17],"}");

   TEST_ASSERT_TRUE(run_main(test_cont,test_line_cont));

   TEST_ASSERT_EQUAL_DOUBLE(test_line_cont->pending_line->rotation,357);

   free_line_cont(test_line_cont);

   test_cont->position=0;
   test_line_cont=init_line_cont();


   strcpy(test_cont->words[0],"{");
   strcpy(test_cont->words[1],"LT");
   strcpy(test_cont->words[2],"-10");
   strcpy(test_cont->words[3],"FD");
   strcpy(test_cont->words[4],"30");

   strcpy(test_cont->words[5],"LT");
   strcpy(test_cont->words[6],"-10");
   strcpy(test_cont->words[7],"FD");
   strcpy(test_cont->words[8],"30");

   strcpy(test_cont->words[9],"LT");
   strcpy(test_cont->words[10],"-45");
   strcpy(test_cont->words[11],"FD");
   strcpy(test_cont->words[12],"30");

   strcpy(test_cont->words[13],"LT");
   strcpy(test_cont->words[14],"46");
   strcpy(test_cont->words[15],"FD");
   strcpy(test_cont->words[16],"30");

   strcpy(test_cont->words[17],"}");

   TEST_ASSERT_TRUE(run_main(test_cont,test_line_cont));
   TEST_ASSERT_EQUAL_DOUBLE(test_line_cont->pending_line->rotation,341);
   free_line_cont(test_line_cont);
   free_word_cont(test_cont);
}


void test_polish(void)
{
   word_cont* test_cont;
   double test_double;
   test_cont=init_word_cont();
   strcpy(test_cont->words[0],"+");
   TEST_ASSERT_TRUE(get_op(test_cont)==plus);
   strcpy(test_cont->words[0],"-");
   TEST_ASSERT_TRUE(get_op(test_cont)==minus);
   strcpy(test_cont->words[0],"*");
   TEST_ASSERT_TRUE(get_op(test_cont)==mult);
   strcpy(test_cont->words[0],"/");
   TEST_ASSERT_TRUE(get_op(test_cont)==divide);
   strcpy(test_cont->words[0],"29");
   TEST_ASSERT_TRUE(get_op(test_cont)==invalid_op);


   TEST_ASSERT_TRUE(polish_num(test_cont));
   TEST_ASSERT_TRUE(test_cont->position==1);
   TEST_ASSERT_EQUAL_DOUBLE(*(double*)test_cont->stackptr->start->data,29);
   TEST_ASSERT_TRUE(test_cont->stackptr->size==1);
   strcpy(test_cont->words[1],"90");
   TEST_ASSERT_TRUE(polish_num(test_cont));
   TEST_ASSERT_EQUAL_DOUBLE(*(double*)test_cont->stackptr->start->data,90);
   TEST_ASSERT_EQUAL_DOUBLE(*(double*)test_cont->stackptr->start->next->data,29);
   TEST_ASSERT_TRUE(test_cont->stackptr->size==2);


   strcpy(test_cont->words[2],"no");
   TEST_ASSERT_TRUE(!polish_num(test_cont));
   TEST_ASSERT_TRUE(test_cont->stackptr->size==2);

   strcpy(test_cont->words[2],"70.a");
   TEST_ASSERT_TRUE(!polish_num(test_cont));
   TEST_ASSERT_TRUE(test_cont->stackptr->size==2);

   strcpy(test_cont->words[2],"");
   TEST_ASSERT_TRUE(!polish_num(test_cont));
   TEST_ASSERT_TRUE(test_cont->stackptr->size==2);

   strcpy(test_cont->words[2],"+");
   TEST_ASSERT_TRUE(do_operation(test_cont));
   TEST_ASSERT_EQUAL_DOUBLE(*(double*)test_cont->stackptr->start->data,119);
   TEST_ASSERT_TRUE(!test_cont->stackptr->start->next);
   TEST_ASSERT_TRUE(test_cont->stackptr->size==1);
   TEST_ASSERT_TRUE(test_cont->position==3);

   strcpy(test_cont->words[3],"1.1");
   TEST_ASSERT_TRUE(polish_num(test_cont));
   strcpy(test_cont->words[4],"-");
   TEST_ASSERT_TRUE(do_operation(test_cont));
   TEST_ASSERT_EQUAL_DOUBLE(*(double*)test_cont->stackptr->start->data,117.9);

   strcpy(test_cont->words[5],"2");
   TEST_ASSERT_TRUE(polish_num(test_cont));
   strcpy(test_cont->words[6],"/");
   TEST_ASSERT_TRUE(do_operation(test_cont));
   TEST_ASSERT_EQUAL_DOUBLE(*(double*)test_cont->stackptr->start->data,58.95);
   strcpy(test_cont->words[7],"*");
   TEST_ASSERT_TRUE(!do_operation(test_cont));

   strcpy(test_cont->words[7],"56");
   TEST_ASSERT_TRUE(polish_num(test_cont));
   strcpy(test_cont->words[8],")");
   TEST_ASSERT_TRUE(!do_operation(test_cont));
   strcpy(test_cont->words[8],"");
   TEST_ASSERT_TRUE(!do_operation(test_cont));

   free_word_cont(test_cont);

   test_cont=init_word_cont();
   /*not enough numbers*/
   TEST_ASSERT_TRUE(!finish_polish(test_cont,&test_double));
   strcpy(test_cont->words[0],"90");
   TEST_ASSERT_TRUE(polish_num(test_cont));
   strcpy(test_cont->words[1],"90");
   TEST_ASSERT_TRUE(polish_num(test_cont));
   /*too many numbers*/
   TEST_ASSERT_TRUE(!finish_polish(test_cont,&test_double));
   free_word_cont(test_cont);

   test_cont=init_word_cont();
   strcpy(test_cont->words[0],"90");
   TEST_ASSERT_TRUE(polish_num(test_cont));
   strcpy(test_cont->words[1],"90");
   TEST_ASSERT_TRUE(polish_num(test_cont));
   strcpy(test_cont->words[2],"+");
   TEST_ASSERT_TRUE(do_operation(test_cont));
   /*goldilocks amount*/
   TEST_ASSERT_TRUE(finish_polish(test_cont,&test_double));
   TEST_ASSERT_EQUAL_DOUBLE(test_double,180);
   free_word_cont(test_cont);


   test_cont=init_word_cont();
   strcpy(test_cont->words[0],"90");
   strcpy(test_cont->words[1],"90");
   strcpy(test_cont->words[2],"+");
   strcpy(test_cont->words[3],";");
   TEST_ASSERT_TRUE(run_polish(test_cont,&test_double));
   TEST_ASSERT_EQUAL_DOUBLE(test_double,180);
   free_word_cont(test_cont);

   test_cont=init_word_cont();
   strcpy(test_cont->words[0],"90");
   strcpy(test_cont->words[1],"90");
   strcpy(test_cont->words[2],"+");
   strcpy(test_cont->words[3],"40");
   strcpy(test_cont->words[4],"/");
   strcpy(test_cont->words[5],"19.3");
   strcpy(test_cont->words[6],"-");
   strcpy(test_cont->words[7],";");
   TEST_ASSERT_TRUE(run_polish(test_cont,&test_double));
   TEST_ASSERT_EQUAL_DOUBLE(test_double,-14.8);
   free_word_cont(test_cont);

   test_cont=init_word_cont();
   strcpy(test_cont->words[0],"90");
   strcpy(test_cont->words[1],";");
   TEST_ASSERT_TRUE(run_polish(test_cont,&test_double));
   TEST_ASSERT_EQUAL_DOUBLE(test_double,90);
   free_word_cont(test_cont);

   test_cont=init_word_cont();
   strcpy(test_cont->words[0],";");
   TEST_ASSERT_TRUE(!run_polish(test_cont,&test_double));
   TEST_ASSERT_TRUE(test_cont->position==0);

   strcpy(test_cont->words[0],"7");
   TEST_ASSERT_TRUE(!run_polish(test_cont,&test_double));

   strcpy(test_cont->words[1],"7");
   strcpy(test_cont->words[2],";");
   TEST_ASSERT_TRUE(!run_polish(test_cont,&test_double));
   free_word_cont(test_cont);

   test_cont=init_word_cont();
   strcpy(test_cont->words[0],"7");
   strcpy(test_cont->words[1],"7");
   strcpy(test_cont->words[2],"+");
   strcpy(test_cont->words[3],"+");
   strcpy(test_cont->words[4],";");
   TEST_ASSERT_TRUE(!run_polish(test_cont,&test_double));
   free_word_cont(test_cont);

}


void test_set(void)
{
   word_cont* test_cont;
   int i;
   double test_double;

   test_cont=init_word_cont();
   strcpy(test_cont->words[0],"A");
   TEST_ASSERT_TRUE(valid_variable(test_cont));
   strcpy(test_cont->words[0],"Z");
   TEST_ASSERT_TRUE(valid_variable(test_cont));
   strcpy(test_cont->words[0],"a");
   TEST_ASSERT_TRUE(!valid_variable(test_cont));
   strcpy(test_cont->words[0],"");
   TEST_ASSERT_TRUE(!valid_variable(test_cont));
   strcpy(test_cont->words[0],"!");
   TEST_ASSERT_TRUE(!valid_variable(test_cont));

   strcpy(test_cont->words[0],"A");
   TEST_ASSERT_TRUE(get_var_pos(test_cont,&i));
   TEST_ASSERT_TRUE(i==0);
   test_cont->position=0;

   strcpy(test_cont->words[0],"Z");
   TEST_ASSERT_TRUE(get_var_pos(test_cont,&i));
   TEST_ASSERT_TRUE(i==25);
   test_cont->position=0;

   strcpy(test_cont->words[0],"B");
   TEST_ASSERT_TRUE(get_var_pos(test_cont,&i));
   TEST_ASSERT_TRUE(i==1);
   test_cont->position=0;


   strcpy(test_cont->words[0],"0");
   TEST_ASSERT_TRUE(!get_var_pos(test_cont,&i));
   strcpy(test_cont->words[0],"AB");
   TEST_ASSERT_TRUE(!get_var_pos(test_cont,&i));
   strcpy(test_cont->words[0],"a");
   TEST_ASSERT_TRUE(!get_var_pos(test_cont,&i));

   free_word_cont(test_cont);


   test_cont=init_word_cont();

   strcpy(test_cont->words[0],"SET");
   strcpy(test_cont->words[1],"A");
   strcpy(test_cont->words[2],":=");
   strcpy(test_cont->words[3],"90");
   strcpy(test_cont->words[4],"90");
   strcpy(test_cont->words[5],"+");
   strcpy(test_cont->words[6],";");
   TEST_ASSERT_TRUE(run_set(test_cont));
   TEST_ASSERT_EQUAL_DOUBLE(*test_cont->var_array[0],180);
   free_word_cont(test_cont);


   test_cont=init_word_cont();

   strcpy(test_cont->words[0],"SET");
   strcpy(test_cont->words[1],"B");
   strcpy(test_cont->words[2],":=");
   strcpy(test_cont->words[3],"90");
   strcpy(test_cont->words[4],"2");
   strcpy(test_cont->words[5],"/");
   strcpy(test_cont->words[6],";");
   TEST_ASSERT_TRUE(run_set(test_cont));
   TEST_ASSERT_EQUAL_DOUBLE(*test_cont->var_array[1],45);
   free_word_cont(test_cont);

   test_cont=init_word_cont();
   strcpy(test_cont->words[0],"SET");
   strcpy(test_cont->words[1],"Z");
   strcpy(test_cont->words[2],":=");
   strcpy(test_cont->words[3],"90.5");
   strcpy(test_cont->words[4],"2");
   strcpy(test_cont->words[5],"/");
   strcpy(test_cont->words[6],";");
   TEST_ASSERT_TRUE(run_set(test_cont));
   TEST_ASSERT_EQUAL_DOUBLE(*test_cont->var_array[25],45.25);
   free_word_cont(test_cont);

   test_cont=init_word_cont();
   strcpy(test_cont->words[0],"SET");
   strcpy(test_cont->words[1],"Z");
   strcpy(test_cont->words[2],":=");
   strcpy(test_cont->words[3],"90.5");
   strcpy(test_cont->words[4],";");
   TEST_ASSERT_TRUE(run_set(test_cont));
   TEST_ASSERT_EQUAL_DOUBLE(*test_cont->var_array[25],90.5);
   free_word_cont(test_cont);


   test_cont=init_word_cont();
   strcpy(test_cont->words[0],"SET");
   strcpy(test_cont->words[1],"Z");
   strcpy(test_cont->words[2],":=");
   strcpy(test_cont->words[3],"-90.5");
   strcpy(test_cont->words[4],";");
   TEST_ASSERT_TRUE(run_set(test_cont));
   TEST_ASSERT_EQUAL_DOUBLE(*test_cont->var_array[25],-90.5);
   free_word_cont(test_cont);

   test_cont=init_word_cont();
   strcpy(test_cont->words[0],"SET");
   strcpy(test_cont->words[1],"Z");
   strcpy(test_cont->words[2],":=");
   strcpy(test_cont->words[3],"-90.5");
   strcpy(test_cont->words[4],"10");
   strcpy(test_cont->words[5],"+");
   strcpy(test_cont->words[6],";");
   TEST_ASSERT_TRUE(run_set(test_cont));
   TEST_ASSERT_EQUAL_DOUBLE(*test_cont->var_array[25],-80.5);
   free_word_cont(test_cont);

   test_cont=init_word_cont();
   strcpy(test_cont->words[0],"SET");
   strcpy(test_cont->words[1],"Z");
   strcpy(test_cont->words[2],":=");
   strcpy(test_cont->words[3],"90.5");
   strcpy(test_cont->words[4],"2");
   strcpy(test_cont->words[5],"/");
   TEST_ASSERT_TRUE(!run_set(test_cont));
   free_word_cont(test_cont);

   test_cont=init_word_cont();
   strcpy(test_cont->words[0],"SET");
   strcpy(test_cont->words[1],"Z");
   strcpy(test_cont->words[2],":=");
   strcpy(test_cont->words[3],"90.5");
   strcpy(test_cont->words[4],"/");
   strcpy(test_cont->words[5],";");
   TEST_ASSERT_TRUE(!run_set(test_cont));
   free_word_cont(test_cont);

   test_cont=init_word_cont();
   strcpy(test_cont->words[0],"SET");
   strcpy(test_cont->words[1],"a");
   strcpy(test_cont->words[2],":=");
   strcpy(test_cont->words[3],"90.5");
   TEST_ASSERT_TRUE(!run_set(test_cont));
   free_word_cont(test_cont);

   test_cont=init_word_cont();
   strcpy(test_cont->words[0],"SET");
   strcpy(test_cont->words[1],"A");
   strcpy(test_cont->words[2],":");
   strcpy(test_cont->words[3],"90.5");
   TEST_ASSERT_TRUE(!run_set(test_cont));
   free_word_cont(test_cont);

   /*test other funcs using variables especially polish*/
   test_cont=init_word_cont();
   strcpy(test_cont->words[0],"SET");
   strcpy(test_cont->words[1],"Z");
   strcpy(test_cont->words[2],":=");
   strcpy(test_cont->words[3],"90.5");
   strcpy(test_cont->words[4],";");
   strcpy(test_cont->words[5],"Z");
   TEST_ASSERT_TRUE(run_set(test_cont));
   TEST_ASSERT_TRUE(get_var(test_cont,&test_double));
   TEST_ASSERT_EQUAL_DOUBLE(test_double,90.5);
   free_word_cont(test_cont);

   test_cont=init_word_cont();
   strcpy(test_cont->words[0],"SET");
   strcpy(test_cont->words[1],"Z");
   strcpy(test_cont->words[2],":=");
   strcpy(test_cont->words[3],"90.5");
   strcpy(test_cont->words[4],";");
   strcpy(test_cont->words[5],"z");
   TEST_ASSERT_TRUE(run_set(test_cont));
   TEST_ASSERT_TRUE(!get_var(test_cont,&test_double));
   free_word_cont(test_cont);

   test_cont=init_word_cont();
   strcpy(test_cont->words[0],"SET");
   strcpy(test_cont->words[1],"Z");
   strcpy(test_cont->words[2],":=");
   strcpy(test_cont->words[3],"90.5");
   strcpy(test_cont->words[4],";");
   strcpy(test_cont->words[5],"A");
   TEST_ASSERT_TRUE(run_set(test_cont));
   TEST_ASSERT_TRUE(!get_var(test_cont,&test_double));
   free_word_cont(test_cont);

   test_cont=init_word_cont();
   strcpy(test_cont->words[0],"SET");
   strcpy(test_cont->words[1],"Z");
   strcpy(test_cont->words[2],":=");
   strcpy(test_cont->words[3],"90.5");
   strcpy(test_cont->words[4],";");
   strcpy(test_cont->words[5],"Z");
   TEST_ASSERT_TRUE(run_set(test_cont));
   TEST_ASSERT_TRUE(get_varnum(test_cont,&test_double));
   TEST_ASSERT_TRUE(test_cont->position==6);
   TEST_ASSERT_EQUAL_DOUBLE(test_double,90.5);
   free_word_cont(test_cont);

   test_cont=init_word_cont();
   strcpy(test_cont->words[0],"SET");
   strcpy(test_cont->words[1],"Z");
   strcpy(test_cont->words[2],":=");
   strcpy(test_cont->words[3],"90.5");
   strcpy(test_cont->words[4],";");
   strcpy(test_cont->words[5],"90.5");
   TEST_ASSERT_TRUE(run_set(test_cont));
   TEST_ASSERT_TRUE(get_varnum(test_cont,&test_double));
   TEST_ASSERT_TRUE(test_cont->position==6);
   TEST_ASSERT_EQUAL_DOUBLE(test_double,90.5);
   free_word_cont(test_cont);

   test_cont=init_word_cont();
   strcpy(test_cont->words[0],"90.5");
   TEST_ASSERT_TRUE(get_varnum(test_cont,&test_double));
   TEST_ASSERT_EQUAL_DOUBLE(test_double,90.5);
   TEST_ASSERT_TRUE(test_cont->position==1);
   free_word_cont(test_cont);
}

void test_instruct_vars(void)
{
   word_cont* test_cont;
   line_cont* test_line_cont;
   double test_double;
   test_line_cont=init_line_cont();

   test_cont=init_word_cont();
   strcpy(test_cont->words[0],"SET");
   strcpy(test_cont->words[1],"Z");
   strcpy(test_cont->words[2],":=");
   strcpy(test_cont->words[3],"90");
   strcpy(test_cont->words[4],";");
   strcpy(test_cont->words[5],"LT");
   strcpy(test_cont->words[6],"Z");
   strcpy(test_cont->words[7],"FD");
   strcpy(test_cont->words[8],"30");
   strcpy(test_cont->words[9],"}");
   TEST_ASSERT_TRUE(run_instruction_list(test_cont,test_line_cont));
   TEST_ASSERT_TRUE(test_cont->position==10);
   TEST_ASSERT_TRUE(test_line_cont->size==1);
   TEST_ASSERT_DOUBLE_WITHIN(EPSILON,test_line_cont->array[0]->end->y,0);
   TEST_ASSERT_EQUAL_DOUBLE(test_line_cont->array[0]->end->x,-30);
   TEST_ASSERT_DOUBLE_WITHIN(EPSILON,test_line_cont->pending_line->start->y,0);
   TEST_ASSERT_EQUAL_DOUBLE(test_line_cont->pending_line->start->x,-30);

   free_line_cont(test_line_cont);
   free_word_cont(test_cont);



   test_line_cont=init_line_cont();

   test_cont=init_word_cont();
   strcpy(test_cont->words[0],"SET");
   strcpy(test_cont->words[1],"Z");
   strcpy(test_cont->words[2],":=");
   strcpy(test_cont->words[3],"90");
   strcpy(test_cont->words[4],";");
   strcpy(test_cont->words[5],"SET");
   strcpy(test_cont->words[6],"A");
   strcpy(test_cont->words[7],":=");
   strcpy(test_cont->words[8],"30");
   strcpy(test_cont->words[9],";");
   strcpy(test_cont->words[10],"LT");
   strcpy(test_cont->words[11],"Z");
   strcpy(test_cont->words[12],"FD");
   strcpy(test_cont->words[13],"A");
   strcpy(test_cont->words[14],"}");
   TEST_ASSERT_TRUE(run_instruction_list(test_cont,test_line_cont));
   TEST_ASSERT_TRUE(test_cont->position==15);
   TEST_ASSERT_TRUE(test_line_cont->size==1);
   TEST_ASSERT_DOUBLE_WITHIN(EPSILON,test_line_cont->array[0]->end->y,0);
   TEST_ASSERT_EQUAL_DOUBLE(test_line_cont->array[0]->end->x,-30);
   TEST_ASSERT_DOUBLE_WITHIN(EPSILON,test_line_cont->pending_line->start->y,0);
   TEST_ASSERT_EQUAL_DOUBLE(test_line_cont->pending_line->start->x,-30);

   free_line_cont(test_line_cont);
   free_word_cont(test_cont);


   test_cont=init_word_cont();
   strcpy(test_cont->words[0],"SET");
   strcpy(test_cont->words[1],"Z");
   strcpy(test_cont->words[2],":=");
   strcpy(test_cont->words[3],"90");
   strcpy(test_cont->words[4],";");

   strcpy(test_cont->words[5],"SET");
   strcpy(test_cont->words[6],"A");
   strcpy(test_cont->words[7],":=");
   strcpy(test_cont->words[8],"40");
   strcpy(test_cont->words[9],";");

   strcpy(test_cont->words[10],"SET");
   strcpy(test_cont->words[11],"G");
   strcpy(test_cont->words[12],":=");
   strcpy(test_cont->words[13],"19.3");
   strcpy(test_cont->words[14],";");
   /*can use a mix of nums and vars as well*/
   strcpy(test_cont->words[15],"Z");
   strcpy(test_cont->words[16],"90");
   strcpy(test_cont->words[17],"+");
   strcpy(test_cont->words[18],"A");
   strcpy(test_cont->words[19],"/");
   strcpy(test_cont->words[20],"G");
   strcpy(test_cont->words[21],"-");
   strcpy(test_cont->words[22],";");
   TEST_ASSERT_TRUE(run_set(test_cont));
   TEST_ASSERT_TRUE(run_set(test_cont));
   TEST_ASSERT_TRUE(run_set(test_cont));
   TEST_ASSERT_TRUE(run_polish(test_cont,&test_double));
   TEST_ASSERT_EQUAL_DOUBLE(test_double,-14.8);
   free_word_cont(test_cont);

}


void test_do(void)
{
   word_cont* test_cont;
   line_cont* test_line_cont;
   double test_double , test_double_2;
   int i;
   test_cont=init_word_cont();

   test_line_cont=init_line_cont();
   strcpy(test_cont->words[0],"DO");
   strcpy(test_cont->words[1],"A");
   strcpy(test_cont->words[2],"FROM");
   strcpy(test_cont->words[3],"3");
   strcpy(test_cont->words[4],"TO");
   strcpy(test_cont->words[5],"9");
   strcpy(test_cont->words[6],"{");
   strcpy(test_cont->words[7],"FD");
   strcpy(test_cont->words[8],"1");
   strcpy(test_cont->words[9],"}");

   TEST_ASSERT_TRUE(do_helper(test_cont,&i, &test_double,&test_double_2));
   TEST_ASSERT_TRUE(i==0);
   TEST_ASSERT_EQUAL_DOUBLE(test_double,3);
   TEST_ASSERT_EQUAL_DOUBLE(test_double_2,9);
   TEST_ASSERT_TRUE(test_cont->position==7);
   test_cont->position=0;
   TEST_ASSERT_TRUE(run_do(test_cont,test_line_cont));
   TEST_ASSERT_TRUE(test_line_cont->size==7);

   TEST_ASSERT_TRUE(test_cont->position==10);


   free_line_cont(test_line_cont);
   free_word_cont(test_cont);



   /*set A then use it in a loop with itself*/
   test_cont=init_word_cont();

   test_line_cont=init_line_cont();
   strcpy(test_cont->words[0],"SET");
   strcpy(test_cont->words[1],"A");
   strcpy(test_cont->words[2],":=");
   strcpy(test_cont->words[3],"3");
   strcpy(test_cont->words[4],";");

   strcpy(test_cont->words[5],"DO");
   strcpy(test_cont->words[6],"A");
   strcpy(test_cont->words[7],"FROM");
   strcpy(test_cont->words[8],"A");
   strcpy(test_cont->words[9],"TO");
   strcpy(test_cont->words[10],"9");
   strcpy(test_cont->words[11],"{");
   strcpy(test_cont->words[12],"FD");
   strcpy(test_cont->words[13],"1");
   strcpy(test_cont->words[14],"}");

   TEST_ASSERT_TRUE(run_set(test_cont));
   TEST_ASSERT_TRUE(run_do(test_cont,test_line_cont));
   TEST_ASSERT_TRUE(test_line_cont->size==7);
   TEST_ASSERT_TRUE(test_cont->position==15);


   free_line_cont(test_line_cont);
   free_word_cont(test_cont);

   /*change the var in the loop*/
   test_cont=init_word_cont();

   test_line_cont=init_line_cont();
   strcpy(test_cont->words[0],"DO");
   strcpy(test_cont->words[1],"A");
   strcpy(test_cont->words[2],"FROM");
   strcpy(test_cont->words[3],"3");
   strcpy(test_cont->words[4],"TO");
   strcpy(test_cont->words[5],"9");
   strcpy(test_cont->words[6],"{");
   strcpy(test_cont->words[7],"SET");
   strcpy(test_cont->words[8],"A");
   strcpy(test_cont->words[9],":=");
   strcpy(test_cont->words[10],"10");
   strcpy(test_cont->words[11],";");
   strcpy(test_cont->words[12],"FD");
   strcpy(test_cont->words[13],"1");
   strcpy(test_cont->words[14],"}");
   TEST_ASSERT_TRUE(run_do(test_cont,test_line_cont));
   TEST_ASSERT_TRUE(test_line_cont->size==1);
   TEST_ASSERT_TRUE(test_cont->position==15);


   free_line_cont(test_line_cont);
   free_word_cont(test_cont);

   test_cont=init_word_cont();

   test_line_cont=init_line_cont();
   strcpy(test_cont->words[0],"DO");
   strcpy(test_cont->words[1],"A");
   strcpy(test_cont->words[2],"FROM");
   strcpy(test_cont->words[3],"9");
   strcpy(test_cont->words[4],"TO");
   strcpy(test_cont->words[5],"3");
   strcpy(test_cont->words[6],"{");
   strcpy(test_cont->words[7],"FD");
   strcpy(test_cont->words[8],"1");
   strcpy(test_cont->words[9],"}");
   TEST_ASSERT_TRUE(run_do(test_cont,test_line_cont));
   TEST_ASSERT_TRUE(test_line_cont->size==0);


   free_line_cont(test_line_cont);
   free_word_cont(test_cont);


   test_cont=init_word_cont();

   test_line_cont=init_line_cont();
   strcpy(test_cont->words[0],"DO");
   strcpy(test_cont->words[1],"A");
   strcpy(test_cont->words[2],"FROM");
   strcpy(test_cont->words[3],"-1");
   strcpy(test_cont->words[4],"TO");
   strcpy(test_cont->words[5],"3");
   strcpy(test_cont->words[6],"{");
   strcpy(test_cont->words[7],"FD");
   strcpy(test_cont->words[8],"1");
   strcpy(test_cont->words[9],"}");
   TEST_ASSERT_TRUE(run_do(test_cont,test_line_cont));
   TEST_ASSERT_TRUE(test_line_cont->size==5);


   free_line_cont(test_line_cont);
   free_word_cont(test_cont);

   test_cont=init_word_cont();

   test_line_cont=init_line_cont();
   strcpy(test_cont->words[0],"DO");
   strcpy(test_cont->words[1],"A");
   strcpy(test_cont->words[2],"FROM");
   strcpy(test_cont->words[3],"3");
   strcpy(test_cont->words[4],"TO");
   strcpy(test_cont->words[5],"9");
   strcpy(test_cont->words[6],"{");
   strcpy(test_cont->words[7],"FD");
   strcpy(test_cont->words[8],"F");
   strcpy(test_cont->words[9],"}");
   TEST_ASSERT_TRUE(!run_do(test_cont,test_line_cont));

   free_line_cont(test_line_cont);
   free_word_cont(test_cont);

   /*even if loop never happens we
   want to spot syntax errors*/
   test_cont=init_word_cont();
   test_line_cont=init_line_cont();
   strcpy(test_cont->words[0],"DO");
   strcpy(test_cont->words[1],"A");
   strcpy(test_cont->words[2],"FROM");
   strcpy(test_cont->words[3],"9");
   strcpy(test_cont->words[4],"TO");
   strcpy(test_cont->words[5],"3");
   strcpy(test_cont->words[6],"{");
   strcpy(test_cont->words[7],"FD");
   strcpy(test_cont->words[8],"ffkkmd");
   strcpy(test_cont->words[9],"}");
   TEST_ASSERT_TRUE(!run_do(test_cont,test_line_cont));

   free_line_cont(test_line_cont);
   free_word_cont(test_cont);


   test_cont=init_word_cont();

   test_line_cont=init_line_cont();
   strcpy(test_cont->words[0],"DER");
   strcpy(test_cont->words[1],"A");
   strcpy(test_cont->words[2],"FROM");
   strcpy(test_cont->words[3],"9");
   strcpy(test_cont->words[4],"TO");
   strcpy(test_cont->words[5],"3");
   strcpy(test_cont->words[6],"{");
   strcpy(test_cont->words[7],"FD");
   strcpy(test_cont->words[8],"1");
   strcpy(test_cont->words[9],"}");
   TEST_ASSERT_TRUE(!run_do(test_cont,test_line_cont));

   free_line_cont(test_line_cont);
   free_word_cont(test_cont);


   test_cont=init_word_cont();

   test_line_cont=init_line_cont();
   strcpy(test_cont->words[0],"DO");
   strcpy(test_cont->words[1],"A");
   strcpy(test_cont->words[2],"FROM");
   strcpy(test_cont->words[3],"9");
   strcpy(test_cont->words[4],"TO");
   strcpy(test_cont->words[5],"3");
   strcpy(test_cont->words[6],"{");
   strcpy(test_cont->words[7],"FD");
   strcpy(test_cont->words[8],"1");
   strcpy(test_cont->words[9],"");
   TEST_ASSERT_TRUE(!run_do(test_cont,test_line_cont));

   free_line_cont(test_line_cont);
   free_word_cont(test_cont);

   /*loop within a loop*/
   test_cont=init_word_cont();

   test_line_cont=init_line_cont();
   strcpy(test_cont->words[0],"DO");
   strcpy(test_cont->words[1],"A");
   strcpy(test_cont->words[2],"FROM");
   strcpy(test_cont->words[3],"3");
   strcpy(test_cont->words[4],"TO");
   strcpy(test_cont->words[5],"9");
   strcpy(test_cont->words[6],"{");
   strcpy(test_cont->words[7],"DO");
   strcpy(test_cont->words[8],"B");
   strcpy(test_cont->words[9],"FROM");
   strcpy(test_cont->words[10],"1");
   strcpy(test_cont->words[11],"TO");
   strcpy(test_cont->words[12],"2");
   strcpy(test_cont->words[13],"{");
   strcpy(test_cont->words[14],"FD");
   strcpy(test_cont->words[15],"1");
   strcpy(test_cont->words[16],"}");
   strcpy(test_cont->words[17],"}");
   TEST_ASSERT_TRUE(run_do(test_cont,test_line_cont));
   TEST_ASSERT_TRUE(test_line_cont->size==14);
   TEST_ASSERT_TRUE(test_cont->position==18);
   TEST_ASSERT_EQUAL_DOUBLE(test_line_cont->array[13]->end->y,14);

   free_line_cont(test_line_cont);
   free_word_cont(test_cont);
}


void test_main_full(void)
{
   word_cont* test_cont;
   line_cont* test_line_cont;

   test_cont=init_word_cont();

   test_line_cont=init_line_cont();
   strcpy(test_cont->words[0],"{");
   strcpy(test_cont->words[1],"DO");
   strcpy(test_cont->words[2],"A");
   strcpy(test_cont->words[3],"FROM");
   strcpy(test_cont->words[4],"3");
   strcpy(test_cont->words[5],"TO");
   strcpy(test_cont->words[6],"9");
   strcpy(test_cont->words[7],"{");
   strcpy(test_cont->words[8],"DO");
   strcpy(test_cont->words[9],"B");
   strcpy(test_cont->words[10],"FROM");
   strcpy(test_cont->words[11],"1");
   strcpy(test_cont->words[12],"TO");
   strcpy(test_cont->words[13],"2");
   strcpy(test_cont->words[14],"{");
   strcpy(test_cont->words[15],"FD");
   strcpy(test_cont->words[16],"1");
   strcpy(test_cont->words[17],"}");
   strcpy(test_cont->words[18],"}");
   strcpy(test_cont->words[19],"}");

   TEST_ASSERT_TRUE(run_main(test_cont,test_line_cont));
   TEST_ASSERT_TRUE(test_cont->position==20);
   TEST_ASSERT_TRUE(test_line_cont->size==14);
   TEST_ASSERT_EQUAL_DOUBLE(test_line_cont->array[13]->end->y,14);


   free_line_cont(test_line_cont);
   free_word_cont(test_cont);

   test_cont=init_word_cont();
   test_line_cont=init_line_cont();

   strcpy(test_cont->words[0],"{");
   strcpy(test_cont->words[1],"DO");
   strcpy(test_cont->words[2],"A");
   strcpy(test_cont->words[3],"FROM");
   strcpy(test_cont->words[4],"1");
   strcpy(test_cont->words[5],"TO");
   strcpy(test_cont->words[6],"8");
   strcpy(test_cont->words[7],"{");
   strcpy(test_cont->words[8],"FD");
   strcpy(test_cont->words[9],"30");
   strcpy(test_cont->words[10],"LT");
   strcpy(test_cont->words[11],"45");
   strcpy(test_cont->words[12],"}");
   strcpy(test_cont->words[13],"}");


   TEST_ASSERT_TRUE(run_main(test_cont,test_line_cont));
   TEST_ASSERT_TRUE(test_cont->position==14);
   TEST_ASSERT_TRUE(test_line_cont->size==8);
   TEST_ASSERT_DOUBLE_WITHIN(EPSILON,test_line_cont->array[7]->end->x,0);
   TEST_ASSERT_DOUBLE_WITHIN(EPSILON,test_line_cont->array[7]->end->y,0);

   free_line_cont(test_line_cont);
   free_word_cont(test_cont);


   test_cont=init_word_cont();
   test_line_cont=init_line_cont();

   strcpy(test_cont->words[0],"{");
   strcpy(test_cont->words[1],"DO");
   strcpy(test_cont->words[2],"A");
   strcpy(test_cont->words[3],"FROM");
   strcpy(test_cont->words[4],"1");
   strcpy(test_cont->words[5],"TO");
   strcpy(test_cont->words[6],"50");
   strcpy(test_cont->words[7],"{");
   strcpy(test_cont->words[8],"FD");
   strcpy(test_cont->words[9],"A");
   strcpy(test_cont->words[10],"RT");
   strcpy(test_cont->words[11],"30");
   strcpy(test_cont->words[12],"DO");
   strcpy(test_cont->words[13],"B");
   strcpy(test_cont->words[14],"FROM");
   strcpy(test_cont->words[15],"1");
   strcpy(test_cont->words[16],"TO");
   strcpy(test_cont->words[17],"8");
   strcpy(test_cont->words[18],"{");

   strcpy(test_cont->words[19],"SET");
   strcpy(test_cont->words[20],"C");
   strcpy(test_cont->words[21],":=");
   strcpy(test_cont->words[22],"A");
   strcpy(test_cont->words[23],"5");
   strcpy(test_cont->words[24],"/");
   strcpy(test_cont->words[25],";");
   strcpy(test_cont->words[26],"FD");
   strcpy(test_cont->words[27],"C");
   strcpy(test_cont->words[28],"RT");
   strcpy(test_cont->words[29],"45");
   strcpy(test_cont->words[30],"}");
   strcpy(test_cont->words[31],"}");
   strcpy(test_cont->words[32],"}");

   TEST_ASSERT_TRUE(run_main(test_cont,test_line_cont));
   TEST_ASSERT_TRUE(test_line_cont->size==450);


   free_line_cont(test_line_cont);
   free_word_cont(test_cont);




   test_cont=read_in_file("test_files/test_turtles/parser/valid/adding_loops.ttl");

   test_line_cont=init_line_cont();
   TEST_ASSERT_TRUE(run_main(test_cont,test_line_cont));
   TEST_ASSERT_TRUE(test_line_cont->size==8);


   free_line_cont(test_line_cont);
   free_word_cont(test_cont);

   test_cont=read_in_file("test_files/test_turtles/parser/valid/nested_loops.ttl");
   test_line_cont=init_line_cont();

   TEST_ASSERT_TRUE(run_main(test_cont,test_line_cont));
   TEST_ASSERT_TRUE(test_line_cont->size==450);
   free_line_cont(test_line_cont);
   free_word_cont(test_cont);


   test_cont=read_in_file("test_files/test_turtles/parser/valid/polish.ttl");
   test_line_cont=init_line_cont();
   TEST_ASSERT_TRUE(!run_main(test_cont,test_line_cont));
   free_line_cont(test_line_cont);
   free_word_cont(test_cont);


   test_cont=read_in_file("test_files/test_turtles/parser/valid/polish_2.ttl");
   test_line_cont=init_line_cont();
   TEST_ASSERT_TRUE(!run_main(test_cont,test_line_cont));
   TEST_ASSERT_TRUE(strcmp("potentially haven't set variable before calling it",test_cont->err_message)==0);
   free_line_cont(test_line_cont);
   free_word_cont(test_cont);


   test_cont=read_in_file("test_files/test_turtles/interpreter/invalid/too_many_nums.ttl");
   test_line_cont=init_line_cont();
   TEST_ASSERT_TRUE(!run_main(test_cont,test_line_cont));
   TEST_ASSERT_TRUE(strcmp("more than one number left on stack at end of expr",test_cont->err_message)==0);
   free_line_cont(test_line_cont);
   free_word_cont(test_cont);


   test_cont=read_in_file("test_files/test_turtles/interpreter/invalid/no_nums.ttl");
   test_line_cont=init_line_cont();
   TEST_ASSERT_TRUE(!run_main(test_cont,test_line_cont));
   TEST_ASSERT_TRUE(strcmp("no numbers on the stack at end of expr",test_cont->err_message)==0);
   free_line_cont(test_line_cont);
   free_word_cont(test_cont);
}


int main(void)
{
   UNITY_BEGIN();
   RUN_TEST(test_coord_funcs);
   RUN_TEST(test_get_num);
   RUN_TEST(test_directions);
   RUN_TEST(test_line_cont);
   RUN_TEST(test_move_forward);
   RUN_TEST(test_run_instruct_basic);
   RUN_TEST(test_basic_instruct_list);
   RUN_TEST(test_basic_main);
   RUN_TEST(test_polish);
   RUN_TEST(test_set);
   RUN_TEST(test_instruct_vars);
   RUN_TEST(test_do);
   RUN_TEST(test_main_full);
   return UNITY_END();
}






/*pretty much just for testing so we dont
deal with files until end*/
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
