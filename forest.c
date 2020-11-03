#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <string.h>
#include <assert.h>
#include "neillsimplescreen.h"


typedef enum state {empty,tree,fire} state;
typedef enum bool {false,true} bool;


#define WAITTIME 0.1

#define MAXCOLS 80
#define MAXROWS 30
/*board is 80*30 + another 30 for the \ns and 1 for \0*/
#define MAXSIZESTR 2431

#define BEHIND -1
#define INFRONT 1
#define INLINE 0

#define G 250
#define L 10*G
#define ITERS 1000
#define EVENT 1
#define NOEVENT 2
/*going to assume that everything that isnt on the
 board is empty so dont have to worry about it */


/*returns true if position not on board*/
bool over_edge(int current_row,int current_col,\
               int num_rows,int num_cols);


/*puts the board into string format*/
void stringify(const state board[][MAXCOLS],\
            char target_str[],int num_rows,int num_cols);


/*returns fire if one is found within 8 cell neighbourhood
 otherwise tree*/
bool fire_in_area(int current_row, int current_col,\
               const state board[][MAXCOLS],\
               int num_rows,int num_cols);

/*given random number if that number is 1 then
returns a tree else returns empty*/
state empty_to_tree(int random);

/*given random number if that number is 1 then
returns a fire else returns tree*/
state lightning(int random);

/*basically a wrapper to combine fire in area and lightning
to make code more readible first checks if fire_in_area
 then if not generates random number and sees if it matches
  conditions for lightning
*/
state tree_to_fire(int current_row, int current_col,\
                  const state board[][MAXCOLS],\
                  int num_rows,int num_cols,\
                  int (*random_generator)(int));

/* takes the previous board and outputs a new cell using
rules given.
 to get around randoms have a function pointer as one of
 the arguments where in testing can point to hard coded
 number and in main can point to a random num to simulate
 thunder/tree growth*/
state trans_cell(int current_row, int current_col,\
                  const state board[][MAXCOLS],\
                  int num_rows,int num_cols,\
                  int (*random_generator)(int));

/*fills new board using previous*/
void fill_board(const state previous[][MAXCOLS],\
               state target[][MAXCOLS],int num_rows,\
               int num_cols,int (*random_generator)(int));

/*function for copying a board to another board of the
same size*/
void copy_board(const state previous[][MAXCOLS],\
               state target[][MAXCOLS],\
               int num_rows,int num_cols);


/*generates a random number between 0 and max num
inclusive of 0 not inclusive of max num*/
int random_num_gen(int max_num);

/*test func for creating fires/trees. not used in main
program.
needs to have int as argument so it is the same form
as random_num_gen*/
int test_generator_event(int placeholder);

/*test func for not creating fires/trees. not
used in main program.
needs to have int as argument so it is the same
 form as random_num_gen*/
int test_generator_no_event(int placeholder);

/*function for tests*/
void test(void);



/*takes a string of trees fires +empties and prints
fires in red and trees in green*/
void print_colour(char string[]);

int main(void)
{
   int i;
   state previous[MAXROWS][MAXCOLS]={{0}};
   state current[MAXROWS][MAXCOLS]={{0}};
   char str_board[MAXSIZESTR];
   srand(time(NULL));
   test();
   /*for uncoloured*/
   /*for(i=0;i<ITERS;i++)
   {
      stringify(previous,str_board,MAXROWS,MAXCOLS);
      printf("%s",str_board);
      fill_board(previous,current,MAXROWS,MAXCOLS,\
                  random_num_gen);
      copy_board(current,previous,MAXROWS,MAXCOLS);
      printf("\n\n");
   }*/
   /*for coloured*/
   neillclrscrn();
   neillcursorhome();
   for(i=0;i<ITERS;i++)
   {

      stringify(previous,str_board,MAXROWS,MAXCOLS);
      print_colour(str_board);
      fill_board(previous,current,MAXROWS,MAXCOLS,\
                  random_num_gen);
      copy_board(current,previous,MAXROWS,MAXCOLS);
      neillcursorhome();
      neillbusywait(WAITTIME);
   }
   neillclrscrn();
   neillfgcol(white);
   return 0;
}

