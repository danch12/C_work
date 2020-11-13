#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <string.h>
#include <assert.h>

#include "neillsdl2.h"

#define MAXLEN 256
#define RECTSIZE 10

typedef enum state{off, on} state;

void turn_off(state board[][MAXLEN],int sq_size,int start_row,int start_col);
void print_board(state board[][MAXLEN],int num_rows,int num_cols);
void stringify_array_2d(state arr[][MAXLEN],int num_rows,\
                           int num_cols,char string[]);
void split_and_turn(state board[][MAXLEN],int orig_len,\
                     int len,int start_row,int start_col);
void turn_three(state board[][MAXLEN],int start_row,int start_col);
void turn_on_board(state board[][MAXLEN],int num_rows,int num_cols);
void draw_tri(state arr[][MAXLEN]);
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
   state big_board[MAXLEN][MAXLEN];
   char test_str[10000];
   turn_off(test_board,8,0,0);
   stringify_array_2d(test_board,8,8,test_str);

   assert(strcmp(test_str,"00111100\n00111100\n00111100\n00111100\n11111111\n11111111\n11111111\n11111111\n")==0);
   turn_on_board(test_board,8,8);

   turn_on_board(big_board,MAXLEN,MAXLEN);
   split_and_turn(big_board,MAXLEN,MAXLEN,0,0);
   print_board(big_board,MAXLEN,MAXLEN);
   draw_tri(big_board);
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
      /*print_board(board,orig_len,orig_len);
      printf("\n");*/
      printf("%d\n",half_square);
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

void draw_tri(state arr[][MAXLEN])
{
   int i,j,pos_x,pos_y;
   SDL_Simplewin sw;
   SDL_Rect rectangle;
   rectangle.w = RECTSIZE;
   rectangle.h = RECTSIZE;
   Neill_SDL_Init(&sw);
   

  for(i=0;i<MAXLEN;i++)
  {
     for(j=0;j<MAXLEN;j++)
     {
        if(arr[i][j]==0)
        {
           Neill_SDL_SetDrawColour(&sw, 255,
                                        255,
                                        255);
        }
        else
        {
           Neill_SDL_SetDrawColour(&sw, 0,
                                       0,
                                       0);
        }
        pos_x= MAXLEN-j;
        pos_y= MAXLEN-i;
        rectangle.x = (WWIDTH-RECTSIZE-256-pos_x);
        rectangle.y = (WHEIGHT-RECTSIZE-256-pos_y);
        SDL_RenderFillRect(sw.renderer, &rectangle);
        Neill_SDL_UpdateScreen(&sw);
     }
  }
  do
  {
    Neill_SDL_Events(&sw);
  }while(!sw.finished);
  SDL_Quit();
  atexit(SDL_Quit);

}
