## Stacks

Push down stacks

way of storing collections of data

first in last out / last in first out

operations include push and pop

```c
#include "../General/general.h"
typedef struct stack stack;
#include <stdio.h> 
#include <stdlib.h> 
#include <assert.h> 
#include <string.h>
typedef enum bool {false, true} bool;

/* Create an empty stack */
stack* stack_init(void);

/* Add element to top */
void stack_push(stack* s, datatype i);

/* Take element from top */
datatype stack_pop(stack* s);

/* Clears all space used */ 
bool stack_free(stack* s);

/* Copy top element into d (but don’t pop it) */ bool stack_peek(stack* s, datatype* d);

/* Make a string version for testing etc. */ void stack_tostring(stack* s, char* str);
```



## ADT Stacks Realloc I

.h file will look like this

```c
typedef int datatype;
#define FORMATSTR "%d"
#define ELEMSIZE 20
#define STACKTYPE "Realloc"
#define FIXEDSIZE 16
#define SCALEFACTOR 2

typedef struct stack
{
  datatype* a;
  int size;
  int capacity;
}stack;

```

.c file will look like

```c
stack* stack_init(void)
{
  stack* s;
  s=(stack*)malloc(sizeof(stack));
  if(s==NULL)
  {
    
  }
  s->a=(datatype*)malloc(sizeof(datatype)*FIXEDSIZE);
  s->size=0;
  s->capacity=FIXEDSIZE;
  return s;
}

void stack_push(stack* s, datatype i)
{
  s->a[s->size]=i;
  s->size+=1;
  if(size>=capacity)
  {
    s->a=(datatype*)realloc(s->a,sizeof(datatype)*s->capacity*SCALEFACTOR);
    s->capacity = s->capacity*SCALEFACTOR;
    if(s->a==NULL)
    {
      error;
    }
  }
}


datatype stack_pop(stack* s)
{
  
  
  s->size-=1;
  if(s->size<0)
  {
    error 
  }
	return s->a[s->size]; 
}

bool stack_peek(stack *s,datatype *d)
{
  if((s==NULL)||s->size<=0)
  {
    /*stack empty*/
    return false;
  }
  *d=s->a[s->size-1];
  return true;
}

void stack_tostring(stack* s, char* str)
{
  int i;
  char temp[ELEMSIZE];
  str[0]='\0';
  if(s==NULL||s->size<1)
  {
    return;
  }
  for(i=s->size-1;i>=0;i--)
  {
    sprintf(temp,FORMATSTR,s->a[i]);
    strcat(str,temp);
    strcat(str,"|");
  }
  /*going to have extra bar at end
  so this removes it */
  str[strlen(str)-1]='\0';
}


bool stack_free(stack* s)
{
  if(s==NULL)
  {
    return true;
  }
  free(s->a);
  free(s);
  return true;
}

```



## Stacks linked lists

need a datastruct to store a stack and a linked list

```c
typedef struct element
{
	datatype d;
  struct element* next;
}element;


typedef struct stack
{
  element* start;
  int size;
}


stack* stack_init(void)
{
  stack* s
  stack* s=(stack*) calloc(sizeof(stack),1);
  if(s==NULL)
  {
    error
  }
  return s;
}

void stack_push(stack* s,datatype d)
{
  element* f;
  if(s)
  {
    f=calloc(sizeof(element),1);
    f->i=d;
    f->next=s->start;
    s->start=f;
    s->size+=1;
  }
}


bool stack_pop(stack* s, datatype* new_d)
{
  element temp;
  
  
  if(s&&s->start)
  {
    temp=s->start;
    s->start=s->start->next;
    *new_d=temp->d;
    free(temp);
    s->size-=1;
    return true;
  }
  return false;
}
bool stack_peek(stack* s,datatype* d)
{
  if(s==NULL|| s->start==NULL)
  {
    return false;
  }
  *d=s->start->i;
  return true;
}

void stack_tostring(stack* s,char* str)
{
  datatype *p;
  char temp[ELEMSIZE];
  str[0]='\0';
  if(s==NULL||s->size<1)
  {
    return;
  }
  p=s->start;
  while(p)
  {
    sprintf(temp,FORMATSTR,p->i);
    strcat(str,temp);
    strcat(str,"|");
    p=p->next;
  }
  str[strlen(str)-1]='\0';
}

bool stack_free(stack* s)
{
  element* prev;
  
  if(s==NULL)
  {
    return;
  }
 
  while(s->start)
  {
    p=s->start;
    s->start=s->start->next;
    free(p);
  }
  free(s);
}
```



