#include "specific.h"
#include "../assoc.h"
#define TESTCAP 15053
#define DOTFILE 10000

/*((1-(1/100000))^100000)*100000 = probability that
number does not appear at least once*/


typedef struct test_struct
{
   int test;
   int test_2;
}test_struct;

/*test for this just in case*/
typedef struct test_struct_str
{
   char word[10];
   int number;
}test_struct_str;

/*just going to have a go for fun
using your queues one as sort of a template*/
void to_dot(void* key);


assoc* _assoc_resized(int keysize,int n_cap,int old_cap);
int _wrap_around(int max_num,int position);
/*http://www.cse.yorku.ca/~oz/hash.html#djb2 sdbm*/
unsigned int _first_hash(void* key,const assoc* a);
unsigned int _sec_hash(void* key,const assoc* a);
/*the capacity/lower prime is always going to be a unsigned int
so taking its modulus will mean that all numbers land within
the unsigned int range*/
unsigned long _byte_convert(void* key,const assoc* a);
assoc* _resize(assoc* a);
bool _insertion(assoc* a, k_v_pair* kv);
bool _insertion_helper(assoc* a,k_v_pair* kv,int insertion_point);
/*double hash does the probing to find empty spot*/
bool _double_hash(unsigned int step_size,unsigned int start_point, assoc* a, k_v_pair* kv);


void* _safe_calloc(size_t nitems, size_t size);
k_v_pair* _init_kv_pair(void* key, void* data);
assoc* _bigger_array(assoc* a);
bool _same_key(const void* key1,const void* key2,int bytesize);
int _sieve_of_e_helper(int new_cap_target);

/*pretty much the reverse of _double_hash()*/
void* _assoc_lookup_helper(int step_size,int start_point,\
                           const assoc* a,const void* key);
/*frees structs but not kv pairs*/
void _partial_free(assoc* a);
void test(void);
int main(void)
{

   test();
   return 0;
}


