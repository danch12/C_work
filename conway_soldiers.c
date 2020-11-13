#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <string.h>
#include <assert.h>

#define MAXROWS 8
#define MAXCOLS 7


typedef enum state {off,on} state;
typedef enum bool {false, true} bool;

typedef struct element{
   state board[MAXROWS][MAXCOLS];
   int num_children;
   struct element* parent;
}element;

typedef element* link;

bool legal_move(state board[MAXROWS][MAXCOLS],int start_row, \
               int start_col,int end_row,int end_col,\
               int num_rows,int num_cols);

bool grouped_inbounds(int start_row, int start_col,int end_row,\
                     int end_col,int num_rows,int num_cols);
bool has_won(state board[MAXROWS][MAXCOLS],int num_cols);
bool inbounds(int x,int max_size);
void print_2d_arr(state board[MAXROWS][MAXCOLS],int num_rows,int num_cols);
void print_boards(link child,int num_rows,int num_cols);
link make_move(link parent_board,int start_row, \
            int start_col,int end_row,int end_col,int num_rows,int num_cols);
link create_moves(link parent_board,int num_rows,int num_cols);
void free_final(link child);
/*bool same_array_2d(const state board1[MAXROWS][MAXCOLS],\
    const state board2[MAXROWS][MAXCOLS],int num_rows,int num_cols);*/
void free_offshoot(link child);
void test(void);

int main(void)
{
   test();
   return 0;
}



void test(void)
{
   link test_link,test_link2,test_link3;
   int i,j;
   state test_board[MAXROWS][MAXCOLS]={{0,1,1,0},
                                       {0,0,0,0},
                                       {1,1,1,1},
                                       {1,1,1,1}};


   state test_board_2[MAXROWS][MAXCOLS]={{0,0,0,0},
                                         {0,0,0,0},
                                         {1,1,1,1},
                                         {1,1,1,1}};
   test_link=(link)malloc(sizeof(element));

   test_link->num_children=0;
   test_link->parent=NULL;
   for(i=0;i<4;i++)
   {
      for(j=0;j<4;j++)
      {
         test_link->board[i][j]=test_board_2[i][j];
      }
   }
   assert(legal_move(test_board,3,1,1,1,4,4)==true);
   assert(legal_move(test_board,0,1,0,3,4,4)==true);

   assert(legal_move(test_board,0,0,0,1,4,4)==false);
   assert(legal_move(test_board,2,0,4,0,4,4)==false);
   assert(legal_move(test_board,2,0,4,0,4,4)==false);
   assert(legal_move(test_board,3,0,1,3,4,4)==false);
   assert(legal_move(test_board,3,1,3,-1,4,4)==false);
   assert(has_won(test_board,4)==true);

   /*ll stuff*/

   test_link2=make_move(test_link,3,1,1,1,4,4);
   assert(test_link->board[3][1]==1);
   assert(test_link2->board[3][1]==0);

   test_link3=create_moves(test_link,4,4);
   print_boards(test_link3,4,4);
   /*print_2d_arr(test_link3->parent->board,4,4);*/

   free(test_link2);
   free_final(test_link3);

}

bool inbounds(int x,int max_size)
{
   if(x>=0&&x<max_size)
   {
      return true;
   }
   return false;
}

/*grouped the inbounds to make it cleaner in legal function*/
bool grouped_inbounds(int start_row, int start_col,int end_row,\
                     int end_col,int num_rows,int num_cols)
{
   if(!inbounds(start_col, num_cols))
   {
      return false;
   }
   if(!inbounds( end_col, num_cols))
   {
      return false;
   }
   if(!inbounds( start_row, num_rows))
   {
      return false;
   }
   if(!inbounds( end_row, num_rows))
   {
      return false;
   }
   return true;
}

bool legal_move(state board[MAXROWS][MAXCOLS],int start_row, \
               int start_col,int end_row,int end_col,int num_rows,int num_cols)
{
   int mid_point_row, mid_point_col;

   if(!grouped_inbounds(start_row,start_col,end_row,\
      end_col,num_rows,num_cols))
   {
      return false;
   }
   if(((abs(start_col-end_col)==2) && (start_row==end_row))||\
      ((abs(start_row-end_row)==2) && (start_col==end_col)))
   {
      if(abs(start_col-end_col)==2)
      {
         mid_point_row=start_row;
         mid_point_col = (start_col>end_col) ? start_col-1 : start_col+1;
      }
      if(abs(start_row-end_row)==2)
      {
         mid_point_col=start_col;
         mid_point_row = (start_row>end_row) ? start_row-1 : start_row+1;
      }

      if((board[end_row][end_col]==off&&\
         board[start_row][start_col]==on)&&\
         board[mid_point_row][mid_point_col]==on)
      {
         return true;
      }
   }

   return false;
}




