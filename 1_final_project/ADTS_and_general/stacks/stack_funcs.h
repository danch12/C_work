#ifndef STACK_FUNCS_H
#define STACK_FUNCS_H

#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <string.h>
#include <assert.h>

#include "../general.h"

#define MAXELELEN 50


typedef struct node
{
   /*can store it as a number
   because we dont actually put
   operators on the stack*/
	double num;
  struct node* next;
}node;

typedef node* nodeptr;

typedef struct stack
{
  nodeptr start;
  int size;
}stack;

/*return void then cast when appropriate*/
stack* stack_init(void);
nodeptr init_node(double d);
void stack_push(stack* s,double d);
bool stack_pop(stack* s, double* new_d);
bool stack_free(stack* s);
void stack_tostr(stack* s, char* str);
bool stack_peek(stack* s, double* new_d);

#endif