void test(void)
{
   int i,test_hash1,test_hash2,test_int,count,j;
   void* void_ptr;
   long i_long,i_long2;
   double t_double;
   float test_float,test_float2;
   FILE *fp;
   test_struct test_s,test_y;
   test_struct_str ss_test;
   k_v_pair* test_kv;
   int test_hashes[10000]={0};
   int test_hashes2[10000]={0};
   int test_arr[10000]={0};
   int histogram_hashes[TESTCAP]={0};
   char word[50];
   char word2[50];
   assoc* test_assoc,*test_assoc2;

   test_s.test=10;
   i=10;
   to_dot(&i);

   test_assoc=assoc_init(sizeof(test_struct));
   test_assoc2=assoc_init(sizeof(int));

   /*checking for NULLS in assoc_insert so dont need to double
   check*/

   test_s.test=10;
   test_y.test=10;
   assert(_first_hash(&test_s,test_assoc)==_first_hash(&test_s,test_assoc));
   assert(_first_hash(&test_s,test_assoc)==_first_hash(&test_y,test_assoc));
   test_s.test_2=11;
   test_y.test_2=12;
   assert(_first_hash(&test_s,test_assoc)!=_first_hash(&test_y,test_assoc));
   test_s.test_2=12;
   test_s.test=11;
   assert(_first_hash(&test_s,test_assoc)!=_first_hash(&test_y,test_assoc));

   assert(_first_hash(&test_s,test_assoc)!=_first_hash(&i,test_assoc2));

   assoc_free(test_assoc);
   assoc_free(test_assoc2);
   strcpy(ss_test.word,"test");
   ss_test.number=1;
   test_assoc=assoc_init(sizeof(test_struct_str));
   assert(_first_hash(&ss_test,test_assoc));
   assoc_free(test_assoc);

   /*testing hashing function*/

   test_assoc=assoc_init(0);
   test_assoc->capacity=TESTCAP;

   fp = fopen("../../../Data/Words/eng_370k_shuffle.txt", "rt");
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
      assert((test_hashes[i]<TESTCAP)&&(test_hashes[i]>=0));
   }
   /*testing spread of hash distribution roughly - should use
   a chi squared test or something similar but this will do*/
   for(i=0;i<TESTCAP;i++)
   {
      assert(histogram_hashes[i]<50);
   }
   fclose(fp);
   test_assoc->capacity=INITSIZE;
   assoc_free(test_assoc);




   test_assoc=assoc_init(sizeof(int));
   test_assoc->capacity=TESTCAP;
   memset(histogram_hashes,0,sizeof(int)*TESTCAP);
   /*TESTCAP is prime*/
   for(i=0;i<10000;i++)
   {
      test_hashes[i]=_first_hash(&i,test_assoc);
      histogram_hashes[_first_hash(&i,test_assoc)]++;
   }
   for(i=0;i<10000;i++)
   {
      assert((test_hashes[i]<TESTCAP)&&(test_hashes[i]>=0));
   }

   for(i=0;(unsigned int)i<test_assoc->capacity;i++)
   {

      assert(histogram_hashes[i]<50);
   }
   memset(histogram_hashes,0,sizeof(int)*TESTCAP);
   test_assoc->capacity=INITSIZE;
   assoc_free(test_assoc);



   test_assoc=assoc_init(sizeof(long));
   test_assoc->capacity=TESTCAP;
   i=0;
   for(i_long=INT_MAX;i_long<(INT_MAX+(long)1000);i_long++)
   {
      test_hashes[i]=_first_hash(&i_long,test_assoc);
      histogram_hashes[_first_hash(&i_long,test_assoc)]++;
      i++;
   }
   for(i=0;i<INITSIZE;i++)
   {
      assert((test_hashes[i]<TESTCAP)&&(test_hashes[i]>=0));

   }
   for(i=0;(unsigned int)i<test_assoc->capacity;i++)
   {

      assert(histogram_hashes[i]<50);
   }
   test_assoc->capacity=INITSIZE;
   assoc_free(test_assoc);



   memset(histogram_hashes,0,sizeof(int)*TESTCAP);

   test_assoc=assoc_init(sizeof(int));
   test_assoc->capacity=TESTCAP;
   for(i=0;i<10000;i++)
   {
      test_hashes[i]=_first_hash(&i,test_assoc);
      histogram_hashes[_first_hash(&i,test_assoc)]++;
   }
   for(i=0;i<10000;i++)
   {
      assert((test_hashes[i]<TESTCAP)&&(test_hashes[i]>=0));

   }
   for(i=0;(unsigned int)i<test_assoc->capacity;i++)
   {

      assert(histogram_hashes[i]<5);
   }
   memset(histogram_hashes,0,sizeof(int)*TESTCAP);
   test_assoc->capacity=INITSIZE;
   assoc_free(test_assoc);




   test_assoc=assoc_init(sizeof(double));
   test_assoc->capacity=TESTCAP;
   i=0;
   for(t_double=10;t_double<11;t_double+=0.001)
   {

      test_hashes[i]=_first_hash(&t_double,test_assoc);
      histogram_hashes[_first_hash(&t_double,test_assoc)]++;
      i++;
   }
   for(i=0;i<1000;i++)
   {
      assert((test_hashes[i]<TESTCAP)&&(test_hashes[i]>=0));
   }
   for(i=0;(unsigned int)i<test_assoc->capacity;i++)
   {
      assert(histogram_hashes[i]<20);
   }
   test_assoc->capacity=INITSIZE;
   assoc_free(test_assoc);

   test_assoc=assoc_init(sizeof(double));
   t_double=30.666;
   void_ptr=&t_double;
   assert(_first_hash(void_ptr,test_assoc)==_first_hash(void_ptr,test_assoc));
   assoc_free(test_assoc);

   test_assoc=assoc_init(0);
   assert(_first_hash(word,test_assoc)==_first_hash(word,test_assoc));

   strcpy(word,"");
   assert(_first_hash(word,test_assoc)==_first_hash(word,test_assoc));
   assoc_free(test_assoc);


   test_assoc=assoc_init(sizeof(int));
   i=30;
   void_ptr=&i;
   assert(_first_hash(void_ptr,test_assoc)==_first_hash(void_ptr,test_assoc));
   assoc_free(test_assoc);

   /*second hashing func*/

   test_assoc=assoc_init(sizeof(int));

   i=0;
   assert(_byte_convert(&i,test_assoc)==0);
   i=100;
   assert(_byte_convert(&i,test_assoc)==100);

   assoc_free(test_assoc);
   /*not sure on testing byte convert with floats
   as the bytes are a bit funky*/
   test_assoc=assoc_init(sizeof(float));
   test_float=0.0;
   assert(_byte_convert(&test_float,test_assoc)==0);
   test_float=10.679;
   assert(_byte_convert(&test_float,test_assoc)==_byte_convert(&test_float,test_assoc));
   test_float2=10.678;
   assert(_byte_convert(&test_float,test_assoc)!=_byte_convert(&test_float2,test_assoc));
   /*floats are accurate to 6 dps*/
   test_float=10.555556;
   test_float2=10.555555;
   assert(_byte_convert(&test_float,test_assoc)==_byte_convert(&test_float,test_assoc));
   assert(_byte_convert(&test_float,test_assoc)!=_byte_convert(&test_float2,test_assoc));

   assoc_free(test_assoc);
   assert(_byte_convert(&test_float,NULL)==0);
   assert(_byte_convert(NULL,NULL)==0);

   test_assoc=assoc_init(0);
   assert(_byte_convert(NULL,test_assoc)==0);
   strcpy(word,"hello");

   assert(_byte_convert(word,test_assoc)==532);
   strcpy(word,"");

   /*things like this are why i put the safeguard in
   my double hash func*/
   assert(_byte_convert(word,test_assoc)==0);
   assert(_sec_hash(word,test_assoc)!=0);
   strcpy(word," ");
   assert(_byte_convert(word,test_assoc)==32);
   assoc_free(test_assoc);



   test_assoc=assoc_init(sizeof(int));
   test_assoc->capacity=TESTCAP;
   for(i=0;i<10000;i++)
   {

      test_hash2=_sec_hash(&i,test_assoc);
      assert((test_hash2<TESTCAP)&&(test_hash2>0));
   }
   test_assoc->capacity=INITSIZE;
   assoc_free(test_assoc);

   test_assoc=assoc_init(sizeof(char));
   test_assoc->capacity=TESTCAP;

   for(i='a';i<'z';i++)
   {

      test_hash2=_sec_hash(&i,test_assoc);
      assert((test_hash2<TESTCAP)&&(test_hash2>0));
   }
   test_assoc->capacity=INITSIZE;
   assoc_free(test_assoc);


   test_assoc=assoc_init(0);
   strcpy(word,"");
   assert(_sec_hash(word,test_assoc)!=0);
   strcpy(word,"jdjndjvjvbj");
   assert(_sec_hash(word,test_assoc)!=0);
   assoc_free(test_assoc);
   /*test for same results- not entirely necessary but would be nice
   to have different results for each hash function on the same key*/
   test_assoc=assoc_init(sizeof(int));
   test_assoc->capacity=TESTCAP;
   for(i=0;i<10000;i++)
   {
      test_arr[i]=i;
      test_hashes[i]=_first_hash(&test_arr[i],test_assoc);
      test_hashes2[i]=_sec_hash(&test_arr[i],test_assoc);
      assert(test_hashes[i]<TESTCAP&&test_hashes[i]>=0);
      assert(test_hashes2[i]<TESTCAP&&test_hashes2[i]>0);
      /*dont think you want the hash function
      to be getting the same number frequently*/
   }
   count=0;
   for(i=0;i<10000;i++)
   {
      for(j=i+1;j<10000;j++)
      {
         if(test_hashes[i]==test_hashes[j])
         {
            if(test_hashes2[i]==test_hashes2[j])
            {
               count+=1;
            }
         }
      }
   }
   assert(count<5);

   /*just checking it works for negative nums as well*/
   count=0;
   for(i=0;i> -10000;i--)
   {
      test_arr[count]=i;
      test_hashes[count]=_first_hash(&test_arr[count],test_assoc);
      test_hashes2[count]=_sec_hash(&test_arr[count],test_assoc);
      assert(test_hashes[count]<TESTCAP&&test_hashes[count]>=0);
      assert(test_hashes2[count]<TESTCAP&&test_hashes2[count]>0);
      count++;
   }

   count=0;
   for(i=0;i<10000;i++)
   {
      for(j=i+1;j<10000;j++)
      {
         if(test_hashes[i]==test_hashes[j])
         {
            if(test_hashes2[i]==test_hashes2[j])
            {
               count+=1;
            }
         }
      }
   }
   test_assoc->capacity=INITSIZE;
   assoc_free(test_assoc);
   assert(count<5);

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

   test_assoc->capacity=TESTCAP;
   test_assoc2=_bigger_array(test_assoc);
   assert(test_assoc2->capacity==36109);
   assert(test_assoc2->lower_prime==TESTCAP);
   test_assoc->capacity=INITSIZE;
   assoc_free(test_assoc);
   assoc_free(test_assoc2);
   assert(!_bigger_array(NULL));


   assert(_same_key(NULL,NULL,sizeof(int))==false);
   assert(_same_key(NULL,&i,sizeof(int))==false);
   j=7;
   i=6;
   assert(_same_key(&j,&i,sizeof(int))==false);
   assert(_same_key(&i,&i,sizeof(int))==true);
   strcpy(word,"dan");
   strcpy(word2,"dna");
   assert(_same_key(word,word2,0)==false);
   assert(_same_key(word,word,0)==true);
   /*inserting a kv_pair*/
   test_assoc=assoc_init(sizeof(long));
   for(i=0;i<(int)test_assoc->capacity;i++)
   {
      assert(test_assoc->arr[i]==NULL);
   }
   assert(test_assoc->size==0);
   i_long=10;
   i_long2=12;

   assert(_init_kv_pair(NULL,&i_long2)==NULL);
   test_kv=_init_kv_pair(&i_long,&i_long2);
   /*adding to empty space*/
   assert(_insertion_helper(test_assoc,test_kv,0));
   assert(*(long*)test_assoc->arr[0]->key==10);
   assert(*(long*)test_assoc->arr[0]->value==12);


   test_kv=_init_kv_pair(&i_long,&i_long);
   /*updating value with same key*/
   assert(_insertion_helper(test_assoc,test_kv,0));
   assert(*(long*)test_assoc->arr[0]->value==10);

   test_kv=_init_kv_pair(&i_long2,&i_long);
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
   test_kv=_init_kv_pair(&word,&i);
   assert(_insertion_helper(test_assoc,test_kv,0));
   assert(strcmp(test_assoc->arr[0]->key,word)==0);
   assert(*(int*)test_assoc->arr[0]->value==1);

   test_s.test_2 =10;
   test_kv=_init_kv_pair(&word,&test_s);
   assert(_insertion_helper(test_assoc,test_kv,0));

   assert((*(test_struct*)test_assoc->arr[0]->value).test_2==10);

   strcpy(word2,"938Neilliscool\n");
   test_kv=_init_kv_pair(&word2,&test_s);
   assert(!_insertion_helper(test_assoc,test_kv,0));
   free(test_kv);
   assert(!_insertion_helper(test_assoc,NULL,0));
   assert(!_insertion_helper(test_assoc,NULL,0));
   assoc_free(test_assoc);


   test_assoc=assoc_init(sizeof(int));

   i=50;
   test_kv=_init_kv_pair(&i,&word2);
   assert(_insertion_helper(test_assoc,test_kv,0));
   assert(strcmp(word2,(char*)test_assoc->arr[0]->value)==0);
   test_int =49;

   test_kv=_init_kv_pair(&test_int,&i);
   assert(_double_hash(7,0,test_assoc,test_kv));
   assert(*(int*)test_assoc->arr[7]->value==50);

   test_float=50.1;
   test_kv=_init_kv_pair(&test_float,&test_int);
   assert(_double_hash(7,0,test_assoc,test_kv));
   assert(*(int*)test_assoc->arr[14]->value==49);
   assoc_free(test_assoc);



   test_assoc=assoc_init(sizeof(int));
   for(i=0;i<15;i++)
   {
      test_kv=_init_kv_pair(&test_hashes[i],&i);
      assert(_insertion(test_assoc,test_kv));
   }

   assert(test_assoc->size==15);
   assoc_free(test_assoc);


   test_assoc=assoc_init(sizeof(int));
   for(i=0;i<15;i++)
   {
      test_kv=_init_kv_pair(&test_hashes[i],&i);
      assert(_insertion(test_assoc,test_kv));
   }

   test_assoc=_resize(test_assoc);
   assert(test_assoc->size==15);
   assert(test_assoc->capacity==37);
   /*check that after resizing everything still there*/

   for(i=0;i<15;i++)
   {
      test_hash1=_first_hash(&test_hashes[i],test_assoc);
      assert(*(int*)test_assoc->arr[test_hash1]->value==i);
   }

   assoc_free(test_assoc);
   srand(time(NULL));

   test_assoc=assoc_init(sizeof(int));


   assoc_insert(&test_assoc,NULL,NULL);
   assoc_insert(&test_assoc,NULL,&i);
   assert(test_assoc->size==0);


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


   /*testing lookup*/


   test_assoc=assoc_init(sizeof(int));
   i=0;
   assert(assoc_lookup(test_assoc,&i)==NULL);
   assert(assoc_lookup(NULL,&i)==NULL);
   assert(assoc_lookup(NULL,NULL)==NULL);
   assert(assoc_lookup(test_assoc,NULL)==NULL);

   for(i=0;i<10000;i++)
   {
      test_hashes[i]=i;
      assoc_insert(&test_assoc, &test_hashes[i],&i);
      assert(assoc_lookup(test_assoc,&test_hashes[i])==&i);
   }

   assoc_free(test_assoc);

   test_assoc=assoc_init(0);

   fp = fopen("../../../Data/Words/eng_370k_shuffle.txt", "rt");
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


      assoc_insert(&test_assoc, word,&i);
      assert(assoc_lookup(test_assoc,word)==&i);
   }

   fclose(fp);
   assoc_free(test_assoc);

   test_assoc=assoc_init(sizeof(int));
   test_s.test=10;
   for(i=0;i<10000;i++)
   {
      assert(assoc_count(test_assoc)==(unsigned int)i);
      test_hashes[i]=i;
      assoc_insert(&test_assoc,&test_hashes[i],&test_s);

   }
   for(i=0;i<10000;i++)
   {
      assert(assoc_lookup(test_assoc,&test_hashes[i])==&test_s);
   }
   assoc_free(test_assoc);
   assert(assoc_count(NULL)==0);

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
   n_assoc->arr=(k_v_pair**)_safe_calloc(INITSIZE,sizeof(k_v_pair*));
   /*set to 0 because insertion function updates this*/
   n_assoc->size=0;
   n_assoc->lower_prime=INITLOWPRIME;
   return n_assoc;
}


