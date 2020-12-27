CFLAGSFAST= -Wall -Wextra -Wfloat-equal -pedantic -std=c90 -O3

CFLAGSSAN= -Wall -Wextra -Wfloat-equal -pedantic -std=c90 -fsanitize=undefined -fsanitize=address -g3
CFLAGSSDL= `sdl2-config --cflags` -O2 -Wall -Wextra -pedantic -Wfloat-equal -std=c90
LIDLIBSSDL= `sdl2-config --libs` -lm
LIDLIBS = -lm
CC =/usr/local/bin/gcc-10
INCPARSE = -Iparse
INCINTERP = -Iinterpreter

test_parser_fast: parser_funcs.h parser_tests.c parser_funcs.c parse/specific.h parse/specific.c ADTS_and_general/general.c ADTS_and_general/general.h
	$(CC) parser_funcs.c parser_tests.c parse/specific.c ADTS_and_general/general.c  -o parser_test_fast $(CFLAGSFAST) $(INCPARSE) $(LIDLIBS)

test_parser_san: parser_funcs.h parser_tests.c parser_funcs.c parse/specific.h parse/specific.c ADTS_and_general/general.c ADTS_and_general/general.h
	$(CC) parser_funcs.c parser_tests.c parse/specific.c ADTS_and_general/general.c  -o parser_test_san $(CFLAGSSAN) $(INCPARSE) $(LIDLIBS)

test_stacks_fast: ADTS_and_general/stacks/stack_tests.c ADTS_and_general/stacks/stack_funcs.c ADTS_and_general/stacks/stack_funcs.h ADTS_and_general/general.c  ADTS_and_general/general.h
	$(CC) ADTS_and_general/stacks/stack_tests.c ADTS_and_general/stacks/stack_funcs.c ADTS_and_general/general.c  -o test_stacks_fast $(CFLAGSFAST) $(LIDLIBS)

test_stacks_san: ADTS_and_general/stacks/stack_tests.c ADTS_and_general/stacks/stack_funcs.c ADTS_and_general/stacks/stack_funcs.h ADTS_and_general/general.c  ADTS_and_general/general.h
	 $(CC) ADTS_and_general/stacks/stack_tests.c ADTS_and_general/stacks/stack_funcs.c ADTS_and_general/general.c -o test_stacks_san $(CFLAGSSAN) $(LIDLIBS)

hash_map_fast: ADTS_and_general/hash_map/hash_map_tests.c ADTS_and_general/hash_map/hash_funcs.c ADTS_and_general/hash_map/hash_funcs.h extension.h parser_funcs.c parser_funcs.h ADTS_and_general/general.c ADTS_and_general/general.h ADTS_and_general/stacks/stack_funcs.c ADTS_and_general/stacks/stack_funcs.h interpreter/specific.c interpreter/specific.h
	$(CC) ADTS_and_general/hash_map/hash_map_tests.c ADTS_and_general/hash_map/hash_funcs.c parser_funcs.c ADTS_and_general/general.c ADTS_and_general/stacks/stack_funcs.c interpreter/specific.c -o hash_map_fast $(CFLAGSFAST) $(INCINTERP) $(LIDLIBS)

hash_map_san: ADTS_and_general/hash_map/hash_map_tests.c ADTS_and_general/hash_map/hash_funcs.c ADTS_and_general/hash_map/hash_funcs.h extension.h parser_funcs.c parser_funcs.h ADTS_and_general/general.c ADTS_and_general/general.h ADTS_and_general/stacks/stack_funcs.c ADTS_and_general/stacks/stack_funcs.h interpreter/specific.c interpreter/specific.h
	$(CC) ADTS_and_general/hash_map/hash_map_tests.c ADTS_and_general/hash_map/hash_funcs.c parser_funcs.c ADTS_and_general/general.c ADTS_and_general/stacks/stack_funcs.c interpreter/specific.c -o hash_map_san $(CFLAGSSAN) $(INCINTERP) $(LIDLIBS)

parser_san: parser.c parser_funcs.c parser_funcs.h ADTS_and_general/general.c ADTS_and_general/general.h ADTS_and_general/stacks/stack_funcs.c ADTS_and_general/stacks/stack_funcs.h parse/specific.h parse/specific.c
	$(CC) parser.c parser_funcs.c ADTS_and_general/stacks/stack_funcs.c ADTS_and_general/general.c parse/specific.c  -o parser_san $(CFLAGSSAN) $(INCPARSE) $(LIDLIBS)

