#include "specific.h"

bool valid_mv(word_cont* to_check,char move[INSTRUCTLEN]);
bool valid_set(word_cont* to_check);
bool valid_do(word_cont* to_check);
bool valid_var(word_cont* to_check);
bool valid_num(word_cont* to_check);

bool free_word_cont(word_cont* to_free)
{
   int i;
   if(to_free)
   {
      for(i=0;i<to_free->capacity;i++)
      {
         free(to_free->words[i]);
      }
      free(to_free->words);
      free(to_free);
   }

   return true;
}


bool valid_varnum(word_cont* to_check)
{
   if(to_check->position>=to_check->capacity)
   {
      return false;
   }
   if(valid_num(to_check))
   {
      return true;
   }
   if(valid_var(to_check))
   {
      return true;
   }
   return false;
}


void read_words(char* filename,word_cont* n_cont)
{
   int num_lines,count;
   FILE* fp;
   char buffer[MAXLEN];
   num_lines=0;
   fp=get_file_words(filename,&num_lines);
   n_cont->capacity=num_lines;
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
}

word_cont* read_in_file(char* filename)
{
   word_cont* n_cont;
   n_cont=(word_cont*)safe_calloc(1,sizeof(word_cont));
   read_words(filename,n_cont);
   n_cont->position=0;
   n_cont->err_message[0]='\0';
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

opcode get_opcode(word_cont* to_check)
{
   if(safe_samestr(to_check,"FD"))
   {
      return fd;
   }
   if((safe_samestr(to_check,"LT"))||\
      (safe_samestr(to_check,"RT")))
   {
      return rot;
   }
   if(safe_samestr(to_check,"DO"))
   {
      return do_loop;
   }
   if(safe_samestr(to_check,"SET"))
   {
      return set;
   }
   return inv_opcode;
}




bool valid_instruct(word_cont* to_check)
{
   opcode current_op;
   int init_pos;
   init_pos=to_check->position;
   current_op=get_opcode(to_check);
   switch(current_op)
   {
      case fd:
      if(valid_mv(to_check,"FD"))
      {
         return true;
      }
      break;
      case rot:
      if(valid_mv(to_check,"LT"))
      {
         return true;
      }
      to_check->position=init_pos;
      if(valid_mv(to_check,"RT"))
      {
         return true;
      }
      break;
      case do_loop:
      if(valid_do(to_check))
      {
         return true;
      }
      break;
      case set:
      if(valid_set(to_check))
      {
         return true;
      }
      break;
      default:
      return false;
   }
   return false;
}
