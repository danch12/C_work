CFLAGS = `sdl2-config --cflags` -O2 -Wall -Wextra -pedantic -Wfloat-equal -std=c90
LDLIBS =  `sdl2-config --libs` -lm
CC = gcc

# Ensure sure this file is called 'Makefile' or 'makefile'
# and then type: make blocks, make clean or make run

blocks: Prime_factors_SDL.c neillsdl2.c neillsdl2.h
	$(CC) Prime_factors_SDL.c neillsdl2.c -o primes $(CFLAGS) $(LDLIBS)

clean:
	rm -f primes

run: blocks
	./primes
