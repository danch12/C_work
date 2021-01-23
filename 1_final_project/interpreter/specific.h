#ifndef SPECIFIC_H
#define SPECIFIC_H
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "../ADTS_and_general/general.h"
#include "../ADTS_and_general/stacks/stack_funcs.h"

#ifdef INTERP_PRODUCTION
#include "../ADTS_and_general/neillsdl2.h"
#define MIDWIDTH WWIDTH/2
#define MIDHEIGHT WHEIGHT/2
#define WHITE 255

#endif

#define MAXLEN 1000
#define NUMVARS 26
#define MAXERRLEN 100
#define INSTRUCTLEN 3
#define FINAL_INCREASE 1
#define INITSIZE 16
#define SCALEFACTOR 2
#define LETTER_TO_NUM 65
#define PI 3.14159265359
#define DEGTORAD 180
#define DEGREES 360
#define ORIGIN 0
#define SPREAD 5
#define EPSILON 0.000001

typedef enum direction {left, right,invalid} direction;
typedef enum op {plus, minus,mult,divide,invalid_op} op;
typedef enum opcode {fd,rot,do_loop,set,i_list,inv_opcode} opcode;

typedef struct word_container
{
   char** words;
   int position;
   int capacity;
   stack* stackptr;
   double* var_array[NUMVARS];
   char err_message[MAXERRLEN];
   #ifdef INTERP_PRODUCTION
   SDL_Simplewin sw;
   #endif
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
void read_words(char* filename,word_cont* n_cont);
word_cont* read_in_file(char* filename);
bool free_word_cont(word_cont* to_free);
opcode get_opcode(word_cont* to_check);
bool run_instruction(word_cont* to_check,line_cont* line_arr);
bool valid_instruct(word_cont* to_check);

double wrap_around(double i, double i_max);

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
#ifdef INTERP_PRODUCTION
void simple_draw(word_cont* to_check,line* to_draw);
#endif


#endif
