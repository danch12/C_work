CFLAGSFAST= -Wall -Wextra -Wfloat-equal -pedantic -std=c90 -O3

CFLAGSSAN= -Wall -Wextra -Wfloat-equal -pedantic -std=c90 -fsanitize=undefined -fsanitize=address -g3
CFLAGSSDL= `sdl2-config --cflags` -O2 -Wall -Wextra -pedantic -Wfloat-equal -std=c90
LIDLIBSSDL= `sdl2-config --libs` -lm
LIDLIBS = -lm
CC =/usr/local/bin/gcc-10
INCPARSE = -Iparse
INCINTERP = -Iinterpreter
INCEXTEN = -Iexten

GENERALFULL = ADTS_and_general/general.c ADTS_and_general/general.h
GENERALRUN = ADTS_and_general/general.c
STACKSFULL = ADTS_and_general/stacks/stack_funcs.c ADTS_and_general/stacks/stack_funcs.h
STACKSRUN = ADTS_and_general/stacks/stack_funcs.c
HASHFULL = ADTS_and_general/hash_map/hash_funcs.c ADTS_and_general/hash_map/hash_funcs.h
HASHRUN = ADTS_and_general/hash_map/hash_funcs.c
ARRFULL= ADTS_and_general/arrays/array_funcs.h ADTS_and_general/arrays/array_funcs.c
ARRRUN = ADTS_and_general/arrays/array_funcs.c
INTERPFULL = interpreter_funcs.c  interpreter_funcs.h
INTERPRUN = interpreter_funcs.c
PARSEFULL = parser_funcs.c parser_funcs.h
PARSERUN = parser_funcs.c



u_stacks_san: unity_tests/Test_stacks.c  src/unity.c  src/unity.h $(STACKSFULL) $(GENERALFULL)
	$(CC) unity_tests/Test_stacks.c src/unity.c $(STACKSRUN) $(GENERALRUN) -o u_stacks_san $(CFLAGSSAN) $(LIDLIBS)

u_stacks_fast: unity_tests/Test_stacks.c  src/unity.c  src/unity.h $(STACKSFULL) $(GENERALFULL)
	$(CC) unity_tests/Test_stacks.c src/unity.c $(STACKSRUN) $(GENERALRUN) -o u_stacks_fast $(CFLAGSFAST) $(LIDLIBS)


u_hash_map_san: unity_tests/Test_hashmap.c src/unity.c src/unity.h $(HASHFULL) $(GENERALFULL)
	$(CC) unity_tests/Test_hashmap.c src/unity.c $(HASHRUN) $(GENERALRUN) -o u_hash_map_san $(CFLAGSSAN) $(LIDLIBS)

u_hash_map_fast: unity_tests/Test_hashmap.c src/unity.c src/unity.h $(HASHFULL) $(GENERALFULL)
	$(CC) unity_tests/Test_hashmap.c src/unity.c $(HASHRUN) $(GENERALRUN) -o u_hash_map_fast $(CFLAGSFAST) $(LIDLIBS)

u_arrays_san: unity_tests/Test_arrays.c src/unity.c src/unity.h $(ARRFULL) $(GENERALFULL)
	$(CC) unity_tests/Test_arrays.c  src/unity.c $(ARRRUN) $(GENERALRUN) -o u_arrays_san $(CFLAGSSAN) $(LIDLIBS)

u_arrays_fast: unity_tests/Test_arrays.c src/unity.c src/unity.h $(ARRFULL) $(GENERALFULL)
	$(CC) unity_tests/Test_arrays.c  src/unity.c $(ARRRUN) $(GENERALRUN) -o u_arrays_fast $(CFLAGSFAST) $(LIDLIBS)

u_parse_san: unity_tests/Test_parser.c src/unity.c src/unity.h $(PARSEFULL) parse/specific.h parse/specific.c $(GENERALFULL)
	$(CC) unity_tests/Test_parser.c src/unity.c $(PARSERUN) parse/specific.c $(GENERALRUN) -o u_parse_san $(CFLAGSSAN) $(INCPARSE) $(LIDLIBS)

