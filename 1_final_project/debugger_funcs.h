#ifndef DEBUGGER_FUNCS_H
#define DEBUGGER_FUNCS_H

#include "specific.h"
#include "ADTS_and_general/general.h"
#include "specific.h"
#include "parser_funcs.h"
#include "interpreter_funcs.h"
#include "extension_funcs.h"
#include "extension_flow.h"
#include "extension_arrays.h"

#ifdef LIVE_VERSION
#include "ADTS_and_general/neillsdl2.h"
#define MIDWIDTH WWIDTH/2
#define MIDHEIGHT WHEIGHT/2
#define WHITE 255
#endif

#define NOTFOUND -1
#define FULLARGSTRLEN 300
#define ONEARGLEN 15
#define TOALPHA 65
#define MAXPOSLEN 1000
#define TWODP 100
#define MAXACTIONLEN 100
#define INFOLEN 200
#define LEVIND 1
#define NWORDS 8
#define MWORDLEN 5
#define LARGESTART 10

typedef enum action {to_mistake,s_step,show_vars,show_pos,show_code,show_coords,suggestion,visualize,invalid_act} action;

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
int find_end_pos(word_cont* to_check,int starting_brackets);
/*checks to see if extra code after main */
bool check_past_main(debugger* debug);

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
bool initial_checks(debugger* to_check);
bool check_start(debugger* to_check);
/*will show 5 words behind and 5 ahead*/
void show_code_pos(debugger* to_check,\
               char out_str[FULLARGSTRLEN]);
/*will show last 5 coords */
void show_recent_coords(debugger* debug,\
                  char out_str[FULLARGSTRLEN]);


int min_three(int a,int b, int c);
/*levenshtein matrix is 1 indexed so have to add
1 to a lot of things*/
int levenshtein(char* word_1,char* word_2);

int find_low(int lev_distance[NWORDS]);
void str_to_upper(char orig[MAXLEN],char upper[MAXLEN]);
/*will not make suggestions for things like hfhjfhdkhdjfhfhffzzzzz
because there is no telling what they wanted there */
void suggest_keyword(char orig[MAXLEN], \
         char suggestion[FULLARGSTRLEN]);
void make_suggestion(debugger* to_check,\
            char result_str[FULLARGSTRLEN]);
void draw_lines(line_cont* l_arr);
#endif
