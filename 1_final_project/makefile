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

u_test_interp_san: unity_tests/Test_interp.c src/unity.c src/unity.h $(PARSEFULL) $(INTERPFULL) interpreter/specific.c interpreter/specific.h $(STACKSFULL) $(GENERALFULL)
		$(CC) unity_tests/Test_interp.c src/unity.c interpreter/specific.c  $(PARSERUN) $(INTERPRUN) $(STACKSRUN) $(GENERALRUN) -o u_test_interp_san $(CFLAGSSAN) $(INCINTERP) $(LIDLIBS)

u_test_interp_fast: unity_tests/Test_interp.c src/unity.c src/unity.h $(PARSEFULL) $(INTERPFULL) interpreter/specific.c interpreter/specific.h $(STACKSFULL) $(GENERALFULL)
		$(CC) unity_tests/Test_interp.c src/unity.c interpreter/specific.c  $(PARSERUN) $(INTERPRUN) $(STACKSRUN) $(GENERALRUN) -o u_test_interp_fast $(CFLAGSFAST) $(INCINTERP) $(LIDLIBS)

u_exten_func_san:unity_tests/Test_extension_funcs.c src/unity.c src/unity.h extension_funcs.c extension_flow.c extension_flow.h  extension_funcs.h extension_arrays.c extension_arrays.h  $(PARSEFULL) $(INTERPFULL) exten/specific.c exten/specific.h $(STACKSFULL) $(GENERALFULL) $(HASHFULL) $(ARRFULL)
	$(CC) unity_tests/Test_extension_funcs.c src/unity.c extension_funcs.c extension_flow.c extension_arrays.c  exten/specific.c $(PARSERUN) $(INTERPRUN)  $(STACKSRUN) $(GENERALRUN) $(HASHRUN) $(ARRRUN)  -o u_exten_func_san $(CFLAGSSAN) $(INCEXTEN) $(LIDLIBS)

u_exten_func_fast:unity_tests/Test_extension_funcs.c src/unity.c src/unity.h extension_funcs.c extension_flow.c extension_flow.h  extension_funcs.h extension_arrays.c extension_arrays.h  $(PARSEFULL) $(INTERPFULL) exten/specific.c exten/specific.h $(STACKSFULL) $(GENERALFULL) $(HASHFULL) $(ARRFULL)
	$(CC) unity_tests/Test_extension_funcs.c src/unity.c extension_funcs.c extension_flow.c extension_arrays.c  exten/specific.c $(PARSERUN) $(INTERPRUN)  $(STACKSRUN) $(GENERALRUN) $(HASHRUN) $(ARRRUN)  -o u_exten_func_fast $(CFLAGSFAST) $(INCEXTEN) $(LIDLIBS)


u_exten_flow_san: unity_tests/Test_flow.c src/unity.c src/unity.h extension_flow.c extension_flow.h extension_funcs.c  extension_funcs.h extension_arrays.c extension_arrays.h  $(PARSEFULL) $(INTERPFULL) exten/specific.c exten/specific.h $(STACKSFULL) $(GENERALFULL) $(HASHFULL) $(ARRFULL)
	$(CC) unity_tests/Test_flow.c src/unity.c extension_flow.c extension_funcs.c exten/specific.c extension_arrays.c $(PARSERUN) $(INTERPRUN)  $(STACKSRUN) $(GENERALRUN) $(HASHRUN) $(ARRRUN)  -o u_exten_flow_san $(CFLAGSSAN) $(INCEXTEN) $(LIDLIBS)

u_exten_flow_fast: unity_tests/Test_flow.c src/unity.c src/unity.h extension_flow.c extension_flow.h extension_funcs.c  extension_funcs.h extension_arrays.c extension_arrays.h $(PARSEFULL) $(INTERPFULL) exten/specific.c exten/specific.h $(STACKSFULL) $(GENERALFULL) $(HASHFULL) $(ARRFULL)
	$(CC) unity_tests/Test_flow.c src/unity.c extension_flow.c extension_funcs.c exten/specific.c extension_arrays.c $(PARSERUN) $(INTERPRUN)  $(STACKSRUN) $(GENERALRUN) $(HASHRUN) $(ARRRUN)  -o u_exten_flow_fast $(CFLAGSFAST) $(INCEXTEN) $(LIDLIBS)

