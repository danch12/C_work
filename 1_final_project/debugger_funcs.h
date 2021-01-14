#infdef DEBUGGER_FUNCS_H
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
   double current_val;
   int var_pos;
   double end_val;
}loop_tracker;




typedef struct debugger
{
   word_cont* program;
   line_cont* output;
   loop_tracker** ;

}debugger;


#endif
