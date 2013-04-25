#include "UG_LIB.h"

INT_ ug_get_double_array
 (char Label[],
  INT_ dim,
  INT_ ilabel,
  INT_ mclose,
  INT_ *n,
  double ** data_ptr)

{

/*
 * Get double data from tmp file.
 *
 * UG LIB : Unstructured Grid - General Purpose Routine Library
 * $Id: ug_get_double_array.c,v 1.5 2013/03/13 04:05:32 marcum Exp $
 * Copyright 1994-2012, David L. Marcum
 */

  CHAR_UG_MAX File_Name;

  double *data = NULL;

  INT_ ierr;

  if (ilabel >= 0)
    sprintf (File_Name, "%s_Data_%i", Label, (int) ilabel);
  else
    sprintf (File_Name, "%s_Data", Label);

  *data_ptr = NULL;

  *n = 0;

  ierr = ug_get_double_data (File_Name, dim, n, &data[1]);

  if (ierr != 0)
    return (0);

  *data_ptr = (double *) ug_malloc (&ierr, (*n+1) * dim * sizeof (double));

  if (ierr > 0)
    return (100404);

  data = *data_ptr;

  ierr = ug_get_double_data (File_Name, dim, n, &data[1]);

  if (ierr > 0)
    return (ierr);

  if (mclose == 1)
    ierr = ug_close_data (File_Name);
  else
    ierr = ug_close_dataf (File_Name);

  return (0);

}
