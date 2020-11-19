#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <string.h>
#include <assert.h>
#include <ctype.h>

#define NUMCOLOURS 8
#define INITLEN 10
#define SCALEFACTOR 2
#define MAXSIZE 9
#define STARTROW 0
#define MAXSTRLEN 91
#define MAXBKS 600000
#define BUFFERSIZE 100

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
when increaseing capacity but then again we are callocing a lot
more so speed difference negligable.

The thing that clinches it for me is I can return NULLS
if boards are invalid/ sometimes I have a check that returns
a board if it is happy else returns NULL which is beneficial.
The alternative i think would be having loads of void functions
which are slightly harder to test. Eg if im testing what would
happen if i tried to make a illegal move in my code i can just
see if it returns a NULL. I guess you could make the void functions
into boolean functions but the benefit of returning a pointer
rather than a boolean means that you can seperate functions out based
on their jobs eg for this function -


to make it into a boolean youd have to put in another bit that
puts the bookcase into the array as well which isnt that
bad but just means its a bit longer to test.


Also I've tried putting creating pointers to structs in their
 own functions anyway so its pretty much abstracted
out of the rest of the code (I hope anyway)*/
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

/*mostly for testing what bookcases look like*/
char* num_to_letter(colours c);
void board_to_str(colours bookcase[MAXSIZE][MAXSIZE],\
                  char* str,int num_rows,int num_cols);

/*return void then cast when appropriate*/
void* safe_calloc(size_t nitems, size_t size);

/*deals with trying to move empties
  moving to full row
  trying to move into same row
  invalid row positions*/
bool legal_move(int start_row,int target_row, colours bookcase[MAXSIZE][MAXSIZE],\
               int num_rows,int num_cols);

/*from an original board create first node*/
nodeptr create_orig_node(colours bookcase[MAXSIZE][MAXSIZE]);

/*create a copy of a node and increase the num
of children in parent bookcase by 1*/
nodeptr init_copy_node(nodeptr parent_bookcase);

/*if legal moves a book from start row to target row*/
nodeptr make_move(int start_row,int target_row,\
            nodeptr parent_bookcase,int num_rows,int num_cols);

/*add a bookcase to the bookcase array*/
void add_bookcase(bookcase_arr* books_arr,nodeptr to_add);
bookcase_arr* init_bookcase_arr(void);


nodeptr iterate_one_board(nodeptr parent_bookcase,\
            int num_rows,int num_cols,bookcase_arr* bk_arr);
nodeptr go_through_boards(bookcase_arr* bk_arr,int num_rows,int num_cols,\
                  colours init_bookcase[MAXSIZE][MAXSIZE],int max_iters);


bool free_bookcase_arr(bookcase_arr* b_arr);

/*enumed the colours so can just create a bool hist
 and use the colours as indexes*/
int count_num_colours(colours init_bookcase[MAXSIZE][MAXSIZE],\
                     int num_rows,int num_cols);

int count_colour(colours init_bookcase[MAXSIZE][MAXSIZE],\
                     int num_rows,int num_cols,colours colour);

bool impossible_start(colours init_bookcase[MAXSIZE][MAXSIZE],\
                     int num_rows,int num_cols);

bool colour_impossibility(colours init_bookcase[MAXSIZE][MAXSIZE],\
                     int num_rows,int num_cols);
void print_lineage(nodeptr happy_board,bool verbose,\
                  int num_rows,int num_cols);

bool oscillating(bookcase_arr* bk_arr,int num_rows,int num_cols,int pos);
bool oscilating_counter(bookcase_arr* bk_arr,int num_rows,\
                  int num_cols,int pos,int* count);
colours letter_to_num(char c);

bool fill_from_file(colours empty_bookcase[MAXSIZE][MAXSIZE],\
                  int* num_rows,int* num_cols,char* filename);

void test(void);

int main(void)
{
   test();
   return 0;
}

