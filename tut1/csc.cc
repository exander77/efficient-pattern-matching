#include <iostream>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

/*
 * Cyclical string comparison.
 * Complexity: O(n).
 */

int csc(char* x, int n, char* y, int m) {
  int i=-1, j=-1, k;
  bool last=0;
  while (i<n-1 && j<m-1) {
    k=1;
    while (k<=n && x[(i+k)%n]==y[(j+k)%m]) k++;
    if (k>n) return n > m ? 1 : n < m ? -1 : 0;
    last=x[(i+k)%n] > y[(j+k)%m];
    if (last) i+=k;
    else j+=k;
  }

  return last ? 1 : -1;
}

int main (int argc, char *argv[]) {
  if (argc < 3) return 1;

  int n = strlen(argv[1]);
  int m = strlen(argv[2]);
  printf("Length A: %u\n", n);
  printf("Length B: %u\n", m);
  printf("String A: %s\n", argv[1]);
  printf("String B: %s\n", argv[2]);
  int r = csc(argv[1], n, argv[2], m);
  printf("  Result: %d\n", r);
  return 0;
}

// vim: ts=2 fdm=marker syntax=cpp expandtab sw=2
