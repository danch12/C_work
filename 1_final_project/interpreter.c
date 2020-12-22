#include <ctype.h>
#include <math.h>
#include "general.h"
#include "stack_funcs.h"


#define PI 3.14159265359
#define DEGTORAD 180
#define DEGREES 360
#define ORIGIN 0
#define EPSILON 0.000001
#define LAG 1
#define MAXTESTCAP 100
#define MAXTESTLEN 50
#define INITSIZE 16
#define SCALEFACTOR 2

typedef enum direction {left, right,invalid} direction;
typedef enum op {plus, minus,mult,divide,invalid_op} op;

typedef struct coord
{
   double x;
   double y;
}coord;

typedef struct line
{
   coord* start;
   /*going to be a running total
   take abs(num%360) */
   double rotation;
   coord* end;
}line;


/*may store the stack and any hashmaps in the
word container as they are sort of like input
controls where as line container is more of
a output control so will keep separate
we can see word_container as holding all the
intermediate stages of instructions - basically
everything apart from the moves
because of this it makes sense to include a pending_line
member that tracks the progress of a line before we
have recieved a FD call
 */
typedef struct word_container
{
   char** words;
   int position;
   int capacity;
   stack* stackptr;
}word_cont;

typedef struct line_container
{
   line** array;
   int size;
   int capacity;
}line_cont;



word_cont* init_word_cont(void);
bool free_word_cont(word_cont* to_free);

line_cont* init_line_cont(void);
bool free_line_cont(line_cont* to_free);
void store_line(line_cont* l_arr, line* to_add);

bool rotate(double degrees,coord* to_rotate, coord* rotation_point);
coord* init_coords(double x, double y);
bool get_num(word_cont* to_check,double* num);

/*pretty much the same as parser valid_num except we
dont increase the position at the end
called different things because we import*/
bool valid_num(word_cont* to_check);

/*potentially add pending line in to word_cont -
as word cont deals with intermediate stages of instructions
which is basically what pending line is*/
bool run_instruction(word_cont* to_check,line_cont* line_arr,\
                     line* pending_line);
bool run_instruction_list(word_cont* to_check,line_cont* line_arr,\
                           line* pending_line);
bool run_main(word_cont* to_check,line_cont* line_arr,\
            line* pending_line);
bool get_rotation(word_cont* to_check,line* pending_line);
direction direction_helper(word_cont* to_check);

bool move_forward(word_cont* to_check,line* pending_line,line_cont* l_arr);
line* init_origin(void);
bool free_line(line* to_free);
line* finish_line(line* prev_line,coord* endpoint);


/*get op v similar to parser except we dont increase position
and we return a op instead of bool*/
op get_op(word_cont* to_check);
bool do_operation(word_cont* to_check);
bool polish_num(word_cont* to_check);
/*need to get remaining num and check only one num there*/
bool finish_polish(word_cont* to_check,double* result);
/*num is going to be passed in by set function*/
bool run_polish(word_cont* to_check,double* num);
/*returns true if same*/
bool compare_doubles(double d_1, double d_2);
void test(void);



int main(void)
{
   test();
   return 0;
}


void test(void)
{
   coord* test_coord,*test_coord2, *origin;
   int i;
   word_cont* test_cont;
   double test_double;
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
}


/*https://www.khanacademy.org/computing/pixar/sets/rotation/v/sets-8
found formulas for rotating a point */
bool rotate(double degrees,coord* to_rotate, coord* rotation_point)
{
   double radians;
   double n_y , n_x;

   if(to_rotate&&rotation_point)
   {

      radians = degrees * (PI/DEGTORAD);

      /*go to origin then rotate*/
      n_y= to_rotate->y - rotation_point->y;
      n_x= to_rotate->x -rotation_point->x;

      to_rotate->y = ((n_x *sin(radians)) + \
                     (n_y * cos(radians)))+ rotation_point->y;

      to_rotate->x = (n_x * cos(radians) - \
                     (n_y *sin(radians)))+rotation_point->x;
      return true;
   }
   return false;
}

coord* init_coords(double x, double y)
{
   coord* n_coord;
   n_coord=safe_calloc(1,sizeof(coord));
   n_coord->x=x;
   n_coord->y=y;
   return n_coord;
}

line* finish_line(line* prev_line,coord* endpoint)
{
   line* n_line;
   n_line= safe_calloc(1,sizeof(line));
   n_line->start=init_coords(prev_line->start->x,prev_line->start->y);

   n_line->rotation=prev_line->rotation;
   n_line->end=endpoint;
   return n_line;
}

