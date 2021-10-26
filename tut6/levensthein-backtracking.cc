#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "distance.h"
#include "distance.c"

int main (int argc, char **argv) {
  if (argc < 3) return EXIT_FAILURE;

  char* t = argv[1];
  char* p = argv[2];
  size_t n = strlen(t);
  size_t m = strlen(p);

  printf("Length: %3lu\n", n);
  printf("String:");
  for (int i=0;i<n;++i) {
    printf("   %c", t[i]);
  }
  printf("\n");

  printf("Length: %3lu\n", m);
  printf("String:");
  for (int i=0;i<m;++i) {
    printf("   %c", p[i]);
  }
  printf("\n");

  cell_t *table; uchar *alignment, *a; size_t w, h;
  int r = levensthein(t, p, t+n, p+m, &table, &w, &h, &alignment);
  printf("Distance: %d\n", r);
  print_dptable(t, p, table, w, h);
  print_alignment(t, p, alignment);

  return EXIT_SUCCESS;
}
// vim: ts=2 fdm=marker syntax=cpp expandtab sw=2
