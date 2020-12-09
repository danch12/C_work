#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <string.h>
#include <assert.h>
#include <ctype.h>
#include <limits.h>

#define INITSIZE 17
#define INITLOWPRIME 13
#define SCALEFACTOR 2
#define PRIMESCALE 1.2
#define MAXFP 1000000
#define EPSILON 0.0000001
#define NOKEY -1
#define NOPRIME -1
typedef enum bool {false, true} bool;

typedef struct k_v_pair
{
   void* key;
   void* value;
}k_v_pair;


/*need to check bad entries = null pointers*/

typedef struct assoc
{
   k_v_pair* arr;
   int bytesize;
   unsigned int capacity;
   unsigned int size;
   /*needed for double hashing*/
   int lower_prime;
}assoc;

assoc* assoc_init(int keysize);
int _wrap_around(int max_num,int position);
int _first_hash(void* key,assoc* a);
/*http://www.cse.yorku.ca/~oz/hash.html#djb2 sdbm*/
int _first_str_hash(void* key,assoc* a);

/*going to count on only initial 16 to be a non prime arr_size
as will set anything else to a prime number*/
int _first_num_hash(void* key, assoc* a);
void* _float_convert(void* key,assoc* a);
bool _float_check(void* num,assoc* a);

int sec_hash(int f_hash,assoc* a);
void* _safe_calloc(size_t nitems, size_t size);

bool _bigger_array(assoc* a);
int _sieve_of_e_helper(int new_cap_target);
/* Free up all allocated space from 'a' */
void assoc_free(assoc* a);

void test(void);

int main(void)
{
   test();
   return 0;
}


