#ifndef DEBUGGER_FUNCS_H
#define DEBUGGER_FUNCS_H

#define NOTFOUND -1
#define FULLARGSTRLEN 300
#define ONEARGLEN 15
#define TOALPHA 65
#define MAXPOSLEN 1000
#define TWODP 100
#define MAXACTIONLEN 100
#define INFOLEN 200
#define DEBUGSTART 1

#include "specific.h"
#include "ADTS_and_general/general.h"
#include "specific.h"
#include "parser_funcs.h"
#include "interpreter_funcs.h"
#include "extension_funcs.h"
#include "extension_flow.h"
#include "extension_arrays.h"

typedef enum action {to_mistake,s_step,show_vars,show_pos,help,invalid_act} action;

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
   char info[INFOLEN];
   bool stop;
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

/*returns true if no mistakes else false*/
bool advance_to_mistake(debugger* to_check);

void str_num(double num,char num_str[ONEARGLEN]);
void show_current_vars(debugger* debug,char out_str[FULLARGSTRLEN]);
void collate_instruct_messages(debugger* to_check,\
                              char result_str[FULLARGSTRLEN]);
bool check_step_end(debugger* to_check);
action get_action(char action_str[MAXACTIONLEN]);
/*returns false if weird action entered*/
bool run_action(debugger* to_check, char action_str[MAXACTIONLEN],\
               char result_str[FULLARGSTRLEN]);
#endif
