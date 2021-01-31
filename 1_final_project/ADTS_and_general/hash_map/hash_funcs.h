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

#define INIT_SIZE_HASH 17
#define INITLOWPRIME 13
#define SCALEFACTOR 2
#define PRIMESCALE 1.2
#define NOKEY -1
#define NOPRIME -1
#define FAILSAFE 1
#define START_P 2
#define SDBM_ROLL_1 6
#define SDBM_ROLL_2 16

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


int _wrap_around(int max_num,int position);
/*http://www.cse.yorku.ca/~oz/hash.html#djb2 sdbm*/
unsigned int _first_hash(char* key,const assoc* a);
unsigned int _sec_hash(char* key,const assoc* a);
/*the capacity/lower prime is always going to be a unsigned int
so taking its modulus will mean that all numbers land within
the unsigned int range*/
unsigned long _byte_convert(char* key);
assoc* _resize(assoc* a);
bool _insertion(assoc* a, k_v_pair* kv);
bool _insertion_helper(assoc* a,k_v_pair* kv,int insertion_point);
/*double hash does the probing to find empty spot*/
bool _double_hash(unsigned int step_size,unsigned int start_point,\
                  assoc* a, k_v_pair* kv);
assoc* _assoc_resized(int n_cap,int old_cap);

k_v_pair* _init_kv_pair(char* key, void* data);
assoc* _bigger_array(assoc* a);
bool _same_key(const char* key1,const char* key2);
int _sieve_of_e_helper(int new_cap_target);

/*pretty much the reverse of _double_hash()*/
void* _assoc_lookup_helper(int step_size,int start_point,\
                           const assoc* a,const char* key);
/*frees structs but not kv pairs*/
void _partial_free(assoc* a);

#endif
