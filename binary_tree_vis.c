#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <time.h>
#include <string.h>

/*this code is a absolute mess*/
typedef struct node
{
   int val;
   struct node* left;
   struct node* right;
}node;


/*typedef char* string;*/

typedef struct board
{
   int** arr;
   int width;
   int depth;
}board;



int get_ind_child_start(char* tree,int start);
int get_ind_child_end(char* tree,int start);
int right_side_start(char* tree,int start);
int num_of_right_children(char* tree,int start,int num_children);
int num_of_left_children(char* tree,int start,int num_children);
/*start at number rather than (
counts one side of tree*/
void print_number(char* tree, int start);
void print_tree(char* tree,int start,int offset);
int longest_left_hang(char* tree,int start);
int num_children_side(char* tree, int start);
node* init_node(void);
void print_preorder(node* root);
node* tree_from_str(char* str,int index);
int longest_right_descendants(node* root);
int depth_right(node* root);
void translate(int n);
board* init_board(int width, int depth);
board* create_sized_array(node* root);
int num_digits(int num);

board* fill_arr(node* root,board* arr,int offset_depth,\
               int offset_width,int arr_depth,int arr_width);
void n2dfree(board* p, int h,int w);
board* n2drecalloc(board* p, int oh, int nh, int ow, int nw);
void on_error(const char* s);

void get_number(char* dest,char* tree, int start);
void print_as_str(board* to_print);
void test(void);


int main(void)
{
   test();
   return 0;
}


void test(void)
{
   int i,j;
   node* test_node;
   char* test_str_simple="20(*)(12(*)(13(*)(14(*)(*))))";
   /*char* test_str_full="20(10(5(*)(*))(17(*)(*)))(30(21(*)(*))(*))";
   char* test_str_simple="20(*)(12(*)(13(*)(14(*)(*))))";
   char* test_str_simple1="20(10(*)(*))(*)";
   char* right_look="20(10(5(*)(*))(17(*)(*)))(30(21(*)(*))(90(*)(*)))";
   char* right_look_2="20(10(5(*)(*))(17(*)(*)))(30(21(50(70(*)(*))(*))(*))(90(*)(40(*)(*))))";
   char* right_look_simpler=" 20(*)(30(40(50(*)(*))(*))(*))";*/
   board* test_board;
   node* test_tree;
   test_tree=init_node();
   test_tree->val=1;
   test_tree->left=init_node();
   test_tree->left->val=2;
   test_tree->right=init_node();
   test_tree->right->val=3;
   test_tree->right->right=init_node();
   test_tree->right->right->val=4;
   test_tree->left->right=init_node();
   test_tree->left->right->val=5;
   test_tree->left->right->right=init_node();
   test_tree->left->right->right->val=6;
   test_tree->left->right->right->right=init_node();
   test_tree->left->right->right->right->val=7;
   test_tree->left->right->left=init_node();
   test_tree->left->right->left->val=10;
   test_tree->left->right->right->left=init_node();
   test_tree->left->right->right->left->val=12;

/*   assert(get_ind_child_start(test_str_simple,0)==2);
   assert(get_ind_child_start(test_str_simple,3)==5);
   assert(get_ind_child_end(test_str_simple,3)==4);
   assert(get_ind_child_end(test_str_simple1,3)==11);*/

   /*assert(num_children_side(test_str_simple,0)==1);
   assert(num_children_side(test_str_simple1,0)==3);
   assert(num_children_side(test_str_full,0)==7);*/
   /*assert(right_side_start(test_str_simple1,0)==12);
   assert(right_side_start(test_str_simple1,3)==8);
   assert(right_side_start(right_look,0)==25);*/
   /*void num_of_right_children(char* tree,int start,int num_children)*/
/*   assert(num_of_right_children(right_look,0,0)==2);
   assert(num_of_right_children(right_look,28,0)==1);
   assert(num_of_right_children(test_str_full,0,0)==1);
   assert(num_of_right_children(test_str_simple1,0,0)==0);

   assert(num_of_left_children(test_str_full,0,0)==2);
   assert(num_of_left_children(test_str_full,15,0)==0);


   assert(longest_left_hang(right_look_simpler,0)==2);

   assert(longest_left_hang(right_look_2,0)==3);
   assert(longest_left_hang(test_str_simple1,0)==1);
   printf("%d\n",longest_left_hang(right_look_2,0));
   assert(longest_left_hang(right_look_simpler,0)==3);*/
   test_node=tree_from_str(test_str_simple,0);
   /*print_preorder(test_tree);*/
   test_node=tree_from_str(test_str_simple,0);
   /*printf("%d\n",longest_right_descendants(test_tree));
   test_board=init_board(5,5);
   for(i=0;i<5;i++)
   {
      for(j=0;j<5;j++)
      {
         printf(test_board[i][j]);
      }
   }*/
   test_board=create_sized_array(test_tree);
   /*string** fill_arr(node* root,string** arr,int offset_depth,\
                  int offset_width,int arr_depth,int arr_width)*/
   test_board=fill_arr(test_tree,test_board,0,0,test_board->depth,test_board->width);
   for(i=0;i<test_board->depth;i++)
   {
      for(j=0;j<test_board->width;j++)
      {
         printf("%d",test_board->arr[i][j]);
      }
      printf("\n");
   }
   print_as_str(test_board);


   printf("%d\n",num_digits(156));
   printf("%d\n",num_digits(0));
   printf("%d\n",depth_right(test_tree->left));
}



