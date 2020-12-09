#CFLAGS=-Wall -Wextra -pedantic -ansi -O2 -lm -Wfloat-equal
CFLAGS=-Wall -Wextra -Wfloat-equal -pedantic -Wpedantic -std=c90 -O2 -lm -fsanitize=address -fsanitize=undefined -g3
CC=/usr/local/bin/gcc-10

bool: testboolarr.c  boolean_arrays.c general.c boolarr.h general.h
	$(CC) testboolarr.c  boolean_arrays.c general.c -o boolean_arrays $(CFLAGS)
