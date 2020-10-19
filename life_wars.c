#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <assert.h>

#define ROWS 90
#define COLS 150
#define BUFFERSIZE 100
#define GAMES 50
#define GENERATIONS 5000

typedef int cells;
typedef enum bool {false, true} bool;
typedef enum status {dead,player1,player2} status;

void copyboard(status copy_from[][COLS],status target[][COLS]);

int alive_counter(int position_row, int position_col,\
               status player_to_count, status board[][COLS]);

/*because we dont know which team we can turn this to a bool and
 then only change if dead*/
bool survived_player(int alive_count_team1,int alive_count_team2);

/*this stumped me for ages but realised that can only birth with 3 cells
so dont have to worry about draws*/
status birth_player(int alive_count_team1,int alive_count_team2);

/*bit more verbiose than needed but thought it was clearer this way*/
void fillboard(status previous[][COLS],status target[][COLS]);

int count_board_pieces(status player_to_count,status board[][COLS]);

void initialise_board(status player,char* filename,status board[][COLS]);

/*uses positions in file to clear those spots on board*/
void clear_player(status player,int x_cord_start,int y_cord_start,\
   char* filename,status board[][COLS]);

void print_board(status board[][COLS]);


int main(void)
{
   int i,j;
   int total_player_1,total_player_2;
   status previous[ROWS][COLS]={{0}};
   status current[ROWS][COLS]={{0}};
   srand(time(NULL));
   total_player_1=0;
   total_player_2=0;
   for(i=0;i<GAMES;i++)
   {
      initialise_board(player1,"bigglider_106.lif",previous);
      initialise_board(player2,"gosperglidergun_106.lif",previous);

      for(j=0;j<GENERATIONS;j++)
      {
         fillboard(previous,current);
         total_player_1+=count_board_pieces(player1,current);
         total_player_2+=count_board_pieces(player2,current);

         copyboard(current,previous);
      }
      printf("%2d  %5d   %5d   %s\n",i,total_player_1,total_player_2,\
         ((total_player_1>total_player_2)? "Player 1": "Player 2"));
   }
}


int alive_counter(int position_row, int position_col,\
               status player_to_count, status board[][COLS])
{
   int i,j,alive_count;
   int final_row,final_col,self;
   alive_count=0;
   for(i=-1;i<2;i++)
   {
      for(j=-1;j<2;j++)
      {
         final_row= ((position_row+i)>=ROWS) ?\
                     (abs(ROWS-position_row+i)) : (position_row+i);
         final_col=((position_col+i)>=COLS) ?\
                     (abs(COLS-position_col+j)): (position_col+j);
         if(board[final_row][final_col]==player_to_count)
         {
            alive_count+=1;
         }
      }
   }
   /*need to take away yourself if alive*/
   self = (board[final_row][final_col]>0) ? 1:0;
   return alive_count-self;
}

/*because we dont know which team we can turn this to a bool and
 then only change if dead*/
bool survived_player(int alive_count_team1,int alive_count_team2)
{
   int total_alive;
   total_alive = alive_count_team1 + alive_count_team2;
   if(total_alive==2||total_alive==3)
   {
      return true;
   }
   else
   {
      return false;
   }
}

/*this stumped me for ages but realised that can only birth with 3 cells
so dont have to worry about draws*/
status birth_player(int alive_count_team1,int alive_count_team2)
{
   int total_alive;
   total_alive = alive_count_team1 + alive_count_team2;
   if(total_alive==3)
   {
      return (alive_count_team1>alive_count_team2) ? player1:player2;
   }
   else{
      return dead;
   }
}


/*bit more verbiose than needed but thought it was clearer this way*/
void fillboard(status previous[][COLS],status target[][COLS])
{
   int i,j;
   status new_cell;
   for(i=0;i<ROWS;i++)
   {
      for(j=0;j<COLS;j++)
      {
         if(previous[i][j]!=dead)
         {
            /*if dead*/
            if(!survived_player(alive_counter(i,j,player1,previous),\
                                     alive_counter(i,j,player2,previous)))
            {
               new_cell=dead;
            }
         }
         else
         {
            new_cell=birth_player(alive_counter(i,j,player1,previous),\
                                 alive_counter(i,j,player2,previous));
         }
         target[i][j]=new_cell;
      }
   }
}

void copyboard(status copy_from[][COLS],status target[][COLS])
{
   int i,j;
   for(i=0;i<ROWS;i++)
   {
      for(j=0;j<COLS;j++)
      {
         target[i][j]=copy_from[i][j];
      }
   }
}

int count_board_pieces(status player_to_count,status board[][COLS])
{
   int i,j,counter;
   counter=0;
   for(i=0;i<ROWS;i++)
   {
      for(j=0;j<COLS;j++)
      {
         if(board[i][j]==player_to_count)
         {
            counter+=1;
         }
      }
   }
   return counter;
}


void initialise_board(status player,char* filename,status board[][COLS])
{
   FILE *fp;
   char buffer[BUFFERSIZE];
   int x_cord,y_cord,start_x,start_y;
   int position_x,position_y;

   fp= fopen(filename,"r");
   if(fp==NULL)
   {
      perror("error while opening file\n");
      exit(1);
   }
   fgets(buffer,BUFFERSIZE,fp);
   start_x=rand()% COLS;
   start_y=rand()% ROWS;
   while(fscanf(fp,"%d %d\n",&x_cord, &y_cord)!=EOF)
   {
      position_x= ((start_x+x_cord)>=ROWS) ?\
                  (abs(ROWS-start_x + x_cord)) : (start_x+x_cord);

      position_y= ((start_y+y_cord)>=COLS) ?\
                  (abs(COLS-start_y + y_cord)) : (start_y+y_cord);

      if(board[position_x][position_y]==dead)
      {
         board[position_x][position_y]=player;
      }
      else
      {
         fclose(fp);
         clear_player(player,start_x,start_y,filename,board);
         initialise_board(player,filename,board);
      }
   }
}

/*uses positions in file to clear those spots on board*/
void clear_player(status player,int x_cord_start,int y_cord_start,\
   char* filename,status board[][COLS])
{
   FILE *fp;
   char buffer[BUFFERSIZE];
   int position_x,position_y,x_cord,y_cord;
   fp= fopen(filename,"r");
   if(fp==NULL)
   {
      perror("error while opening file\n");
      exit(1);
   }
   fgets(buffer,BUFFERSIZE,fp);
   while(fscanf(fp,"%d %d\n",&x_cord, &y_cord)!=EOF)
   {
      position_x= ((x_cord_start+x_cord)>=ROWS) ?\
                  (abs(ROWS-x_cord_start + x_cord)) : (x_cord_start+x_cord);
      position_y= ((y_cord_start+y_cord)>=COLS) ?\
                  (abs(ROWS-y_cord_start + y_cord)) : (y_cord_start+y_cord);
      if(board[position_x][position_y]==player)
      {
         board[position_x][position_y]=dead;
      }
   }
   fclose(fp);
}

void print_board(status board[][COLS])
{
   int i,j;
   for(i=0;i<ROWS;i++)
   {
      for(j=0;j<COLS;j++)
      {
         if(board[i][j]==player1)
         {
            printf("@");
         }
         else if(board[i][j]==player2)
         {
            printf("+");
         }
         else
         {
            printf(".");
         }
      }
      printf("\n");
   }
}