assoc* _assoc_resized(int keysize,int n_cap,int old_cap)
{
   assoc* n_assoc;
   assert(keysize>=0);
   n_assoc=_safe_calloc(1,sizeof(assoc));
   n_assoc->capacity=n_cap;
   if(n_assoc->capacity>UINT_MAX/2)
   {
      fprintf(stderr,"warning -potential overflow in next resize\n");
   }
   n_assoc->bytesize=keysize;
   n_assoc->arr=(k_v_pair**)_safe_calloc(n_cap,sizeof(k_v_pair*));
   /*set to 0 because insertion function updates this*/
   n_assoc->size=0;
   n_assoc->lower_prime=old_cap;
   return n_assoc;
}

/* Free up all allocated space from 'a' */
void assoc_free(assoc* a)
{
   unsigned int i;
   if(a)
   {
      for(i=0;i<a->capacity;i++)
      {
         if(a->arr[i])
         {
            free(a->arr[i]);
         }
      }
      free(a->arr);
      free(a);
   }
}

/*when resizing dont want to free all the values*/
void _partial_free(assoc* a)
{
   free(a->arr);
   free(a);
}


int _wrap_around(int max_num,int position)
{
   if(position<0)
   {
      return max_num-position;
   }
   return position % max_num;
}




/*http://www.cse.yorku.ca/~oz/hash.html#djb2 sdbm*/
unsigned int _first_hash(void* key,const assoc* a)
{
   unsigned int i;
   unsigned long hash;
   char* str;
   str= (char*) key;
   hash=0;


   if(a->bytesize>0)
   {
      for(i=0;i< a->bytesize;i++,str++)
      {
         hash+= (*str)+(hash<<SDBM_ROLL_1) +(hash<<SDBM_ROLL_2)-hash;
      }
      return hash% a->capacity;
   }
   else
   {
      i=0;
      while(str[i])
      {
         hash+= str[i]+(hash<<SDBM_ROLL_1) +(hash<<SDBM_ROLL_2)-hash;
         i++;
      }
      return hash% a->capacity;
   }

}