u_parse_fast: unity_tests/Test_parser.c src/unity.c src/unity.h $(PARSEFULL) parse/specific.h parse/specific.c $(GENERALFULL)
	$(CC) unity_tests/Test_parser.c src/unity.c $(PARSERUN) parse/specific.c $(GENERALRUN) -o u_parse_fast $(CFLAGSFAST) $(INCPARSE) $(LIDLIBS)

parser_san: parser.c $(PARSEFULL) $(GENERALFULL) parse/specific.h parse/specific.c
	$(CC) parser.c $(PARSERUN) $(GENERALRUN) parse/specific.c  -o parser_san $(CFLAGSSAN) $(INCPARSE) $(LIDLIBS)

parser_fast: parser.c $(PARSEFULL) $(GENERALFULL) parse/specific.h parse/specific.c
	$(CC) parser.c $(PARSERUN) $(GENERALRUN) parse/specific.c  -o parser_fast $(CFLAGSFAST) $(INCPARSE) $(LIDLIBS)


u_test_interp_san: unity_tests/Test_interp.c src/unity.c src/unity.h $(PARSEFULL) $(INTERPFULL) interpreter/specific.c interpreter/specific.h $(STACKSFULL) $(GENERALFULL)
		$(CC) unity_tests/Test_interp.c src/unity.c interpreter/specific.c  $(PARSERUN) $(INTERPRUN) $(STACKSRUN) $(GENERALRUN) -o u_test_interp_san $(CFLAGSSAN) $(INCINTERP) $(LIDLIBS)

u_test_interp_fast: unity_tests/Test_interp.c src/unity.c src/unity.h $(PARSEFULL) $(INTERPFULL) interpreter/specific.c interpreter/specific.h $(STACKSFULL) $(GENERALFULL)
		$(CC) unity_tests/Test_interp.c src/unity.c interpreter/specific.c  $(PARSERUN) $(INTERPRUN) $(STACKSRUN) $(GENERALRUN) -o u_test_interp_fast $(CFLAGSFAST) $(INCINTERP) $(LIDLIBS)




u_exten_func_san:unity_tests/Test_extension_funcs.c src/unity.c src/unity.h extension_funcs.c extension_flow.c extension_flow.h  extension_funcs.h $(PARSEFULL) $(INTERPFULL) exten/specific.c exten/specific.h $(STACKSFULL) $(GENERALFULL) $(HASHFULL) $(ARRFULL)
	$(CC) unity_tests/Test_extension_funcs.c src/unity.c extension_funcs.c extension_flow.c  exten/specific.c $(PARSERUN) $(INTERPRUN)  $(STACKSRUN) $(GENERALRUN) $(HASHRUN) $(ARRRUN)  -o u_exten_func_san $(CFLAGSSAN) $(INCEXTEN) $(LIDLIBS)

u_exten_func_fast:unity_tests/Test_extension_funcs.c src/unity.c src/unity.h extension_funcs.c extension_flow.c extension_flow.h  extension_funcs.h $(PARSEFULL) $(INTERPFULL) exten/specific.c exten/specific.h $(STACKSFULL) $(GENERALFULL) $(HASHFULL) $(ARRFULL)
	$(CC) unity_tests/Test_extension_funcs.c src/unity.c extension_funcs.c extension_flow.c  exten/specific.c $(PARSERUN) $(INTERPRUN)  $(STACKSRUN) $(GENERALRUN) $(HASHRUN) $(ARRRUN)  -o u_exten_func_fast $(CFLAGSFAST) $(INCEXTEN) $(LIDLIBS)


u_exten_flow_san: unity_tests/Test_flow.c src/unity.c src/unity.h extension_flow.c extension_flow.h extension_funcs.c  extension_funcs.h $(PARSEFULL) $(INTERPFULL) exten/specific.c exten/specific.h $(STACKSFULL) $(GENERALFULL) $(HASHFULL) $(ARRFULL)
	$(CC) unity_tests/Test_flow.c src/unity.c extension_flow.c extension_funcs.c exten/specific.c $(PARSERUN) $(INTERPRUN)  $(STACKSRUN) $(GENERALRUN) $(HASHRUN) $(ARRRUN)  -o u_exten_flow_san $(CFLAGSSAN) $(INCEXTEN) $(LIDLIBS)