u_exten_interactions_san: unity_tests/Test_exten_interactions.c src/unity.c src/unity.h extension_funcs.c  extension_funcs.h extension_flow.c extension_flow.h  extension_arrays.c extension_arrays.h $(PARSEFULL) $(INTERPFULL) exten/specific.c exten/specific.h $(STACKSFULL) $(GENERALFULL) $(HASHFULL) $(ARRFULL)
	$(CC) unity_tests/Test_exten_interactions.c src/unity.c extension_funcs.c extension_flow.c exten/specific.c extension_arrays.c $(PARSERUN) $(INTERPRUN)  $(STACKSRUN) $(GENERALRUN) $(HASHRUN) $(ARRRUN)  -o u_exten_interactions_san $(CFLAGSSAN) $(INCEXTEN) $(LIDLIBS)

u_exten_interactions_fast: unity_tests/Test_exten_interactions.c src/unity.c src/unity.h extension_funcs.c  extension_funcs.h extension_flow.c extension_flow.h extension_arrays.c extension_arrays.h $(PARSEFULL) $(INTERPFULL) exten/specific.c exten/specific.h $(STACKSFULL) $(GENERALFULL) $(HASHFULL) $(ARRFULL)
	$(CC) unity_tests/Test_exten_interactions.c src/unity.c extension_funcs.c extension_flow.c exten/specific.c extension_arrays.c $(PARSERUN) $(INTERPRUN)  $(STACKSRUN) $(GENERALRUN) $(HASHRUN) $(ARRRUN)  -o u_exten_interactions_fast $(CFLAGSFAST) $(INCEXTEN) $(LIDLIBS)

u_exten_arr_san: unity_tests/Test_extension_arrays.c src/unity.c src/unity.h  extension_arrays.c extension_arrays.h  exten/specific.c exten/specific.h  extension_funcs.c  extension_funcs.h extension_flow.c extension_flow.h $(PARSEFULL) $(INTERPFULL) $(STACKSFULL) $(GENERALFULL) $(HASHFULL) $(ARRFULL)
	$(CC) unity_tests/Test_extension_arrays.c src/unity.c extension_arrays.c  exten/specific.c extension_funcs.c extension_flow.c  $(PARSERUN) $(INTERPRUN)  $(STACKSRUN) $(GENERALRUN) $(HASHRUN) $(ARRRUN) -o u_exten_arr_san $(CFLAGSSAN) $(INCEXTEN) $(LIDLIBS)

u_exten_arr_fast: unity_tests/Test_extension_arrays.c src/unity.c src/unity.h  extension_arrays.c extension_arrays.h  exten/specific.c exten/specific.h  extension_funcs.c  extension_funcs.h extension_flow.c extension_flow.h $(PARSEFULL) $(INTERPFULL) $(STACKSFULL) $(GENERALFULL) $(HASHFULL) $(ARRFULL)
	$(CC) unity_tests/Test_extension_arrays.c src/unity.c extension_arrays.c  exten/specific.c extension_funcs.c extension_flow.c  $(PARSERUN) $(INTERPRUN)  $(STACKSRUN) $(GENERALRUN) $(HASHRUN) $(ARRRUN) -o u_exten_arr_fast $(CFLAGSFAST) $(INCEXTEN) $(LIDLIBS)

u_debugger_san:unity_tests/Test_debugger.c src/unity.c src/unity.h debugger_funcs.c debugger_funcs.h    $(PARSEFULL) $(INTERPFULL) interpreter/specific.c interpreter/specific.h $(STACKSFULL) $(GENERALFULL) $(HASHFULL)
	$(CC) unity_tests/Test_debugger.c src/unity.c debugger_funcs.c   interpreter/specific.c  $(PARSERUN) $(INTERPRUN)  $(STACKSRUN) $(GENERALRUN) $(HASHRUN) $(ARRRUN)  -o u_debugger_san $(CFLAGSSAN) $(INCINTERP) $(LIDLIBS)

u_debugger_fast:unity_tests/Test_debugger.c src/unity.c src/unity.h debugger_funcs.c debugger_funcs.h    $(PARSEFULL) $(INTERPFULL) interpreter/specific.c interpreter/specific.h $(STACKSFULL) $(GENERALFULL) $(HASHFULL)
	$(CC) unity_tests/Test_debugger.c src/unity.c debugger_funcs.c   interpreter/specific.c  $(PARSERUN) $(INTERPRUN)  $(STACKSRUN) $(GENERALRUN) $(HASHRUN) $(ARRRUN)  -o u_debugger_fast $(CFLAGSFAST) $(INCINTERP) $(LIDLIBS)

