#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*
 * Naive string matching.
 * Complexity: O(n*m).
 * Forward matching algorithm.
 *
 * Usage: ./naive_matching [STRING] [PATTERN]
 * Author: Jakub Kulik
 */

int main(int argc, char **argv) {
  int i, size, psize;

  if (argc < 3) return EXIT_FAILURE;

  size = strlen(argv[1]);
  psize = strlen(argv[2]);

  for (i = 0; i <= size - psize; i++)
    if (! strncmp(argv[1] + i, argv[2], psize))
      printf("%d\n", i);

  return EXIT_SUCCESS;
}