void test(void)
{
   int i,test_hash1,test_hash2;
   void* void_ptr,*void_ptr2;
   long i_long;
   double t_double;
   float test_float;
   FILE *fp;
   int test_hashes[10000]={0};

   int histogram_hashes[15053]={0};
   char word[50];
   assoc* test_assoc;

   /*testing hashing function*/

   test_assoc=assoc_init(0);
   test_assoc->capacity=15053;

   fp = fopen("eng_370k_shuffle.txt", "rt");
   if(fp==NULL)
   {
      fprintf(stderr,"file not there?\n");
      exit(EXIT_FAILURE);
   }
   for(i=0;i<10000;i++)
   {
      if(fscanf(fp, "%s", word)!=1)
      {
         fprintf(stderr,"failed scan of word\n");
         exit(EXIT_FAILURE);
      }
      test_hashes[i]=_first_str_hash(word,test_assoc);
      histogram_hashes[_first_str_hash(word,test_assoc)]++;
   }
   for(i=0;i<10000;i++)
   {
      assert((test_hashes[i]<15053)&&(test_hashes[i]>=0));
   }
   /*testing spread of hash distribution - should use
   a chi squared test or something similar but this will do for now*/
   for(i=0;i<15053;i++)
   {
      assert(histogram_hashes[i]<15);
   }
   fclose(fp);
   assoc_free(test_assoc);

   test_assoc=assoc_init(sizeof(int));
   memset(histogram_hashes,0,sizeof(int)*15053);
   /*15053 is prime*/
   for(i=0;i<INITSIZE;i++)
   {
      test_hashes[i]=_first_num_hash(&i,test_assoc);
      histogram_hashes[_first_num_hash(&i,test_assoc)]++;
   }
   for(i=0;i<INITSIZE;i++)
   {
      assert((test_hashes[i]<INITSIZE)&&(test_hashes[i]>=0));
   }

   for(i=0;(unsigned int)i<test_assoc->capacity;i++)
   {
      assert(histogram_hashes[i]<5);
   }
   memset(histogram_hashes,0,sizeof(int)*15053);
   assoc_free(test_assoc);



   test_assoc=assoc_init(sizeof(long));
   i=0;
   for(i_long=INT_MAX;i_long<(INT_MAX+(long)INITSIZE);i_long++)
   {
      test_hashes[i]=_first_num_hash(&i_long,test_assoc);
      histogram_hashes[_first_num_hash(&i_long,test_assoc)]++;
      i++;
   }
   for(i=0;i<INITSIZE;i++)
   {
      assert((test_hashes[i]<INITSIZE)&&(test_hashes[i]>=0));

   }
   for(i=0;(unsigned int)i<test_assoc->capacity;i++)
   {
      assert(histogram_hashes[i]<5);
   }
   assoc_free(test_assoc);


   /*testing float/double conversion for hashing*/

   test_assoc=assoc_init(sizeof(double));
   t_double=100.00;
   assert(_float_check(&t_double,test_assoc)==false);
   t_double=100.1;
   assert(_float_check(&t_double,test_assoc)==true);
   assoc_free(test_assoc);


   test_assoc=assoc_init(sizeof(float));
   test_float=10.0;
   void_ptr=&test_float;
   assert(_float_check(void_ptr,test_assoc)==false);
   test_float=10.1;
   void_ptr=&test_float;
   assert(_float_check(void_ptr,test_assoc)==true);
   test_float=10;
   void_ptr=&test_float;
   assert(_float_check(void_ptr,test_assoc)==false);
   assoc_free(test_assoc);

   test_assoc=assoc_init(sizeof(float));
   test_float=10.82;
   void_ptr=&test_float;
   /*float convert pads with 0s*/
   void_ptr2=_float_convert(void_ptr,test_assoc);
   assert(*(int*)void_ptr2==10820000);
   assert(*(float*)void_ptr-test_float<EPSILON);
   free(void_ptr2);

   test_float=10.8245;
   void_ptr=&test_float;
   void_ptr2=_float_convert(void_ptr,test_assoc);
   assert(*(int*)void_ptr2==10824500);
   assert(*(float*)void_ptr-test_float<EPSILON);
   free(void_ptr2);

   /*will return nulls on ints*/
   i=10;
   void_ptr=&i;
   void_ptr2=_float_convert(void_ptr,test_assoc);
   assert(!void_ptr2);

   void_ptr2=_float_convert(NULL,test_assoc);
   assert(void_ptr2==NULL);
   assert(*(int*)void_ptr==10);
   assoc_free(test_assoc);



   test_assoc=assoc_init(sizeof(double));
   t_double=30.666;
   void_ptr=&t_double;
   void_ptr2=_float_convert(void_ptr,test_assoc);
   assert(*(long*)void_ptr2==30666000);
   free(void_ptr2);
   t_double=30.1;
   void_ptr=&t_double;
   void_ptr2=_float_convert(void_ptr,test_assoc);
   assert(*(long*)void_ptr2==30100000);
   free(void_ptr2);
   t_double=30.1;
   void_ptr=&t_double;
   void_ptr2=_float_convert(void_ptr,test_assoc);
   assert(*(long*)void_ptr2==30100000);
   free(void_ptr2);
   void_ptr2=_float_convert(NULL,test_assoc);
   assert(!void_ptr2);
   assoc_free(test_assoc);



   /*using general hash function*/

   memset(histogram_hashes,0,sizeof(int)*15053);

   test_assoc=assoc_init(0);
   test_assoc->capacity=15053;

   fp = fopen("eng_370k_shuffle.txt", "rt");
   if(fp==NULL)
   {
      fprintf(stderr,"file not there?\n");
      exit(EXIT_FAILURE);
   }
   for(i=0;i<10000;i++)
   {
      if(fscanf(fp, "%s", word)!=1)
      {
         fprintf(stderr,"failed scan of word\n");
         exit(EXIT_FAILURE);
      }
      test_hashes[i]=_first_hash(word,test_assoc);
      histogram_hashes[_first_hash(word,test_assoc)]++;
   }
   for(i=0;i<10000;i++)
   {
      assert((test_hashes[i]<15053)&&(test_hashes[i]>=0));
   }

   for(i=0;i<15053;i++)
   {
      assert(histogram_hashes[i]<15);
   }
   fclose(fp);
   assoc_free(test_assoc);



   memset(histogram_hashes,0,sizeof(int)*15053);
   test_assoc=assoc_init(sizeof(int));
   test_assoc->capacity=15053;
   for(i=0;i<10000;i++)
   {
      test_hashes[i]=_first_hash(&i,test_assoc);
      histogram_hashes[_first_hash(&i,test_assoc)]++;
   }
   for(i=0;i<10000;i++)
   {
      assert((test_hashes[i]<15053)&&(test_hashes[i]>=0));

   }
   for(i=0;(unsigned int)i<test_assoc->capacity;i++)
   {
      assert(histogram_hashes[i]<5);
   }
   memset(histogram_hashes,0,sizeof(int)*15053);
   assoc_free(test_assoc);



   test_assoc=assoc_init(sizeof(long));
   test_assoc->capacity=15053;
   i=0;
   for(i_long=INT_MAX;i_long<(INT_MAX+(long)1000);i_long++)
   {
      test_hashes[i]=_first_hash(&i_long,test_assoc);
      histogram_hashes[_first_hash(&i_long,test_assoc)]++;
      i++;
   }
   for(i=0;i<1000;i++)
   {
      assert((test_hashes[i]<15053)&&(test_hashes[i]>=0));

   }
   for(i=0;(unsigned int)i<test_assoc->capacity;i++)
   {
      assert(histogram_hashes[i]<5);
   }
   memset(histogram_hashes,0,sizeof(int)*15053);
   assoc_free(test_assoc);



   test_assoc=assoc_init(sizeof(double));
   test_assoc->capacity=15053;
   i=0;
   for(t_double=10;t_double<11;t_double+=0.001)
   {

      test_hashes[i]=_first_hash(&t_double,test_assoc);
      histogram_hashes[_first_hash(&t_double,test_assoc)]++;
      i++;
   }
   for(i=0;i<1000;i++)
   {
      assert((test_hashes[i]<15053)&&(test_hashes[i]>=0));
   }
   for(i=0;(unsigned int)i<test_assoc->capacity;i++)
   {
      assert(histogram_hashes[i]<5);
   }
   assoc_free(test_assoc);


   test_assoc=assoc_init(sizeof(double));
   t_double=30.666;
   void_ptr=&t_double;
   assert(_first_num_hash(void_ptr,test_assoc)==_first_num_hash(void_ptr,test_assoc));
   assoc_free(test_assoc);

   test_assoc=assoc_init(0);
   assert(_first_num_hash(word,test_assoc)==_first_num_hash(word,test_assoc));

   strcpy(word,"");
   assert(_first_num_hash(word,test_assoc)==_first_num_hash(word,test_assoc));
   assoc_free(test_assoc);


   test_assoc=assoc_init(sizeof(int));
   i=30;
   void_ptr=&i;
   assert(_first_num_hash(void_ptr,test_assoc)==_first_num_hash(void_ptr,test_assoc));
   assoc_free(test_assoc);

   /*second hashing func*/

   test_assoc=assoc_init(sizeof(int));
   test_assoc->capacity=15053;

   for(i=0;i<10000;i++)
   {
      test_hash1=_first_hash(&i,test_assoc);
      test_hash2=sec_hash(test_hash1,test_assoc);
      assert((test_hash2<15053)&&(test_hash2>=0));
   }
   assoc_free(test_assoc);

   test_assoc=assoc_init(sizeof(char));
   test_assoc->capacity=15053;

   for(i='a';i<'z';i++)
   {
      test_hash1=_first_hash(&i,test_assoc);
      test_hash2=sec_hash(test_hash1,test_assoc);
      assert((test_hash2<15053)&&(test_hash2>=0));
   }
   assoc_free(test_assoc);


   /*bigger primes */

   assert(_sieve_of_e_helper(1049)==1039);
   assert(_sieve_of_e_helper(2677)==2671);
   assert(_sieve_of_e_helper(0)==NOPRIME);


   test_assoc=assoc_init(sizeof(long));

   _bigger_array(test_assoc);
   assert(test_assoc->capacity==37);
   assert(test_assoc->lower_prime==INITSIZE);
   test_assoc->capacity=15053;
   _bigger_array(test_assoc);
   assert(test_assoc->capacity==36109);
   assert(test_assoc->lower_prime==15053);
   assoc_free(test_assoc);
}

