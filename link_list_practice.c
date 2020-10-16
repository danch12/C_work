#include <stdio.h>
#include<stdlib.h>
#include <string.h>

typedef struct {
  char name[10];
  int age;
  int weight;
} DATA;

typedef struct list{
  DATA info;
  struct list *next;
} element;



typedef element *Link;

void print_l(Link head);
Link create_l(DATA arr[],int position,int size);
int count_above(Link head, int weight_lim);

int compare_age(DATA a, DATA b);
int compare_weight(DATA a, DATA b);
Link merge_sorted_lists(int (*compare)(DATA a, DATA b), Link a, Link b);
void split_in_middle(Link head, Link* front, Link* back);
void MergeSort(Link *head,int (*compare)(DATA a, DATA b));
Link copy_list(Link h);
void concat_lists(Link h1, Link h2);
Link copy_cat(Link h1, Link h2);
void insert_head(Link* h,DATA d);

int main(void)
{
  DATA test_arr[4],a,b,c,d;
  DATA test_t[3],x,y,z;
  DATA f;

  strcpy(a.name,"Dan");
  a.age =201;
  a.weight = 30;

  strcpy(b.name,"doon");
  b.age =20;
  b.weight = 1;


  strcpy(c.name,"caon");
  c.age =19;
  c.weight = 50;


  strcpy(d.name,"msn");
  d.age =400;
  d.weight = 300;

  test_arr[0]=a;
  test_arr[1]=b;
  test_arr[2]=c;
  test_arr[3]=d;


  strcpy(x.name,"doon");
  x.age =25;
  x.weight = 1;


  strcpy(y.name,"caon");
  y.age =215;
  y.weight = 50;


  strcpy(z.name,"msn");
  z.age =300;
  z.weight = 300;

  test_t[0]=x;
  test_t[1]=y;
  test_t[2]=z;

   strcpy(f.name,"head");
   f.age = 50;
   f.weight = 60;



  Link test_link=create_l(test_arr,0, 4);
  Link test_link2=create_l(test_t,0,3);
  Link copy_test = copy_list(test_link);
  print_l(test_link);

  printf("copy of test_link-\n");
  print_l(copy_test);
  //Link merged = merge_sorted_lists(test_link,test_link2);
  printf("merging the lists\n");
  MergeSort(&test_link,compare_age);
  print_l(test_link);
  printf("fatties =%d\n",count_above(test_link, 30));

  concat_lists(test_link,test_link2);
  print_l(test_link);

  printf("adding head-\n");
  insert_head(&test_link2, f);
  print_l(test_link2);

  return 0;
}

Link create_l(DATA arr[],int position, int size)
{
  Link head;
  if (position==size)
  {
    //base case
    return NULL;
  }
  else

  head = malloc(sizeof(element));
  head->info = arr[position];
  head->next= create_l(arr,position+1,size);

  return head;
}

void print_l(Link head)
{
  while(head!=NULL)
  {
    printf("%s\n",head->info.name);
    printf("%d\n", head->info.age);
    printf("%d\n",head->info.weight);
    head=head->next;
  }
}

int count_above(Link head, int weight_lim)
{
  int count=0;
  while (head!=NULL)
  {
    if(head->info.weight>weight_lim)
    {
      count+=1;
    }
    head=head->next;
  }
  return count;
}

int compare_age(DATA a, DATA b)
{
  return (a.age<=b.age) ? 1:0;
}

int compare_weight(DATA a, DATA b)
{
  return (a.weight<=a.weight) ? 1:0;
}

Link merge_sorted_lists(int (*compare)(DATA a, DATA b), Link a, Link b)
{
  if(a==NULL)
  {
    return b;
  }
  else if(b==NULL)
  {
    return a;
  }

  Link merge_head = NULL;
  if (compare(a->info,b->info))
  {
    merge_head = a;
    a=a->next;
  }
  else
  {
    merge_head = b;
    b= b->next;
  }

  Link merge_tail = merge_head;

  while (a!=NULL && b!=NULL)
  {
    Link temp = NULL;
    if (compare(a->info,b->info))
    {
      temp= a;
      a=a->next;
    }
    else
    {
      temp=b;
      b=b->next;
    }
    merge_tail->next = temp;
    merge_tail = merge_tail->next;
  }

  //connecting rest
  if(a!=NULL)
  {
    merge_tail->next= a;
  }
  else if(b!=NULL)
  {
    merge_tail->next=b;
  }
  return merge_head;

}

void split_in_middle(Link head, Link* front, Link* back)
{
  Link fast;
  Link slow;
  slow = head;
  fast = head->next;

  while(fast!=NULL)
  {
    fast =fast->next;
    if(fast!=NULL)
    {
      slow=slow->next;
      fast=fast->next;
    }
  }

  *front = head;
  *back = slow->next;
  slow->next=NULL;

}

void MergeSort(Link *head,int (*compare)(DATA a, DATA b))
{
  Link h = *head;
  Link a;
  Link b;

  //base case -- length 0 or 1
  if ((h==NULL)||(h->next==NULL))
  {
    return;
  }
  split_in_middle(h, &a,&b);
  MergeSort(&a,compare);
  MergeSort(&b,compare);

  *head = merge_sorted_lists(compare,a,b);

}

Link copy_list(Link h)
{
   Link temp;
   if(h==NULL)
   {
      return NULL;
   }
   else
   {
      temp=malloc(sizeof(element));
      temp->info = h->info;
      temp->next= copy_list(h->next);
      return temp;
   }
}

void concat_lists(Link h1, Link h2)
{
   while(h1->next!=NULL)
   {
      h1=h1->next;
   }
   h1->next = h2;
}

Link copy_cat(Link h1, Link h2)
{
   Link c_h1= copy_list(h1);
   Link c_h2 = copy_list(h2);
   concat_lists(c_h1,c_h2);
   return c_h1;
}

void insert_head(Link* h,DATA d)
{
   Link temp;
   temp = malloc(sizeof(element));
   temp->info = d;
   temp->next= (*h);
   *h=temp;
}
