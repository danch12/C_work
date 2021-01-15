#include "../src/unity.h"
#include "../ADTS_and_general/hash_map/hash_funcs.h"
#define TESTCAP 15053

void setUp(void)
{

}

void tearDown(void)
{

}

void test_hash(void)
{
   int test_hashes[10000]={0};

   int histogram_hashes[TESTCAP]={0};
   FILE* fp;
   char word[50];
   assoc* test_assoc;
   int i;
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
      TEST_ASSERT_TRUE((test_hashes[i]<TESTCAP)&&(test_hashes[i]>=0));
   }
   /*testing spread of hash distribution roughly - should use
   a chi squared test or something similar but this will do*/
   for(i=0;i<10000;i++)
   {
      TEST_ASSERT_TRUE(histogram_hashes[i]<50);
   }
   fclose(fp);
   test_assoc->capacity=INIT_SIZE_HASH;
   assoc_free(test_assoc);
}


void test_byte_convert(void)
{
   char word[50];
   assoc* test_assoc;
   test_assoc=assoc_init();
   TEST_ASSERT_TRUE(_byte_convert(NULL)==0);
   strcpy(word,"hello");

   TEST_ASSERT_TRUE(_byte_convert(word)==532);
   strcpy(word,"");

   /*things like this are why i put the safeguard in
   my double hash func*/
   TEST_ASSERT_TRUE(_byte_convert(word)==0);
   TEST_ASSERT_TRUE(_sec_hash(word,test_assoc)!=0);
   strcpy(word," ");
   TEST_ASSERT_TRUE(_byte_convert(word)==32);
   assoc_free(test_assoc);
}

void test_wrap_around(void)
{
   int i,j;
   TEST_ASSERT_TRUE(_wrap_around(10,11)==1);
   TEST_ASSERT_TRUE(_wrap_around(10,10)==0);

   /*should just cycle from 0 to 9 over and over*/
   j=0;
   for(i=20;i>0;i--)
   {
      TEST_ASSERT_TRUE(_wrap_around(10,i)==j);
      j--;
      if(j<0)
      {
         j=9;
      }
   }
   j=0;
   for(i=0;i<-20;i++)
   {
      TEST_ASSERT_TRUE(_wrap_around(10,i)==j);
      j++;
      if(j>9)
      {
         j=0;
      }
   }
}



void test_sec_hash(void)
{
   int i,j,count;
   assoc* test_assoc;
   int test_hashes[10000]={0};
   int test_hashes2[10000]={0};
   FILE* fp;
   char word[50];

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
      TEST_ASSERT_TRUE(test_hashes[i]<TESTCAP&&test_hashes[i]>=0);
      TEST_ASSERT_TRUE(test_hashes2[i]<TESTCAP&&test_hashes2[i]>0);

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
   TEST_ASSERT_TRUE(count<300);
   fclose(fp);
   test_assoc->capacity=INIT_SIZE_HASH;
   assoc_free(test_assoc);
}

void test_insertion_helper(void)
{

   k_v_pair* test_kv;
   int i,j,count;
   assoc* test_assoc;

   char word[50];
   char word2[50];
   i=10;
   test_assoc=assoc_init();
   test_kv=_init_kv_pair(word,&i);
   TEST_ASSERT_TRUE(_insertion_helper(test_assoc,test_kv,0));
   TEST_ASSERT_TRUE(strcmp(word,test_assoc->arr[0]->key)==0);
   TEST_ASSERT_TRUE(*(int*)test_assoc->arr[0]->value==10);


   j=20;
   test_kv=_init_kv_pair(word2,&j);
   TEST_ASSERT_TRUE(_double_hash(7,0,test_assoc,test_kv));
   TEST_ASSERT_TRUE(*(int*)test_assoc->arr[7]->value==20);


   count=30;
   test_kv=_init_kv_pair(" ",&count);
   TEST_ASSERT_TRUE(_double_hash(7,0,test_assoc,test_kv));
   TEST_ASSERT_TRUE(*(int*)test_assoc->arr[14]->value==30);

   assoc_free(test_assoc);

   TEST_ASSERT_TRUE(_same_key(NULL,NULL)==false);
   TEST_ASSERT_TRUE(_same_key(NULL,word)==false);
   TEST_ASSERT_TRUE(_same_key(word,NULL)==false);

   strcpy(word,"dan");
   strcpy(word2,"dna");
   TEST_ASSERT_TRUE(_same_key(word,word2)==false);
   TEST_ASSERT_TRUE(_same_key(word,word)==true);

   test_assoc=assoc_init();
   test_kv=_init_kv_pair(word,NULL);
   TEST_ASSERT_TRUE(_insertion_helper(test_assoc,test_kv,0));
   TEST_ASSERT_TRUE(strcmp(test_assoc->arr[0]->key,word)==0);
   strcpy(word2,"938Neilliscool\n");
   test_kv=_init_kv_pair(word2,NULL);
   TEST_ASSERT_TRUE(!_insertion_helper(test_assoc,test_kv,0));
   free(test_kv);
   TEST_ASSERT_TRUE(!_insertion_helper(test_assoc,NULL,0));
   TEST_ASSERT_TRUE(!_insertion_helper(test_assoc,NULL,0));
   assoc_free(test_assoc);
}

