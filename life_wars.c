#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <assert.h>

#define COLS 150
#define ROWS 90
#define TESTCOLS 4
#define TESTROWS 4

#define BUFFERSIZE 100
#define GAMES 5
#define GENERATIONS 5000

typedef int cells;
typedef enum bool {false, true} bool;
typedef enum status {dead,player1,player2} status;

/*copies board to another board*/
void copyboard(status copy_from[][COLS],status target[][COLS],\
               int num_rows,int num_cols);

/*counts how many alive cells of one player there are*/
int alive_counter(int position_row, int position_col,\
               status player_to_count, status board[][COLS],int num_rows,int num_cols);


int wrap_around(int max_num,int position);
/*because we dont know which team we can turn this to a bool and
 then only change if dead*/
bool survived_player(int alive_count_team1,int alive_count_team2);

/*this stumped me for ages but realised that can only birth with 3 cells
so dont have to worry about draws*/
status birth_player(int alive_count_team1,int alive_count_team2);

/*bit more verbiose than needed but thought it was clearer this way*/
void fillboard(status previous[][COLS],status target[][COLS],\
               int num_rows,int num_cols);

/*given a player count how many pieces on the board they have*/
int count_board_pieces(status player_to_count,status board[][COLS],\
                     int num_rows,int num_cols);

/*given a player and a file inits the board based on random position*/
int initialise_board(status player,char* filename,status board[][COLS],\
                     int num_rows,int num_cols,int start_x,int start_y);

/*uses positions in file to clear those spots on board*/
void clear_player(status player,int x_cord_start,int y_cord_start,\
   char* filename,status board[][COLS],int num_rows,int num_cols);

/*prints the board */
void print_board(status board[][COLS],int num_rows,int num_cols);

void clear_board(status board[][COLS],int num_rows,int num_cols);

void test(void);


int main(int argc,char* argv[])
{
   int i,j;
   int total_player_1,total_player_2,temp_player_1,temp_player_2;
   status previous[ROWS][COLS]={{0}};
   status current[ROWS][COLS]={{0}};
   srand(time(NULL));
   total_player_1=0;
   total_player_2=0;
   test();
   if(argc<3)
   {
      printf("not enough files\n");
      return 1;
   }
   for(i=0;i<GAMES;i++)
   {
      /*need to empty board*/
      clear_board(previous,ROWS,COLS);
      initialise_board(player1,argv[1],previous,\
                        ROWS,COLS,(rand()% ROWS),(rand()% COLS));
      /*basically keep running until find free space*/
      while(initialise_board(player2,argv[2],\
            previous,ROWS,COLS,(rand()% ROWS),(rand()% COLS)));

      temp_player_1=0;
      temp_player_2=0;
      for(j=0;j<GENERATIONS;j++)
      {
         fillboard(previous,current,ROWS,COLS);
         temp_player_1=count_board_pieces(player1,current,ROWS,COLS);
         temp_player_2=count_board_pieces(player2,current,ROWS,COLS);

         copyboard(current,previous,ROWS,COLS);
      }
      total_player_1+=temp_player_1;
      total_player_2+=temp_player_2;
      printf("%2d  %5d   %5d   %s\n",i+1,temp_player_1,temp_player_2,\
         ((temp_player_1>temp_player_2)? "Player 1": "Player 2"));
   }
   printf("%s won with %5d to %5d\n",\
         (total_player_1>total_player_2)? "Player 1": "Player 2",\
         (total_player_1>total_player_2)? total_player_1:total_player_2,\
         (total_player_1>total_player_2)?total_player_2:total_player_1);
}


