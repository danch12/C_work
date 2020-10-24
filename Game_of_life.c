#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <string.h>
#include <assert.h>

#define ITERS 10
#define SELF 1
#define COLS 150
#define ROWS 90
#define TESTCOLS 20
#define TESTROWS 10

#define BEHIND -1
#define INFRONT 1
#define INLINE 0
#define BUFFERSIZE 100


typedef enum bool {false, true} bool;
typedef enum status {dead,alive} status;
typedef enum edges {none,up,down,left,right} edges;
typedef enum rule {original,replicator} rule;

typedef struct box{
   int left;
   int right;
   int up;
   int down;
} box;

/*based on row position will return what edges you are on if any*/
edges on_edge_row(int position_row,int num_rows);

/*based on col position will return what edges you are on if any*/
edges on_edge_col(int position_col,int num_cols);

/*creates a kinda box that identifies area we need to count survived cells in*/
box create_box(edges edge_row,edges edge_col);

/*returns count of alive cells in relevant area*/
int alive_counter(int position_row, int position_col, status board[][COLS],\
                  int num_rows,int num_cols);

/*based on position return if alive cell survived or not using original rules*/
status survived_original(int alive_count);

/*based on position return if uninhabited cell
is born or not using original rules*/
status birth_original(int alive_count);

/*based on position return if  cell survives/births
using replicator rule*/
status replicator_rule(int alive_count);

/*copies board to target array*/
void copyboard(status copy_from[][COLS],status target[][COLS],\
               int num_rows,int num_cols);

/*fills board using previous iteration*/
void fillboard(status previous[][COLS],status target[][COLS],int num_rows,\
         int num_cols,status (*survived_rule)(int), status (*birth_rule)(int));

/*fills board using life file*/
void fill_start_board(char* filename,status board[][COLS],\
                     int board_rows,int board_cols);

void print_board(status board[][COLS],int num_rows,int num_cols);
/*testing function*/
void test(void);


status (*which_rule(char* rule_spec,status dead_or_alive))(int alive_count);

char* string_board(status board[][COLS],int num_rows,int num_cols);





int main(int argc,char* argv[])
{
   int i;
   status previous[ROWS][COLS]={{0}};
   status current[ROWS][COLS]={{0}};
   status (*rule_ptr_survived)(int);
   status (*rule_ptr_birth)(int);
   char* board_to_print;
   test();
   if(argc<3)
   {
      printf("error need more args\n");
      return 1;
   }
   else
   {
      rule_ptr_survived= which_rule(argv[1],alive);
      rule_ptr_birth = which_rule(argv[1],dead);

      fill_start_board(argv[2],current,ROWS,COLS);
      for(i=0;i<ITERS;i++)
      {
         printf("iteration= %d\n",i);
         board_to_print=string_board(current,ROWS,COLS);
         printf("%s",board_to_print);
         free(board_to_print);
         copyboard(current,previous,ROWS,COLS);
         fillboard(previous,current,ROWS,COLS,rule_ptr_survived,rule_ptr_birth);
         printf("\n");
      }
      return 0;
   }
}