/*return void then cast when appropriate*/
void* _safe_calloc(size_t nitems, size_t size)
{
   void* ptr;
   ptr = calloc(nitems, size);
   if(ptr==NULL)
   {
      fprintf(stderr,"Not enough space\n");
   	exit(EXIT_FAILURE);
   }
   return ptr;
}


assoc* assoc_init(int keysize)
{
   assoc* n_assoc;
   assert(keysize>=0);
   n_assoc=_safe_calloc(1,sizeof(assoc));
   n_assoc->capacity=INITSIZE;
   n_assoc->bytesize=keysize;
   n_assoc->arr=(k_v_pair*)_safe_calloc(INITSIZE,sizeof(k_v_pair*));
   n_assoc->size=0;
   n_assoc->lower_prime=INITLOWPRIME;
   return n_assoc;
}

/* Free up all allocated space from 'a' */
void assoc_free(assoc* a)
{
   free(a->arr);
   free(a);
}

int _wrap_around(int max_num,int position)
{
   if(position>=max_num)
   {
      return abs(max_num-position);
   }
   else if(position<0)
   {
      return max_num+position;
   }
   else
   {
      return position;
   }
}


/*void assoc_insert(assoc** a, void* key, void* data);*/
/*check size of key we are using*/


int _first_hash(void* key,assoc* a)
{
   int hash;
   void* converted;
   if(key)
   {
      if(a->bytesize==0)
      {
         hash=_first_str_hash(key,a);
      }
      else
      {
         converted=_float_convert(key,a);
         if(converted)
         {
            hash=_first_num_hash(converted,a);
            free(converted);
         }
         else
         {
            hash=_first_num_hash(key,a);
         }
      }
      return hash;
   }
   return NOKEY;

}


