#include "interpreter_funcs.h"
#include "ADTS_and_general/neillsdl2.h"

#define MIDWIDTH WWIDTH/2
#define MIDHEIGHT WHEIGHT/2
#define WHITE 255



void draw_lines(line_cont* l_arr);


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
            exit(EXIT_FAILURE);
         }
         draw_lines(l_cont);
      }
   }

   free_line_cont(l_cont);
   free_word_cont(w_cont);
   return 0;
}




/*your functions looked pretty good for the
housekeeping stuff*/
void draw_lines(line_cont* l_arr)
{
   SDL_Simplewin sw;
   int i;
   if(l_arr)
   {
      Neill_SDL_Init(&sw);
      /*technically only white when all three are white
      but MAX doesnt really give same effect*/
      Neill_SDL_SetDrawColour(&sw,WHITE,WHITE,WHITE);
      for(i=0;i<l_arr->size;i++)
      {

         SDL_RenderDrawLine(sw.renderer, \
                           (int)l_arr->array[i]->start->x+MIDWIDTH,\
                           (int)l_arr->array[i]->start->y+MIDHEIGHT, \
                            (int)l_arr->array[i]->end->x+MIDWIDTH,\
                         (int)l_arr->array[i]->end->y+MIDHEIGHT);
         Neill_SDL_UpdateScreen(&sw);
      }
      do
      {
        Neill_SDL_Events(&sw);
      }while(!sw.finished);
      SDL_Quit();
      atexit(SDL_Quit);
   }


}
