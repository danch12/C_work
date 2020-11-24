#include "common_bk.h"


int main(void)
{

   nodeptr test_node1, test_node2;

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

   assert(strcmp(num_to_letter(empty),".")==0);
   assert(strcmp(num_to_letter(red),"R")==0);
   assert(strcmp(num_to_letter(black),"K")==0);
   assert(letter_to_num('k')==black);
   assert(letter_to_num('K')==black);
   assert(letter_to_num('.')==empty);

   test_node1=create_orig_node(test_bc,7,4);
   assert(legal_move(0,5,test_node1)==false);
   assert(legal_move(50,5,test_node1)==false);
   assert(legal_move(0,50,test_node1)==false);
   assert(legal_move(4,0,test_node1)==false);
   assert(legal_move(0,1,test_node1)==true);
   assert(legal_move(0,0,test_node1)==false);
   assert(legal_move(0,1,NULL)==false);
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

   test_node1=create_orig_node(test_bc,7,4);
   bookcase_to_str(test_node1,test_str);
   assert(strcmp(test_str,"R...\nBY..\nGGG.\nGGG.\n....\nMMMM\n....\n")==0);
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


   test_node1=create_orig_node(test_empties1,2,4);
   assert(empties_before_colour(test_node1)==true);
   free(test_node1);

   test_node1=create_orig_node(test_empties2,2,4);
   assert(empties_before_colour(test_node1)==true);
   free(test_node1);

   test_node1=create_orig_node(test_empties3,2,4);
   assert(empties_before_colour(test_node1)==true);
   free(test_node1);


   assert(duplicate_colour_rows(test_dupes,1)==false);
   assert(duplicate_colour_rows(test_dupes,4)==false);
   assert(duplicate_colour_rows(test_dupes,7)==true);
   return 0;
}
