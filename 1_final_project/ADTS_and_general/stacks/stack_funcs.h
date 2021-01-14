#ifndef STACK_FUNCS_H
#define STACK_FUNCS_H

#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <string.h>
#include <assert.h>

#include "../general.h"

#define MAXELELEN 50
/*i used your stacks code as a starting (and sometimes ending)
point for my stacks code*/

typedef struct node
{
   /*can store it as a number
   because we dont actually put
   operators on the stack*/
	void* data;
  struct node* next;
}node;

typedef node* nodeptr;

typedef struct stack
{
  nodeptr start;
  int size;
  size_t data_size;
}stack;

/*return void then cast when appropriate*/
stack* stack_init(size_t datasize);
void stack_push(stack* s,void* d);
bool stack_pop(stack* s, void** new_d);
bool stack_free(stack* s);
void stack_tostr(stack* s, char* str);
bool stack_peek(stack* s, void** new_d);

#endif