u_exten_flow_fast: unity_tests/Test_flow.c src/unity.c src/unity.h extension_flow.c extension_flow.h extension_funcs.c  extension_funcs.h $(PARSEFULL) $(INTERPFULL) exten/specific.c exten/specific.h $(STACKSFULL) $(GENERALFULL) $(HASHFULL) $(ARRFULL)
	$(CC) unity_tests/Test_flow.c src/unity.c extension_flow.c extension_funcs.c exten/specific.c $(PARSERUN) $(INTERPRUN)  $(STACKSRUN) $(GENERALRUN) $(HASHRUN) $(ARRRUN)  -o u_exten_flow_fast $(CFLAGSFAST) $(INCEXTEN) $(LIDLIBS)

u_exten_func_flow_san: unity_tests/Test_func_flow.c src/unity.c src/unity.h extension_funcs.c  extension_funcs.h extension_flow.c extension_flow.h $(PARSEFULL) $(INTERPFULL) exten/specific.c exten/specific.h $(STACKSFULL) $(GENERALFULL) $(HASHFULL) $(ARRFULL)
	$(CC) unity_tests/Test_func_flow.c src/unity.c extension_funcs.c extension_flow.c exten/specific.c $(PARSERUN) $(INTERPRUN)  $(STACKSRUN) $(GENERALRUN) $(HASHRUN) $(ARRRUN)  -o u_exten_func_flow_san $(CFLAGSSAN) $(INCEXTEN) $(LIDLIBS)

u_exten_func_flow_fast: unity_tests/Test_func_flow.c src/unity.c src/unity.h extension_funcs.c  extension_funcs.h extension_flow.c extension_flow.h $(PARSEFULL) $(INTERPFULL) exten/specific.c exten/specific.h $(STACKSFULL) $(GENERALFULL) $(HASHFULL) $(ARRFULL)
	$(CC) unity_tests/Test_func_flow.c src/unity.c extension_funcs.c extension_flow.c exten/specific.c $(PARSERUN) $(INTERPRUN)  $(STACKSRUN) $(GENERALRUN) $(HASHRUN) $(ARRRUN)  -o u_exten_func_flow_fast $(CFLAGSFAST) $(INCEXTEN) $(LIDLIBS)

exten_arr_san: extension_arrays.c  exten/specific.c exten/specific.h  extension_funcs.c  extension_funcs.h extension_flow.c extension_flow.h $(PARSEFULL) $(INTERPFULL) $(STACKSFULL) $(GENERALFULL) $(HASHFULL) $(ARRFULL)
	$(CC) extension_arrays.c  exten/specific.c extension_funcs.c extension_flow.c  $(PARSERUN) $(INTERPRUN)  $(STACKSRUN) $(GENERALRUN) $(HASHRUN) $(ARRRUN) -o exten_arr_san $(CFLAGSSAN) $(INCEXTEN) $(LIDLIBS)

u_exten_arr_san: unity_tests/Test_extension_arrays.c src/unity.c src/unity.h  extension_arrays.c extension_arrays.h  exten/specific.c exten/specific.h  extension_funcs.c  extension_funcs.h extension_flow.c extension_flow.h $(PARSEFULL) $(INTERPFULL) $(STACKSFULL) $(GENERALFULL) $(HASHFULL) $(ARRFULL)
	$(CC) unity_tests/Test_extension_arrays.c src/unity.c extension_arrays.c  exten/specific.c extension_funcs.c extension_flow.c  $(PARSERUN) $(INTERPRUN)  $(STACKSRUN) $(GENERALRUN) $(HASHRUN) $(ARRRUN) -o u_exten_arr_san $(CFLAGSSAN) $(INCEXTEN) $(LIDLIBS)



