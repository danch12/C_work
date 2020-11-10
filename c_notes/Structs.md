## Structs

you can fill in a struct using {} like in an array which is easier.



also when including other c files need to mention when compiling - although dont have to do this when you include a header file i dont think as you put the # include in the c file.

###  Via Fixed Array

```c

typedef int datatype;
#define COLLTYPE "Fixed"
#define FIXEDSIZE 5000

typedef struct coll 
{
   /* Underlying array */
   datatype a[FIXEDSIZE];
   int size;
} coll;



coll* coll_init(void)
{
  coll *c;
  *c = (coll*)malloc(sizeof(coll));
  if(*c==NULL)
  {
    fprintf(stderr,"no space\n");
    exit(EXIT_FAILURE);
  }
  c->size=0;
  return c;
}

int coll_size(coll *c)
{
  if(c==NULL)
  {
    return 0;
  }
  return c->size;
}

bool coll_isin(coll* c, datatype d)
{
  int i;
  for(i=0;i<coll_size(c);i++)
  {
    if(c->a[i]==d)
    {
      return true;
    }
  }
  return false;
}

void coll_add(coll* c, datatype d)
{
 /*skip if null*/
  if(c)
  {
    c->a[c->size]=d;
    c->size=c->size+1;
    if(c->size>=FIXEDSIZE)
    {
       fprintf(stderr,"collection overflow\n");
    	 exit(EXIT_FAILURE);
    }
  }
}

bool coll_remove(coll *c, datatype d)
{
  int i,j;
  for(i=0;i<coll_size(c);i++)
  {
    if(c->a[i]==d)
    {
      /*shuffle down*/
      for(j=i;j<coll_size(c);j++)
      {
        c->a[j]=c->a[j+1];
      }
      c->size=c->size-1;
      return true;
    }
  }
}

bool coll_free(coll *c)
{
  free(c);
  return true;
}
```



### dynamic array ADT with realloc

after doing calloc/malloc if you want to add more space or whatever you can use realloc to add more space. Works really similarly to malloc



```c
typedef int datatype;

#define COLLTYPE "Realloc"

#define FIXEDSIZE 16
#define SCALEFACTOR 2

typedef struct coll 
{
  /*underlying array*/
  datatype* a;
  int size;
  int capacity;
}

coll * coll_init(void)
{
  coll* c =(coll*) calloc(sizeof(coll),1);
  if(c==NULL)
  {
     fprintf(stderr,"no space\n");
   	 exit(EXIT_FAILURE);
  }
  c->a= (datatype*) 	calloc(sizeof(coll),FIXEDSIZE);
  if(c->a==NULL)
  {
     fprintf(stderr,"no space\n");
   	 exit(EXIT_FAILURE);
  }
  c->size=0;
  c->capacity= FIXEDSIZE;
  return c;
}

void coll_add(coll* c, datatype d)
{
  if(c)
  {
    c->a[c->size]=d;
    c->size=c->size+1;
    if(c->size==c->capacity)
    {
      c->a=(datatype*) realloc(c->a,sizeof(datatype)*c->capactiy*SCALEFACTOR);
      if(c->a==NULL)
      {
        fprintf(stderr,"no space\n");
   	 		exit(EXIT_FAILURE);
      }
      c->capacity=c->capacity*SCALEFACTOR;
    }
  }
}

bool coll_free(coll* c)
{
  if(c==NULL)
  {
    return true;
  }
  free(c->a);
  free(c);
  return true;
}

```

everything else is the same to above

realloc is really expensive time wise so may favour using linked lists

## Collection ADT via Linked List

```c
typedef int datatype;
typedef struct dataframe
{
  datatype i;
  struct dataframe* next;
}dataframe;

struct coll
{
  /*underlying array*/
  dataframe* start;
  int size;
}

coll* coll_init(void)
{
  coll *c =(coll*) calloc(sizeof(coll),1);
  return c;
}

int coll_size(coll* c)
{
  if(c==NULL)
  {
    return 0;
  }
  return c->size;
}

bool coll_isin(coll*c,datatype d)
{
  dataframe *f;
  if(c==NULL||c->start==NULL)
  {
    return false;
  }
  f=c->start;
  do
  {
    if(f->i==d)
    {
      return true;
    }
    f=f->next;
  }while(f!=NULL);
  return false;
}

void coll_add(coll* c , datatype d)
{
  dataframe* f;
  if(c)
  {
    f=calloc(sizeof(dataframe),1);
    f->i=d;
    f->next=c->start;
    c->start= f;
   	c->size=c->size+1;
  }
}
```



