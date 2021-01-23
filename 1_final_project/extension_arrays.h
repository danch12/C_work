#ifndef EXTENSION_ARRAYS_H
#define EXTENSION_ARRAYS_H


#include "specific.h"
#include "parser_funcs.h"
#include "interpreter_funcs.h"
#include "../ADTS_and_general/stacks/stack_funcs.h"
#include "../ADTS_and_general/hash_map/hash_funcs.h"
#include "../ADTS_and_general/arrays/array_funcs.h"
#include "extension_flow.h"
#include "extension_funcs.h"


#define MAXARRLEN 50
#define MINARRLEN 2


bool get_arr_identifier(word_cont* to_check,\
                        char arr_name[MAXARRLEN]);
bool valid_arr_identifier(word_cont* to_check);
bool valid_init_arr(word_cont* to_check);
/*store value into array*/
void store_arr(word_cont* to_check,char arr_name[MAXARRLEN],\
               turt_arr* n_arr);

/*guard aginst decimals being used as index*/
bool get_valid_ind(word_cont* to_check,line_cont* line_arr,int* ind);
bool run_init_arr(word_cont* to_check);
bool valid_append(word_cont* to_check);
bool run_append(word_cont* to_check,line_cont* line_arr);
turt_arr* get_arr(word_cont* to_check,char arr_name[MAXARRLEN]);

/*gets info and checks for validity*/
bool change_helper(word_cont* to_check, line_cont* line_arr,\
                  int* ind,double* num,char arr_name[MAXARRLEN]);
bool valid_change(word_cont* to_check);
bool run_change(word_cont* to_check,line_cont* line_arr);

/*gets info and checks for validity*/
bool delete_helper(word_cont* to_check,line_cont* line_arr,\
                  int* ind,char arr_name[MAXARRLEN]);
/*if an index above the size of array then just returns true*/
bool run_delete_arr_val(word_cont* to_check,line_cont* line_arr);
bool valid_delete_arr_val(word_cont* to_check);


/*once you set the bounds for a loop cant change
so we prevent  weird behaviour when values in a list are
deleted this is only relevant for do loops in relation to
lists really*/
bool valid_access_val(word_cont* to_check);
bool run_access_val(word_cont* to_check,line_cont* line_arr,double* num);
bool access_helper(word_cont* to_check,line_cont* line_arr,\
                  char arr_name[MAXARRLEN],int* ind);

bool valid_filepath(word_cont* to_check);
bool valid_file_to_array(word_cont* to_check);

bool load_in(word_cont* to_check,turt_arr* arr,char* filepath);
bool run_file_to_array(word_cont* to_check);

bool valid_len(word_cont* to_check);
bool run_len(word_cont* to_check,double* num);

#endif
