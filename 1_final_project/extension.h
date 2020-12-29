#ifndef EXTENSION_H
#define EXTENSION_H
#include "specific.h"
#include "parser_funcs.h"
#include "interpreter_funcs.h"

#include "../ADTS_and_general/hash_map/hash_funcs.h"


#define MAXFUNCLEN 50
#define INITFUNCCAP 20
#define FUNCSCALE 2
#define UNUSED -1
#define START 0
#define START_BRACKET 1
/*we know that there is max 26 arguments
so when we define the func we add a number
into arg placer that tells us which var was
used in that position eg if the first argument
is a C we put a 2 in the first position of arg placer
then when we run the function and an argument is used
in the first position we can use the arg placer array
to place the argument into the C spot in the var array
and run the code from there

https://tenthousandmeters.com/blog/python-behind-the-scenes-1-how-the-cpython-vm-works/
had a look at this for a bit of inspiration but definitely
nowhere near as advanced

*/




 #endif