void test(void)
{
   int i;
   nodeptr test_node1, test_node2,clean_test;
   int num_rows,num_cols;
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


   colours test_bc_simple_4[MAXSIZE][MAXSIZE]={{red,red,red,empty},
                                                {green,green,blue,empty},
                                                {red,green,empty,empty},
                                                {empty,empty,empty,empty},
                                                {blue,empty,empty,empty}};

   /*going to consider empty boards happy*/
   colours test_bc_empty[MAXSIZE][MAXSIZE]={{empty,empty,empty,empty},
                                             {empty,empty,empty,empty},
                                             {empty,empty,empty,empty},
                                             {empty,empty,empty,empty},
                                             {empty,empty,empty,empty}};

   colours test_not_imposs[MAXSIZE][MAXSIZE]={{red,red,red,empty},
                                          {green,green,green,red}};

   colours test_imposs[MAXSIZE][MAXSIZE]={{red,red,red,empty},
                                          {green,green,red,red}};

   colours test_imposs_2[MAXSIZE][MAXSIZE]={{red,yellow,empty,empty},
                                          {green,empty,empty,empty}};

   colours test_oscillations[MAXSIZE][MAXSIZE]={{red,red,green,empty},
                                          {green,green,green,red}};

   char test_str[200];

   assert(count_colour(test_bc_empty,5,4,empty)==20);
   assert(count_colour(test_bc_simple_4,5,4,empty)==11);
   assert(count_colour(test_bc_simple_3,5,4,empty)==0);
   assert(count_colour(test_bc_simple_2,5,4,empty)==12);

   assert(count_num_colours(test_bc_simple_4,4,4)==3);
   assert(count_num_colours(test_bc_simple_3,5,4)==1);
   assert(count_num_colours(test_bc_simple,7,4)==1);
   assert(count_num_colours(test_bc,7,4)==5);
   assert(count_num_colours(test_bc_empty,4,4)==0);

   assert(len_shelf_full(0,4,test_bc)==1);
   assert(len_shelf_full(1,4,test_bc)==2);
   assert(len_shelf_full(2,4,test_bc)==3);
   assert(len_shelf_full(4,4,test_bc)==0);
   assert(len_shelf_full(5,4,test_bc)==4);

   assert(is_happy(4,4,0,test_bc_empty)==true);
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


   assert(colour_impossibility(test_bc_simple_3,5,4)==true);
   assert(colour_impossibility(test_bc_simple,7,4)==true);
   assert(colour_impossibility(test_bc_simple,3,4)==false);
   assert(colour_impossibility(test_bc_empty,5,4)==false);
   assert(colour_impossibility(test_not_imposs,2,4)==false);
   assert(colour_impossibility(test_bc,7,4)==true);


   assert(impossible_start(test_imposs_2,2,4)==true);
   assert(impossible_start(test_imposs,2,4)==true);
   assert(impossible_start(test_bc,7,4)==true);
   assert(impossible_start(test_imposs_2,1,4)==true);
   assert(impossible_start(test_imposs_2,1,1)==false);
   assert(impossible_start(test_bc_empty,5,4)==false);
   assert(impossible_start(test_bc,3,4)==true);
   assert(impossible_start(test_bc,1,4)==false);

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

   /*testing initiation of bookcase array*/
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


   /*testing oscillating bookcase and iterate one board*/

   test_node1=create_orig_node(test_bc_simple);
   test_book_arr=init_bookcase_arr();
   iterate_one_board(test_node1,7,4,test_book_arr);
   /* 7*6 moves*/
   assert(test_book_arr->size==42);
   assert(oscillating(test_book_arr,5,4,30)==false);

   free_bookcase_arr(test_book_arr);
   free(test_node1);

   test_node1=create_orig_node(test_bc_simple_2);
   test_book_arr=init_bookcase_arr();
   iterate_one_board(test_node1,5,4,test_book_arr);
   assert(oscillating(test_book_arr,5,4,10)==false);
   i=50;
   assert(oscilating_counter(test_book_arr,5,4,10,&i)==false);
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

   /*testing a ocilating bookcase==true*/
   test_book_arr=init_bookcase_arr();
   test_node1=create_orig_node(test_oscillations);
   add_bookcase(test_book_arr,test_node1);
   for(i=0;i<1000;i++)
   {
      test_node2=iterate_one_board(test_book_arr->array[i]\
                                    ,2,4,test_book_arr);
   }
   assert(oscillating(test_book_arr,2,4,50)==true);
   assert(oscillating(test_book_arr,2,4,5)==true);
   /*counter doesnt check if count under 50*/
   i=10;
   assert(oscilating_counter(test_book_arr,2,4,50,&i)==false);

   i=50;
   assert(oscilating_counter(test_book_arr,2,4,50,&i)==true);
   /*bool oscilating_counter(bookcase_arr* bk_arr,int num_rows,\
                     int num_cols,int pos,int* count)*/
   free_bookcase_arr(test_book_arr);

   test_book_arr=init_bookcase_arr();
   assert(oscillating(test_book_arr,0,0,50)==false);
   assert(oscilating_counter(test_book_arr,0,0,50,&i)==false);
   free_bookcase_arr(test_book_arr);

   /*testing go through boards*/

   /*working board*/
   test_book_arr=init_bookcase_arr();
   clean_test=go_through_boards(test_book_arr,5,4,test_bc_simple_4,MAXBKS);
   assert(clean_test!=NULL);
   assert(is_happy(5,4,0,clean_test->bookcase));
   free_bookcase_arr(test_book_arr);

   /*oscillating board*/
   test_book_arr=init_bookcase_arr();
   clean_test=go_through_boards(test_book_arr,2,4,test_oscillations,1000);
   assert(clean_test==NULL);
   free_bookcase_arr(test_book_arr);

   /*impossible board from start*/
   test_book_arr=init_bookcase_arr();
   clean_test=go_through_boards(test_book_arr,5,4,test_bc_simple_3,1000);
   assert(clean_test==NULL);
   free_bookcase_arr(test_book_arr);



   /*reading in files*/
   fill_from_file(test_bc_simple_3,&num_rows,&num_cols,"rrggccyy-437.bc");
   assert(num_rows==4);
   assert(num_cols==3);
   board_to_str(test_bc_simple_3,test_str,num_rows,num_cols);
   assert(strcmp(test_str,"RG.\nGR.\nCY.\nYC.\n")==0);

   /*tested for invalid chars in file and
   it throws an invalid colour error*/
   /*fill_from_file(test_bc_simple_3,&num_rows,&num_cols,"eng_370k_shuffle.txt");*/
   
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
         if(bookcase[i][j]!=colour_track[i]&&\
            bookcase[i][j]!=empty)
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
         if((colour_track[i]==colour_track[j])&&\
            (colour_track[i]!=empty))
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
      fprintf(stderr,"No Solution?\n");
   	exit(EXIT_FAILURE);
   }
   return ptr;
}


