#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>

/*
 * Karp-Rabin string matching algorithm.
 * Complexity: O(n*m).
 * Forward matching algorithm.
 *
 * Usage: ./karp_rabin [STRING] [PATTERN]
 * Author: Jakub Kulik
 */

#define BASE 5
#define MODULE 8

inline int get_hash(char *pattern, int size) {
  int i, result = 0;

  for (i = 0; i < size; i++)
    result += (pattern[i] - 'a')*pow(BASE, size - i - 1);

  return result % MODULE;
}

inline int roll_hash(int hash, char prev, char next, int size) {
  return ((int)((hash - (prev-'a')*pow(BASE, size-1))*BASE + (next-'a'))%MODULE + MODULE)%MODULE;
}

int main(int argc, char **argv) {
  int i, size, psize;
  int phash, hash;

  if (argc < 3) return EXIT_FAILURE;

  size = strlen(argv[1]);
  psize = strlen(argv[2]);

  hash = get_hash(argv[1], psize);
  phash = get_hash(argv[2], psize);

  for (i = 0 ; i < size - psize + 1; i++) {
    if (hash == phash)
      if (! strncmp(argv[1] + i, argv[2], psize))
        printf("%d\n", i);

    hash = roll_hash(hash, argv[1][i], argv[1][psize+i], psize);
  }
  return EXIT_SUCCESS;
}
