#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <string.h>
#include <assert.h>
#include <ctype.h>
#include <limits.h>
#include <math.h>


#define TESTCAP 15053
#define INITSIZE 17

#define SCALEFACTOR 2


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
   unsigned int tot_size;
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
};

/*test for this just in case*/
typedef struct test_struct_str
{
   char word[10];
   int number;
}test_struct_str;

void assoc_free(assoc* a);
assoc* assoc_init(int keysize);


table* _table_init(assoc* a,\
                  unsigned int (*hash_f)(void*,const table* ));
/*return void then cast when appropriate*/
void* _safe_calloc(size_t nitems, size_t size);
int _wrap_around(int max_num,int position);
/*http://www.cse.yorku.ca/~oz/hash.html#djb2 sdbm*/
unsigned int _first_hash(void* key,const table* a);
/*your version of Bernstein pretty much*/
unsigned int _sec_hash(void* key,const table* a);
assoc* assoc_init(int keysize);
k_v_pair* _init_kv_pair(void* key, void* data);
bool _same_key(const void* key1,const void* key2,int bytesize);
k_v_pair* _table_insertion(table* a, k_v_pair* kv);
bool _table_reinsert(assoc* n_assoc, table* old_table);
assoc* _assoc_resized(int keysize,int n_cap);
assoc* _resize(assoc* a, k_v_pair* leftover,int n_size);
bool _table_reinsert(assoc* n_assoc, table* old_table);
/*returns NULL if we dont need to resize
otherwise returns the pair that was bumped out last*/
k_v_pair* _outer_insert(k_v_pair* kv,assoc* a);
/*when resizing dont want to free all the values*/
void _partial_free(assoc* a);

int _log_2(int num);
void test(void);


int main(void)
{
   test();
   return 0;

}

