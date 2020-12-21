CFLAGSFAST= -Wall -Wextra -Wfloat-equal -pedantic -std=c90 -O3
CFLAGSSAN= -Wall -Wextra -Wfloat-equal -pedantic -std=c90 -fsanitize=undefined -fsanitize=address -g3
CFLAGSSDL= `sdl2-config --cflags` -02 -Wall -Wextra -pedantic -Wfloat-equal -std=90
LIDLIBSSDL= `sdl2-config --libs` -lm
LIDLIBS = -lm
CC =/usr/local/bin/gcc-10 
#INCBASIC = -Ibasic
#INCEXT = -Iextension



test_parser_fast: parser_funcs.h parser_tests.c parser_funcs.c general.c general.h
	$(CC) parser_funcs.c parser_tests.c general.c  -o parser_test_fast $(CFLAGSFAST)  $(LIDLIBS)

test_parser_san: parser_tests.c parser_funcs.c parser_funcs.h general.c general.h
	$(CC) parser_tests.c parser_funcs.c general.c -o parser_test_san $(CFLAGSSAN) $(LIDLIBS)

test_stacks_fast: stack_tests.c stack_funcs.c stack_funcs.h general.c general.h
	$(CC) stack_tests.c stack_funcs.c general.c  -o test_stacks_fast $(CFLAGSSAN) $(LIDLIBS)

test_stacks_san: stack_tests.c stack_funcs.c stack_funcs.h general.c general.h
	 $(CC) stack_tests.c stack_funcs.c general.c -o test_stacks_san $(CFLAGSSAN) $(LIDLIBS)

parser_san: parser.c parser_funcs.c parser_funcs.h general.c general.h
	$(CC) parser.c parser_funcs.c general.c  -o parser_san $(CFLAGSSAN) $(LIDLIBS)

interp_san: interpreter.c stack_funcs.c stack_funcs.h general.c general.h
	$(CC) interpreter.c stack_funcs.c  general.c  -o interp_san $(CFLAGSSAN) $(LIDLIBS)

clean:
	rm -f parser_test_fast parser_test_san test_stacks_fast test_stacks_san  parser_san 
