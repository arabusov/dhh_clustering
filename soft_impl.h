#pragma once
#include "defines.h"
#define DECLARE_PRINT_MAP(name, type) \
void print_ ## name ## _map (const type map[ROWS][COLS])\
{\
  int i, j;\
  for (i=0; i< ROWS; i++)\
  {\
    for (j=0; j<COLS;j++)\
      printf ("%01X", (unsigned int)map[i][j]);\
    printf ("\n");\
  }\
}


void fill_neighbors (int i, int j, unsigned int fill);
void pre_process ();
void process ();
ssize_t fill_ampl_map (int fd);
void init ();
