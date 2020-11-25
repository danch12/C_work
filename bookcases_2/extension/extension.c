#include "common_bk.h"

/*find gini coeff of one row*/
double find_gini_row(int colour_hist[NUMCOLOURS]);

/*averaged gini coefficent of whole bookcase*/
bool calculate_gini_bc(nodeptr bk_container);

bool compare_doubles_equality(double num_1,double num_2,\
                              double epsilon);

nodeptr make_move(int start_row,int target_row,\
         const nodeptr parent_bookcase);

bookcase_arr* init_bookcase_arr(void);
bool free_bookcase_arr(bookcase_arr* b_arr);
void add_bookcase(bookcase_arr* books_arr,nodeptr to_add);
/*can assume books_arr not null*/
bool insert_sorted(bookcase_arr* books_arr,nodeptr to_add);
bool free_bookcase_arr(bookcase_arr* to_free);
nodeptr go_through_bookcases(bookcase_arr* bk_arr,\
                        nodeptr orig_bookcase,int max_iters);

bool duplicate(bookcase_arr* bk_arr,nodeptr to_add);
/*count times a colour appears in a row*/
void count_colour_apps_row(const nodeptr bk_container,\
               int colour_hist[NUMCOLOURS],int row);
/*count how many different colours appear in a row*/
int count_colour_row(const nodeptr bk_container,colours colour,int row);

nodeptr iterate_one_bookcase(nodeptr parent_bookcase,bookcase_arr* bk_arr);
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