parser_san: parser.c $(PARSEFULL) $(GENERALFULL) parse/specific.h parse/specific.c
	$(CC) parser.c $(PARSERUN) $(GENERALRUN) parse/specific.c  -o parser_san $(CFLAGSSAN) $(INCPARSE) $(LIDLIBS)

debugger:debugger.c debugger_funcs.c debugger_funcs.h  $(PARSEFULL) $(INTERPFULL) interpreter/specific.c interpreter/specific.h $(STACKSFULL) $(GENERALFULL) $(HASHFULL) ADTS_and_general/neillsdl2.c ADTS_and_general/neillsdl2.h
	$(CC) -DLIVE_VERSION debugger.c debugger_funcs.c  interpreter/specific.c  $(PARSERUN) $(INTERPRUN)  $(STACKSRUN) $(GENERALRUN) $(HASHRUN) $(ARRRUN) ADTS_and_general/neillsdl2.c -o debugger $(CFLAGSSDL) $(INCINTERP) $(LIDLIBSSDL)

parse: parser.c $(PARSEFULL) $(GENERALFULL) parse/specific.h parse/specific.c
	$(CC) parser.c $(PARSERUN) $(GENERALRUN) parse/specific.c  -o parse $(CFLAGSFAST) $(INCPARSE) $(LIDLIBS)

extension: extension.c extension_master.h extension_funcs.c  extension_funcs.h extension_flow.c extension_flow.h $(PARSEFULL) $(INTERPFULL) exten/specific.c exten/specific.h $(STACKSFULL) $(GENERALFULL) $(HASHFULL) ADTS_and_general/neillsdl2.c ADTS_and_general/neillsdl2.h extension_arrays.c extension_arrays.h $(ARRFULL)
	$(CC) -DINTERP_PRODUCTION extension.c extension_funcs.c extension_flow.c exten/specific.c extension_arrays.c $(PARSERUN) $(INTERPRUN)  $(STACKSRUN) $(GENERALRUN) $(HASHRUN) $(ARRRUN) ADTS_and_general/neillsdl2.c -o extension $(CFLAGSSDL) $(INCEXTEN) $(LIDLIBSSDL)

interp: interpreter.c interpreter_master.h $(PARSEFULL) $(INTERPFULL) interpreter/specific.c interpreter/specific.h  ADTS_and_general/neillsdl2.c ADTS_and_general/neillsdl2.h  $(STACKSFULL) $(GENERALFULL)
	$(CC) -DINTERP_PRODUCTION interpreter.c $(PARSERUN) $(INTERPRUN)  interpreter/specific.c   ADTS_and_general/neillsdl2.c $(STACKSRUN) $(GENERALRUN) -o interp $(CFLAGSSDL) $(INCINTERP) $(LIDLIBSSDL)


run_valgrind: interp extension parse debugger u_debugger_fast u_exten_arr_fast u_exten_interactions_fast u_exten_flow_fast u_exten_func_fast u_test_interp_fast u_parse_fast u_arrays_fast u_hash_map_fast
	valgrind --leak-check=full --show-leak-kinds=all ./u_debugger_fast
	valgrind --leak-check=full --show-leak-kinds=all ./u_exten_arr_fast
	valgrind --leak-check=full --show-leak-kinds=all ./u_exten_interactions_fast
	valgrind --leak-check=full --show-leak-kinds=all ./u_exten_flow_fast
	valgrind --leak-check=full --show-leak-kinds=all ./u_exten_func_fast
	valgrind --leak-check=full --show-leak-kinds=all ./u_test_interp_fast
	valgrind --leak-check=full --show-leak-kinds=all ./u_parse_fast
	valgrind --leak-check=full --show-leak-kinds=all ./u_arrays_fast
	valgrind --leak-check=full --show-leak-kinds=all ./u_hash_map_fast
	valgrind --leak-check=full --show-leak-kinds=all ./u_stacks_fast
	valgrind --leak-check=full --show-leak-kinds=all ./parse
	valgrind --gen-suppressions=all --suppressions=./linux_sdl_gl.sup --leak-check=full --show-leak-kinds=all ./interp
	valgrind --gen-suppressions=all --suppressions=./linux_sdl_gl.sup --leak-check=full --show-leak-kinds=all ./extension
	valgrind --gen-suppressions=all --suppressions=./linux_sdl_gl.sup --leak-check=full --show-leak-kinds=all ./debugger


