#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <string.h>
#include <assert.h>

#define MAXROWS 8
#define MAXCOLS 7
#define INITSIZE 10
#define SCALEFACTOR 2

typedef enum state {off,on} state;
typedef enum bool {false, true} bool;

typedef struct element{
   state board[MAXROWS][MAXCOLS];
   int num_children;
   struct element* parent;
}element;



typedef struct board_cont{
   state board[MAXROWS][MAXCOLS];
   int num_soldiers;
   struct board_cont* parent;
}board_cont;

typedef struct board_arr{
   int size;
   int capacity;
   board_cont** array;
}board_arr;

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

board_cont* make_move(board_cont* parent_board,int start_row, \
            int start_col,int end_row,int end_col,int num_rows,int num_cols);
int count_soldiers(state new_board[MAXROWS][MAXCOLS],int num_rows,int num_cols);
link create_moves(link parent_board,int num_rows,int num_cols,\
                  board_arr* seen_boards);
void free_final(link child);
bool same_array_2d(state board_1[][MAXCOLS],state board_2[][MAXCOLS],\
                     int num_rows,int num_cols);

bool seen_board(board_arr* seen_boards, state current_board[MAXROWS][MAXCOLS],\
             int num_rows,int num_cols,int soldiers_current);

board_cont* go_through_list(board_arr* seen_boards,state origin_board[MAXROWS][MAXCOLS],\
                int num_rows,int num_cols);
void add_board(board_arr* seen_boards,state new_board[MAXROWS][MAXCOLS],\
             board_cont* parent_board,int num_rows,int num_cols);
void free_offshoot(link child);
void moves_board(board_arr* seen_boards,board_cont* current_board,\
                  int num_rows,int num_cols);
void moves_square(board_arr* seen_boards,board_cont* current_board,int start_row,
                  int start_col,int num_rows,int num_cols);
void free_container_arr(board_arr* to_free);
void add_container(board_arr* seen_boards,board_cont* to_add);
board_arr* init_container_arr(void);
int num_of_zero_rows(state board[MAXROWS][MAXCOLS],int num_rows,int num_cols);
bool impossible_to_win(board_cont* current_board,int num_rows,int num_cols);


void test(void);

int main(void)
{
   test();
   return 0;
}



