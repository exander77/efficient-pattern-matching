#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define SYMBOL_AMOUNT 256

/*
 * Boyer-Moore-Sunday string matching algorithm (quick search).
 * Complexity: O(nm).
 * Backward matching algorithm.
 *
 * Usage: ./bms [STRING] [PATTERN]
 * Author: Radomír Polách, Jakub Kulik
 */

char fillers[1024];
char spaces[1024];

int* calc_table(char* pattern, int size) {
  int i;
  int *table = (int*) malloc (SYMBOL_AMOUNT * sizeof(int));

  for (i = 0; i < SYMBOL_AMOUNT; i++)
    table[i] = size + 1;
  for (i = 0; i < size; i++)
    table[pattern[i]] = size - i;

  return table;
}

inline void print_table(int* bcs, int size) {
  int i;
  
  printf("\nIndex:  ");
  for (i = 0; i < SYMBOL_AMOUNT; i++)
    if (bcs[i] != size + 1)
      printf(" %3d", i);

  printf(" ???");

  printf("\nBCS:     ");
  for (i = 0; i < SYMBOL_AMOUNT; i++)
    if (bcs[i] != size + 1)
      printf("%3d ", bcs[i]);
  
  printf("%3d ", size + 1);

  printf("\n");  
}

inline void boyer_moore_sunday(char* string, char* pattern, int size, int psize) {
  int i, j, *table;
  
  printf("Preprocessing:\n");
  table = calc_table(pattern, psize);
  print_table(table, psize);

  printf("\nProcessing:\n");
  printf("%s\n", string);
  for (i = psize - 1; i < size; i += i+1 < size ? table[string[i+1]] : 1) {
    j = 0;
    while (string[i-j] == pattern[psize-1-j]) {
      if ((++j) == psize) {
        break;
      }
    }
    if (j == psize) {
      printf("%*.s%.*s%.*s", i-psize+1, spaces, psize-j, fillers, j, pattern+psize-j);
      printf("%*c  match on position %d\n", size-i, ' ', i-j+1);
    } else {
      printf("%*.s%.*s%.*s", i-psize+1, spaces, psize-j-1, fillers, j+1, pattern+psize-j-1);
      printf("\n");
    }
  }
  free(table);
}

int main(int argc, char **argv) {
  int size, psize;

  if (argc < 3) return EXIT_FAILURE;
  memset(fillers, (int)'.', 1024);
  memset(spaces, (int)' ', 1024);

  size = strlen(argv[1]);
  psize = strlen(argv[2]);
  boyer_moore_sunday(argv[1], argv[2], size, psize);

  return EXIT_SUCCESS;
}