/*mostly going to test new stuff ive added-
to see the other tests check out common_tests.c*/
void test(void)
{
   int i;
   double gini_val,epsilon;
   nodeptr test_node1, test_node2;
   bookcase_arr* test_bk_arr;
   char test_string[100000];
   int colour_test[NUMCOLOURS];
   int gini_test_1[NUMCOLOURS]={19,21,40};
   int gini_test_2[NUMCOLOURS]={0,0,0};
   int gini_test_3[NUMCOLOURS]={1,0,0};
   int gini_test_4[NUMCOLOURS]={1,0,1};
   colours test_bc_simple_4[MAXSIZE][MAXSIZE]={{red,red,red,empty},
                                                {green,green,blue,empty},
                                                {red,green,empty,empty},
                                                {empty,empty,empty,empty},
                                                {blue,empty,empty,empty}};

   colours test_bc_empty[MAXSIZE][MAXSIZE]={{empty,empty,empty,empty},
                                             {empty,empty,empty,empty},
                                             {empty,empty,empty,empty},
                                             {empty,empty,empty,empty},
                                             {empty,empty,empty,empty}};

   colours test_not_imposs[MAXSIZE][MAXSIZE]={{red,red,red,empty},
                                          {green,green,green,red}};

   colours test_bc_simple_3[MAXSIZE][MAXSIZE]={{red,red,red,red},
                                                {red,red,red,red},
                                                {red,red,red,red},
                                                {red,red,red,red},
                                                {red,red,red,red}};

   colours test_oscillations[MAXSIZE][MAXSIZE]={{red,red,green,empty},
                                          {green,green,green,red}};

   colours test_happy[MAXSIZE][MAXSIZE]={{red,red,red,empty},
                                          {green,green,green,green}};

   colours test_5050[MAXSIZE][MAXSIZE]={{red,red,green,green},
                                       {red,red,green,green}};
   epsilon=0.001;

   test_node1=create_orig_node(test_bc_simple_4,5,4);
   /*only differences to the main bookcase file*/
   assert(compare_doubles_equality(test_node1->impurity,IMPOSS,epsilon));
   assert(test_node1->seen==false);
   count_colour_apps_row(test_node1,colour_test,0);
   assert(colour_test[0]==3);
   for(i=1;i<NUMCOLOURS;i++)
   {
      assert(colour_test[i]==0);
   }
   count_colour_apps_row(test_node1,colour_test,3);
   for(i=0;i<NUMCOLOURS;i++)
   {
      assert(colour_test[i]==0);
   }
   count_colour_apps_row(test_node1,colour_test,1);
   assert(colour_test[1]==2);
   assert(colour_test[3]==1);
   free(test_node1);

   gini_val=find_gini_row(gini_test_1);

   assert(compare_doubles_equality(gini_val,0.6247,epsilon));

   gini_val=find_gini_row(gini_test_2);
   assert(compare_doubles_equality(gini_val,0,epsilon));

   gini_val=find_gini_row(gini_test_3);
   assert(compare_doubles_equality(gini_val,0,epsilon));

   gini_val=find_gini_row(gini_test_4);
   assert(compare_doubles_equality(gini_val,0.5,epsilon));


   assert(calculate_gini_bc(NULL)==false);

   test_node1=create_orig_node(test_bc_empty,5,4);
   assert(calculate_gini_bc(test_node1)==true);
   assert(compare_doubles_equality(test_node1->impurity,0,epsilon));
   free(test_node1);

   test_node1=create_orig_node(test_bc_simple_3,5,4);
   assert(calculate_gini_bc(test_node1)==true);
   assert(compare_doubles_equality(test_node1->impurity,0,epsilon));
   free(test_node1);

   test_node1=create_orig_node(test_bc_simple_3,5,4);
   assert(calculate_gini_bc(test_node1)==true);
   assert(compare_doubles_equality(test_node1->impurity,0,epsilon));
   free(test_node1);

   test_node1=create_orig_node(test_not_imposs,1,4);
   assert(calculate_gini_bc(test_node1)==true);
   assert(compare_doubles_equality(test_node1->impurity,0,epsilon));
   free(test_node1);

   test_node1=create_orig_node(test_not_imposs,2,4);
   assert(calculate_gini_bc(test_node1)==true);
   assert(compare_doubles_equality(test_node1->impurity,0.1875,epsilon));
   free(test_node1);

   test_node1=create_orig_node(test_bc_simple_4,2,4);
   assert(calculate_gini_bc(test_node1)==true);
   assert(compare_doubles_equality(test_node1->impurity,0.2222,epsilon));
   free(test_node1);

   test_node1=create_orig_node(test_bc_simple_4,5,4);
   assert(calculate_gini_bc(test_node1)==true);
   assert(compare_doubles_equality(test_node1->impurity,0.18888,epsilon));
   free(test_node1);




   test_node1=create_orig_node(test_not_imposs,2,4);
   test_node2=make_move(1,0,test_node1);
   assert(compare_doubles_equality(test_node2->impurity,0,epsilon));
   free(test_node1);
   free(test_node2);


   test_node1=create_orig_node(test_bc_simple_4,3,4);
   test_node2=make_move(1,0,test_node1);
   assert(compare_doubles_equality(test_node2->impurity,0.2916,epsilon));
   free(test_node1);
   free(test_node2);
   assert(make_move(1,0,NULL)==NULL);


   /*testing bookcase ll funcs*/

   test_bk_arr= init_bookcase_arr();
   assert(test_bk_arr->size==0);

   test_node1=create_orig_node(test_not_imposs,2,4);
   calculate_gini_bc(test_node1);
   assert(insert_sorted(test_bk_arr,test_node1)==true);
   bookcase_to_str(test_bk_arr->head,test_string);
   assert(strcmp(test_string,"RRR.\nGGGR\n")==0);

   /*if i have not calculated the gini for some reason
   the node should be placed at the back*/
   test_node1=create_orig_node(test_bc_simple_4,5,4);
   insert_sorted(test_bk_arr,test_node1);
   bookcase_to_str(test_bk_arr->head->next,test_string);
   assert(strcmp(test_string,"RRR.\nGGB.\nRG..\n....\nB...\n")==0);


   test_node1=create_orig_node(test_bc_simple_4,2,4);
   assert(calculate_gini_bc(test_node1)==true);
   insert_sorted(test_bk_arr,test_node1);
   bookcase_to_str(test_bk_arr->head->next,test_string);
   assert(strcmp(test_string,"RRR.\nGGB.\n")==0);

   /*placing something with same impurity level as an
   already existing node- places before the existing node*/
   test_node1=create_orig_node(test_not_imposs,1,4);
   assert(calculate_gini_bc(test_node1)==true);
   insert_sorted(test_bk_arr,test_node1);
   bookcase_to_str(test_bk_arr->head,test_string);
   assert(strcmp(test_string,"RRR.\n")==0);

   test_node1=create_orig_node(test_5050,2,4);
   assert(calculate_gini_bc(test_node1)==true);
   assert(insert_sorted(test_bk_arr,test_node1));
   bookcase_to_str(test_bk_arr->head->next->next->next,test_string);
   assert(strcmp(test_string,"RRGG\nRRGG\n")==0);

   assert(insert_sorted(test_bk_arr,NULL)==false);

   test_node1=test_bk_arr->head;
   test_node2=test_bk_arr->head->next;
   while(test_node2!=NULL)
   {
      assert(test_node1->impurity<test_node2->impurity);
      test_node1=test_node1->next;
      test_node2=test_node2->next;
   }

   assert(free_bookcase_arr(test_bk_arr)==true);


   test_bk_arr= init_bookcase_arr();
   test_node1=create_orig_node(test_not_imposs,1,4);
   assert(calculate_gini_bc(test_node1)==true);
   add_bookcase(test_bk_arr,test_node1);
   assert(test_bk_arr->size==1);

   test_node1=create_orig_node(test_not_imposs,1,4);
   assert(calculate_gini_bc(test_node1)==true);
   add_bookcase(test_bk_arr,test_node1);
   assert(test_bk_arr->size==2);
   add_bookcase(test_bk_arr,NULL);
   assert(test_bk_arr->size==2);

   assert(free_bookcase_arr(test_bk_arr)==true);



   test_bk_arr= init_bookcase_arr();
   test_node1=create_orig_node(test_not_imposs,1,4);
   test_node2=create_orig_node(test_bc_simple_4,5,4);
   /*no duplicate if empty list*/
   assert(duplicate(test_bk_arr,test_node1)==false);

   add_bookcase(test_bk_arr,test_node1);
   assert(duplicate(test_bk_arr,test_node1)==true);

   assert(duplicate(test_bk_arr,test_node2)==false);
   free(test_node2);
   assert(free_bookcase_arr(test_bk_arr)==true);




   test_bk_arr= init_bookcase_arr();
   test_node1=create_orig_node(test_bc_simple_4,5,4);
   iterate_one_bookcase(test_node1,test_bk_arr);
   assert(test_bk_arr->size==16);
   free(test_node1);
   free_bookcase_arr(test_bk_arr);

   test_bk_arr= init_bookcase_arr();
   test_node1=create_orig_node(test_bc_simple_3,5,4);
   iterate_one_bookcase(test_node1,test_bk_arr);
   /*no legal moves*/
   assert(test_bk_arr->size==0);
   assert(test_node1->seen==true);
   free(test_node1);
   free_bookcase_arr(test_bk_arr);

   test_bk_arr= init_bookcase_arr();
   test_node1=create_orig_node(test_oscillations,2,4);
   iterate_one_bookcase(test_node1,test_bk_arr);
   /*no legal moves*/
   assert(test_bk_arr->size==1);
   assert(test_node1->seen==true);
   free(test_node1);
   free_bookcase_arr(test_bk_arr);


   test_bk_arr= init_bookcase_arr();
   test_node1=create_orig_node(test_bc_simple_4,5,4);
   test_node2=go_through_bookcases(test_bk_arr,test_node1,100);
   assert(test_node2!=NULL);
   assert(num_of_gens(test_node2)==7);
   free_bookcase_arr(test_bk_arr);

   /*dont even need to check for oscillating as duplicates
   will pick it up*/
   test_bk_arr= init_bookcase_arr();
   test_node1=create_orig_node(test_oscillations,2,4);
   test_node2=go_through_bookcases(test_bk_arr,test_node1,100);
   assert(test_node2==NULL);
   free_bookcase_arr(test_bk_arr);


   test_bk_arr= init_bookcase_arr();
   test_node1=create_orig_node(test_happy,2,4);
   test_node2=go_through_bookcases(test_bk_arr,test_node1,100);
   assert(test_node2!=NULL);
   assert(num_of_gens(test_node2)==1);
   free_bookcase_arr(test_bk_arr);
}


