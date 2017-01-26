#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define SYMBOL_AMOUNT 26 // amount of symbols (starting from a to biggest one used)

/*
 * Boyer-Moore-Sunday string matching algorithm (quick search).
 * Complexity: O(nm).
 * Backward matching algorithm.
 *
 * Usage: ./bms [STRING] [PATTERN]
 * Author: Jakub Kulik
 */

int* calc_table(char* pattern, int size) {
  int i;
  int *table = (int*) malloc (SYMBOL_AMOUNT * sizeof(int));

  for (i = 0; i < SYMBOL_AMOUNT; i++)
    table[i] = size+1;
  for (i = 0; i < size; i++)
    table[pattern[i] - 'a'] = size-i;

  return table;
}

inline void boyer_moore_sunday(char* string, char* pattern, int size, int psize) {
  int i, j, *table;
  table = calc_table(pattern, psize);

  i = psize - 1;
  while (i < size) {
    j = 0;
    while (string[i-j] == pattern[psize-1-j]) {
      if ((++j) == psize) {
        printf("%d\n", i-j+1);
        break;
      }
    }
    i += ((i+1) < size) ? table[string[i+1] - 'a'] : 1;
  }
  free(table);
}

int main(int argc, char **argv) {
  int size, psize;

  if (argc < 3) return EXIT_FAILURE;

  size = strlen(argv[1]);
  psize = strlen(argv[2]);
  boyer_moore_sunday(argv[1], argv[2], size, psize);

  return EXIT_SUCCESS;
}
