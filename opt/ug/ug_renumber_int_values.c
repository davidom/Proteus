#include "UG_LIB.h"

INT_ ug_renumber_int_values
 (INT_ n,
  INT_ *number_of_new_int_values,
  INT_ *max_int_value,
  INT_ *min_int_value,
  INT_1D * int_values,
  INT_1D * new_int_values,
  INT_1D ** map_ptr)

{

/*
 * Renumber a list of integer values so that the values are in either
 * consecutive and ascending order 1,2,3... or consecutive and first encountered
 * order and save a map of the new values.
 *
 * UG LIB : Unstructured Grid - General Purpose Routine Library
 * $Id: ug_renumber_int_values.c,v 1.8 2012/08/25 18:54:13 marcum Exp $
 * Copyright 1994-2012, David L. Marcum
 */

  INT_1D *map = NULL;

  INT_ i, ierr, j, order_flag;

  // set order flag

  order_flag = (*number_of_new_int_values == -1) ? 0: 1;

  *number_of_new_int_values = 0;

  *max_int_value = 0;
  *min_int_value = n;

  for (i = 1; i <= n; i++)
  {
    *max_int_value = MAX (*max_int_value, int_values[i]);
    *min_int_value = MIN (*min_int_value, int_values[i]);
  }

  ierr = 0;

  *map_ptr = (INT_1D *) ug_malloc (&ierr,
                                   ((*max_int_value)-(*min_int_value)+1)
                                 * (INT_) sizeof (INT_1D));

  if (ierr > 0)
    return (100410);

  map = *map_ptr;

  for (j = 0; j <= (*max_int_value)-(*min_int_value); j++)
  {
    map[j] = 0;
  }

  // remap the values so that they are consecutive and ascending order 1,2,3...

  if (order_flag)
  {
    for (i = 1; i <= n; i++)
    {
      j = int_values[i] - (*min_int_value);

      if (! map[j]) map[j] = 1;
    }

    for (j = 0; j <= (*max_int_value)-(*min_int_value); j++)
    {
      if (map[j]) map[j] = ++(*number_of_new_int_values);
    }
  }

  // remap the values so that they are consecutive and first encountered order

  else
  {
    for (i = 1; i <= n; i++)
    {
      j = int_values[i] - (*min_int_value);

      if (! map[j]) map[j] = ++(*number_of_new_int_values);
    }
  }

  // set new int values

  if (new_int_values)
  {
    for (i = 1; i <= n; i++)
    {
      new_int_values[i] = int_values[i];
    }

    for (i = 1; i <= n; i++)
    {
      j = new_int_values[i] - (*min_int_value);

      new_int_values[i] = map[j];
    }
  }

  return (0);
}
