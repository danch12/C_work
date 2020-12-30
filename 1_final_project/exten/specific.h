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
#define INSTRUCTLEN 3
#define NUMINSTRUCTIONS 3
#define UNUSED -1

/*started off by making a struct that specifically holds
functions - but makes more sense to just add that
functionality to word_container so we dont have to redo
every function so it allows function containers
also adding a pointer to the functions "parent"
basically so we can use mains functions within other
functions
*/
typedef struct word_container
{
   int arg_placer[NUMVARS];
   int n_args;
   char** words;
   int position;
   int capacity;
   stack* stackptr;
   double* var_array[NUMVARS];
   char err_message[MAXERRLEN];
   assoc* func_map;
   struct word_container* parent;
}word_cont;

typedef struct coord
{
   double x;
   double y;
}coord;

typedef struct line
{
   coord* start;
   /*going to be a running total
   take abs(num%360) */
   double rotation;
   coord* end;
}line;


typedef struct line_container
{
   line* pending_line;
   line** array;
   int size;
   int capacity;
}line_cont;



FILE* get_file_words(char* filename,int* lines);
word_cont* read_in_file(char* filename);
bool free_word_cont(word_cont* to_free);
bool run_instruction(word_cont* to_check,line_cont* line_arr);
bool valid_instruct(word_cont* to_check);
#endif
