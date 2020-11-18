#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <string.h>
#include <assert.h>


#define INITLEN 10
#define SCALEFACTOR 2
#define MAXSIZE 9

typedef enum colours {empty,red, green,yellow,blue,magenta,cyan,white,black} colours;
typedef enum bool {false, true} bool;

/*worst case different book in bookcase[MAXSIZE-1][MAXSIZE-2]
which i think is O(n*n), with num books added worst case
is O(n*n+n) which decreases to just O(n*n) anyway
and potential upside is a lot higher as a lot of cases
will be */

/*to find impossible boards one of the checks will be
oscillating boards - eg move to a free shelf then move
back again *a bazillion - to do this want to check if only
one child and if so check parent boards to see repeated */


typedef struct node
{
   colours bookcase[MAXSIZE][MAXSIZE];
   struct node* parent;
   int num_children;
   /*int num_books_shelf[MAXSIZE];*/
}node;

typedef node* nodeptr;

/*because im going to be reallocing a lot
seems to make sense to use a array of pointers
to the structs rather than a array of structs themselves
meaning less space getting moved around+ less space wasted
when increaseing capacity
this is something im unsure on tho due to slower access*/
typedef struct bookcase_arr{
   int size;
   int capacity;
   nodeptr* array;
}bookcase_arr;



/*win conditions*/
bool is_happy(int num_rows,int num_cols,\
            int start_row,colours bookcase[MAXSIZE][MAXSIZE]);

/*how full is one bookcase shelf -
couldnt think of a better name sorry*/
int len_shelf_full(int row,int num_cols,colours bookcase[MAXSIZE][MAXSIZE]);



/*
legallity_checker_init() */

char* num_to_letter(colours c);
void board_to_str(colours board[MAXSIZE][MAXSIZE],\
                  char* str,int num_rows,int num_cols);

/*return void then cast when appropriate*/
void* safe_calloc(size_t nitems, size_t size);

bool legal_move(int start_row,int target_row, colours board[MAXSIZE][MAXSIZE],\
               int num_rows,int num_cols);

nodeptr create_orig_node(colours board[MAXSIZE][MAXSIZE]);

nodeptr init_copy_node(nodeptr parent_bookcase);
nodeptr make_move(int start_row,int target_row,\
            nodeptr parent_bookcase,int num_rows,int num_cols);


void add_bookcase(bookcase_arr* books_arr,nodeptr to_add);
bookcase_arr* init_bookcase_arr(void);
void iterate_one_board(nodeptr parent_bookcase,\
            int num_rows,int num_cols,bookcase_arr* bk_arr);
bool free_bookcase_arr(bookcase_arr* b_arr);


void test(void);

int main(void)
{
   test();
   return 0;
}

