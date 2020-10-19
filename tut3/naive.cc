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

inline void naive(char* string, char* pattern, int size, int psize) {
  int i, j;

  printf("%s\n", string);
  for (i = 0; i < size; i++) {
    //printf("Comparing string[%d] with pattern[%d].\n", i, j);
    for (j = 0; j < psize; j++) {
      if (string[i+j] == pattern[j]) {
        printf("%c", pattern[j]);
        if (j+1 == psize) {
	  printf("%*c  match on position %d", size-i, ' ', i);
          printf("\n%*c", i+1, ' ');
          break;
        }
      } else {
        printf("%c\n", pattern[j]);
        printf("%*c", i+1, ' ');
        break;
      }
    }
  }
  printf("\n");
}

int main(int argc, char **argv) {
  int i, size, psize;

  if (argc < 3) return EXIT_FAILURE;

  size = strlen(argv[1]);
  psize = strlen(argv[2]);

  naive(argv[1], argv[2], size, psize);

  return EXIT_SUCCESS;
}
