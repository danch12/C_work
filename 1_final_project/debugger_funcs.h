#ifndef DEBUGGER_FUNCS_H
#define DEBUGGER_FUNCS_H

#define NOTFOUND -1

#include "specific.h"
#include "ADTS_and_general/general.h"
#include "specific.h"
#include "parser_funcs.h"
#include "interpreter_funcs.h"
#include "extension_funcs.h"
#include "extension_flow.h"
#include "extension_arrays.h"


typedef struct loop_tracker
{
   int start_pos;
   int end_pos;
   double start_val;
   int var_pos;
   double end_val;
   bool fin_flag;
}loop_tracker;




typedef struct debugger
{
   word_cont* program;
   line_cont* output;
   stack* loop_stack;
   char info[100];
}debugger;


bool step_do(debugger* debug);
loop_tracker* step_do_helper(word_cont* to_check);
debugger* init_debugger(void);
bool free_debugger(debugger* to_free);
bool step_instruction(debugger* to_check);

/*finds end of loop/ instructlist*/
int find_end_pos(word_cont* to_check);

/*checks to see if we are at the end of a loop
and if so updates to position back to start*/
void check_loop_end(debugger* to_check);

/*checks to see if all iterations of loop have finished*/
bool check_loop_fin(debugger* to_check);
bool instruct_checks(debugger* to_check);
#endif