run_tests_report: u_stacks_san u_stacks_fast u_hash_map_san u_hash_map_fast u_arrays_san u_arrays_fast u_parse_san u_parse_fast u_test_interp_san u_test_interp_fast u_exten_func_san u_exten_func_fast u_exten_flow_san u_exten_flow_fast u_exten_arr_san u_exten_arr_fast u_exten_interactions_san u_exten_interactions_fast u_debugger_san u_debugger_fast
	./u_stacks_san > testing_results.txt
	./u_stacks_fast >> testing_results.txt
	./u_hash_map_san >> testing_results.txt
	./u_hash_map_fast >> testing_results.txt
	./u_arrays_san >> testing_results.txt
	./u_arrays_fast >> testing_results.txt
	./u_parse_san >> testing_results.txt
	./u_parse_fast >> testing_results.txt
	./u_test_interp_san >> testing_results.txt
	./u_test_interp_fast >> testing_results.txt
	./u_exten_func_san >> testing_results.txt
	./u_exten_func_fast >> testing_results.txt
	./u_exten_flow_san >> testing_results.txt
	./u_exten_flow_fast >> testing_results.txt
	./u_exten_arr_san >> testing_results.txt
	./u_exten_arr_fast >> testing_results.txt
	./u_exten_interactions_san >> testing_results.txt
	./u_exten_interactions_fast >> testing_results.txt
	./u_debugger_san >> testing_results.txt
	./u_debugger_fast >> testing_results.txt


run_tests: u_stacks_san u_stacks_fast u_hash_map_san u_hash_map_fast u_arrays_san u_arrays_fast u_parse_san u_parse_fast u_test_interp_san u_test_interp_fast u_exten_func_san u_exten_func_fast u_exten_flow_san u_exten_flow_fast u_exten_arr_san u_exten_arr_fast u_exten_interactions_san u_exten_interactions_fast u_debugger_san u_debugger_fast
		./u_stacks_san
		./u_stacks_fast
		./u_hash_map_san
		./u_hash_map_fast
		./u_arrays_san
		./u_arrays_fast
		./u_parse_san
		./u_parse_fast
		./u_test_interp_san
		./u_test_interp_fast
		./u_exten_func_san
		./u_exten_func_fast
		./u_exten_flow_san
		./u_exten_flow_fast
		./u_exten_arr_san
		./u_exten_arr_fast
		./u_exten_interactions_san
		./u_exten_interactions_fast
		./u_debugger_san
		./u_debugger_fast


#runs a series of invalid files and stores error message in p_errors.txt the | true stops the makefile from terminating after seeing an error
run_parser_err: parser
	echo "test_files/test_turtles/parser/invalid/bad_spacing.ttl" > p_errors.txt
	./parser test_files/test_turtles/parser/invalid/bad_spacing.ttl 2>> p_errors.txt | true
	echo "test_files/test_turtles/parser/invalid/invalid_command.ttl" >> p_errors.txt
	./parser test_files/test_turtles/parser/invalid/invalid_command.ttl 2>>p_errors.txt | true
	echo "test_files/test_turtles/parser/invalid/missing_bracket.ttl" >> p_errors.txt
	./parser test_files/test_turtles/parser/invalid/missing_bracket.ttl 2>> p_errors.txt |true
	echo "test_files/test_turtles/parser/invalid/invalid_do.ttl" >> p_errors.txt
	./parser test_files/test_turtles/parser/invalid/invalid_do.ttl 2>> p_errors.txt | true
	echo "test_files/test_turtles/parser/invalid/invalid_nested_loop.ttl" >> p_errors.txt
	./parser test_files/test_turtles/parser/invalid/invalid_nested_loop.ttl 2>> p_errors.txt | true
	echo "test_files/test_turtles/parser/invalid/invalid_set.ttl" >> p_errors.txt
	./parser test_files/test_turtles/parser/invalid/invalid_set.ttl 2>> p_errors.txt | true
	echo "test_files/test_turtles/parser/invalid/invalid_set_2.ttl" >> p_errors.txt
	./parser test_files/test_turtles/parser/invalid/invalid_set_2.ttl 2>> p_errors.txt | true
	echo "test_files/test_turtles/parser/invalid/text_after_main.ttl" >> p_errors.txt
	./parser test_files/test_turtles/parser/invalid/text_after_main.ttl 2>> p_errors.txt | true



run_parser_val: parser
	./parser test_files/test_turtles/parser/valid/adding_loops.ttl
	./parser test_files/test_turtles/parser/valid/more_do.ttl
	./parser test_files/test_turtles/parser/valid/nested_loops.ttl
	./parser test_files/test_turtles/parser/valid/polish_2.ttl
	./parser test_files/test_turtles/parser/valid/polish.ttl
	./parser test_files/test_turtles/parser/valid/stupid_valid_do.ttl
	./parser test_files/test_turtles/parser/valid/using_variables.ttl


