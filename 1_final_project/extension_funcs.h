#ifndef EXTENSION_FUNCS_H
#define EXTENSION_FUNCS_H

#include "specific.h"
#include "parser_funcs.h"
#include "interpreter_funcs.h"

#include "../ADTS_and_general/hash_map/hash_funcs.h"


#define MAXFUNCLEN 50
#define INITFUNCCAP 20
#define FUNCSCALE 2

#define START 0
#define START_BRACKET 1



bool set_funcvar(word_cont* to_check,char func_name[MAXFUNCLEN]);
bool valid_funcvar(word_cont* to_check);

bool valid_argset(word_cont* to_check);
bool get_argset(word_cont* to_check,word_cont* n_func);
bool get_arg(word_cont* to_check,word_cont* n_func);
bool get_func_info(word_cont* to_check,word_cont* n_func);
bool valid_funcset(word_cont* to_check);
bool run_funcset(word_cont* to_check);
bool copy_over(word_cont* to_check,word_cont* n_func);

bool get_funcvar(word_cont* to_check,word_cont** to_get);
bool place_arg(word_cont* to_check,word_cont* n_func,\
               int position);
bool place_all_args(word_cont* to_check,word_cont* n_func,int pos);
void reset_func(word_cont* func);
bool valid_argsrun(word_cont* to_check);
bool valid_funcrun(word_cont* to_check);
bool run_funcrun(word_cont* to_check,line_cont* line_arr);

word_cont* init_func_cont(void);
bool free_word_cont(word_cont* to_free);
void resize(word_cont* n_func);

 #endif