void test(void)
{
   int i;
   nodeptr test_node1, test_node2;
   bookcase_arr* test_book_arr;
   colours test_bc[MAXSIZE][MAXSIZE]={{red,empty,empty,empty},
                                    {blue,yellow,empty,empty},
                                    {green,green,green,empty},
                                    {green,green,green,empty},
                                    {empty,empty,empty,empty},
                                    {magenta,magenta,magenta,magenta},
                                    {empty,empty,empty,empty}};


   colours test_bc_simple[MAXSIZE][MAXSIZE]={{red,empty,empty,empty},
                                             {red,empty,empty,empty},
                                             {red,empty,empty,empty},
                                             {red,empty,empty,empty},
                                             {red,empty,empty,empty},
                                             {red,empty,empty,empty},
                                             {red,empty,empty,empty}};

   colours test_bc_simple_2[MAXSIZE][MAXSIZE]={{red,red,red,red},
                                                {red,empty,empty,empty},
                                                {red,empty,empty,empty},
                                                {red,empty,empty,empty},
                                                {red,empty,empty,empty}};
   colours test_bc_simple_3[MAXSIZE][MAXSIZE]={{red,red,red,red},
                                                {red,red,red,red},
                                                {red,red,red,red},
                                                {red,red,red,red},
                                                {red,red,red,red}};
   char test_str[200];


   assert(len_shelf_full(0,4,test_bc)==1);
   assert(len_shelf_full(1,4,test_bc)==2);
   assert(len_shelf_full(2,4,test_bc)==3);
   assert(len_shelf_full(4,4,test_bc)==0);
   assert(len_shelf_full(5,4,test_bc)==4);

   assert(is_happy(5,4,3,test_bc)==true);
   assert(is_happy(5,4,5,test_bc)==true);
   assert(is_happy(5,4,6,test_bc)==true);
   assert(is_happy(5,4,2,test_bc)==false);
   assert(is_happy(5,4,1,test_bc)==false);
   assert(is_happy(5,4,0,test_bc)==false);

   assert(strcmp(num_to_letter(empty),".")==0);
   assert(strcmp(num_to_letter(red),"R")==0);
   assert(strcmp(num_to_letter(black),"K")==0);


   board_to_str(test_bc,test_str,7,4);
   assert(strcmp(test_str,"R...\nBY..\nGGG.\nGGG.\n....\nMMMM\n....\n")==0);


   assert(legal_move(0,5,test_bc,7,4)==false);
   assert(legal_move(50,5,test_bc,7,4)==false);
   assert(legal_move(0,50,test_bc,7,4)==false);
   assert(legal_move(4,0,test_bc,7,4)==false);
   assert(legal_move(0,1,test_bc,7,4)==true);
   assert(legal_move(0,0,test_bc,7,4)==false);

   test_node1=create_orig_node(test_bc);
   assert(test_node1->num_children==0);

   test_node2=init_copy_node(test_node1);
   assert(test_node1->num_children==1);
   board_to_str(test_node1->bookcase,test_str,7,4);
   assert(strcmp(test_str,"R...\nBY..\nGGG.\nGGG.\n....\nMMMM\n....\n")==0);
   assert(test_node2->num_children==0);
   board_to_str(test_node2->bookcase,test_str,7,4);
   assert(strcmp(test_str,"R...\nBY..\nGGG.\nGGG.\n....\nMMMM\n....\n")==0);

   free(test_node2);
   free(test_node1);
   assert(init_copy_node(NULL)==NULL);

   test_node1=create_orig_node(test_bc);
   /*nodeptr make_move(int start_row,int target_row,\
               nodeptr parent_bookcase,int num_rows,int num_cols)*/

   test_node2=make_move(0,1,test_node1,7,4);
   board_to_str(test_node2->bookcase,test_str,7,4);
   assert(strcmp(test_str,"....\nBYR.\nGGG.\nGGG.\n....\nMMMM\n....\n")==0);
   free(test_node2);

   test_node2=make_move(5,0,test_node1,7,4);
   board_to_str(test_node2->bookcase,test_str,7,4);
   assert(strcmp(test_str,"RM..\nBY..\nGGG.\nGGG.\n....\nMMM.\n....\n")==0);
   free(test_node2);
   /*invalid row*/
   test_node2=make_move(-1,1,test_node1,7,4);
   assert(test_node2==NULL);
   free(test_node2);

   /*trying to add a empty to a shelf*/
   test_node2=make_move(6,1,test_node1,7,4);
   assert(test_node2==NULL);
   free(test_node2);

   /*adding to full shelf*/
   test_node2=make_move(1,5,test_node1,7,4);
   assert(test_node2==NULL);
   free(test_node2);


   test_book_arr=init_bookcase_arr();
   assert(test_book_arr->size==0);
   assert(test_book_arr->capacity==INITLEN);

   add_bookcase(test_book_arr,NULL);
   assert(test_book_arr->size==0);

   for(i=0;i<10000;i++)
   {
      assert(test_book_arr->size==i);
      do
      {
         test_node2=make_move(rand()%6+0,rand()%6+0,test_node1,7,4);
      } while(!test_node2);

      add_bookcase(test_book_arr,test_node2);
   }
   free_bookcase_arr(test_book_arr);

   free(test_node1);



   test_node1=create_orig_node(test_bc_simple);
   test_book_arr=init_bookcase_arr();
   iterate_one_board(test_node1,7,4,test_book_arr);
   /* 7*6 */
   assert(test_book_arr->size==42);
   free_bookcase_arr(test_book_arr);
   free(test_node1);

   test_node1=create_orig_node(test_bc_simple_2);
   test_book_arr=init_bookcase_arr();
   iterate_one_board(test_node1,5,4,test_book_arr);
   /*5*4 - 4 for the 4 impossible moves*/
   assert(test_book_arr->size==16);
   free_bookcase_arr(test_book_arr);
   free(test_node1);

   test_node1=create_orig_node(test_bc_simple_3);
   test_book_arr=init_bookcase_arr();
   iterate_one_board(test_node1,5,4,test_book_arr);
   /*all moves impossible*/
   assert(test_book_arr->size==0);
   free_bookcase_arr(test_book_arr);
   free(test_node1);
}


