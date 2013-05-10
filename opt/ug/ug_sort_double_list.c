#include "UG_LIB.h"

INT_ ug_sort_double_list
 (INT_ m,
  INT_ n,
  DOUBLE_1D * double_values,
  INT_1D * list)

{

/*
 * Sort a list of double values and create a corresponding index.
 * 
 * UG LIB : Unstructured Grid - General Purpose Routine Library
 * $Id: ug_sort_double_list.c,v 1.6 2012/08/25 18:54:13 marcum Exp $
 * Copyright 1994-2012, David L. Marcum
 */

  INT_1D *int_values = NULL;

  INT_ i, j;

  INT_ ierr = 0;
  INT_ ivmax = 1000000000;

  DOUBLE_1D *double_values0 = NULL;

  double v, vmax, vmin, w;

  int_values = (INT_1D *) ug_malloc (&ierr, n * sizeof (INT_1D));

  if (ierr > 0)
    return (100401);

  vmax = double_values[1];
  vmin = double_values[1];

  for (i = 2; i <= n; ++i)
  {
    vmax = MAX (vmax, double_values[i]);
    vmin = MIN (vmin, double_values[i]);
  }

  w = ((double) ivmax) / (vmax - vmin);

  for (i = 0; i < n; ++i)
  {
    v = (double_values[i+1] - vmin) * w;

    int_values[i] = NINT (v);
  }

  ierr = ug_sort_int_list (0, n, int_values-1, list);

  ug_free (int_values);

  if (ierr > 0)
    return (ierr);

  if (m == 1)
  {
    double_values0 = (DOUBLE_1D *) ug_malloc (&ierr, n * sizeof (DOUBLE_1D));

    if (ierr > 0)
      return (100402);

    for (i = 0; i < n; ++i)
    {
      double_values0[i] = double_values[i+1];
    }

    for (i = 1; i <= n; ++i)
    {
      j = list[i];

      double_values[i] = double_values0[j-1];
    }

    ug_free (double_values0);
  }

  return (0);
}