void test(void)
{
   assoc* test_assoc,*test_assoc2;
   int i,j,count,x;
   int test_arr[10000];
   int test_hashes[10000];
   int test_hashes2[10000];
   char word[50];
   float test_float,test_float2;
   double d_i;
   k_v_pair* test_kv;
   FILE *fp;


   test_assoc=assoc_init(sizeof(int));
   test_assoc->table_1->capacity=TESTCAP;
   test_assoc->table_2->capacity=TESTCAP;
   assert(test_assoc->table_1->hash_func!=test_assoc->table_2->hash_func);
   assert(test_assoc->bytesize==sizeof(int));


   for(i=0;i<10000;i++)
   {
      test_arr[i]=i;
      test_hashes[i]=test_assoc->table_1->hash_func(&test_arr[i],test_assoc->table_1);
      test_hashes2[i]=test_assoc->table_2->hash_func(&test_arr[i],test_assoc->table_2);
      assert(test_hashes[i]<TESTCAP&&test_hashes[i]>=0);
      assert(test_hashes2[i]<TESTCAP&&test_hashes2[i]>=0);

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
   assert(count<10);
   test_assoc->table_1->capacity=INITSIZE;
   test_assoc->table_2->capacity=INITSIZE;
   assoc_free(test_assoc);



   test_assoc=assoc_init(0);
   test_assoc->table_1->capacity=TESTCAP;
   test_assoc->table_2->capacity=TESTCAP;
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
      test_hashes[i]=test_assoc->table_1->hash_func(word,test_assoc->table_1);
      test_hashes2[i]=test_assoc->table_2->hash_func(word,test_assoc->table_2);
      assert(test_hashes[i]<TESTCAP&&test_hashes[i]>=0);
      assert(test_hashes2[i]<TESTCAP&&test_hashes2[i]>=0);
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
   assert(count<10);

   fclose(fp);
   test_assoc->table_1->capacity=INITSIZE;
   test_assoc->table_2->capacity=INITSIZE;
   assoc_free(test_assoc);

   test_assoc=assoc_init(sizeof(double));
   test_assoc->table_1->capacity=TESTCAP;
   test_assoc->table_2->capacity=TESTCAP;

   for(i=0,d_i=0.0;d_i<10;d_i+=0.01,i++)
   {
      test_hashes[i]=test_assoc->table_1->hash_func(&d_i,test_assoc->table_1);
      test_hashes2[i]=test_assoc->table_2->hash_func(&d_i,test_assoc->table_2);
      assert(test_hashes[i]<TESTCAP&&test_hashes[i]>=0);
      assert(test_hashes2[i]<TESTCAP&&test_hashes2[i]>=0);
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
   assert(count<10);
   test_assoc->table_1->capacity=INITSIZE;
   test_assoc->table_2->capacity=INITSIZE;
   assoc_free(test_assoc);



   test_assoc=assoc_init(sizeof(int));
   i=0;
   j=1;
   test_kv= _init_kv_pair(&i,&j);
   assert(_table_insertion(NULL,test_kv)==NULL);
   assert(_table_insertion(NULL,NULL)==NULL);
   assert(_table_insertion(test_assoc->table_1,NULL)==NULL);
   /*this doesnt increase the size of the whole assoc yet*/
   assert(_table_insertion(test_assoc->table_1,test_kv)==NULL);
   assert(*(int*)(test_assoc->table_1->arr[_first_hash(&i,test_assoc->table_1)]->key)==0);
   j=2;
   test_kv= _init_kv_pair(&i,&j);
   assert(_table_insertion(test_assoc->table_1,test_kv)==NULL);
   assert(*(int*)(test_assoc->table_1->arr[_first_hash(&i,test_assoc->table_1)]->value)==2);
   assoc_free(test_assoc);


   test_assoc=assoc_init(sizeof(int));
   i=1;
   j=18;
   test_kv= _init_kv_pair(&i,&j);
   assert(_table_insertion(test_assoc->table_1,test_kv)==NULL);
   test_kv= _init_kv_pair(&j,&i);

   test_kv=_table_insertion(test_assoc->table_1,test_kv);
   assert(*(int*)test_kv->key==1);

   assert(*(int*)(test_assoc->table_1->arr[_first_hash(&j,test_assoc->table_1)]->value)==1);
   free(test_kv);
   assoc_free(test_assoc);

   test_assoc=assoc_init(0);

   strcpy(word,"hello");
   test_kv= _init_kv_pair(word,&j);
   assert(_table_insertion(test_assoc->table_1,test_kv)==NULL);
   test_kv= _init_kv_pair(word,&i);
   assert(_table_insertion(test_assoc->table_1,test_kv)==NULL);
   assert(*(int*)(test_assoc->table_1->arr[_first_hash(word,test_assoc->table_1)]->value)==1);


   assoc_free(test_assoc);
   /*second hash func - literally the same tests*/

   test_assoc=assoc_init(sizeof(int));
   i=0;
   j=1;
   test_kv= _init_kv_pair(&i,&j);
   assert(_table_insertion(NULL,test_kv)==NULL);
   assert(_table_insertion(NULL,NULL)==NULL);
   assert(_table_insertion(test_assoc->table_2,NULL)==NULL);
   /*this doesnt increase the size of the whole assoc yet*/
   assert(_table_insertion(test_assoc->table_2,test_kv)==NULL);
   assert(*(int*)(test_assoc->table_2->arr[_sec_hash(&i,test_assoc->table_2)]->key)==0);
   j=2;
   test_kv= _init_kv_pair(&i,&j);
   assert(_table_insertion(test_assoc->table_2,test_kv)==NULL);
   assert(*(int*)(test_assoc->table_2->arr[_sec_hash(&i,test_assoc->table_2)]->value)==2);
   assoc_free(test_assoc);


   test_assoc=assoc_init(sizeof(int));
   i=1;
   j=9;
   test_kv= _init_kv_pair(&i,&j);
   assert(_table_insertion(test_assoc->table_2,test_kv)==NULL);
   test_kv= _init_kv_pair(&j,&i);


   test_kv=_table_insertion(test_assoc->table_2,test_kv);
   assert(*(int*)test_kv->key==1);
   assert(*(int*)test_kv->value==9);
   assert(*(int*)(test_assoc->table_2->arr[_sec_hash(&j,test_assoc->table_2)]->value)==1);
   free(test_kv);
   assoc_free(test_assoc);

   test_assoc=assoc_init(0);

   strcpy(word,"hello");
   test_kv= _init_kv_pair(word,&j);
   assert(_table_insertion(test_assoc->table_2,test_kv)==NULL);
   test_kv= _init_kv_pair(word,&i);
   assert(_table_insertion(test_assoc->table_2,test_kv)==NULL);
   assert(*(int*)(test_assoc->table_2->arr[_sec_hash(word,test_assoc->table_2)]->value)==1);
   assoc_free(test_assoc);



   assert(_log_2(16)==4);
   assert(_log_2(17)==4);
   assert(_log_2(64)==6);


   i=0;
   j=17;
   x=34;
   test_assoc=assoc_init(sizeof(int));
   test_kv=_init_kv_pair(&i,&i);
   assert(_outer_insert(test_kv,test_assoc)==NULL);
   test_kv=_init_kv_pair(&j,&j);
   assert(_outer_insert(test_kv,test_assoc)==NULL);
   test_kv=_init_kv_pair(&x,&x);
   test_kv=_outer_insert(test_kv,test_assoc);
   assert(*(int*)test_kv->key==i);
   free(test_kv);
   assert(test_assoc->tot_size==2);
   /*x bounces out j which bounces out i which bounces out x and so on*/
   assert(*(int*)test_assoc->table_1->arr[_first_hash(&x,test_assoc->table_1)]->value==j);
   assert(*(int*)test_assoc->table_2->arr[_sec_hash(&x,test_assoc->table_2)]->value==x);
   assoc_free(test_assoc);


   test_assoc=_assoc_resized(sizeof(int),501);
   assert(test_assoc->table_1->capacity==501);
   assert(test_assoc->table_2->capacity==501);
   for(i=0;i<30;i++)
   {
      test_kv=_init_kv_pair(&test_arr[i],&i);
      assert(!_table_insertion(test_assoc->table_1,test_kv));
   }

   test_assoc2=assoc_init(sizeof(int));
   /*trying to fit too many into a small table*/
   assert(_table_reinsert(test_assoc2,test_assoc->table_1)==false);
   assert(_table_reinsert(NULL,test_assoc->table_1)==false);
   assert(_table_reinsert(test_assoc,NULL)==false);

   /*doesnt matter if theres kv pairs in the table thats too
   small as we never create new kv pairs so wont leak memory
   when freeing the table thats too small*/
   assoc_free(test_assoc);
   _partial_free(test_assoc2);
   test_assoc2=assoc_init(sizeof(int));
   test_assoc=assoc_init(sizeof(int));
   for(i=0;i<4;i++)
   {
      test_kv=_init_kv_pair(&test_arr[i],&i);
      assert(!_table_insertion(test_assoc2->table_1,test_kv));
   }

   assert(_table_reinsert(test_assoc,test_assoc2->table_1)==true);
   assert(test_assoc->tot_size==4);
   assoc_free(test_assoc);
   _partial_free(test_assoc2);



   /*
   test_assoc=assoc_init(sizeof(int));
   for(i=0;i<10;i++)
   {

      test_kv=_init_kv_pair(&test_arr[i],&i);
      assert(_table_insertion(test_assoc->table_1,test_kv));
   }
   i++;
   test_kv=_init_kv_pair(&test_arr[i],&i);
   test_assoc=_resize(test_assoc,test_kv,INITSIZE*SCALEFACTOR);
   for(i=0;i<11;i++)
   {
      lookup goes here
   } need to finish this when i do my look up func*/

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
   n_assoc->table_1=_table_init(n_assoc,&_first_hash);
   n_assoc->table_2=_table_init(n_assoc,&_sec_hash);
   /*set to 0 because insertion function updates this*/
   n_assoc->tot_size=0;
   return n_assoc;
}

table* _table_init(assoc* a,\
                  unsigned int (*hash_f)(void*,const table* ))
{
   table* n_table;
   n_table=(table*) _safe_calloc(1,sizeof(table));
   n_table->arr=(k_v_pair**)_safe_calloc(a->capacity,sizeof(k_v_pair*));
   n_table->hash_func=hash_f;
   n_table->capacity=a->capacity;
   n_table->bytesize=a->bytesize;

   return n_table;
}

void assoc_free(assoc* a)
{
   unsigned int i;
   for(i=0;i<a->capacity;i++)
   {
      if(a->table_1->arr[i])
      {
         free(a->table_1->arr[i]);
      }
      if(a->table_2->arr[i])
      {
         free(a->table_2->arr[i]);
      }
   }
   free(a->table_1->arr);
   free(a->table_2->arr);
   free(a->table_1);
   free(a->table_2);
   free(a);
}

/*when resizing dont want to free all the values*/
void _partial_free(assoc* a)
{
   free(a->table_1->arr);
   free(a->table_2->arr);
   free(a->table_1);
   free(a->table_2);
   free(a);
}

/*checking for NULLS in outer func*/
/*http://www.cse.yorku.ca/~oz/hash.html#djb2 sdbm*/
unsigned int _first_hash(void* key,const table* a)
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

         hash+= (*str)+(hash<<6) +(hash<<16)-hash;
      }
      return hash% a->capacity;
   }
   else
   {
      i=0;
      while(str[i])
      {

         hash+= str[i]+(hash<<6) +(hash<<16)-hash;
         i++;
      }

      return hash% a->capacity;
   }


}