void test(void)
{
   box test_box_1,test_box_2,test_box_3,test_box_4;
   int i,j;
   status test_board[ROWS][COLS],empty_board[ROWS][COLS];
   char* test_string_target="....................\n....................\n....................\n....................\n...........@........\n.........@.@........\n..........@@........\n....................\n....................\n....................\n";
   char* test_string;

   status fillboard_test_start[ROWS][COLS]=\
   {{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
   {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
   {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
   {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
   {0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0},
   {0,0,0,0,0,0,0,0,0,1,0,1,0,0,0,0,0,0,0,0},
   {0,0,0,0,0,0,0,0,0,0,1,1,0,0,0,0,0,0,0,0},
   {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
   {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
   {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}};

   status fillboard_test_end[ROWS][COLS]=\
   {{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
   {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
   {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
   {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
   {0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0},
   {0,0,0,0,0,0,0,0,0,0,0,1,1,0,0,0,0,0,0,0},
   {0,0,0,0,0,0,0,0,0,0,1,1,0,0,0,0,0,0,0,0},
   {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
   {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
   {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}};



   for(i=0;i<TESTROWS;i++)
   {
      for(j=0;j<TESTCOLS;j++)
      {
         test_board[i][j]=alive;
         empty_board[i][j]=dead;
      }
   }
   assert(on_edge_row(0,100)==up);
   assert(on_edge_row(9,TESTROWS)==down);
   assert(on_edge_row(5,TESTROWS)==none);
   assert(on_edge_col(0,TESTCOLS)==left);
   assert(on_edge_col(TESTCOLS-1,TESTCOLS)==right);
   assert(on_edge_col(6, TESTCOLS)==none);

   test_box_1=create_box(up,right); /*top right */
   test_box_2=create_box(left,left);
   test_box_3=create_box(none,left); /*should be the same as test box 2*/
   test_box_4=create_box(down,left);

   assert(test_box_1.up==INLINE);
   assert(test_box_3.left==INLINE);
   assert(test_box_2.left==INLINE);
   assert(test_box_2.down==test_box_3.down==INFRONT);
   assert(test_box_4.down==INLINE);
   assert(test_box_4.up==BEHIND);



   assert(alive_counter(0,0,test_board,TESTROWS,TESTCOLS)==3);
   assert(alive_counter(5,5,test_board,TESTROWS,TESTCOLS)==8);
   assert(alive_counter(0,5,test_board,TESTROWS,TESTCOLS)==5);

   /*positions 0 indexed so have to take 1*/
   assert(alive_counter(TESTROWS-1,5,test_board,TESTROWS,TESTCOLS)==5);
   assert(alive_counter(6,TESTCOLS-1,test_board,TESTROWS,TESTCOLS)==5);
   /*need more tests when i can create boards easy*/

   assert(survived_original(10)==dead);
   assert(survived_original(2)==alive);
   assert(birth_original(3)==alive);
   assert(birth_original(1)==dead);
   assert(replicator_rule(2)==dead);
   assert(replicator_rule(1)==alive);

   copyboard(test_board,empty_board,TESTROWS,TESTCOLS);
   for(i=0;i<TESTROWS;i++)
   {
      for(j=0;j<TESTCOLS;j++)
      {
         assert(test_board[i][j]==empty_board[i][j]);
      }
   }
   /*this test only works with COLS=20 and ROWS=10
    because it was long to write out all the numbers*/
   fillboard(fillboard_test_start,empty_board,\
            TESTROWS,TESTCOLS,survived_original,birth_original);

   for(i=0;i<TESTROWS;i++)
   {
      for(j=0;j<TESTCOLS;j++)
      {
         assert(fillboard_test_end[i][j]==empty_board[i][j]);
      }
   }

   for(i=0;i<TESTROWS;i++)
   {
      for(j=0;j<TESTCOLS;j++)
      {
         empty_board[i][j]=dead;
      }
   }

   fill_start_board("glider.txt",empty_board,TESTROWS,TESTCOLS);
   for(i=0;i<TESTROWS;i++)
   {
      for(j=0;j<TESTCOLS;j++)
      {
         assert(empty_board[i][j]==fillboard_test_start[i][j]);
      }
   }

   test_string=string_board(empty_board,TESTROWS,TESTCOLS);

   for(i=0;test_string[i]!='\0';i++)
   {
      assert(test_string[i]==test_string_target[i]);
   }
   free(test_string);
}

edges on_edge_row(int position_row,int num_rows)
{
   if(position_row==0)
   {
      return up;
   }
   if(position_row==num_rows-1)
   {
      return down;
   }
   else
   {
      return none;
   }
}

edges on_edge_col(int position_col,int num_cols)
{
   if(position_col==0)
   {
      return left;
   }
   if(position_col==num_cols-1)
   {
      return right;
   }
   else
   {
      return none;
   }
}

/*creates a box that identifies area we need to count survived cells in*/
box create_box(edges edge_row,edges edge_col)
{
   box countarea;

   switch(edge_row)
   {
      case(up):
      countarea.up=INLINE;
      countarea.down=INFRONT;
      break;

      case(down):
      countarea.up=BEHIND;
      countarea.down=INLINE;
      break;

      default:
      countarea.up=BEHIND;
      countarea.down=INFRONT;
   }

   switch(edge_col)
   {
      case(left):
      countarea.left=INLINE;
      countarea.right=INFRONT;
      break;

      case(right):
      countarea.left=BEHIND;
      countarea.right=INLINE;
      break;

      default:
      countarea.left=BEHIND;
      countarea.right=INFRONT;
   }
   return countarea;
}

int alive_counter(int position_row, int position_col, status board[][COLS],\
                  int num_rows,int num_cols)
{
   int i,j,alive_count;
   edges edge_row,edge_col;
   box relevant_area;
   alive_count=0;

   edge_row = on_edge_row(position_row,num_rows);
   edge_col = on_edge_col(position_col,num_cols);

   relevant_area = create_box(edge_row,edge_col);
   for(i=relevant_area.up;i<relevant_area.down+1;i++)
   {
      for(j=relevant_area.left;j<relevant_area.right+1;j++)
      {

         if(board[position_row+i][position_col+j]==alive)
         {
            alive_count+=1;
         }
      }
   }
   /*need to take away yourself*/
   return alive_count-board[position_row][position_col];
}

status survived_original(int alive_count)
{
   if(alive_count==2||alive_count==3)
   {
      return alive;
   }
   else
   {
      return dead;
   }
}

status birth_original(int alive_count)
{
   if(alive_count==3)
   {
      return alive;
   }
   else
   {
      return dead;
   }
}


status replicator_rule(int alive_count)
{
   if(alive_count%2!=0)
   {
      return alive;
   }
   else
   {
      return dead;
   }
}


void fillboard(status previous[][COLS],status target[][COLS],int num_rows,\
         int num_cols,status (*survived_rule)(int), status (*birth_rule)(int))
{
   int i,j;
   status new_cell;
   for(i=0;i<num_rows;i++)
   {
      for(j=0;j<num_cols;j++)
      {
         if(previous[i][j]==alive)
         {
            new_cell=survived_rule(alive_counter(i,j,previous,num_rows,num_cols));
         }
         else
         {
            new_cell=birth_rule(alive_counter(i,j,previous,num_rows,num_cols));
         }
         target[i][j]=new_cell;
      }
   }
}

void fill_start_board(char* filename,status board[][COLS],\
                     int board_rows,int board_cols)
{
   FILE *fp;
   char buffer[BUFFERSIZE];
   int x_cord,y_cord,mid_row,mid_col;
   mid_row=board_rows/2;
   mid_col=board_cols/2;
   x_cord=0;
   y_cord=0;
   fp= fopen(filename,"r");
   if(fp==NULL)
   {
      perror("error while opening file\n");
      exit(1);
   }
   /*need to increment past first line in file*/
   fgets(buffer,BUFFERSIZE,fp);
   /*then deal with rest of file */
   while(fscanf(fp,"%d %d\n",&x_cord, &y_cord)!=EOF)
   {
      if((x_cord+mid_row<board_rows)&&(y_cord+mid_col<board_cols))
      {
         /*here not sure if to exit if it doesnt fit on board
         or just not include it - ive gone with just not including*/
         board[x_cord+mid_row][y_cord+mid_col]=1;
      }

   }
   fclose(fp);
}



/*turn this into string output so can test*/
void print_board(status board[][COLS],int num_rows,int num_cols)
{
   int i,j;
   for(i=0;i<num_rows;i++)
   {
      for(j=0;j<num_cols;j++)
      {
         if(board[i][j]==alive)
         {
            printf("@");
         }
         else
         {
            printf(".");
         }
      }
      printf("\n");
   }
}

/*remember to free aferward*/
char* string_board(status board[][COLS],int num_rows,int num_cols)
{
   int i,j;
   int position;
   char* string_board;
   /*need to add one col for the \n's and one for the \0 */
   string_board=(char *)malloc(sizeof(char)*num_rows*(num_cols+1)+1);
   position=0;
   for(i=0;i<num_rows;i++)
   {

      for(j=0;j<num_cols;j++)
      {
         if(board[i][j]==alive)
         {
            string_board[position]='@';
            position++;
         }
         else
         {
            string_board[position]='.';
            position++;
         }
      }
      string_board[position]='\n';
      position++;
   }
   string_board[position]='\0';
   return string_board;
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

status (*which_rule(char* rule_spec,status dead_or_alive))(int alive_count)
{
   if(dead_or_alive==dead)
   {
      if(strcmp(rule_spec,"original")==0)
      {
         return &birth_original;
      }
      else if(strcmp(rule_spec,"replicator")==0)
      {
         return &replicator_rule;
      }
      else
      {
         printf("wrongly entered rule\n");
         exit(1);
      }
   }
   else
   {
      if(strcmp(rule_spec,"original")==0)
      {
         return &survived_original;
      }
      else if(strcmp(rule_spec,"replicator")==0)
      {
         return &replicator_rule;
      }
      else
      {
         printf("wrongly entered rule\n");
         exit(1);
      }
   }
}
