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
#define MAXOSCIL 50

#define MAXSTRLEN (MAXSIZE*(MAXSIZE+1)+1)
#define MAXBKS 1000000
#define BUFFERSIZE 100

typedef enum colours {empty,red, green,yellow,blue,magenta,cyan,white,black} colours;
typedef enum bool {false, true} bool;


typedef struct node
{
   colours bookcase[MAXSIZE][MAXSIZE];
   struct node* parent;
   int num_children;
   int num_rows;
   int num_cols;
}node;

typedef node* nodeptr;


typedef struct bookcase_arr{
   int size;
   int capacity;
   nodeptr* array;
}bookcase_arr;



/*win conditions*/
bool is_happy(int start_row,nodeptr bk_container);
bool duplicate_colour_rows(colours* array,int array_size);
/*how full is one bookcase shelf -
couldnt think of a better name sorry*/
int len_shelf_full(int row,nodeptr bk_container);


/*
legallity_checker_init() */

/*mostly for testing what bookcases look like*/
char* num_to_letter(colours c);
void bookcase_to_str(nodeptr bk_container,char* str);

/*return void then cast when appropriate*/
void* safe_calloc(size_t nitems, size_t size);

/*deals with trying to move empties
  moving to full row
  trying to move into same row
  +invalid row positions*/
bool legal_move(int start_row,int target_row, \
            nodeptr bk_container);

/*from an original bookcase create first node*/
nodeptr create_orig_node(colours bookcase[MAXSIZE][MAXSIZE],\
                        int num_rows,int num_cols);

/*create a copy of a node and increase the num
of children in parent bookcase by 1*/
nodeptr init_copy_node(nodeptr parent_bookcase);

/*if legal creates a new bookcase with book moved
to target row otherwise returns NULL*/
nodeptr make_move(int start_row,int target_row,\
            nodeptr parent_bookcase);

/*add a bookcase to the bookcase array*/
void add_bookcase(bookcase_arr* books_arr,nodeptr to_add);

bookcase_arr* init_bookcase_arr(void);

/*make all possible moves with one bookcase*/
nodeptr iterate_one_bookcase(nodeptr parent_bookcase,bookcase_arr* bk_arr);

/*driver func to iterate through array of bookcases*/
nodeptr go_through_bookcases(bookcase_arr* bk_arr,\
                        nodeptr orig_bookcase,int max_iters);

/*first frees all the bookcase containers
then frees array
then frees the struct itself*/
bool free_bookcase_arr(bookcase_arr* b_arr);


/*enumed the colours so can just create a bool hist
 and use the colours as indexes*/
int count_num_colours(nodeptr bk_container);
int count_colour(nodeptr bk_container,colours colour);

/*considers basic impossibilities - doesnt cover
everything */
bool impossible_start(nodeptr bk_container);
bool colour_impossibility(nodeptr bk_container);


bool empties_before_colour(nodeptr bk_container);

void print_lineage(nodeptr happy_bookcase,bool verbose);

/*returns -1 if NULL*/
int num_of_gens(nodeptr happy_bookcase);
/*to find impossible bookcases one of the checks will be
no (simple)oscillating bookcases - eg move to a free shelf
 then move back again *a million - to do this
 want to check if only one child and if so check parent
  bookcases to see repeated */
bool oscillating(bookcase_arr* bk_arr,int pos);
/*counts to 50 single children in a row
then sets of oscillating checker*/
bool oscilating_counter(bookcase_arr* bk_arr,int pos,int* count);

colours letter_to_num(char c);

/*get num rows and cols*/
bool get_sizes(FILE *fp,int* num_rows,int* num_cols);
/*creates a node from a file*/
nodeptr fill_from_file(char* filename);

void test(void);