line* init_origin(void)
{
   line* origin_line;
   origin_line= safe_calloc(1,sizeof(line));
   origin_line->start=init_coords(ORIGIN,ORIGIN);
   origin_line->rotation=0;
   origin_line->end=init_coords(ORIGIN,ORIGIN);
   return origin_line;
}

bool free_line(line* to_free)
{
   if(to_free)
   {
      free(to_free->start);
      free(to_free->end);

      free(to_free);

   }
   return true;
}





bool get_num(word_cont* to_check,double* num)
{

   char* ptr;
   if(to_check)
   {
      if(valid_num(to_check))
      {
         *num=strtod(to_check->words[to_check->position],&ptr);
         to_check->position++;
         return true;
      }
   }
   return false;
}


/*pretty much the same as parser except we
dont increase the position at the end*/
bool valid_num(word_cont* to_check)
{
   int i;
   int len;
   if(to_check->position>=to_check->capacity)
   {
      return false;
   }
   len=strlen(to_check->words[to_check->position]);
   /*blank string not number*/
   if(len==0)
   {
      return false;
   }
   for(i=0;i<len;i++)
   {
      if(!isdigit(to_check->words[to_check->position][i])\
         &&(to_check->words[to_check->position][i]!='.'))
      {
         return false;
      }
   }
   return true;
}


bool run_main(word_cont* to_check,line_cont* line_arr,\
            line* pending_line)
{
   if(to_check->position>=to_check->capacity)
   {
      return false;
   }
   if(strcmp(to_check->words[to_check->position],"{")==0)
   {
      to_check->position++;

      if(run_instruction_list(to_check,line_arr,pending_line))
      {
         return true;
      }
   }
   return false;
}



bool run_instruction_list(word_cont* to_check,line_cont* line_arr,\
                           line* pending_line)
{
   if(to_check->position>=to_check->capacity)
   {
      return false;
   }
   if(strcmp(to_check->words[to_check->position],"}")==0)
   {
      /*doesnt matter for end but for do loops important
      to increase position*/
      to_check->position++;
      return true;
   }
   else
   {
      if(run_instruction(to_check,line_arr,pending_line))
      {
         if(run_instruction_list(to_check,line_arr,pending_line))
         {
            return true;
         }
      }
   }
   return false;
}




/*potentially add pending line in to word_cont -
as word cont deals with intermediate stages of instructions
which is basically what pending line is*/
bool run_instruction(word_cont* to_check,line_cont* line_arr,\
                     line* pending_line)
{
   int init_pos;
   init_pos=to_check->position;
   if(init_pos>=to_check->capacity)
   {
      return false;
   }
   if(get_rotation(to_check,pending_line))
   {
      return true;
   }
   to_check->position=init_pos;
   if(move_forward(to_check,pending_line,line_arr))
   {
      return true;
   }
   return false;
}



bool get_rotation(word_cont* to_check,line* pending_line)
{
   double num;
   double temp;
   direction dir;
   dir=direction_helper(to_check);
   if(dir==invalid)
   {
      return false;
   }
   if(get_num(to_check,&num))
   {
      if(dir==left)
      {
         temp=num+pending_line->rotation;
      }
      else
      {
         temp= (DEGREES-num)+pending_line->rotation;
      }
      temp=fabs(fmod(temp,DEGREES));
      pending_line->rotation=temp;
      return true;
   }
   return false;
}


direction direction_helper(word_cont* to_check)
{
   direction dir;
   if(strcmp(to_check->words[to_check->position],"LT")==0)
   {
      to_check->position++;
      dir=left;
      return dir;
   }
   if(strcmp(to_check->words[to_check->position],"RT")==0)
   {
      to_check->position++;
      dir=right;
      return dir;
   }
   else
   {
      dir=invalid;
      return dir;
   }
}


/*to move forward we add a endpoint to the pending
line then we add that line to a collection then we create
a new line- may actually be easier to add a copy of the
line to the array so we dont have to deal with changing
addresses or anything
this is pretty much the meat and potatoes of the
whole thing since lines only appear when you move forward*/
bool move_forward(word_cont* to_check,line* pending_line,line_cont* l_arr)
{
   double num;
   double n_y;
   coord* end_coord;
   line* finished_line;
   if(strcmp(to_check->words[to_check->position],"FD")==0)
   {
      to_check->position++;
      if(get_num(to_check,&num))
      {
         n_y=pending_line->start->y +num;
         end_coord=init_coords(pending_line->start->x,n_y);
         /*rotate returns true if sucessful even if it
         rotates by nothing*/
         if(rotate(pending_line->rotation,\
            end_coord,pending_line->start))
         {
            finished_line=finish_line(pending_line,end_coord);
            store_line(l_arr,finished_line);
            /*reusing pending_line over and over by just changing
            its start point to the last lines end point*/
            pending_line->start->x =finished_line->end->x;
            pending_line->start->y =finished_line->end->y;
            return true;
         }
      }
   }
   return false;
}


