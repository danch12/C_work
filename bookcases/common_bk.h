#ifndef _common_bk_h
#define _common_bk_h

#include <stdio.h>
#define BUFFERSIZE 100
#define MAXSIZE 9

typedef enum colours {empty,red, green,yellow,blue,magenta,cyan,white,black} colours;
typedef enum bool {false, true} bool;


bool check_lower_or_equal(const int dimension,const int comparison);
bool check_equality(const size_t dimension,const size_t comparison);
void* safe_calloc(size_t nitems, size_t size);
char* num_to_letter(colours c);
bool duplicate_colour_rows(const colours* array,int array_size);
colours letter_to_num(char c);
bool get_sizes(FILE *fp,int* num_rows,int* num_cols);
colours letter_to_num(char c);
#endif
