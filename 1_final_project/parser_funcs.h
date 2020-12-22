#ifndef PARSER_FUNCS_H
#define PARSER_FUNCS_H
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <string.h>
#include <assert.h>
#include <ctype.h>
#include <math.h>

#include "general.h"
#include "stack_funcs.h"

#define NUMINSTRUCTIONS 3
#define INSTRUCTLEN 3
#define MAXLEN 1000
#define MAXERRLEN 100
#define NUMVARS 26

typedef struct word_container
{
   char** words;
   int position;
   int capacity;
   stack* stackptr;
   double* var_array[NUMVARS];

    /*would be cool to have possibility of specific error
    messages when theres stuff like stack errors etc*/
   char err_message[MAXERRLEN];
}word_cont;

/*valid funcs return true if entry is valid*/
bool valid_num(word_cont* to_check);
bool valid_instruct(word_cont* to_check);
bool valid_mv(word_cont* to_check,char move[INSTRUCTLEN]);
bool valid_instructlist(word_cont* to_check);
bool valid_main(word_cont* to_check);
bool valid_op(word_cont* to_check);
bool valid_var(word_cont* to_check);
bool valid_varnum(word_cont* to_check);
bool valid_polish(word_cont* to_check);
bool valid_set(word_cont* to_check);
bool valid_do(word_cont* to_check);


FILE* get_file_words(char* filename,int* lines);
word_cont* read_in_file(char* filename);
bool free_word_cont(word_cont* to_free);
/*used for testing
bool concat_word_cont(word_cont* to_concat, char target[MAXLEN]);
word_cont* init_word_cont(void);*/



#endif