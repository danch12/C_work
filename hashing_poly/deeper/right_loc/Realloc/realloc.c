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
   k_v_pair** arr;
   int bytesize;
   unsigned int capacity;
   unsigned int size;
   /*needed for double hashing*/
   int lower_prime;
}assoc;

/*not going to accept structs as keys but can be values
*/
typedef struct test_struct
{
   int test;
   int test_2;
}test_struct;

assoc* assoc_init(int keysize);
void assoc_insert(assoc** a, void* key, void* data);
void assoc_free(assoc* a);
unsigned int assoc_count(assoc* a);
void* assoc_lookup(assoc* a, void* key);
/*seemed stupid to use init then recalloc*/
assoc* assoc_resized(int keysize,int n_cap,int old_cap);
int _wrap_around(int max_num,int position);
int _first_hash(void* key,assoc* a);
/*http://www.cse.yorku.ca/~oz/hash.html#djb2 sdbm*/
int _first_str_hash(void* key,assoc* a);



assoc* _resize(assoc* a);
bool _insertion(assoc* a,k_v_pair* kv);
bool _insertion_helper(assoc* a,k_v_pair* kv,int insertion_point);
bool _double_hash(int step_size,int start_point, assoc* a,k_v_pair* kv);
/*going to count on only initial 16 to be a non prime arr_size
as will set anything else to a prime number*/
int _first_num_hash(void* key, assoc* a);
void* _float_convert(void* key,assoc* a);
bool _float_check(void* num,assoc* a);

int sec_hash(int f_hash,assoc* a);
void* _safe_calloc(size_t nitems, size_t size);
k_v_pair* init_kv_pair(void* key, void* data);
assoc* _bigger_array(assoc* a);
bool _same_key(void* key1,void* key2,int bytesize);
int _sieve_of_e_helper(int new_cap_target);
/* Free up all allocated space from 'a' */

void _partial_free(assoc* a);
void test(void);

int main(void)
{
   test();
   return 0;
}