void test(void)
{
   int i,j,tree_count,fire_count;
   int hist[10];
   int n;
   state test_board[4][MAXCOLS]={{1,0,1,2},
                                 {1,0,0,0},
                                 {1,1,1,0},
                                 {0,1,1,2}};

   state target_board[4][MAXCOLS]={{0},{0},{0},{0}};

   state empty_board[MAXROWS][MAXCOLS]={{0}};
   state empty_target_board[MAXROWS][MAXCOLS]={{0}};



   char* target_str="@ @*\n@   \n@@@ \n @@*\n";
   char* target_str_all_events= "*@* \n*@@@\n***@\n@** \n";
   char* target_str_no_events= "@ * \n@   \n@@* \n @* \n";

   /*board is 80*30 + another 30 for the \ns and 1 for \0*/
   char test_str[2431];


   assert(over_edge(0,0,10,10)==false);
   assert(over_edge(-1,0,10,10)==true);
   assert(over_edge(0,-1,10,10)==true);
   assert(over_edge(10,1,10,10)==true);
   assert(over_edge(1,10,10,10)==true);



   /*making sure stringify works*/
   stringify(test_board,test_str,4,4);
   assert(strcmp(test_str,target_str)==0);


   /*for this func need to test how it performs on sides
   as well as in centre dont need to worry if it classifies
   a empty space as a tree or something as it is going in a
   bigger function that will check for that*/
   assert(fire_in_area(0,0,test_board,4,4)==false);
   assert(fire_in_area(0,2,test_board,4,4)==true);
   assert(fire_in_area(2,1,test_board,4,4)==false);
   assert(fire_in_area(3,2,test_board,4,4)==true);
   assert(fire_in_area(3,2,test_board,4,4)==true);

   assert(empty_to_tree(3)==empty);
   assert(empty_to_tree(1)==tree);
   assert(empty_to_tree(0)==empty);

   assert(lightning(1)==fire);
   assert(lightning(3)==tree);
   assert(lightning(0)==tree);

   /*test if fire_in_area working within function*/
   assert(trans_cell(3,2,test_board,4,4,\
         test_generator_no_event)==fire);
   /*test the empty cells */
   assert(trans_cell(3,0,test_board,4,4,\
         test_generator_event)==tree);
   assert(trans_cell(3,0,test_board,4,4,\
         test_generator_no_event)==empty);
   /*test tree cells when no fire around*/
   assert(trans_cell(0,0,test_board,4,4,\
         test_generator_no_event)==tree);
   assert(trans_cell(0,0,test_board,4,4,\
            test_generator_event)==fire);
   /*testing fire cells*/
   assert(trans_cell(0,3,test_board,4,4,\
         test_generator_event)==empty);
   assert(trans_cell(0,3,test_board,4,4,\
      test_generator_no_event)==empty);


   for(i=0;i<100000;i++)
   {
      n=random_num_gen(10);
      assert(n>=0);
      assert(n<10);
      hist[n]++;
   }
   for(i=0;i<10;i++)
   {
      assert(hist[i]<20000);
   }


   /*test what board would look like when every
   random event happens*/
   fill_board(test_board,target_board,4,4,\
            test_generator_event);
   stringify(target_board,test_str,4,4);
   assert(strcmp(test_str,target_str_all_events)==0);

   /*test what board would look like when no random
    event happens*/
   fill_board(test_board,target_board,4,4,\
            test_generator_no_event);
   stringify(target_board,test_str,4,4);
   assert(strcmp(test_str,target_str_no_events)==0);

   /*going to test that using my random function
    that it doesnt cause too many trees/fires to appear*/
   fill_board(empty_board,empty_target_board,MAXROWS,\
               MAXCOLS,random_num_gen);

   /*checking for how many trees after an iteration
   from having all empty cells*/
   tree_count=0;
   for(i=0;i<MAXROWS;i++)
   {
      for(j=0;j<MAXCOLS;j++)
      {
         if(empty_target_board[i][j]==tree)
         {
            tree_count++;
         }
      }
   }
   assert(tree_count<30);

   /*checking for how many fires after an iteration
   from having all trees*/
   for(i=0;i<MAXROWS;i++)
   {
      for(j=0;j<MAXCOLS;j++)
      {
         empty_board[i][j]=tree;
      }
   }
   fill_board(empty_board,empty_target_board,\
            MAXROWS,MAXCOLS,random_num_gen);

   fire_count=0;
   for(i=0;i<MAXROWS;i++)
   {
      for(j=0;j<MAXCOLS;j++)
      {
         if(empty_target_board[i][j]==fire)
         {
            fire_count++;
         }
      }
   }
   assert(fire_count<20);

   /*checking copying board function works*/
   copy_board(test_board,target_board,4,4);
   stringify(target_board,test_str,4,4);
   assert(strcmp(test_str,target_str)==0);


   assert(tree_to_fire(0,2,test_board,4,4,\
      test_generator_no_event)==fire);

   assert(tree_to_fire(0,0,test_board,4,4,\
      test_generator_no_event)==tree);

   assert(tree_to_fire(0,0,test_board,4,4,\
      test_generator_event)==fire);
}





bool over_edge(int current_row,int current_col,\
               int num_rows,int num_cols)
{
   if(current_row<0||current_row>=num_rows)
   {
      return true;
   }
   if(current_col<0||current_col>=num_cols)
   {
      return true;
   }
   return false;
}






/*this puts a board into a string so can easily
   test also lets me print out board after each
   iteration +can be tested*/
