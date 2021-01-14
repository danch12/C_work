#ifndef STACK_DEBUG_FUNCS_H
#define STACK_DEBUG_FUNCS_H

#include "../general.h"
/*i used your stacks code as a starting (and sometimes ending)
point for my stacks code*/

typedef struct loop_tracker loop_tracker;



typedef struct dnode
{
   /*can store it as a number
   because we dont actually put
   operators on the stack*/
  loop_tracker* data;
  struct node* next;
}dnode;

typedef node* dnodeptr;

typedef struct dstack
{
  dnodeptr start;
  int size;
}dstack;

/*return void then cast when appropriate*/
dstack* dstack_init(void);
void dstack_push(dstack* s,loop_tracker* d);
bool dstack_pop(dstack* s, loop_tracker* new_d);
bool dstack_free(dstack* s);
bool dstack_peek(dstack* s, loop_tracker* new_d);
