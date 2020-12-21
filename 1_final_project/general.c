#include "general.h"

/*return void then cast when appropriate*/
void* safe_calloc(size_t nitems, size_t size)
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



FILE* safe_fopen(char* filename)
{
   FILE *fp;

   fp= fopen(filename,"r");
   if(fp==NULL)
   {
      fprintf(stderr,"error while opening file\n");
      exit(EXIT_FAILURE);
   }
   return fp;
}
