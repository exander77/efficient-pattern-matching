#include <stdlib.h>
#include <string.h>
#include <stdio.h>

/*
 * Border Array construction.
 * Complexity: O(n).
 *
 * Usage: ./ba [STRING]
 */

int* borderarray(char* x, int n) {
  int i=0, j=-1;
  int* ba = (int *)malloc(sizeof(int)*(n+1));
  
  ba[i]=j;
  while (i<n)
  {
    while (j>=0 && x[i%n]!=x[j%n]) j=ba[j];
    i++; j++;
    ba[i]=j;
  }
  return ba;
}

int main (int argc, char *argv[]) {
  if (argc < 2) return EXIT_FAILURE;

  int n = strlen(argv[1]);
  printf("Length: %3u\n", n);
  printf("String:");
  for (int i=0;i<n;++i) {
    printf("   %c", argv[1][i]);
  }
  printf("\n");
  int* ba = borderarray(argv[1], n);
  printf("Border: ");
  for (int i=1;i<n+1;++i) {
    printf("%3i ", ba[i]);
  }
  printf("\n");
  free(ba);  

  return EXIT_SUCCESS;
}

// vim: ts=2 fdm=marker syntax=cpp expandtab sw=2
