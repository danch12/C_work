#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <string.h>
#include <assert.h>

#define MAXLEN 100

typedef enum state{off, on} state;

void turn_off(state board[][MAXLEN],int sq_size,int start_row,int start_col);
void print_board(state board[][MAXLEN],int num_rows,int num_cols);
void stringify_array_2d(state arr[][MAXLEN],int num_rows,\
                           int num_cols,char string[]);
void split_and_turn(state board[][MAXLEN],int orig_len,\
                     int len,int start_row,int start_col);
void turn_three(state board[][MAXLEN],int start_row,int start_col);
void turn_on_board(state board[][MAXLEN],int num_rows,int num_cols);
void test(void);


int main(void)
{
   test();
   return 0;
}

void test(void)
{
   state test_board[][MAXLEN]={{1,1,1,1,1,1,1,1},
                              {1,1,1,1,1,1,1,1},
                               {1,1,1,1,1,1,1,1},
                               {1,1,1,1,1,1,1,1},
                              {1,1,1,1,1,1,1,1},
                              {1,1,1,1,1,1,1,1},
                              {1,1,1,1,1,1,1,1},
                              {1,1,1,1,1,1,1,1},};
   state big_board[100][100];
   char test_str[10000];
   turn_off(test_board,8,0,0);
   stringify_array_2d(test_board,8,8,test_str);

   assert(strcmp(test_str,"00111100\n00111100\n00111100\n00111100\n11111111\n11111111\n11111111\n11111111\n")==0);
   turn_on_board(test_board,8,8);

   turn_on_board(big_board,100,100);
   split_and_turn(big_board,100,100,0,0);
   print_board(big_board,100,100);
}


/*void turn_off(state board[][MAXLEN],int orig_size,int start_row,int start_col)
{
   int i,j,end_row,end_col;

   for(i=start_row;i<end_row;i++)
   {
      for(j=start_col;j<end_col;j++)
      {
         board[i][j]=off;
      }
   }
}*/
void turn_on_board(state board[][MAXLEN],int num_rows,int num_cols)
{
   int i,j;
   for(i=0;i<num_rows;i++)
   {
      for(j=0;j<num_cols;j++)
      {
         board[i][j]=on;
      }

   }
}



void turn_off(state board[][MAXLEN],int orig_size,int start_row,int start_col)
{
   int half_square,margins,i,j;
   /*want to cut top half of orig square into two squares
   but one square has to be in middle
   so take half a square from each side*/
   half_square= orig_size/2;
   margins = half_square/2;
   for(i=0;i<half_square;i++)
   {
      for(j=0;j<orig_size;j++)
      {
         /*skip middle*/
         if(j<margins||j>=margins+half_square)
         {
            board[i+start_row][j+start_col]=off;
         }
      }
   }
}

void split_and_turn(state board[][MAXLEN],int orig_len,\
               int len,int start_row,int start_col)
{
   int half_square,margins;
   half_square=len/2;
   margins = half_square/2;

   if(half_square<2)
   {
      return;
   }
   else
   {

      turn_off(board,len,start_row,start_col);
      print_board(board,orig_len,orig_len);
      printf("\n");
      split_and_turn(board,orig_len,half_square,start_row,start_col+margins);
      split_and_turn(board,orig_len,half_square,start_row+half_square,start_col);
      split_and_turn(board,orig_len,half_square,\
                     start_row+half_square,start_col+half_square);
   }
}

void print_board(state board[][MAXLEN],int num_rows,int num_cols)
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


void stringify_array_2d(state arr[][MAXLEN],int num_rows,\
                           int num_cols,char string[])
{
   int i,j,pos;
   pos=0;
   for(i=0;i<num_rows;i++)
   {
      for(j=0;j<num_cols;j++)
      {
         string[pos]=arr[i][j]+'0';
         pos++;
      }
      string[pos]='\n';
      pos++;
   }
   string[pos]='\0';
}
