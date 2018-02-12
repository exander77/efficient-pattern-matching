#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

/*
 * Morris-Pratt string matching algorithm.
 * Complexity: O(n).
 * Forward matching algorithm.
 *
 * Usage: ./morris_pratt [STRING] [PATTERN]
 * Author: Radomír Polách, Jakub Kulik
 */

char fillers[1024];

inline int* border_array(char* str, int size) {
  int i, b, *ba;

  ba = (int*) malloc (size * sizeof(int));
  ba[0] = 0;

  for (i = 1; i < size; i++) {
    b = ba[i-1];
    while (b > 0 && str[i] != str[b]) 
      b = ba[b-1];
    ba[i] = ((str[i] == str[b]) ? b + 1 : 0);
  }
  return ba;
}

inline int* mp_function(int* ba, int size) {
  int i;
  int *mp = (int*) malloc ((size+1) * sizeof(int));

  mp[0] = 0;
  for (i = 1; i <= size; i++)
    mp[i] = ba[i-1] + 1;
  return mp;
}

inline void morris_pratt(int* mp, char* string, char* pattern, int size, int psize) {
  int i, j;

  printf("%s\n", string);
  for (i = 0, j = 0; i < size; i++) {
    //printf("Comparing string[%d] with pattern[%d].\n", i, j);
    if (string[i] == pattern[j]) {
      printf("%c", pattern[j]);
      if (++j == psize) 
        printf("%*c  match on position %d", size-i, ' ', i-j+1);
    } else {
      i -= (bool)(mp[j]);
      printf("%c\n", pattern[j]);
      j = mp[j] ? mp[j]-1: 0;
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
  for (i = 0; i < size; i++)
    printf("%2d ", ba[i]);

  printf("\nMP:      ");
  for (i = 0; i <= size; i++)
    printf("%2d ", kmp[i]);
  printf("\n");  
}

int main(int argc, char **argv) {
  int psize, size;
  int *ba, *mp;

  if (argc < 3) return EXIT_FAILURE;
  memset(fillers, (int)'?', 1024);

  size = strlen(argv[1]);
  psize = strlen(argv[2]);

  ba = border_array(argv[2], psize);
  mp = mp_function(ba, psize);
  printf("Preprocessing:\n");
  print_table(ba, mp, argv[2], psize);
  printf("\nProcessing:\n");

  morris_pratt(mp, argv[1], argv[2], size, psize);

  free(ba);
  free(mp);

  return EXIT_SUCCESS;
}