int len_shelf_full(int row,int num_cols,colours bookcase[MAXSIZE][MAXSIZE])
{
   int i;
   int count;
   count=0;
   for(i=0;(i<num_cols) && (bookcase[row][i]!=empty) ;i++)
   {
      count+=1;
   }
   return count;
}


/*only including start_row
 so can do multiple tests on one board*/

bool is_happy(int num_rows,int num_cols,\
            int start_row,colours bookcase[MAXSIZE][MAXSIZE])
{
   int i,j;
   colours* colour_track;
   colour_track=(colours*)safe_calloc(num_rows,sizeof(colours));
   for(i=start_row;i<num_rows;i++)
   {
       /*something has gone wrong if an empty is before a colour
       will check for that in seperate func bc of this safe to
      assign first item to the tracker*/
      colour_track[i]=bookcase[i][0];
      for(j=0;j<num_cols;j++)
      {
         if(bookcase[i][j]!=colour_track[i]&&bookcase[i][j]!=empty)
         {
            free(colour_track);
            return false;
         }
      }
   }

   for(i=0;i<num_rows-1;i++)
   {
      for(j=i+1;j<num_rows;j++)
      {
         if((colour_track[i]==colour_track[j])&&(colour_track[i]!=empty))
         {
            free(colour_track);
            return false;
         }
      }
   }

   free(colour_track);
   return true;
}

/*return void then cast when appropriate*/
void* safe_calloc(size_t nitems, size_t size)
{
   void* ptr;
   ptr = calloc(nitems, size);
   if(ptr==NULL)
   {
      fprintf(stderr,"no space\n");
   	exit(EXIT_FAILURE);
   }
   return ptr;
}


void board_to_str(colours board[MAXSIZE][MAXSIZE],\
                  char* str,int num_rows,int num_cols)
{
   int i,j;

   str[0]='\0';

   for(i=0;i<num_rows;i++)
   {
      for(j=0;j<num_cols;j++)
      {

         strcat(str,num_to_letter(board[i][j]));
      }
      strcat(str,"\n");
   }


}


char* num_to_letter(colours c)
{
   switch(c)
   {
      case(0):
      return ".";
      case(1):
      return "R";
      case(2):
      return "G";
      case(3):
      return "Y";
      case(4):
      return "B";
      case(5):
      return "M";
      case(6):
      return "C";
      case(7):
      return "W";
      case(8):
      return "K";
      default:
      fprintf(stderr,"invalid colour in board\n");
   	exit(EXIT_FAILURE);
   }
}


bool legal_move(int start_row,int target_row, colours board[MAXSIZE][MAXSIZE],\
               int num_rows,int num_cols)
{

   int book_index;
   if(start_row==target_row)
   {
      /*this is not a move +will lead to waste*/
      return false;
   }
   /*not sure when this would happen but still check*/
   if(start_row>=num_rows||target_row>=num_rows||start_row<0||target_row<0)
   {
      return false;
   }
   if(len_shelf_full(target_row,num_cols,board)==num_cols)
   {
      return false;
   }
   book_index=len_shelf_full(start_row,num_cols,board)-1;

   /*empty shelf*/
   if(book_index<0)
   {
      return false;
   }

   return true;
}