/*https://www.geeksforgeeks.org/double-hashing/ idea for second
hashing function
 */
unsigned int _sec_hash(void* key,const assoc* a)
{
   unsigned long hash;
   hash=_byte_convert(key,a);
   hash=a->lower_prime - (hash % a->lower_prime);
   if(hash==0)
   {
      /*fail safe as if we return 0 then we will
      probe same index of our array over and over*/
      return FAILSAFE;
   }
   else
   {
      return hash;
   }
}

unsigned long _byte_convert(void* key,const assoc* a)
{
   unsigned int i;
   unsigned long s;
   char* str;
   str= (char*) key;
   s=0;
   if(key&&a)
   {
      if(a->bytesize>0)
      {
         for(i=0;i< a->bytesize;i++,str++)
         {
            s+= *str;
         }
      }
      else
      {
         i=0;
         while(str[i])
         {
            s+= str[i];
            i++;
         }
      }
   }
   return s;
}



/*assigns a prime number to table size and a lower*/
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
         prime_limit=(double)prime_limit*PRIMESCALE;
      }
      a_n=_assoc_resized(a->bytesize,bigger_table,a->capacity);
      return a_n;
   }
   return NULL;
}

/*https://en.wikipedia.org/wiki/Sieve_of_Eratosthenes*/
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
   for(p=START;p*p<new_cap_target;p++)
   {
      if(bool_arr[p]==true)
      {
         for(i=p*p; i<new_cap_target;i+=p)
         {
            bool_arr[i]=false;
         }
      }
   }
   /*go backwards from target and return highest prime*/
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
   unsigned int hash_1,hash_2;
   if(kv)
   {
      hash_1=_first_hash(kv->key,a);
      if(_insertion_helper(a,kv,hash_1))
      {
         return true;
      }
      else
      {
         hash_2=_sec_hash(kv->key,a);
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
bool _double_hash(unsigned int step_size,unsigned int start_point, assoc* a, k_v_pair* kv)
{
   int i;
   unsigned int count,ind;
   count=0;
   i=start_point;
   while(count<a->capacity)
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
      }
   }
   return false;
}


