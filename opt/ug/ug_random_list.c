#include "UG_LIB.h"

void ug_random_list
 (INT_ n,
  INT_ imax,
  INT_1D *random_list)

{

/*
 * Create a random list of numbers;
 * 
 * UG LIB : Unstructured Grid - General Purpose Routine Library
 * $Id: ug_random_list.c,v 1.8 2012/08/25 18:54:13 marcum Exp $
 * Copyright 1994-2012, David L. Marcum
 */

  INT_ i, seed;

  seed = time (0);

  ug_srandom (seed);

  for (i = 1; i <= n; ++i)
  {
    random_list[i] = (INT_) (ug_random () % ((long int) imax));
  }

  return;
}
