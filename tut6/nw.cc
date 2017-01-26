#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#define min(x,y)      (((x) < (y)) ? (x) : (y))
#define max(x,y)      (((x) > (y)) ? (x) : (y))
#define max3(x,y,z) (((x) > max(y,z)) ? (x) : max(y,z))

typedef struct {
  char* alphabet;
  int** sub;
  int c;
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
  table->sub = (int**)malloc(sizeof(int*)*n);
  for (int i=0;i<n;++i) {
    table->sub[i] = (int*)malloc(sizeof(int)*n);
    for (int j=0;j<n;++j) {
      fscanf(f, "%d", &table->sub[i][j]);
    }
  }
  printf("S");
  for (int i=0;i<n;++i) {
    printf("%4c", table->alphabet[i]);
  }
  printf("\n");
  for (int i=0;i<n;++i) {
    printf("%c ", table->alphabet[i]);
    for (int j=0;j<n;++j) {
      printf("%3d ", table->sub[i][j]);
    }
    printf("\n");
  }
  fscanf(f, "%d", &table->c);
  printf("c = %d\n", table->c);
  fclose(f);
  return 0;
}

int unload(table_t* table) {
  free(table->alphabet);
  free(table->sub);
}

int nw(char* t, size_t n, char* p, size_t m, table_t* table) {
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
        d[i] = table->c*max(i,j);
      } else {
        char tc = strchr(table->alphabet, t[i-1])-table->alphabet;
        char pc = strchr(table->alphabet, p[j-1])-table->alphabet;
        int rep = table->sub[pc][tc];
        d[i] = max3(d[i-1]+table->c, d[i]+table->c, prev + rep);
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


  int r = nw(t, n, p, m, &table);
  printf("Distance: %d\n", r);

  return EXIT_SUCCESS;
}
// vim: ts=2 fdm=marker syntax=cpp expandtab sw=2