void test(void)
{

   int i,j;
   board_arr* test_container;
   board_arr* test_container2;
   board_cont* test_cont,*winning_cont;
   board_cont *move_test_legal;
   state test_board[MAXROWS][MAXCOLS]={{0,1,1,0},
                                       {0,0,0,0},
                                       {1,1,1,1},
                                       {1,1,1,1}};


   state test_board_2[MAXROWS][MAXCOLS]={{0,0,0,0,0,0,0},
                                         {0,0,0,0,0,0,0},
                                         {0,0,0,0,0,0,0},
                                         {0,0,0,0,0,0,0},
                                         {1,1,1,1,1,1,1},
                                         {1,1,1,1,1,1,1},
                                         {1,1,1,1,1,1,1},
                                         {1,1,1,1,1,1,1}};

   state moved_board[MAXROWS][MAXCOLS]={{0,0,0,0,0,0,0},
                                        {0,0,0,0,0,0,0},
                                        {0,0,0,0,0,0,0},
                                        {1,0,0,0,0,0,0},
                                        {0,1,1,1,1,1,1},
                                        {0,1,1,1,1,1,1},
                                        {1,1,1,1,1,1,1},
                                        {1,1,1,1,1,1,1}};

   state not_seen_board[MAXROWS][MAXCOLS]={{0,0,0,0,0,0,0},
                                      {0,0,0,0,0,0,0},
                                      {0,0,0,0,0,0,0},
                                      {1,0,0,0,0,0,0},
                                      {0,1,1,1,1,1,0},
                                      {0,1,1,1,1,1,1},
                                      {1,1,1,1,1,1,1},
                                      {1,1,1,1,1,1,1}};

   test_cont=(board_cont*)malloc(sizeof(board_cont));

   memcpy(test_cont->board,test_board_2,MAXROWS*MAXCOLS*sizeof(int));
   test_cont->num_soldiers=count_soldiers(test_cont->board,8,7);
   test_cont->parent=NULL;
   assert(legal_move(test_board,3,1,1,1,4,4)==true);
   assert(legal_move(test_board,0,1,0,3,4,4)==true);

   assert(legal_move(test_board,0,0,0,1,4,4)==false);
   assert(legal_move(test_board,2,0,4,0,4,4)==false);
   assert(legal_move(test_board,2,0,4,0,4,4)==false);
   assert(legal_move(test_board,3,0,1,3,4,4)==false);
   assert(legal_move(test_board,3,1,3,-1,4,4)==false);
   assert(has_won(test_board,4)==true);
   assert(has_won(test_board_2,4)==false);

   assert(count_soldiers(test_board,4,4)==10);
   assert(count_soldiers(test_board_2,1,7)==0);
   /*container tests*/

   test_container=init_container_arr();
   assert(test_container->size==0);



   /*void add_board(board_arr* seen_boards,state new_board[MAXROWS][MAXCOLS],\
                  board_cont* parent_board,int num_rows,int num_cols)*/

   /*board_cont* make_move(board_cont* parent_board,int start_row, \
               int start_col,int end_row,int end_col,int num_rows,int num_cols)*/
   add_board(test_container,test_board_2,NULL,8,7);
   assert(test_container->size==1);
   assert(count_soldiers(test_container->array[0]->board,8,7)\
         ==count_soldiers(test_board_2,8,7));

   /*add_container(board_arr* seen_boards,board_cont* to_add)*/
   add_container(test_container,test_cont);
   assert(test_container->size==2);
   assert(count_soldiers(test_container->array[0]->board,8,7)\
                        ==count_soldiers(test_board_2,8,7));



   /*not checking for illegal moves here*/
   move_test_legal=make_move(test_container->array[0],5,0,3,0,8,7);
   add_container(test_container,move_test_legal);
   assert(move_test_legal->board[5][0]==0);
   assert(move_test_legal->board[4][0]==0);
   assert(move_test_legal->board[3][0]==1);

   assert(num_of_zero_rows(test_board_2,8,7)==4);
   assert(num_of_zero_rows(test_board,4,4)==0);



   assert(seen_board(test_container,moved_board,8,7,27)==true);
   assert(seen_board(test_container,not_seen_board,8,7,26)==false);


   test_container2=init_container_arr();
   winning_cont=go_through_list(test_container2,test_board_2,8,7);
   while(winning_cont!=NULL)
   {
      print_2d_arr(winning_cont->board,8,7);
      printf("\n\n");
      winning_cont=winning_cont->parent;
   }
   /*free_container(test_container);*/
   /*ll stuff*/





   /*test_link2=make_move(test_link,5,1,3,1,8,7);
   assert(test_link->board[5][1]==1);
   assert(test_link2->board[5][1]==0);
   assert(test_link->board[3][1]==0);
   assert(test_link2->board[3][1]==1);*/



   /*test_link3=create_moves(test_link,8,7,test_container);
   print_boards(test_link3,8,7);
   print_2d_arr(test_link3->parent->board,4,4);*/


   /*free_final(test_link3);*/

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


bool same_array_2d(state board_1[][MAXCOLS],state board_2[][MAXCOLS],\
                     int num_rows,int num_cols)
{
   int i,j;
   for(i=0;i<num_rows;i++)
   {
      for(j=0;j<num_cols;j++)
      {
         if(board_1[i][j]!=board_2[i][j])
         {
            return false;
         }
      }
   }
   return true;
}


bool seen_board(board_arr* seen_boards, state current_board[MAXROWS][MAXCOLS],\
                int num_rows,int num_cols,int soldiers_current)
{
   int i;
   for(i=0;i<seen_boards->size;i++)
   {
      if(soldiers_current==seen_boards->array[i]->num_soldiers)
      {
         if(same_array_2d(current_board,seen_boards->array[i]->board,\
                           num_rows,num_cols))
         {
            return true;
         }
      }

   }
   return false;
}

void copy_board(state to_copy[MAXROWS][MAXCOLS],\
               state target[MAXROWS][MAXCOLS],int num_rows,int num_cols)
{
   int i,j;
   for(i=0;i<num_rows;i++)
   {
      for(j=0;j<num_cols;j++)
      {
         target[i][j]=to_copy[i][j];
      }
   }
}

board_arr* init_container_arr(void)
{
   board_arr* b;
   b=(board_arr*)calloc(1,sizeof(board_arr));
   if(b==NULL)
   {
      fprintf(stderr,"no space\n");
   	exit(EXIT_FAILURE);
   }
   b->array=(board_cont**)calloc(INITSIZE,sizeof(board_cont));
   if(b->array==NULL)
   {
      fprintf(stderr,"no space\n");
   	exit(EXIT_FAILURE);
   }
   b->size=0;
   b->capacity =INITSIZE;
   return b;
}


void free_container_arr(board_arr* to_free)
{
   int i;
   /*less than size as we increase size after we add*/
   for(i=0;i<to_free->size;i++)
   {
      free(to_free->array[i]);
   }
   free(to_free->array);
   free(to_free);
}

int count_soldiers(state new_board[MAXROWS][MAXCOLS],\
                  int num_rows,int num_cols)
{
   int i,j,count;
   count=0;
   for(i=0;i<num_rows;i++)
   {
      for(j=0;j<num_cols;j++)
      {
         count+=new_board[i][j];
      }
   }
   return count;
}

void add_board(board_arr* seen_boards,state new_board[MAXROWS][MAXCOLS],\
               board_cont* parent_board,int num_rows,int num_cols)
{
   board_cont* new_board_cont;
   new_board_cont=(board_cont*)malloc(sizeof(board_cont));
   copy_board(new_board,new_board_cont->board,num_rows,num_cols);
   new_board_cont->num_soldiers=count_soldiers(new_board,num_rows,num_cols);
   new_board_cont->parent= parent_board;
   seen_boards->array[seen_boards->size]=new_board_cont;
   seen_boards->size+=1;
   if(seen_boards->size==seen_boards->capacity)
   {
      seen_boards->array=(board_cont**)realloc(seen_boards->array,\
                  sizeof(board_cont)*seen_boards->capacity*SCALEFACTOR);
      if(seen_boards->array==NULL)
      {
         fprintf(stderr,"no space\n");
   	 	exit(EXIT_FAILURE);
      }
      seen_boards->capacity=seen_boards->capacity*SCALEFACTOR;
   }
}


void add_container(board_arr* seen_boards,board_cont* to_add)
{
   seen_boards->array[seen_boards->size]=to_add;
   seen_boards->size+=1;
   if(seen_boards->size==seen_boards->capacity)
   {
      seen_boards->array=(board_cont**)realloc(seen_boards->array,\
                  sizeof(board_cont)*seen_boards->capacity*SCALEFACTOR);
      if(seen_boards->array==NULL)
      {
         fprintf(stderr,"no space\n");
   	 	exit(EXIT_FAILURE);
      }
      seen_boards->capacity=seen_boards->capacity*SCALEFACTOR;
   }
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


board_cont* go_through_list(board_arr* seen_boards,state origin_board[MAXROWS][MAXCOLS],\
                  int num_rows,int num_cols)
{
   int i;
   add_board(seen_boards,origin_board,NULL,num_rows,num_cols);

   for(i=0;i<seen_boards->size;i++)
   {
      
      if(has_won(seen_boards->array[i]->board,num_cols))
      {
         return seen_boards->array[i];
      }
      else
      {
         moves_board(seen_boards,seen_boards->array[i],num_rows,num_cols);
      }
   }
   return NULL;

}

void moves_board(board_arr* seen_boards,board_cont* current_board,\
                  int num_rows,int num_cols)
{
   int start_row, start_col;


   for(start_row=0;start_row<num_rows;start_row++)
   {
      for(start_col=0;start_col<num_cols;start_col++)
      {
         moves_square(seen_boards,current_board,\
                     start_row,start_col,num_rows,num_cols);
      }
   }


}

void moves_square(board_arr* seen_boards,board_cont* current_board,int start_row,
                  int start_col,int num_rows,int num_cols)
{
   int i,j,end_row,end_col;
   board_cont* child_b;
   for(i=-2;i<3;i++)
   {
      for(j=-2;j<3;j++)
      {
         end_row=start_row+i;
         end_col=start_col+j;
         if(legal_move(current_board->board,start_row,\
                     start_col,end_row,end_col,num_rows,num_cols))
         {
            child_b=make_move(current_board,start_row,\
                           start_col,end_row,end_col,num_rows,num_cols);
            if((!seen_board(seen_boards,child_b->board,\
                     num_rows,num_cols,child_b->num_soldiers))&&\
                  !impossible_to_win(child_b,num_rows,num_cols))
            {
               add_container(seen_boards,child_b);
            }
         }
      }
   }
}





board_cont* make_move(board_cont* parent_board,int start_row, \
            int start_col,int end_row,int end_col,int num_rows,int num_cols)
{
   int i,j,mid_point_row,mid_point_col;
   board_cont* new_move_board;
   new_move_board=(board_cont*)malloc(sizeof(board_cont));
   new_move_board->parent=parent_board;

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
   new_move_board->num_soldiers=count_soldiers(new_move_board->board,\
                                          num_rows,num_cols);
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

bool impossible_to_win(board_cont* current_board,int num_rows,int num_cols)
{
   int zero_rows;
   zero_rows=num_of_zero_rows(current_board->board,num_rows,num_cols);
   if(current_board->num_soldiers>=20)
   {
      if(zero_rows>=5)
      {
         return true;
      }
      return false;
   }
   if(current_board->num_soldiers>=8)
   {
      if(zero_rows>3)
      {
         return true;
      }
      return false;
   }
   if(current_board->num_soldiers>=4)
   {
      if(zero_rows>2)
      {
         return true;
      }
      return false;
   }
   if(current_board->num_soldiers>=2)
   {
      if(zero_rows>1)
      {
         return true;
      }
      return false;
   }
   return false;
}

int num_of_zero_rows(state board[MAXROWS][MAXCOLS],int num_rows,int num_cols)
{
   int i,j;
   int count;
   count=0;
   for(i=0;i<num_rows;i++)
   {
      for(j=0;j<num_cols;j++)
      {
         if(board[i][j]==1)
         {
            return count;
         }
      }
      count+=1;
   }
   return count;
}

/*void free_final(link child)
{
   link temp;
   while(child!=NULL)
   {
      temp=child;
      child=child->parent;
      free(temp);
   }
}
*/


/*slow and hard to make into smaller functions-->bad*/
/*link create_moves(link parent_board,int num_rows,int num_cols,\
                  board_arr* seen_boards)
{
   link child,temp;
   int i,j,end_row,end_col;
   int start_row, start_col;
   int num_soldiers;

   if(has_won(parent_board->board,num_cols))
   {
      print_2d_arr(parent_board->board,num_rows,num_cols);
      printf("\n\n");
      return parent_board;
   }
   num_soldiers=count_soldiers(parent_board->board,num_rows,num_cols);

   if(!seen_board(seen_boards,parent_board->board,num_rows,num_cols,num_soldiers))
   {*/
      /*go through whole board*/
   /*   for(start_row=0;start_row<num_rows;start_row++)
      {
         for(start_col=0;start_col<num_cols;start_col++)
         {*/
            /*go through each move for each square*/
            /*should prob put this block in seperate func*/
         /*   for(i=-2;i<3;i++)
            {
               for(j=-2;j<3;j++)
               {
                  end_row=start_row+i;
                  end_col=start_col+j;
                  if(legal_move(parent_board->board,start_row,\
                              start_col,end_row,end_col,num_rows,num_cols))
                  {

                     child=make_move(parent_board,start_row,start_col,\
                                       end_row,end_col,num_rows,num_cols);

                     if(!seen_board(seen_boards,child->board,num_rows,num_cols,num_soldiers))
                     {

                        temp=create_moves(child,num_rows,num_cols,seen_boards);*/
                        /*check nulls*/
                     /*   if(temp)
                        {*/
                           /*check if a child has won*/
                        /*   if(has_won(temp->board,num_cols))
                           {
                              return temp;
                           }

                        }
                     }




                  }
               }
            }
         }
      }
   }*/


   /*freeing offshoots that go nowhere-
      does this free far enough?*/
   /*if(!has_won(parent_board->board,num_cols)&&parent_board->num_children==0)
   {
      if(!seen_board(seen_boards,parent_board->board,num_rows,num_cols,num_soldiers))
      {
         add_board(seen_boards,parent_board->board,num_rows,num_cols);
      }
      parent_board->parent->num_children-=1;
      free(parent_board);
   }
   return NULL;
}*/
