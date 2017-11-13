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

DECLARE_PRINT_MAP (int, unsigned int);
DECLARE_PRINT_MAP (char, unsigned char);

struct hit
{
  uint32_t row : 10;
  uint32_t col : 6;
  uint32_t dhpt : 2;
  uint32_t ampl : 8;
};

struct cluster
{
  unsigned int id;
} clu_table [ROWS*COLS];//actually, this is "pre_table"

struct final_cluster
{
  unsigned int size;
  int first_row;
} final_clu_table[ROWS*COLS];
unsigned int final_clu_table_size=0;

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
          final_clu_table[cluster_map[i][j]].size += 4;
          final_clu_table_size ++;
        }
        else if ((i-final_clu_table[cluster_map[i][j]].first_row)%2 == 0)
        {
          final_clu_table[cluster_map[i][j]].size += 4;
        }
        else
        {
          final_clu_table[cluster_map[i][j]].size += 2;
        }
      }
  for (int i=1;i<final_clu_table_size+1; i++)
    if (final_clu_table[i].size %4 == 2)
      final_clu_table[i].size += 2;

}

ssize_t fill_ampl_map (int fd)
{
  return read (fd, ampl_map, ROWS*COLS);
}

int main (int argc, char**argv)
{
  int fd;
  if (argc != 2)
    return -1;
  fd = open (argv[1], O_RDONLY);
  if (fd == -1)
    return -2;
  for (unsigned int i=0; i<COLS*ROWS;i++)
  {
    final_clu_table[i].size = 0;
    final_clu_table[i].first_row= -1;
  }
  printf ("Input:\n");
  printf("%d\n",fill_ampl_map (fd));
  print_char_map (ampl_map);
  pre_process();
  printf ("Preprocessing:\n");
  print_int_map (cluster_map);
  printf ("Table after preprocessing:\n");
  for (int i=0; i<clu_table[0].id+1;i++)
    printf ("table[%d] = %d\n", i, clu_table[i].id);
  process ();
  printf ("Output:\n");
  print_int_map (cluster_map);
  printf ("Table after postprocessing:\n");
  for (int i=1; i<final_clu_table_size+1;i++)
  {
    printf ("clu_size[%d] = %d\n", i, final_clu_table[i].size);
    printf ("first_row[%d] = %d\n", i, final_clu_table[i].first_row);
  }
  return 0;
}
