#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <string.h>
#include <assert.h>
#include "neillsimplescreen.h"

#define MAXLEN 50


typedef enum state{off, on} state;

void square_off(state board[][MAXLEN],int sq_size,int start_row,int start_col);
void print_board(state board[][MAXLEN],int num_rows,int num_cols);
void stringify_array_2d(state arr[][MAXLEN],int num_rows,int num_cols,char string[]);
void turn_on_board(state board[][MAXLEN],int num_rows,int num_cols);
void print_colour(char string[]);
void split_and_turn(state board[][MAXLEN],int orig_len,\
               int len,int start_row,int start_col);

void test(void);


int main(void)
{
   test();
   return 0;
}

void test(void)
{

   state test_board[27][MAXLEN]={{0}};
   char big_string[100000];
   turn_on_board(test_board,27,27);

   /*square_off(test_board,2,1,1);*/
   /*stringify_array_2d(test_board,8,8,big_string);*/
   /*assert(strcmp(big_string,"1111\n1001\n1001\n1111\n")==0);*/
   split_and_turn(test_board,27,27,0,0);
   stringify_array_2d(test_board,27,27,big_string);
   print_colour(big_string);

}


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

/*start pos is top left of square*/
void square_off(state board[][MAXLEN],int sq_size,int start_row,int start_col)
{
   int i,j,end_row,end_col;
   end_row=start_row+sq_size;
   end_col=start_col+sq_size;
   for(i=start_row;i<end_row;i++)
   {
      for(j=start_col;j<end_col;j++)
      {
         board[i][j]=off;
      }
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


void stringify_array_2d(state arr[][MAXLEN],int num_rows,int num_cols,char string[])
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

/*turn middle off then loop over surrounding squares passing in start and finish
coords and call the function on each of them*/


void split_and_turn(state board[][MAXLEN],int orig_len,\
               int len,int start_row,int start_col)
{
   int inner_sq_len,i,j;
   float inner_sq_len_fl;
   inner_sq_len_fl=(float)len/(float)3;

   inner_sq_len=inner_sq_len_fl;
   /*printf("current row= %d current col=%d\n",start_row,start_col);
   printf("inner sq len =%d as a float= %f\n",inner_sq_len,inner_sq_len_fl);*/
   if(inner_sq_len<1)
   {
      return;
   }
   else
   {
      /*printf("inner_sq_len= %d\n",inner_sq_len);

      printf("len =%d\n",len);*/
      square_off(board,inner_sq_len,start_row+inner_sq_len,start_col+inner_sq_len);
      print_board(board,orig_len,orig_len);
      printf("\n");
      for(i=start_row;i<len+start_row;i+=inner_sq_len)
      {
         for(j=start_col;j<len+start_col;j+=inner_sq_len)
         {
            /*printf("current row= %d current col=%d\n",i,j);*/
            split_and_turn(board,orig_len,inner_sq_len,i,j);
         }

      }
   }
}


void print_colour(char string[])
{
   int i;
   for(i=0;string[i]!='\0';i++)
   {

      if(string[i]=='1')
      {
         neillfgcol(black);
      }
      if(string[i]=='0')
      {
         neillfgcol(red);
      }
      if(string[i]=='\n')
      {
         printf("\n");
      }
      else
      {
         printf("#");
      }
   }
}
