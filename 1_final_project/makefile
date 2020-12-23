CFLAGSFAST= -Wall -Wextra -Wfloat-equal -pedantic -std=c90 -O3
CFLAGSSAN= -Wall -Wextra -Wfloat-equal -pedantic -std=c90 -fsanitize=undefined -fsanitize=address -g3
CFLAGSSDL= `sdl2-config --cflags` -O2 -Wall -Wextra -pedantic -Wfloat-equal -std=c90
LIDLIBSSDL= `sdl2-config --libs` -lm
LIDLIBS = -lm
CC =/usr/local/bin/gcc-10 
#INCBASIC = -Ibasic
#INCEXT = -Iextension



test_parser_fast: parser_funcs.h parser_tests.c parser_funcs.c stack_funcs.c stack_funcs.h  general.c general.h
	$(CC) parser_funcs.c parser_tests.c stack_funcs.c general.c  -o parser_test_fast $(CFLAGSFAST)  $(LIDLIBS)

test_parser_san: parser_tests.c parser_funcs.c parser_funcs.h stack_funcs.c stack_funcs.h  general.c general.h
	$(CC) parser_tests.c parser_funcs.c stack_funcs.c general.c -o parser_test_san $(CFLAGSSAN) $(LIDLIBS)

test_stacks_fast: stack_tests.c stack_funcs.c stack_funcs.h general.c general.h
	$(CC) stack_tests.c stack_funcs.c general.c  -o test_stacks_fast $(CFLAGSSAN) $(LIDLIBS)

test_stacks_san: stack_tests.c stack_funcs.c stack_funcs.h general.c general.h
	 $(CC) stack_tests.c stack_funcs.c general.c -o test_stacks_san $(CFLAGSSAN) $(LIDLIBS)

parser_san: parser.c parser_funcs.c parser_funcs.h general.c general.h stack_funcs.c stack_funcs.h
	$(CC) parser.c parser_funcs.c stack_funcs.c general.c  -o parser_san $(CFLAGSSAN) $(LIDLIBS)

parser_fast: parser.c parser_funcs.c parser_funcs.h general.c general.h stack_funcs.c stack_funcs.h
	$(CC) parser.c parser_funcs.c stack_funcs.c general.c  -o parser_fast $(CFLAGSFAST) $(LIDLIBS)


test_interp_san : interpreter_tests.c interpreter_funcs.c interpreter_funcs.h parser_funcs.c parser_funcs.h general.c general.h stack_funcs.c stack_funcs.h
	$(CC) interpreter_tests.c interpreter_funcs.c parser_funcs.c general.c stack_funcs.c -o test_interp_san $(CFLAGSSAN) $(LIDLIBS) 

test_interp_fast : interpreter_tests.c interpreter_funcs.c interpreter_funcs.h parser_funcs.c parser_funcs.h general.c general.h stack_funcs.c stack_funcs.h
	$(CC) interpreter_tests.c interpreter_funcs.c parser_funcs.c general.c stack_funcs.c -o test_interp_fast $(CFLAGSFAST) $(LIDLIBS)


interp_san: interpreter.c parser_funcs.c parser_funcs.h stack_funcs.c stack_funcs.h general.c general.h
	$(CC) interpreter.c parser_funcs.c stack_funcs.c  general.c  -o interp_san $(CFLAGSSAN) $(LIDLIBS)

interp_sdl: interpreter.c parser_funcs.c parser_funcs.h neillsdl2.c neillsdl2.h  stack_funcs.c stack_funcs.h general.c general.h
	$(CC) interpreter.c parser_funcs.c neillsdl2.c stack_funcs.c  general.c  -o interp_sdl $(CFLAGSSDL) $(LIDLIBSSDL)


#runs a series of invalid files and stores error message in p_errors.txt the | true stops the makefile from terminating after seeing an error
run_parser_err: parser_fast
	echo "test_files/test_turtles/parser/invalid/bad_spacing.txt" > p_errors.txt
	./parser_fast test_files/test_turtles/parser/invalid/bad_spacing.txt 2>> p_errors.txt | true
	echo "test_files/test_turtles/parser/invalid/invalid_command.txt" >> p_errors.txt	
	./parser_fast test_files/test_turtles/parser/invalid/invalid_command.txt 2>>p_errors.txt | true	
	echo "test_files/test_turtles/parser/invalid/missing_bracket.txt" >> p_errors.txt
	./parser_fast test_files/test_turtles/parser/invalid/missing_bracket.txt 2>> p_errors.txt |true
	echo "test_files/test_turtles/parser/invalid/invalid_do.txt" >> p_errors.txt
	./parser_fast test_files/test_turtles/parser/invalid/invalid_do.txt 2>> p_errors.txt | true
	echo "test_files/test_turtles/parser/invalid/invalid_nested_loop.txt" >> p_errors.txt
	./parser_fast test_files/test_turtles/parser/invalid/invalid_nested_loop.txt 2>> p_errors.txt | true
	echo "test_files/test_turtles/parser/invalid/invalid_set.txt" >> p_errors.txt
	./parser_fast test_files/test_turtles/parser/invalid/invalid_set.txt 2>> p_errors.txt | true
	echo "test_files/test_turtles/parser/invalid/invalid_set_2.txt" >> p_errors.txt
	./parser_fast test_files/test_turtles/parser/invalid/invalid_set_2.txt 2>> p_errors.txt 

run_parser_val: parser_fast
	./parser_fast test_files/test_turtles/parser/valid/adding_loops.txt
	./parser_fast test_files/test_turtles/parser/valid/more_do.txt
	./parser_fast test_files/test_turtles/parser/valid/nested_loops.txt
	./parser_fast test_files/test_turtles/parser/valid/polish_2.txt
	./parser_fast test_files/test_turtles/parser/valid/polish.txt
	./parser_fast test_files/test_turtles/parser/valid/stupid_valid_do.txt
	./parser_fast test_files/test_turtles/parser/valid/using_variables.txt


clean:
	rm -f parser_test_fast parser_test_san test_stacks_fast test_stacks_san  parser_san test_interp_san test_interp_fast 
