#include <iostream>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

/*
 * LZ factorization.
 * Complexity: naive.
 */

int* lz_factorization(char* x, int n) {
  int* lzd = (int *)malloc(sizeof(int)*n);
  lzd[0] = 0;
  int i=0, c=0;
  while (i < n) {
    int l=1; int k;
    for (int j=0; j<i; ++j) {
      for (k=0; x[i+k] == x[j+k]; ++k);
      if (k>l) l=k;
    }
    lzd[c+1] = lzd[c++] += l;
    i+=l;
  }
  return lzd;
}

int main (int argc, char *argv[]) {
  if (argc < 2) return 1;

  int n = strlen(argv[1]);
  printf("Length: %u\n", n);
  printf("String: %s\n", argv[1]);

  int* lzd = lz_factorization(argv[1], n);
  printf("LZ-fac: ");
  int i=0,j=0;
  while (i < n) {
    printf("%c", argv[1][i++]);
    if (i==lzd[j]) {
      printf(".");
      ++j;
    }
  }
  printf("\n");
  free(lzd);

  return 0;
}

// vim: ts=2 fdm=marker syntax=cpp expandtab sw=2
