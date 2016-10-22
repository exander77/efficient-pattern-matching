#include <iostream>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

/*
 * Lyndon rotation.
 * Finds lexicographically least rotation (conjugate) of string.
 * Complexity: O(n).
 */

int lyndon_rotation(char* x, int n) {
  int* ba = (int *)malloc(sizeof(int)*n+1);
  int i=0, j=-1, k=0;
  ba[i]=j;
  while (i<2*n) {
    if (i-k >= n) return k;
    while (j>=0 && x[i%n] != x[(k+j)%n]) {
      if (x[i%n] < x[(k+j)%n]) k = i-j;
      j = ba[j];
    }
    i++; j++;
    ba[i-k] = j;
  }
  return k;
}

int main (int argc, char *argv[]) {
  if (argc < 2) return 1;

  int n = strlen(argv[1]);
  printf("Length: %u\n", n);
  printf("String: %s\n", argv[1]);

  int k = lyndon_rotation(argv[1], n);
  printf("   Rot: %d\n", k);
  printf("Lyndon: ");
  for (int i=0;i<n;++i) {
    printf("%c", argv[1][(i+k)%n]);
  }
  printf("\n");

  return 0;
}

// vim: ts=2 fdm=marker syntax=cpp expandtab sw=2
