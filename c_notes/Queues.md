## Queues

first in first out

with linked lists we will need pointers to the start and the end of the linked list

```c
#include "../General/general.h" typedef struct queue queue;
#include <stdio.h> 
#include <stdlib.h> 
#include <string.h> 
#include <assert.h>

typedef enum bool {false, true} bool;
/* Create an empty queue */
queue* queue_init(void);
/* Add element on end */
void queue_enqueue(queue* q, datatype v); /* Take element off front */
bool queue_dequeue(queue* q, datatype* d); /* Return size of queue */
int queue_size(queue* q);
/* Clears all space used */
bool queue_free(queue* q);
/* Helps with visualisation & testing */ void queue_tostring(queue* q, char* str);
```

```c
//func to add to queue/ take away things from queue
//most efficent way of doing it is to allow it to wrap around

void _inc(datatype* p)
{
  *p =(*p+1)%MAXLEN;
}
```



![Screenshot 2020-11-17 at 17.12.18](/Users/danielchow/Desktop/Screenshot 2020-11-17 at 17.12.18.png)

can only overflow in a fixed size array when there is exactly the max amount of values in there.

this is why to see if we have overflown we check to see if the queue end = queue start



## dynamic queue



```c
typedef int datatype;
#define FORMATSTR "%d"
#define ELEMSIZE 20
#define QUEUETYPE "Linked"

typedef struct element{
  datatype i;
  struct element* next;
}element;

struct queue{
  /*underlying array*/
  dataframe* front;
  dataframe* end;
  int size;
}



queue* queue_init(void)
{
  queue* q=(queue*)calloc(sizeof(queue),1);
  if(q==NULL)
  {
    warn and error
  }
  return q;
}

void queue_enqueue(queue* q, datatype d)
{
  element* f;
  if(q==NULL)
  {
    return;
  }
  /*copy data*/
  f=calloc(sizeof(element),1);
  f->i=d;
  
  if(q->front==NULL)
  {
    q->front=f;
    q->end=f;
    q->size=q->size+1;
    return;
  }
  q->end->next=f;
  q->end=f;
  q->size=q->size+1;
}

bool queue_dequeue(queue* q, datatype* d)
{
  element* f;
  if((q==NULL)||(q->front==NULL)||(q->end==NULL))
  {
    return false;
  }
  f=q->front->next;
  *d= q->front->i;
  free(q->front);
  q->front=f;
  q->size=q->size-1;
  return true;
}

bool queue_free(queue* q)
{
  if(q)
  {
    element* temp;
    element* p;
    p=q->front;
    while(p!=NULL)
    {
      temp=p->next;
      free(p);
      p=temp;
    }
     free(q);
  }
  return true;
}

void queue_tostring(queue* q, char* str) 
{
  dataframe *p;
  char tmp[ELEMSIZE];
  str[0] = ’\0’;
  if((q==NULL) || (q->front == NULL))
  {
    return; 
  }
  p = q->front; while(p)
  {
    sprintf(tmp, FORMATSTR, p->i); 
    strcat(str, tmp);
    strcat(str, "|");
    p = p->next;
  }
  str[strlen(str)-1] = ’\0’; 
}


int queue_size(queue* q) 
{
  if((q==NULL) || (q->front==NULL))
  { 
    return 0;
  }
  return q->size; 
}
```