/*although technically if key1 and key2 are NULLs
they would be the same but for this im going to
say they arent because i want functions to exit
if both are NULL anyway*/
bool _same_key(const void* key1,const void* key2,int bytesize)
{
   if(key1 && key2)
   {
      if(bytesize==0)
      {
         if(strcmp(key1,key2)==0)
         {
            return true;
         }
         return false;
      }
   }
   return false;
}

k_v_pair* _init_kv_pair(void* key, void* data)
{

   k_v_pair* n_kv;
   if(key)
   {
      n_kv=_safe_calloc(1,sizeof(k_v_pair));
      n_kv->key = key;
      n_kv->value= data;
      return n_kv;
   }
   return NULL;
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
      if(key)
      {
         /*if the array gets 60% full */
         if(a_ref->size > ((float)a_ref->capacity*0.6))
         {
            a_ref=_resize(a_ref);
            *a = a_ref;
         }
         kv=_init_kv_pair(key,data);
         if(!_insertion(a_ref,kv))
         {
            /*failsafe if insertion fails for memory leaks*/
            if(kv)
            {
               free(kv);
            }
         }
      }
   }
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
cell
but if we find an empty cell then not in*/
void* assoc_lookup(assoc* a, void* key)
{
   unsigned int hash_1, hash_2;

   /*check if either are null*/
   if(a && key)
   {
      hash_1=_first_hash(key,a);
      if(!a->arr[hash_1])
      {
         return NULL;
      }
      if(_same_key(a->arr[hash_1]->key,key,a->bytesize))
      {
         return a->arr[hash_1]->value;
      }
      hash_2=_sec_hash(key,a);
      return _assoc_lookup_helper(hash_2,hash_1,a,key);

   }

   return NULL;
}

