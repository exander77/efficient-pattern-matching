#ifndef DISTANCE_H
#define DISTANCE_H

#define END     0x0
#define INSERT  0x1
#define DELETE  0x2
#define REPLACE (INSERT+DELETE)
#define MATCH   0x4

typedef unsigned int uint;
typedef unsigned char uchar;

typedef struct {
    uint value;
    uchar source;
} cell_t;

int levensthein(char* ts, char* ps, char* te, char* pe, cell_t **table, size_t *w, size_t *h, uchar** alignment);

void print_dptable(char* ts, char* ps, cell_t* cell, size_t w, size_t h);
void print_alignment(char* ts, char* ps, uchar* alignment);

#endif

// vim: ts=2 fdm=marker syntax=cpp expandtab sw=2
