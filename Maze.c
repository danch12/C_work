#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <string.h>
#include <assert.h>

typedef enum bool {false,true} bool;

#define ROWS 4
#define COLS 4
#define EXIT 3
#define WALL 2
#define PATH 1
#define BEEN 0

bool inbounds(int x,int size);
void test(void);
bool can_go(int x, int y,int maze[ROWS][COLS]);
int explore(int x,int y, int maze[ROWS][COLS]);

int main(void)
{
   test();
   return 0;
}

void test(void)
{
   int test_maze[4][4]={{2,2,1,2},
                        {2,1,1,2},
                        {3,1,2,2}};

   assert(inbounds(50,5)==false);
   assert(inbounds(-1,4)==false);
   assert(inbounds(1,5)==true);
   assert(inbounds(0,5)==true);
   explore(0,2,test_maze);


}



bool inbounds(int x,int size)
{
   if(x>=0&&x<size)
   {
      return true;
   }
   return false;
}

int explore(int x,int y, int maze[ROWS][COLS])
{
   printf("%d\n",maze[x][y]);
   if(maze[x][y]==EXIT)
   {
      return 1;
   }

   maze[x][y]=BEEN;
   if(can_go(x-1,y,maze))
   {
      if(explore(x-1,y,maze))
      {
         return 1;
      }
   }
   if(can_go(x+1,y,maze))
   {
      if(explore(x+1,y,maze))
      {
         return 1;
      }
   }
   if(can_go(x,y-1,maze))
   {
      if(explore(x,y-1,maze))
      {
         return 1;
      }
   }
   if(can_go(x,y+1,maze))
   {
      if(explore(x,y+1,maze))
      {
         return 1;
      }
   }
   return 0;
}

bool can_go(int x, int y,int maze[ROWS][COLS])
{
   if(inbounds(x,ROWS)&&inbounds(y,COLS))
   {
      if(maze[x][y]!=WALL&&maze[x][y]!=BEEN)
      {
         return true;
      }
   }
   return false;
}
