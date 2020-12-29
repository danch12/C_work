#ifndef SPECIFIC_H
#define SPECIFIC_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../ADTS_and_general/general.h"
#include "../ADTS_and_general/stacks/stack_funcs.h"
#include "../ADTS_and_general/hash_map/hash_funcs.h"

#define MAXLEN 1000
#define NUMVARS 26
#define MAXERRLEN 100



typedef struct word_container
{
   char** words;
   int position;
   int capacity;
   stack* stackptr;
   double* var_array[NUMVARS];
   char err_message[MAXERRLEN];
   assoc* func_map;
}word_cont;

typedef struct function_container
{

   int arg_placer[NUMVARS];
   int n_args;
   double* var_array[NUMVARS];
   char** words;
   stack* stackptr;
   int position;
   int capacity;
   assoc* func_map;
}func_cont;


FILE* get_file_words(char* filename,int* lines);
word_cont* read_in_file(char* filename);
bool free_word_cont(word_cont* to_free);
#endif