/*your version of Bernstein modified a little*/
unsigned int _sec_hash(void* key,const table* a)
{
   unsigned long hash;
   unsigned int i;
   char* str;
   str= (char*) key;
   hash=5381;
   if(a->bytesize!=0)
   {
      for(i=0;i< a->bytesize;i++,str++)
      {
         hash+= 33*hash ^ (*str);
      }
      return hash% a->capacity;
   }
   else
   {
      i=0;
      while(str[i])
      {
         hash+= 33*hash ^ str[i];
         i++;
      }
      return hash % a->capacity;
   }

}


/*gunna have a table insert func that returns NULL if it
places into an empty space otherwise will return the
thing it kicked out*/

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




/*from previous exercise*/
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
      else
      {
         if(memcmp(key1,key2,bytesize)==0)
         {
            return true;
         }
         return false;
      }
   }
   return false;
}



/*gunna have a table insert func that returns NULL if it
places into an empty space otherwise will return the
thing it kicked out
will expect a new kv pair each time - cant insert an existing
kv pair thats already in - as in that specific entity -not another kv pair
with the same key and value
*/
k_v_pair* _table_insertion(table* a, k_v_pair* kv)
{
   k_v_pair* replaced;
   int insertion_point;
   if(kv&&a)
   {
      insertion_point= a->hash_func(kv->key, a);


      if(a->arr[insertion_point]==NULL)
      {
         a->arr[insertion_point]=kv;
         return NULL;
      }
      else
      {
         if(_same_key(a->arr[insertion_point]->key,kv->key,a->bytesize))
         {
            free(a->arr[insertion_point]);
            a->arr[insertion_point]=kv;
            return NULL;
         }
         else
         {

            replaced=a->arr[insertion_point];
            a->arr[insertion_point]=kv;
            return replaced;
         }
      }
   }
   return NULL;
}