parser_fast: parser.c parser_funcs.c parser_funcs.h ADTS_and_general/general.c ADTS_and_general/general.h ADTS_and_general/stacks/stack_funcs.c ADTS_and_general/stacks/stack_funcs.h parse/specific.h parse/specific.c
	$(CC) parser.c parser_funcs.c ADTS_and_general/stacks/stack_funcs.c ADTS_and_general/general.c parse/specific.c  -o parser_fast $(CFLAGSFAST) $(INCPARSE) $(LIDLIBS)


test_interp_san : interpreter_tests.c interpreter_funcs.c interpreter/specific.c interpreter/specific.h interpreter_funcs.h parser_funcs.c parser_funcs.h ADTS_and_general/general.c ADTS_and_general/general.h ADTS_and_general/stacks/stack_funcs.c ADTS_and_general/stacks/stack_funcs.h
	$(CC) interpreter_tests.c interpreter_funcs.c interpreter/specific.c parser_funcs.c ADTS_and_general/general.c ADTS_and_general/stacks/stack_funcs.c  -o test_interp_san $(CFLAGSSAN) $(INCINTERP) $(LIDLIBS)

test_interp_fast : interpreter_tests.c interpreter_funcs.c interpreter/specific.c interpreter/specific.h interpreter_funcs.h parser_funcs.c parser_funcs.h ADTS_and_general/general.c ADTS_and_general/general.h ADTS_and_general/stacks/stack_funcs.c ADTS_and_general/stacks/stack_funcs.h
	$(CC) interpreter_tests.c interpreter_funcs.c interpreter/specific.c parser_funcs.c ADTS_and_general/general.c ADTS_and_general/stacks/stack_funcs.c  -o test_interp_fast $(CFLAGSFAST) $(INCINTERP) $(LIDLIBS)



interp: interpreter.c parser_funcs.c parser_funcs.h interpreter_funcs.c interpreter_funcs.h interpreter/specific.c interpreter/specific.h  ADTS_and_general/neillsdl2.c ADTS_and_general/neillsdl2.h  ADTS_and_general/stacks/stack_funcs.c ADTS_and_general/stacks/stack_funcs.h ADTS_and_general/general.c ADTS_and_general/general.h
	$(CC) interpreter.c parser_funcs.c interpreter_funcs.c interpreter/specific.c   ADTS_and_general/neillsdl2.c ADTS_and_general/stacks/stack_funcs.c  ADTS_and_general/general.c  -o interp $(CFLAGSSDL) $(INCINTERP) $(LIDLIBSSDL)



run_tests: test_interp_san test_interp_fast hash_map_san hash_map_fast test_stacks_san test_stacks_fast test_parser_san test_parser_fast
	./parser_test_fast
	./parser_test_san
	./test_stacks_fast
	./test_stacks_san
	./hash_map_fast
	./hash_map_san
	./test_interp_san
	./test_interp_fast


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

run_interp_err: interp
	echo "test_files/test_turtles/interpreter/invalid/no_nums.txt" > i_errors.txt
	./interp test_files/test_turtles/interpreter/invalid/no_nums.txt 2>> i_errors.txt |true
	echo "test_files/test_turtles/interpreter/invalid/too_many_nums.txt" >> i_errors.txt
	./interp test_files/test_turtles/interpreter/invalid/too_many_nums.txt 2>> i_errors.txt | true
	echo "test_files/test_turtles/interpreter/invalid/unset_var.txt" >> i_errors.txt
	./interp test_files/test_turtles/interpreter/invalid/unset_var.txt 2>> i_errors.txt | true
	 echo "test_files/test_turtles/parser/invalid/invalid_nested_loop.txt" >> i_errors.txt
	./interp test_files/test_turtles/parser/invalid/invalid_nested_loop.txt 2>> i_errors.txt | true
	echo "test_files/test_turtles/parser/invalid/invalid_set.txt" >> i_errors.txt
	./interp test_files/test_turtles/parser/invalid/invalid_set.txt 2>> i_errors.txt | true

clean:
	rm -f parser_test_fast parser_test_san test_stacks_fast test_stacks_san  parser_san test_interp_san test_interp_fast hash_map_san hash_map_fast interp