void board_to_str(colours bookcase[MAXSIZE][MAXSIZE],\
                  char* str,int num_rows,int num_cols)
{
   int i,j;

   str[0]='\0';

   for(i=0;i<num_rows;i++)
   {
      for(j=0;j<num_cols;j++)
      {

         strcat(str,num_to_letter(bookcase[i][j]));
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
      fprintf(stderr,"invalid colour in bookcase\n");
   	exit(EXIT_FAILURE);
   }
}


bool legal_move(int start_row,int target_row, \
            colours bookcase[MAXSIZE][MAXSIZE],\
               int num_rows,int num_cols)
{

   int book_index;
   if(start_row==target_row)
   {
      /*this is not a move +will lead to waste*/
      return false;
   }
   /*not sure when this would happen but still check*/
   if(start_row>=num_rows||target_row>=num_rows\
      ||start_row<0||target_row<0)
   {
      return false;
   }
   if(len_shelf_full(target_row,num_cols,bookcase)==num_cols)
   {
      return false;
   }
   /*len shelf returns size need to minus 1 to get index*/
   book_index=len_shelf_full(start_row,num_cols,bookcase)-1;

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

   if(legal_move(start_row,target_row,\
               parent_bookcase->bookcase,num_rows,num_cols))
   {
      start_row_pos=len_shelf_full(start_row,num_cols,\
                              parent_bookcase->bookcase);

      target_row_pos=len_shelf_full(target_row,num_cols,\
                              parent_bookcase->bookcase);

      child=init_copy_node(parent_bookcase);
      /*start/target pos gets the first empty
      so need to minus 1 to find the end book*/
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
      parent_bookcase->num_children=\
      parent_bookcase->num_children+1;

      child=(nodeptr)safe_calloc(1,sizeof(node));
      child->parent=parent_bookcase;
      child->num_children=0;
      memcpy(child->bookcase,parent_bookcase->bookcase,\
               MAXSIZE*MAXSIZE*sizeof(colours));
      return child;
   }

   return NULL;
}

nodeptr create_orig_node(colours bookcase[MAXSIZE][MAXSIZE])
{
   nodeptr origin;

   origin=(nodeptr)safe_calloc(1,sizeof(node));
   origin->parent=NULL;
   origin->num_children=0;

   memcpy(origin->bookcase,bookcase,\
         MAXSIZE*MAXSIZE*sizeof(colours));
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
            fprintf(stderr,"No Solution?\n");
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
      /*free all the boards*/
      for(i=0;i<b_arr->size;i++)
      {
         free(b_arr->array[i]);
      }
      /*free the array then free the whole struct*/
      free(b_arr->array);
      free(b_arr);
   }
   return true;
}