void test_same_key(void)
{
   char word[50];
   char word2[50];
   TEST_ASSERT_TRUE(_same_key(NULL,NULL)==false);
   TEST_ASSERT_TRUE(_same_key(NULL,word)==false);
   TEST_ASSERT_TRUE(_same_key(word,NULL)==false);

   strcpy(word,"dan");
   strcpy(word2,"dna");
   TEST_ASSERT_TRUE(_same_key(word,word2)==false);
   TEST_ASSERT_TRUE(_same_key(word,word)==true);
}
void test_bigger_arrays(void)
{
   assoc* test_assoc,*test_assoc2;
   char word_arr[500][50]={{'\0'}};
   int test_hashes[10000]={0};
   char word[50];
   k_v_pair* test_kv;
   int i;
   /*gets closest lower prime */
   TEST_ASSERT_TRUE(_sieve_of_e_helper(1049)==1039);
   TEST_ASSERT_TRUE(_sieve_of_e_helper(2677)==2671);
   TEST_ASSERT_TRUE(_sieve_of_e_helper(0)==NOPRIME);
   test_assoc=assoc_init();
   test_assoc2=_bigger_array(test_assoc);
   TEST_ASSERT_TRUE(test_assoc2->capacity==37);
   TEST_ASSERT_TRUE(test_assoc2->lower_prime==INIT_SIZE_HASH);
   assoc_free(test_assoc2);

   test_assoc->capacity=TESTCAP;
   test_assoc2=_bigger_array(test_assoc);
   TEST_ASSERT_TRUE(test_assoc2->capacity==36109);
   TEST_ASSERT_TRUE(test_assoc2->lower_prime==TESTCAP);
   test_assoc->capacity=INIT_SIZE_HASH;
   assoc_free(test_assoc);
   assoc_free(test_assoc2);
   TEST_ASSERT_TRUE(!_bigger_array(NULL));

   strcpy(word,"");
   test_assoc=assoc_init();

   for(i=0;i<15;i++)
   {
      test_hashes[i]=i;
      strcpy(word_arr[i],word);
      test_kv=_init_kv_pair(word_arr[i],&test_hashes[i]);
      TEST_ASSERT_TRUE(_insertion(test_assoc,test_kv));
      strcat(word,"a");

   }

   test_assoc=_resize(test_assoc);
   TEST_ASSERT_TRUE(test_assoc->size==15);
   TEST_ASSERT_TRUE(test_assoc->capacity==37);
   /*check that after resizing everything still there*/
   strcpy(word,"");
   for(i=0;i<15;i++)
   {
      TEST_ASSERT_TRUE(assoc_lookup(test_assoc,word));
      strcat(word,"a");
   }
   assoc_free(test_assoc);
}


void test_insert_lookup(void)
{
   assoc* test_assoc;
   char word_arr[500][50]={{'\0'}};
   int test_hashes[10000]={0};
   char word[50];
   int i;
   FILE* fp;
   test_assoc=assoc_init();
   TEST_ASSERT_TRUE(assoc_lookup(NULL,word)==NULL);
   TEST_ASSERT_TRUE(assoc_lookup(NULL,NULL)==NULL);
   TEST_ASSERT_TRUE(assoc_lookup(test_assoc,NULL)==NULL);
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

      TEST_ASSERT_TRUE(assoc_count(test_assoc)==(unsigned int)i);
      test_hashes[i]=i;
      assoc_insert(&test_assoc, word_arr[i],&test_hashes[i]);
      TEST_ASSERT_TRUE(*(int*)assoc_lookup(test_assoc,word_arr[i])==i);


   }
   fclose(fp);
   assoc_free(test_assoc);
}

int main(void)
{
   UNITY_BEGIN();
   RUN_TEST(test_hash);
   RUN_TEST(test_byte_convert);
   RUN_TEST(test_wrap_around);
   RUN_TEST(test_sec_hash);
   RUN_TEST(test_insertion_helper);
   RUN_TEST(test_same_key);
   RUN_TEST(test_bigger_arrays);
   RUN_TEST(test_insert_lookup);
   return UNITY_END();
}
