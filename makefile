#CFLAGS=-Wall -Wextra -pedantic -ansi -O2 -lm -Wfloat-equal
CFLAGS=-Wall -Wextra -Wfloat-equal -pedantic -std=c90 -O2 -lm -fsanitize=address -fsanitize=undefined -g3
CC=/usr/local/bin/gcc-10
