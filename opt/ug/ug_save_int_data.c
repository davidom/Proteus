#include "UG_LIB.h"

INT_ ug_save_int_data
 (char File_Name[],
  INT_ dim,
  INT_ n,
  INT_ npw,
  INT_ *data)

{

/*
 * Incrementally save integer array data in a tmp file.
 *  
 * UG LIB : Unstructured Grid - General Purpose Routine Library
 * $Id: ug_save_int_data.c,v 1.21 2013/03/16 19:12:24 marcum Exp $
 * Copyright 1994-2012, David L. Marcum
 */

  CHAR_133 Text;

  FILE *Data_File = NULL;

  INT_ i1, i2, nd, ni, iwrite, loc;

  if (ug_file_status_monitor_flag())
  {
    sprintf (Text, "ug_save_int_data         : writing %i items of %i-dim data (%i per write) to TMP file %s", (int) n, (int) dim, (int) npw, File_Name);
    ug_message (Text);
  }

  ug_set_byte_order (ug_get_byte_order ());

  Data_File = ug_fopen (File_Name, "tmp");

  if (Data_File == NULL)
    return (407);

  iwrite = 0;

  i2 = 0;

  loc = 0;

  do
  {
    i1 = i2 + 1;
    i2 = i2 + npw;
    i2 = MIN (i2, n);

    ni = i2 - i1 + 1;

    iwrite = iwrite + ug_fwrite (&ni, sizeof (INT_), 1, Data_File);

    iwrite = iwrite - 1;

    nd = dim*ni;

    if (nd > 0)
      iwrite = iwrite
             + ug_fwrite (&data[loc], sizeof (INT_), nd, Data_File);

    iwrite = iwrite - nd;

    loc = loc + nd;
  }
  while (i2 < n);

  if (npw < n)
  {
    ni = 0;

    iwrite = iwrite + ug_fwrite (&ni, sizeof (INT_), 1, Data_File);

    iwrite = iwrite - 1;
  }

  if (iwrite < 0)
  {
    ug_close_data (File_Name);
    return (407);
  }

  if (ni == 0)
    ug_close_dataf (File_Name);

  return (0);

}
