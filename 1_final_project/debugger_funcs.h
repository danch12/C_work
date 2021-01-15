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
}loop_tracker;




typedef struct debugger
{
   word_cont* program;
   line_cont* output;
   stack* loop_stack;

}debugger;
bool step_do(debugger* debug);
loop_tracker* step_do_helper(word_cont* to_check,line_cont* line_arr);
debugger* init_debugger(void);
bool free_debugger(debugger* to_free);
bool step_instruction(debugger* to_check);
int find_end_pos(word_cont* to_check);
#endif