int main(int argc,char* argv[])
{
   bool verbose;
   nodeptr origin,solution;
   bookcase_arr* bk_arr;
   test();
   if(argc<2||argc>3)
   {
      fprintf(stderr,"wrong number of arguments please try again\n");
      exit(EXIT_FAILURE);
   }
   origin=fill_from_file(argv[1]);
   if(empties_before_colour(origin))
   {
      fprintf(stderr,"invalid file- empties occur before books\n");
      exit(EXIT_FAILURE);
   }
   bk_arr=init_bookcase_arr();
   solution=go_through_bookcases(bk_arr,origin,MAXBKS);
   if(solution)
   {
      if(argc==3)
      {
         verbose=(strcmp(argv[2],"verbose")==0);
      }
      else
      {
         verbose=false;
      }
      print_lineage(solution,verbose);
   }
   else
   {
      printf("No Solution?\n");
      free_bookcase_arr(bk_arr);
      return 0;
   }
   free_bookcase_arr(bk_arr);
   return 0;
}

void test(void)
{
   /*have commented out vars used for testing files
   and those asserts as well just in case you dont have
   the files*/
   int i;/*,num_rows,num_cols;*/
   nodeptr test_node1, test_node2,clean_test,temp;
   /*FILE *fp;*/
   bookcase_arr* test_book_arr;

   colours test_dupes[7]={red,green,empty,empty,blue,white,red};

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

   /*going to consider empty bookcases happy*/
   colours test_bc_empty[MAXSIZE][MAXSIZE]={{empty,empty,empty,empty},
                                             {empty,empty,empty,empty},
                                             {empty,empty,empty,empty},
                                             {empty,empty,empty,empty},
                                             {empty,empty,empty,empty}};

   colours test_not_imposs[MAXSIZE][MAXSIZE]={{red,red,red,empty},
                                          {green,green,green,red}};

   colours test_not_imposs2[MAXSIZE][MAXSIZE]={{red,red,red,red},
                                          {green,green,green,green}};

   colours test_imposs[MAXSIZE][MAXSIZE]={{red,red,red,empty},
                                          {green,green,red,red}};

   colours test_imposs_2[MAXSIZE][MAXSIZE]={{red,yellow,empty,empty},
                                          {green,empty,empty,empty}};

   colours test_oscillations[MAXSIZE][MAXSIZE]={{red,red,green,empty},
                                          {green,green,green,red}};

   colours test_empties1[MAXSIZE][MAXSIZE]={{red,empty,green,empty},
                                          {green,green,green,red}};

   colours test_empties2[MAXSIZE][MAXSIZE]={{empty,empty,green,empty},
                                          {green,green,green,red}};

   colours test_empties3[MAXSIZE][MAXSIZE]={{green,green,green,empty},
                                          {empty,green,green,red}};

   char test_str[200];

   test_node1=create_orig_node(test_bc_empty,5,4);
   assert(count_colour(test_node1,empty)==20);
   free(test_node1);

   test_node1=create_orig_node(test_bc_simple_4,5,4);
   assert(count_colour(test_node1,empty)==11);
   free(test_node1);

   test_node1=create_orig_node(test_bc_simple_3,5,4);
   assert(count_colour(test_node1,empty)==0);
   free(test_node1);

   test_node1=create_orig_node(test_bc_simple_2,5,4);
   assert(count_colour(test_node1,empty)==12);
   assert(count_colour(test_node1,red)==8);
   assert(count_colour(test_node1,yellow)==0);
   free(test_node1);

   /*not testing to see if it picks up on a empty being
   before a colour as will check for that in beginning*/
   test_node1=create_orig_node(test_bc_simple_4,4,4);
   assert(count_num_colours(test_node1)==3);
   free(test_node1);
   test_node1=create_orig_node(test_bc_simple_3,5,4);
   assert(count_num_colours(test_node1)==1);
   free(test_node1);
   test_node1=create_orig_node(test_bc_simple,7,4);
   assert(count_num_colours(test_node1)==1);
   free(test_node1);
   test_node1=create_orig_node(test_bc,7,4);
   assert(count_num_colours(test_node1)==5);
   free(test_node1);
   test_node1=create_orig_node(test_bc_empty,4,4);
   assert(count_num_colours(test_node1)==0);
   free(test_node1);

   test_node1=create_orig_node(test_bc,7,4);
   assert(len_shelf_full(0,test_node1)==1);
   assert(len_shelf_full(1,test_node1)==2);
   assert(len_shelf_full(2,test_node1)==3);
   assert(len_shelf_full(4,test_node1)==0);
   assert(len_shelf_full(5,test_node1)==4);
   free(test_node1);

   test_node1=create_orig_node(test_bc,7,4);
   test_node2=create_orig_node(test_bc_empty,5,4);
   assert(is_happy(0,test_node2)==true);
   assert(is_happy(3,test_node1)==true);
   assert(is_happy(5,test_node1)==true);
   assert(is_happy(6,test_node1)==true);
   assert(is_happy(2,test_node1)==false);
   assert(is_happy(1,test_node1)==false);
   assert(is_happy(0,test_node1)==false);
   free(test_node1);
   free(test_node2);

   assert(strcmp(num_to_letter(empty),".")==0);
   assert(strcmp(num_to_letter(red),"R")==0);
   assert(strcmp(num_to_letter(black),"K")==0);
   assert(letter_to_num('k')==black);
   assert(letter_to_num('K')==black);
   assert(letter_to_num('.')==empty);

   test_node1=create_orig_node(test_bc,7,4);
   bookcase_to_str(test_node1,test_str);
   assert(strcmp(test_str,"R...\nBY..\nGGG.\nGGG.\n....\nMMMM\n....\n")==0);
   free(test_node1);

   test_node1=create_orig_node(test_bc,7,4);
   assert(legal_move(0,5,test_node1)==false);
   assert(legal_move(50,5,test_node1)==false);
   assert(legal_move(0,50,test_node1)==false);
   assert(legal_move(4,0,test_node1)==false);
   assert(legal_move(0,1,test_node1)==true);
   assert(legal_move(0,0,test_node1)==false);
   assert(legal_move(0,1,NULL)==false);
   free(test_node1);

   test_node1=create_orig_node(test_bc_simple_3,5,4);
   assert(colour_impossibility(test_node1)==true);
   free(test_node1);

   test_node1=create_orig_node(test_bc_simple,7,4);
   assert(colour_impossibility(test_node1)==true);
   free(test_node1);

   test_node1=create_orig_node(test_bc_simple,3,4);
   assert(colour_impossibility(test_node1)==false);
   free(test_node1);

   test_node1=create_orig_node(test_bc_empty,5,4);
   assert(colour_impossibility(test_node1)==false);
   free(test_node1);

   test_node1=create_orig_node(test_not_imposs,2,4);
   assert(colour_impossibility(test_node1)==false);
   free(test_node1);

   test_node1=create_orig_node(test_bc,7,4);
   assert(colour_impossibility(test_node1)==true);
   free(test_node1);

   test_node1=create_orig_node(test_imposs_2,2,4);
   assert(impossible_start(test_node1)==true);
   free(test_node1);

   test_node1=create_orig_node(test_imposs,2,4);
   assert(impossible_start(test_node1)==true);
   free(test_node1);

   test_node1=create_orig_node(test_bc,7,4);
   assert(impossible_start(test_node1)==true);
   free(test_node1);

   test_node1=create_orig_node(test_imposs_2,1,4);
   assert(impossible_start(test_node1)==true);
   free(test_node1);

   test_node1=create_orig_node(test_imposs_2,1,1);
   assert(impossible_start(test_node1)==false);
   free(test_node1);

   test_node1=create_orig_node(test_bc_empty,5,4);
   assert(impossible_start(test_node1)==false);
   free(test_node1);

   test_node1=create_orig_node(test_bc,3,4);
   assert(impossible_start(test_node1)==true);
   free(test_node1);

   test_node1=create_orig_node(test_bc,1,4);
   assert(impossible_start(test_node1)==false);
   free(test_node1);

   test_node1=create_orig_node(test_not_imposs2,2,4);
   assert(colour_impossibility(test_node1)==false);
   free(test_node1);

   test_node1=create_orig_node(test_bc,7,4);
   assert(test_node1->num_children==0);

   test_node2=init_copy_node(test_node1);
   assert(test_node1->num_children==1);
   bookcase_to_str(test_node1,test_str);
   assert(strcmp(test_str,"R...\nBY..\nGGG.\nGGG.\n....\nMMMM\n....\n")==0);
   assert(test_node2->num_children==0);

   bookcase_to_str(test_node2,test_str);
   assert(strcmp(test_str,"R...\nBY..\nGGG.\nGGG.\n....\nMMMM\n....\n")==0);

   free(test_node2);
   free(test_node1);

   assert(init_copy_node(NULL)==NULL);

   test_node1=create_orig_node(test_bc,7,4);
   /*nodeptr make_move(int start_row,int target_row,\
               nodeptr parent_bookcase,int num_rows,int num_cols)*/

   test_node2=make_move(0,1,test_node1);
   bookcase_to_str(test_node2,test_str);
   assert(strcmp(test_str,"....\nBYR.\nGGG.\nGGG.\n....\nMMMM\n....\n")==0);
   free(test_node2);

   test_node2=make_move(5,0,test_node1);
   bookcase_to_str(test_node2,test_str);
   assert(strcmp(test_str,"RM..\nBY..\nGGG.\nGGG.\n....\nMMM.\n....\n")==0);
   free(test_node2);
   /*invalid row*/
   test_node2=make_move(-1,1,test_node1);
   assert(test_node2==NULL);
   free(test_node2);

   /*trying to add a empty to a shelf*/
   test_node2=make_move(6,1,test_node1);
   assert(test_node2==NULL);
   free(test_node2);

   /*adding to full shelf*/
   test_node2=make_move(1,5,test_node1);
   assert(test_node2==NULL);
   free(test_node2);

   /*testing initiation of bookcase array*/
   test_book_arr=init_bookcase_arr();
   assert(test_book_arr->size==0);
   assert(test_book_arr->capacity==INITLEN);

   add_bookcase(test_book_arr,NULL);
   assert(test_book_arr->size==0);
   temp=init_copy_node(test_node1);

   for(i=0;i<10000;i++)
   {
      assert(test_book_arr->size==i);
      do
      {

         test_node2=make_move(rand()%6+0,rand()%6+0,temp);
         assert(empties_before_colour(test_node2)==false);
      } while(!test_node2);
      free(temp);
      temp=init_copy_node(test_node2);
      add_bookcase(test_book_arr,test_node2);
   }
   free_bookcase_arr(test_book_arr);
   free(temp);
   free(test_node1);




   test_node1=create_orig_node(test_bc_simple,7,4);
   test_book_arr=init_bookcase_arr();
   iterate_one_bookcase(test_node1,test_book_arr);
   /* 7*6 moves*/
   assert(test_book_arr->size==42);
   assert(oscillating(test_book_arr,30)==false);

   free_bookcase_arr(test_book_arr);
   free(test_node1);

   test_node1=create_orig_node(test_bc_simple_2,5,4);
   test_book_arr=init_bookcase_arr();
   iterate_one_bookcase(test_node1,test_book_arr);
   assert(oscillating(test_book_arr,10)==false);
   i=50;
   assert(oscilating_counter(test_book_arr,10,&i)==false);
   /*5*4 - 4 for the 4 impossible moves*/
   assert(test_book_arr->size==16);
   free_bookcase_arr(test_book_arr);
   free(test_node1);


   test_node1=create_orig_node(test_bc_simple_3,5,4);
   test_book_arr=init_bookcase_arr();
   iterate_one_bookcase(test_node1,test_book_arr);
   /*all moves impossible*/
   assert(test_book_arr->size==0);
   free_bookcase_arr(test_book_arr);
   free(test_node1);

   /*testing a ocilating bookcase==true*/
   test_book_arr=init_bookcase_arr();
   test_node1=create_orig_node(test_oscillations,2,4);
   add_bookcase(test_book_arr,test_node1);
   for(i=0;i<1000;i++)
   {
      test_node2=iterate_one_bookcase(test_book_arr->array[i],\
                                    test_book_arr);

      assert(empties_before_colour(test_node2)==false);
   }
   assert(oscillating(test_book_arr,50)==true);
   assert(oscillating(test_book_arr,5)==true);
   /*counter doesnt check if count under 50*/
   i=10;
   assert(oscilating_counter(test_book_arr,50,&i)==false);

   i=50;
   assert(oscilating_counter(test_book_arr,50,&i)==true);
   free_bookcase_arr(test_book_arr);

   test_book_arr=init_bookcase_arr();
   assert(oscillating(test_book_arr,50)==false);
   assert(oscilating_counter(test_book_arr,50,&i)==false);
   free_bookcase_arr(test_book_arr);

   /*testing go through bookcases*/

   /*working bookcase*/
   test_book_arr=init_bookcase_arr();
   test_node1=create_orig_node(test_bc_simple_4,5,4);
   clean_test=go_through_bookcases(test_book_arr,test_node1,MAXBKS);
   assert(clean_test!=NULL);
   assert(num_of_gens(clean_test)==4);
   assert(is_happy(0,clean_test));
   free_bookcase_arr(test_book_arr);

   /*oscillating bookcase*/
   test_book_arr=init_bookcase_arr();
   test_node1=create_orig_node(test_oscillations,2,4);
   clean_test=go_through_bookcases(test_book_arr,test_node1,1000);
   assert(clean_test==NULL);
   free_bookcase_arr(test_book_arr);

   /*impossible bookcase from start*/
   test_book_arr=init_bookcase_arr();
   test_node1=create_orig_node(test_bc_simple_3,5,4);
   clean_test=go_through_bookcases(test_book_arr,test_node1,1000);
   assert(clean_test==NULL);
   assert(num_of_gens(clean_test)==-1);
   free_bookcase_arr(test_book_arr);



   /*fp= fopen("rrggccyy-437.bc","r");
   if(fp==NULL)
   {
      fprintf(stderr,"error while opening file in testing\n");
      exit(EXIT_FAILURE);
   }

   num_rows=num_cols=0;
   assert(get_sizes(fp,&num_rows,&num_cols));
   assert(num_rows==4);
   assert(num_cols==3);*/

   /*reading in files test*/
   /*test_node1=fill_from_file("rrggccyy-437.bc");
   assert(test_node1->num_rows==4);
   assert(test_node1->num_cols==3);

   bookcase_to_str(test_node1,test_str);
   assert(strcmp(test_str,"RG.\nGR.\nCY.\nYC.\n")==0);
   free(test_node1);*/

   /*tested for invalid chars in file and
   it throws an invalid colour error*/
   /*fill_from_file("eng_370k_shuffle.txt");*/

   /*tested for invalid num rows in file and
   it throws an invalid number of rows error*/
   /*fill_from_file("norows.txt");*/



   test_node1=create_orig_node(test_empties1,2,4);
   assert(empties_before_colour(test_node1)==true);
   free(test_node1);

   test_node1=create_orig_node(test_empties2,2,4);
   assert(empties_before_colour(test_node1)==true);
   free(test_node1);

   test_node1=create_orig_node(test_empties3,2,4);
   assert(empties_before_colour(test_node1)==true);
   free(test_node1);
   /*colours test_dupes[7]={red,green,empty,empty,blue,white,red};*/

   assert(duplicate_colour_rows(test_dupes,1)==false);
   assert(duplicate_colour_rows(test_dupes,4)==false);
   assert(duplicate_colour_rows(test_dupes,7)==true);
}


