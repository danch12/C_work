#ifndef ARRAY_FUNCS_H
#define ARRAY_FUNCS_H

#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <string.h>
#include <assert.h>
#include <ctype.h>
#include <math.h>
#include "../general.h"


typedef struct t_node
{
   double data;
   struct t_node* next;
}t_node;

typedef struct turt_arr
{
   unsigned int size;
   t_node* head;
   t_node* tail;
}turt_arr;


bool remove_val(turt_arr* t_arr,int position);
bool get_arr_val(turt_arr* t_arr,int position,double* val);
bool free_arr(turt_arr* to_free);
turt_arr* arr_init(void);
bool insert_arr(double data,turt_arr* t_arr,int position);
bool append_arr(double data,turt_arr* t_arr);
bool change_val_arr(double n_data,int position,\
                  turt_arr* t_arr);

#endif
