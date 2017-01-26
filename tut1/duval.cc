#include <stdlib.h>
#include <string.h>
#include <stdio.h>

/*
 * Duval algorithm for Lyndon decomposition.
 * Splits string into longest possible Lyndon words from left to right.
 * Complexity: O(n).
 *
 * Usage: ./duval [STRING]
 */

inline int* duval(char* x, int n) {
  int i, j, k, l;
  int* ld = (int*) malloc (n * sizeof(int));

  k = l = 0;
  while (k < n) {
    i=k; j=k+1;
    while(1) {
      if (j<n) {
        if (x[i] < x[j]) {i=k; ++j; continue;}  
        else if (x[i] == x[j]) {++i; ++j; continue;}
      }
      do {k+=(j-i); ld[l++] = k;}
      while (k<=i);
      break;
    }
  }
  return ld;
}

inline void print_results(int* ld, char* str, int size) {
  int i, j;

  printf("Length: %u\n", size);
  printf("String: %s\n", str);
  printf("Lyndon: ");
  for (i = 0, j = 0; i < size; i++) {
    printf("%c", str[i]);
    if (i == (ld[j] - 1)) {
      printf("|");
      j++;
    }
  }
  printf("\n");
}

int main (int argc, char *argv[]) {
  int size, *ld;

  if (argc < 2) return EXIT_FAILURE;

  size = strlen(argv[1]);
  ld = duval(argv[1], size);
  print_results(ld, argv[1], size);

  free(ld);
  return EXIT_SUCCESS;
}

// vim: ts=2 fdm=marker syntax=cpp expandtab sw=2