nodeptr iterate_one_board(nodeptr parent_bookcase,\
            int num_rows,int num_cols,bookcase_arr* bk_arr)
{
   int start_row,target_row;
   nodeptr child;
   for(start_row=0;start_row<num_rows;start_row++)
   {
      for(target_row=0;target_row<num_rows;target_row++)
      {
         /*legal move inside make move already checks for
            same row start->target, returns NULL if illegal
             */
         child=make_move(start_row,target_row,parent_bookcase,\
                           num_rows,num_cols);
         /*checks for NULLS*/
         if(child)
         {

            add_bookcase(bk_arr,child);
            /*if happy can stop creating bookcases*/
            if(is_happy(num_rows,num_cols,STARTROW,\
                        child->bookcase))
            {
               return child;
            }
         }
      }
   }
   return NULL;
}

nodeptr go_through_boards(bookcase_arr* bk_arr,\
                        int num_rows,int num_cols,\
      colours init_bookcase[MAXSIZE][MAXSIZE],int max_iters)
{
   int i,count;
   nodeptr happy,orig;
   orig=create_orig_node(init_bookcase);
   add_bookcase(bk_arr,orig);
   count=0;
   for(i=0;i<bk_arr->size;i++)
   {
      if(oscilating_counter(bk_arr,num_rows,\
                        num_cols,i,&count))
      {
         return NULL;
      }

      if(i==max_iters)
      {
         return NULL;
      }
      if(is_happy(num_rows,num_cols,STARTROW,\
                  bk_arr->array[i]->bookcase))
      {
         return bk_arr->array[i];
      }
      /*if an iteration of current board is happy
      then skip straight to it and return */
      happy=iterate_one_board(bk_arr->array[i],\
                        num_rows,num_cols,bk_arr);
      if(happy)
      {
         return happy;
      }
   }
   return NULL;
}


/*doesnt matter which order i do the if statements in
since they all need to be fufilled*/
bool impossible_start(colours init_bookcase[MAXSIZE][MAXSIZE],\
                     int num_rows,int num_cols)
{
   int num_colours;
   int num_empty_spaces;
   num_colours=count_num_colours(init_bookcase,\
                              num_rows,num_cols);
   if(num_colours>num_rows)
   {
      return true;
   }
   if(colour_impossibility(init_bookcase, num_rows,num_cols))
   {
      return true;
   }
   num_empty_spaces=count_colour(init_bookcase,\
                        num_rows,num_cols,empty);
   if(num_empty_spaces==0)
   {
      if(!is_happy(num_rows,num_cols,\
                  STARTROW,init_bookcase))
      {
         return true;
      }
   }
   return false;
}

/*checking to see if a bookcase is oscilating from two positions*/
bool oscillating(bookcase_arr* bk_arr,int num_rows,\
                  int num_cols,int pos)
{
   int i;
   char bookcase_1[MAXSTRLEN];
   char bookcase_2[MAXSTRLEN];
   char temp_case[MAXSTRLEN];
   if(bk_arr->size<=pos)
   {
      return false;
   }
   board_to_str(bk_arr->array[pos]->bookcase,\
               bookcase_1,num_rows,num_cols);
   board_to_str(bk_arr->array[pos-1]->bookcase,\
               bookcase_2,num_rows,num_cols);
   /*check last 50 positions - if all duplicates then
      bookcase is oscillating*/
   for(i=pos; i>=0 && i>pos-50 ;i--)
   {
      board_to_str(bk_arr->array[i]->bookcase,\
                  temp_case,num_rows,num_cols);
      if((strcmp(temp_case,bookcase_1)!=0)&&\
         (strcmp(temp_case,bookcase_2)!=0))
      {
         return false;
      }
   }
   return true;
}

