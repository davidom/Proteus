#include "UG_LIB.h"

int ug_sort_list_test
 (const void *,
  const void *);

INT_ ug_sort_int_list
 (INT_ m,
  INT_ n,
  INT_1D * int_values,
  INT_1D * list)

{

/*
 * Sort a list of integer values and create a corresponding index.
 * 
 * UG LIB : Unstructured Grid - General Purpose Routine Library
 * $Id: ug_sort_int_list.c,v 1.6 2012/08/25 18:54:13 marcum Exp $
 * Copyright 1994-2012, David L. Marcum
 */

  INT_2D *map = NULL;

  INT_ i;

  INT_ ierr = 0;

  map = (INT_2D *) ug_malloc (&ierr, n * sizeof (INT_2D));

  if (ierr > 0)
    return (100400);

  for (i = 0; i < n; ++i)
  {
    map[i][0] = i+1;
    map[i][1] = int_values[i+1];
  }

  qsort (map, (size_t) n, sizeof (INT_2D), ug_sort_list_test);

  for (i = 1; i <= n; ++i)
  {
    list[i] = map[i-1][0];
  }

  if (m == 1)
  {
    for (i = 1; i <= n; ++i)
    {
      int_values[i] = map[i-1][1];
    }
  }

  ug_free (map);

  return (0);
}

int ug_sort_list_test
 (const void *a_ptr,
  const void *b_ptr)

{
  INT_2D * a;
  INT_2D * b;

  a = (INT_2D *) a_ptr;
  b = (INT_2D *) b_ptr;

  if (a[0][1] < b[0][1])
    return (-1);

  else if (a[0][1] > b[0][1])
    return (1);

  else
    return (0);
}