/*start at number*/
int get_ind_child_start(char* tree,int start)
{
   int i;

   for(i=start;(tree[i]!='(') &&tree[i]!='\0';i++)
   {

   }
   return i;
}

/*from parent node get right side of tree (*/
int right_side_start(char* tree,int start)
{
   int left_ind_start,left_ind_end;
   int right;
   left_ind_start=get_ind_child_start(tree,start);

   left_ind_end=get_ind_child_end(tree,left_ind_start+1);
   right=get_ind_child_start(tree,left_ind_end);

   return right;

}


node* init_node(void)
{
   node* n_node;
   n_node=calloc(1,sizeof(node));
   if(n_node==NULL)
   {
      fprintf(stderr,"No space\n");
   	exit(EXIT_FAILURE);
   }
   return n_node;
}
/* 20(*)(30(40(50(*)(*))(*))  (*))*/
/*this is wrong*/
node* tree_from_str(char* str,int index)
{
   node* current_node;
   char num_str[10];
   int num,right_side,left_side;
   if(str[index]=='*'||str[index+1]=='*')
   {
      return NULL;
   }
   else
   {
      current_node=init_node();
      get_number(num_str,str,index);
      sscanf(num_str,"%d\n",&num);

      current_node->val=num;
      right_side=right_side_start(str,index);
      current_node->right=tree_from_str(str,right_side);

      left_side=get_ind_child_start(str,index);
      current_node->left=tree_from_str(str,left_side+1);
      return current_node;
   }
}


void print_preorder(node* root)
{
   if(root!=NULL)
   {
      printf("%d\n",root->val);
      print_preorder(root->left);
      print_preorder(root->right);
   }
}

/*int depth_right(node* root)
{
   int count;
   count=0;
   while(root->right)
   {
      count=count+1;
      root=root->right;
   }
   return count;
}*/

int depth_left(node* root)
{
   int count;
   count=0;
   while(root->left)
   {
      count=count+1;
      root=root->left;
   }
   return count;
}



int depth_right(node* root)
{
   int count;
   count=num_digits(root->val);
   while(root->right)
   {
      count=count+1;
      count=count+5;
      root=root->right;
   }
   return count;
}

int longest_right_descendants(node* root)
{
   int count;
   int overall_longest;
   count=0;
   if(root)
   {
      count=depth_right(root);
      if(root->left)
      {
         overall_longest=longest_right_descendants(root->left);
         if(overall_longest>count)
         {
            return overall_longest;
         }
      }
   }

   return count;
}






int longest_left_descendants(node* root)
{
   int count;
   int overall_longest;
   count=0;

   count=depth_left(root);
   if(root->right)
   {
      overall_longest=longest_left_descendants(root->right);
      if(overall_longest>count)
      {
         return overall_longest;
      }
   }
   return count;
}

/*probably easiest to create 2d array and fill it - dont have to worry about
doing it one line at a time then*/

board* init_board(int width, int depth)
{
   int** board_ptr;
   board* n_board;
   int i;
   int j;
   n_board=calloc(1,sizeof(board));
   n_board->width=width;
   n_board->depth=depth;
   board_ptr=(int**)calloc(depth,sizeof(int*));
   for(i=0;i<depth;i++)
   {
      board_ptr[i]=(int*)calloc(width,sizeof(int));
   }
   for(i=0;i<depth;i++)
   {
      for(j=0;j<width;j++)
      {

         board_ptr[i][j]=-1;

      }
   }
   n_board->arr=board_ptr;
   return n_board;
}

board* n2drecalloc(board* p, int oh, int nh, int ow, int nw)
{

   board* n;
   int j,i;
   n = init_board(nw, nh);

   for(i=0;i<oh;i++)
   {
      for(j=0;j<ow;j++)
      {
         n->arr[i][j]=p->arr[i][j];

      }
   }
   n2dfree(p, oh,ow);
   return n;

}

int num_digits(int num)
{
   int count;
   if(num==0)
   {
      return 1;
   }
   count=0;
   while(num>0)
   {
      count++;
      num=num/10;
   }
   return count;
}




board* create_sized_array(node* root)
{
   board* arr;
   int init_width,init_height;
   init_width=longest_right_descendants(root);
   init_height= longest_left_descendants(root);
   arr=init_board(init_width*2,init_height*2);
   return arr;
}



