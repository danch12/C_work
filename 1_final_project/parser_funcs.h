#ifndef PARSER_FUNCS_H
#define PARSER_FUNCS_H
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <string.h>
#include <assert.h>
#include <ctype.h>
#include <math.h>

#include "ADTS_and_general/general.h"

#include "specific.h"

#define MAXERRLEN 100
#define NUMVARS 26
#define BLANK 0
#define NOTFOUND -1
#define BRACKETSTART 0


/*valid funcs return true if entry is valid will also increase the
position of to_check*/
bool valid_larger_than_one(word_cont* to_check,int len);
bool valid_num(word_cont* to_check);

bool valid_mv(word_cont* to_check,char move[INSTRUCTLEN]);
bool valid_instructlist(word_cont* to_check);
bool valid_main(word_cont* to_check);
bool valid_op(word_cont* to_check);
bool valid_var(word_cont* to_check);
bool valid_varnum(word_cont* to_check);
bool valid_polish(word_cont* to_check);
bool valid_set(word_cont* to_check);
bool valid_do(word_cont* to_check);



#endif
