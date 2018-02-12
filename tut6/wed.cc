#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#define min(x,y)      (((x) < (y)) ? (x) : (y))
#define max(x,y)      (((x) > (y)) ? (x) : (y))
#define min3(x,y,z) (((x) < min(y,z)) ? (x) : min(y,z))

/*
 * Weighted Edit Distance
 * Complexity: O(nm).
 *
 * Usage: ./wed text1 text2 table_file
 * Author: Radomír Polách
 *
 * table_file format:
 * 4
 * a g c t
 * 0 1 2 3
 * 2 0 5 2
 * 2 5 0 0
 * 3 2 0 0
 * 7 10 8 9
 * 10 7 9 8
 */

typedef struct {
  char* alphabet;
  int** rep;
  int* ins;
  int* del;
} table_t;

int load(table_t* table, char* name) {
  FILE *f = fopen(name, "r");
  if (!f) return -1;
  int n;
  fscanf(f, "%d", &n);
  table->alphabet = (char*)malloc(sizeof(char)*(n+1)); 
  table->alphabet[n] = '\0';
  for (int i=0;i<n;++i) {
    fscanf(f, "%s", &table->alphabet[i]);
  }
  printf("Alphabet: %s\n", table->alphabet);
  table->rep = (int**)malloc(sizeof(int*)*n);
  for (int i=0;i<n;++i) {
    table->rep[i] = (int*)malloc(sizeof(int)*n);
    for (int j=0;j<n;++j) {
      fscanf(f, "%d", &table->rep[i][j]);
    }
  }
  printf("R");
  for (int i=0;i<n;++i) {
    printf("%4c", table->alphabet[i]);
  }
  printf("\n");
  for (int i=0;i<n;++i) {
    printf("%c ", table->alphabet[i]);
    for (int j=0;j<n;++j) {
      printf("%3d ", table->rep[i][j]);
    }
    printf("\n");
  }
  table->ins = (int*)malloc(sizeof(int)*n);
  printf("I ");
  for (int i=0;i<n;++i) {
    fscanf(f, "%d", &table->ins[i]);
    printf("%3d ", table->ins[i]);
  }
  printf("\n");
  table->del = (int*)malloc(sizeof(int)*n);
  printf("D ");
  for (int i=0;i<n;++i) {
    fscanf(f, "%d", &table->del[i]);
    printf("%3d ", table->del[i]);
  }
  printf("\n");
  fclose(f);
  return 0;
}

int unload(table_t* table) {
  free(table->alphabet);
  free(table->rep);
  free(table->ins);
  free(table->del);
}

int wed(char* t, size_t n, char* p, size_t m, table_t* table) {
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
      if (min(i,j) == 0) {
        if (max(i,j) == 0) {
          d[i] = 0;
        } else {
          char tc = strchr(table->alphabet, t[i-1])-table->alphabet;
          char pc = strchr(table->alphabet, p[j-1])-table->alphabet;
          if (i == 0) {
            d[i] = table->ins[pc]+d[i];
          } else if (j == 0) {
            d[i] = table->del[tc]+d[i-1];
          }
        }
      } else {
        char tc = strchr(table->alphabet, t[i-1])-table->alphabet;
        char pc = strchr(table->alphabet, p[j-1])-table->alphabet;
        int ins = table->ins[pc];
        int del = table->del[tc];
        int rep = table->rep[pc][tc];
        d[i] = min3(d[i-1]+del, d[i]+ins, prev + (t[i-1] == p[j-1] ? 0 : rep));
      }
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
  if (argc < 4) return EXIT_FAILURE;

  char* t = argv[1];
  char* p = argv[2];
  char* name = argv[3];
  size_t n = strlen(t);
  size_t m = strlen(p);
  table_t table;
  if (load(&table, name) != 0) {
    return EXIT_FAILURE;
  }

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


  int r = wed(t, n, p, m, &table);
  printf("Distance: %d\n", r);

  return EXIT_SUCCESS;
}
// vim: ts=2 fdm=marker syntax=cpp expandtab sw=2
