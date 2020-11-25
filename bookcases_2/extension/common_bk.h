#ifndef COMMON_BK_H
#define COMMON_BK_H

#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <string.h>
#include <assert.h>
#include <ctype.h>
#include <math.h>

#define NUMCOLOURS 8
#define STARTROW 0
#define MAXSIZE 9
#define MAXSTRLEN (MAXSIZE*(MAXSIZE+1)+1)
#define MAXBKS 1000000
#define BUFFERSIZE 100
#define IMPOSS 2



typedef enum colours {empty,red, green,yellow,blue,magenta,cyan,white,black} colours;
typedef enum bool {false, true} bool;

typedef struct node
{
   colours bookcase[MAXSIZE][MAXSIZE];
   struct node* parent;
   int num_children;
   int num_rows;
   int num_cols;
   /*because im sorting need to have
   way of telling if already seen the node*/
   bool seen;
   double impurity;
   struct node* next;
}node;

typedef node* nodeptr;

/*since im basically doing insertion sort
it makes sense to do linked list - no point
making a faster way of searching if inserting
is going to take ages*/
typedef struct bookcase_arr{
   int size;
   nodeptr head;
}bookcase_arr;


/*from an original bookcase create first node*/
nodeptr create_orig_node(colours bookcase[MAXSIZE][MAXSIZE],\
                        int num_rows,int num_cols);

/*create a copy of a node and increase the num
of children in parent bookcase by 1
if no parent then just returns a NULL ->not sure
when this would happen but just in case*/
nodeptr init_copy_node(nodeptr parent_bookcase);

/*how full is one bookcase shelf -
couldnt think of a better name sorry*/
int len_shelf_full(int row,const nodeptr bk_container);

/*mostly for testing what bookcases look like*/
void bookcase_to_str(const nodeptr bk_container,char* str);

/*win conditions*/
bool is_happy(int start_row,const nodeptr bk_container);

/*deals with trying to move empties
  moving to full row
  trying to move into same row
  +invalid row positions*/
bool legal_move(int start_row,int target_row, \
            const nodeptr bk_container);

void print_lineage(nodeptr happy_bookcase,bool verbose);
/*returns -1 if NULL*/
int num_of_gens(nodeptr happy_bookcase);
/*returns true if there are empty spaces before colour*/
bool empties_before_colour(const nodeptr bk_container);
/*count number of different colours*/
int count_num_colours(const nodeptr bk_container);
/*count number of particular colour book*/
int count_colour(const nodeptr bk_container,colours colour);
/*considers basic impossibilities - doesnt cover
everything */
bool impossible_start(nodeptr bk_container);

/*based on amount of colours is board impossible?*/
bool colour_impossibility(const nodeptr bk_container);
/*checks if dimension is lower/ equal to comparison*/
bool check_lower_or_equal(const int dimension,const int comparison);
bool check_equality(const size_t dimension,const size_t comparison);
/*returns void then cast as appropriate*/
void* safe_calloc(size_t nitems, size_t size);
char* num_to_letter(colours c);
colours letter_to_num(char c);
/*returns true if the are two rows with same colour*/
bool duplicate_colour_rows(const colours* array,int array_size);

/*get dimensions of bookcase*/
bool get_sizes(FILE *fp,int* num_rows,int* num_cols);


#endif
