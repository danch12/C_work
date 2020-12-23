#ifndef INTERPRETER_FUNCS_H
#define INTERPRETER_FUNCS_H

#include "parser_funcs.h"

#define MAXERRLEN 100
#define PI 3.14159265359
#define DEGTORAD 180
#define DEGREES 360
#define ORIGIN 0


#define INITSIZE 16
#define SCALEFACTOR 2
#define LETTER_TO_NUM 65


typedef enum direction {left, right,invalid} direction;
typedef enum op {plus, minus,mult,divide,invalid_op} op;



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
   line** array;
   int size;
   int capacity;
}line_cont;

line_cont* init_line_cont(void);
bool free_line_cont(line_cont* to_free);
void store_line(line_cont* l_arr, line* to_add);
line* init_origin(void);
bool free_line(line* to_free);
line* finish_line(line* prev_line,coord* endpoint);
/*rotate a coord around a point*/
bool rotate(double degrees,coord* to_rotate, coord* rotation_point);
coord* init_coords(double x, double y);

/*extract num from word container*/
bool get_num(word_cont* to_check,double* num);
/*pretty much the same as parser valid_num except we
dont increase the position at the end
called different things because we import*/
bool valid_number(word_cont* to_check);

bool run_instruction(word_cont* to_check,line_cont* line_arr,\
                     line* pending_line);
bool run_instruction_list(word_cont* to_check,line_cont* line_arr,\
                           line* pending_line);
bool run_main(word_cont* to_check,line_cont* line_arr,\
            line* pending_line);
/*get rotation for lt and rt commands - stores them in
a pending line and executes when fd command is issued*/
bool get_rotation(word_cont* to_check,line* pending_line);
direction direction_helper(word_cont* to_check);


bool move_forward(word_cont* to_check,line* pending_line,\
                  line_cont* l_arr);

/*get op v similar to parser except we dont increase position
and we return a op instead of bool*/
op get_op(word_cont* to_check);
bool do_operation(word_cont* to_check);
bool polish_num(word_cont* to_check);
bool finish_polish(word_cont* to_check,double* result);
bool run_polish(word_cont* to_check,double* num);

bool run_set(word_cont* to_check);
/*pass position back through var_p
*we see vars as A- Z but really they could be 0-25*/
bool get_var_pos(word_cont* to_check,int* var_p);
bool valid_variable(word_cont* to_check);
bool get_var(word_cont* to_check,double* num);
bool get_varnum(word_cont* to_check,double* num);


/*need to check for syntax errors even if loop not carried out*/
bool run_do(word_cont* to_check,line_cont* line_arr,\
                           line* pending_line);
bool do_helper(word_cont* to_check,int* var_pos,\
            double* start,double* end);

 #endif
