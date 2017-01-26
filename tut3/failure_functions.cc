#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>

/*
 * Border Array, Morris-Pratt function and Knutt-Morris-Pratt function construction.
 * Complexity: O(n).
 *
 * Usage: ./ba_extended [STRING]
 * Author: Jakub Kulik
 */

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

inline int* mp_function(int* ba, int size) {
  int i;
  size += 1;
  int *mp = (int*) malloc (size * sizeof(int));

  mp[0] = 0;
  for (i = 1; i < size; i++)
    mp[i] = ba[i] + 1;
  return mp;
}

inline int* kmp_function(char* pattern, int* ba, int size) {
  int i, k;
  size += 1;
  int *kmp = (int*) malloc (size * sizeof(int));

  kmp[0] = 0;
  for (i = 1; i < size; i++) {
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

inline void print_results(int* ba, int* mp, int* kmp, char* str, int size) {
  int i;

  printf("Length:  %2d\nString:  ", size);
  for (i = 0; i < size; i++)
    printf("%2c ", str[i]);

  printf("\nBorder:  ");
  for (i = 1; i <= size; i++)
    printf("%2d ", ba[i]);

  printf("\nMP:      ");
  for (i = 0; i <= size; i++)
    printf("%2d ", mp[i]);

  printf("\nKMP:     ");
  for (i = 0; i <= size; i++)
    printf("%2d ", kmp[i]);
  printf("\n");  
}

int main(int argc, char **argv) {
  int size, *ba, *mp, *kmp;

  if (argc < 2) return EXIT_FAILURE;

  size = strlen(argv[1]);

  ba = border_array(argv[1], size);
  mp = mp_function(ba, size);
  kmp = kmp_function(argv[1], ba, size);
  print_results(ba, mp, kmp, argv[1], size);

  free(ba);
  free(mp);
  free(kmp);

  return EXIT_SUCCESS;
}
