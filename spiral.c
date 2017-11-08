#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include "defines.h"


unsigned char map [ROWS][COLS];
unsigned char color (unsigned int i, unsigned int j, unsigned int vector)
{
  return vector+4; //why not?
}
//vector: 0 -- right, 1 -- down, 2 -- left, 3 -- up

unsigned int nextv (unsigned int vector)
{
  return (vector+1)%4;
}
void spiral (unsigned int i, unsigned int j, unsigned int vector);
void rotate (unsigned int i, unsigned int j, unsigned int vector)
{
  switch (vector)
  {
    case 0:
      i = i+1;
      break;
    case 1:
      j = j-1;
      break;
    case 2:
      i = i-1;
      break;
    case 3:
      j = j+1;
      break;
  }
  vector++;
  vector%=4;
  spiral (i, j, vector);
}

void next (unsigned int *i, unsigned int *j, unsigned int vector, unsigned int
delta)
{
  switch (vector)
  {
    case 0:
      *j = *j+delta;
      break;
    case 1:
      *i = *i+delta;
      break;
    case 2:
      *j = *j-delta;
      break;
    case 3:
      *i = *i-delta;
      break;
  }
}

unsigned int prevv (unsigned int vector)
{
  return (vector-1)%4;
}
  
void spiral (unsigned int i, unsigned int j, unsigned int vector)
{
  unsigned int ni=i, nj=j, nvector=vector;
  //exit conditions
  if (i >= ROWS)
    return;
  if (j >= COLS)
    return;
  if (map [i][j] != 0)
    return;
  if (vector >= 4)
    return;
  map[i][j] = color (i, j, vector);
  //check border
  switch (vector)
  {
    case 0:
      if (j==COLS-1)
      {
        spiral (i+1,j,nextv(vector));
        return;
      }
      else if (j==COLS-2)
        if (map[i][j+1]!=0)
          return;
        else
        {
          spiral (i,j+1,vector);
          return;
        }
      break;
    case 1:
      if (i==ROWS-1)
      {
        spiral (i,j-1,nextv(vector));
        return;
      }
      else if (i==ROWS-2)
        if (map[i+1][j]!=0)
          return;
        else
        {
          spiral (i+1,j,vector);
          return;
        }
      break;
    case 2:
      if (j==0)
      {
        spiral (i-1,j,nextv(vector));
        return;
      }
      else if (j==1)
        if (map[i][j-1]!=0)
          return;
        else
        {
          spiral (i,j-1,vector);
          return;
        }
      break;
    case 3:
      if (i==0)
      {
        spiral (i,j+1,nextv(vector));
        return;
      }
      else if (i==1)
        if (map[i-1][j]!=0)
          return;
        else
        {
          spiral (i-1,j,vector);
          return;
        }
      break;
  }
  next (&ni,&nj, vector,2);
  if (map[ni][nj] != 0)
  {
    rotate (i,j,vector);
  }
  else
  {
    next (&i, &j, vector, 1);
    spiral (i, j, vector);
  }
}

void fill_map ()
{
  int i, j;
  //fill zero
  for (i=0; i<ROWS; i++)
    for (j=0; j<COLS; j++)
    {
      map[i][j]=0;
    }
  //fill only spiral
  spiral (0,0,0);
}

int main (int argc, char**argv)
{
  int fd;
  if (argc != 2)
    return -1;
  fd = creat (argv[1], 00664);
  if (fd == -1)
  {
    printf ("No such file or dir: %s\n", argv[1]);
    return -1;
  }
  fill_map();
  write (fd, map, ROWS*COLS);
}
