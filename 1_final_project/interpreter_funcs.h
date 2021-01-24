#ifndef INTERPRETER_FUNCS_H
#define INTERPRETER_FUNCS_H
#include "specific.h"
#include "parser_funcs.h"




/*need to create some new functions that do very similar things
to parser stage but increment at different times - this is
because we will use old parser functions to check for
syntax errors in do loops so even if do loop not executed
it will throw errors for bad syntax i thought about adding
arguments to the exsiting functions but as we call alot of them
recursively it would be hard to pass the arguments in at the right
time*/

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

bool run_instruction(word_cont* to_check,line_cont* line_arr);
bool run_instruction_list(word_cont* to_check,line_cont* line_arr);
bool run_main(word_cont* to_check,line_cont* line_arr);
/*get amount we rotate for lt and rt commands - stores them in
a pending line and executes when fd command is issued*/
bool get_rotation(word_cont* to_check,line_cont* line_arr);
direction direction_helper(word_cont* to_check);


bool move_forward(word_cont* to_check,line_cont* l_arr);

/*get op v similar to parser except we dont increase position
and we return a op instead of bool*/
op get_op(word_cont* to_check);
bool do_operation(word_cont* to_check);

bool finish_polish(word_cont* to_check,double* result);

/*pass position back through var_p
*we see vars as A- Z but really they could be 0-25*/
bool get_var_pos(word_cont* to_check,int* var_p);
bool valid_variable(word_cont* to_check);
bool get_var(word_cont* to_check,double* num);



 #endif
