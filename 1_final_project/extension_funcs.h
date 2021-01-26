#ifndef EXTENSION_FUNCS_H
#define EXTENSION_FUNCS_H

#include "specific.h"
#include "parser_funcs.h"
#include "interpreter_funcs.h"
#include "../ADTS_and_general/stacks/stack_funcs.h"
#include "../ADTS_and_general/hash_map/hash_funcs.h"
#include "extension_flow.h"
#include "extension_arrays.h"


#define MAXFUNCLEN 50
#define INITFUNCCAP 20
#define FUNCSCALE 2

#define START 0
#define START_BRACKET 1


/*sets the name of the function*/
bool set_funcvar(word_cont* to_check,char func_name[MAXFUNCLEN]);
bool valid_funcvar(word_cont* to_check);

/*argset is the arguments used when setting the function*/
bool valid_argset(word_cont* to_check);
bool get_argset(word_cont* to_check,word_cont* n_func);
/*get single arg - used recursively by get argset*/
bool get_arg(word_cont* to_check,word_cont* n_func);
/*checks that the set function is valid and then copies over
the function into a word container*/
bool get_func_info(word_cont* to_check,word_cont* n_func);
bool valid_funcset(word_cont* to_check);
/*does the whole SETFUNC instruction */
bool run_funcset(word_cont* to_check);
/*creates space in new func and copies over the words from
setfunc instruct list*/
bool copy_over_orig(word_cont* to_check,word_cont* n_func);

/*looks for a set function - will look for local functions
first before going up in the hierarchy */
bool get_funcvar(word_cont* to_check,word_cont** to_get);
/*when a function is called in the turtle code this places
a single arg */
bool place_arg(word_cont* to_check,word_cont* n_func,\
               int position,line_cont* line_arr);
/*when a function is called in the turtle code this places
a all args */
bool place_all_args(word_cont* to_check,word_cont* n_func,\
                  int pos,line_cont* line_arr);
void reset_func(word_cont* func);
bool valid_argsrun(word_cont* to_check);
bool valid_funcrun(word_cont* to_check);
/*runs a function when called*/
bool run_funcrun(word_cont* to_check,\
               line_cont* line_arr,double** return_val);
bool run_return(word_cont* to_check,line_cont* line_arr);
bool valid_return(word_cont* to_check);
word_cont* init_func_cont(void);
bool free_word_cont(word_cont* to_free);
void resize(word_cont* n_func);
word_cont* deep_copy_func(word_cont* to_copy);
void free_copy(word_cont* to_free);
 #endif
