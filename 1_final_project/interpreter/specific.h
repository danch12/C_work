#ifndef SPECIFIC_H
#define SPECIFIC_H
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "../ADTS_and_general/general.h"
#include "../ADTS_and_general/stacks/stack_funcs.h"

#define MAXLEN 1000
#define NUMVARS 26
#define MAXERRLEN 100
#define INSTRUCTLEN 3
#define NUMINSTRUCTIONS 3
#define INITSIZE 16
#define SCALEFACTOR 2
#define LETTER_TO_NUM 65
#define PI 3.14159265359
#define DEGTORAD 180
#define DEGREES 360
#define ORIGIN 0

typedef enum direction {left, right,invalid} direction;
typedef enum op {plus, minus,mult,divide,invalid_op} op;


typedef struct word_container
{
   char** words;
   int position;
   int capacity;
   stack* stackptr;
   double* var_array[NUMVARS];
   char err_message[MAXERRLEN];
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
bool run_instruction(word_cont* to_check,line_cont* line_arr);
bool valid_instruct(word_cont* to_check);


bool get_rotation(word_cont* to_check,line_cont* line_arr);
bool move_forward(word_cont* to_check,line_cont* l_arr);
bool valid_varnum(word_cont* to_check);
bool get_varnum(word_cont* to_check,double* num);
bool run_set(word_cont* to_check);
bool polish_num(word_cont* to_check);
bool run_polish(word_cont* to_check,double* num);
bool run_do(word_cont* to_check,line_cont* line_arr);
/*grabs info and checks validity*/
bool do_helper(word_cont* to_check,int* var_pos,\
               double* start,double* end);
#endif
