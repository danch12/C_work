#ifndef FUNCTION_stack_FUNCS_H
#define FUNCTION_stack_FUNCS_H

#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <string.h>
#include <assert.h>

#include "../general.h"




typedef struct func_node
{
   /*can store it as a number
   because we dont actually put
   operators on the stack*/
	void* data;
  struct func_node* next;
}func_node;

typedef func_node* func_nodeptr;

typedef struct func_stack
{
  func_nodeptr start;
  int size;
  double* return_val;
}func_stack;

/*return void then cast when appropriate*/
func_stack* func_stack_init(void);
func_nodeptr init_func_node(void* d);
void func_stack_push(func_stack* s,void* d);
bool func_stack_pop(func_stack* s, void** new_d);
bool func_stack_free(func_stack* s);
void func_stack_tostr(func_stack* s, char* str);
bool func_stack_peek(func_stack* s, void** new_d);

#endif