void test(void)
{
   int i,test_hash1,test_hash2,test_int;
   void* void_ptr,*void_ptr2;
   long i_long,i_long2;
   double t_double;
   float test_float;
   FILE *fp;
   test_struct test_s;
   k_v_pair* test_kv;
   int test_hashes[10000]={0};

   int histogram_hashes[15053]={0};
   char word[50];
   char word2[50];
   assoc* test_assoc,*test_assoc2;

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
   test_assoc->capacity=INITSIZE;
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
   test_assoc->capacity=INITSIZE;
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
   test_assoc->capacity=INITSIZE;
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
   test_assoc->capacity=INITSIZE;
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
   test_assoc->capacity=INITSIZE;
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
   test_assoc->capacity=INITSIZE;
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
   test_assoc->capacity=INITSIZE;
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
   test_assoc->capacity=INITSIZE;
   assoc_free(test_assoc);

   test_assoc=assoc_init(sizeof(char));
   test_assoc->capacity=15053;

   for(i='a';i<'z';i++)
   {
      test_hash1=_first_hash(&i,test_assoc);
      test_hash2=sec_hash(test_hash1,test_assoc);
      assert((test_hash2<15053)&&(test_hash2>=0));
   }
   test_assoc->capacity=INITSIZE;
   assoc_free(test_assoc);



   assert(_wrap_around(10,11)==1);
   assert(_wrap_around(10,10)==0);

   /*should just cycle from 0 to 9 over and over*/
   test_int=0;
   for(i=20;i>0;i--)
   {
      assert(_wrap_around(10,i)==test_int);
      test_int--;
      if(test_int<0)
      {
         test_int=9;
      }
   }
   test_int=0;
   for(i=0;i<-20;i++)
   {
      assert(_wrap_around(10,i)==test_int);
      test_int++;
      if(test_int>9)
      {
         test_int=0;
      }
   }



   /*bigger primes */

   /*gets closest lower prime */
   assert(_sieve_of_e_helper(1049)==1039);
   assert(_sieve_of_e_helper(2677)==2671);
   assert(_sieve_of_e_helper(0)==NOPRIME);


   test_assoc=assoc_init(sizeof(long));

   test_assoc2=_bigger_array(test_assoc);
   assert(test_assoc2->capacity==37);
   assert(test_assoc2->lower_prime==INITSIZE);
   assoc_free(test_assoc2);

   test_assoc->capacity=15053;
   test_assoc2=_bigger_array(test_assoc);
   assert(test_assoc2->capacity==36109);
   assert(test_assoc2->lower_prime==15053);
   test_assoc->capacity=INITSIZE;
   assoc_free(test_assoc);
   assoc_free(test_assoc2);
   assert(!_bigger_array(NULL));


   /*inserting a kv_pair*/
   test_assoc=assoc_init(sizeof(long));
   for(i=0;i<(int)test_assoc->capacity;i++)
   {
      assert(test_assoc->arr[i]==NULL);
   }
   assert(test_assoc->size==0);
   i_long=10;
   i_long2=12;

   test_kv=init_kv_pair(&i_long,&i_long2);
   /*adding to empty space*/
   assert(_insertion_helper(test_assoc,test_kv,0));
   assert(*(long*)test_assoc->arr[0]->key==10);
   assert(*(long*)test_assoc->arr[0]->value==12);


   test_kv=init_kv_pair(&i_long,&i_long);
   /*updating value with same key*/
   assert(_insertion_helper(test_assoc,test_kv,0));
   assert(*(long*)test_assoc->arr[0]->value==10);

   test_kv=init_kv_pair(&i_long2,&i_long);
   /*trying to insert with different key*/
   assert(!_insertion_helper(test_assoc,test_kv,0));
   free(test_kv);
   /*size should only increase when adding new kv pairs
   not when updating existing ones*/
   assert(test_assoc->size==1);
   assoc_free(test_assoc);

   /*testing words and mixed types*/
   test_assoc=assoc_init(0);
   strcpy(word,"938Neill\n");
   i=1;
   test_kv=init_kv_pair(&word,&i);
   assert(_insertion_helper(test_assoc,test_kv,0));
   assert(strcmp(test_assoc->arr[0]->key,word)==0);
   assert(*(int*)test_assoc->arr[0]->value==1);

   test_s.test_2 =10;
   test_kv=init_kv_pair(&word,&test_s);
   assert(_insertion_helper(test_assoc,test_kv,0));
   /*we can also put structs as values but not as keys */
   assert((*(test_struct*)test_assoc->arr[0]->value).test_2==10);

   strcpy(word2,"938Neilliscool\n");
   test_kv=init_kv_pair(&word2,&test_s);
   assert(!_insertion_helper(test_assoc,test_kv,0));
   free(test_kv);
   assert(!_insertion_helper(test_assoc,NULL,0));
   assert(!_insertion_helper(test_assoc,NULL,0));
   assoc_free(test_assoc);


   test_assoc=assoc_init(sizeof(int));

   i=50;
   test_kv=init_kv_pair(&i,&word2);
   assert(_insertion_helper(test_assoc,test_kv,0));
   assert(strcmp(word2,(char*)test_assoc->arr[0]->value)==0);
   test_int =49;

   test_kv=init_kv_pair(&test_int,&i);
   assert(_double_hash(7,0,test_assoc,test_kv));
   assert(*(int*)test_assoc->arr[7]->value==50);

   test_float=50.1;
   test_kv=init_kv_pair(&test_float,&test_int);
   assert(_double_hash(7,0,test_assoc,test_kv));
   assert(*(int*)test_assoc->arr[14]->value==49);
   assoc_free(test_assoc);



   test_assoc=assoc_init(sizeof(int));
   for(i=0;i<15;i++)
   {
      test_kv=init_kv_pair(&test_hashes[i],&i);
      assert(_insertion(test_assoc,test_kv));
   }
   assert(test_assoc->size==15);
   assoc_free(test_assoc);


   test_assoc=assoc_init(sizeof(int));
   for(i=0;i<15;i++)
   {
      test_kv=init_kv_pair(&test_hashes[i],&i);
      assert(_insertion(test_assoc,test_kv));
   }

   test_assoc=_resize(test_assoc);
   assert(test_assoc->size==15);
   assert(test_assoc->capacity==37);
   for(i=0;i<15;i++)
   {
      test_hash1=_first_num_hash(&test_hashes[i],test_assoc);
      assert(*(int*)test_assoc->arr[test_hash1]->value==i);
   }

   assoc_free(test_assoc);

   srand(time(NULL));
   test_assoc=assoc_init(sizeof(int));
   for(i=0;i<10000;i++)
   {
      test_hashes[i]=rand()%10000;
      assoc_insert(&test_assoc, &test_hashes[i],&i);
   }
   assert(test_assoc->size<10000);
   assoc_free(test_assoc);
   test_assoc=assoc_init(sizeof(int));
   for(i=0;i<10000;i++)
   {
      test_hashes[i]=i;
      assoc_insert(&test_assoc, &test_hashes[i],&i);
   }
   assert(test_assoc->size==10000);
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
   if(keysize>8)
   {
      fprintf(stderr,"not an accepted key format\n");
      exit(EXIT_FAILURE);
   }
   assert(keysize>=0);
   n_assoc=_safe_calloc(1,sizeof(assoc));
   n_assoc->capacity=INITSIZE;
   n_assoc->bytesize=keysize;
   n_assoc->arr=(k_v_pair**)_safe_calloc(INITSIZE,sizeof(k_v_pair*));
   /*set to 0 because insertion function updates this*/
   n_assoc->size=0;
   n_assoc->lower_prime=INITLOWPRIME;
   return n_assoc;
}

