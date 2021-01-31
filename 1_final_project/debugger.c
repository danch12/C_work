#include "debugger_funcs.h"



int main(int argc, char* argv[])
{
   debugger* debug;
   char command[MAXACTIONLEN];
   char num[MAXACTIONLEN];
   char result[FULLARGSTRLEN];
   debug=NULL;
   if(argc!=2)
   {
      fprintf(stderr, "wrong number of arguments\n" );
      exit(EXIT_FAILURE);
   }
   else
   {
      if(!init_debug_from_file(argv[1],&debug))
      {
         fprintf(stderr,"%s\n",debug->info);
         exit(EXIT_FAILURE);
      }
      result[0]='\0';
      if(debug->program)
      {
         printf("enter a command:\n");
         safe_fgets(command,FULLARGSTRLEN,stdin);
         while(strcmp(command, "quit\n") != 0)
         {
            if(strcmp(command,"set break\n")==0)
            {
               printf("enter line number:\n");
               safe_fgets(num,FULLARGSTRLEN,stdin);
            }
            if(!run_action(debug,command,num,result))
            {
               printf("unknown command\n");
            }
            if(strlen(result)>0)
            {
               printf("%s\n",result);
               result[0]='\0';
            }
            safe_fgets(command,FULLARGSTRLEN,stdin);
         }
      }
      free_debugger(debug);
      return 0;
   }
}
