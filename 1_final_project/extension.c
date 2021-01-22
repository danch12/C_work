#include "extension_master.h"


int main(int argc, char* argv[])
{
   word_cont* w_cont;
   line_cont* l_cont;
   if(argc!=2)
   {
      fprintf(stderr, "wrong number of arguments\n" );
      exit(EXIT_FAILURE);
   }
   else
   {
      w_cont=read_in_file(argv[1]);
      l_cont=init_line_cont();
      if(w_cont)
      {
         if(!run_main(w_cont,l_cont))
         {
            fprintf(stderr,"error in processing file around word %d %s\n",\
                  w_cont->position,w_cont->words[w_cont->position]);
            if(strlen(w_cont->err_message)>0)
            {
               fprintf(stderr,"%s\n",w_cont->err_message);
            }
            do
            {
              Neill_SDL_Events(&w_cont->sw);
            }while(!w_cont->sw.finished);
            SDL_Quit();
            atexit(SDL_Quit);
            exit(EXIT_FAILURE);
         }

      }
   }
   do
   {
     Neill_SDL_Events(&w_cont->sw);
   }while(!w_cont->sw.finished);
   SDL_Quit();
   atexit(SDL_Quit);
   free_line_cont(l_cont);
   free_word_cont(w_cont);
   return 0;
}
