#include <assert.h>
#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define ROW_OF(i) ((i) / 9)
#define COL_OF(i) ((i) % 9)
#define REG_OF(i) (((i) / 27) * 3) + (((i) % 9) / 3)

#define ROW_INDEX(r, i) ((r) * 9 + (i))
#define COL_INDEX(c, i) ((i) * 9 + (c))
#define REG_INDEX(r, i)                                                        \
  (((((r) / 3) * 27) + (((r) % 3) * 3)) + ((((i) / 3) * 9) + ((i) % 3)))

void print_board(int *b) {
  for (int i = 0; i < 81; ++i) {
    putchar(b[i] ? b[i] + '0' : '.');
    if (i % 9 == 8) {
      putchar('\n');
    }
  }
}

int *alloc_board(void) { return calloc(81, sizeof(int)); }

bool is_valid(int *b, int index, int val) {
  if (val == 0) {
    return true;
  }
  for (int i = 0; i < 9; ++i) {
    int row_index = ROW_INDEX(ROW_OF(index), i);
    int col_index = COL_INDEX(COL_OF(index), i);
    int reg_index = REG_INDEX(REG_OF(index), i);
    if ((b[row_index] == val && row_index != index) ||
        (b[col_index] == val && col_index != index) ||
        (b[reg_index] == val && reg_index != index)) {
      return false;
    }
  }
  return true;
}

bool validate(int *b) {
  // NOTE: There is certainly a faster version of this function
  for (int i = 0; i < 81; ++i) {
    if (!is_valid(b, i, b[i])) {
      return false;
    }
  }
  return true;
}

int *solve(int *b, int i) {
  // advance index if the cell is already filled
  while (b[i] != 0) {
    ++i;
  }

  // return the current state if we've reached the end
  if (i >= 81) {
    int *nb = alloc_board();
    memmove(nb, b, 81 * sizeof(int));
    return nb;
  }

  // attempt all valid possibilties for the next cell
  int *res = NULL;
  for (int v = 1; v <= 9; ++v) {
    if (is_valid(b, i, v)) {
      int nb[81];
      memmove(nb, b, 81 * sizeof(int));
      nb[i] = v;
      res = solve(nb, i + 1);
      if (res) {
        return res;
      }
    }
  }

  // all options exhausted
  return NULL;
}

int main(int argc, char **argv) {
  FILE *ifile = (argc > 1) ? fopen(argv[1], "r") : stdin;
  if (!ifile) {
    perror(NULL);
    return 1;
  }

  int b[81];

  for (int i = 0, c = fgetc(ifile); i < 81; c = fgetc(ifile)) {
    if (c == EOF) {
      fputs("not enough characters", stderr);
      return 1;
    }

    if (isdigit(c)) {
      b[i] = c - '0';
      ++i;
    } else if (c == ' ' || c == '0') {
      b[i] = 0;
      ++i;
    }
  }

  if (!validate(b)) {
    fputs("invalid input provided\n", stderr);
    return 1;
  }

  int *res = solve(b, 0);
  assert(res);

  if (!validate(res)) {
    fputs("invalid result\n", stderr);
  }
  print_board(res);
  return 0;
}
