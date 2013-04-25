#include "UG_LIB.h"

INT_ ug_find_file_in_path_list
 (const char *name,
  const char *path_list,
  char *path)

{

/*
 * Find given file in a directory included in a given path list.
 *
 * UG LIB : Unstructured Grid - General Purpose Routine Library
 * $Id: ug_find_file_in_path_list.c,v 1.5 2012/08/25 18:54:13 marcum Exp $
 * Copyright 1994-2012, David L. Marcum
 */

  CHAR_256 *items = NULL;

  INT_ err, i, n;

  if (path_list == NULL)
    return (-1);

  err = ug_get_items_from_list (path_list, UG_PATH_DIR_SEP, &items, &n);

  if (err)
  {
    ug_free (items);
    return (-1);
  }

  i = 0;

  do
  {
    strcpy (path, items[i]);
    strcat (path, UG_PATH_SEP);
    strcat (path, name);

    err = ug_check_file_isreg (path);

    ++i;
  }
  while (i < n && err);

  ug_free (items);
  
  return (err);

}