extension: extension.c extension_funcs.c  extension_funcs.h extension_flow.c extension_flow.h $(PARSEFULL) $(INTERPFULL) exten/specific.c exten/specific.h $(STACKSFULL) $(GENERALFULL) $(HASHFULL) ADTS_and_general/neillsdl2.c ADTS_and_general/neillsdl2.h
	$(CC) extension.c extension_funcs.c extension_flow.c exten/specific.c $(PARSERUN) $(INTERPRUN)  $(STACKSRUN) $(GENERALRUN) $(HASHRUN) ADTS_and_general/neillsdl2.c -o extension $(CFLAGSSDL) $(INCEXTEN) $(LIDLIBSSDL)

interp: interpreter.c $(PARSEFULL) $(INTERPFULL) interpreter/specific.c interpreter/specific.h  ADTS_and_general/neillsdl2.c ADTS_and_general/neillsdl2.h  $(STACKSFULL) $(GENERALFULL)
	$(CC) interpreter.c $(PARSERUN) $(INTERPRUN)  interpreter/specific.c   ADTS_and_general/neillsdl2.c $(STACKSRUN) $(GENERALRUN) -o interp $(CFLAGSSDL) $(INCINTERP) $(LIDLIBSSDL)


run_tests: test_interp_san test_interp_fast hash_map_san hash_map_fast test_stacks_san test_stacks_fast test_parser_san test_parser_fast exten_func_fast exten_func_san exten_flow_san exten_flow_fast exten_func_flow_san exten_func_flow_fast arrays_san arrays_fast
	./parser_test_fast
	./parser_test_san
	./test_stacks_fast
	./test_stacks_san
	./hash_map_fast
	./hash_map_san
	./test_interp_san
	./test_interp_fast
	./exten_func_fast
	./exten_func_san
	./exten_flow_fast
	./exten_flow_san
	./exten_func_flow_san
	./exten_func_flow_fast
	./arrays_san
	./arrays_fast


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
	./interp test_files/test_turtles/interpreter/invalid/no_nums.txt 2>> i_errors.txt | true
	echo "test_files/test_turtles/interpreter/invalid/too_many_nums.txt" >> i_errors.txt
	./interp test_files/test_turtles/interpreter/invalid/too_many_nums.txt 2>> i_errors.txt | true
	echo "test_files/test_turtles/interpreter/invalid/unset_var.txt" >> i_errors.txt
	./interp test_files/test_turtles/interpreter/invalid/unset_var.txt 2>> i_errors.txt | true
	 echo "test_files/test_turtles/parser/invalid/invalid_nested_loop.txt" >> i_errors.txt
	./interp test_files/test_turtles/parser/invalid/invalid_nested_loop.txt 2>> i_errors.txt | true
	echo "test_files/test_turtles/parser/invalid/invalid_set.txt" >> i_errors.txt
	./interp test_files/test_turtles/parser/invalid/invalid_set.txt 2>> i_errors.txt | true

run_extension_err:extension
	echo "test_files/test_turtles/extension/invalid/no_arguments.txt" > ext_errors.txt
	./extension test_files/test_turtles/extension/invalid/no_arguments.txt 2>> ext_errors.txt | true
	echo "test_files/test_turtles/extension/invalid/too_many_args.txt" >> ext_errors.txt
	./extension test_files/test_turtles/extension/invalid/too_many_args.txt 2>> ext_errors.txt | true
	echo "test_files/test_turtles/extension/invalid/unknown_func.txt" >> ext_errors.txt
	./extension test_files/test_turtles/extension/invalid/unknown_func.txt 2>> ext_errors.txt | true
	echo "test_files/test_turtles/extension/invalid/same_var.txt" >> ext_errors.txt
	./extension test_files/test_turtles/extension/invalid/same_var.txt 2>> ext_errors.txt | true



clean:
	rm -f parser_test_fast parser_test_san test_stacks_fast test_stacks_san  parser_san test_interp_san test_interp_fast hash_map_san hash_map_fast interp extension exten_test_fast exten_test_san parser_fast exten_func_flow_san exten_func_flow_fast arrays_san arrays_fast