/*http://www.cse.yorku.ca/~oz/hash.html#djb2 sdbm*/
int _first_str_hash(void* key,assoc* a)
{
   int i;
   unsigned long hash;
   char* str;
   str= (char*) key;
   hash=0;
   i=0;
   while(str[i]!='\0')
   {
      hash+= str[i]+(hash<<6) +(hash<<16)-hash;
      i++;
   }
   return hash%a->capacity;
}

/*https://www.geeksforgeeks.org/double-hashing/ idea for hashing
need to check bytesize of the key as otherwise could potentially lose
data in longs which could be bad if all the numbers were greater than
max number ints can hold - would mean 100% collisions
*/
int _first_num_hash(void* key, assoc* a)
{
   if(a->bytesize==sizeof(long))
   {

      return (int) (*(long*) key % (long)a->capacity);

   }
   else
   {
      return *(int*)key % a->capacity;
   }
}

/*if it is a float convert to int or if double then convert it to a long
by taking out the decimal point so 10.43 becomes 1043(0000)
probably not the best way of doing it but seems like an edge case
either way
potential to cause overflow if lots of digits but on the other
hand why would you be using numbers with that many decimals
 in a dictionary anyway*/
void* _float_convert(void* key,assoc* a)
{
   char str[MAXFP];
   char new_str[MAXFP];
   char* ptr;
   void* n_key;
   int i,count;
   if(key!=NULL)
   {
      if(_float_check(key,a))
      {
         if(a->bytesize==sizeof(float))
         {
            sprintf(str,"%f",*(float*)key);
         }
         if(a->bytesize==sizeof(double))
         {
            sprintf(str,"%f",*(double*)key);
         }
         count=0;
         for(i=0;str[i]!='\0';i++)
         {
            if(isdigit(str[i]))
            {
               new_str[count]=str[i];
               count++;
            }
         }
         if(a->bytesize==sizeof(float))
         {
            n_key=_safe_calloc(1,sizeof(float));
            *(int*)n_key=(int)strtol(new_str,&ptr,10);
            return (void*)n_key;
         }
         if(a->bytesize==sizeof(double))
         {
            n_key=_safe_calloc(1,sizeof(*(long*)key));
            *(long*)n_key= strtol(new_str,&ptr,10);
            return (void*)n_key;
         }
      }

   }
   return NULL;
}

