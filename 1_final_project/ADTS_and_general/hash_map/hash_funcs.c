#include "hash_funcs.h"
#define TESTCAP 15053

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





void test(void)
{
   int i,j,count;
   k_v_pair* test_kv;
   assoc* test_assoc,*test_assoc2;
   int test_hashes[10000]={0};
   int test_hashes2[10000]={0};
   int histogram_hashes[TESTCAP]={0};
   FILE* fp;
   char word[50];
   char word2[50];
   char word_arr[500][50]={{'\0'}};


   test_assoc=assoc_init();
   test_assoc->capacity=TESTCAP;
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
      assert((test_hashes[i]<TESTCAP)&&(test_hashes[i]>=0));
   }
   /*testing spread of hash distribution roughly - should use
   a chi squared test or something similar but this will do*/
   for(i=0;i<10000;i++)
   {
      assert(histogram_hashes[i]<50);
   }
   fclose(fp);
   test_assoc->capacity=INIT_SIZE_HASH;
   assoc_free(test_assoc);

   test_assoc=assoc_init();
   assert(_byte_convert(NULL)==0);
   strcpy(word,"hello");

   assert(_byte_convert(word)==532);
   strcpy(word,"");

   /*things like this are why i put the safeguard in
   my double hash func*/
   assert(_byte_convert(word)==0);
   assert(_sec_hash(word,test_assoc)!=0);
   strcpy(word," ");
   assert(_byte_convert(word)==32);
   assoc_free(test_assoc);




   test_assoc=assoc_init();
   test_assoc->capacity=TESTCAP;
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
      test_hashes2[i]=_sec_hash(word,test_assoc);
      assert(test_hashes[i]<TESTCAP&&test_hashes[i]>=0);
      assert(test_hashes2[i]<TESTCAP&&test_hashes2[i]>0);

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
   assert(count<300);
   fclose(fp);
   test_assoc->capacity=INIT_SIZE_HASH;
   assoc_free(test_assoc);



   assert(_wrap_around(10,11)==1);
   assert(_wrap_around(10,10)==0);

   /*should just cycle from 0 to 9 over and over*/
   j=0;
   for(i=20;i>0;i--)
   {
      assert(_wrap_around(10,i)==j);
      j--;
      if(j<0)
      {
         j=9;
      }
   }
   j=0;
   for(i=0;i<-20;i++)
   {
      assert(_wrap_around(10,i)==j);
      j++;
      if(j>9)
      {
         j=0;
      }
   }

   i=10;
   test_assoc=assoc_init();
   test_kv=_init_kv_pair(word,&i);
   assert(_insertion_helper(test_assoc,test_kv,0));
   assert(strcmp(word,test_assoc->arr[0]->key)==0);
   assert(*(int*)test_assoc->arr[0]->value==10);


   j=20;
   test_kv=_init_kv_pair(word2,&j);
   assert(_double_hash(7,0,test_assoc,test_kv));
   assert(*(int*)test_assoc->arr[7]->value==20);


   count=30;
   test_kv=_init_kv_pair(" ",&count);
   assert(_double_hash(7,0,test_assoc,test_kv));
   assert(*(int*)test_assoc->arr[14]->value==30);

   assoc_free(test_assoc);


   /*gets closest lower prime */
   assert(_sieve_of_e_helper(1049)==1039);
   assert(_sieve_of_e_helper(2677)==2671);
   assert(_sieve_of_e_helper(0)==NOPRIME);

   test_assoc=assoc_init();
   test_assoc2=_bigger_array(test_assoc);
   assert(test_assoc2->capacity==37);
   assert(test_assoc2->lower_prime==INIT_SIZE_HASH);
   assoc_free(test_assoc2);

   test_assoc->capacity=TESTCAP;
   test_assoc2=_bigger_array(test_assoc);
   assert(test_assoc2->capacity==36109);
   assert(test_assoc2->lower_prime==TESTCAP);
   test_assoc->capacity=INIT_SIZE_HASH;
   assoc_free(test_assoc);
   assoc_free(test_assoc2);
   assert(!_bigger_array(NULL));
   assert(_same_key(NULL,NULL)==false);
   assert(_same_key(NULL,word)==false);
   assert(_same_key(word,NULL)==false);

   strcpy(word,"dan");
   strcpy(word2,"dna");
   assert(_same_key(word,word2)==false);
   assert(_same_key(word,word)==true);

   test_assoc=assoc_init();
   test_kv=_init_kv_pair(word,NULL);
   assert(_insertion_helper(test_assoc,test_kv,0));
   assert(strcmp(test_assoc->arr[0]->key,word)==0);
   strcpy(word2,"938Neilliscool\n");
   test_kv=_init_kv_pair(word2,NULL);
   assert(!_insertion_helper(test_assoc,test_kv,0));
   free(test_kv);
   assert(!_insertion_helper(test_assoc,NULL,0));
   assert(!_insertion_helper(test_assoc,NULL,0));
   assoc_free(test_assoc);




   strcpy(word,"");
   test_assoc=assoc_init();

   for(i=0;i<15;i++)
   {
      test_hashes[i]=i;
      strcpy(word_arr[i],word);
      test_kv=_init_kv_pair(word_arr[i],&test_hashes[i]);
      assert(_insertion(test_assoc,test_kv));
      strcat(word,"a");

   }

   test_assoc=_resize(test_assoc);
   assert(test_assoc->size==15);
   assert(test_assoc->capacity==37);
   /*check that after resizing everything still there*/
   strcpy(word,"");
   for(i=0;i<15;i++)
   {
      assert(assoc_lookup(test_assoc,word));
      strcat(word,"a");
   }



   assert(assoc_lookup(NULL,word)==NULL);
   assert(assoc_lookup(NULL,NULL)==NULL);
   assert(assoc_lookup(test_assoc,NULL)==NULL);
   assoc_free(test_assoc);


   test_assoc=assoc_init();
   fp = fopen("eng_370k_shuffle.txt", "rt");
   if(fp==NULL)
   {
      fprintf(stderr,"file not there?\n");
      exit(EXIT_FAILURE);
   }
   for(i=0;i<500;i++)
   {
      if(fscanf(fp, "%s", word)!=1)
      {
         fprintf(stderr,"failed scan of word\n");
         exit(EXIT_FAILURE);
      }
      strcpy(word_arr[i],word);

      assert(assoc_count(test_assoc)==(unsigned int)i);
      test_hashes[i]=i;
      assoc_insert(&test_assoc, word_arr[i],&test_hashes[i]);
      assert(*(int*)assoc_lookup(test_assoc,word_arr[i])==i);


   }
   fclose(fp);
   assoc_free(test_assoc);

}








