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
      debug->program->position=DEBUGSTART;
      debug->output=init_line_cont();
      result[0]='\0';
      if(debug->program)
      {
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
