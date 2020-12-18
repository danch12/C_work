## Parsing and Grammars

Infix notation is x+y rather than x y +

as you scan a traditional infix expression such as 

A+B/C+D

 from left to right it is impossible to tell when you initially encounter the + sign whether or not you should apply the indicated operation to A

you must probe deeper into the expression to determine whether an operation with a higher priority occurs.

this gets v complicated especially when you start adding in brackets

this is why we use reverse polish notation

## implementing reverse polish 

reverse polish may be evaluated using a stack 

we examine the next character 

if it is a number or variable we push it onto the stack

if it is an operator we pop off the top two items perform the operation and push the result

if we have reached the end the answer is the one and only item on the stack- else repeat



```c
char input[MAXINPUT]
stack* s;
stacktype d, g1, g2;

s=stack_init();
while(fgets(input,MAXINPUT,stdin))
{
  /*if number push*/
  if(sscanf(input,FORMATSTR,&d)==1)
  {
    stack_push(s,d);
  }
  else
  {
    /*must be a operator
    take off g2 first then g1*/
    assert(stack_pop(s,&g2));
    assert(stack_pop(s,&g1));
    switch(input[0])
    {
      case '+':
        d= g1+g2;
        break;
      case '-':
        d=g1-g2;
        break;
      case '*':
        d=g1*g2;
        break;
      case '/':
        d=g1/g2;
        break;
      default:
      	fprintf(stderr,"Cant understand\n");
        exit(EXIT_FAILURE)
    }
    stack_push(s,&d);
  }
  assert(stack_pop(s,&d));
  if(stack_peek(s,&d)==true)
  {
    fprintf(stderr,"stack still has things on it\n");
    exit(EXIT_FAILURE);
  }
  stack_free(s);
  return 0;
}
```



## Formal Grammars 

context free grammers dont have to make sense as long as they follow the rules



In the noughts and 1s example the end statement is actually used as the recursive base case in the formal grammar

parser doesnt actually do anything other than check that the input has the correct syntax

