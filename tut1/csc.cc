#include <stdlib.h>
#include <string.h>
#include <stdio.h>

/*
 * Cyclical string comparison.
 * Complexity: O(n).
 *
 * Usage: ./csc [STRING]
 */

int csc(char* x, int n, char* y, int m) {
   int i, j, k, last = 0;

   i = j = -1;
   while (i<n-1 && j<m-1) {
      k = 1;
      while (k<=n && x[(i+k)%n]==y[(j+k)%m]) k++;
      if (k>n) return n > m ? 1 : n < m ? -1 : 0;

      last=x[(i+k)%n] > y[(j+k)%m];
      if (last) i+=k;
      else j+=k;
   }
   return last ? 1 : -1;
}

int main (int argc, char *argv[]) {
   int m, n, r;

   if (argc < 3) return EXIT_FAILURE;

   n = strlen(argv[1]);
   m = strlen(argv[2]);
   r = csc(argv[1], n, argv[2], m);

   printf("Length A: %u\n", n);
   printf("Length B: %u\n", m);
   printf("String A: %s\n", argv[1]);
   printf("String B: %s\n", argv[2]);
   printf("  Result: %d\n", r);

   return EXIT_SUCCESS;
}

// vim: ts=2 fdm=marker syntax=cpp expandtab sw=2
