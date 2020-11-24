CFLAGSFAST= -Wall -Wextra -Wfloat-equal -pedantic -std=c90 -O3
CFLAGSSAN= -Wall -Wextra -Wfloat-equal -pedantic -std=c90 -fsanitize=undefined -fsanitize=address -g3
LIDLIBS = -lm
CC = gcc 
INCBASIC = -Ibasic
INCEXT = -Iextension


all: basic_fast basic_san extension_fast extension_san


test_basic_fast: basic/common_bk.h common_tests.c basic/common_bk.c
	$(CC) common_tests.c basic/common_bk.c -o basic_common_test_fast $(CFLAGSFAST) $(INCBASIC) $(LIDLIBS)

test_basic_san: basic/common_bk.h common_tests.c basic/common_bk.c
	$(CC) common_tests.c basic/common_bk.c -o basic_common_test_san $(CFLAGSSAN) $(INCBASIC) $(LIDLIBS)

test_ext_fast: extension/common_bk.h common_tests.c extension/common_bk.c
	$(CC) common_tests.c extension/common_bk.c -o ext_common_test_fast $(CFLAGSFAST) $(INCEXT) $(LIDLIBS)

test_ext_san: extension/common_bk.h common_tests.c extension/common_bk.c
	$(CC) common_tests.c extension/common_bk.c -o ext_common_test_san $(CFLAGSSAN) $(INCEXT) $(LIDLIBS)

basic_fast: basic/bookcase.c basic/common_bk.c basic/common_bk.h
	$(CC) basic/bookcase.c basic/common_bk.c  -o basic_fast $(CFLAGSFAST) $(LIDLIBS)

basic_san: basic/bookcase.c basic/common_bk.c basic/common_bk.h
	$(CC) basic/bookcase.c basic/common_bk.c -o basic_san $(CFLAGSSAN) $(LIDLIBS)

extension_fast: extension/extension.c extension/common_bk.c extension/common_bk.h
	$(CC) extension/extension.c extension/common_bk.c -o extension_fast $(CFLAGSFAST) $(LIDLIBS)

extension_san: extension/extension.c extension/common_bk.c extension/common_bk.h
	$(CC) extension/extension.c extension/common_bk.c -o extension_san $(CFLAGSSAN) $(LIDLIBS)

rm:
	rm -f  basic_fast basic_san extension_fast extension_san basic_common_test_fast basic_common_test_san ext_common_test_san ext_common_test_fast

run_common_test:test_basic_fast test_basic_san test_ext_fast test_ext_san
	./basic_common_test_fast
	./basic_common_test_san
	./ext_common_test_fast
	./ext_common_test_san

run_all: run_common_test all
	./basic_san $(ARGS) 
	./basic_fast $(ARGS)
	./extension_fast $(ARGS) 
	./extension_san $(ARGS)

run: basic_fast
	./basic_fast $(ARGS) 
