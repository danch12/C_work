#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <string.h>
#include <assert.h>


typedef enum state {empty,tree,fire} state;
typedef enum bool {false,true} bool;

/*these refer to edges of board*/
typedef enum edges {none,up,down,left,right} edges;

/*struct that gives area for counting fires*/
typedef struct box{
   int left;
   int right;
   int up;
   int down;
} box;


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
/*going to assume that everything that isnt on the board is empty
 so dont have to worry about it */




bool over_edge(int position_row,int position_col,int num_rows,int num_cols);


/*creates a kinda box that identifies area we need to count fire cells in*/
box create_box(edges edge_row,edges edge_col);

/*this puts a board into a string so can easily test also lets me
  print out board after each iteration +can be tested itself as well*/
void stringify(const state board[][MAXCOLS],char target_str[],\
              int num_rows,int num_cols);


/*returns fire if one is found within 8 cell neighbourhood otherwise tree*/
state fire_in_area(int current_row, int current_col,const state board[][MAXCOLS],\
                  int num_rows,int num_cols);

/*given random number if that number is 1 then
returns a tree else returns empty*/
state empty_to_tree(int random);

/*could combine this and above function but looks quite neat like
this and having different names for them will hopefully lead to
clearer code when putting them in another function*/
state tree_to_fire(int random);

/*in this function to get around randoms im going to have a pointer
to a function as one of the arguments where in testing can point to
a function that returns a known number and then in the actual can point
to a random num
doing this sacrifices some readability perhaps but think its worth it*/
state trans_cell(int current_row, int current_col,const state board[][MAXCOLS],\
                  int num_rows,int num_cols,int (*random_generator)(int));

/*this function fills a target board from a previous board have to
pass in function to generate random numbers as well- did this so i
can pass a function that returns a known number for testing*/
void fill_board(const state previous[][MAXCOLS],state target[][MAXCOLS],\
             int num_rows,int num_cols,int (*random_generator)(int));

/*function for copying a board to another board of the same size*/
void copy_board(const state previous[][MAXCOLS],state target[][MAXCOLS],\
               int num_rows,int num_cols);


/*generates a random number between 1 and max num*/
int random_num_gen(int max_num);

/*test func for creating fires/trees not used in main program
needs to have int as argument so it is the same form as random_num_gen*/
int test_generator_event(int placeholder);

/*test func for not creating fires/trees not used in main program
needs to have int as argument so it is the same form as random_num_gen*/
int test_generator_no_event(int placeholder);

void test(void);

int main(void)
{
   int i;
   state previous[MAXROWS][MAXCOLS]={{0}};
   state current[MAXROWS][MAXCOLS]={{0}};
   char str_board[MAXSIZESTR];
   srand(time(NULL));
   test();
   for(i=0;i<ITERS;i++)
   {
      stringify(previous,str_board,MAXROWS,MAXCOLS);
      printf("%s",str_board);
      fill_board(previous,current,MAXROWS,MAXCOLS,random_num_gen);
      copy_board(current,previous,MAXROWS,MAXCOLS);
      printf("\n\n\n");
   }

   return 0;
}

void test(void)
{
   int i;
   box test_box_1,test_box_2,test_box_3,test_box_4,test_box_5;
   state test_board[][MAXCOLS]={{1,0,1,2},
                                 {1,0,0,0},
                                 {1,1,1,0},
                                 {0,1,1,2}};
   state target_board[][MAXCOLS]={{0},{0},{0},{0}};
   /*im pretty sure this makes the string constant and dont really
    want it to change as we are just using it for
    comparing outputs of functions*/
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
   as well as in centre dont need to worry if it classifies a empty space
   as a tree or something as it is going in a bigger function that will
   check for that*/
   assert(fire_in_area(0,0,test_board,4,4)==tree);
   assert(fire_in_area(0,2,test_board,4,4)==fire);
   assert(fire_in_area(2,1,test_board,4,4)==tree);
   assert(fire_in_area(3,2,test_board,4,4)==fire);
   assert(fire_in_area(3,2,test_board,4,4)==fire);

   assert(empty_to_tree(3)==empty);
   assert(empty_to_tree(1)==tree);
   assert(empty_to_tree(0)==empty);


   /*test if fire_in_area working within function*/
   assert(trans_cell(3,2,test_board,4,4,test_generator_no_event)==fire);
   /*test the empty cells */
   assert(trans_cell(3,0,test_board,4,4,test_generator_event)==tree);
   assert(trans_cell(3,0,test_board,4,4,test_generator_no_event)==empty);
   /*test tree cells when no fire around*/
   assert(trans_cell(0,0,test_board,4,4,test_generator_no_event)==tree);
   assert(trans_cell(0,0,test_board,4,4,test_generator_event)==fire);
   /*testing fire cells*/
   assert(trans_cell(0,3,test_board,4,4,test_generator_event)==empty);
   assert(trans_cell(0,3,test_board,4,4,test_generator_no_event)==empty);

   for(i=0;i<10000;i++)
   {
      assert(random_num_gen(10)>=1);
      assert(random_num_gen(10)<=10);
   }


   /*void fill_board(const state previous[][MAXCOLS],state target[][MAXCOLS],\
                   int num_rows,int num_cols,int (*random_generator)(int))*/
   /*test what board would look like when every random event happens*/
   fill_board(test_board,target_board,4,4,test_generator_event);
   stringify(target_board,test_str,4,4);
   assert(strcmp(test_str,target_str_all_events)==0);

   /*test what board would look like when no random event happens*/
   fill_board(test_board,target_board,4,4,test_generator_no_event);
   stringify(target_board,test_str,4,4);
   assert(strcmp(test_str,target_str_no_events)==0);


   copy_board(test_board,target_board,4,4);
   stringify(target_board,test_str,4,4);
   assert(strcmp(test_str,target_str)==0);
}





