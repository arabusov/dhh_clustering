#include <stdio.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include "defines.h"
#include "maps.h"
#include "soft_impl.h"

DECLARE_PRINT_MAP (int, unsigned int);
DECLARE_PRINT_MAP (char, unsigned char);

int main (int argc, char**argv)
{
  int fd;
  if (argc != 2)
    return -1;
  fd = open (argv[1], O_RDONLY);
  if (fd == -1)
    return -2;
  init ();
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
    printf ("prev_row[%d] = %d\n", i, final_clu_table[i].prev_row);
  }
  return 0;
}
