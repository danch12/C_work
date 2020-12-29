#include "specific.h"

bool valid_mv(word_cont* to_check,char move[INSTRUCTLEN]);
bool valid_set(word_cont* to_check);
bool valid_do(word_cont* to_check);

bool free_word_cont(word_cont* to_free)
{
   int i;
   if(to_free)
   {
      for(i=0;i<to_free->capacity;i++)
      {
         free(to_free->words[i]);
      }
      free(to_free);
   }

   return true;
}


word_cont* read_in_file(char* filename)
{
   FILE* fp;
   word_cont* n_cont;
   int num_lines,count;
   char buffer[MAXLEN];
   num_lines=0;
   n_cont=(word_cont*)safe_calloc(1,sizeof(word_cont));
   fp=get_file_words(filename,&num_lines);
   n_cont->capacity=num_lines;
   n_cont->position=0;
   n_cont->words= (char**)safe_calloc(num_lines,sizeof(char*));
   count=0;
   while(fscanf(fp,"%s",buffer)==1)
   {
      n_cont->words[count]=(char*)safe_calloc(strlen(buffer)+1,\
                                             sizeof(char));
      strcpy(n_cont->words[count],buffer);
      count++;
   }
   fclose(fp);
   return n_cont;
}


FILE* get_file_words(char* filename,int* lines)
{
   FILE *fp;
   int num_lines;
   char buffer[MAXLEN];
   num_lines=0;
   fp= safe_fopen(filename);

   while(fscanf(fp,"%s",buffer)==1)
   {
      num_lines++;
   }
   *lines=num_lines;
   rewind(fp);
   return fp;
}



bool valid_instruct(word_cont* to_check)
{
   int i;
   int init_pos;
   char instructions[NUMINSTRUCTIONS][INSTRUCTLEN]= {"FD", "LT","RT"};
   init_pos=to_check->position;
   if(init_pos>=to_check->capacity)
   {
      return false;
   }
   for(i=0;i<NUMINSTRUCTIONS;i++)
   {
      if(valid_mv(to_check,instructions[i]))
      {
         return true;
      }
      else
      {
         to_check->position=init_pos;
      }
   }
   if(valid_set(to_check))
   {
      return true;
   }
   to_check->position=init_pos;

   if(valid_do(to_check))
   {
      return true;
   }

   return false;
}
