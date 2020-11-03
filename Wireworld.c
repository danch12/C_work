#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <string.h>
#include <assert.h>
#include <ctype.h>

typedef enum state {empty,head,tail,copper} state;
typedef enum edges {none,up,down,left,right} edges;
typedef enum bool {false,true} bool;

#define MAXCOLS 40
#define MAXROWS 40

#define MAXCHARS 1641

#define BEHIND -1
#define INFRONT 1
#define INLINE 0
#define ITERS 100
/*going to assume that everything that isnt on the board is empty
 so dont have to worry about it */
void neillbusywait(const double secs);

 bool over_edge(int current_row,int current_col,\
                int num_rows,int num_cols);


/*counts the cells around current to decide if the cell should
be copper or head*/
state copper_or_head(int position_row, int position_col, state board[][MAXCOLS],\
                  int num_rows,int num_cols);

/*given previous board and position returns a new cell based on rules*/
state trans_cell(int position_row, int position_col, state board[][MAXCOLS],\
            int num_rows,int num_cols);

/*this puts a board into a string so can easily test*/
void stringify(state board[][MAXCOLS],char target_str[],int num_rows,int num_cols);

void fillboard(state previous[][MAXCOLS],state target[][MAXCOLS],int num_rows,\
               int num_cols);

void copyboard(state previous[][MAXCOLS],state target[][MAXCOLS],int num_rows,\
               int num_cols);

state switch_input(char c);

void init_board(char* filename, state board[][MAXCOLS],\
               int num_rows,int num_cols);

/*function for testing other functions*/
void test(void);

int main(void)
{
   int i;
   state previous[MAXROWS][MAXCOLS]={{0}};
   state current[MAXROWS][MAXCOLS]={{0}};
   char str_board[1641];
   test();
   init_board("wirewcircuit1.txt",previous,MAXROWS,MAXCOLS);
   for(i=0;i<ITERS;i++)
   {
      stringify(previous,str_board,MAXROWS,MAXCOLS);
      printf("%s",str_board);
      fillboard(previous,current,MAXROWS,MAXCOLS);
      copyboard(current,previous,MAXROWS,MAXCOLS);
      neillbusywait(0.1);
      system("clear");

   }
   return 0;
}

void test(void)
{
   state test_board[][MAXCOLS]={{1,3,3,2},
                                 {1,0,0,0},
                                 {1,1,2,0},
                                 {0,1,1,2}};

   state empty_board[][MAXCOLS]={{0},{0},{0},{0}};
   /*board is 40*40 + another 40 for the \ns and 1 for \0*/
   char test_str[1641];

   assert(over_edge(0,0,10,10)==false);
   assert(over_edge(-1,0,10,10)==true);
   assert(over_edge(0,-1,10,10)==true);
   assert(over_edge(10,1,10,10)==true);
   assert(over_edge(1,10,10,10)==true);

   /*dont have to check what current cell is here
   as will do it in trans cell*/
   assert(copper_or_head(0,0,test_board,4,4)==head);
   assert(copper_or_head(3,3,test_board,4,4)==head);
   assert(copper_or_head(0,2,test_board,4,4)==copper);

   assert(trans_cell(0,2,test_board,4,4)==copper);
   assert(trans_cell(0,1,test_board,4,4)==head);
   assert(trans_cell(1,1,test_board,4,4)==empty);
   assert(trans_cell(0,0,test_board,4,4)==tail);

   fillboard(test_board,empty_board,4,4);
   stringify(empty_board,test_str,4,4);
   printf("%s",test_str);
   assert(strcmp(test_str,"THCC\nT...\nTTC.\n.TTC\n")==0);
}

bool over_edge(int current_row,int current_col,\
               int num_rows,int num_cols)
{
   if(current_row<0||current_row>=num_rows)
   {
      return true;
   }
   if(current_col<0||current_col>=num_cols)
   {
      return true;
   }
   return false;
}



state copper_or_head(int current_row, int current_col, state board[][MAXCOLS],\
                  int num_rows,int num_cols)
{
   int i,j,head_count;
   head_count=0;
   for(i=BEHIND;i<=INFRONT;i++)
   {
      for(j=BEHIND;j<=INFRONT;j++)
      {
         if(!over_edge(current_row+i,current_col+j,num_rows,num_cols))
         {
            if(board[current_row+i][current_col+j]==head)
            {
               head_count+=1;
            }
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



void fillboard(state previous[][MAXCOLS],state target[][MAXCOLS],int num_rows,\
               int num_cols)
{
   int i,j;

   for(i=0;i<num_rows;i++)
   {
      for(j=0;j<num_cols;j++)
      {
         target[i][j]=trans_cell(i,j,previous,num_rows,num_cols);
      }
   }
}

void copyboard(state previous[][MAXCOLS],state target[][MAXCOLS],int num_rows,\
               int num_cols)
{
   int i,j;
   for(i=0;i<num_rows;i++)
   {
      for(j=0;j<num_cols;j++)
      {
         target[i][j]=previous[i][j];

      }
   }
}

void init_board(char* filename, state board[][MAXCOLS],\
               int num_rows,int num_cols)
{
   FILE *ifp;
   char symbol_store[1641];
   char symbol;
   int i,j,pos;
   i=0;
   if((ifp=fopen(filename,"r"))==NULL)
   {
      fprintf(stderr,"cant open\n");
      exit(EXIT_FAILURE);
   }
   while((symbol=fgetc(ifp))!=EOF)
   {
      if(symbol!='\n')
      {
         symbol_store[i]=symbol;
         i++;
      }
   }
   symbol_store[i]='\0';
   pos=0;
   for(i=0;i<num_rows;i++)
   {
      for(j=0;j<num_cols;j++)
      {
         board[i][j]=switch_input(symbol_store[pos]);
         pos++;
      }
   }
   fclose(ifp);
}



state switch_input(char c)
{
   char upper;
   upper=toupper(c);
   switch(upper)
   {
      case(' '):
      return empty;

      case('H'):
      return head;


      case('T'):
      return tail;


      case('C'):
      return copper;

      default:
      fprintf(stderr,"%c not valid cell\n",c);
      exit(EXIT_FAILURE);
   }
}



void neillbusywait(const double secs)
{
   clock_t t2;
   const clock_t t1 = clock();
   do{
      t2 = clock();
   }while((t2-t1) < (clock_t)((double)CLOCKS_PER_SEC*secs));
}
