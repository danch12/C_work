#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <string.h>
#include <assert.h>

typedef enum state {empty,head,tail,copper} state;
typedef enum edges {none,up,down,left,right} edges;

typedef struct box{
   int left;
   int right;
   int up;
   int down;
} box;

#define MAXCOLS 40
#define MAXROWS 40


#define BEHIND -1
#define INFRONT 1
#define INLINE 0

/*going to assume that everything that isnt on the board is empty
 so dont have to worry about it */


/*based on row position will return what edges you are on if any*/
edges on_edge_row(int position_row,int num_rows);
/*based on col position will return what edges you are on if any*/
edges on_edge_col(int position_col,int num_cols);

/*creates a kinda box that identifies area we need to count survived cells in*/
box create_box(edges edge_row,edges edge_col);

/*counts the cells around current to decide if the cell should
be copper or head*/
state copper_or_head(int position_row, int position_col, state board[][MAXCOLS],\
                  int num_rows,int num_cols);

/*given previous board and position returns a new cell based on rules*/
state trans_cell(int position_row, int position_col, state board[][MAXCOLS],\
            int num_rows,int num_cols);

/*this puts a board into a string so can easily test*/
void stringify(state board[][MAXCOLS],char target_str[],int num_rows,int num_cols);

/*function for testing other functions*/
void test(void);

int main(void)
{
   test();
   return 0;
}

void test(void)
{
   state test_board[][MAXCOLS]={{1,3,3,2},
                                 {1,0,0,0},
                                 {1,1,2,0},
                                 {0,1,1,2}};
   /*board is 40*40 + another 40 for the \ns and 1 for \0*/
   char test_str[1641];
   box test_box_1,test_box_2,test_box_3,test_box_4;
   /*only thing that this doesnt test for is if the board is
   1 col big or 1 row long - this because we get given a board size
   of 40 by 40 so dont have to worry about that*/
   assert(on_edge_row(0,100)==up);
   assert(on_edge_row(9,10)==down);
   assert(on_edge_row(5,50)==none);
   assert(on_edge_col(0,40)==left);
   assert(on_edge_col(39,40)==right);
   assert(on_edge_col(6, 40)==none);

   test_box_1=create_box(up,right); /*top right */
   test_box_2=create_box(left,left);
   test_box_3=create_box(none,left); /*should be the same as test box 2*/
   test_box_4=create_box(down,left);

   assert(test_box_1.up==INLINE);
   assert(test_box_3.left==INLINE);
   assert(test_box_2.left==INLINE);
   assert(test_box_2.down==test_box_3.down==INFRONT);
   assert(test_box_4.down==INLINE);
   assert(test_box_4.up==BEHIND);

   /*dont have to check what current cell is here
   as will do it in trans cell*/
   assert(copper_or_head(0,0,test_board,4,4)==head);
   assert(copper_or_head(3,3,test_board,4,4)==head);
   assert(copper_or_head(0,2,test_board,4,4)==copper);

   assert(trans_cell(0,2,test_board,4,4)==copper);
   assert(trans_cell(0,1,test_board,4,4)==head);
   assert(trans_cell(1,1,test_board,4,4)==empty);
   assert(trans_cell(0,0,test_board,4,4)==tail);
   stringify(test_board,test_str,4,4);
   printf("%s",test_str);

}

edges on_edge_row(int position_row,int num_rows)
{
   if(position_row==0)
   {
      return up;
   }
   if(position_row==num_rows-1)
   {
      return down;
   }
   else
   {
      return none;
   }
}

edges on_edge_col(int position_col,int num_cols)
{
   if(position_col==0)
   {
      return left;
   }
   if(position_col==num_cols-1)
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


state copper_or_head(int position_row, int position_col, state board[][MAXCOLS],\
                  int num_rows,int num_cols)
{
   int i,j,head_count;
   edges edge_row,edge_col;
   box relevant_area;
   head_count=0;
   edge_row = on_edge_row(position_row,num_rows);
   edge_col = on_edge_col(position_col,num_cols);
   relevant_area = create_box(edge_row,edge_col);
   for(i=relevant_area.up;i<relevant_area.down+1;i++)
   {
      for(j=relevant_area.left;j<relevant_area.right+1;j++)
      {

         if(board[position_row+i][position_col+j]==head)
         {
            head_count+=1;
         }
      }
   }
   if(head_count==1||head_count==2)
   {
      return head;
   }
   else
   {
      return copper;
   }
}


state trans_cell(int position_row, int position_col, state board[][MAXCOLS],\
                  int num_rows,int num_cols)
{
   state current_cell;
   current_cell=board[position_row][position_col];
   switch(current_cell)
   {
      case(empty):
      return empty;

      case(head):
      return tail;

      case(tail):
      return copper;

      case(copper):
      return copper_or_head(position_row,position_col,board,num_rows,num_cols);
      default:
      printf("not a valid cell\n");
      exit(1);
   }
}

/*this puts a board into a string so can easily test also lets me
  print out board after each iteration +can be tested*/
void stringify(state board[][MAXCOLS],char target_str[],int num_rows,int num_cols)
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
            target_str[board_pos]='.';
            board_pos++;
            break;
            case(head):
            target_str[board_pos]='H';
            board_pos++;
            break;
            case(tail):
            target_str[board_pos]='T';
            board_pos++;
            break;
            case(copper):
            target_str[board_pos]='C';
            board_pos++;
            break;
         }
      }
      target_str[board_pos]='\n';
      board_pos++;
   }
   target_str[board_pos]='\0';
}