void store_line(line_cont* l_arr, line* to_add)
{
   if(to_add)
   {
      l_arr->array[l_arr->size]=to_add;
      l_arr->size+=1;
      if(l_arr->size==l_arr->capacity)
      {
         l_arr->array=(line**)realloc(l_arr->array, \
            sizeof(line*)*l_arr->capacity*SCALEFACTOR);
         if(l_arr->array==NULL)
         {
            fprintf(stderr,"Ran out of space\n");
      	 	exit(EXIT_FAILURE);
         }
         l_arr->capacity=l_arr->capacity*SCALEFACTOR;
      }
   }
}


line_cont* init_line_cont(void)
{
   line_cont* l_arr;
   l_arr=(line_cont* )safe_calloc(1,sizeof(line_cont));
   l_arr->array=(line**)safe_calloc(INITSIZE,sizeof(line*));
   l_arr->capacity=INITSIZE;
   l_arr->size=0;
   return l_arr;
}

bool free_line_cont(line_cont* to_free)
{
   int i;
   if(to_free)
   {
      for(i=0;i<to_free->size;i++)
      {
         free_line(to_free->array[i]);
      }
      free(to_free->array);
      free(to_free);
   }
   return true;

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
   return n_cont;
}

bool free_word_cont(word_cont* to_free)
{
   int i;
   if(to_free)
   {
      for(i=0;i<to_free->capacity;i++)
      {
         free(to_free->words[i]);
      }
      free(to_free->words);
      stack_free(to_free->stackptr);
      free(to_free);
   }

   return true;
}



/*get op v similar to parser except we dont increase position
and we return a op instead of bool*/
op get_op(word_cont* to_check)
{
   if(to_check->position>=to_check->capacity)
   {
      return invalid_op;
   }
   if(strcmp(to_check->words[to_check->position],"+")==0)
   {
      return plus;
   }
   if(strcmp(to_check->words[to_check->position],"-")==0)
   {
      return minus;
   }
   if(strcmp(to_check->words[to_check->position],"*")==0)
   {
      return mult;
   }
   if(strcmp(to_check->words[to_check->position],"/")==0)
   {
      return divide;
   }
   return invalid_op;
}

bool do_operation(word_cont* to_check)
{
   op operator;
   double result, num1, num2;
   operator=get_op(to_check);
   if(operator==invalid_op)
   {
      return false;
   }
   if(!stack_pop(to_check->stackptr, &num2) || \
      !stack_pop(to_check->stackptr, &num1))
   {
      return false;
   }
   switch(operator)
   {
      case plus:
      result= num1 + num2;
      break;

      case minus:
      result= num1 - num2;
      break;

      case mult:
      result= num1 * num2;
      break;

      case divide:
      result =  num1 / num2;
      break;

      default:
      fprintf(stderr,"unexpected operator\n");
      exit(EXIT_FAILURE);
   }
   stack_push(to_check->stackptr,result);
   to_check->position++;
   return true;
}

/*add variables option here*/
bool polish_num(word_cont* to_check)
{
   double num;
   if(get_num(to_check,&num))
   {
      stack_push(to_check->stackptr,num);
      return true;
   }
   return false;

}

/*need to get remaining num and check only one num there*/
bool finish_polish(word_cont* to_check,double* result)
{
   if(!stack_pop(to_check->stackptr,result))
   {
      return false;
   }
   /*if numbers left over then something is wrong*/
   if(stack_peek(to_check->stackptr,result))
   {
      return false;
   }
   return true;

}

/*num is going to be passed in by set function*/
bool run_polish(word_cont* to_check,double* num)
{
   if(to_check->position>=to_check->capacity)
   {
      return false;
   }
   if(strcmp(to_check->words[to_check->position],";")==0)
   {
      /*need to get num and check only one num here*/
      if(finish_polish(to_check,num))
      {
         to_check->position++;
         return true;
      }
   }
   if(do_operation(to_check))
   {
      if(run_polish(to_check,num))
      {
         return true;
      }
   }
   if(polish_num(to_check))
   {
      if(run_polish(to_check,num))
      {
         return true;
      }
   }
   return false;
}



/*keep all lines that you draw*/

/*in the do loops the last number is inclusive*/

/*returns true if same*/
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