/*need to check if it is a float or not as sizeof float and
sizeof int the same same applies with doubles and longs*/
bool _float_check(void* num, assoc* a)
{
   int num_i;
   double diff_d;
   float diff_f;
   if(a->bytesize==sizeof(float))
   {
      num_i= *(float*)num;
      diff_f= *(float*)num - (float) num_i;
      if(diff_f>EPSILON)
      {
         return true;
      }
      return false;
   }
   if(a->bytesize==sizeof(double))
   {
      num_i = *(double*)num;
      diff_d= *(double*)num - (double)num_i;
      if(diff_d>EPSILON)
      {
         return true;
      }
      return false;
   }
   if(a->bytesize==sizeof(long double))
   {
      /*could just cast as double for now will not support */
      fprintf(stderr,"long doubles not supported\n");
      exit(EXIT_FAILURE);
   }
   return false;
}




/*https://www.geeksforgeeks.org/double-hashing/ idea for second
hashing function */
int sec_hash(int f_hash,assoc* a)
{
   return (a->lower_prime - (f_hash% a->lower_prime));
}


/*assigns a prime number to table size and a lower
https://en.wikipedia.org/wiki/Sieve_of_Eratosthenes*/
bool _bigger_array(assoc* a)
{
   int bigger_table;
   int new_cap_target,prime_limit;
   if(a)
   {
      bigger_table=a->capacity;
      new_cap_target=a->capacity*SCALEFACTOR;
      prime_limit=new_cap_target;
      /*keep going until find a bigger prime by increasing
      the highest number we look up to*/
      while(bigger_table<new_cap_target)
      {
         bigger_table=_sieve_of_e_helper(prime_limit);
         prime_limit=(float)prime_limit*PRIMESCALE;
      }
      a->lower_prime=a->capacity;
      a->capacity=bigger_table;
      return true;
   }
   return false;
}

int _sieve_of_e_helper(int new_cap_target)
{
   bool *bool_arr;
   int i,p;
   bool_arr= (bool *)_safe_calloc(new_cap_target,sizeof(bool));
   for (i=0 ; i<new_cap_target;i++)
   {
      bool_arr[i]=true;
   }
   /*if val is true go through and turn all of its powers false*/
   for(p=2;p*p<new_cap_target;p++)
   {
      if(bool_arr[p]==true)
      {
         for(i=p*p; i<new_cap_target;i+=p)
         {
            bool_arr[i]=false;
         }
      }
   }
   /*1 isnt a prime*/
   for(i=new_cap_target-1;i>1;i--)
   {
      if(bool_arr[i])
      {
         free(bool_arr);
         return i;
      }
   }
   free(bool_arr);
   return NOPRIME;
}




bool _insertion(assoc* a,  void* key, void* data)
{
   k_v_pair to_insert;
   int hash_1;
   int hash_2;
   if(key)
   {
      hash_1=_first_hash(key,a);
      if(a->arr[hash_1]==NULL)
      {
         to_insert=init_kv_pair(key,data);
         a->arr[hash_1]=to_insert;
         a->size++;
         return true;
      }
      else
      {
         if(_same_key(a->arr[hash_1]->key,key,a->bytesize))
         {
            a->arr[hash_1]->value=data;
            return true;
         }
         else
         {
            hash2=sec_hash(hash_1,a);
            _double_hash()
         }
      }
   }
   return false;
}

bool _double_hash()


bool _same_key(void* key1,void* key2,int bytesize)
{
   if(bytesize==0)
   {
      if(strcmp(key1,key2)==0)
      {
         return true;
      }
      return false;
   }
   else
   {
      if(memcmp(key1,key2,bytesize)==0)
      {
         return true;
      }
      return false;
   }
}


k_v_pair* init_kv_pair(void* key, void* data)
{
   k_v_pair n_kv;
   n_kv=safe_calloc(1,sizeof(k_v_pair));
   n_kv->key = key;
   n_kv->value= data;
   return n_kv;
}