bool has_won(state board[MAXROWS][MAXCOLS],int num_cols)
{
   int i;
   for(i=0;i<num_cols;i++)
   {
      if(board[0][i]==on)
      {
         return true;
      }
   }
   return false;
}

/*bool same_array_2d(const state board1[MAXROWS][MAXCOLS],\
    const state board2[MAXROWS][MAXCOLS],int num_rows,int num_cols)
{
   int i,j;
   for(i=0;i<num_rows;i++)
   {
      for(j=0;j<num_cols;j++)
      {
         if(board1[i][j]!=board2[i][j])
         {
            return false;
         }
      }
   }
   return true;
}*/



/*bool parent_duplicate(link current_board,int num_rows,int num_cols)
{
   link temp;
   temp=current_board;
   while(current_board!=NULL)
   {
      if()
   }
}*/


/*so when we only have a link to the parent may be hard to free some elements that
lead nowhere so having a num children count and then when we reach a dead end
can just free up all the elements with only one child*/

link create_moves(link parent_board,int num_rows,int num_cols)
{
   link child,temp;
   int i,j,count,end_row,end_col;
   int start_row, start_col;
   if(has_won(parent_board->board,num_cols))
   {
      return parent_board;
   }

   count=0;
   for(start_row=0;start_row<num_rows;start_row++)
   {
      for(start_col=0;start_col<num_cols;start_col++)
      {
         for(i=-2;i<3;i++)
         {
            for(j=-2;j<3;j++)
            {
               end_row=start_row+i;
               end_col=start_col+j;
               if(legal_move(parent_board->board,start_row,\
                           start_col,end_row,end_col,num_rows,num_cols))
               {
                  count+=1;
                  child=make_move(parent_board,start_row,start_col,\
                                    end_row,end_col,num_rows,num_cols);

                  temp=create_moves(child,num_rows,num_cols);
                  /*check nulls*/
                  if(temp)
                  {
                     /*check if a child has won*/
                     if(has_won(temp->board,num_cols))
                     {
                        return temp;
                     }

                  }


               }
            }
         }
      }
   }
   /*freeing offshoots that go nowhere-
      does this free far enough?*/
   if(!has_won(parent_board->board,num_cols)&&parent_board->num_children==0)
   {
      parent_board->parent->num_children-=1;
      free(parent_board);
   }
   return NULL;
}



link make_move(link parent_board,int start_row, \
            int start_col,int end_row,int end_col,int num_rows,int num_cols)
{
   int i,j,mid_point_row,mid_point_col;
   link new_move_board;
   new_move_board=(link)malloc(sizeof(element));
   new_move_board->parent=parent_board;
   new_move_board->num_children=0;
   parent_board->num_children+=1;

   for(i=0;i<num_rows;i++)
   {
      for(j=0;j<num_cols;j++)
      {
         new_move_board->board[i][j]=parent_board->board[i][j];
      }
   }

   if(end_row==start_row)
   {
      mid_point_row=start_row;
      mid_point_col = (start_col>end_col) ? start_col-1 : start_col+1;
   }
   else
   {
      mid_point_col=start_col;
      mid_point_row = (start_row>end_row) ? start_row-1 : start_row+1;
   }
   new_move_board->board[start_row][start_col]=0;
   new_move_board->board[mid_point_row][mid_point_col]=0;
   new_move_board->board[end_row][end_col]=1;

   return new_move_board;
}

void print_boards(link child,int num_rows,int num_cols)
{
   int count;
   count=0;
   while(child!=NULL)
   {
      count+=1;
      printf("iter %d\n",count);
      print_2d_arr(child->board,num_rows,num_cols);
      printf("\n\n");
      child=child->parent;
   }
}


void print_2d_arr(state board[MAXROWS][MAXCOLS],int num_rows,int num_cols)
{
   int i,j;
   for(i=0;i<num_rows;i++)
   {
      for(j=0;j<num_cols;j++)
      {
         printf("%d",board[i][j]);
      }
      printf("\n");
   }
}


void free_final(link child)
{
   link temp;
   while(child!=NULL)
   {
      temp=child;
      child=child->parent;
      free(temp);
   }
}