assoc* assoc_init(void)
{
   assoc* n_assoc;
   static bool tested=false;
   if(!tested)
   {
      tested=true;
      test();
   }
   n_assoc=safe_calloc(1,sizeof(assoc));
   n_assoc->capacity=INIT_SIZE_HASH;

   n_assoc->arr=(k_v_pair**)safe_calloc(INIT_SIZE_HASH,sizeof(k_v_pair*));
   /*set to 0 because insertion function updates this*/
   n_assoc->size=0;
   n_assoc->lower_prime=INITLOWPRIME;
   return n_assoc;
}

assoc* _assoc_resized(int n_cap,int old_cap)
{
   assoc* n_assoc;

   n_assoc=safe_calloc(1,sizeof(assoc));
   n_assoc->capacity=n_cap;

   n_assoc->arr=(k_v_pair**)safe_calloc(n_cap,sizeof(k_v_pair*));
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



/*
void _free_func_cont(func_cont* to_free)
{
   int i;
   if(to_free)
   {
      for(i=0;i<to_free->capacity;i++)
      {

         free(to_free->words[i]);
      }
      for(i=0;i<NUMVARS;i++)
      {
         if(to_free->var_array[i])
         {
            printf("here\n");
            free(to_free->var_array[i]);
         }
      }
      if(to_free->words)
      {
         free(to_free->words);
      }
      if(to_free->stackptr)
      {
         stack_free(to_free->stackptr);
      }

      free(to_free);
   }

}
*/





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

/*http://www.cse.yorku.ca/~oz/hash.html#djb2 sdbm*/
unsigned int _first_hash(char* key,const assoc* a)
{
   unsigned int i;
   unsigned long hash;

   hash=0;
   i=0;
   while(key[i])
   {
      hash+= key[i]+(hash<<SDBM_ROLL_1) +(hash<<SDBM_ROLL_2)-hash;
      i++;
   }
   return hash% a->capacity;


}



/*https://www.geeksforgeeks.org/double-hashing/ idea for second
hashing function
 */
unsigned int _sec_hash(char* key,const assoc* a)
{
   unsigned long hash;
   hash=_byte_convert(key);
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

unsigned long _byte_convert(char* key)
{
   unsigned int i;
   unsigned long s;

   s=0;
   if(key)
   {
      i=0;
      while(key[i])
      {
         s+= key[i];
         i++;
      }
   }
   return s;
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
         if(_same_key(a->arr[insertion_point]->key,kv->key))
         {
            free(a->arr[insertion_point]);
            a->arr[insertion_point]=kv;

            return true;
         }
      }
   }
   return false;
}

bool _double_hash(unsigned int step_size,unsigned int start_point, \
               assoc* a, k_v_pair* kv)
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

k_v_pair* _init_kv_pair(char* key, void* data)
{

   k_v_pair* n_kv;
   if(key)
   {

      n_kv=safe_calloc(1,sizeof(k_v_pair));
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


void assoc_insert(assoc** a, char* key, void* data)
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
      a_n=_assoc_resized(bigger_table,a->capacity);
      return a_n;
   }
   return NULL;
}


/*https://en.wikipedia.org/wiki/Sieve_of_Eratosthenes*/
int _sieve_of_e_helper(int new_cap_target)
{
   bool *bool_arr;
   int i,p;
   bool_arr= (bool *)safe_calloc(new_cap_target,sizeof(bool));
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


void* assoc_lookup(assoc* a, char* key)
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
      if(_same_key(a->arr[hash_1]->key,key))
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
                           const assoc* a,const char* key)
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
      if(_same_key(a->arr[ind]->key,key))
      {
         return a->arr[ind]->value;
      }
      i=i-step_size;
      count++;
   }
   return NULL;
}


bool _same_key(const char* key1,const char* key2)
{
   if(key1 && key2)
   {
      if(strcmp(key1,key2)==0)
      {
         return true;
      }
   }
   return false;
}
