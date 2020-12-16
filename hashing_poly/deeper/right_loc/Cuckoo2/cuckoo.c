#include "specific.h"
#include "../assoc.h"
#define TESTCAP 15053

/*test for this just in case*/
typedef struct test_struct_str
{
   char word[50];
   int number;
}test_struct_str;


table* _table_init(assoc* a,\
                  unsigned int (*hash_f)(void*,const table* ));
/*return void then cast when appropriate*/
void* _safe_calloc(size_t nitems, size_t size);
int _wrap_around(int max_num,int position);
/*http://www.cse.yorku.ca/~oz/hash.html#djb2 sdbm*/
unsigned int _first_hash(void* key,const table* a);
/*your version of Bernstein pretty much*/
unsigned int _sec_hash(void* key,const table* a);


k_v_pair* _init_kv_pair(void* key, void* data);
/*returns true if keys are same*/
bool _same_key(const void* key1,const void* key2,int bytesize);

/*returns null if successful otherwise returns kv pair it kicked out
this basically tells us we need to resize*/
k_v_pair* _table_insertion(table* a, k_v_pair* kv);
/*different to table_insertion - tries to put in whole table
and returns true if successful*/
bool _table_reinsert(assoc* n_assoc, table* old_table);

assoc* _bigger_array(assoc* a,int n_size);
assoc* _assoc_resized(assoc* old_assoc,int n_cap);
/*this func keeps resizing until everything can fit- only resizes to
prime numbers */
assoc* _resize(assoc* a, k_v_pair* leftover,int n_size);
bool _table_reinsert(assoc* n_assoc, table* old_table);
/*returns NULL if we dont need to resize
otherwise returns the pair that was bumped out last*/
k_v_pair* _outer_insert(k_v_pair* kv,assoc* a);
/*when resizing dont want to free all the values*/
void _partial_free(assoc* a);

void* _table_lookup(void* key,table* to_look);

int _sieve_of_e_helper(int new_cap_target);
int _log_2(int num);
void test(void);
/*
int main(void)
{
   test();
   return 0;
}*/

