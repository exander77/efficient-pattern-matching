#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#define min(x,y)      (((x) < (y)) ? (x) : (y))
#define max(x,y)      (((x) > (y)) ? (x) : (y))
#define min3(x,y,z) (((x) < min(y,z)) ? (x) : min(y,z))

/*
 * Levensthein Distance
 * Complexity: O(nm).
 *
 * Usage: ./levensthein text1 text2
 * Author: Radomír Polách
 */

int levensthein(char* t, size_t n, char* p, size_t m) {
  if (n == 0) return m;
  if (m == 0) return n;

  int* d = (int *)malloc(sizeof(int)*(n+1));
  printf("    -");
  for (int i=0;i<n+1;++i) { //rows
    printf(" %3c", t[i]);
  }
  printf("\n");
  for (int j=0;j<m+1;++j) { //rows
    int prev;
    if (j==0) printf("-");
    else printf("%c", p[j-1]);
    for (int i=0;i<n+1;++i) { //column
      int tmp = d[i];
      if (min(i,j) == 0) d[i] = max(i,j); 
      else d[i] = min3(d[i-1]+1, d[i]+1, prev + (t[i-1] == p[j-1] ? 0 : 1));
      printf(" %3i", d[i]);
      prev = tmp;
    }
    printf("\n");
  }

  int r = d[n];
  free(d);
  return r;
}


int main (int argc, char *argv[]) {
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


  int r = levensthein(t, n, p, m);
  printf("Distance: %d\n", r);

  return EXIT_SUCCESS;
}
// vim: ts=2 fdm=marker syntax=cpp expandtab sw=2