void count_colour_apps_row(const nodeptr bk_container,\
               int colour_hist[NUMCOLOURS],int row)
{
   int y;
   /*remove gargbage values*/
   memset(colour_hist,0,NUMCOLOURS*sizeof(int));

   for(y=red;y<=black;y++)
   {
      /*red is 0 as ignoring empties in this func*/
      colour_hist[y-1]= count_colour_row(bk_container,y,row);
   }
}



int count_colour_row(const nodeptr bk_container,colours colour,int row)
{
   int x,count;
   count=0;

   for(x=0;x<bk_container->num_cols;x++)
   {
      if(bk_container->bookcase[row][x]==colour)
      {
         count+=1;
      }
   }
   return count;
}


double find_gini_row(int colour_hist[NUMCOLOURS])
{
   double total_count,temp,gini;
   int i;
   total_count=0;
   for(i=0;i<NUMCOLOURS;i++)
   {
      total_count+=colour_hist[i];
   }
   /*if only zeros then it is pure*/
   if((int)total_count==0)
   {
      return 0;
   }
   gini=1;
   for(i=0;i<NUMCOLOURS;i++)
   {
      if(colour_hist[i]>0)
      {
         temp=colour_hist[i]/total_count;
         temp=temp*temp;
         gini-=temp;
      }
   }
   return gini;
}

/*thought about putting this func in
the orig node initialiser but useful to
have the orig node func to easily test this func*/
bool calculate_gini_bc(nodeptr bk_container)
{
   int i;
   int colour_hist[NUMCOLOURS];
   double row_gini, total_gini;
   if(bk_container)
   {
      total_gini=0;
      for(i=0;i<bk_container->num_rows;i++)
      {
         count_colour_apps_row(bk_container,colour_hist,i);
         row_gini=find_gini_row(colour_hist);
         total_gini+=row_gini;
      }
      /*if num rows was 0 would be invalid board so
      safe to divide*/
      total_gini=total_gini/bk_container->num_rows;
      bk_container->impurity=total_gini;
      return true;
   }
   return false;
}





