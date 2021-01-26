#include "parser_funcs.h"


int main(int argc, char* argv[])
{
   word_cont* n_cont;
   if(argc!=2)
   {
      fprintf(stderr, "wrong number of arguments\n" );
      exit(EXIT_FAILURE);
   }
   else
   {
      n_cont=read_in_file(argv[1]);
      if(n_cont)
      {
         if(!valid_main(n_cont))
         {
            fprintf(stderr,"error in processing file around word %d %s\n",\
                  n_cont->position,n_cont->words[n_cont->position]);
            if(strlen(n_cont->err_message)>0)
            {
               fprintf(stderr,"%s",n_cont->err_message);
            }
            exit(EXIT_FAILURE);
         }
         if(n_cont->capacity!=n_cont->position)
         {
            fprintf(stderr,"there may be code after main that has not been checked; be careful\n");
         }
         free_word_cont(n_cont);
      }

   }
   return 0;
}
