#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <time.h>
#include <string.h>
#define STRSIZE 5000


typedef enum bool {false, true} bool;

struct node{
   char c;
   struct node *left;
   struct node *right;
};

typedef struct node Node;

Node *MakeNode(char c);

void InsertRandom(Node *t, Node *n);
char *PrintTree(Node *t);
int find_depth(int deepest,Node *t);
bool identical(Node *head_1, Node *head_2);

int main(void)
{
   char c;
   char* str_1,*str_2;
   int d,d_2;
   Node *n,*n2;
   Node *head_1, *head_2;

   head_1 = MakeNode('A');
   head_2 = MakeNode('A');
   head_1->left=MakeNode('B');
   head_2->left=MakeNode('B');
   head_1->left->left=MakeNode('C');
   head_2->left->left=MakeNode('C');
   head_1->left->right=MakeNode('C');
   head_2->left->right=MakeNode('C');
   srand(time(NULL));
   /*for(c = 'B'; c < 'G'; c++)
   {
      n = MakeNode(c);
      n2 = MakeNode(c);
      InsertRandom(head_1, n);
      InsertRandom(head_2, n2);
   }*/
   str_1 = PrintTree(head_1);
   str_2 = PrintTree(head_2);
   printf("%s\n",str_1);
   printf("%s\n",str_2);
   d=find_depth(0,head_1);
   d_2=find_depth(0,head_2);
   printf("%d\n",d);
   printf("%d\n",d_2);
   if(identical(head_1,head_2))
   {
      printf("same\n");
   }
   free(str_1);
   free(str_2);
   return 0;
}


Node *MakeNode(char c)
{
   Node *n = (Node *)calloc(1, sizeof(Node));
   assert(n != NULL);
   n->c = c;
   return n;
}

void InsertRandom(Node *t, Node *n)
{
   if((rand()%2) == 0)
   { /* Left */
      if(t->left == NULL)
      {
         t->left = n;
      }
      else
      {
         InsertRandom(t->left, n); }
      }
   else
   { /* Right */
      if(t->right == NULL)
      {
         t->right = n;
      }
      else
      {
         InsertRandom(t->right, n);
      }
   }
}

char *PrintTree(Node *t)
{
   char *str;
   assert((str = calloc(STRSIZE, sizeof(char))) != NULL);
   if(t == NULL)
   {
      strcpy(str, "*");
      return str;
   }
   sprintf(str, "%c (%s) (%s)", t->c, PrintTree(t->left), PrintTree(t->right));
   return str;
}



int find_depth(int deepest,Node *t)
{
   int deeper;
   int left,right;
   if(t==NULL)
   {
      return deepest;
   }
   left=find_depth(deepest+1,t->left);
   right= find_depth(deepest+1,t->right);
   deeper= (left>right)? left:right;
   return deeper;

}



bool identical(Node *head_1, Node *head_2)
{
   bool left, right;
   if(head_1==NULL && head_2!=NULL)
   {
      return false;
   }
   if(head_1!=NULL && head_2==NULL)
   {
      return false;
   }
   if(head_1==NULL && head_2==NULL)
   {
      return true;
   }
   if(head_1->c!=head_2->c)
   {
      return false;
   }
   left=identical(head_1->left,head_2->left);
   right= identical(head_1->right,head_2->right);
   if(left&&right)
   {
      return true;
   }
   else
   {
      return false;
   }
}
