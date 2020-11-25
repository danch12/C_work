#include "common_bk.h"

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

nodeptr init_copy_node(const nodeptr parent_bookcase)
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


/*return void then cast when appropriate*/
void* safe_calloc(size_t nitems, size_t size)
{
  void* ptr;
  ptr = calloc(nitems, size);
  if(ptr==NULL)
  {
     fprintf(stderr,"No Space available\n");
  	  exit(EXIT_FAILURE);
  }
  return ptr;
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



bool duplicate_colour_rows(const colours* array,int array_size)
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
/*get num rows and cols*/
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
   if(*num_rows>MAXSIZE||*num_rows<1||\
      *num_cols>MAXSIZE||*num_cols<1)
   {
      fprintf(stderr,"invalid number of rows/cols\n");
      exit(EXIT_FAILURE);
   }
   return true;
}


bool check_equality(const size_t dimension,const size_t comparison)
{
   if(dimension!=comparison)
   {
      fprintf(stderr,"dimension different to what is stated on top line\n");
      exit(EXIT_FAILURE);
   }
   return true;
}

bool check_lower_or_equal(const int dimension,const int comparison)
{
   if(dimension>comparison)
   {
      fprintf(stderr,"dimension greater than what is stated on top line\n");
      exit(EXIT_FAILURE);
   }
   return true;
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

/*how full is one bookcase shelf -
couldnt think of a better name sorry*/
int len_shelf_full(int row,const nodeptr bk_container)
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

/*mostly for testing what bookcases look like*/
void bookcase_to_str(const nodeptr bk_container,char* str)
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

/*win conditions
only including start_row
 so can do multiple tests on one bookcase*/
bool is_happy(int start_row,const nodeptr bk_container)
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


/*deals with trying to move empties
  moving to full row
  trying to move into same row
  +invalid row positions*/
bool legal_move(int start_row,int target_row, \
           const nodeptr bk_container)
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



void print_lineage(nodeptr happy_bookcase,bool verbose)
{
   int count;
   char temp_str[MAXSTRLEN];
   count=num_of_gens(happy_bookcase);
   if(count==-1)
   {
      /*num of gens returns -1 if NULL
      which happens when theres no solution*/
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


bool empties_before_colour(const nodeptr bk_container)
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

/*counts number of different colours*/
/*enumed the colours so can just create a bool hist
 and use the colours as indexes*/
int count_num_colours(const nodeptr bk_container)
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

/*counts number of one particular colour*/
int count_colour(const nodeptr bk_container,colours colour)
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
/*returns true if bookcase cannot be happy due to num of
specific colour books*/
bool colour_impossibility(const nodeptr bk_container)
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
