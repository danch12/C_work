As the folder is quite big thought it would be handy to signpost where things are.

ADTS are in the ADTS_and_general folder with the testing for the ADTS in unity_tests

Parser:

Parser funcs are in parser_funcs.c and parser/specific.c
Parser main is in parser.c
Parser testing is in unity_tests/Test_parser.c
Turtle files for the parser are in test_files/test_turtles/parser


Interpreter:
As well as using the parser functions in parser_funcs.c

Interpreter funcs are in interpreter_funcs.c and interpreter/specific.c
Interpreter main is interpreter.c
Interpreter testing is in unity_tests/Test_interp.c
Turtle files for the Interpreter are in test_files/test_turtles/Interpreter



Extension grammar:
As well as using the parser functions in parser_funcs.c and the interpreter funcs in interpreter_funcs.c

Functions used across extension is in exten/specific.c

Implementing functions is in extension_funcs.c with testing in unity_tests/Test_extension_funcs.c
Implementing control flow is in extension_flow.c unity_tests/Test_flow.c
Implementing arrays is in extension_arrays.c unity_tests/Test_extension_arrays.c


Test turtle files for all three sections are in test_files/test_turtles/extension



Extension debugger:

As well as using the parser functions in parser_funcs.c and the interpreter funcs in interpreter_funcs.c and interpreter/specifc.c


Debugger funcs are in debugger_funcs.c
Debugger main in debugger.c
Debugger tests are in test_debugger.c

I used the interpreter and parser turtle files to test the debugger as well.



