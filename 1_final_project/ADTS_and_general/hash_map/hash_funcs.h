#ifndef HASH_FUNCS_H
#define HASH_FUNCS_H

#include "../../extension.h"


#define INIT_SIZE_HASH 17
#define INITLOWPRIME 13
#define SCALEFACTOR 2
#define PRIMESCALE 1.2
#define NOKEY -1
#define NOPRIME -1
#define FAILSAFE 1
#define START 2
#define SDBM_ROLL_1 6
#define SDBM_ROLL_2 16

typedef struct k_v_pair
{
   char* key;
   func_cont* value;
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
void assoc_insert(assoc** a, char* key, func_cont* data);
unsigned int assoc_count(assoc* a);
func_cont* assoc_lookup(assoc* a, char* key);
void assoc_free(assoc* a);


#endif
