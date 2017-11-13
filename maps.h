#pragma once
#include "defines.h"
extern unsigned int cluster_map [ROWS][COLS];
extern unsigned char ampl_map [ROWS][COLS];


struct cluster
{
  unsigned int id;
}; 
extern struct cluster clu_table [ROWS*COLS];//actually, this is "pre_table"

struct final_cluster
{
  unsigned int size;
  int first_row;
  int prev_row;
};
extern struct final_cluster final_clu_table[ROWS*COLS];
extern unsigned int final_clu_table_size;
extern unsigned int event_size;