int len_shelf_full(int row,nodeptr bk_container)
{
   int x;
   int count;
   count=0;
   for(x=0;(x<bk_container->num_cols) && \
   (bk_container->bookcase[row][x]!=empty) ;x++)
   {
      count+=1;
   }
   return count;
}


/*only including start_row
 so can do multiple tests on one bookcase*/

bool is_happy(int start_row,nodeptr bk_container)
{
   int y,x;
   colours* colour_track;
   colour_track=(colours*)safe_calloc(bk_container->num_rows,\
                                    sizeof(colours));
   for(y=start_row;y<bk_container->num_rows;y++)
   {
       /*something has gone wrong if an empty is before a colour
       will check for that in seperate func bc of this safe to
      assign first item to the tracker*/
      colour_track[y]=bk_container->bookcase[y][0];
      for(x=0;x<bk_container->num_cols;x++)
      {
         if(bk_container->bookcase[y][x]!=colour_track[y]&&\
            bk_container->bookcase[y][x]!=empty)
         {
            free(colour_track);
            return false;
         }
      }
   }
   if(duplicate_colour_rows(colour_track,bk_container->num_rows))
   {
      free(colour_track);
      return false;
   }
   free(colour_track);
   return true;
}

bool duplicate_colour_rows(colours* array,int array_size)
{
   int y,x;
   for(y=0;y<array_size-1;y++)
   {
      for(x=y+1;x<array_size;x++)
      {
         if((array[y]==array[x])&&\
            (array[y]!=empty))
         {
            return true;
         }
      }
   }
   return false;
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


void bookcase_to_str(nodeptr bk_container,char* str)
{
   int y,x;

   str[0]='\0';

   for(y=0;y<bk_container->num_rows;y++)
   {
      for(x=0;x<bk_container->num_cols;x++)
      {

         strcat(str,num_to_letter(bk_container->bookcase[y][x]));
      }
      strcat(str,"\n");
   }


}


char* num_to_letter(colours c)
{
   switch(c)
   {
      case(empty):
      return ".";
      case(red):
      return "R";
      case(green):
      return "G";
      case(yellow):
      return "Y";
      case(blue):
      return "B";
      case(magenta):
      return "M";
      case(cyan):
      return "C";
      case(white):
      return "W";
      case(black):
      return "K";
      default:
      fprintf(stderr,"invalid colour in bookcase\n");
   	exit(EXIT_FAILURE);
   }
}


bool legal_move(int start_row,int target_row, \
            nodeptr bk_container)
{

   int book_index;
   if(bk_container==NULL)
   {
      return false;
   }
   if(start_row==target_row)
   {
      /*this is not a move +will lead to waste*/
      return false;
   }
   /*not sure when this would happen but still check*/
   if((start_row>=bk_container->num_rows)||\
      (target_row>=bk_container->num_rows)\
      ||(start_row<0)||(target_row<0))
   {
      return false;
   }
   if(len_shelf_full(target_row,bk_container)==\
      bk_container->num_cols)
   {
      return false;
   }
   /*len shelf returns size need to minus 1 to get index*/
   book_index=len_shelf_full(start_row,bk_container)-1;
   /*empty shelf*/
   if(book_index<0)
   {
      return false;
   }

   return true;
}

nodeptr make_move(int start_row,int target_row,\
         nodeptr parent_bookcase)
{
   nodeptr child;
   int start_row_pos,target_row_pos;

   if(legal_move(start_row,target_row,parent_bookcase))
   {
      start_row_pos=len_shelf_full(start_row,parent_bookcase);

      target_row_pos=len_shelf_full(target_row,parent_bookcase);

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

/*initialises a copy of parent bookcase as a child
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
      child->num_rows=parent_bookcase->num_rows;
      child->num_cols=parent_bookcase->num_cols;
      memcpy(child->bookcase,parent_bookcase->bookcase,\
               MAXSIZE*MAXSIZE*sizeof(colours));
      return child;
   }

   return NULL;
}

nodeptr create_orig_node(colours bookcase[MAXSIZE][MAXSIZE],\
                        int num_rows,int num_cols)
{
   nodeptr origin;

   origin=(nodeptr)safe_calloc(1,sizeof(node));
   origin->parent=NULL;
   origin->num_children=0;
   origin->num_rows=num_rows;
   origin->num_cols=num_cols;
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
   int y;
   if(b_arr)
   {
      /*free all the bookcases*/
      for(y=0;y<b_arr->size;y++)
      {
         free(b_arr->array[y]);
      }
      /*free the array then free the whole struct*/
      free(b_arr->array);
      free(b_arr);
   }
   return true;
}


nodeptr iterate_one_bookcase(nodeptr parent_bookcase,bookcase_arr* bk_arr)
{
   int start_row,target_row;
   nodeptr child;
   for(start_row=0;start_row<parent_bookcase->num_rows;\
      start_row++)
   {
      for(target_row=0;target_row<parent_bookcase->num_rows\
         ;target_row++)
      {
         /*legal move inside make move already checks for
            same row start->target, returns NULL if illegal
             */
         child=make_move(start_row,target_row,parent_bookcase);
         /*checks for NULLS*/
         if(child)
         {

            add_bookcase(bk_arr,child);
            /*if happy can stop creating bookcases*/
            if(is_happy(STARTROW,child))
            {
               return child;
            }
         }
      }
   }
   return NULL;
}

nodeptr go_through_bookcases(bookcase_arr* bk_arr,\
                        nodeptr orig_bookcase,int max_iters)
{
   int i,count;
   nodeptr happy;
   add_bookcase(bk_arr,orig_bookcase);
   count=0;
   for(i=0;i<bk_arr->size;i++)
   {
      if(oscilating_counter(bk_arr,i,&count))
      {
         return NULL;
      }

      if(i==max_iters)
      {
         return NULL;
      }
      if(is_happy(STARTROW,bk_arr->array[i]))
      {
         return bk_arr->array[i];
      }
      /*if an iteration of current bookcase is happy
      then skip straight to it and return */
      happy=iterate_one_bookcase(bk_arr->array[i],bk_arr);
      if(happy)
      {
         return happy;
      }
   }
   return NULL;
}


/*doesnt matter which order i do the if statements in
since they all need to be fufilled*/
bool impossible_start(nodeptr bk_container)
{
   int num_colours;
   int num_empty_spaces;
   num_colours=count_num_colours(bk_container);
   if(num_colours>bk_container->num_rows)
   {
      return true;
   }
   if(colour_impossibility(bk_container))
   {
      return true;
   }
   num_empty_spaces=count_colour(bk_container,empty);
   if(num_empty_spaces==0)
   {
      if(!is_happy(STARTROW,bk_container))
      {
         return true;
      }
   }
   return false;
}

/*checking to see if a bookcase is oscilating from two positions*/
bool oscillating(bookcase_arr* bk_arr,int pos)
{
   int i;
   char bookcase_1[MAXSTRLEN];
   char bookcase_2[MAXSTRLEN];
   char temp_case[MAXSTRLEN];
   if(bk_arr->size<=pos)
   {
      return false;
   }
   bookcase_to_str(bk_arr->array[pos],bookcase_1);
   bookcase_to_str(bk_arr->array[pos-1],bookcase_2);
   /*check last N positions - if all duplicates then
      bookcase is oscillating*/
   for(i=pos; i>=0 && i>pos-MAXOSCIL ;i--)
   {
      bookcase_to_str(bk_arr->array[i],temp_case);
      if((strcmp(temp_case,bookcase_1)!=0)&&\
         (strcmp(temp_case,bookcase_2)!=0))
      {
         return false;
      }
   }
   return true;
}

bool oscilating_counter(bookcase_arr* bk_arr,int pos,int* count)
{
   /*checking out of bounds*/
   if(bk_arr->size<=pos)
   {
      return false;
   }
   if(*count<MAXOSCIL)
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
   /*check if oscilating and return count to 0 if not*/
   else
   {
      if(oscillating(bk_arr,pos))
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

bool colour_impossibility(nodeptr bk_container)
{
   int i;
   for(i=red;i<=black;i++)
   {
      if(count_colour(bk_container,i)>bk_container->num_cols)
      {
         return true;
      }
   }
   return false;
}


int count_colour(nodeptr bk_container,colours colour)
{
   int y,x,count;
   count=0;
   for(y=0;y<bk_container->num_rows;y++)
   {
      for(x=0;x<bk_container->num_cols;x++)
      {
         if(bk_container->bookcase[y][x]==colour)
         {
            count+=1;
         }
      }

   }
   return count;
}
/*enumed the colours so can just create a bool hist
 and use the colours as indexes*/
int count_num_colours(nodeptr bk_container)
{
   int y,x,count;
   bool colour_hist[NUMCOLOURS]={false};

   for(y=0;y<bk_container->num_rows;y++)
   {
      for(x=0;x<bk_container->num_cols;x++)
      {
         if(bk_container->bookcase[y][x]!=empty)
         {
            /*minus 1 as 0 indexed*/
            colour_hist[bk_container->bookcase[y][x]-1]=true;
         }
      }
   }
   count=0;
   for(y=0;y<NUMCOLOURS;y++)
   {
      count+=(int)colour_hist[y];
   }
   return count;
}


void print_lineage(nodeptr happy_bookcase,bool verbose)
{
   int count;
   char temp_str[MAXSTRLEN];
   count=num_of_gens(happy_bookcase);
   if(count==-1)
   {
      printf("No Solution?\n");
      return;
   }
   printf("%d\n",count);
   if(verbose)
   {
      while(happy_bookcase!=NULL)
      {
         bookcase_to_str(happy_bookcase,temp_str);
         printf("%s",temp_str);
         printf("\n");

         happy_bookcase=happy_bookcase->parent;
      }
   }
}

int num_of_gens(nodeptr happy_bookcase)
{
   int count;
   count=-1;
   if(happy_bookcase)
   {
      count+=2;
      while(happy_bookcase->parent!=NULL)
      {
         count+=1;
         happy_bookcase=happy_bookcase->parent;
      }
   }
   return count;
}

colours letter_to_num(char c)
{
   switch(toupper(c))
   {
      case('.'):
      return empty;
      case('R'):
      return red;
      case('G'):
      return green;
      case('Y'):
      return yellow;
      case('B'):
      return blue;
      case('M'):
      return magenta;
      case('C'):
      return cyan;
      case('W'):
      return white;
      case('K'):
      return black;
      default:
      fprintf(stderr,"invalid colour in bookcase\n");
   	exit(EXIT_FAILURE);
   }
}

nodeptr fill_from_file(char* filename)
{
   FILE *fp;
   int x,n_rows,n_cols,curr_row;
   colours origin_bk[MAXSIZE][MAXSIZE];
   char buffer[BUFFERSIZE];
   nodeptr bk_container;

   fp= fopen(filename,"r");
   if(fp==NULL)
   {
      fprintf(stderr,"error while opening file\n");
      exit(EXIT_FAILURE);
   }
   if(!get_sizes(fp,&n_rows,&n_cols))
   {
      fprintf(stderr,"error getting number of rows and columns\n");
      exit(EXIT_FAILURE);
   }

   curr_row=0;
   while(fgets(buffer,BUFFERSIZE,fp)!=NULL)
   {
      /*need to take away the newline*/
      assert((strlen(buffer)-1)==(size_t)n_cols&& "more/less cols than expected");
      assert(curr_row<=n_rows&& "more rows than expected");
      for(x=0;x<n_cols;x++)
      {
         origin_bk[curr_row][x]=letter_to_num(buffer[x]);
      }
      curr_row+=1;
   }
   assert(curr_row==n_rows && "rows not as advertised");
   bk_container=create_orig_node(origin_bk,n_rows,n_cols);
   fclose(fp);
   return bk_container;
}


bool get_sizes(FILE *fp,int* num_rows,int* num_cols)
{
   char buffer[BUFFERSIZE];

   if(fgets(buffer,BUFFERSIZE,fp)==NULL)
   {
      fprintf(stderr,"file empty\n");
      exit(EXIT_FAILURE);
   }
   sscanf(buffer, "%d %d",num_rows,num_cols);
   /*considering a bookcase with 0 rows || 0 cols
   not to be a bookcase mostly because it isnt*/
   if(*num_rows>MAXSIZE||*num_rows<1||*num_cols>MAXSIZE||*num_cols<1)
   {
      fprintf(stderr,"invalid number of rows/cols\n");
      exit(EXIT_FAILURE);
   }
   return true;
}

bool empties_before_colour(nodeptr bk_container)
{
   int y,x;
   int first_empty_ind;
   if(bk_container)
   {
      for(y=0;y<bk_container->num_rows;y++)
      {
         first_empty_ind=len_shelf_full(y,bk_container);
         for(x=first_empty_ind;x<bk_container->num_cols;x++)
         {
            if(bk_container->bookcase[y][x]!=empty)
            {
               return true;
            }
         }
      }

   }
   return false;
}
