#ifndef HASH_FUNCS_H
#define HASH_FUNCS_H
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <string.h>
#include <assert.h>
#include <ctype.h>
#include <math.h>
#include "../general.h"

/*not sure if we get penalised for plagerising ourselves
but a lot of this code for the hashmaps is from the previous project*/

/*make value void so we dont have to have a cascade
of includes that rely on one another */
typedef struct k_v_pair
{
   char* key;
   void* value;
}k_v_pair;

typedef struct assoc
{
   k_v_pair** arr;
   unsigned int capacity;
   unsigned int size;
   /*needed for double hashing*/
   int lower_prime;
}assoc;


assoc* assoc_init(void);
void assoc_insert(assoc** a, char* key, void* data);
unsigned int assoc_count(assoc* a);
void* assoc_lookup(assoc* a, char* key);
void assoc_free(assoc* a);


#endif
