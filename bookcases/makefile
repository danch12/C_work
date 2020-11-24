CFLAGSFAST= -Wall -Wextra -Wfloat-equal -pedantic -std=c90 -O3

CFLAGSSAN= -Wall -Wextra -Wfloat-equal -pedantic -std=c90 -fsanitize=undefined -fsanitize=address -g3
LIDLIBS = -lm
CC = gcc 

all: basic_fast basic_san extension_fast extension_san

basic_fast: bookcase.c common_bk.c common_bk.h
	$(CC) bookcase.c common_bk.c  -o bookcase_fast $(CFLAGSFAST) $(LIDLIBS)

basic_san: bookcase.c common_bk.c common_bk.h
	$(CC) bookcase.c common_bk.c -o bookcase_san $(CFLAGSSAN) $(LIDLIBS)

extension_fast: extension.c common_bk.c common_bk.h
	$(CC) extension.c common_bk.c -o extension_fast $(CFLAGSFAST) $(LIDLIBS)

extension_san: extension.c common_bk.c common_bk.h
	$(CC) extension.c common_bk.c  -o extension_san $(CFLAGSSAN) $(LIDLIBS)

rm:
	rm -f  bookcase_fast bookcase_san extension_fast extension_san

run: basic_fast
	./bookcase_fast $(ARGS) 
