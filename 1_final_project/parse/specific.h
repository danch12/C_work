#ifndef SPECIFIC_H
#define SPECIFIC_H
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../ADTS_and_general/general.h"

#define MAXLEN 1000
#define NUMVARS 26
#define INSTRUCTLEN 3
#define NUMINSTRUCTIONS 3
#define MAXERRLEN 100
#define FINAL_INCREASE 1

typedef struct word_container
{
   char** words;
   int position;
   /*capacity is index of final word*/
   int capacity;
   char err_message[MAXERRLEN];
}word_cont;

typedef enum opcode {fd,rot,do_loop,set,inv_opcode} opcode;


FILE* get_file_words(char* filename,int* lines);
void read_words(char* filename,word_cont* n_cont);
word_cont* read_in_file(char* filename);
bool free_word_cont(word_cont* to_free);
bool valid_instruct(word_cont* to_check);
opcode get_opcode(word_cont* to_check);
bool valid_varnum(word_cont* to_check);
#endif
