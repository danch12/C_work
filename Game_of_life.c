#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <assert.h>

#define SELF 1
#define COLS 150
#define ROWS 90
#define BEHIND -1
#define INFRONT 1
#define INLINE 0

typedef enum bool {false, true} bool;
typedef enum status {dead,alive} status;
typedef enum edges {none,up,down,left,right} edges;

typedef struct box{
   int left;
   int right;
   int up;
   int down;
} box;

/*based on row position will return what edges you are on if any*/
edges on_edge_row(int position_row);

/*based on col position will return what edges you are on if any*/
edges on_edge_col(int position_col);

/*creates a box that identifies area we need to count survived cells in*/
box create_box(edges edge_row,edges edge_col);

/*returns count of alive cells in relevant area*/
int alive_counter(int position_row, int position_col, status board[][COLS]);

/*based on position return if alive cell survived or not*/
status survived(int alive_count);

/*based on position return if uninhabited cell is born or not*/
status birth(int alive_count);

/*copies board to target array*/
/*void copyboard(status to_copy[][COLS],status target[][COLS]);*/

/*fills board using previous iteration*/
/*void fillboard(status previous[][COLS],status target[][COLS])*/

void test(void);

int main(void)
{
   test();
   return 0;
}


void test(void)
{
   box test_box_1,test_box_2,test_box_3,test_box_4;
   int i,j;
   status test_board[ROWS][COLS];
   for(i=0;i<ROWS;i++)
   {
      for(j=0;j<COLS;j++)
      {
         test_board[i][j]=alive;
      }
   }


   assert(on_edge_row(0)==up);
   assert(on_edge_row(ROWS-1)==down);
   assert(on_edge_row(40)==none);
   assert(on_edge_col(0)==left);
   assert(on_edge_col(COLS-1)==right);
   assert(on_edge_col(12)==none);

   test_box_1=create_box(up,right); /*top right */
   test_box_2=create_box(left,left);
   test_box_3=create_box(left,none); /*should be the same as test box 2*/
   test_box_4=create_box(down,left);

   assert(test_box_1.up==INLINE);
   assert(test_box_2.left==test_box_3.left==INLINE && test_box_2.down==test_box_3.down==INFRONT);
   assert(test_box_4.down==INLINE);



   assert(alive_counter(0,0,test_board)==3);
   assert(alive_counter(10,10,test_board)==8);
   assert(alive_counter(0,10,test_board)==5);

   /*positions 0 indexed so have to take 1*/
   assert(alive_counter(ROWS-1,20,test_board)==5);
   assert(alive_counter(20,COLS-1,test_board)==5);
   /*need more tests when i can create boards easy*/

   assert(survived(10)==dead);
   assert(survived(2)==alive);
}

edges on_edge_row(int position_row)
{
   if(position_row==0)
   {
      return up;
   }
   if(position_row==ROWS-1)
   {
      return down;
   }
   else
   {
      return none;
   }
}

edges on_edge_col(int position_col)
{
   if(position_col==0)
   {
      return left;
   }
   if(position_col==COLS-1)
   {
      return right;
   }
   else
   {
      return none;
   }
}

/*creates a box that identifies area we need to count survived cells in*/
box create_box(edges edge_row,edges edge_col)
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
}

int alive_counter(int position_row, int position_col, status board[][COLS])
{
   int i,j,alive_count;
   edges edge_row,edge_col;
   box relevant_area;
   alive_count=0;

   edge_row = on_edge_row(position_row);
   edge_col = on_edge_col(position_col);

   relevant_area = create_box(edge_row,edge_col);
   for(i=relevant_area.up;i<relevant_area.down+1;i++)
   {
      for(j=relevant_area.left;j<relevant_area.right+1;j++)
      {

         if(board[position_row+i][position_col+j]==alive)
         {

            alive_count+=1;
         }
      }
   }
   /*need to take away yourself*/
   return alive_count-SELF;
}

status survived(int alive_count)
{
   if(alive_count==2||alive_count==3)
   {
      return alive;
   }
   else
   {
      return dead;
   }
}

status birth(int alive_count)
{
   if(alive_count==3)
   {
      return alive;
   }
   else
   {
      return dead;
   }
}


void fillboard(status previous[][COLS],status target[][COLS])
{
   int i,j;
   status new_cell;
   for(i=0;i<ROWS;i++)
   {
      for(j=0;<COLS;j++)
      {
         if(previous[i][j]==alive)
         {
            new_cell=survived(alive_counter(i,j,previous));
         }
         else
         {
            new_cell=birth(alive_counter(i,j,previous));
         }
         target[i][j]=new_cell;
      }
   }
}

void init_board(status tofill[][COLS], char* filename)
{
   FILE *fp;
   int middle_row_index;
   int middle_column_index;
   middle_row_index= (ROWS-1)/2;
   middle_column_index = (COLS-1)/2;

   fp= fopen(filename,"r");
   while(fgets())
}