/*seemed stupid to use init then recalloc*/
assoc* assoc_resized(int keysize,int n_cap,int old_cap)
{
   assoc* n_assoc;
   if(keysize>8)
   {
      fprintf(stderr,"not an accepted key format\n");
      exit(EXIT_FAILURE);
   }
   assert(keysize>=0);
   n_assoc=_safe_calloc(1,sizeof(assoc));
   n_assoc->capacity=n_cap;
   n_assoc->bytesize=keysize;
   n_assoc->arr=(k_v_pair**)_safe_calloc(n_cap,sizeof(k_v_pair*));
   n_assoc->size=0;
   n_assoc->lower_prime=old_cap;
   return n_assoc;
}

/* Free up all allocated space from 'a' */
void assoc_free(assoc* a)
{
   int i;
   for(i=0;i<(int)a->capacity;i++)
   {
      if(a->arr[i])
      {
         free(a->arr[i]);
      }
   }
   free(a->arr);
   free(a);
}

/*when resizing dont want to free all the values*/
void _partial_free(assoc* a)
{
   free(a->arr);
   free(a);
}


int _wrap_around(int max_num,int position)
{

   return abs(position % max_num);
}





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
   if(key)
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
assoc* _bigger_array(assoc* a)
{
   int bigger_table;
   int new_cap_target,prime_limit;
   assoc* a_n;
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
      a_n=assoc_resized(a->bytesize,bigger_table,a->capacity);
      return a_n;
   }
   return NULL;
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





bool _insertion(assoc* a,k_v_pair* kv)
{
   int hash_1;
   int hash_2;
   if(kv)
   {
      hash_1=_first_hash(kv->key,a);
      if(_insertion_helper(a,kv,hash_1))
      {
         return true;
      }
      else
      {
         hash_2=sec_hash(hash_1,a);
         if(_double_hash(hash_2,hash_1,a,kv))
         {
            return true;
         }
      }
   }
   return false;
}

/*because in theory the second hash step size will mean
that a free space is found eventually if there is one
and we check that the array is not full before _insertion
it is ok to have a seemingly infinite loop
however we will also put a count that acts as safeguard*/
bool _double_hash(int step_size,int start_point, assoc* a,k_v_pair* kv)
{
   int i,ind;
   int count;
   count=0;
   i=start_point;
   while(count<(int)(a->capacity*100))
   {
      count++;

      i = i-step_size;
      ind=_wrap_around(a->capacity,i);
      if(_insertion_helper(a,kv,ind))
      {
         return true;
      }

   }
   return false;
}


bool _insertion_helper(assoc* a,k_v_pair* kv,int insertion_point)
{

   if(kv)
   {
      if(a->arr[insertion_point]==NULL)
      {

         a->arr[insertion_point]=kv;
         a->size++;
         return true;
      }
      else
      {
         if(_same_key(a->arr[insertion_point]->key,kv->key,a->bytesize))
         {
            free(a->arr[insertion_point]);
            a->arr[insertion_point]=kv;
            return true;
         }
         else
         {
            return false;
         }
      }
   }
   return false;
}



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

/*pretty sure convention is to just point
to original data and not copy but could be wrong*/
k_v_pair* init_kv_pair(void* key, void* data)
{

   k_v_pair* n_kv;
   n_kv=_safe_calloc(1,sizeof(k_v_pair));
   n_kv->key = key;
   n_kv->value= data;
   return n_kv;
}




assoc* _resize(assoc* a)
{
   unsigned int i;
   assoc* n_ass;

   n_ass=_bigger_array(a);
   /*basically checks for nulls as bigger array
   returns NULL if a is NULL*/
   if(n_ass)
   {
      for(i=0;i<a->capacity;i++)
      {
         /*if data present*/
         if(a->arr[i])
         {
            if(!_insertion(n_ass,a->arr[i]))
            {
               fprintf(stderr, "error in reinserting values to hash table\n");
               exit(EXIT_FAILURE);
            }
         }
      }
      _partial_free(a);
      return n_ass;
   }
   return NULL;
}


void assoc_insert(assoc** a, void* key, void* data)
{
   assoc* a_ref;
   k_v_pair* kv;
   a_ref = *a;

   if(a_ref!=NULL)
   {
      /*if the array gets 60% full */

      if(a_ref->size > ((float)a_ref->capacity*0.6))
      {
         a_ref=_resize(a_ref);
      }
      kv=init_kv_pair(key,data);
      if(!_insertion(a_ref,kv))
      {
         if(kv)
         {
            free(kv);
         }
      }

   }
   /*setting again in case array grew*/
   *a = a_ref;

}



unsigned int assoc_count(assoc* a)
{
   if(a)
   {
      return a->size;
   }
   return 0;
}



/*theoretically the value could be in
any cell of the array if the hash function
was awful so will still need to check every
cell*/
void* assoc_lookup(assoc* a, void* key)
{

}