void test(void)
{
   int i,j;
   status empty_board[][COLS]={{0,0,0,0},
                              {0,0,0,0},
                              {0,0,0,0},
                              {0,0,0,0}};

   status test_board[][COLS]={{0,0,1,0},
                              {1,0,1,0},
                              {0,0,1,0},
                              {0,0,1,0}};

   status test_board_player_2[][COLS]={{0,0,2,0},
                                       {2,0,2,0},
                                       {0,0,2,0},
                                       {0,0,2,0}};

   status target_board_player_1[][COLS]={{0,0,1,0},
                                          {0,0,1,0},
                                          {0,0,1,0},
                                          {0,1,1,1}};

   status target_board_player_2[][COLS]={{0,0,2,0},
                                         {0,0,2,0},
                                         {0,0,2,0},
                                          {0,2,2,2}};

   status glider_board[][COLS]={{0,1,0,1},
                                {1,1,0,0},
                                {0,0,0,0},
                                {0,1,0,0}};

   assert(survived_player(1,2)==true);
   assert(survived_player(0,2)==false);
   assert(survived_player(0,4)==true);
   assert(wrap_around(100,10)==10);
   assert(wrap_around(100,100)==0);
   assert(wrap_around(100,-1)==99);
   assert(wrap_around(4,-1)==3);
   assert(wrap_around(4,2)==2);
   assert(wrap_around(4,2)==2);
   assert(birth_player(0,0)==dead);
   assert(birth_player(2,1)==player1);
   assert(birth_player(1,2)==player2);
   assert(alive_counter(1,1,player1,test_board,TESTROWS,TESTCOLS)==4);
   assert(alive_counter(1,0,player1,test_board,TESTROWS,TESTCOLS)==1);
   assert(alive_counter(3,3,player1,test_board,TESTROWS,TESTCOLS)==3);
   assert(alive_counter(0,2,player1,test_board,TESTROWS,TESTCOLS)==3);

   fillboard(test_board,empty_board,TESTROWS,TESTCOLS);
   for(i=0;i<TESTROWS;i++)
   {
      for(j=0;j<TESTCOLS;j++)
      {
         assert(empty_board[i][j]==target_board_player_1[i][j]);
      }
   }

   fillboard(test_board_player_2,empty_board,TESTROWS,TESTCOLS);
   for(i=0;i<TESTROWS;i++)
   {
      for(j=0;j<TESTCOLS;j++)
      {
         assert(empty_board[i][j]==target_board_player_2[i][j]);
      }
   }
   for(i=0;i<TESTROWS;i++)
   {
      for(j=0;j<TESTCOLS;j++)
      {
         empty_board[i][j]=dead;
      }
   }
   copyboard(test_board,empty_board,TESTROWS,TESTCOLS);

   for(i=0;i<TESTROWS;i++)
   {
      for(j=0;j<TESTCOLS;j++)
      {
         assert(empty_board[i][j]==test_board[i][j]);
      }
   }

   assert(count_board_pieces(player1,target_board_player_1,TESTROWS,TESTCOLS)==6);
   assert(count_board_pieces(player1,target_board_player_2,TESTROWS,TESTCOLS)==0);
   assert(count_board_pieces(player2,target_board_player_2,TESTROWS,TESTCOLS)==6);

   for(i=0;i<TESTROWS;i++)
   {
      for(j=0;j<TESTCOLS;j++)
      {
         empty_board[i][j]=dead;
      }
   }

   assert(initialise_board(player1,"glider.txt",empty_board,\
                                 TESTROWS,TESTCOLS,0,0)==0);
   for(i=0;i<TESTROWS;i++)
   {
      for(j=0;j<TESTCOLS;j++)
      {
         assert(empty_board[i][j]==glider_board[i][j]);
      }
   }

   /*void clear_player(status player,int x_cord_start,int y_cord_start,\
      char* filename,status board[][COLS],int num_rows,int num_cols)*/
   clear_player(player1,0,0,"glider.txt",empty_board,TESTROWS,TESTCOLS);

   for(i=0;i<TESTROWS;i++)
   {
      for(j=0;j<TESTCOLS;j++)
      {
         assert(empty_board[i][j]==dead);
      }
   }
   initialise_board(player1,"glider.txt",empty_board,\
                                 TESTROWS,TESTCOLS,0,0);

   clear_player(player2,0,0,"glider.txt",empty_board,TESTROWS,TESTCOLS);

   for(i=0;i<TESTROWS;i++)
   {
      for(j=0;j<TESTCOLS;j++)
      {
         assert(empty_board[i][j]==glider_board[i][j]);
      }
   }

   clear_board(empty_board,TESTROWS,TESTCOLS);
   for(i=0;i<TESTROWS;i++)
   {
      for(j=0;j<TESTCOLS;j++)
      {
         assert(empty_board[i][j]==dead);
      }
   }
}



/*alive counter will count current cell as alive
 this is removed in the future as is easier to know what cell contains+
 i call alive counter twice per birth/death*/
int alive_counter(int position_row, int position_col,\
      status player_to_count, status board[][COLS],int num_rows,int num_cols)
{
   int i,j,alive_count;
   int final_row,final_col;
   alive_count=0;
   for(i=-1;i<2;i++)
   {
      for(j=-1;j<2;j++)
      {
         final_row=wrap_around(num_rows,(position_row+i));
         final_col=wrap_around(num_cols,(position_col+j));

         if(board[final_row][final_col]==player_to_count)
         {
            alive_count+=1;
         }
      }
   }
   return alive_count;
}


int wrap_around(int max_num,int position)
{
   if(position>=max_num)
   {
      return abs(max_num-position);
   }
   else if(position<0)
   {
      return max_num+position;
   }
   else
   {
      return position;
   }
}


