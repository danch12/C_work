#ifndef SPECIFIC_H
#define SPECIFIC_H
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <string.h>
#include <assert.h>
#include <ctype.h>
#include <limits.h>
#include <math.h>
#define INITSIZE 17
#define SCALEFACTOR 2
#define PRIMESCALE 1.2
#define NOPRIME -1

typedef enum bool {false, true} bool;

typedef struct k_v_pair
{
   void* key;
   void* value;
}k_v_pair;

typedef struct table table;


typedef struct assoc
{
   unsigned long bytesize;
   unsigned int capacity;
   table* table_1;
   table* table_2;
}assoc;

/*made sense to put a function pointer in each table
stops me from getting confused which func relates to
which
also makes sense to have a bytesize member and capacity
otherwise would have to pass in the assoc to a lot of
functions as well as the table
is there any point having individual fullness for each of the arrays?
seems like we resize only when there is a lot of bouncing
which could theoretically happen at almost any  fullness
*/
struct table
{
   k_v_pair** arr;
   unsigned int(*hash_func)(void*,const table*);
   unsigned long bytesize;
   unsigned int capacity;
   unsigned int size;
};

#endif