board* fill_arr(node* root,board* arr,int offset_depth,\
               int offset_width,int arr_depth,int arr_width)
{
   /*char temp_str[10];*/
   node* nodeptr;
   int i;
   int right_descend;
   /*sprintf(temp_str,"%d",root->val);*/

   if(offset_width>=arr_width)
   {
      arr=n2drecalloc(arr,arr_depth,arr_depth,arr_width,offset_width+2);
   }
   if(offset_depth+1>=arr_depth)
   {
      arr=n2drecalloc(arr,arr_depth,offset_depth+2,arr_width,arr_width);
   }

   arr->arr[offset_depth][offset_width]=root->val;
   /*need to make enough space for the right children of left node*/
   right_descend=longest_right_descendants(root->left);
   if(root->right)
   {
      if(((right_descend)+2)>=arr_width)
      {
         arr=n2drecalloc(arr,arr_depth,arr_depth,arr_width,(right_descend+2));
      }
      printf("right descend %d\n",right_descend);
      for(i=offset_width+1;i<=offset_width+(right_descend+1);i++)
      {

         arr->arr[offset_depth][i]=-2;
      }
      arr=fill_arr(root->right,arr,offset_depth,i,arr->depth,arr->width);
   }
   if(root->left)
   {

      arr->arr[offset_depth+1][offset_width]=-3;
      arr=fill_arr(root->left,arr,offset_depth+2,offset_width,arr->depth,arr->width);

   }
   return arr;
}


void print_as_str(board* to_print)
{
   int y,x;
   for(y=0;y<to_print->depth;y++)
   {
      for(x=0;x<to_print->width;x++)
      {
         translate(to_print->arr[y][x]);
      }
      printf("\n");
   }
}

void translate(int n)
{
   int i;
   switch(n)
   {
      case -1:
      printf(" ");
      break;
      case -2:
      printf("-");
      break;
      case -3:
      printf("|");
      break;
      default:
      for(i=0;i<5-num_digits(n);i++)
      {
         printf("0");
      }
      printf("%d",n);
   }
}


void n2dfree(board* p, int h,int w)
{
   int i,j;
   for(i=0; i<h; i++){
      free(p->arr[i]);
   }
   free(p->arr);
   free(p);
}


void on_error(const char* s)
{
   fprintf(stderr, "%s\n", s);
   exit(EXIT_FAILURE);
}


void get_number(char* dest,char* tree, int start)
{
   int i,j;
   i=start;
   j=0;
   while(tree[i]!='(')
   {
      dest[j]=tree[i];
      i++;
      j++;
   }
}

/*start ind after the (*/
int get_ind_child_end(char* tree,int start)
{
   int i;
   int left_brackets,right_brackets;
   left_brackets=0;
   right_brackets=0;
   i=start;

   for(; tree[i]!='\0' ; i++)
   {

      if(tree[i]=='(')
      {

         left_brackets++;
         printf("left brackets=%d\n",left_brackets);
      }
      if(tree[i]==')')
      {

         right_brackets++;
         printf("right brackets=%d\n",right_brackets);
      }

      if(left_brackets==right_brackets)
      {
         return i;
      }
   }
   return -1;
}

/*start at number rather than (
counts one side of tree*/
int num_children_side(char* tree, int start)
{
   int start_ind;
   int end_ind;
   int children;
   int i;
   children=0;
   start_ind=get_ind_child_start(tree,start);
   end_ind=get_ind_child_end(tree,start_ind+1);
   for(i=start_ind;i<=end_ind;i++)
   {
      if(tree[i]=='(')
      {
         children++;
      }
   }
   return children;
}




int num_of_right_children(char* tree,int start,int num_children)
{

   int start_right;
   start_right=right_side_start(tree,start);
   if(tree[start_right+1]!='*'&&tree[start_right+1]!='\0')
   {
      return num_of_right_children(tree,start_right+1,num_children+1);
   }
   return num_children;
}

/*"20(10(*)(*))(*)";
20(10(5(*)(*)) (17(10(12(13((*)(*)))(*)(*))(*))))(30(21(*)(*))(*))*/

int num_of_left_children(char* tree,int start,int num_children)
{

   int start_left;
   start_left=get_ind_child_start(tree,start);

   if(tree[start_left+1]!='*'&&tree[start_left+1]!='\0')
   {
      return num_of_left_children(tree,start_left+1,num_children+1);
   }
   return num_children;
}


int longest_left_hang(char* tree,int start)
{
   int overall_longest,right_side_hangers;
   int start_ind_left,right_start;


   start_ind_left=get_ind_child_start(tree,start);
   overall_longest=num_of_left_children(tree,start_ind_left,0);

   right_start=right_side_start(tree,start);

   if(tree[right_start+1]=='*')
   {
      return overall_longest;
   }
   right_side_hangers=longest_left_hang(tree,right_start+1);
   if(right_side_hangers>overall_longest)
   {
      overall_longest=right_side_hangers;
   }
   return overall_longest;

}


/*find longest left in top level
then go one level down and see if longest left there*/
