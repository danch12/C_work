#include "debugger_funcs.h"



int main(int argc, char* argv[])
{
   debugger* debug;
   char command[MAXACTIONLEN];
   char result[FULLARGSTRLEN];
   if(argc!=2)
   {
      fprintf(stderr, "wrong number of arguments\n" );
      exit(EXIT_FAILURE);
   }
   else
   {
      debug=init_debugger();
      debug->program=read_in_file(argv[1]);
      if(!initial_checks(debug))
      {
         fprintf(stderr,"%s",debug->info);
         exit(EXIT_FAILURE);
      }
      debug->output=init_line_cont();
      result[0]='\0';
      if(debug->program)
      {
         printf("enter a command:\n");
         fgets(command,FULLARGSTRLEN,stdin);
         while(strcmp(command, "quit\n") != 0)
         {
            if(!run_action(debug,command,result))
            {
               printf("unknown command\n");
            }
            if(strlen(result)>0)
            {
               printf("%s\n",result);
               result[0]='\0';
            }
            fgets(command,FULLARGSTRLEN,stdin);
         }
      }
      free_debugger(debug);
      return 0;
   }
}


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
