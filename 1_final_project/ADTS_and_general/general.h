#ifndef GENERAL_H
#define GENERAL_H
#include <stdio.h>
#include <stdlib.h>

typedef enum bool {false, true} bool;
/*return void then cast when appropriate*/
void* safe_calloc(size_t nitems, size_t size);
FILE* safe_fopen(char* filename);
void* safe_calloc_2d(int height,int width ,size_t size);
void free_2d(int** ptr,int height);
#endif
