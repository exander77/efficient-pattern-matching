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
 * Author: Jakub Kulik
 */

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

inline void morris_pratt(int* kmp, char* string, char* pattern, int size, int psize) {
  int i, j;

  for (i = 0, j = 0; i < size; i++) {
    if (string[i] == pattern[j]) {
      if ((++j) == psize) 
        printf("%d\n", i-j+1);
    } else {
      i -= (bool)(kmp[j]);
      j = (kmp[j]) ? kmp[j]-1: 0;
    }
  }
}


int main(int argc, char **argv) {
  int psize, size;
  int *ba, *mp;

  if (argc < 3) return EXIT_FAILURE;

  size = strlen(argv[1]);
  psize = strlen(argv[2]);

  ba = border_array(argv[2], psize);
  mp = mp_function(ba, psize);

  morris_pratt(mp, argv[1], argv[2], size, psize);

  free(ba);
  free(mp);
  return EXIT_SUCCESS;
}