/*pretty much the reverse of _double_hash()*/
void* _assoc_lookup_helper(int step_size,int start_point,\
                           const assoc* a,const void* key)
{
   int i,ind;
   unsigned int count;
   count=0;
   i=start_point;
   while(count < a->capacity)
   {
      ind=_wrap_around(a->capacity,i);
      /*if empty then not in hash table*/
      if(!a->arr[ind])
      {
         return NULL;
      }
      if(_same_key(a->arr[ind]->key,key,a->bytesize))
      {
         return a->arr[ind]->value;
      }
      i=i-step_size;
      count++;
   }
   return NULL;
}











/*this is for ints, only use for small tables,give it a key and
it will show you where it double bounces*/
void to_dot(void* key)
{
   assoc* n_assoc;
   char str[DOTFILE];
   char tmp[DOTFILE];
   unsigned int i;
   int next_step,first_step,step_size,prev_step,count;
   FILE* fp;
   n_assoc=assoc_init(sizeof(int));

   sprintf(str, "digraph g {graph [ rankdir=\"LR\"];Subgraph cluster{");

   for(i=0;i<n_assoc->capacity;i++)
   {

       sprintf(tmp, "n%d [label=\"position=%d",i,i);
       strcat(str, tmp);
       sprintf(tmp, " }\" shape = \"record\"];\n");
       strcat(str, tmp);


   }
   sprintf(tmp, "n%d [label=\"{data incoming key =%d}\"] ",-1, *(int*)key);
   strcat(str, tmp);
   first_step=_first_hash(key,n_assoc);
   sprintf(tmp, "n%d->n%d;",-1,first_step );
   strcat(str, tmp);
   step_size=_sec_hash(key,n_assoc);
   prev_step=first_step;
   next_step=_wrap_around(n_assoc->capacity,first_step-step_size);
   count=0;
   while(count<n_assoc->capacity)
   {

      sprintf(tmp, "n%d->n%d;",prev_step,next_step);
      strcat(str, tmp);
      prev_step=next_step;
      next_step=_wrap_around(n_assoc->capacity,next_step-step_size);
      count++;
   }


   /*for(i=0;i<n_assoc->capacity;i++)
   {
      if(n_assoc->arr[i])
      {
         next_step= _wrap_around(n_assoc->capacity,i-_sec_hash(n_assoc->arr[i]->key,n_assoc));
         sprintf(tmp, "n%d->n%d;",i,next_step );
         strcat(str, tmp);
      }
   }*/
   strcat(str,"}}");
   fp = fopen("test.dot", "wt");
   fprintf(fp, "%s\n", str);
   fclose(fp);
   assoc_free(n_assoc);
}
