#include <stdlib.h>
#include <string.h>
#include <stdio.h>

/*
 * Lyndon rotation.
 * Finds lexicographically least rotation (conjugate) of string.
 * Complexity: O(n).
 *
 * Usage: ./lyndon_rotation [STRING]
 */

inline int lyndon_rotation(char* x, int n)
{
  int* ba = (int *)malloc(sizeof(int)*2*n);
  for (int i=0; i<2*n; i++) ba[i] = -1;
  int k = 0;
  for (int j=1; j<2*n; j++)
  {
    int i = ba[j-k-1];
    while ((i != -1) && (x[j%n] != x[(k+i+1)%n]))
    {
      if (x[j%n] < x[(k+i+1)%n]) k = j-i-1;
      i = ba[i];
    }
    if ((i == -1) && (x[j%n] != x[(k+i+1)%n]))
    {
      if (x[j%n] < x [(k+i+1)%n]) k = j;
      ba[j-k] = -1;
    }
    else ba[j-k] = i+1;
  }
  free(ba);
  return k;
}

inline void print_results(char* str, int size, int k) {
  int i;

  printf("Length: %u\n", size);
  printf("String: %s\n", str);

  printf("   Rot: %d\n", k);
  printf("Lyndon: ");
  for (i = 0; i < size; i++)
    printf("%c", str[(i+k)%size]);
  printf("\n");
}

int main (int argc, char *argv[]) {
  int size, k;

  if (argc < 2) return EXIT_FAILURE;

  size = strlen(argv[1]);
  k = lyndon_rotation(argv[1], size);
  print_results(argv[1], size, k);

  return EXIT_SUCCESS;
}

// vim: ts=2 fdm=marker syntax=cpp expandtab sw=2
