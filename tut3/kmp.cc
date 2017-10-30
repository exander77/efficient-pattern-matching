#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

/*
 * Knutt-Morris-Pratt string matching algorithm.
 * Complexity: O(n).
 * Forward matching algorithm.
 *
 * Usage: ./knutt_morris_pratt [STRING] [PATTERN]
 * Author: Radomír Polách, Jakub Kulik
 */

char fillers[1024];

inline int* border_array(char* str, int size) {
  int i, b, *ba;

  ba = (int*) malloc ((size+1) * sizeof(int));

  ba[0] = -1;
  ba[1] = 0;

  for (i = 1; i < size; i++) {
    b = ba[i];
    while (b > 0 && str[i] != str[b]) 
      b = ba[b];
    ba[i+1] = ((str[i] == str[b]) ? b + 1 : 0);
  }
  return ba;
}

inline int* kmp_function(char* pattern, int* ba, int size) {
  int i, k;
  int *kmp = (int*) malloc ((size+1) * sizeof(int));

  kmp[0] = 0;
  for (i = 1; i <= size; i++) {
    k = ba[i];
    while (k >= 0) {
      if (pattern[k] != pattern[i])
        break;
      else
        k = ba[k];
    }
    kmp[i] = k + 1;
  }
  return kmp;
}

inline void knutt_morris_pratt(int* kmp, char* string, char* pattern, int size, int psize) {
  int i, j;

  printf("%s\n", string);
  for (i = 0, j = 0; i < size; i++) {
    //printf("Comparing string[%d] with pattern[%d].\n", i, j);
    if (string[i] == pattern[j]) {
      printf("%c", pattern[j]);
      if (++j == psize) 
        printf("%*c  match on position %d", size-i, ' ', i-j+1);
    } else {
      i -= (bool)(kmp[j]);
      printf("%c\n", pattern[j]);
      j = (kmp[j]) ? kmp[j]-1: 0;
      printf("%*c%.*s", i+1-j, ' ', j, fillers);
    }
  }
  printf("\n");
}

inline void print_table(int* ba, int* kmp, char* str, int size) {
  int i;
  
  printf("\nIndex:  ");
  for (i = 0; i < size; i++)
    printf(" %2d", i);

  printf("\nString:  ");
  for (i = 0; i < size; i++)
    printf("%2c ", str[i]);

  printf("\nBorder:  ");
  for (i = 1; i <= size; i++)
    printf("%2d ", ba[i]);

  printf("\nKMP:     ");
  for (i = 0; i <= size; i++)
    printf("%2d ", kmp[i]);
  printf("\n");  
}

int main(int argc, char **argv) {
  int *ba, *kmp;
  int psize, size;

  if (argc < 3) return EXIT_FAILURE;
  memset(fillers, (int)'?', 1024);

  size = strlen(argv[1]);
  psize = strlen(argv[2]);

  ba = border_array(argv[2], psize);
  kmp = kmp_function(argv[2], ba, psize);
  printf("Preprocessing:\n");
  print_table(ba, kmp, argv[2], psize);
  printf("\nProcessing:\n");

  knutt_morris_pratt(kmp, argv[1], argv[2], size, psize);

  free(ba);
  free(kmp);

  return EXIT_SUCCESS;
}
