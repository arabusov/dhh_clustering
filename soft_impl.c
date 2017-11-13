#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdio.h>
#include "defines.h"
#include "maps.h"
unsigned int cluster_map [ROWS][COLS];
unsigned char ampl_map [ROWS][COLS];
#include "packer.h"


struct cluster
 clu_table [ROWS*COLS];//actually, this is "pre_table"

struct final_cluster
 final_clu_table[ROWS*COLS];
unsigned int final_clu_table_size=0;
unsigned int event_size =0;

void fill_neighbors (int i, int j, unsigned int fill)
{
  if ((i < 0) || (j<0) || (i>=ROWS) || (j>=COLS))
    return;
  if (j<COLS-1)
  {
    if (ampl_map[i][j+1] != 0)
    {
      if (cluster_map[i][j+1] != 0)
        clu_table[fill].id=cluster_map[i][j+1];
      cluster_map [i][j+1] = fill;
    }
    if (i < ROWS -1)
      if (ampl_map[i+1][j+1] != 0)
      {
        if (cluster_map[i+1][j+1] != 0)
          clu_table[fill].id = cluster_map [i+1][j+1];
        cluster_map [i+1][j+1] = fill;
      }
  }
  if (i < ROWS -1)
    if (ampl_map[i+1][j] != 0)
    {
      if (cluster_map[i+1][j] !=0)
        clu_table[fill].id=cluster_map[i+1][j];
      cluster_map [i+1][j] = fill;
    }
}

void pre_process ()
{
  int i, j;
  unsigned int cluster_counter=1;
  clu_table[0].id=0;
  for (i=0; i<ROWS; i++)
    for (j=0; j<COLS; j++)
      if (ampl_map[i][j] != 0)
      {
        if (cluster_map[i][j] != 0)
        {
          fill_neighbors (i,j, cluster_map[i][j]);
        }
        else
        {
          cluster_map[i][j] = cluster_counter;
          clu_table [cluster_counter].id = cluster_counter;
          clu_table[0].id++;
          fill_neighbors (i,j, cluster_counter);
          cluster_counter++;
        }
      }
}

void process ()
{
  for (int i=1; i<clu_table[0].id+1; i++)
  {
    clu_table[i].id = clu_table[clu_table[i].id].id;
  }
  for (int i=0; i<ROWS; i++)
    for (int j=0; j<COLS; j++)
      if (cluster_map [i][j] != 0)
      {
        cluster_map[i][j] = clu_table[cluster_map[i][j]].id;
        if (final_clu_table[cluster_map[i][j]].first_row == -1)
        {
          final_clu_table[cluster_map[i][j]].first_row = i;
          final_clu_table[cluster_map[i][j]].prev_row = i;
          final_clu_table[cluster_map[i][j]].size += 4;
          final_clu_table_size ++;
        }
        else if ((i-final_clu_table[cluster_map[i][j]].prev_row)>=2)
        {
          final_clu_table[cluster_map[i][j]].size += 4;
          final_clu_table[cluster_map[i][j]].prev_row = i;
        }
        else
        {
          final_clu_table[cluster_map[i][j]].size += 2;
        }
      }
  for (int i=1;i<final_clu_table_size+1; i++)
    if ((final_clu_table[i].size %4 )== 2)
    {
      final_clu_table[i].size += 2;
    }

}

ssize_t fill_ampl_map (int fd)
{
  return read (fd, ampl_map, ROWS*COLS);
}

void init ()
{
  event_size =0;
  final_clu_table_size =0;
  for (unsigned int i=0; i<COLS*ROWS;i++)
  {
    final_clu_table[i].size = 0;
    final_clu_table[i].first_row= -1;
    final_clu_table[i].prev_row= -1;

    clu_table[i].id = 0;
  }
}

