#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define SYMBOL_AMOUNT 256

#define MAX(a,b) ((a) > (b) ? a : b)

/*
 * Boyer-Moore string matching algorithm.
 * Complexity: O(nm).
 * Backward matching algorithm.
 *
 * Usage: ./bmh [STRING] [PATTERN]
 * Author: Radomír Polách, Jakub Kulik
 */

char fillers[1024];
char spaces[1024];

//BCS
int* calc_table(char* pattern, int size) {
  int i;
  int *table = (int*) malloc (SYMBOL_AMOUNT * sizeof(int));

  for (i = 0; i < SYMBOL_AMOUNT; i++)
    table[i] = size;
  for (i = 0; i < (size-1); i++)
    table[pattern[i]] = size - i - 1;

  return table;
}

int is_prefix(char *pattern, int size, int pos) {
	int suffixlen = size - pos;

	for (int i=0; i < suffixlen; i++)
		if (pattern[i] != pattern[pos+i]) return 0;
	return 1;
}

int suffix_length(char *pattern, int size, int pos) {
	int i;
	// increment suffix length i to the first mismatch or beginning
	// of the pattern
	for (i = 0; (pattern[pos-i] == pattern[size-1-i]) && (i < pos); i++);
	return i;
}

//GSS
int* calc_gss(char* pattern, int size) {
	int i;
  int *table = (int*) malloc (SYMBOL_AMOUNT * sizeof(int));
	int last_prefix_index = 1;

	// first loop, prefix patern
	for (i=size-1; i>=0; i--) {
			if (is_prefix(pattern, size, i+1)) last_prefix_index = i+1;
			table[i] = (size-1 - i) + last_prefix_index;
	}

	// this is overly cautious, but will not produce anything wrong
	// second loop, suffix pattern
	for (i=0; i < size-1; i++) {
		int slen = suffix_length(pattern, size, i);
		if (pattern[i - slen] != pattern[size-1 - slen]) table[size-1 - slen] = size-1 - i + slen;
	}

	return table;
}

inline void print_table(int* bcs, int size) {
  int i;
  
  printf("\nIndex:  ");
  for (i = 0; i < SYMBOL_AMOUNT; i++)
    if (bcs[i] != size)
      printf(" %3d", i);

  printf(" ???");

  printf("\nBCS:     ");
  for (i = 0; i < SYMBOL_AMOUNT; i++)
    if (bcs[i] != size)
      printf("%3d ", bcs[i]);
  
  printf("%3d ", size);

  printf("\n");  
}

inline void print_gss(int* gss, int size) {
  int i;
  
  printf("\nIndex:  ");
  for (i = 0; i < size; i++)
    printf(" %3d", i);

  printf("\nGSS:     ");
  for (i = 0; i < size; i++)
    printf("%3d ", gss[i]);
  
  printf("\n");  
}

inline void boyer_moore_horspool(char* string, char* pattern, int size, int psize) {
  int i, j, *table, *table2;
  
  printf("Preprocessing:\n");
  table = calc_table(pattern, psize);
  print_table(table, psize);
  table2 = calc_gss(pattern, psize);
  print_gss(table2, psize);

  printf("\nProcessing:\n");
  printf("%s\n", string);
  for (i = psize - 1; i < size; i += MAX(table2[j], table[string[i]])) {
    j = 0;
    while (string[i-j] == pattern[psize-1-j]) {
      if ((++j) == psize) {
        break;
      }
    }
    if (j == psize) {
      printf("%*.s%.*s%.*s", i-psize+1, spaces, psize-j, fillers, j, pattern+psize-j);
      printf("%*c  match on position %d\n", size-i, ' ', i-j+1);
    } else {
      printf("%*.s%.*s%.*s", i-psize+1, spaces, psize-j-1, fillers, j+1, pattern+psize-j-1);
      printf("\n");
    }
  }
  free(table);
}

int main(int argc, char **argv) {
  int size, psize;

  if (argc < 3) return EXIT_FAILURE;
  memset(fillers, (int)'.', 1024);
  memset(spaces, (int)' ', 1024);

  size = strlen(argv[1]);
  psize = strlen(argv[2]);
  boyer_moore_horspool(argv[1], argv[2], size, psize);

  return EXIT_SUCCESS;
}
