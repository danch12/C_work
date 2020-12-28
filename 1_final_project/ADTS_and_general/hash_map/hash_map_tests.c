#include "hash_funcs.h"

#define WORDS 370119
#define WORDLEN 50

/*assigns a int to position member for easy
testing without having to worry about properly
creating function holder */



/* Make a copy, reversed */
char* strduprev(char* str);

int main(void)
{
   static char strs[WORDS][50]={{0}};
   static int i[WORDS];
   char target[50];
   FILE *fp;
   char* tstr;

   unsigned int lngst;
   unsigned int j;
   assoc* a;
   void* p;

   a = assoc_init();
   fp = safe_fopen("eng_370k_shuffle.txt");
   for(j=0; j<WORDS; j++){
     assert(assoc_count(a)==j);
      i[j] = j;
     if(fscanf(fp, "%s", strs[j])!=1)
     { 
        fprintf(stderr,"Failed to scan in a word?");
        exit(EXIT_FAILURE);
     }
     assoc_insert(&a, strs[j], &i[j]);
   }
   fclose(fp);

   /*
     What's the longest word that is still spelled
     correctly when reversed, but is not a palindrome ?
   */
   lngst = 0;
   for(j=0; j<WORDS; j++)
   {
     /* Longest */
     if(strlen(strs[j]) > lngst)
     {
        tstr = strduprev(strs[j]);
        /* Not a palindrome */
        if(strcmp(tstr, strs[j]))
        {
           /* Spelled correctly */
           if((p = assoc_lookup(a, tstr))!=NULL)
           {
              lngst = strlen(tstr);
              sprintf(target,"%s", tstr);
           }
        }
        free(tstr);
     }
   }
   assert(strcmp("redrawer",target)==0);
   assert(lngst==8);
   assoc_free(a);
   return 0;
}





/* Make a copy, reversed */
char* strduprev(char* str)
{
   int i, j;
   char* t;
   j = strlen(str);
   t = safe_calloc(j+1, 1); /* Add null char */
   strcpy(t, str);
   for(i=0, j--; i<j; i++,j--){
      /* Swap using bit-twiddling */
      t[i] ^= t[j];
      t[j] ^= t[i];
      t[i] ^= t[j];
   }
   return t;
}