run_interp_err: interp
	echo "test_files/test_turtles/interpreter/invalid/no_nums.ttl" > i_errors.txt
	./interp test_files/test_turtles/interpreter/invalid/no_nums.ttl 2>> i_errors.txt | true
	echo "test_files/test_turtles/interpreter/invalid/too_many_nums.ttl" >> i_errors.txt
	./interp test_files/test_turtles/interpreter/invalid/too_many_nums.ttl 2>> i_errors.txt | true
	echo "test_files/test_turtles/interpreter/invalid/unset_var.ttl" >> i_errors.txt
	./interp test_files/test_turtles/interpreter/invalid/unset_var.ttl 2>> i_errors.txt | true
	 echo "test_files/test_turtles/parser/invalid/invalid_nested_loop.ttl" >> i_errors.txt
	./interp test_files/test_turtles/parser/invalid/invalid_nested_loop.ttl 2>> i_errors.txt | true
	echo "test_files/test_turtles/parser/invalid/invalid_set.ttl" >> i_errors.txt
	./interp test_files/test_turtles/parser/invalid/invalid_set.ttl 2>> i_errors.txt | true

run_extension_err:extension
	echo "test_files/test_turtles/extension/invalid/no_arguments.ttl" > ext_errors.txt
	./extension test_files/test_turtles/extension/invalid/no_arguments.ttl 2>> ext_errors.txt | true
	echo "test_files/test_turtles/extension/invalid/too_many_args.ttl" >> ext_errors.txt
	./extension test_files/test_turtles/extension/invalid/too_many_args.ttl 2>> ext_errors.txt | true
	echo "test_files/test_turtles/extension/invalid/unknown_func.ttl" >> ext_errors.txt
	./extension test_files/test_turtles/extension/invalid/unknown_func.ttl 2>> ext_errors.txt | true
	echo "test_files/test_turtles/extension/invalid/same_var.ttl" >> ext_errors.txt
	./extension test_files/test_turtles/extension/invalid/same_var.ttl 2>> ext_errors.txt | true
	echo "./extension test_files/test_turtles/extension/invalid/unknown_array.ttl" >> ext_errors.txt
	./extension test_files/test_turtles/extension/invalid/unknown_array.ttl 2>> ext_errors.txt | true
	echo "./extension test_files/test_turtles/extension/invalid/invalid_array_name.ttl" >> ext_errors.txt
	./extension test_files/test_turtles/extension/invalid/invalid_array_name.ttl 2>> ext_errors.txt | true
	echo "./extension test_files/test_turtles/extension/invalid/too_big.ttl" >> ext_errors.txt
	./extension test_files/test_turtles/extension/invalid/too_big.ttl 2>> ext_errors.txt | true
	echo "./extension test_files/test_turtles/extension/invalid/negative_ind.ttl" >> ext_errors.txt
	./extension test_files/test_turtles/extension/invalid/negative_ind.ttl 2>> ext_errors.txt | true
	echo "./extension test_files/test_turtles/extension/invalid/decimal_ind.ttl" >> ext_errors.txt
	./extension test_files/test_turtles/extension/invalid/decimal_ind.ttl 2>> ext_errors.txt | true
	echo "./extension test_files/test_turtles/extension/invalid/invalid_file.ttl" >> ext_errors.txt
	./extension test_files/test_turtles/extension/invalid/invalid_file.ttl 2>> ext_errors.txt | true
	echo "./extension test_files/test_turtles/extension/invalid/wrong_format_file.ttl" >> ext_errors.txt
	./extension test_files/test_turtles/extension/invalid/wrong_format_file.ttl 2>> ext_errors.txt | true
	echo "./extension test_files/test_turtles/extension/invalid/non_existant_file.ttl" >> ext_errors.txt
	./extension test_files/test_turtles/extension/invalid/non_existant_file.ttl 2>> ext_errors.txt | true

clean:
	rm -f u_stacks_san u_stacks_fast u_hash_map_san u_hash_map_fast u_arrays_san u_arrays_fast u_parse_san u_parse_fast u_test_interp_san u_test_interp_fast u_exten_func_san u_exten_func_fast u_exten_flow_san u_exten_flow_fast u_exten_arr_san u_exten_arr_fast u_exten_interactions_san u_exten_interactions_fast u_debugger_san u_debugger_fast
