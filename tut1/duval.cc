#include <iostream>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

/*
 * Duval algorithm for Lyndon decomposition.
 * Splits string into longest possible Lyndon words from left to right.
 * Complexity: O(n).
 */

int* duval(char* x, int n) {
  int* ld = (int *)malloc(sizeof(int)*n);
  int i, j, k=0, l=0;
  while (k < n) {
    i=k; j=k+1;
    while(true) {
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

int main (int argc, char *argv[]) {
  if (argc < 2) return 1;

  int n = strlen(argv[1]);
  printf("Length: %u\n", n);
  printf("String: %s\n", argv[1]);

  int* ld = duval(argv[1], n);
  printf("Lyndon: ");
  int i=0,j=0;
  while (i < n) {
    printf("%c", argv[1][i++]);
    if (i==ld[j]) {
      printf(".");
      ++j;
    }
  }
  printf("\n");
  free(ld);

  return 0;
}

// vim: ts=2 fdm=marker syntax=cpp expandtab sw=2
