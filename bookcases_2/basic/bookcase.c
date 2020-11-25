#include "common_bk.h"

#define MAXOSCIL 50
#define INITLEN 10
#define SCALEFACTOR 2


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

/*to find impossible bookcases one of the checks will be
no (simple)oscillating bookcases - eg move to a free shelf
 then move back again *a million - to do this
 want to check if only one child and if so check parent
  bookcases to see repeated */
bool oscillating(bookcase_arr* bk_arr,int pos);
/*counts to 50 single children in a row
then sets off oscillating checker*/
bool oscilating_counter(bookcase_arr* bk_arr,int pos,int* count);

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
   if(impossible_start(origin))
   {
      printf("No Solution?\n");
      return 0;
   }
   bk_arr=init_bookcase_arr();
   solution=go_through_bookcases(bk_arr,origin,MAXBKS);

   if(argc==3)
   {
      verbose=(strcmp(argv[2],"verbose")==0);
   }
   else
   {
      verbose=false;
   }
   print_lineage(solution,verbose);
   free_bookcase_arr(bk_arr);
   return 0;
}


/*for tests common to both the main and extension parts
of assignment check out common_tests.c*/
void test(void)
{
   int i,num_rows,num_cols;
   nodeptr test_node1, test_node2,clean_test,temp;
   FILE *fp;
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

   colours test_oscillations[MAXSIZE][MAXSIZE]={{red,red,green,empty},
                                          {green,green,green,red}};

   char test_str[200];

   test_node1=create_orig_node(test_bc,7,4);

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



   fp= fopen("rrggccyy-437.bc","r");
   if(fp==NULL)
   {
      fprintf(stderr,"error while opening file in testing\n");
      exit(EXIT_FAILURE);
   }

   num_rows=num_cols=0;
   assert(get_sizes(fp,&num_rows,&num_cols));
   assert(num_rows==4);
   assert(num_cols==3);
   fclose(fp);

   /*reading in files test*/
   test_node1=fill_from_file("rrggccyy-437.bc");
   assert(test_node1->num_rows==4);
   assert(test_node1->num_cols==3);

   bookcase_to_str(test_node1,test_str);
   assert(strcmp(test_str,"RG.\nGR.\nCY.\nYC.\n")==0);
   free(test_node1);

   /*tested for wrong dimensions*/
   /*fill_from_file("eng_370k_shuffle.txt");*/

}


nodeptr make_move(int start_row,int target_row,\
         const nodeptr parent_bookcase)
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
            fprintf(stderr,"Ran out of space\n");
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

/*creates bookcases for all legal moves given initial board*/
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
            same row start-->target, returns NULL if illegal
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

/*iterates through the array of bookcases
and creates all possible children for
each board*/
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

/*putting this counter on go_through_bookcases really
bloated the function so seperated it out here */
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


/*create node from file
didnt really need check equality and check lower
but having all the fprintfs in same func was getting messy*/
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
      if((check_equality(strlen(buffer)-1,(size_t)n_cols))&&
         check_lower_or_equal(curr_row,n_rows))
      {
         for(x=0;x<n_cols;x++)
         {
            origin_bk[curr_row][x]=letter_to_num(buffer[x]);
         }
         curr_row+=1;
      }
   }
   if(check_equality((size_t)curr_row,(size_t)n_rows))
   {
      bk_container=create_orig_node(origin_bk,n_rows,n_cols);
      fclose(fp);
      return bk_container;
   }
   fclose(fp);
   return NULL;
}