/*returns NULL if we dont need to resize
otherwise returns the pair that was bumped out last*/
k_v_pair* _outer_insert(k_v_pair* kv,assoc* a)
{
   k_v_pair* to_place;
   int count;
   int max_bounce;

   if(kv && a)
   {
      max_bounce = _log_2(a->capacity);
      to_place=_table_insertion(a->table_1,kv);
      /*if val was bumped out*/
      if(to_place)
      {
         count=1;
         while(count<max_bounce)
         {
            /*keep bumping things out*/
            to_place=_table_insertion(a->table_2,to_place);
            count++;
            if(!to_place)
            {
               a->tot_size++;
               return NULL;
            }
            to_place=_table_insertion(a->table_1,to_place);
            count++;
            if(!to_place)
            {
               a->tot_size++;
               return NULL;
            }
         }
         /*if we need to resize to fit everything*/
         return to_place;
      }
   }
   a->tot_size++;
   return NULL;
}


/*test all below*/
assoc* _assoc_resized(int keysize,int n_cap)
{
   assoc* n_assoc;
   assert(keysize>=0);
   n_assoc=_safe_calloc(1,sizeof(assoc));
   n_assoc->capacity=n_cap;
   n_assoc->bytesize=keysize;
   n_assoc->table_1=_table_init(n_assoc,&_first_hash);
   n_assoc->table_2=_table_init(n_assoc,&_sec_hash);
   /*set to 0 because insertion function updates this*/
   n_assoc->tot_size=0;
   return n_assoc;
}

assoc* _resize(assoc* a, k_v_pair* leftover,int n_size)
{
   assoc* n_ass;
   if(a)
   {
      n_ass=_assoc_resized(a->bytesize,n_size);

      if(!_table_reinsert(n_ass, a->table_1))
      {
         _partial_free(n_ass);
         return _resize(a,leftover,n_size*SCALEFACTOR);
      }
      if(!_table_reinsert(n_ass, a->table_2))
      {
         _partial_free(n_ass);
         return _resize(a,leftover,n_size*SCALEFACTOR);
      }

      _partial_free(a);
      return n_ass;

   }

   return NULL;
}

bool _table_reinsert(assoc* n_assoc, table* old_table)
{
   unsigned int i;
   if(n_assoc && old_table)
   {
      for(i=0;i<old_table->capacity;i++)
      {
         /*if data present*/
         if(old_table->arr[i])
         {
            if(_outer_insert(old_table->arr[i],n_assoc))
            {
               /*not enough space in new hash tables*/
               return false;
            }
         }
      }
      return true;
   }
   return false;
}

/*need to test*/
void assoc_insert(assoc** a, void* key, void* data)
{
   assoc* a_ref;
   k_v_pair* kv;
   a_ref = *a;
   if(a_ref!=NULL)
   {
      if(key)
      {
         kv=_init_kv_pair(key,data);
         kv=_outer_insert(kv,a_ref);
         while(kv)
         {
            a_ref=_resize(a_Ref,kv,a_ref->capacity*2);
            kv=_outer_insert(kv,a_ref);
         }

      }
   }
   *a = a_ref;
}


void* assoc_lookup(assoc* a, void* key)
{

   k_v_pair* answer;
   /*check if either are null*/
   if(a && key)
   {
      answer=_table_lookup(key,a->table_1);
      if(answer)
      {
         return answer;
      }
      answer=_table_lookup(key,a->table_2);
      if(answer)
      {
         return answer;
      }
   }
   return NULL;
}


void* _table_lookup(void* key,table* to_look)
{
   unsigned int hash;
   hash=to_look->hash_fuc(key,to_look);
   return to_look->arr[hash];
}


int _log_2(int num)
{
   return log10(num)/log10(2);
}
