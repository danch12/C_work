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

typedef struct word_container
{
   char** words;
   int position;
   int capacity;
}word_cont;

FILE* get_file_words(char* filename,int* lines);
word_cont* read_in_file(char* filename);
bool free_word_cont(word_cont* to_free);
bool valid_instruct(word_cont* to_check);
#endif