void stringify(const state board[][MAXCOLS],\
               char target_str[],int num_rows,int num_cols)
{
   int i,j,board_pos;
   board_pos=0;
   for(i=0;i<num_rows;i++)
   {
      for(j=0;j<num_cols;j++)
      {
         switch(board[i][j])
         {
            case(empty):
            target_str[board_pos]=' ';
            board_pos++;
            break;

            case(fire):
            target_str[board_pos]='*';
            board_pos++;
            break;

            case(tree):
            target_str[board_pos]='@';
            board_pos++;
            break;

            default:
            fprintf(stderr,"not valid cell\n");
            exit(EXIT_FAILURE);
         }
      }

      target_str[board_pos]='\n';
      board_pos++;
   }
   target_str[board_pos]='\0';
}



/*returns true if fire is found within 8 cell
neighbourhood otherwise false*/
bool fire_in_area(int current_row, int current_col,\
                  const state board[][MAXCOLS],\
                  int num_rows,int num_cols)
{
   int i,j;

   for(i=BEHIND;i<=INFRONT;i++)
   {
      for(j=BEHIND;j<=INFRONT;j++)
      {
         if(!over_edge(current_row+i,current_col+j,\
            num_rows,num_cols))
         {
            if(board[current_row+i][current_col+j]==fire)
            {
               return true;
            }
         }
      }
   }

   return false;
}


state empty_to_tree(int random)
{
   if(random==EVENT)
   {
      return tree;
   }
   else
   {
      return empty;
   }
}


state lightning(int random)
{
   if(random==EVENT)
   {
      return fire;
   }
   else
   {
      return tree;
   }
}

state tree_to_fire(int current_row, int current_col,\
                  const state board[][MAXCOLS],\
                  int num_rows,int num_cols,\
                  int (*random_generator)(int))
{
   int random_num;
   if(fire_in_area(current_row,current_col,\
      board,num_rows,num_cols))
   {
      return fire;
   }
   else
   {
      /*generates random number up to L*/
      random_num= random_generator(L);
      /*returns fire if random number is 1 else tree*/
      return lightning(random_num);
   }
}

/* to get around randoms have a function pointer
 as one of the arguments where in testing can
  point to hard coded number and in main can point to
 a random num*/
state trans_cell(int current_row, int current_col,\
               const state board[][MAXCOLS],\
               int num_rows,int num_cols,\
               int (*random_generator)(int))
{
   state current_cell,random_num;
   current_cell=board[current_row][current_col];
   switch(current_cell)
   {
      case(empty):
      /*generates random number up to G*/
      random_num= random_generator(G);
      /*returns tree if random number is 1 else empty*/
      return empty_to_tree(random_num);

      case(tree):
      return tree_to_fire(current_row,current_col,board,\
                     num_rows,num_cols,random_generator);

      case(fire):
      return empty;

      default:
      fprintf(stderr,"not valid cell\n");
      exit(EXIT_FAILURE);
   }
}


/*fills a target board from a previous board have to
 pass in function to generate random numbers as well-
in testing pass in function with known num*/
void fill_board(const state previous[][MAXCOLS],\
               state target[][MAXCOLS],\
                int num_rows,int num_cols,\
                int (*random_generator)(int))
{
   int i,j;
   for(i=0;i<num_rows;i++)
   {
      for(j=0;j<num_cols;j++)
      {
         target[i][j]=trans_cell(i,j,previous,\
                      num_rows,num_cols,random_generator);
      }
   }
}

/*test func for creating fires/trees not used in
main program
needs to have int as argument so it is the same
 form as random_num_gen but is unused*/
int test_generator_event(int placeholder)
{
   int unused;
   unused=placeholder;
   return EVENT;
}

/*test func for not creating fires/trees not used in
 main program
needs to have int as argument so it is the same form
as random_num_gen but is unused*/
int test_generator_no_event(int placeholder)
{
   int unused;
   unused=placeholder;
   return NOEVENT;
}



int random_num_gen(int max_num)
{
   int random_num;
   random_num = rand()% max_num;
   return random_num;
}

void copy_board(const state previous[][MAXCOLS],\
                  state target[][MAXCOLS],\
                  int num_rows,int num_cols)
{
   int i,j;
   for(i=0;i<num_rows;i++)
   {
      for(j=0;j<num_cols;j++)
      {
         target[i][j]=previous[i][j];
      }
   }
}


/*function for excercise 4.6*/


/*wasnt sure how to test this since its just printing and
not sure if you can send to string the ansci codes*/
void print_colour(char string[])
{
   int i;
   for(i=0;string[i]!='\0';i++)
   {

      if(string[i]=='@')
      {
         neillfgcol(green);
      }
      if(string[i]=='*')
      {
         neillfgcol(red);
      }
      printf("%c",string[i]);
   }
}