nodeptr make_move(int start_row,int target_row,\
            nodeptr parent_bookcase,int num_rows,int num_cols)
{
   nodeptr child;
   int start_row_pos,target_row_pos;

   /*may put this earlier on in process to speed up*/
   if(legal_move(start_row,target_row,parent_bookcase->bookcase,\
               num_rows,num_cols))
   {
      start_row_pos=len_shelf_full(start_row,num_cols,\
                              parent_bookcase->bookcase);
      target_row_pos=len_shelf_full(target_row,num_cols,\
                              parent_bookcase->bookcase);

      child=init_copy_node(parent_bookcase);

      child->bookcase[target_row][target_row_pos]=\
      child->bookcase[start_row][start_row_pos-1];

      child->bookcase[start_row][start_row_pos-1]=empty;
      return child;
   }
   else
   {
      return NULL;
   }
}

/*initialises a copy of parent board as a child
if no parent then just returns a NULL ->not sure
when this would happen but just in case*/
nodeptr init_copy_node(nodeptr parent_bookcase)
{
   nodeptr child;
   if(parent_bookcase)
   {
      parent_bookcase->num_children+=1;

      child=(nodeptr)safe_calloc(1,sizeof(node));
      child->parent=parent_bookcase;
      child->num_children=0;
      memcpy(child->bookcase,parent_bookcase->bookcase,\
               MAXSIZE*MAXSIZE*sizeof(colours));
      return child;
   }

   return NULL;
}

nodeptr create_orig_node(colours board[MAXSIZE][MAXSIZE])
{
   nodeptr origin;

   origin=(nodeptr)safe_calloc(1,sizeof(node));
   origin->parent=NULL;
   origin->num_children=0;

   memcpy(origin->bookcase,board,MAXSIZE*MAXSIZE*sizeof(colours));
   return origin;
}


void add_bookcase(bookcase_arr* books_arr,nodeptr to_add)
{
   /*dont add nulls*/
   if(to_add)
   {
      books_arr->array[books_arr->size]=to_add;
      books_arr->size+=1;
      if(books_arr->size==books_arr->capacity)
      {
         books_arr->array=(nodeptr*)realloc(books_arr->array,\
                     sizeof(nodeptr)*books_arr->capacity*SCALEFACTOR);
         if(books_arr->array==NULL)
         {
            fprintf(stderr,"no space\n");
      	 	exit(EXIT_FAILURE);
         }
         books_arr->capacity=books_arr->capacity*SCALEFACTOR;
      }
   }
}

bookcase_arr* init_bookcase_arr(void)
{
   bookcase_arr* b_arr;
   b_arr=(bookcase_arr*)safe_calloc(1,sizeof(bookcase_arr));
   b_arr->array=(nodeptr*)safe_calloc(INITLEN,sizeof(nodeptr));
   b_arr->capacity=INITLEN;
   b_arr->size=0;
   return b_arr;
}


bool free_bookcase_arr(bookcase_arr* b_arr)
{
   int i;
   if(b_arr)
   {

      for(i=0;i<b_arr->size;i++)
      {

         free(b_arr->array[i]);
      }
      free(b_arr->array);
      free(b_arr);
   }
   return true;
}


void iterate_one_board(nodeptr parent_bookcase,\
            int num_rows,int num_cols,bookcase_arr* bk_arr)
{
   int start_row,target_row;
   nodeptr child;
   for(start_row=0;start_row<num_rows;start_row++)
   {
      for(target_row=0;target_row<num_rows;target_row++)
      {
         /*legal move inside make move already checks for
            same row start->target may be faster to put the
            legal move check here but harder to test for
             */

         child=make_move(start_row,target_row,parent_bookcase,\
                           num_rows,num_cols);
         if(child)
         {
            add_bookcase(bk_arr,child);
         }

      }
   }


}
