#include "UG_LIB.h"

INT_ ug_get_items_from_list
 (const char *list,
  const char *sep,
  CHAR_256 **items,
  INT_ *n)

{

/*
 * Get vector of items in a list.
 * 
 * UG LIB : Unstructured Grid - General Purpose Routine Library
 * $Id: ug_get_items_from_list.c,v 1.5 2012/08/25 18:54:13 marcum Exp $
 * Copyright 1994-2012, David L. Marcum
 */

  char *str_ptr;

  CHAR_UG_MAX str;

  INT_ err, i, length, max_length, j;

  if (! strstr (list, sep))
    return (-1);

  for (j = 1; j <= 2; ++j)
  {
    strcpy (str, list);

    i = 0;

    max_length = 0;

    do
    {
      length = (INT_) strcspn (str, sep);

      if (length)
      {
        max_length = MAX (max_length, length);

        if (max_length >= 256)
          return (414);

        if (j == 2)
        {
          strcpy ((*items)[i], "");

          strncat ((*items)[i], str, (int) length);
        }

        ++i;
      }

      str_ptr = strstr (str, sep);

      if (str_ptr)
      {
        ++str_ptr;

        length = (INT_) strlen (str_ptr);

        if (length)
          strcpy (str, str_ptr);
        else
          str_ptr = NULL;
      }
      else
        str_ptr = NULL;
    }
    while (str_ptr);

    *n = i;

    if (j == 1)
    {
      err = 0;

      *items = (CHAR_256 *) ug_malloc (&err, *n * sizeof (CHAR_256));

      if (err)
        return (100408);
    }
  }

  return (0);

}