bool compare_doubles_equality(double num_1,double num_2,\
                              double epsilon)
{
   if(fabs(num_1-num_2)>epsilon)
   {
      return false;
   }
   else
   {
      return true;
   }
}


nodeptr make_move(int start_row,int target_row,\
         const nodeptr parent_bookcase)
{
   nodeptr child;
   int start_row_pos,target_row_pos;
   if(parent_bookcase)
   {
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
         if(!calculate_gini_bc(child))
         {
            fprintf(stderr,"error in board\n");
         	exit(EXIT_FAILURE);
         }
         child->seen=false;
         return child;
      }
   }
   return NULL;

}


bookcase_arr* init_bookcase_arr(void)
{
   bookcase_arr* b_arr;
   b_arr=(bookcase_arr*)safe_calloc(1,sizeof(bookcase_arr));
   b_arr->size=0;
   b_arr->head=NULL;
   return b_arr;
}



void add_bookcase(bookcase_arr* books_arr,nodeptr to_add)
{
   /*dont run if books arr is null*/
   if(books_arr)
   {
      if(insert_sorted(books_arr,to_add))
      {
         books_arr->size+=1;
      }
   }
}

/*can assume books_arr not null as this func only
used in add_bookcase*/
bool insert_sorted(bookcase_arr* books_arr,nodeptr to_add)
{
   nodeptr current,previous;
   if(to_add)
   {
      /*dealing with the empty list*/
      if(books_arr->head==NULL)
      {
         books_arr->head=to_add;
         return true;
      }
      /*putting things at head of list*/
      if(books_arr->head->impurity >= to_add->impurity)
      {
         to_add->next=books_arr->head;
         books_arr->head=to_add;
         return true;
      }
      /*and the body- already taken care of first
      position so while loop should always run
      assigning previous twice to silence warnings*/
      current=books_arr->head;
      previous=current;
      while((current!=NULL)&&\
            (current->impurity < to_add->impurity))
      {
         previous=current;
         current=current->next;
      }
      previous->next=to_add;
      to_add->next=current;
      return true;
   }
   return false;
}


bool free_bookcase_arr(bookcase_arr* to_free)
{
   nodeptr current,temp;
   current=to_free->head;
   temp=current;
   while(current!=NULL)
   {
      temp=current;
      current=current->next;
      free(temp);
   }

   free(to_free);
   return true;
}


nodeptr iterate_one_bookcase(nodeptr parent_bookcase,bookcase_arr* bk_arr)
{
   int start_row,target_row;
   nodeptr child;
   parent_bookcase->seen=true;
   for(start_row=0;start_row<parent_bookcase->num_rows;\
      start_row++)
   {
      for(target_row=0;target_row<parent_bookcase->num_rows;\
         target_row++)
      {
         /*legal move inside make move already checks for
            same row start->target, returns NULL if illegal*/
         child=make_move(start_row,target_row,parent_bookcase);
         /*checks for NULLS*/
         if(child)
         {
            /*if not a duplicate then add to list
            good to add before checking if happy
            so easier/cleaner to free at end*/
            if(!duplicate(bk_arr,child))
            {
               add_bookcase(bk_arr,child);
               /*if happy can stop creating bookcases*/
               if(is_happy(STARTROW,child))
               {
                  return child;
               }
            }
            else
            {
               free(child);
            }
         }
      }
   }
   return NULL;
}




nodeptr go_through_bookcases(bookcase_arr* bk_arr,\
                        nodeptr orig_bookcase,int max_iters)
{
   nodeptr current,happy;
   int count;
   count=0;
   add_bookcase(bk_arr,orig_bookcase);
   current=bk_arr->head;
   while(current!=NULL)
   {
      count+=1;
      if(count>=max_iters)
      {
         return NULL;
      }
      if(current->seen==false)
      {

         if(is_happy(STARTROW,current))
         {
            return current;
         }
         happy=iterate_one_bookcase(current,bk_arr);
         if(happy)
         {
            return happy;
         }
         /*may have put some children at beginning
         so need to start again*/
         current=bk_arr->head;
      }
      current=current->next;
   }
   return NULL;
}

/*need to find way of spotting duplicates
otherwise will just fill up front of
ll with loads of duplicates and never
get anywhere*/
bool duplicate(bookcase_arr* bk_arr,nodeptr to_add)
{
   nodeptr current;
   char to_add_str[MAXSTRLEN];
   char comp_str[MAXSTRLEN];
   current=bk_arr->head;
   bookcase_to_str(to_add,to_add_str);
   while(current!=NULL)
   {
      bookcase_to_str(current,comp_str);
      if(strcmp(comp_str,to_add_str)==0)
      {
         return true;
      }
      current=current->next;
   }
   return false;
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
