#ifndef EXTENSION_FLOW_H
#define EXTENSION_FLOW_H

#include "specific.h"
#include "parser_funcs.h"
#include "interpreter_funcs.h"
#include "extension_funcs.h"

#define EPSILON 0.00001
typedef enum comparator {equal,greater,less,inv_op} comparator;

comparator get_comparator(word_cont* to_check);
bool valid_expression(word_cont* to_check);
bool valid_flowstate(word_cont* to_check);
bool valid_else(word_cont* to_check);


bool run_flowstate(word_cont* to_check,line_cont* line_arr);
bool run_false(word_cont* to_check,line_cont* line_arr);
bool run_else(word_cont* to_check,line_cont* line_arr);
bool run_true(word_cont* to_check,line_cont* line_arr);
bool do_comparison(word_cont* to_check,bool* result,line_cont* line_arr);
bool do_comparison_helper(double vn_1,double vn_2,comparator cmp);
bool comp_doubles(double d_1, double d_2);

#endif
