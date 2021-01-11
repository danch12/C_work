#ifndef SPECIFIC_H
#define SPECIFIC_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../ADTS_and_general/general.h"
#include "../ADTS_and_general/stacks/stack_funcs.h"
#include "../ADTS_and_general/arrays/array_funcs.h"
#include "../ADTS_and_general/hash_map/hash_funcs.h"

#define MAXLEN 1000
#define NUMVARS 26
#define MAXERRLEN 100
#define INSTRUCTLEN 3
#define NUMINSTRUCTIONS 3
#define UNUSED -1
#define INITSIZE 16
#define SCALEFACTOR 2
#define LETTER_TO_NUM 65

#define PI 3.14159265359
#define DEGTORAD 180
#define DEGREES 360
#define ORIGIN 0


typedef enum direction {left, right,invalid} direction;
typedef enum op {plus, minus,mult,divide,invalid_op} op;

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
   assoc* arr_map;
   assoc* func_map;
   struct word_container* parent;
   double* return_val;

}word_cont;

typedef struct coord
{
   double x;
   double y;
}coord;

typedef struct line
{
   coord* start;
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
bool valid_instruct(word_cont* to_check);

bool valid_varnum(word_cont* to_check);
bool get_varnum(word_cont* to_check,double* num,line_cont* line_arr);
bool run_set(word_cont* to_check,line_cont* line_arr);
bool run_instruction(word_cont* to_check,line_cont* line_arr);
bool move_forward(word_cont* to_check,line_cont* l_arr);
bool get_rotation(word_cont* to_check,line_cont* line_arr);
bool run_do(word_cont* to_check,line_cont* line_arr);
/*grabs info and checks validity*/
bool do_helper(word_cont* to_check,int* var_pos,\
               double* start,double* end,line_cont* line_arr);
bool polish_num(word_cont* to_check,line_cont* line_arr);
/*num is going to be passed in by set function*/
bool run_polish(word_cont* to_check,double* num,line_cont* line_arr);
bool add_path(char* filename,word_cont* n_cont);
#endif
