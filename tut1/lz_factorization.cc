#include <stdlib.h>
#include <string.h>
#include <stdio.h>

/*
 * LZ factorization.
 * Complexity: naive.
 *
 * Usage: ./lz_factorization [STRING]
 */

inline int* lz_factorization(char* x, int size) {
  int i, j, c, l, k, *lzd;

  lzd = (int*) malloc ((size+1) * sizeof(int));
  lzd[0] = i = c = 0;

  while (i < size) {
    l = 1;
    for (j = 0; j < i; j++) {
      for (k = 0; x[i+k] == x[j+k]; k++);
      if (k>l) l=k;
    }
    lzd[c+1] = lzd[c] += l;
    c++;
    i+=l;
  }
  return lzd;
}

inline void print_results(int* lzd, char* str, int size) {
  int i, j;

  printf("Length: %d\n", size);
  printf("String: %s\n", str);
  printf("LZ-fac: ");
  for (i = 0, j = 0; i < size; i++) {
    printf("%c", str[i]);
    if (i == (lzd[j] - 1)) {
      printf("|");
      j++;
    }
  }
  printf("\n");
}

int main(int argc, char **argv) {
  int size, *lzd;

  if (argc < 2) return EXIT_FAILURE;

  size = strlen(argv[1]);
  lzd = lz_factorization(argv[1], size);
  print_results(lzd, argv[1], size);

  free(lzd);
  return EXIT_SUCCESS;
}

// vim: ts=2 fdm=marker syntax=cpp expandtab sw=2