/*because we dont know which team in the function we can turn this to a
bool and then only change if dead*/
bool survived_player(int alive_count_team1,int alive_count_team2)
{
   int total_alive;
   /*alive counter counts the current cell as well so have to minus it
   we know its alive as we are considering survival*/
   total_alive = alive_count_team1 + alive_count_team2-1;
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
   /*alive counter counts the current
   but know that it is dead since we are talking about birth*/
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
void fillboard(status previous[][COLS],status target[][COLS],\
               int num_rows,int num_cols)
{
   int i,j;
   status new_cell;
   for(i=0;i<num_rows;i++)
   {
      for(j=0;j<num_cols;j++)
      {

         if(previous[i][j]!=dead)
         {
            /*if dead*/
            if(!survived_player(alive_counter(i,j,player1,previous,num_rows,num_cols),\
                        alive_counter(i,j,player2,previous,num_rows,num_cols)))
            {
               new_cell=dead;
            }
            else
            {
               new_cell=previous[i][j];
            }
         }
         else
         {
            new_cell=birth_player(alive_counter(i,j,player1,previous,num_rows,num_cols),\
                        alive_counter(i,j,player2,previous,num_rows,num_cols));
         }
         target[i][j]=new_cell;
      }
   }
}

void copyboard(status copy_from[][COLS],status target[][COLS],\
               int num_rows,int num_cols)
{
   int i,j;
   for(i=0;i<num_rows;i++)
   {
      for(j=0;j<num_cols;j++)
      {
         target[i][j]=copy_from[i][j];
      }
   }
}

int count_board_pieces(status player_to_count,status board[][COLS],\
                        int num_rows,int num_cols)
{
   int i,j,counter;
   counter=0;
   for(i=0;i<num_rows;i++)
   {
      for(j=0;j<num_cols;j++)
      {
         if(board[i][j]==player_to_count)
         {
            counter+=1;
         }
      }
   }
   return counter;
}

/*
void initialise_board(status player,char* filename,status board[][COLS],\
                     int num_rows,int num_cols)
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
   start_x=rand()% ROWS;
   start_y=rand()% COLS;
   while(fscanf(fp,"%d %d\n",&x_cord, &y_cord)!=EOF)
   { */
      /*if row/col goes over max then wrap around*/
   /*   position_x= ((start_x+x_cord)>=ROWS) ?\
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
}*/


int initialise_board(status player,char* filename,status board[][COLS],\
                     int num_rows,int num_cols,int start_x,int start_y)
{
   FILE *fp;
   char buffer[BUFFERSIZE];
   int x_cord,y_cord;
   int position_x,position_y;

   fp= fopen(filename,"r");
   if(fp==NULL)
   {
      perror("error while opening file\n");
      exit(1);
   }
   fgets(buffer,BUFFERSIZE,fp);

   while(fscanf(fp,"%d %d\n",&x_cord, &y_cord)!=EOF)
   {
      /*if row/col goes over max then wrap around*/
      position_x= wrap_around(num_rows,start_x+x_cord);
      position_y= wrap_around(num_cols,start_y+y_cord);
      if(board[position_x][position_y]==dead)
      {
         board[position_x][position_y]=player;
      }
      else
      {
         /*if it finds a occupied cell then clears everything and returns */
         fclose(fp);
         clear_player(player,start_x,start_y,filename,board,num_rows,num_cols);
         return 1;
      }
   }
   return 0;
}

/*uses positions in file to clear those spots on board*/
void clear_player(status player,int x_cord_start,int y_cord_start,\
   char* filename,status board[][COLS],int num_rows,int num_cols)
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

      position_x= wrap_around(num_rows,x_cord_start+x_cord);
      position_y= wrap_around(num_cols,y_cord_start+y_cord);
      if(board[position_x][position_y]==player)
      {
         board[position_x][position_y]=dead;
      }
   }
   fclose(fp);
}


/*this isnt really needed but its nice to see the board for curiosity*/
void print_board(status board[][COLS],int num_rows,int num_cols)
{
   int i,j;
   for(i=0;i<num_rows;i++)
   {
      for(j=0;j<num_cols;j++)
      {
         if(board[i][j]==player1)
         {
            printf("1,");
         }
         else if(board[i][j]==player2)
         {
            printf("2,");
         }
         else
         {
            printf("0,");
         }
      }
      printf("\n");
   }
}


void clear_board(status board[][COLS],int num_rows,int num_cols)
{
   int i,j;
   for(i=0;i<num_rows;i++)
   {
      for(j=0;j<num_cols;j++)
      {
         board[i][j]=dead;
      }
   }
}
