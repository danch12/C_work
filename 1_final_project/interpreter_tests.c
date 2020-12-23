#include "interpreter_funcs.h"

#define MAXTESTCAP 100
#define MAXTESTLEN 50
#define EPSILON 0.000001
word_cont* init_word_cont(void);
bool compare_doubles(double d_1, double d_2);

int main(void)
{
   coord* test_coord,*test_coord2, *origin;
   int i;
   word_cont* test_cont;
   double test_double, test_double_2;
   line* test_line;
   line_cont* test_line_cont;

   origin=init_coords(0,0);
   test_coord=init_coords(4,3);
   assert(compare_doubles(test_coord->x,4));
   assert(compare_doubles(test_coord->y,3));

   assert(rotate(180,test_coord,origin));

   assert(compare_doubles(test_coord->x,-4));
   assert(compare_doubles(test_coord->y,-3));

   rotate(180,test_coord,origin);
   assert(compare_doubles(test_coord->x,4));
   assert(compare_doubles(test_coord->y,3));

   rotate(90,test_coord,origin);
   assert(compare_doubles(test_coord->x,-3));
   assert(compare_doubles(test_coord->y,4));

   rotate(270,test_coord,origin);
   assert(compare_doubles(test_coord->x,4));
   assert(compare_doubles(test_coord->y,3));

   free(test_coord);

   test_coord=init_coords(3,-4);

   assert(rotate(29.5, test_coord,origin));
   assert(compare_doubles(test_coord->x,4.580761));
   assert(compare_doubles(test_coord->y,-2.004152));
   free(test_coord);
   assert(!rotate(29.5, NULL,origin));


   test_coord=init_coords(5,6);

   for(i=0;i<8;i++)
   {
      rotate(45,test_coord,origin);
   }
   assert(compare_doubles(test_coord->x,5));
   assert(compare_doubles(test_coord->y,6));
   free(test_coord);


   test_coord2=init_coords(11,1.2);
   test_coord=init_coords(11,1.2);

   assert(rotate(10, test_coord,origin));
   assert(rotate(10, test_coord,origin));

   assert(rotate(20, test_coord2,origin));


   assert(compare_doubles(test_coord->x,test_coord2->x));
   assert(compare_doubles(test_coord->y,test_coord2->y));


   assert(rotate(0.5, test_coord,origin));
   assert(rotate(0.5, test_coord,origin));
   assert(rotate(1, test_coord2,origin));
   assert(compare_doubles(test_coord->x,test_coord2->x));
   assert(compare_doubles(test_coord->y,test_coord2->y));


   free(test_coord);
   free(test_coord2);
   free(origin);

   origin=init_coords(10,10);
   test_coord=init_coords(4,3);

   assert(rotate(180,test_coord,origin));
   assert(compare_doubles(test_coord->x,16));
   assert(compare_doubles(test_coord->y,17));


   assert(rotate(180,test_coord,origin));

   assert(rotate(270,test_coord,origin));

   assert(compare_doubles(test_coord->x,3));
   assert(compare_doubles(test_coord->y,16));
   free(test_coord);
   free(origin);

   test_cont=init_word_cont();
   strcpy(test_cont->words[0],"123.4");
   assert(get_num(test_cont,&test_double));
   assert(test_cont->position==1);
   assert(test_double -123.4 <EPSILON);
   assert(compare_doubles(test_double,123.4));

   strcpy(test_cont->words[0],"123.b");
   test_cont->position=0;

   assert(!get_num(test_cont,&test_double));
   assert(!get_num(NULL,&test_double));
   free_word_cont(test_cont);

   test_cont=init_word_cont();
   strcpy(test_cont->words[0],"LT");
   strcpy(test_cont->words[1],"123.4");

   assert(direction_helper(test_cont)==left);

   strcpy(test_cont->words[0],"RT");
   test_cont->position=0;
   assert(direction_helper(test_cont)==right);

   strcpy(test_cont->words[0],"inv");
   test_cont->position=0;
   assert(direction_helper(test_cont)==invalid);
   /*position doesnt change when invalid*/
   assert(test_cont->position==0);


   test_cont->position=0;
   strcpy(test_cont->words[0],"LT");
   test_line=init_origin();

   assert(compare_doubles(test_line->rotation,0));
   assert(compare_doubles(test_line->start->x,0));
   assert(compare_doubles(test_line->start->y,0));


   get_rotation(test_cont,test_line);

   assert(compare_doubles(test_line->rotation,123.4));
   assert(test_cont->position==2);

   strcpy(test_cont->words[2],"LT");
   strcpy(test_cont->words[3],"500");
   get_rotation(test_cont,test_line);

   assert(compare_doubles(test_line->rotation,263.4));

   strcpy(test_cont->words[4],"LT");
   strcpy(test_cont->words[5],"50");
   get_rotation(test_cont,test_line);
   assert(compare_doubles(test_line->rotation,313.4));


   strcpy(test_cont->words[6],"RT");
   strcpy(test_cont->words[7],"50");
   get_rotation(test_cont,test_line);
   assert(compare_doubles(test_line->rotation,263.4));

   strcpy(test_cont->words[8],"RT");
   strcpy(test_cont->words[9],"500");
   get_rotation(test_cont,test_line);

   assert(compare_doubles(test_line->rotation,123.4));

   strcpy(test_cont->words[10],"RT");
   strcpy(test_cont->words[11],"123.4");
   get_rotation(test_cont,test_line);

   assert(compare_doubles(test_line->rotation,0));

   free_word_cont(test_cont);
   free_line(test_line);


   /*test line cont funcs*/

   test_line_cont=init_line_cont();
   assert(test_line_cont->size==0);
   assert(test_line_cont->capacity==INITSIZE);

   /*should have no effect*/
   store_line(test_line_cont,NULL);
   assert(test_line_cont->size==0);

   for(i=0;i<10000;i++)
   {
      test_line=init_origin();
      assert(test_line_cont->size==i);
      store_line(test_line_cont,test_line);

   }
   free_line_cont(test_line_cont);

   /*************test move forward**************/

   test_line_cont=init_line_cont();
   test_cont=init_word_cont();
   test_line=init_origin();
   strcpy(test_cont->words[0],"FD");
   strcpy(test_cont->words[1],"30");
   assert(move_forward(test_cont,test_line,test_line_cont));
   assert(test_line->start->y-30<EPSILON);
   assert(test_line->start->x<EPSILON);

   assert(compare_doubles(test_line_cont->array[0]->start->y,0));
   assert(compare_doubles(test_line_cont->array[0]->start->x,0));
   free_line(test_line);

   test_line=init_origin();
   test_cont->position=0;
   strcpy(test_cont->words[0],"LT");
   strcpy(test_cont->words[1],"90");
   strcpy(test_cont->words[2],"FD");
   strcpy(test_cont->words[3],"30");
   assert(get_rotation(test_cont,test_line));
   assert(move_forward(test_cont,test_line,test_line_cont));
   assert(compare_doubles(test_line_cont->array[1]->end->y,0));
   assert(compare_doubles(test_line_cont->array[1]->end->x,-30));
   assert(compare_doubles(test_line->start->y,0));
   assert(compare_doubles(test_line->start->x,-30));
   free_line_cont(test_line_cont);
   free_line(test_line);

   test_line=init_origin();
   test_line_cont=init_line_cont();
   test_cont->position=0;
   strcpy(test_cont->words[0],"RT");
   strcpy(test_cont->words[1],"90");
   strcpy(test_cont->words[2],"FD");
   strcpy(test_cont->words[3],"30");
   assert(get_rotation(test_cont,test_line));
   assert(move_forward(test_cont,test_line,test_line_cont));
   assert(compare_doubles(test_line_cont->array[0]->end->y,0));
   assert(compare_doubles(test_line_cont->array[0]->end->x,30));
   assert(compare_doubles(test_line->start->y,0));
   assert(compare_doubles(test_line->start->x,30));
   assert(compare_doubles(test_line->rotation,270));

   free_line_cont(test_line_cont);
   free_line(test_line);

   test_cont->position=0;
   test_line_cont=init_line_cont();
   for(i=0;i<8;i++)
   {
      strcpy(test_cont->words[i*4],"FD");
      strcpy(test_cont->words[(i*4)+1],"30");
      strcpy(test_cont->words[(i*4)+2],"LT");
      strcpy(test_cont->words[(i*4)+3],"45");
   }

   test_line=init_origin();
   for(i=0;i<8;i++)
   {

      assert(move_forward(test_cont,test_line,test_line_cont));
      assert(get_rotation(test_cont,test_line));
   }

   /*back at origin*/
   assert(compare_doubles(test_line->start->y,0));
   assert(compare_doubles(test_line->start->x,0));
   assert(compare_doubles(test_line->rotation,0));
   assert(compare_doubles(test_line_cont->array[4]->rotation,180));
   assert(test_cont->position==(8*4));

   assert(free_line_cont(test_line_cont));
   assert(free_line(test_line));

   test_cont->position=0;
   test_line_cont=init_line_cont();
   for(i=0;i<8;i++)
   {
      strcpy(test_cont->words[i*4],"FD");
      strcpy(test_cont->words[(i*4)+1],"30");
      strcpy(test_cont->words[(i*4)+2],"RT");
      strcpy(test_cont->words[(i*4)+3],"45");
   }

   test_line=init_origin();
   for(i=0;i<8;i++)
   {

      assert(move_forward(test_cont,test_line,test_line_cont));
      assert(get_rotation(test_cont,test_line));
   }

   /*back at origin*/
   assert(compare_doubles(test_line->start->y,0));
   assert(compare_doubles(test_line->start->x,0));
   assert(compare_doubles(test_line->rotation,0));
   assert(compare_doubles(test_line_cont->array[4]->rotation,180));
   assert(test_cont->position==(8*4));

   test_cont->position=0;
   strcpy(test_cont->words[0],"NO");

   assert(!move_forward(test_cont,test_line,test_line_cont));
   assert(free_line(test_line));
   assert(free_line(NULL));
   assert(free_line_cont(test_line_cont));
   assert(free_line_cont(NULL));
   free_word_cont(test_cont);

   test_line_cont=init_line_cont();
   test_cont=init_word_cont();
   test_line=init_origin();
   strcpy(test_cont->words[0],"LT");
   strcpy(test_cont->words[1],"90");
   strcpy(test_cont->words[2],"FD");
   strcpy(test_cont->words[3],"30");

   /******* test run_instruction ***********/


   assert(run_instruction(test_cont,test_line_cont,test_line));
   assert(compare_doubles(test_line->rotation,90));
   assert(test_cont->position==2);
   assert(test_line_cont->size==0);

   assert(run_instruction(test_cont,test_line_cont,test_line));
   assert(test_cont->position==4);
   assert(test_line_cont->size==1);
   assert(compare_doubles(test_line_cont->array[0]->end->y,0));
   assert(compare_doubles(test_line_cont->array[0]->end->x,-30));
   assert(compare_doubles(test_line->start->y,0));
   assert(compare_doubles(test_line->start->x,-30));

   strcpy(test_cont->words[0],"L");
   test_cont->position=0;
   assert(!run_instruction(test_cont,test_line_cont,test_line));
   strcpy(test_cont->words[0],"LT");

   test_cont->position=0;
   strcpy(test_cont->words[1],"LT");
   assert(!run_instruction(test_cont,test_line_cont,test_line));
   strcpy(test_cont->words[1],"90");

   test_cont->position=0;
   strcpy(test_cont->words[0],"");
   assert(!run_instruction(test_cont,test_line_cont,test_line));
   strcpy(test_cont->words[0],"LT");

   free_line(test_line);
   free_line_cont(test_line_cont);
   free_word_cont(test_cont);


   /************* test run instruction list + run_main *********/
   test_line_cont=init_line_cont();
   test_line=init_origin();
   test_cont=init_word_cont();

   strcpy(test_cont->words[0],"LT");
   strcpy(test_cont->words[1],"90");
   strcpy(test_cont->words[2],"FD");
   strcpy(test_cont->words[3],"30");
   strcpy(test_cont->words[4],"}");
   assert(run_instruction_list(test_cont,test_line_cont,test_line));
   assert(test_cont->position==5);
   assert(test_line_cont->size==1);
   assert(compare_doubles(test_line_cont->array[0]->end->y,0));
   assert(compare_doubles(test_line_cont->array[0]->end->x,-30));
   assert(compare_doubles(test_line->start->y,0));
   assert(compare_doubles(test_line->start->x,-30));
   free_line(test_line);
   free_line_cont(test_line_cont);
   free_word_cont(test_cont);


   test_line_cont=init_line_cont();
   test_line=init_origin();
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
   assert(!run_main(test_cont,test_line_cont,test_line));
   assert(run_instruction_list(test_cont,test_line_cont,test_line));
   assert(test_cont->position==17);
   assert(test_line_cont->size==4);
   assert(compare_doubles(test_line->rotation,180));

   assert(compare_doubles(test_line_cont->array[0]->rotation,90));
   assert(compare_doubles(test_line_cont->array[1]->rotation,180));
   assert(compare_doubles(test_line_cont->array[2]->rotation,270));
   assert(compare_doubles(test_line_cont->array[3]->rotation,180));


   assert(compare_doubles(test_line_cont->array[0]->start->x,0));
   assert(compare_doubles(test_line_cont->array[0]->start->y,0));
   assert(compare_doubles(test_line_cont->array[0]->end->x,-30));
   assert(compare_doubles(test_line_cont->array[0]->end->y,0));

   assert(compare_doubles(test_line_cont->array[1]->start->x,-30));
   assert(compare_doubles(test_line_cont->array[1]->start->y,0));
   assert(compare_doubles(test_line_cont->array[1]->end->x,-30));
   assert(compare_doubles(test_line_cont->array[1]->end->y,-30));

   assert(compare_doubles(test_line_cont->array[2]->start->x,-30));
   assert(compare_doubles(test_line_cont->array[2]->start->y,-30));
   assert(compare_doubles(test_line_cont->array[2]->end->x,0));
   assert(compare_doubles(test_line_cont->array[2]->end->y,-30));


   assert(compare_doubles(test_line_cont->array[3]->start->x,0));
   assert(compare_doubles(test_line_cont->array[3]->start->y,-30));
   assert(compare_doubles(test_line_cont->array[3]->end->x,0));
   assert(compare_doubles(test_line_cont->array[3]->end->y,-60));


   test_cont->position=0;
   strcpy(test_cont->words[0],"NO");
   assert(!run_instruction_list(test_cont,test_line_cont,test_line));
   strcpy(test_cont->words[0],"LT");

   test_cont->position=0;
   strcpy(test_cont->words[1],"LT");
   assert(!run_instruction_list(test_cont,test_line_cont,test_line));
   strcpy(test_cont->words[1],"90");

   test_cont->position=0;
   strcpy(test_cont->words[16],"FD");
   assert(!run_instruction_list(test_cont,test_line_cont,test_line));
   strcpy(test_cont->words[16],"}");

   test_cont->position=0;
   strcpy(test_cont->words[11],"}");
   assert(!run_instruction_list(test_cont,test_line_cont,test_line));
   strcpy(test_cont->words[11],"30");

   test_cont->position=0;
   strcpy(test_cont->words[1],"");
   assert(!run_instruction_list(test_cont,test_line_cont,test_line));
   strcpy(test_cont->words[1],"90");

   test_cont->position=0;
   strcpy(test_cont->words[1],"aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa90");
   assert(!run_instruction_list(test_cont,test_line_cont,test_line));
   strcpy(test_cont->words[1],"90");

   free_line(test_line);
   free_line_cont(test_line_cont);

   test_cont->position=0;
   test_line_cont=init_line_cont();
   test_line=init_origin();

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
   assert(!run_instruction_list(test_cont,test_line_cont,test_line));
   assert(run_main(test_cont,test_line_cont,test_line));
   free_line(test_line);
   free_line_cont(test_line_cont);


   test_cont->position=0;
   test_line_cont=init_line_cont();
   test_line=init_origin();

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

   assert(run_main(test_cont,test_line_cont,test_line));

   assert(compare_doubles(test_line->rotation,357));
   free_line(test_line);
   free_line_cont(test_line_cont);
   free_word_cont(test_cont);



   /******** polish tests no variables ************/

   test_cont=init_word_cont();
   strcpy(test_cont->words[0],"+");
   assert(get_op(test_cont)==plus);
   strcpy(test_cont->words[0],"-");
   assert(get_op(test_cont)==minus);
   strcpy(test_cont->words[0],"*");
   assert(get_op(test_cont)==mult);
   strcpy(test_cont->words[0],"/");
   assert(get_op(test_cont)==divide);
   strcpy(test_cont->words[0],"29");
   assert(get_op(test_cont)==invalid_op);

   assert(polish_num(test_cont));
   assert(test_cont->position==1);
   assert(compare_doubles(test_cont->stackptr->start->num,29));
   assert(test_cont->stackptr->size==1);
   strcpy(test_cont->words[1],"90");
   assert(polish_num(test_cont));
   assert(compare_doubles(test_cont->stackptr->start->num,90));
   assert(compare_doubles(test_cont->stackptr->start->next->num,29));
   assert(test_cont->stackptr->size==2);

   strcpy(test_cont->words[2],"no");
   assert(!polish_num(test_cont));
   assert(test_cont->stackptr->size==2);

   strcpy(test_cont->words[2],"70.a");
   assert(!polish_num(test_cont));
   assert(test_cont->stackptr->size==2);

   strcpy(test_cont->words[2],"");
   assert(!polish_num(test_cont));
   assert(test_cont->stackptr->size==2);

   strcpy(test_cont->words[2],"+");
   assert(do_operation(test_cont));
   assert(compare_doubles(test_cont->stackptr->start->num,119));
   assert(!test_cont->stackptr->start->next);
   assert(test_cont->stackptr->size==1);
   assert(test_cont->position==3);

   strcpy(test_cont->words[3],"1.1");
   assert(polish_num(test_cont));
   strcpy(test_cont->words[4],"-");
   assert(do_operation(test_cont));
   assert(compare_doubles(test_cont->stackptr->start->num,117.9));

   strcpy(test_cont->words[5],"2");
   assert(polish_num(test_cont));
   strcpy(test_cont->words[6],"/");
   assert(do_operation(test_cont));
   assert(compare_doubles(test_cont->stackptr->start->num,58.95));
   strcpy(test_cont->words[7],"*");
   assert(!do_operation(test_cont));

   strcpy(test_cont->words[7],"56");
   assert(polish_num(test_cont));
   strcpy(test_cont->words[8],")");
   assert(!do_operation(test_cont));
   strcpy(test_cont->words[8],"");
   assert(!do_operation(test_cont));

   free_word_cont(test_cont);

   test_cont=init_word_cont();
   /*not enough numbers*/
   assert(!finish_polish(test_cont,&test_double));
   strcpy(test_cont->words[0],"90");
   assert(polish_num(test_cont));
   strcpy(test_cont->words[1],"90");
   assert(polish_num(test_cont));
   /*too many numbers*/
   assert(!finish_polish(test_cont,&test_double));
   free_word_cont(test_cont);

   test_cont=init_word_cont();
   strcpy(test_cont->words[0],"90");
   assert(polish_num(test_cont));
   strcpy(test_cont->words[1],"90");
   assert(polish_num(test_cont));
   strcpy(test_cont->words[2],"+");
   assert(do_operation(test_cont));
   /*goldilocks amount*/
   assert(finish_polish(test_cont,&test_double));
   assert(compare_doubles(test_double,180));
   free_word_cont(test_cont);


   test_cont=init_word_cont();
   strcpy(test_cont->words[0],"90");
   strcpy(test_cont->words[1],"90");
   strcpy(test_cont->words[2],"+");
   strcpy(test_cont->words[3],";");
   assert(run_polish(test_cont,&test_double));
   assert(compare_doubles(test_double,180));
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
   assert(run_polish(test_cont,&test_double));
   assert(compare_doubles(test_double,-14.8));
   free_word_cont(test_cont);

   test_cont=init_word_cont();
   strcpy(test_cont->words[0],"90");
   strcpy(test_cont->words[1],";");
   assert(run_polish(test_cont,&test_double));
   assert(compare_doubles(test_double,90));
   free_word_cont(test_cont);

   test_cont=init_word_cont();
   strcpy(test_cont->words[0],";");
   assert(!run_polish(test_cont,&test_double));
   assert(test_cont->position==0);

   strcpy(test_cont->words[0],"7");
   assert(!run_polish(test_cont,&test_double));

   strcpy(test_cont->words[1],"7");
   strcpy(test_cont->words[2],";");
   assert(!run_polish(test_cont,&test_double));
   free_word_cont(test_cont);

   test_cont=init_word_cont();
   strcpy(test_cont->words[0],"7");
   strcpy(test_cont->words[1],"7");
   strcpy(test_cont->words[2],"+");
   strcpy(test_cont->words[3],"+");
   strcpy(test_cont->words[4],";");
   assert(!run_polish(test_cont,&test_double));
   free_word_cont(test_cont);

   test_cont=init_word_cont();
   strcpy(test_cont->words[0],"7");
   assert(!run_polish(test_cont,&test_double));
   free_word_cont(test_cont);


   /*test set funcs*/

   test_cont=init_word_cont();
   strcpy(test_cont->words[0],"A");
   assert(valid_variable(test_cont));
   strcpy(test_cont->words[0],"Z");
   assert(valid_variable(test_cont));
   strcpy(test_cont->words[0],"a");
   assert(!valid_variable(test_cont));
   strcpy(test_cont->words[0],"");
   assert(!valid_variable(test_cont));
   strcpy(test_cont->words[0],"!");
   assert(!valid_variable(test_cont));

   strcpy(test_cont->words[0],"A");
   assert(get_var_pos(test_cont,&i));
   assert(i==0);
   test_cont->position=0;

   strcpy(test_cont->words[0],"Z");
   assert(get_var_pos(test_cont,&i));
   assert(i==25);
   test_cont->position=0;

   strcpy(test_cont->words[0],"B");
   assert(get_var_pos(test_cont,&i));
   assert(i==1);
   test_cont->position=0;


   strcpy(test_cont->words[0],"0");
   assert(!get_var_pos(test_cont,&i));
   strcpy(test_cont->words[0],"AB");
   assert(!get_var_pos(test_cont,&i));
   strcpy(test_cont->words[0],"a");
   assert(!get_var_pos(test_cont,&i));

   free_word_cont(test_cont);


   test_cont=init_word_cont();

   strcpy(test_cont->words[0],"SET");
   strcpy(test_cont->words[1],"A");
   strcpy(test_cont->words[2],":=");
   strcpy(test_cont->words[3],"90");
   strcpy(test_cont->words[4],"90");
   strcpy(test_cont->words[5],"+");
   strcpy(test_cont->words[6],";");
   assert(run_set(test_cont));
   assert(compare_doubles(*test_cont->var_array[0],180));
   free_word_cont(test_cont);


   test_cont=init_word_cont();

   strcpy(test_cont->words[0],"SET");
   strcpy(test_cont->words[1],"B");
   strcpy(test_cont->words[2],":=");
   strcpy(test_cont->words[3],"90");
   strcpy(test_cont->words[4],"2");
   strcpy(test_cont->words[5],"/");
   strcpy(test_cont->words[6],";");
   assert(run_set(test_cont));
   assert(compare_doubles(*test_cont->var_array[1],45));
   free_word_cont(test_cont);

   test_cont=init_word_cont();
   strcpy(test_cont->words[0],"SET");
   strcpy(test_cont->words[1],"Z");
   strcpy(test_cont->words[2],":=");
   strcpy(test_cont->words[3],"90.5");
   strcpy(test_cont->words[4],"2");
   strcpy(test_cont->words[5],"/");
   strcpy(test_cont->words[6],";");
   assert(run_set(test_cont));
   assert(compare_doubles(*test_cont->var_array[25],45.25));
   free_word_cont(test_cont);

   test_cont=init_word_cont();
   strcpy(test_cont->words[0],"SET");
   strcpy(test_cont->words[1],"Z");
   strcpy(test_cont->words[2],":=");
   strcpy(test_cont->words[3],"90.5");
   strcpy(test_cont->words[4],";");
   assert(run_set(test_cont));
   assert(compare_doubles(*test_cont->var_array[25],90.5));
   free_word_cont(test_cont);

   test_cont=init_word_cont();
   strcpy(test_cont->words[0],"SET");
   strcpy(test_cont->words[1],"Z");
   strcpy(test_cont->words[2],":=");
   strcpy(test_cont->words[3],"90.5");
   strcpy(test_cont->words[4],"2");
   strcpy(test_cont->words[5],"/");
   assert(!run_set(test_cont));
   free_word_cont(test_cont);

   test_cont=init_word_cont();
   strcpy(test_cont->words[0],"SET");
   strcpy(test_cont->words[1],"Z");
   strcpy(test_cont->words[2],":=");
   strcpy(test_cont->words[3],"90.5");
   strcpy(test_cont->words[4],"/");
   strcpy(test_cont->words[5],";");
   assert(!run_set(test_cont));
   free_word_cont(test_cont);

   test_cont=init_word_cont();
   strcpy(test_cont->words[0],"SET");
   strcpy(test_cont->words[1],"a");
   strcpy(test_cont->words[2],":=");
   strcpy(test_cont->words[3],"90.5");
   assert(!run_set(test_cont));
   free_word_cont(test_cont);

   test_cont=init_word_cont();
   strcpy(test_cont->words[0],"SET");
   strcpy(test_cont->words[1],"A");
   strcpy(test_cont->words[2],":");
   strcpy(test_cont->words[3],"90.5");
   assert(!run_set(test_cont));
   free_word_cont(test_cont);

   /*test other funcs using variables especially polish*/
   test_cont=init_word_cont();
   strcpy(test_cont->words[0],"SET");
   strcpy(test_cont->words[1],"Z");
   strcpy(test_cont->words[2],":=");
   strcpy(test_cont->words[3],"90.5");
   strcpy(test_cont->words[4],";");
   strcpy(test_cont->words[5],"Z");
   assert(run_set(test_cont));
   assert(get_var(test_cont,&test_double));
   assert(compare_doubles(test_double,90.5));
   free_word_cont(test_cont);

   test_cont=init_word_cont();
   strcpy(test_cont->words[0],"SET");
   strcpy(test_cont->words[1],"Z");
   strcpy(test_cont->words[2],":=");
   strcpy(test_cont->words[3],"90.5");
   strcpy(test_cont->words[4],";");
   strcpy(test_cont->words[5],"z");
   assert(run_set(test_cont));
   assert(!get_var(test_cont,&test_double));
   free_word_cont(test_cont);

   test_cont=init_word_cont();
   strcpy(test_cont->words[0],"SET");
   strcpy(test_cont->words[1],"Z");
   strcpy(test_cont->words[2],":=");
   strcpy(test_cont->words[3],"90.5");
   strcpy(test_cont->words[4],";");
   strcpy(test_cont->words[5],"A");
   assert(run_set(test_cont));
   assert(!get_var(test_cont,&test_double));
   free_word_cont(test_cont);

   test_cont=init_word_cont();
   strcpy(test_cont->words[0],"SET");
   strcpy(test_cont->words[1],"Z");
   strcpy(test_cont->words[2],":=");
   strcpy(test_cont->words[3],"90.5");
   strcpy(test_cont->words[4],";");
   strcpy(test_cont->words[5],"Z");
   assert(run_set(test_cont));
   assert(get_varnum(test_cont,&test_double));
   assert(test_cont->position==6);
   assert(compare_doubles(test_double,90.5));
   free_word_cont(test_cont);

   test_cont=init_word_cont();
   strcpy(test_cont->words[0],"SET");
   strcpy(test_cont->words[1],"Z");
   strcpy(test_cont->words[2],":=");
   strcpy(test_cont->words[3],"90.5");
   strcpy(test_cont->words[4],";");
   strcpy(test_cont->words[5],"90.5");
   assert(run_set(test_cont));
   assert(get_varnum(test_cont,&test_double));
   assert(test_cont->position==6);
   assert(compare_doubles(test_double,90.5));
   free_word_cont(test_cont);

   test_cont=init_word_cont();
   strcpy(test_cont->words[0],"90.5");
   assert(get_varnum(test_cont,&test_double));
   assert(compare_doubles(test_double,90.5));
   assert(test_cont->position==1);
   free_word_cont(test_cont);


   /*going to redo a couple of the previous tests with
   some variables now that we can set them*/
   test_line_cont=init_line_cont();
   test_line=init_origin();
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
   assert(run_instruction_list(test_cont,test_line_cont,test_line));
   assert(test_cont->position==10);
   assert(test_line_cont->size==1);
   assert(compare_doubles(test_line_cont->array[0]->end->y,0));
   assert(compare_doubles(test_line_cont->array[0]->end->x,-30));
   assert(compare_doubles(test_line->start->y,0));
   assert(compare_doubles(test_line->start->x,-30));
   free_line(test_line);
   free_line_cont(test_line_cont);
   free_word_cont(test_cont);



   test_line_cont=init_line_cont();
   test_line=init_origin();
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
   assert(run_instruction_list(test_cont,test_line_cont,test_line));
   assert(test_cont->position==15);
   assert(test_line_cont->size==1);
   assert(compare_doubles(test_line_cont->array[0]->end->y,0));
   assert(compare_doubles(test_line_cont->array[0]->end->x,-30));
   assert(compare_doubles(test_line->start->y,0));
   assert(compare_doubles(test_line->start->x,-30));
   free_line(test_line);
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
   assert(run_set(test_cont));
   assert(run_set(test_cont));
   assert(run_set(test_cont));
   assert(run_polish(test_cont,&test_double));
   assert(compare_doubles(test_double,-14.8));
   free_word_cont(test_cont);

   /*testing do loops*/
   test_cont=init_word_cont();
   test_line=init_origin();
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

   assert(do_helper(test_cont,&i, &test_double,&test_double_2));
   assert(i==0);
   assert(compare_doubles(test_double,3));
   assert(compare_doubles(test_double_2,9));
   assert(test_cont->position==7);
   test_cont->position=0;
   assert(run_do(test_cont,test_line_cont,test_line));
   assert(test_line_cont->size==7);
   assert(test_cont->position==10);

   free_line(test_line);
   free_line_cont(test_line_cont);
   free_word_cont(test_cont);



   /*set A then use it in a loop with itself*/
   test_cont=init_word_cont();
   test_line=init_origin();
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

   assert(run_set(test_cont));
   assert(run_do(test_cont,test_line_cont,test_line));
   assert(test_line_cont->size==7);
   assert(test_cont->position==15);

   free_line(test_line);
   free_line_cont(test_line_cont);
   free_word_cont(test_cont);

   /*change the var in the loop*/
   test_cont=init_word_cont();
   test_line=init_origin();
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
   assert(run_do(test_cont,test_line_cont,test_line));
   assert(test_line_cont->size==1);
   assert(test_cont->position==15);

   free_line(test_line);
   free_line_cont(test_line_cont);
   free_word_cont(test_cont);

   test_cont=init_word_cont();
   test_line=init_origin();
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
   assert(run_do(test_cont,test_line_cont,test_line));
   assert(test_line_cont->size==0);

   free_line(test_line);
   free_line_cont(test_line_cont);
   free_word_cont(test_cont);

   test_cont=init_word_cont();
   test_line=init_origin();
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
   assert(!run_do(test_cont,test_line_cont,test_line));
   free_line(test_line);
   free_line_cont(test_line_cont);
   free_word_cont(test_cont);

   /*even if loop never happens we
   want to spot syntax errors*/
   test_cont=init_word_cont();
   test_line=init_origin();
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
   assert(!run_do(test_cont,test_line_cont,test_line));
   free_line(test_line);
   free_line_cont(test_line_cont);
   free_word_cont(test_cont);


   test_cont=init_word_cont();
   test_line=init_origin();
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
   assert(!run_do(test_cont,test_line_cont,test_line));
   free_line(test_line);
   free_line_cont(test_line_cont);
   free_word_cont(test_cont);


   test_cont=init_word_cont();
   test_line=init_origin();
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
   assert(!run_do(test_cont,test_line_cont,test_line));
   free_line(test_line);
   free_line_cont(test_line_cont);
   free_word_cont(test_cont);

   /*loop within a loop*/
   test_cont=init_word_cont();
   test_line=init_origin();
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
   assert(run_do(test_cont,test_line_cont,test_line));
   assert(test_line_cont->size==14);
   assert(test_cont->position==18);
   assert(compare_doubles(test_line_cont->array[13]->end->y,14));
   free_line(test_line);
   free_line_cont(test_line_cont);
   free_word_cont(test_cont);


   /*test main with full sets of instructions*/

   test_cont=init_word_cont();
   test_line=init_origin();
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

   assert(run_main(test_cont,test_line_cont,test_line));
   assert(test_cont->position==20);
   assert(test_line_cont->size==14);
   assert(compare_doubles(test_line_cont->array[13]->end->y,14));

   free_line(test_line);
   free_line_cont(test_line_cont);
   free_word_cont(test_cont);

   test_cont=init_word_cont();
   test_line=init_origin();
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


   assert(run_main(test_cont,test_line_cont,test_line));
   assert(test_cont->position==14);
   assert(test_line_cont->size==8);
   assert(compare_doubles(test_line_cont->array[7]->end->x,0));
   assert(compare_doubles(test_line_cont->array[7]->end->y,0));
   free_line(test_line);
   free_line_cont(test_line_cont);
   free_word_cont(test_cont);


   test_cont=init_word_cont();
   test_line=init_origin();
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

   assert(run_main(test_cont,test_line_cont,test_line));
   assert(test_line_cont->size==450);

   free_line(test_line);
   free_line_cont(test_line_cont);
   free_word_cont(test_cont);

   test_cont=init_word_cont();
   test_line=init_origin();
   test_line_cont=init_line_cont();


   test_cont=read_in_file("test_files/test_turtles/parser/valid/adding_loops.txt");
   test_line=init_origin();
   test_line_cont=init_line_cont();
   assert(run_main(test_cont,test_line_cont,test_line));
   assert(test_line_cont->size==8);

   free_line(test_line);
   free_line_cont(test_line_cont);
   free_word_cont(test_cont);

   test_cont=read_in_file("test_files/test_turtles/parser/valid/nested_loops.txt");
   test_line=init_origin();
   test_line_cont=init_line_cont();

   assert(run_main(test_cont,test_line_cont,test_line));
   assert(test_line_cont->size==450);
   free_line(test_line);
   free_line_cont(test_line_cont);
   free_word_cont(test_cont);


   test_cont=read_in_file("test_files/test_turtles/parser/valid/polish.txt");
   test_line=init_origin();
   test_line_cont=init_line_cont();
   assert(!run_main(test_cont,test_line_cont,test_line));
   free_line(test_line);
   free_line_cont(test_line_cont);
   free_word_cont(test_cont);


   test_cont=read_in_file("test_files/test_turtles/parser/valid/polish_2.txt");
   test_line=init_origin();
   test_line_cont=init_line_cont();
   assert(!run_main(test_cont,test_line_cont,test_line));
   assert(strcmp("potentially haven't set variable before calling it",test_cont->err_message)==0);
   free_line(test_line);
   free_line_cont(test_line_cont);
   free_word_cont(test_cont);


   test_cont=read_in_file("test_files/test_turtles/interpreter/invalid/too_many_nums.txt");
   test_line=init_origin();
   test_line_cont=init_line_cont();
   assert(!run_main(test_cont,test_line_cont,test_line));
   assert(strcmp("more than one number left on stack at end of expr",test_cont->err_message)==0);
   free_line(test_line);
   free_line_cont(test_line_cont);
   free_word_cont(test_cont);


   test_cont=read_in_file("test_files/test_turtles/interpreter/invalid/no_nums.txt");
   test_line=init_origin();
   test_line_cont=init_line_cont();
   assert(!run_main(test_cont,test_line_cont,test_line));
   assert(strcmp("no numbers on the stack at end of expr",test_cont->err_message)==0);
   free_line(test_line);
   free_line_cont(test_line_cont);
   free_word_cont(test_cont);
   return 0;

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

   n_cont->stackptr=stack_init();
   for(i=0;i<NUMVARS;i++)
   {
      n_cont->var_array[i]=NULL;
   }
   n_cont->err_message[0]='\0';
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
