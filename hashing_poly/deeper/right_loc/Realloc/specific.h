#ifndef SPECIFIC_H
#define SPECIFIC_H

#define INITSIZE 17
#define INITLOWPRIME 13
#define SCALEFACTOR 2
#define PRIMESCALE 1.2
#define NOKEY -1
#define NOPRIME -1
#define FAILSAFE 1
typedef enum bool {false, true} bool;

typedef struct k_v_pair
{
   void* key;
   void* value;
}k_v_pair;


typedef struct assoc
{
   k_v_pair** arr;
   unsigned long int bytesize;
   unsigned int capacity;
   unsigned int size;
   /*needed for double hashing*/
   int lower_prime;


}assoc;

#endif
