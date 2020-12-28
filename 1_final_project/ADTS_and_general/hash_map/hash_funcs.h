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


/*
#define INIT_SIZE_HASH 17
#define INITLOWPRIME 13
#define SCALEFACTOR 2
#define PRIMESCALE 1.2
#define NOKEY -1
#define NOPRIME -1
#define FAILSAFE 1
#define START 2
#define SDBM_ROLL_1 6
#define SDBM_ROLL_2 16*/


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
