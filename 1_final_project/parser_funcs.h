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
#define NUMINSTRUCTIONS 3
#define MAXLEN 1000
#define MAXERRLEN 100
#define NUMVARS 26

/*was going to use a hash map to store variables but
seems a bit extravagent when there are only 26 possible
variables - if i implement functions in future i will
use hash map though- although this also leads to an
interesting question of how we indicate a variable hasnt
been created as something like set A := 0 should be valid
so cant just fill with zeros. because of this going to use
pointers so we know that when we see a not NULL pointer
that variable has been set

will store the stack and any variable arrays in the
word container as they are sort of like input
controls where as line container is more of
a output control so will keep separate-
we can see word_container as holding all the
intermediate stages of instructions - basically
everything apart from the moves
*/


/*valid funcs return true if entry is valid*/
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