bool oscilating_counter(bookcase_arr* bk_arr,int num_rows,\
                  int num_cols,int pos,int* count)
{
   /*checking out of bounds*/
   if(bk_arr->size<=pos)
   {
      return false;
   }
   if(*count<50)
   {
      if(pos>0)
      {

         if(bk_arr->array[pos-1]->num_children==1)
         {
            *count+=1;
         }
      }
      return false;
   }
   else
   {
      if(oscillating(bk_arr,num_rows,num_cols,pos))
      {
         return true;
      }
      else
      {
         *count=0;
         return false;
      }
   }
}

bool colour_impossibility(colours init_bookcase[MAXSIZE][MAXSIZE],\
                     int num_rows,int num_cols)
{
   int i;
   for(i=red;i<=black;i++)
   {
      if(count_colour(init_bookcase,num_rows,num_cols,i)>num_cols)
      {
         return true;
      }
   }
   return false;
}


int count_colour(colours init_bookcase[MAXSIZE][MAXSIZE],\
                     int num_rows,int num_cols,colours colour)
{
   int i,j,count;
   count=0;
   for(i=0;i<num_rows;i++)
   {
      for(j=0;j<num_cols;j++)
      {
         if(init_bookcase[i][j]==colour)
         {
            count+=1;
         }
      }

   }
   return count;
}
/*enumed the colours so can just create a bool hist
 and use the colours as indexes*/
int count_num_colours(colours init_bookcase[MAXSIZE][MAXSIZE],\
                     int num_rows,int num_cols)
{
   int i,j,count;
   bool colour_hist[NUMCOLOURS]={false};

   for(i=0;i<num_rows;i++)
   {
      for(j=0;j<num_cols;j++)
      {
         if(init_bookcase[i][j]!=empty)
         {
            /*minus 1 as 0 indexed*/
            colour_hist[init_bookcase[i][j]-1]=true;
         }
      }
   }
   count=0;
   for(i=0;i<NUMCOLOURS;i++)
   {
      count+=(int)colour_hist[i];
   }
   return count;

}


void print_lineage(nodeptr happy_board,bool verbose,\
                  int num_rows,int num_cols)
{
   int count;
   char temp_str[MAXSTRLEN];
   count=0;

   while(happy_board->parent!=NULL)
   {
      count+=1;
      if(verbose)
      {
         board_to_str(happy_board->bookcase,temp_str,\
                     num_rows,num_cols);

         printf("%d\n",count);
         printf("%s",temp_str);
         printf("\n");

         happy_board=happy_board->parent;
      }
   }
   if(!verbose)
   {
      printf("%d\n",count);
   }
}

colours letter_to_num(char c)
{
   switch(toupper(c))
   {
      case('.'):
      return (colours) 0;
      case('R'):
      return (colours) 1;
      case('G'):
      return (colours) 2;
      case('Y'):
      return (colours) 3;
      case('B'):
      return (colours) 4;
      case('M'):
      return (colours) 5;
      case('C'):
      return (colours) 6;
      case('W'):
      return (colours) 7;
      case('K'):
      return (colours) 8;
      default:
      fprintf(stderr,"invalid colour in bookcase\n");
   	exit(EXIT_FAILURE);
   }
}

bool fill_from_file(colours empty_bookcase[MAXSIZE][MAXSIZE],\
                  int* num_rows,int* num_cols,char* filename)
{
   FILE *fp;
   int i,n_rows,n_cols,curr_row;
   char buffer[BUFFERSIZE];
   fp= fopen(filename,"r");
   if(fp==NULL)
   {
      fprintf(stderr,"error while opening file\n");
      exit(EXIT_FAILURE);
   }

   if(fgets(buffer,BUFFERSIZE,fp)==NULL)
   {
      fprintf(stderr,"file empty\n");
      exit(EXIT_FAILURE);
   }
   sscanf(buffer, "%d %d",&n_rows,&n_cols);
   *num_rows= n_rows;
   *num_cols=n_cols;
   /*assuming here there will be a bookcase*/
   if(n_rows>9||n_rows<1)
   {
      fprintf(stderr,"invalid number of rows\n");
      exit(EXIT_FAILURE);
   }
   if(n_cols>9||n_cols<1)
   {
      fprintf(stderr,"invalid number of cols\n");
      exit(EXIT_FAILURE);
   }

   curr_row=0;
   while(fgets(buffer,BUFFERSIZE,fp)!=NULL)
   {
      for(i=0;i<n_cols;i++)
      {
         empty_bookcase[curr_row][i]=letter_to_num(buffer[i]);
      }
      curr_row+=1;
   }
   return true;
}

/*rrggccyy-437.bc*/
