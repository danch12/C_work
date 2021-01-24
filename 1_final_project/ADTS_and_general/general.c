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

/*adapted version of yours*/
void* safe_calloc_2d(int height,int width ,size_t size)
{
   int i;
   void** ptr;
   ptr = calloc(height, sizeof(void*));
   if(ptr==NULL)
   {
      fprintf(stderr,"Not enough space\n");
   	exit(EXIT_FAILURE);
   }
   for(i=0;i<height;i++)
   {
      ptr[i] = calloc(width, size);
      if(ptr==NULL)
      {
         fprintf(stderr,"Not enough space\n");
      	exit(EXIT_FAILURE);
      }
   }
   return ptr;
}

void free_2d(int** ptr,int height)
{
   int i;
   for(i=0;i<height;i++)
   {
      free(ptr[i]);
   }
   free(ptr);
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


void safe_fgets(char* str,int n, FILE *stream)
{
   if(fgets(str,n,stream)==NULL)
   {
      fprintf(stderr,"failed to read\n");
      exit(EXIT_FAILURE);
   }
}
