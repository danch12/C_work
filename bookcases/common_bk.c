#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include "common_bk.h"




/*return void then cast when appropriate*/
void* safe_calloc(size_t nitems, size_t size)
{
  void* ptr;
  ptr = calloc(nitems, size);
  if(ptr==NULL)
  {
     fprintf(stderr,"No Space available\n");
  	  exit(EXIT_FAILURE);
  }
  return ptr;
}

char* num_to_letter(colours c)
{
   switch(c)
   {
      case(empty):
      return ".";
      case(red):
      return "R";
      case(green):
      return "G";
      case(yellow):
      return "Y";
      case(blue):
      return "B";
      case(magenta):
      return "M";
      case(cyan):
      return "C";
      case(white):
      return "W";
      case(black):
      return "K";
      default:
      fprintf(stderr,"invalid colour in bookcase\n");
   	exit(EXIT_FAILURE);
   }
}



bool duplicate_colour_rows(const colours* array,int array_size)
{
   int y,x;
   for(y=0;y<array_size-1;y++)
   {
      for(x=y+1;x<array_size;x++)
      {
         if((array[y]==array[x])&&\
            (array[y]!=empty))
         {
            return true;
         }
      }
   }
   return false;
}


colours letter_to_num(char c)
{
   switch(toupper(c))
   {
      case('.'):
      return empty;
      case('R'):
      return red;
      case('G'):
      return green;
      case('Y'):
      return yellow;
      case('B'):
      return blue;
      case('M'):
      return magenta;
      case('C'):
      return cyan;
      case('W'):
      return white;
      case('K'):
      return black;
      default:
      fprintf(stderr,"invalid colour in bookcase\n");
   	exit(EXIT_FAILURE);
   }
}
/*get num rows and cols*/
bool get_sizes(FILE *fp,int* num_rows,int* num_cols)
{
   char buffer[BUFFERSIZE];

   if(fgets(buffer,BUFFERSIZE,fp)==NULL)
   {
      fprintf(stderr,"file empty\n");
      exit(EXIT_FAILURE);
   }
   sscanf(buffer, "%d %d",num_rows,num_cols);
   /*considering a bookcase with 0 rows || 0 cols
   not to be a bookcase mostly because it isnt*/
   if(*num_rows>MAXSIZE||*num_rows<1||\
      *num_cols>MAXSIZE||*num_cols<1)
   {
      fprintf(stderr,"invalid number of rows/cols\n");
      exit(EXIT_FAILURE);
   }
   return true;
}


bool check_equality(const size_t dimension,const size_t comparison)
{
   if(dimension!=comparison)
   {
      fprintf(stderr,"dimension different to what is stated on top line\n");
      exit(EXIT_FAILURE);
   }
   return true;
}

bool check_lower_or_equal(const int dimension,const int comparison)
{
   if(dimension>comparison)
   {
      fprintf(stderr,"dimension greater than what is stated on top line\n");
      exit(EXIT_FAILURE);
   }
   return true;
}
