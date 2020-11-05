#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <string.h>
#include <assert.h>

typedef enum bool {false,true} bool;

typedef struct grid{
   int rows;
   int cols;
} grid;

#define BUFFERSIZE 1000


#define ROWS 20
#define COLS 20
#define EXIT 3
#define WALL 2
#define PATH 1
#define BEEN 0

bool inbounds(int x,int size);
void test(void);
bool can_go(int x, int y,int maze[ROWS][COLS], int num_rows,int num_cols);
int explore(int x,int y, int maze[ROWS][COLS], int num_rows,int num_cols);
int switch_input(char symbol);
grid read_in_maze(char* filename, int board[][COLS]);

int main(void)
{
   test();
   return 0;
}

void test(void)
{
   int test_maze[ROWS][COLS]={{2,2,1,2},
                              {2,1,1,2},
                              {3,1,2,2}};
   int empty_board[ROWS][COLS]={{0}};
   int i,j;
   grid test_grid;
   assert(inbounds(50,5)==false);
   assert(inbounds(-1,4)==false);
   assert(inbounds(1,5)==true);
   assert(inbounds(0,5)==true);
   assert(switch_input('#')==WALL);
   assert(switch_input(' ')==PATH);

   explore(0,2,test_maze,4,4);
   test_grid=read_in_maze("maze.txt",empty_board);
   assert(test_grid.cols==10);
   assert(test_grid.rows==10);

   for(i=0;i<10;i++)
   {
      for(j=0;j<10;j++)
      {
         printf("%d",empty_board[i][j]);
      }
      printf("\n");
   }

}

grid read_in_maze(char* filename, int board[][COLS])
{
   FILE *ifp;
   char string_store[100];
   char size_maze[50];
   char symbol;
   int i,j,pos;
   int rows,cols;
   grid size_of_grid;
   /*going to assume all maze files the same*/
   rows=0;
   cols=0;
   if((ifp=fopen(filename,"r"))==NULL)
   {
      fprintf(stderr,"cant open\n");
      exit(EXIT_FAILURE);
   }
   i=0;
   if((fgets(size_maze,BUFFERSIZE,ifp)==NULL))
   {
      fprintf(stderr,"cant read\n");
      exit(EXIT_FAILURE);
   }
   sscanf(size_maze,"%d %d",&rows,&cols);
   while((symbol=fgetc(ifp))!=EOF)
   {
      if(symbol=='#'||symbol==' ')
      {
         string_store[i]=symbol;
         i++;
      }
   }
   pos=0;
   for(i=0;i<rows;i++)
   {
      for(j=0;j<cols;j++)
      {
         board[i][j]=switch_input(string_store[pos]);
         pos++;
      }
   }

   if(fclose(ifp)!=0)
   {
      fprintf(stderr,"not exited\n");
      exit(EXIT_FAILURE);
   }
   size_of_grid.rows=rows;
   size_of_grid.cols=cols;
   return size_of_grid;
}


int switch_input(char symbol)
{
   switch(symbol)
   {
      case(' '):
      return PATH;
      case('#'):
      return WALL;
      default:
      fprintf(stderr,"%c not valid cell\n",symbol);
      exit(EXIT_FAILURE);
   }
}


bool inbounds(int x,int size)
{
   if(x>=0&&x<size)
   {
      return true;
   }
   return false;
}

int explore(int x,int y, int maze[ROWS][COLS], int num_rows,int num_cols)
{
   printf("%d\n",maze[x][y]);
   if(maze[x][y]==EXIT)
   {
      return 1;
   }

   maze[x][y]=BEEN;
   if(can_go(x-1,y,maze,num_rows,num_cols))
   {
      if(explore(x-1,y,maze,num_rows,num_cols))
      {
         return 1;
      }
   }
   if(can_go(x+1,y,maze,num_rows,num_cols))
   {
      if(explore(x+1,y,maze,num_rows,num_cols))
      {
         return 1;
      }
   }
   if(can_go(x,y-1,maze,num_rows,num_cols))
   {
      if(explore(x,y-1,maze,num_rows,num_cols))
      {
         return 1;
      }
   }
   if(can_go(x,y+1,maze,num_rows,num_cols))
   {
      if(explore(x,y+1,maze,num_rows,num_cols))
      {
         return 1;
      }
   }
   return 0;
}

bool can_go(int x, int y,int maze[ROWS][COLS], int num_rows,int num_cols)
{
   if(inbounds(x,num_rows)&&inbounds(y,num_cols))
   {
      if(maze[x][y]!=WALL&&maze[x][y]!=BEEN)
      {
         return true;
      }
   }
   return false;
}
