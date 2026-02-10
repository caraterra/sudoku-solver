CFLAGS=-Wall -Wextra -Wpedantic -std=c99

sudoku: sudoku.c

debug: CFLAGS += -g -fsanitize=address
debug: sudoku

clean:
	rm -rf sudoku

.PHONY: clean debug
