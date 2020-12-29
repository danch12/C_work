#include "specific.h"


bool get_rotation(word_cont* to_check,line_cont* line_arr);
bool move_forward(word_cont* to_check,line_cont* l_arr);
bool run_set(word_cont* to_check);
bool run_do(word_cont* to_check,line_cont* line_arr);
bool run_funcset(word_cont* to_check);
bool run_funcrun(word_cont* to_check,line_cont* line_arr);
bool valid_mv(word_cont* to_check,char move[INSTRUCTLEN]);
bool valid_set(word_cont* to_check);
bool valid_do(word_cont* to_check);
bool valid_funcrun(word_cont* to_check);
bool valid_funcset(word_cont* to_check);

void deep_free_assoc(assoc* a)
{
   unsigned int i;
   if(a)
   {
      for(i=0;i<a->capacity;i++)
      {
         if(a->arr[i])
         {
            free(a->arr[i]->key);
            if(a->arr[i]->value)
            {

               free_word_cont(a->arr[i]->value);
            }

            free(a->arr[i]);
         }
      }
      free(a->arr);
      free(a);
   }
}



bool free_word_cont(word_cont* to_free)
{
   int i;
   if(to_free)
   {
      for(i=0;i<to_free->capacity;i++)
      {
         free(to_free->words[i]);
      }
      for(i=0;i<NUMVARS;i++)
      {
         if(to_free->var_array[i])
         {
            free(to_free->var_array[i]);
         }
      }
      free(to_free->words);
      stack_free(to_free->stackptr);
      deep_free_assoc(to_free->func_map);
      free(to_free);
   }

   return true;
}




word_cont* read_in_file(char* filename)
{
   FILE* fp;
   word_cont* n_cont;
   int num_lines,count,i;
   char buffer[MAXLEN];
   num_lines=0;
   n_cont=(word_cont*)safe_calloc(1,sizeof(word_cont));
   fp=get_file_words(filename,&num_lines);
   n_cont->capacity=num_lines;
   n_cont->position=0;
   n_cont->words= (char**)safe_calloc(num_lines,sizeof(char*));
   n_cont->err_message[0]='\0';

   count=0;
   while(fscanf(fp,"%s",buffer)==1)
   {
      n_cont->words[count]=(char*)safe_calloc(strlen(buffer)+1,\
                                             sizeof(char));
      strcpy(n_cont->words[count],buffer);
      count++;
   }
   fclose(fp);

   n_cont->stackptr=stack_init();
   for(i=0;i<NUMVARS;i++)
   {
      n_cont->var_array[i]=NULL;
   }
   n_cont->func_map=assoc_init();

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



bool run_instruction(word_cont* to_check,line_cont* line_arr)
{
   int init_pos;
   init_pos=to_check->position;
   if(init_pos>=to_check->capacity)
   {
      return false;
   }
   if(get_rotation(to_check,line_arr))
   {
      return true;
   }
   to_check->position=init_pos;
   if(move_forward(to_check,line_arr))
   {
      return true;
   }
   to_check->position=init_pos;
   if(run_set(to_check))
   {
      return true;
   }
   to_check->position=init_pos;
   if(run_do(to_check,line_arr))
   {
      return true;
   }
   to_check->position=init_pos;
   if(run_funcset(to_check))
   {
      return true;
   }
   to_check->position=init_pos;
   if(run_funcrun(to_check,line_arr))
   {
      return true;
   }

   return false;
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
   to_check->position=init_pos;
   if(valid_funcset(to_check))
   {
      return true;
   }
   to_check->position=init_pos;
   if(valid_funcrun(to_check))
   {
      return true;
   }
   return false;
}
