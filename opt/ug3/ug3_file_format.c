#include "UG3_LIB.h"

INT_ ug3_file_format
 (char File_Format_Suffix[])

{

/*
 * Check file format suffix and set byte order.
 * 
 * UG3 LIB : Unstructured Grid - General Purpose Routine Library
 * 3D Version : $Id: ug3_file_format.c,v 1.5 2012/08/25 19:01:45 marcum Exp $
 * Copyright 1994-2012, David L. Marcum
 */

  INT_ File_Format;

  File_Format = (strcmp (File_Format_Suffix, "") == 0) ? 1:
                (strcmp (File_Format_Suffix, ".b8") == 0) ? 2:
                (strcmp (File_Format_Suffix, ".lb8") == 0) ? -2:
                (strcmp (File_Format_Suffix, ".tmp.b8") == 0) ? 3:
                (strcmp (File_Format_Suffix, ".tmp.lb8") == 0) ? -3: 0;

  ug_set_byte_order (File_Format);

  File_Format = abs (File_Format);

  return (File_Format);

}