void test(void)
{
   assoc* test_assoc,*test_assoc2;
   int i,j,count,x;
   int test_arr[10000];
   int test_hashes[10000];
   int test_hashes2[10000];
   float test_floats[1000];
   test_struct_str tss_arr[100];

   char word[50];
   float test_float;
   double d_i;
   k_v_pair* test_kv;
   FILE *fp;


   test_assoc=assoc_init(sizeof(int));
   test_assoc->table_arr[0]->capacity=TESTCAP;
   test_assoc->table_arr[1]->capacity=TESTCAP;
   assert(test_assoc->table_arr[0]->hash_func!=test_assoc->table_arr[1]->hash_func);
   assert(test_assoc->bytesize==sizeof(int));


   for(i=0;i<10000;i++)
   {
      test_arr[i]=i;
      test_hashes[i]=test_assoc->table_arr[0]->hash_func(&test_arr[i],test_assoc->table_arr[0]);
      test_hashes2[i]=test_assoc->table_arr[1]->hash_func(&test_arr[i],test_assoc->table_arr[1]);
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
   test_assoc->table_arr[0]->capacity=INITSIZE;
   test_assoc->table_arr[1]->capacity=INITSIZE;
   assoc_free(test_assoc);



   test_assoc=assoc_init(0);
   test_assoc->table_arr[0]->capacity=TESTCAP;
   test_assoc->table_arr[1]->capacity=TESTCAP;
   fp = fopen("../../Data/Words/eng_370k_shuffle.txt", "rt");
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
      test_hashes[i]=test_assoc->table_arr[0]->hash_func(word,test_assoc->table_arr[0]);
      test_hashes2[i]=test_assoc->table_arr[1]->hash_func(word,test_assoc->table_arr[1]);
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
   test_assoc->table_arr[0]->capacity=INITSIZE;
   test_assoc->table_arr[1]->capacity=INITSIZE;
   assoc_free(test_assoc);

   test_assoc=assoc_init(sizeof(double));
   test_assoc->table_arr[0]->capacity=TESTCAP;
   test_assoc->table_arr[1]->capacity=TESTCAP;

   for(i=0,d_i=0.0;d_i<10;d_i+=0.01,i++)
   {
      test_hashes[i]=test_assoc->table_arr[0]->hash_func(&d_i,test_assoc->table_arr[0]);
      test_hashes2[i]=test_assoc->table_arr[1]->hash_func(&d_i,test_assoc->table_arr[1]);
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
   test_assoc->table_arr[0]->capacity=INITSIZE;
   test_assoc->table_arr[1]->capacity=INITSIZE;
   assoc_free(test_assoc);



   test_assoc=assoc_init(sizeof(int));
   i=0;
   j=1;
   test_kv= _init_kv_pair(&i,&j);
   assert(_table_insertion(NULL,test_kv)==NULL);
   assert(_table_insertion(NULL,NULL)==NULL);
   assert(_table_insertion(test_assoc->table_arr[0],NULL)==NULL);
   /*this doesnt increase the size of the whole assoc yet*/
   assert(_table_insertion(test_assoc->table_arr[0],test_kv)==NULL);
   assert(*(int*)(test_assoc->table_arr[0]->arr[_first_hash(&i,test_assoc->table_arr[0])]->key)==0);
   j=2;
   test_kv= _init_kv_pair(&i,&j);
   assert(_table_insertion(test_assoc->table_arr[0],test_kv)==NULL);
   assert(*(int*)(test_assoc->table_arr[0]->arr[_first_hash(&i,test_assoc->table_arr[0])]->value)==2);
   assoc_free(test_assoc);


   test_assoc=assoc_init(sizeof(int));
   i=1;
   j=18;
   test_kv= _init_kv_pair(&i,&j);
   assert(_table_insertion(test_assoc->table_arr[0],test_kv)==NULL);
   test_kv= _init_kv_pair(&j,&i);

   test_kv=_table_insertion(test_assoc->table_arr[0],test_kv);
   assert(*(int*)test_kv->key==1);

   assert(*(int*)(test_assoc->table_arr[0]->arr[_first_hash(&j,test_assoc->table_arr[0])]->value)==1);
   free(test_kv);
   assoc_free(test_assoc);

   test_assoc=assoc_init(0);

   strcpy(word,"hello");
   test_kv= _init_kv_pair(word,&j);
   assert(_table_insertion(test_assoc->table_arr[0],test_kv)==NULL);
   test_kv= _init_kv_pair(word,&i);
   assert(_table_insertion(test_assoc->table_arr[0],test_kv)==NULL);
   assert(*(int*)(test_assoc->table_arr[0]->arr[_first_hash(word,test_assoc->table_arr[0])]->value)==1);


   assoc_free(test_assoc);
   /*second hash func - literally the same tests*/

   test_assoc=assoc_init(sizeof(int));
   i=0;
   j=1;
   test_kv= _init_kv_pair(&i,&j);
   assert(_table_insertion(NULL,test_kv)==NULL);
   assert(_table_insertion(NULL,NULL)==NULL);
   assert(_table_insertion(test_assoc->table_arr[1],NULL)==NULL);
   /*this doesnt increase the size of the whole assoc yet*/
   assert(_table_insertion(test_assoc->table_arr[1],test_kv)==NULL);
   assert(*(int*)(test_assoc->table_arr[1]->arr[_sec_hash(&i,test_assoc->table_arr[1])]->key)==0);
   j=2;
   test_kv= _init_kv_pair(&i,&j);
   assert(_table_insertion(test_assoc->table_arr[1],test_kv)==NULL);
   assert(*(int*)(test_assoc->table_arr[1]->arr[_sec_hash(&i,test_assoc->table_arr[1])]->value)==2);
   assoc_free(test_assoc);


   test_assoc=assoc_init(sizeof(int));
   i=1;
   j=9;
   test_kv= _init_kv_pair(&i,&j);
   assert(_table_insertion(test_assoc->table_arr[1],test_kv)==NULL);
   test_kv= _init_kv_pair(&j,&i);


   test_kv=_table_insertion(test_assoc->table_arr[1],test_kv);
   assert(*(int*)test_kv->key==1);
   assert(*(int*)test_kv->value==9);
   assert(*(int*)(test_assoc->table_arr[1]->arr[_sec_hash(&j,test_assoc->table_arr[1])]->value)==1);
   free(test_kv);
   assoc_free(test_assoc);

   test_assoc=assoc_init(0);

   strcpy(word,"hello");
   test_kv= _init_kv_pair(word,&j);
   assert(_table_insertion(test_assoc->table_arr[1],test_kv)==NULL);
   test_kv= _init_kv_pair(word,&i);
   assert(_table_insertion(test_assoc->table_arr[1],test_kv)==NULL);
   assert(*(int*)(test_assoc->table_arr[1]->arr[_sec_hash(word,test_assoc->table_arr[1])]->value)==1);
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

   assert(*(int*)test_kv->key==j);
   free(test_kv);

   /*x bounces out j which bounces out i which bounces out x and so on*/
   assert(*(int*)test_assoc->table_arr[0]->arr[_first_hash(&x,test_assoc->table_arr[0])]->value==i);
   assert(*(int*)test_assoc->table_arr[1]->arr[_sec_hash(&x,test_assoc->table_arr[1])]->value==x);
   assoc_free(test_assoc);

   test_assoc2=assoc_init(sizeof(int));
   test_assoc=_assoc_resized(test_assoc2,501);
   assert(test_assoc->table_arr[0]->capacity==501);
   assert(test_assoc->table_arr[1]->capacity==501);
   for(i=0;i<30;i++)
   {
      test_kv=_init_kv_pair(&test_arr[i],&i);
      assert(!_table_insertion(test_assoc->table_arr[0],test_kv));
   }

   /*trying to fit too many into a small table*/
   assert(_table_reinsert(test_assoc2,test_assoc->table_arr[0])==false);
   assert(_table_reinsert(NULL,test_assoc->table_arr[0])==false);
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
      assert(!_table_insertion(test_assoc2->table_arr[0],test_kv));
   }

   assert(_table_reinsert(test_assoc,test_assoc2->table_arr[0])==true);

   assoc_free(test_assoc);
   _partial_free(test_assoc2);


   test_assoc=assoc_init(sizeof(int));
   for(i=0;i<4;i++)
   {

      test_kv=_init_kv_pair(&test_arr[i],&i);
      assert(!_table_insertion(test_assoc->table_arr[0],test_kv));
   }
   i++;
   test_kv=_init_kv_pair(&test_arr[i],&i);
   test_assoc=_resize(test_assoc,test_kv,INITSIZE*SCALEFACTOR);
   assert(assoc_count(test_assoc)==5);
   /*things still can be found after resizing*/
   for(i=0;i<4;i++)
   {

      assert(assoc_lookup(test_assoc, &test_arr[i]));
   }

   for(i=0;i<(int)test_assoc->capacity;i++)
   {
      if(test_assoc->table_arr[0]->arr[i])
      {
         assert(_table_lookup(test_assoc->table_arr[0]->arr[i]->key,test_assoc->table_arr[0]));
      }
   }
   for(i=6;i<100;i++)
   {
      assert(!assoc_lookup(test_assoc, &test_arr[i]));
   }
   assoc_free(test_assoc);

   test_assoc=assoc_init(sizeof(int));
   for(i=0;i<10000;i++)
   {
      assoc_insert(&test_assoc,&test_arr[i],&i);
   }
   assert(assoc_count(test_assoc)==10000);
   assert(test_assoc->capacity>INITSIZE);
   for(i=0;i<10000;i++)
   {
      assert(assoc_lookup(test_assoc,&test_arr[i]));
   }
   count=0;
   for(i=0;i<(int)test_assoc->capacity;i++)
   {
      if(test_assoc->table_arr[0]->arr[i])
      {
         count++;
      }
      if(test_assoc->table_arr[1]->arr[i])
      {
         count++;
      }
   }
   assert(assoc_count(test_assoc)==(unsigned int)count);
   assoc_free(test_assoc);

   /*now going to just test couple different types to see if
   all of them work*/
   test_assoc=assoc_init(sizeof(test_struct_str));
   fp = fopen("../../Data/Words/eng_370k_shuffle.txt", "rt");
   if(fp==NULL)
   {
      fprintf(stderr,"file not there?\n");
      exit(EXIT_FAILURE);
   }
   for(i=0;i<100;i++)
   {
      if(fscanf(fp, "%s", word)!=1)
      {
         fprintf(stderr,"failed scan of word\n");
         exit(EXIT_FAILURE);
      }
      strcpy(tss_arr[i].word, word);
      tss_arr[i].number=i;
      assert(assoc_count(test_assoc)==(unsigned int)i);
      assoc_insert(&test_assoc,&tss_arr[i],&i);
   }
   for(i=0;i<100;i++)
   {

      test_kv=assoc_lookup(test_assoc,&tss_arr[i]);
      assert(*(int*)test_kv==i);
   }
   assoc_free(test_assoc);
   fclose(fp);

   test_assoc=assoc_init(sizeof(float));
   for(i=0,test_float=0;test_float<10;test_float+=0.5,i++)
   {
      test_floats[i]=test_float;

      assert(assoc_count(test_assoc)==(unsigned int)i);
      assoc_insert(&test_assoc,&test_floats[i],&test_float);
   }

   for(i=0,test_float=0;test_float<10;test_float+=0.5,i++)
   {

      test_kv=assoc_lookup(test_assoc,&test_floats[i]);
      assert(*(float*)test_kv-test_float<0.0001);
   }
   assoc_insert(&test_assoc,NULL,NULL);
   assert(assoc_count(test_assoc)==(unsigned int)i);
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
   int i;
   static bool tested=false;
   if(!tested)
   {
      tested=true;
      test();
   }
   assert(keysize>=0);
   n_assoc=_safe_calloc(1,sizeof(assoc));
   n_assoc->capacity=INITSIZE;
   n_assoc->bytesize=keysize;
   for(i=0;i<NUMTABLES;i++)
   {
      if(i%NUMTABLES==0)
      {
         n_assoc->table_arr[i]=_table_init(n_assoc,&_first_hash);
      }
      else
      {
         n_assoc->table_arr[i]=_table_init(n_assoc,&_sec_hash);
      }
   }
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
   n_table->size=0;
   return n_table;
}

void assoc_free(assoc* a)
{
   unsigned int i,j;
   for(j=0;j<NUMTABLES;j++)
   {
      for(i=0;i<a->capacity;i++)
      {
         if(a->table_arr[j]->arr[i])
         {
            free(a->table_arr[j]->arr[i]);
         }

      }
      free(a->table_arr[j]->arr);
      free(a->table_arr[j]);
   }
   free(a);
}

/*when resizing dont want to free all the values*/
void _partial_free(assoc* a)
{
   int i;
   for(i=0;i<NUMTABLES;i++)
   {
      free(a->table_arr[i]->arr);
      free(a->table_arr[i]);
   }
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

/*your version of Bernstein modified a little*/
unsigned int _sec_hash(void* key,const table* a)
{
   unsigned long hash;
   unsigned int i;
   char* str;
   str= (char*) key;
   hash=BERSTEIN_START;
   if(a->bytesize!=0)
   {
      for(i=0;i< a->bytesize;i++,str++)
      {
         hash+= BERN_MULT*hash ^ (*str);
      }
      return hash% a->capacity;
   }
   else
   {
      i=0;
      while(str[i])
      {
         hash+= BERN_MULT*hash ^ str[i];
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

         /*only increasing size when your putting a kv pair into a empty space*/
         a->size++;
         a->arr[insertion_point]=kv;
         return NULL;
      }
      else
      {
         if(_same_key(a->arr[insertion_point]->key,kv->key,a->bytesize))
         {
            /*if it isnt the same kv pair*/
            if(a->arr[insertion_point]!=kv)
            {
               free(a->arr[insertion_point]);
               a->arr[insertion_point]=kv;
            }
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
   to_place=kv;
   if(kv && a)
   {
      max_bounce = _log_2(a->capacity);
      count=0;

      while(count<max_bounce)
      {
         /*count%NUMTABLES will be within bounds*/
         to_place=_table_insertion(a->table_arr[count%NUMTABLES],to_place);
         /*if val was bumped out*/
         if(!to_place)
         {
            return NULL;
         }
         count++;
      }
      /*if we need to resize to fit everything*/
      return to_place;

   }
   return NULL;

}


/*test all below*/
assoc* _assoc_resized(assoc* old_assoc,int n_cap)
{
   assoc* n_assoc;
   int i;
   n_assoc=_safe_calloc(1,sizeof(assoc));
   n_assoc->capacity=n_cap;
   n_assoc->bytesize=old_assoc->bytesize;
   for(i=0;i<NUMTABLES;i++)
   {
      if(i%NUMTABLES==0)
      {
         n_assoc->table_arr[i]=_table_init(n_assoc,&_first_hash);
      }
      else
      {
         n_assoc->table_arr[i]=_table_init(n_assoc,&_sec_hash);
      }
   }
   return n_assoc;
}

assoc* _resize(assoc* a, k_v_pair* leftover,int n_size)
{
   assoc* n_ass;
   int i;
   if(a)
   {
      n_ass=_bigger_array(a,n_size);
      /*loop through the tables until everything inserted*/
      for(i=0;i<NUMTABLES;i++)
      {

         if(!_table_reinsert(n_ass,a->table_arr[i]))
         {
            _partial_free(n_ass);
            return _resize(a,leftover,n_size*SCALEFACTOR);
         }
      }
      /*add in the leftover*/
      if(_outer_insert(leftover,n_ass))
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
            a_ref=_resize(a_ref,kv,a_ref->capacity*2);
            kv=_outer_insert(kv,a_ref);
         }

      }
   }
   *a = a_ref;
}


void* assoc_lookup(assoc* a, void* key)
{

   k_v_pair* answer;
   int i;
   /*check if either are null*/
   if(a && key)
   {
      for(i=0;i<NUMTABLES;i++)
      {
         answer=_table_lookup(key,a->table_arr[i]);
         if(answer)
         {
            if(_same_key(answer->key,key,a->bytesize))
            {
               return answer->value;
            }
         }
      }
   }
   return NULL;
}


void* _table_lookup(void* key,table* to_look)
{
   unsigned int hash;
   hash=to_look->hash_func(key,to_look);
   return to_look->arr[hash];
}


int _log_2(int num)
{
   /*to get log 2 of a number you do log 10 / log 10 of 2
   https://www.purplemath.com/modules/logrules5.htm*/
   return log10(num)/log10(2);
}


unsigned int assoc_count(assoc* a)
{
   int i;
   unsigned int size;
   size=0;
   if(a)
   {
      for(i=0;i<NUMTABLES;i++)
      {
         size+=a->table_arr[i]->size;
      }
   }
   return size;
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


assoc* _bigger_array(assoc* a,int n_size)
{
   int bigger_table;
   int prime_limit;
   assoc* a_n;
   if(a)
   {
      bigger_table=a->capacity;
      prime_limit=n_size;
      /*keep going until find a bigger prime by increasing
      the highest number we look up to*/
      while(bigger_table<n_size)
      {
         bigger_table=_sieve_of_e_helper(prime_limit);
         prime_limit=(double)prime_limit*PRIMESCALE;
      }
      a_n=_assoc_resized(a,bigger_table);
      return a_n;
   }
   return NULL;
}
