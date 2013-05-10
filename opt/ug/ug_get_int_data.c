#include "UG_LIB.h"

INT_ ug_get_int_data
 (char File_Name[],
  INT_ dim,
  INT_ *n,
  INT_ *data)

{

/*
 * Incrementally get integer array data from a tmp file.
 *  
 * UG LIB : Unstructured Grid - General Purpose Routine Library
 * $Id: ug_get_int_data.c,v 1.14 2013/03/16 19:11:06 marcum Exp $
 */

  CHAR_31 Name_Text;
  CHAR_133 Text;

  FILE *Data_File = NULL;

  INT_ iread, mread;

  if (ug_file_status_monitor_flag())
  {
    strcpy (Name_Text, "");
    strncat (Name_Text, File_Name, 29);
    sprintf (Text, "ug_get_int_data          : reading data from TMP file %s",
             Name_Text);
    ug_message (Text);
  }

  ug_set_byte_order (ug_get_byte_order ());

  Data_File = ug_fopen (File_Name, "r_tmp");

  if (Data_File == NULL)
    return (401);

  mread = (*n == 0) ? 1:
          (*n <  0) ? 2: 3;

  iread = 0;

  if (mread <= 2)
  {
    iread = iread + ug_fread (n, sizeof (INT_), 1, Data_File);

    iread = iread - 1;
  }

  if (mread >= 2 && *n > 0)
  {
    iread = iread + ug_fread (data, sizeof (INT_), dim*(*n), Data_File);

    iread = iread - dim*(*n);
  }

  if (iread < 0)
  {
    ug_close_data (File_Name);
    return (401);
  }

  if (ug_file_status_monitor_flag())
  {
    sprintf (Text, "ug_get_int_data          : read %i items of %i-dim data", (int) *n, (int) dim);
    ug_message (Text);
  }

  if (*n == 0)
  {
    ug_rewind (Data_File);
    return (-1);
  }

  return (0);

}