bool over_edge(int position_row,int position_col,int num_rows,int num_cols)
{
   if(position_row<0||position_row>=num_rows)
   {
      return true;
   }
   if(position_col<0||position_col>=num_cols)
   {
      return true;
   }
   return false;
}


/*creates a box that identifies area we need to count fire cells in
also im assuming the 8 neighbourhood is like the up down left right
and diagonals of a central cell*/
/*box create_box(edges edge_row,edges edge_col)
{
   box countarea;
   switch(edge_row)
   {
      case(up):
      countarea.up=INLINE;
      countarea.down=INFRONT;
      break;

      case(down):
      countarea.up=BEHIND;
      countarea.down=INLINE;
      break;

      default:
      countarea.up=BEHIND;
      countarea.down=INFRONT;
   }
   switch(edge_col)
   {
      case(left):
      countarea.left=INLINE;
      countarea.right=INFRONT;
      break;

      case(right):
      countarea.left=BEHIND;
      countarea.right=INLINE;
      break;

      default:
      countarea.left=BEHIND;
      countarea.right=INFRONT;
   }
   return countarea;
}*/



/*this puts a board into a string so can easily test also lets me
  print out board after each iteration +can be tested*/
void stringify(const state board[][MAXCOLS],char target_str[],\
               int num_rows,int num_cols)
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
            printf("not a valid cell\n");
            exit(1);
         }
      }
      /*added this in so can use the func to print stuff in main as
      well as testing the array funcs otherwise wouldnt be needed*/
      target_str[board_pos]='\n';
      board_pos++;
   }
   target_str[board_pos]='\0';
}





/*returns fire if one is found within 8 cell neighbourhood otherwise tree*/
/*state fire_in_area(int current_row, int current_col,const state board[][MAXCOLS],\
                  int num_rows,int num_cols)
{
   int i,j;
   edges edge_row,edge_col;
   box relevant_area;
   edge_row = on_edge_row(current_row,num_rows);
   edge_col = on_edge_col(current_col,num_cols);
   relevant_area = create_box(edge_row,edge_col);
   for(i=relevant_area.up;i<relevant_area.down+1;i++)
   {
      for(j=relevant_area.left;j<relevant_area.right+1;j++)
      {

         if(board[current_row+i][current_col+j]==fire)
         {
            return fire;
         }
      }
   }*/
   /*if no fires are found then should stay a tree*/
/*   return tree;
}*/


/*returns fire if one is found within 8 cell neighbourhood otherwise tree*/
state fire_in_area(int current_row, int current_col,const state board[][MAXCOLS],\
                  int num_rows,int num_cols)
{
   int i,j;

   for(i=BEHIND;i<=INFRONT;i++)
   {
      for(j=BEHIND;j<=INFRONT;j++)
      {
         if(!over_edge(current_row+i,current_col+j,num_rows,num_cols))
         {
            if(board[current_row+i][current_col+j]==fire)
            {
               return fire;
            }
         }
      }
   }
   /*if no fires are found then should stay a tree*/
   return tree;
}

/*dont want random numbers in the function so
going to have it as an argument*/
state empty_to_tree(int random)
{
   if(random==1)
   {
      return tree;
   }
   else
   {
      return empty;
   }
}

/*could combine this and above function
but looks quite neat like this and having
 different names for them
will hopefully lead to clearer code when
 putting them in another function*/
state tree_to_fire(int random)
{
   if(random==1)
   {
      return fire;
   }
   else
   {
      return tree;
   }
}

/*in this function to get around randoms im going to have a pointer
to a function as one of the arguments where in testing can point to
a hard coded number and then in main can point to a random num
although doing this sacrifices some readability but think its worth it*/
state trans_cell(int current_row, int current_col,const state board[][MAXCOLS],\
                  int num_rows,int num_cols,int (*random_generator)(int))
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
      if(fire_in_area(current_row,current_col,board,num_rows,num_cols)==fire)
      {
         return fire;
      }
      else
      {
         /*generates random number up to L*/
         random_num= random_generator(L);
         /*returns fire if random number is 1 else tree*/
         return tree_to_fire(random_num);
      }

      case(fire):
      return empty;

      default:
      printf("not a valid cell\n");
      exit(1);
   }
}


/*this function fills a target board from a previous board have to
 pass in function to generate random numbers as well- did this so i
 can pass a function that returns a known number for testing*/
void fill_board(const state previous[][MAXCOLS],state target[][MAXCOLS],\
                int num_rows,int num_cols,int (*random_generator)(int))
{
   int i,j;
   for(i=0;i<num_rows;i++)
   {
      for(j=0;j<num_cols;j++)
      {
         target[i][j]=trans_cell(i,j,previous,num_rows,num_cols,\
                                 random_generator);
      }
   }
}

/*test func for creating fires/trees not used in main program
needs to have int as argument so it is the same form as random_num_gen
but is unused*/
int test_generator_event(int placeholder)
{
   return 1;
}

/*test func for not creating fires/trees not used in main program
needs to have int as argument so it is the same form as random_num_gen
but is unused*/
int test_generator_no_event(int placeholder)
{
   return 2;
}



int random_num_gen(int max_num)
{
   int random_num;
   random_num = rand()% max_num+1;
   return random_num;
}

void copy_board(const state previous[][MAXCOLS],state target[][MAXCOLS],\
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
