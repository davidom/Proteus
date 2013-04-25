#include "UG_LIB.h"

INT_ ug_save_double_array
 (char Label[],
  INT_ dim,
  INT_ ilabel,
  INT_ n,
  double * data)

{

/*
 * Save double data array in a tmp file.
 *
 * UG LIB : Unstructured Grid - General Purpose Routine Library
 * $Id: ug_save_double_array.c,v 1.6 2013/03/13 04:20:43 marcum Exp $
 * Copyright 1994-2012, David L. Marcum
 */

  CHAR_UG_MAX File_Name;

  INT_ ierr;

  if (data == NULL)
    return (0);

  if (ilabel >= 0)
    sprintf (File_Name, "%s_Data_%i", Label, (int) ilabel);
  else
    sprintf (File_Name, "%s_Data", Label);

  ierr = ug_save_double_data (File_Name, dim, n, n, &data[1]);

  if (ierr == 0)
    ierr = ug_save_double_data (File_Name, dim, 0, 0, &data[1]);

  if (ierr > 0)
    return (ierr);

  return (0);

}
