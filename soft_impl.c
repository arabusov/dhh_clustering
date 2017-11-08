#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdio.h>
#include "defines.h"

unsigned int cluster_map [ROWS][COLS];
unsigned char ampl_map [ROWS][COLS];

#define DECLARE_PRINT_MAP(name, type) \
void print_ ## name ## _map (const type map[ROWS][COLS])\
{\
  int i, j;\
  for (i=0; i< ROWS; i++)\
  {\
    for (j=0; j<COLS;j++)\
      printf ("%02X", (unsigned int)map[i][j]);\
    printf ("\n");\
  }\
}

DECLARE_PRINT_MAP (int, unsigned int);
DECLARE_PRINT_MAP (char, unsigned char);

void fill_neighbors (int i, int j, unsigned int fill)
{
  if ((i < 0) || (j<0) || (i>=ROWS-1) || (j>=COLS-1))
    return;
  if (ampl_map[i][j+1] != 0)
    cluster_map [i][j+1] = fill;
  if (ampl_map[i+1][j+1] != 0)
    cluster_map [i+1][j+1] = fill;
  if (ampl_map[i+1][j] != 0)
    cluster_map [i+1][j] = fill;
}

void process ()
{
  int i, j;
  unsigned int cluster_counter=1;
  for (i=0; i<ROWS-1; i++)
    for (j=0; j<COLS-1; j++)
      if (ampl_map[i][j] != 0)
      {
        if (cluster_map[i][j] != 0)
        {
          fill_neighbors (i,j, cluster_map[i][j]);
        }
        else
        {
          cluster_map[i][j] = cluster_counter;
          fill_neighbors (i,j, cluster_counter);
          cluster_counter++;
        }
      }
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
  printf ("Input:\n");
  printf("%d\n",fill_ampl_map (fd));
  print_char_map (ampl_map);
  process();
  printf ("Output:\n");
  print_int_map (cluster_map);
  return 0;
}

