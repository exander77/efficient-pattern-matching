#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "distance.h"

inline size_t imin3(size_t a, size_t b, size_t c, uchar *index) {
  if (a < b && a < c) return a;
  ++*index;
  if (b < c) return b;
  ++*index;
  return c;
}

inline char printicize(char c) {
  return c<0x20 || c>0x7e ? '.' : c;
}

static const char directions[] = "0|-\\056=";
static const char alignments[] = "0IDR=56=";

// TODO merge match to a single node

int levensthein(char* ts, char* ps, char* te, char* pe, cell_t **table, size_t *w, size_t *h, uchar** alignment) {
  // skip matching symbols from left and from right
  int matchleft, matchright;
  for (matchleft  = 0; ts<te && ps<pe && *ps     == *ts    ; ++matchleft , ++ps, ++ts);
  for (matchright = 0; ts<te && ps<pe && *(pe-1) == *(te-1); ++matchright, --pe, --te);

  // calculate sizes and allocate table and backtrace array which is null terminated
  int n = *w = te-ts+1, m = *h = pe-ps+1, size = n*m, alignment_size = n+m+matchleft+matchright-1;
  cell_t *cell = *table = (cell_t *)malloc(sizeof(cell_t)*size);
  *alignment = (uchar *)malloc(sizeof(uchar)*alignment_size);
  uchar *alignment_end = *alignment + alignment_size - 1; *alignment_end = END;

  // calculate levensthein distance including backtrace information
  for (int i=0; i<n; ++i, ++cell) { cell->source = DELETE; cell->value = i; }    // wrong for cell 0
  cell_t *left = cell - 1, *up = cell - n, *upleft = up - 1;                     // setup source cells
  up->source = END;                                                              // fix cell 0
  for (int i=n, row=0, col; i<size; ++i, ++cell, ++left, ++up, ++upleft) {
    cell->source = (col=i%n) && ts[col-1] == ps[row-1] ? INSERT | MATCH : INSERT;
    cell->value = col ? imin3(up->value+1, left->value+1, upleft->value + (cell->source < MATCH), &cell->source) : ++row;
  }

  // get distance from last cell of the table
  int result = left->value;

  // include skipped matchright symbols
  for (int i=0;i<matchright;++i) *(--alignment_end) = MATCH;
  // extract backtrace array from backtrace information
  while (left->source) { // until we reach cell 0
    int source = *(--alignment_end) = left->source;
    left -= source >> 1 & 1;
    left -= (source & 1) * n;
  }
  // include skipped matchleft symbols
  for (int i=0;i<matchleft;++i) *(--alignment_end) = MATCH;
  memmove(*alignment, alignment_end, alignment_size-(alignment_end-*alignment));

  // return distance
  return result;
}

void print_dptable(char* ts, char* ps, cell_t* cell, size_t w, size_t h) {
  printf("Table:\n@%7c", ' ');
  for (int i=0; i<w-1; ++i) printf("%7c", printicize(ts[i]));
  for (int i=0; i<w*h; ++i, ++cell) {
    int j = i/w-1;
    if (!(i%w)) printf("\n%c ", j < 0 ? ' ' : printicize(ps[j]));
    printf("%c %4d ", directions[cell->source], cell->value);
  }
  printf("\n");
}

void print_alignment(char* ts, char* ps, uchar* alignment) {
  printf("Alignment:\n");
  for (uchar *a = alignment; *a; ++a) printf("%c", *a == INSERT ? ' ' : printicize(*(ts++)));
  printf("\n");
  for (uchar *a = alignment; *a; ++a) printf("%c", alignments[*a]);
  printf("\n");
  for (uchar *a = alignment; *a; ++a) printf("%c", *a == DELETE ? ' ' : printicize(*(ps++)));
  printf("\n");
}

// vim: ts=2 fdm=marker syntax=cpp expandtab sw=2
